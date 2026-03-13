#include "gdk_game_invite.h"
#include "gdk_helpers.h"

using namespace godot;

void GDKGameInvite::_bind_methods() {
    ClassDB::bind_static_method("GDKGameInvite", D_METHOD("register_for_event", "callback"), &GDKGameInvite::register_for_event);
    ClassDB::bind_static_method("GDKGameInvite", D_METHOD("unregister_for_event", "token", "wait"), &GDKGameInvite::unregister_for_event);
}

Ref<GDKGameInviteToken> GDKGameInvite::register_for_event(Callable callback) {
    Callable* cb = new Callable(callback);
    XTaskQueueHandle queue = GDKHelpers::get_async_queue();
    XTaskQueueRegistrationToken token = {};

    HRESULT hr = XGameInviteRegisterForEvent(queue, cb, [](void* context, const char* inviteUri) {
        Callable* cb = static_cast<Callable*>(context);
        cb->call(String(inviteUri ? inviteUri : ""));
    }, &token);

    ERR_FAIL_COND_V_MSG(FAILED(hr), Ref<GDKGameInviteToken>(), vformat("XGameInviteRegisterForEvent Error: 0x%08ux", (uint64_t)hr));
    return GDKGameInviteToken::create(token, cb);
}

bool GDKGameInvite::unregister_for_event(Ref<GDKGameInviteToken> token, bool wait) {
    bool result = XGameInviteUnregisterForEvent(token->get_token(), wait);
    Callable* cb = static_cast<Callable*>(token->get_context());
    delete cb;
    return result;
}
