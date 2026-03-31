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


namespace godot {
class GDKAchievementsResultHandle : public RefCounted {
	GDCLASS(GDKAchievementsResultHandle, RefCounted);

private:

protected:
	static void _bind_methods() { };
	virtual void _notification(int p_what);

public:
	GDKAchievementsResultHandle() = default;

	XblAchievementsResultHandle result_handle = nullptr;

	static Ref<GDKAchievementsResultHandle> create(XblAchievementsResultHandle result_handle);
	Ref<GDKAchievementsResultHandle> duplicate();
	void close_handle();
};

class GDKAchievements : public RefCounted {
	GDCLASS(GDKAchievements, RefCounted)

	Ref<GDKUser> _user;
	XblFunctionContext _progress_change_context;
	Ref<GDKAsyncBlock> internal_set_achievement_percentage(String achievementId, uint32_t percentage, const String &successMessage, const String &failMessage);
	void _notification(int p_what);

protected:
	static void _bind_methods();

public:
	GDKAchievements() = default;
	~GDKAchievements() override = default;

	static Ref<GDKAchievements> create(Ref<GDKUser>);
	Ref<GDKAsyncBlock> get_achievements_async(GDKXblAchievementType::Enum achievement_type, bool unlocked_only, GDKXblAchievementOrderBy::Enum achievement_order, int skip_items, int max_items);
	TypedArray<GDKAchievement> get_achievements_result(Ref<GDKAchievementsResultHandle> result_handle);
	bool has_more_achievements(Ref<GDKAchievementsResultHandle> result_handle);
	Ref<GDKAsyncBlock> get_next_achievements_async(Ref<GDKAchievementsResultHandle> result_handle, int max_items);
	Ref<GDKAsyncBlock> get_achievement(String achievementId);
	Ref<GDKAsyncBlock> unlock_achievement(String achievementId);
	Ref<GDKAsyncBlock> set_achievement_percentage(String achievementId, uint32_t percentage);
};
}
