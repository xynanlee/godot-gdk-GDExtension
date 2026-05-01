#include "gdk_game_save_blob.h"
#include <variant/packed_byte_array.hpp>

using namespace godot;

Ref<GDKGameSaveBlob> GDKGameSaveBlob::create(const XGameSaveBlob *blob) {
	Ref<GDKGameSaveBlob> wrapper;
	if (blob) {
		wrapper.instantiate();
		wrapper->_info = GDKGameSaveBlobInfo::create(&blob->info);

		PackedByteArray data;
		data.resize(blob->info.size);
		memcpy(data.ptrw(), blob->data, blob->info.size);
		wrapper->_data = data;
	}
	return wrapper;
}

void GDKGameSaveBlob::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_info"), &GDKGameSaveBlob::get_info);
	ClassDB::bind_method(D_METHOD("get_data"), &GDKGameSaveBlob::get_data);

	ADD_PROPERTY(PropertyInfo(godot::Variant::OBJECT, "info"), "", "get_info");
	ADD_PROPERTY(PropertyInfo(godot::Variant::PACKED_BYTE_ARRAY, "data"), "", "get_data");
}
