#include "gdk_achievements.h"

#include "gdk_achievement.h"
#include "gdk_asyncblock.h"
#include "gdk_helpers.h"
#include "gdk_user.h"
#include "godot_gdk.h"
#include "godot_cpp/core/memory.hpp"
#include "godot_cpp/core/print_string.hpp"
#include "godot_cpp/variant/typed_array.hpp"

#include <Windows.h>
#include <winapifamily.h>
#include <objbase.h>
#include <XGameRuntimeInit.h>
#include <XTaskQueue.h>
#include <xsapi-c/services_c.h>
#include <XGame.h>

#include <iomanip>

Ref<GDKAchievementsResultHandle> GDKAchievementsResultHandle::create(XblAchievementsResultHandle result_handle) {
	Ref<GDKAchievementsResultHandle> wrapper;
	if (result_handle) {
		wrapper.instantiate();
		wrapper->result_handle = result_handle;
	}
	return wrapper;
}

Ref<GDKAchievementsResultHandle> GDKAchievementsResultHandle::duplicate() {
	XblAchievementsResultHandle duplicated;
	XblAchievementsResultDuplicateHandle(result_handle, &duplicated);
	return GDKAchievementsResultHandle::create(duplicated);
}

void GDKAchievementsResultHandle::close_handle() {
	if(result_handle) {
		XblAchievementsResultCloseHandle(result_handle);
		result_handle = nullptr;
	}
}

void GDKAchievementsResultHandle::_notification(int p_what) {
	if (p_what == NOTIFICATION_PREDELETE) {
		close_handle();
	}
}

void GDKAchievements::_bind_methods() {
	ClassDB::bind_static_method("GDKAchievements", D_METHOD("get_achievements_async", "user", "achievement_type", "unlocked_only", "achievement_order", "skip_items", "max_items"), &GDKAchievements::get_achievements_async);
	ClassDB::bind_static_method("GDKAchievements", D_METHOD("get_achievements_result", "result_handle"), &GDKAchievements::get_achievements_result);
	ClassDB::bind_static_method("GDKAchievements", D_METHOD("has_more_achievements", "result_handle"), &GDKAchievements::has_more_achievements);
	ClassDB::bind_static_method("GDKAchievements", D_METHOD("get_next_achievements_async", "result_handle", "max_items"), &GDKAchievements::get_next_achievements_async);
	ClassDB::bind_static_method("GDKAchievements", D_METHOD("get_achievement", "user", "achievementId"), &GDKAchievements::get_achievement);
	ClassDB::bind_static_method("GDKAchievements", D_METHOD("unlock_achievement", "user", "achievementId"), &GDKAchievements::unlock_achievement);
	ClassDB::bind_static_method("GDKAchievements", D_METHOD("set_achievement_percentage", "user", "achievementId", "percentage"), &GDKAchievements::set_achievement_percentage);
}

Ref<GDKAsyncBlock> GDKAchievements::get_achievements_async(Ref<GDKUser> user, GDKXblAchievementType::Enum achievement_type, bool unlocked_only, GDKXblAchievementOrderBy::Enum achievement_order, int skip_items, int max_items) {
	XTaskQueueHandle queue = GDKHelpers::get_async_queue();
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(queue);

	asyncBlock->set_callback([](XAsyncBlock* asyncBlock) {
		GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(asyncBlock->context);

		XblAchievementsResultHandle resultHandle;
		HRESULT hr = XblAchievementsGetAchievementsForTitleIdResult(asyncBlock, &resultHandle);

		Dictionary return_data;
		if(GodotGDK::CheckResult(hr, "", "Failed to retrieve achievements")) {
			return_data["result"] = GDKAchievementsResultHandle::create(resultHandle);
		}

		return_data["hresult"] = hr;
		wrapper->emit(return_data);
	});

	XblContextHandle handle;

	if (FAILED(XblContextCreateHandle(GodotGDK::GetUserHandle(), &handle))) {
		return asyncBlock;
	}

	uint32_t titleID;
	if (!GodotGDK::CheckResult(!XGameGetXboxTitleId(&titleID), "Successfully retrieved titleID", "Failed to retrieve titleID")) {
		return asyncBlock;
	}

	HRESULT hr = XblAchievementsGetAchievementsForTitleIdAsync(handle, user->get_id(), titleID, static_cast<XblAchievementType>(achievement_type), unlocked_only, static_cast<XblAchievementOrderBy>(achievement_order), skip_items, max_items, asyncBlock->get_block());

	GodotGDK::CheckResult(hr, "Successfully started retrieving achievements", "Failed to start retrieving achievements");

	return asyncBlock;
}

TypedArray<GDKAchievement> GDKAchievements::get_achievements_result(Ref<GDKAchievementsResultHandle> result_handle) {
	TypedArray<GDKAchievement> achievementArray;

	const XblAchievement *achievements;
	size_t size;
	HRESULT hr = XblAchievementsResultGetAchievements(result_handle->result_handle, &achievements, &size);

	if (!GodotGDK::CheckResult(hr, "", "Failed to retrieve achievements")) {
		return achievementArray;
	}

	for (int i = 0; i < size; i++) {
		achievementArray.push_back(memnew(GDKAchievement(achievements[i])));
	}

	return achievementArray;
}

bool GDKAchievements::has_more_achievements(Ref<GDKAchievementsResultHandle> result_handle) {
	bool hasMoreAchievements = false;

	XblAchievementsResultHasNext(result_handle->result_handle, &hasMoreAchievements);

	return hasMoreAchievements;
}

Ref<GDKAsyncBlock> GDKAchievements::get_next_achievements_async(Ref<GDKAchievementsResultHandle> result_handle, int max_items) {
	XTaskQueueHandle queue = GDKHelpers::get_async_queue();
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(queue);

	if(!has_more_achievements(result_handle)) {
		ERR_PRINT("The handle doesn't have more achievements to retrieve");
		return asyncBlock;
	}

	asyncBlock->set_callback([](XAsyncBlock* asyncBlock) {
		GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(asyncBlock->context);

		XblAchievementsResultHandle resultHandle;
		HRESULT hr = XblAchievementsResultGetNextResult(asyncBlock, &resultHandle);

		Dictionary return_data;
		if(GodotGDK::CheckResult(hr, "", "Failed to retrieve more achievements")) {
			return_data["result"] = GDKAchievementsResultHandle::create(resultHandle);
		}

		return_data["hresult"] = hr;
		wrapper->emit(return_data);
	});

	HRESULT hr = XblAchievementsResultGetNextAsync(result_handle->result_handle, max_items, asyncBlock->get_block());

	GodotGDK::CheckResult(hr, "Successfully started retrieving more achievements", "Failed to start retrieving more achievements");

	return asyncBlock;
}

Ref<GDKAsyncBlock> GDKAchievements::get_achievement(Ref<GDKUser> user, String achievementId) {
	XTaskQueueHandle queue = GDKHelpers::get_async_queue();
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(queue);

	asyncBlock->set_callback([](XAsyncBlock* asyncBlock) {
		GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(asyncBlock->context);

		XblAchievementsResultHandle resultHandle;
		HRESULT hr = XblAchievementsGetAchievementResult(asyncBlock, &resultHandle);

		Dictionary return_data;
		if(GodotGDK::CheckResult(hr, "", "Failed to retrieve achievements")) {
			return_data["result"] = GDKAchievementsResultHandle::create(resultHandle);
		}

		return_data["hresult"] = hr;
		wrapper->emit(return_data);
	});

	XblContextHandle handle;

	if (FAILED(XblContextCreateHandle(GodotGDK::GetUserHandle(), &handle))) {
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

	if (FAILED(XblContextCreateHandle(GodotGDK::GetUserHandle(), &handle))) {
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
