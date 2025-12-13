#pragma once

#include "gdk_game_save_blob_info.h"

#include <Windows.h>
#include <winapifamily.h>
#include <objbase.h>
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/variant.hpp"
#include <XAsync.h>
#include <XGameSave.h>
#include <XTaskQueue.h>
#include <list>
#include <xsapi-c/services_c.h>

using namespace godot;

class gdk_game_save_blob : public RefCounted {
	GDCLASS(gdk_game_save_blob, RefCounted)

protected:
	static void _bind_methods();

public:
	gdk_game_save_blob() = default;
	explicit gdk_game_save_blob(const XGameSaveBlob* src);

	~gdk_game_save_blob() override = default;

	gdk_game_save_blob_info* info;
	PackedByteArray data;

	gdk_game_save_blob_info* get_info() const { return info; }
	PackedByteArray get_data() const { return data; }
};
