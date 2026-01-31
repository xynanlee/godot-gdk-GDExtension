#pragma once

#include "gdk_achievement.h"

#include <Windows.h>
#include <winapifamily.h>
#include <objbase.h>
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/variant.hpp"
#include <XAsync.h>
#include <XTaskQueue.h>
#include <list>
#include <xsapi-c/services_c.h>


using namespace godot;

class GDKAchievements : public RefCounted {
	GDCLASS(GDKAchievements, RefCounted)

	static void CALLBACK GetAchievementsCallback(_Inout_ XAsyncBlock* asyncBlock);
	static void InternalSetAchievementPercentage(String achievementId, uint32_t percentage, const std::string &successMessage, const std::string &failMessage);
protected:
	static void _bind_methods();

public:
	GDKAchievements() = default;
	~GDKAchievements() override = default;

	void GetAchievements(Callable callback);
	void UnlockAchievement(String achievementId);
	void SetAchievementPercentage(String achievementId, uint32_t percentage);
};
