#pragma once

#include <classes/ref_counted.hpp>
#include <variant/typed_dictionary.hpp>
#include "gdk_asyncblock.h"
#include <XGameSave.h>

namespace godot {
class GDKGameSaveUpdate;
class GDKGameSaveBlobInfo;
class GDKGameSaveContainerInfo: public RefCounted {
	GDCLASS(GDKGameSaveContainerInfo, RefCounted);

private:
	String _name;
	String _display_name;
	uint32_t _blob_count = 0;
	uint64_t _total_size = 0;
	int64_t _last_modified_time = 0;
	bool _needs_sync = false;
protected:
	static void _bind_methods();

public:
	static Ref<GDKGameSaveContainerInfo> create(const XGameSaveContainerInfo* info);

	inline String get_name() const { return _name; }
	inline String get_display_name() const { return _display_name; }
	inline int64_t get_blob_count() const { return (int64_t)_blob_count; }
	inline String get_total_size() const { return String::num_uint64(_total_size); }
	inline int64_t get_last_modified_time() const { return _last_modified_time; }
	inline bool get_needs_sync() const { return _needs_sync; }
};

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

	TypedArray<Ref<GDKGameSaveBlobInfo>> enumerate_blob_info() const;
	TypedArray<Ref<GDKGameSaveBlobInfo>> enumerate_blob_info_by_name(const String& prefix) const;

	Dictionary read_blob_data(TypedArray<Ref<GDKGameSaveBlobInfo>> blob_infos) const;
	Ref<GDKAsyncBlock> read_blob_data_async(PackedStringArray blobNames);
}; 
}