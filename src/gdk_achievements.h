#pragma once

#include "gdk_achievement.h"
#include "gdk_asyncblock.h"
#include "gdk_user.h"

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

	static void CALLBACK get_achievements_callback(_Inout_ XAsyncBlock* asyncBlock);
	static Ref<GDKAsyncBlock> internal_set_achievement_percentage(Ref<GDKUser> user, String achievementId, uint32_t percentage, const String &successMessage, const String &failMessage);
protected:
	static void _bind_methods();

public:
	GDKAchievements() = default;
	~GDKAchievements() override = default;

	Ref<GDKAsyncBlock> get_achievements(Ref<GDKUser> user, GDKXblAchievementType::Enum achievementType, bool unlockedOnly, GDKXblAchievementOrderBy::Enum achievementOrder);
	Ref<GDKAsyncBlock> get_achievement(Ref<GDKUser> user, String achievementId);
	Ref<GDKAsyncBlock> unlock_achievement(Ref<GDKUser> user, String achievementId);
	Ref<GDKAsyncBlock> set_achievement_percentage(Ref<GDKUser> user, String achievementId, uint32_t percentage);
};
