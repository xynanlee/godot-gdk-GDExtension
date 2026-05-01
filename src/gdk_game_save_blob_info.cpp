#include "gdk_game_save_blob_info.h"

using namespace godot;
Ref<GDKGameSaveBlobInfo> GDKGameSaveBlobInfo::create(const XGameSaveBlobInfo *info) {
	Ref<GDKGameSaveBlobInfo> wrapper;
	if (info != nullptr) {
		wrapper.instantiate();
		wrapper->_name = String(info->name);
		wrapper->_size = (int64_t)info->size;
	}
	return wrapper;
}

void GDKGameSaveBlobInfo::_bind_methods() {
	// Godot-visible properties via getters
	ClassDB::bind_method(D_METHOD("get_name"), &GDKGameSaveBlobInfo::get_name);
	ClassDB::bind_method(D_METHOD("get_size"), &GDKGameSaveBlobInfo::get_size);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "", "get_name");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "size"), "", "get_size");
}
