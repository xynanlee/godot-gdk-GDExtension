#include "gdk_game_save_blob.h"

#include "../godot-cpp/gen/include/godot_cpp/variant/packed_byte_array.hpp"
#include "../godot-cpp/include/godot_cpp/core/memory.hpp"
#include "../godot-cpp/include/godot_cpp/core/print_string.hpp"

GDKGameSaveBlob::GDKGameSaveBlob(const XGameSaveBlob* src) {
	if (!src) return;

	info.instantiate();
	info->setInfo(&src->info);

	data.resize(src->info.size);
	memcpy(data.ptrw(), src->data, src->info.size);
	godot::print_line(data);
}

void GDKGameSaveBlob::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_info"), &GDKGameSaveBlob::get_info);
	ClassDB::bind_method(D_METHOD("get_data"), &GDKGameSaveBlob::get_data);

	ADD_PROPERTY(PropertyInfo(godot::Variant::OBJECT, "info"), "", "get_info");
	ADD_PROPERTY(PropertyInfo(godot::Variant::PACKED_BYTE_ARRAY, "data"), "", "get_data");
}
