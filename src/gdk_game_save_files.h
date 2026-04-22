#pragma once

#include <classes/ref_counted.hpp>
#include <classes/thread.hpp>
#include "gdk_asyncblock.h"
#include "gdk_user.h"

namespace godot {
class GDKPromise: public RefCounted {
    GDCLASS(GDKPromise, RefCounted)

protected:
    Ref<Thread> _asyncThread;
    Array _inputData;

    /*-------------------------------------------------------------------------------------------------------------------*/

protected:
    static void _bind_methods();
    void _notification(int p_what);
    virtual void _threaded_function();

public:
    virtual void start(Array inputData, Thread::Priority priority = Thread::PRIORITY_NORMAL);
};

class GDKGameSaveFiles: public RefCounted {
    GDCLASS(GDKGameSaveFiles, RefCounted)

protected:
    static void _bind_methods();

public:
    static Ref<GDKAsyncBlock> get_folder_with_ui_async(Ref<GDKUser> user);
    static Ref<GDKPromise> get_remaining_quota(Ref<GDKUser> user);
};
}