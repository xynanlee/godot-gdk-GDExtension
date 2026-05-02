#pragma once

#include <classes/ref_counted.hpp>
#include <Windows.h>
#include <XGameSave.h>

namespace godot {
class GDKGameSaveBlobInfo : public RefCounted {
	GDCLASS(GDKGameSaveBlobInfo, RefCounted)

private:
	String _name;
	uint32_t _size = 0;

protected:
	static void _bind_methods();

public:
	static Ref<GDKGameSaveBlobInfo> create(const XGameSaveBlobInfo* info);

    inline String get_name() const { return _name; }
    inline int64_t get_size() const { return (uint32_t)_size; }
};
}