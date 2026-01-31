#include "gdk_game_save_blob_info.h"

void GDKGameSaveBlobInfo::setInfo(const XGameSaveBlobInfo* src) {
	if (!src) return;

	name = src->name;
	size = src->size;
}
void GDKGameSaveBlobInfo::_bind_methods() {
	// Godot-visible properties via getters
	ClassDB::bind_method(D_METHOD("get_name"), &GDKGameSaveBlobInfo::get_name);
	ClassDB::bind_method(D_METHOD("get_size"), &GDKGameSaveBlobInfo::get_size);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "", "get_name");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "size"), "", "get_size");
}
