#include "gdk_achievements.h"

#include "gdk_achievement.h"
#include "gdk_asyncblock.h"
#include "gdk_helpers.h"
#include "gdk_user.h"
#include "godot_gdk.h"
#include "../godot-cpp/include/godot_cpp/core/memory.hpp"
#include "../godot-cpp/include/godot_cpp/core/print_string.hpp"
#include "../godot-cpp/include/godot_cpp/templates/a_hash_map.hpp"
#include "../godot-cpp/include/godot_cpp/variant/typed_array.hpp"

#include <Windows.h>
#include <winapifamily.h>
#include <objbase.h>
#include <XGameRuntimeInit.h>
#include <XTaskQueue.h>
#include <xsapi-c/services_c.h>
#include <XUser.h>
#include <XGame.h>

#include <iomanip>

void GDKAchievements::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_achievements", "user", "achievementType", "unlockedOnly", "achievementOrder"), &GDKAchievements::get_achievements);
	ClassDB::bind_method(D_METHOD("get_achievement", "user", "achievementId"), &GDKAchievements::get_achievement);
	ClassDB::bind_method(D_METHOD("unlock_achievement", "user", "achievementId"), &GDKAchievements::unlock_achievement);
	ClassDB::bind_method(D_METHOD("set_achievement_percentage", "user", "achievementId", "percentage"), &GDKAchievements::set_achievement_percentage);
}

Ref<GDKAsyncBlock> GDKAchievements::get_achievements(Ref<GDKUser> user, GDKXblAchievementType::Enum achievementType, bool unlockedOnly, GDKXblAchievementOrderBy::Enum achievementOrder) {
	XTaskQueueHandle queue = GDKHelpers::get_async_queue();
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(queue);

	asyncBlock->set_callback(get_achievements_callback);
	asyncBlock->extra_data["achievements"] = TypedArray<GDKAchievement>();

	XblContextHandle handle;

	if (!GodotGDK::CreateContextHandle(&handle)) {
		return asyncBlock;
	}

	uint32_t titleID;
	if (!GodotGDK::CheckResult(!XGameGetXboxTitleId(&titleID), "Successfully retrieved titleID", "Failed to retrieve titleID")) {
		return asyncBlock;
	}

	HRESULT hr = XblAchievementsGetAchievementsForTitleIdAsync(handle, user->get_id(), titleID, static_cast<XblAchievementType>(achievementType), unlockedOnly, static_cast<XblAchievementOrderBy>(achievementOrder), 0, 0, asyncBlock->get_block());

	GodotGDK::CheckResult(hr, "Successfully started retrieving achievements", "Failed to start retrieving achievements");

	return asyncBlock;
}

void GDKAchievements::get_achievements_callback(XAsyncBlock* asyncBlock) {
    GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(asyncBlock->context);
	TypedArray<GDKAchievement> achievementArray = wrapper->extra_data["achievements"];

	XblAchievementsResultHandle resultHandle;
	HRESULT hr = XblAchievementsGetAchievementsForTitleIdResult(asyncBlock, &resultHandle);

	if (!GodotGDK::CheckResult(hr, "", "Failed to retrieve achievements")) {
		return;
	}

	const XblAchievement *achievements;
	size_t size;
	hr = XblAchievementsResultGetAchievements(resultHandle, &achievements, &size);

	if (!GodotGDK::CheckResult(hr, "", "Failed to retrieve achievements")) {
		return;
	}

	for (int i = 0; i < size; i++) {
		achievementArray.push_back(memnew(GDKAchievement(&achievements[i])));
	}

	bool hasMoreAchievements = false;

	//GDK can only retrieve 32 achievements at once. Check if there is more, and retrieve until we have all.
	XblAchievementsResultHasNext(resultHandle, &hasMoreAchievements);

	if (hasMoreAchievements) {
		XblAchievementsResultGetNextAsync(resultHandle, 0, asyncBlock);
	} else {
		print_line("Call callback");
		wrapper->emit(wrapper->extra_data);
	}

	XblAchievementsResultCloseHandle(resultHandle);
}


Ref<GDKAsyncBlock> GDKAchievements::get_achievement(Ref<GDKUser> user, String achievementId) {
	XTaskQueueHandle queue = GDKHelpers::get_async_queue();
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(queue);

	asyncBlock->set_callback([](XAsyncBlock* async) {
		GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);

		XblAchievementsResultHandle resultHandle;
		Dictionary return_data;
		HRESULT hr = XblAchievementsGetAchievementResult(async, &resultHandle);

		if(hr == 0) {
			const XblAchievement *achievements;
			size_t size;
			hr = XblAchievementsResultGetAchievements(resultHandle, &achievements, &size);

			if(hr == 0) {
				return_data["achievement"] = memnew(GDKAchievement(&achievements[0]));
			}
		}

		return_data["hresult"] = hr;
		wrapper->emit(return_data);
		XblAchievementsResultCloseHandle(resultHandle);
	});

	XblContextHandle handle;

	if (!GodotGDK::CreateContextHandle(&handle)) {
		return asyncBlock;
	}

	HRESULT hr = XblAchievementsGetAchievementAsync(handle, user->get_id(), GodotGDK::GetSCID(), achievementId.utf8(), asyncBlock->get_block());

	GodotGDK::CheckResult(hr, "Successfully started retrieving achievements", "Failed to start retrieving achievements");

	return asyncBlock;
}

Ref<GDKAsyncBlock> GDKAchievements::unlock_achievement(Ref<GDKUser> user, String achievementId) {
	return internal_set_achievement_percentage(user, achievementId, 100, "Successfully unlocked achievement", "Failed to unlock achievement");
}


Ref<GDKAsyncBlock> GDKAchievements::set_achievement_percentage(Ref<GDKUser> user, String achievementId, uint32_t percentage) {
	return internal_set_achievement_percentage(user, achievementId, percentage, "Successfully set achievement progress", "Failed to set achievement progress");
}

Ref<GDKAsyncBlock> GDKAchievements::internal_set_achievement_percentage(Ref<GDKUser> user, String achievementId, uint32_t percentage, const String &successMessage, const String &failMessage) {
	XTaskQueueHandle queue = GDKHelpers::get_async_queue();
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(queue);

	XblContextHandle handle;

	if (!GodotGDK::CreateContextHandle(&handle)) {
		return asyncBlock;
	}

	asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);

		HRESULT hr = XAsyncGetStatus(async, false);
		Dictionary return_data;
		return_data["hresult"] = hr;
		wrapper->emit(return_data);
	});

	HRESULT result = XblAchievementsUpdateAchievementAsync(handle, user->get_id(), achievementId.utf8(), percentage, asyncBlock->get_block());

	GodotGDK::CheckResult(result, successMessage, failMessage);
	return asyncBlock;
}
