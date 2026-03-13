#include "gdk_game.h"

using namespace godot;

void GDKGame::_bind_methods() {
    ClassDB::bind_static_method("GDKGame", D_METHOD("get_xbox_title_id"), &GDKGame::get_xbox_title_id);
    ClassDB::bind_static_method("GDKGame", D_METHOD("launch_new_game", "exe_path", "args", "default_user"), &GDKGame::launch_new_game);
    ClassDB::bind_static_method("GDKGame", D_METHOD("launch_restart_on_crash", "args"), &GDKGame::launch_restart_on_crash);
}

int GDKGame::get_xbox_title_id() {
    uint32_t title_id = 0;
    HRESULT hr = XGameGetXboxTitleId(&title_id);
    ERR_FAIL_COND_V_MSG(FAILED(hr), 0, vformat("XGameGetXboxTitleId Error: 0x%08ux", (uint64_t)hr));
    return (int)title_id;
}

void GDKGame::launch_new_game(const String &exe_path, const String &args, Ref<GDKUser> default_user) {
    XUserHandle user = default_user.is_valid() ? default_user->get_user() : nullptr;
    const char *args_ptr = args.is_empty() ? nullptr : args.utf8().get_data();
    XLaunchNewGame(exe_path.utf8().get_data(), args_ptr, user);
}

int GDKGame::launch_restart_on_crash(const String &args) {
    const char *args_ptr = args.is_empty() ? nullptr : args.utf8().get_data();
    HRESULT hr = XLaunchRestartOnCrash(args_ptr, 0);
    ERR_FAIL_COND_V_MSG(FAILED(hr), (int)hr, vformat("XLaunchRestartOnCrash Error: 0x%08ux", (uint64_t)hr));
    return (int)hr;
}
