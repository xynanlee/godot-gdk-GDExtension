#include "gdk_achievements.h"
#include "godot_gdk.h"

#include <Windows.h>
#include <winapifamily.h>
#include <objbase.h>
#include <XGameRuntimeInit.h>
#include <XTaskQueue.h>
#include <xsapi-c/services_c.h>
#include <XUser.h>
#include <XGame.h>

#include <iomanip>
#include <ios>
#include <sstream>
#include <string>


void gdk_achievements::_bind_methods() {
	godot::ClassDB::bind_method(D_METHOD("GetAchievements"), &gdk_achievements::GetAchievements);
}

void gdk_achievements::GetAchievements() {
	XAsyncBlock* asyncBlock = new XAsyncBlock();
	XTaskQueueHandle queue_handle = godot_gdk::GetQueueHandle();
	asyncBlock->queue = queue_handle;
	asyncBlock->callback = GetAchievementsCallback;

	XblContextHandle handle;

	if (!godot_gdk::CreateContextHandle(&handle)) {
		return;
	}

	uint32_t titleID;
	if (!godot_gdk::CheckResult(!XGameGetXboxTitleId(&titleID), "Successfully retrieved titleID", "Failed to retrieve titleID"))
	{
		return;
	}

	HRESULT hr = XblAchievementsGetAchievementsForTitleIdAsync(handle, godot_gdk::GetUserId().value, titleID, XblAchievementType::All, false, XblAchievementOrderBy::DefaultOrder, 0, 32, asyncBlock);

	godot_gdk::CheckResult(hr, "Successfully started retrieving achievements", "Failed to start retrieving achievements");
}

void gdk_achievements::GetAchievementsCallback(XAsyncBlock *asyncBlock) {

	XblAchievementsResultHandle resultHandle;
	HRESULT hr = XblAchievementsGetAchievementsForTitleIdResult(asyncBlock, &resultHandle);

	if (!godot_gdk::CheckResult(hr, "Successfully retrieved achievements", "Failed to retrieve achievements")) {
		return;
	}

	const XblAchievement* achievement;
	size_t size;
	hr = XblAchievementsResultGetAchievements(resultHandle, &achievement, &size);

	if (!godot_gdk::CheckResult(hr, "Successfully retrieved achievements", "Failed to retrieve achievements")) {
		return;
	}

	for (int i = 0; i < size; i++) {
		print_line(achievement[i].name);
	}

	XblAchievementsResultCloseHandle(resultHandle);
}
