#pragma once

#include <classes/ref_counted.hpp>
#include <XGameRuntimeTypes.h>

namespace godot {
class GDKXVersion: public RefCounted {
    GDCLASS(GDKXVersion, RefCounted);

private:
    XVersion _version;

protected:
    static void _bind_methods();

public:
    static Ref<GDKXVersion> create(const XVersion* version);
    int get_int_value() const;
    String get_readable_text() const;
};
}