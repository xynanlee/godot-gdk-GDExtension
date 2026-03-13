#pragma once

#include <classes/object.hpp>
#include <classes/ref_counted.hpp>
#include <windows.h>
#include <XGameInvite.h>
#include <XTaskQueue.h>

namespace godot {

class GDKGameInviteToken : public RefCounted {
    GDCLASS(GDKGameInviteToken, RefCounted)

private:
    XTaskQueueRegistrationToken _token = {};
    void* _context = nullptr;

protected:
    static void _bind_methods() {}

public:
    static Ref<GDKGameInviteToken> create(XTaskQueueRegistrationToken token, void* context) {
        Ref<GDKGameInviteToken> wrapper;
        wrapper.instantiate();
        wrapper->_token = token;
        wrapper->_context = context;
        return wrapper;
    }

    inline XTaskQueueRegistrationToken get_token() const { return _token; }
    inline void* get_context() const { return _context; }
};

class GDKGameInvite : public Object {
    GDCLASS(GDKGameInvite, Object)

protected:
    static void _bind_methods();

public:
    static Ref<GDKGameInviteToken> register_for_event(Callable callback);
    static bool unregister_for_event(Ref<GDKGameInviteToken> token, bool wait);
};

} // namespace godot
