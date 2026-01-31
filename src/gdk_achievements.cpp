#include "gdk_achievements.h"

#include "gdk_achievement.h"
#include "godot_gdk.h"
#include "../godot-cpp/include/godot_cpp/core/memory.hpp"

#include <Windows.h>
#include <winapifamily.h>
#include <objbase.h>
#include <XGameRuntimeInit.h>
#include <XTaskQueue.h>
#include <xsapi-c/services_c.h>
#include <XUser.h>
#include <XGame.h>

#include <iomanip>
#include <list>
#include <map>
#include <unordered_map>

void GDKAchievements::_bind_methods() {
	godot::ClassDB::bind_method(D_METHOD("GetAchievements", "callback"), &GDKAchievements::GetAchievements);
	godot::ClassDB::bind_method(D_METHOD("UnlockAchievement", "achievementId"), &GDKAchievements::UnlockAchievement);
	godot::ClassDB::bind_method(D_METHOD("SetAchievementPercentage", "achievementId", "percentage"), &GDKAchievements::SetAchievementPercentage);
}

void GDKAchievements::GetAchievements(godot::Callable callback) {
	XAsyncBlock* asyncBlock = GodotGDK::CreateAsyncBlock();

	std::unordered_map<std::string, void*>* callbackContext = new std::unordered_map<std::string, void*>();
	(*callbackContext)["Callback"] = new godot::Callable(callback);
	(*callbackContext)["Array"] = new godot::Array();

	asyncBlock->context = callbackContext;
	asyncBlock->callback = GetAchievementsCallback;

	XblContextHandle handle;

	if (!GodotGDK::CreateContextHandle(&handle)) {
		return;
	}

	uint32_t titleID;
	if (!GodotGDK::CheckResult(!XGameGetXboxTitleId(&titleID), "Successfully retrieved titleID", "Failed to retrieve titleID")) {
		return;
	}

	HRESULT hr = XblAchievementsGetAchievementsForTitleIdAsync(handle, GodotGDK::GetUserId().value, titleID, XblAchievementType::All, false, XblAchievementOrderBy::DefaultOrder, 0, 0, asyncBlock);

	GodotGDK::CheckResult(hr, "Successfully started retrieving achievements", "Failed to start retrieving achievements");
}

void GDKAchievements::GetAchievementsCallback(XAsyncBlock *asyncBlock) {
	std::unordered_map<std::string, void*>* ExtraContext = static_cast<std::unordered_map<std::string, void*>*>(asyncBlock->context);
	godot::Array* achievementArray = static_cast<godot::Array*>((*ExtraContext)["Array"]);

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
		achievementArray->push_back(memnew(GDKAchievement(&achievements[i])));
	}

	bool hasMoreAchievements = false;

	//GDK can only retrieve 32 achievements at once. Check if there is more, and retrieve until we have all.
	XblAchievementsResultHasNext(resultHandle, &hasMoreAchievements);

	if (hasMoreAchievements) {
		XblAchievementsResultGetNextAsync(resultHandle, 0, asyncBlock);
	} else {

		print_line("Call callback");

		godot::Callable* callback = static_cast<godot::Callable*>((*ExtraContext)["Callback"]);
		if (callback->is_valid()) {
			print_line("Callback is valid");
			callback->call_deferred(*achievementArray);
		}
		print_line("Delete stuff");

		//delete achievementArray;
		//delete callback;
		//delete &ExtraContext;
		delete asyncBlock;
	}

	XblAchievementsResultCloseHandle(resultHandle);
}

void GDKAchievements::UnlockAchievement(godot::String achievementId) {
	InternalSetAchievementPercentage(achievementId, 100, "Successfully unlocked achievement", "Failed to unlock achievement");
}


void GDKAchievements::SetAchievementPercentage(godot::String achievementId, uint32_t percentage) {
	InternalSetAchievementPercentage(achievementId, percentage, "Successfully set achievement progress", "Failed to set achievement progress");
}

void GDKAchievements::InternalSetAchievementPercentage(godot::String achievementId, uint32_t percentage, const std::string &successMessage, const std::string &failMessage) {

	XblContextHandle handle;

	if (!GodotGDK::CreateContextHandle(&handle)) {
		return;
	}

	XAsyncBlock* asyncBlock = GodotGDK::CreateAsyncBlock();

	HRESULT result = XblAchievementsUpdateAchievementAsync(handle, GodotGDK::GetUserId().value, achievementId.utf8(), percentage, asyncBlock);

	GodotGDK::CheckResult(result, successMessage, failMessage);
}
