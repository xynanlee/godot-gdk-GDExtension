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

	static Ref<GDKAsyncBlock> internal_set_achievement_percentage(Ref<GDKUser> user, String achievementId, uint32_t percentage, const String &successMessage, const String &failMessage);

protected:
	static void _bind_methods();

public:
	GDKAchievements() = default;
	~GDKAchievements() override = default;

	static Ref<GDKAsyncBlock> get_achievements_async(Ref<GDKUser> user, GDKXblAchievementType::Enum achievement_type, bool unlocked_only, GDKXblAchievementOrderBy::Enum achievement_order, int skip_items, int max_items);
	static TypedArray<GDKAchievement> get_achievements_result(Ref<GDKAchievementsResultHandle> result_handle);
	static bool has_more_achievements(Ref<GDKAchievementsResultHandle> result_handle);
	static Ref<GDKAsyncBlock> get_next_achievements_async(Ref<GDKAchievementsResultHandle> result_handle, int max_items);
	static Ref<GDKAsyncBlock> get_achievement(Ref<GDKUser> user, String achievementId);
	static Ref<GDKAsyncBlock> unlock_achievement(Ref<GDKUser> user, String achievementId);
	static Ref<GDKAsyncBlock> set_achievement_percentage(Ref<GDKUser> user, String achievementId, uint32_t percentage);
	static int64_t add_achievement_progress_change_handler(Callable callback);
	static void remove_achievement_progress_change_handler(int64_t context);
};
}
