#pragma once

#include <classes/node.hpp>
#include <Windows.h>
extern "C" {
    #include <XTaskQueue.h>
}
#include "gdk_achievement.h"

namespace godot {
class GDKUser;
class GDKXblAchievementsManagerSortOrder: public Object {
    GDCLASS(GDKXblAchievementsManagerSortOrder, Object);

protected:
    static void _bind_methods();

public:
    enum Enum: uint32_t {
        Unsorted = XblAchievementsManagerSortOrder::Unsorted,
        Ascending = XblAchievementsManagerSortOrder::Ascending,
        Descending = XblAchievementsManagerSortOrder::Descending
    };
};

class GDKXblAchievementsManagerResult: public RefCounted {
    GDCLASS(GDKXblAchievementsManagerResult, RefCounted);

private:
    XblAchievementsManagerResultHandle _handle = nullptr;

protected:
    static void _bind_methods();
    void _notification(int p_what);

public:
    static Ref<GDKXblAchievementsManagerResult> create(XblAchievementsManagerResultHandle& result);
    Ref<GDKXblAchievementsManagerResult> duplicate() const;
    TypedArray<Ref<GDKAchievement>> get_achievements() const;
};

class GDKXblAchievementsManagerEvent : public RefCounted {
    GDCLASS(GDKXblAchievementsManagerEvent, RefCounted);

public:
    enum EventType: uint32_t {
        LocalUserInitialStateSynced = XblAchievementsManagerEventType::LocalUserInitialStateSynced,
        AchievementUnlocked = XblAchievementsManagerEventType::AchievementUnlocked,
        AchievementProgressUpdated = XblAchievementsManagerEventType::AchievementProgressUpdated
    };

private:
    Ref<GDKXblAchievementProgressChangeEntry> _changes;
    EventType _event_type;

protected:
    static void _bind_methods();

public:
    static Ref<GDKXblAchievementsManagerEvent> create(const XblAchievementsManagerEvent* event);

    inline Ref<GDKXblAchievementProgressChangeEntry> get_changes() const { return _changes; }
    inline EventType get_event_type() const { return _event_type; }
};

class GDKAchievementsManager: public Node {
    GDCLASS(GDKAchievementsManager, Node);

private:
    XTaskQueueHandle _queue;
    Ref<GDKUser> _user;

protected:
    static void _bind_methods();
    void _notification(int p_what);

public:
    void _ready() override;
    void _process(double delta) override;

    int add_local_user(Ref<GDKUser> user);
    Dictionary get_achievement(const String& achievement_id);
    Dictionary get_achievements(GDKXblAchievementOrderBy::Enum sort_field, GDKXblAchievementsManagerSortOrder::Enum sort_order);
    Dictionary get_achievements_by_state(   GDKXblAchievementOrderBy::Enum sort_field,
                                            GDKXblAchievementsManagerSortOrder::Enum sort_order,
                                            GDKXblAchievementProgressState::Enum state);
    int is_user_initialized() const;
    int remove_local_user();
    int update_achievements(const String& achievements_id, int64_t current_progress);
};
}

VARIANT_ENUM_CAST(GDKXblAchievementsManagerEvent::EventType);
VARIANT_ENUM_CAST(GDKXblAchievementsManagerSortOrder::Enum);