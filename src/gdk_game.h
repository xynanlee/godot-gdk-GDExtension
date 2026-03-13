#pragma once

#include <classes/object.hpp>
#include <windows.h>
#include <XGame.h>
#include "gdk_user.h"

namespace godot {

class GDKGame : public Object {
    GDCLASS(GDKGame, Object)

protected:
    static void _bind_methods();

public:
    static int get_xbox_title_id();
    static void launch_new_game(const String &exe_path, const String &args, Ref<GDKUser> default_user);
    static int launch_restart_on_crash(const String &args);
};

} // namespace godot
