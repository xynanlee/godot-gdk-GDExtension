#pragma once

#include <classes/ref_counted.hpp>
#include "gdk_asyncblock.h"
#include "gdk_user.h"

namespace godot {
class GDKGameSaveFiles: public RefCounted {
    GDCLASS(GDKGameSaveFiles, RefCounted)

protected:
    static void _bind_methods();

public:
    static Ref<GDKAsyncBlock> get_folder_with_ui_async(Ref<GDKUser> user);
    static int64_t get_remaining_quota(Ref<GDKUser> user);
};
}