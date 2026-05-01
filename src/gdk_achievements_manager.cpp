#include "gdk_achievements_manager.h"
#include "gdk_user.h"
#include <xsapi-c/achievements_manager_c.h>
#include <variant/typed_array.hpp>

using namespace godot;


void GDKXblAchievementsManagerSortOrder::_bind_methods() {
    BIND_ENUM_CONSTANT(Unsorted);
    BIND_ENUM_CONSTANT(Ascending);
    BIND_ENUM_CONSTANT(Descending);
}

void GDKXblAchievementsManagerResult::_bind_methods() {
    ClassDB::bind_method(D_METHOD("duplicate"), &GDKXblAchievementsManagerResult::duplicate);
    ClassDB::bind_method(D_METHOD("get_achievements"), &GDKXblAchievementsManagerResult::get_achievements);
}

void GDKXblAchievementsManagerResult::_notification(int p_what) {
	if (p_what == NOTIFICATION_PREDELETE) {
        XblAchievementsManagerResultCloseHandle(_handle);
        _handle = nullptr;
    }
}

Ref<GDKXblAchievementsManagerResult> GDKXblAchievementsManagerResult::create(XblAchievementsManagerResultHandle& result) {
	Ref<GDKXblAchievementsManagerResult> wrapper;
    if (result != nullptr) {
        wrapper.instantiate();
        wrapper->_handle = result;
    }

    return wrapper;
}

Ref<GDKXblAchievementsManagerResult> GDKXblAchievementsManagerResult::duplicate() const {
    XblAchievementsManagerResultHandle newHandle = nullptr;
    HRESULT hr = XblAchievementsManagerResultDuplicateHandle(_handle, &newHandle);
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XblAchievementsManagerResultDuplicateHandle Error: 0x%08ux", (uint64_t)hr));
    return GDKXblAchievementsManagerResult::create(newHandle);
}

TypedArray<Ref<GDKAchievement>> GDKXblAchievementsManagerResult::get_achievements() const {
	TypedArray<Ref<GDKAchievement>> return_data;
    const XblAchievement* achievements = nullptr;
    uint64_t count = 0;
    HRESULT hr = XblAchievementsManagerResultGetAchievements(_handle, &achievements, &count);
    ERR_FAIL_COND_V_MSG(FAILED(hr), return_data, vformat("XblAchievementsManagerResultGetAchievements Error: 0x%08ux", (uint64_t)hr));

    for (uint64_t i = 0; i < count; i++) {
        return_data.push_back(memnew(GDKAchievement(achievements[i])));
    }
    return return_data;
}

void GDKXblAchievementsManagerEvent::_bind_methods() {
    BIND_ENUM_CONSTANT(LocalUserInitialStateSynced);
    BIND_ENUM_CONSTANT(AchievementUnlocked);
    BIND_ENUM_CONSTANT(AchievementProgressUpdated);

    ClassDB::bind_method(D_METHOD("get_changes"), &GDKXblAchievementsManagerEvent::get_changes);
    ClassDB::bind_method(D_METHOD("get_event_type"), &GDKXblAchievementsManagerEvent::get_event_type);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "changes", PROPERTY_HINT_NONE, "GDKXblAchievementProgressChangeEntry"), "", "get_changes");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "event_type", PROPERTY_HINT_ENUM, "LocalUserInitialStateSynced,AchievementUnlocked,AchievementProgressUpdated"), "", "get_event_type"); 
}

Ref<GDKXblAchievementsManagerEvent> GDKXblAchievementsManagerEvent::create(const XblAchievementsManagerEvent *event) {
	Ref<GDKXblAchievementsManagerEvent> wrapper;
    if (event != nullptr) {
        wrapper.instantiate();
        
        wrapper->_changes = GDKXblAchievementProgressChangeEntry::create(&event->progressInfo);
        wrapper->_event_type = (EventType)event->eventType;
    }
    return wrapper;
}

void GDKAchievementsManager::_bind_methods() {
    ADD_SIGNAL(MethodInfo("event_change", PropertyInfo(Variant::ARRAY, "events", PROPERTY_HINT_ARRAY_TYPE, "GDKXblAchievementsManagerEvent")));

    ClassDB::bind_method(D_METHOD("add_local_user", "user"), &GDKAchievementsManager::add_local_user);
    ClassDB::bind_method(D_METHOD("get_achievement", "achievement_id"), &GDKAchievementsManager::get_achievement);
    ClassDB::bind_method(D_METHOD("get_achievements", "sort_field", "sort_order"), &GDKAchievementsManager::get_achievements);
    ClassDB::bind_method(D_METHOD("get_achievements_by_state", "sort_field", "sort_order", "state"), &GDKAchievementsManager::get_achievements_by_state);
    ClassDB::bind_method(D_METHOD("is_user_initialized"), &GDKAchievementsManager::is_user_initialized);
    ClassDB::bind_method(D_METHOD("remove_local_user"), &GDKAchievementsManager::remove_local_user);
    ClassDB::bind_method(D_METHOD("update_achievements", "achievement_id", "current_progress"), &GDKAchievementsManager::update_achievements);
}

void GDKAchievementsManager::_notification(int p_what) {
	if (p_what == NOTIFICATION_PREDELETE) {
        XTaskQueueCloseHandle(_queue);
    }
}

void GDKAchievementsManager::_ready() {
    HRESULT hr = XTaskQueueCreate(XTaskQueueDispatchMode::Immediate, XTaskQueueDispatchMode::Immediate, &_queue);
    ERR_FAIL_COND_MSG(FAILED(hr), vformat("GDKAchievementsManager::XTaskQueueCreate Error: 0x%08ux", (uint64_t)hr));
}

void GDKAchievementsManager::_process(double delta) {
    if (_user.is_null()) {
        return;
    } 

    if (_user->get_user() == nullptr) {
        return;
    }

    const XblAchievementsManagerEvent* events = nullptr;
    size_t event_size = 0;
    XblAchievementsManagerDoWork(&events, &event_size);
    
    if (event_size == 0) {
        return;
    }

    TypedArray<Ref<GDKXblAchievementsManagerEvent>> emit_events;
    for (size_t i = 0; i < event_size; i++) {
        const XblAchievementsManagerEvent* event = &events[i];
        Ref<GDKXblAchievementsManagerEvent> e = GDKXblAchievementsManagerEvent::create(event);
        emit_events.push_back(e);
    }

    emit_signal("event_change", emit_events);
}

int GDKAchievementsManager::add_local_user(Ref<GDKUser> user) {
    HRESULT hr = XblAchievementsManagerAddLocalUser(user->get_user(), _queue);
    return (int64_t)hr;
}

Dictionary GDKAchievementsManager::get_achievement(const String &achievement_id) {
	Dictionary return_data;
    if (_user.is_null()) {
        ERR_PRINT(vformat("GDKAchievementsManager::get_achievement Error: user is null"));
        return_data["hresult"] = E_FAIL;
        return return_data;
    }

    XblAchievementsManagerResultHandle result = nullptr;
    HRESULT hr = XblAchievementsManagerGetAchievement(_user->get_id(), achievement_id.utf8().get_data(), &result);
    return_data["hresult"] = hr;

    if (SUCCEEDED(hr)) {
        return_data["result"] = GDKXblAchievementsManagerResult::create(result);
    }
    return return_data;
}

Dictionary GDKAchievementsManager::get_achievements(GDKXblAchievementOrderBy::Enum sort_field, GDKXblAchievementsManagerSortOrder::Enum sort_order) {
    Dictionary return_data;
    if (_user.is_null()) {
        ERR_PRINT(vformat("GDKAchievementsManager::get_achievements Error: user is null"));
        return_data["hresult"] = E_FAIL;
        return return_data;
    }

    XblAchievementsManagerResultHandle result = nullptr;
    HRESULT hr = XblAchievementsManagerGetAchievements(_user->get_id(), (XblAchievementOrderBy)sort_field, (XblAchievementsManagerSortOrder)sort_order, &result);
    return_data["hresult"] = hr;

    if (SUCCEEDED(hr)) {
        return_data["result"] = GDKXblAchievementsManagerResult::create(result);
    }
    return return_data;
}

Dictionary GDKAchievementsManager::get_achievements_by_state(GDKXblAchievementOrderBy::Enum sort_field, GDKXblAchievementsManagerSortOrder::Enum sort_order, GDKXblAchievementProgressState::Enum state) {
	Dictionary return_data;
    if (_user.is_null()) {
        ERR_PRINT(vformat("GDKAchievementsManager::get_achievements_by_state Error: user is null"));
        return_data["hresult"] = E_FAIL;
        return return_data;
    }

    XblAchievementsManagerResultHandle result = nullptr;
    HRESULT hr = XblAchievementsManagerGetAchievementsByState(_user->get_id(), (XblAchievementOrderBy)sort_field, (XblAchievementsManagerSortOrder)sort_order, (XblAchievementProgressState)state, &result);
    return_data["hresult"] = hr;

    if (SUCCEEDED(hr)) {
        return_data["result"] = GDKXblAchievementsManagerResult::create(result);
    }
    return return_data;
}

int GDKAchievementsManager::is_user_initialized() const {
	if (_user.is_null()) {
        ERR_PRINT(vformat("GDKAchievementsManager::is_user_initialized Error: user is null"));
        return (int64_t)E_FAIL;
    }

    HRESULT hr = XblAchievementsManagerIsUserInitialized(_user->get_id());
    return (int64_t)hr;
}

int GDKAchievementsManager::remove_local_user() {
    if (_user.is_null()) {
        ERR_PRINT(vformat("GDKAchievementsManager::remove_local_user Error: user is null"));
        return (int64_t)E_FAIL;
    }
    HRESULT hr = XblAchievementsManagerRemoveLocalUser(_user->get_user());
    _user = Ref<GDKUser>(nullptr);
    return (int64_t)hr;
}

int GDKAchievementsManager::update_achievements(const String &achievements_id, int64_t current_progress) {
    if (_user.is_null()) {
        ERR_PRINT(vformat("GDKAchievementsManager::update_achievements Error: user is null"));
        return (int64_t)E_FAIL;
    }

    if (current_progress < 1 || current_progress > 100) {
        ERR_PRINT(vformat("GDKAchievementsManager::update_achievements Error: current_progress must be >= 1 and <= 100"));
        return (int64_t)E_INVALIDARG;
    }

    HRESULT hr = XblAchievementsManagerUpdateAchievement(_user->get_id(), achievements_id.utf8().get_data(), current_progress);
    ERR_FAIL_COND_V_MSG(FAILED(hr), (int64_t)hr, vformat("XblAchievementsManagerUpdateAchievement Error: 0x%08ux", (uint64_t)hr));
    return (int64_t)hr;
}
