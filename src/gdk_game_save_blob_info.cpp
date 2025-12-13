#include "gdk_game_save_blob_info.h"

gdk_game_save_blob_info::gdk_game_save_blob_info(const XGameSaveBlobInfo* src) {
	if (!src) return;

	name = src->name;
	size = src->size;
}
void gdk_game_save_blob_info::_bind_methods() {
	// Godot-visible properties via getters
	ClassDB::bind_method(D_METHOD("get_name"), &gdk_game_save_blob_info::get_name);
	ClassDB::bind_method(D_METHOD("get_size"), &gdk_game_save_blob_info::get_size);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "", "get_name");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "size"), "", "get_size");
}
