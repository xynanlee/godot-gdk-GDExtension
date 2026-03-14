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
#include <list>
#include <xsapi-c/services_c.h>

using namespace godot;

class GDKAchievementsResultHandle : public RefCounted {
	GDCLASS(GDKAchievementsResultHandle, RefCounted);

private:

protected:
	static void _bind_methods() { };
	virtual void _notification(int p_what);

public:
	GDKAchievementsResultHandle() = default;
	static Ref<GDKAchievementsResultHandle> create(XblAchievementsResultHandle result_handle);
	XblAchievementsResultHandle result_handle = nullptr;
};

class GDKAchievements : public RefCounted {
	GDCLASS(GDKAchievements, RefCounted)

	static void CALLBACK get_achievements_callback(_Inout_ XAsyncBlock* asyncBlock);
	static Ref<GDKAsyncBlock> internal_set_achievement_percentage(Ref<GDKUser> user, String achievementId, uint32_t percentage, const String &successMessage, const String &failMessage);
protected:
	static void _bind_methods();

public:
	GDKAchievements() = default;
	~GDKAchievements() override = default;

	Ref<GDKAsyncBlock> get_achievements_async(Ref<GDKUser> user, GDKXblAchievementType::Enum achievement_type, bool unlocked_only, GDKXblAchievementOrderBy::Enum achievement_order, int skip_items, int max_items);
	TypedArray<GDKAchievement> get_achievements_result(Ref<GDKAchievementsResultHandle> result_handle);
	bool has_more_achievements(Ref<GDKAchievementsResultHandle> result_handle);
	Ref<GDKAsyncBlock> get_next_achievements_async(Ref<GDKAchievementsResultHandle> result_handle, int max_items);
	Ref<GDKAsyncBlock> get_achievement(Ref<GDKUser> user, String achievementId);
	Ref<GDKAsyncBlock> unlock_achievement(Ref<GDKUser> user, String achievementId);
	Ref<GDKAsyncBlock> set_achievement_percentage(Ref<GDKUser> user, String achievementId, uint32_t percentage);
};
