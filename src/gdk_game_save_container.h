#pragma once

#include <classes/ref_counted.hpp>
#include <variant/typed_dictionary.hpp>
#include "gdk_asyncblock.h"
#include <XGameSave.h>

namespace godot {
class GDKGameSaveUpdate;
//x This is an RAII container, it autocloses the handle when its freed.
class GDKGameSaveContainer: public RefCounted {
	GDCLASS(GDKGameSaveContainer, RefCounted);

private:
	String _name;
	XGameSaveContainerHandle _container_handle = nullptr;

protected:
	static void _bind_methods();
	virtual void _notification(int p_what);

public:
	static Ref<GDKGameSaveContainer> create(XGameSaveContainerHandle containerHandle, const String& name);
	Ref<GDKGameSaveUpdate> create_update() const;
	inline String get_name() const { return _name; }

	TypedDictionary<String, int> enumerate_blob_info() const;
	TypedDictionary<String, int> enumerate_blob_info_by_name(const String& prefix) const;

	Dictionary read_blob_data(PackedStringArray blobNames, int blobSize);
	Ref<GDKAsyncBlock> read_blob_data_async(PackedStringArray blobNames);
}; 
}