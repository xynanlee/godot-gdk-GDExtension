#pragma once

#include <classes/ref_counted.hpp>
#include <Windows.h>
#include <XGameSave.h>
#include "gdk_game_save_blob_info.h"

namespace godot {
class GDKGameSaveBlob : public RefCounted {
	GDCLASS(GDKGameSaveBlob, RefCounted)

private:
	Ref<GDKGameSaveBlobInfo> _info;
	PackedByteArray _data;

protected:
	static void _bind_methods();

public:
	static Ref<GDKGameSaveBlob> create(const XGameSaveBlob* blob);

	inline Ref<GDKGameSaveBlobInfo> get_info() const { return _info; }
	inline PackedByteArray get_data() const { return _data; }
};
}