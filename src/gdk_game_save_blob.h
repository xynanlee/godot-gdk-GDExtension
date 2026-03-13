#pragma once

#include "gdk_game_save_blob_info.h"
#include "godot_cpp/classes/ref.hpp"

#include <Windows.h>
#include <winapifamily.h>
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/variant.hpp"
#include <XGameSave.h>
#include <list>

using namespace godot;

class GDKGameSaveBlob : public RefCounted {
	GDCLASS(GDKGameSaveBlob, RefCounted)

protected:
	static void _bind_methods();

public:
	GDKGameSaveBlob() = default;
	explicit GDKGameSaveBlob(const XGameSaveBlob* src);

	~GDKGameSaveBlob() override = default;

	Ref<GDKGameSaveBlobInfo> info;
	PackedByteArray data;

	Ref<GDKGameSaveBlobInfo> get_info() const { return info; }
	PackedByteArray get_data() const { return data; }
};
