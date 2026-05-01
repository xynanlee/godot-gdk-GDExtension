#pragma once

#include <classes/ref_counted.hpp>
#include <Windows.h>

extern "C" {
    #include <XTaskQueue.h>
}

namespace godot {
class GDKEventObject: public RefCounted {
    GDCLASS(GDKEventObject, RefCounted);

protected:
    static void _bind_methods();

public:
    virtual HRESULT initialize();
};
}