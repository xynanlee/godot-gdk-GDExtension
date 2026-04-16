#pragma once

#include <Windows.h>
#include <classes/ref_counted.hpp>
#include <XGameSave.h>
#include <variant/typed_dictionary.hpp>
#include <variant/typed_array.hpp>
#include "gdk_user.h"
#include "gdk_asyncblock.h"
#include "gdk_macros.h"

namespace godot {

//x This is an RAII container, it autocloses the handle when its freed.
class GDKGameSaveUpdate: public RefCounted {
	GDCLASS(GDKGameSaveUpdate, RefCounted);

private:
	XGameSaveUpdateHandle _update_handle = nullptr;

protected:
	static void _bind_methods();
	virtual void _notification(int p_what);

public:
	static Ref<GDKGameSaveUpdate> create(XGameSaveUpdateHandle updateHandle);
	int delete_blob(const String& blobName) const;
	int write_blob(const String& blobName, PackedByteArray data) const;
	int submit_update() const;

	Ref<GDKAsyncBlock> submit_update_async() const;
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
	Ref<GDKGameSaveUpdate> create_update();
	inline String get_name() const { return _name; }

	TypedDictionary<String, int> enumerate_blob_info() const;
	TypedDictionary<String, int> enumerate_blob_info_by_name(const String& prefix) const;

	Dictionary read_blob_data(PackedStringArray blobNames, int blobSize);
	Ref<GDKAsyncBlock> read_blob_data_async(PackedStringArray blobNames);
}; 

//x This is an RAII container, it autocloses the handle when its freed.
class GDKGameSaveProvider: public RefCounted {
	GDCLASS(GDKGameSaveProvider, RefCounted);

private:
	XGameSaveProviderHandle _save_provider_handle;

protected:
	static void _bind_methods();
	virtual void _notification(int p_what);

public:
	static Ref<GDKGameSaveProvider> create(XGameSaveProviderHandle in_handle);
	Ref<GDKGameSaveContainer> create_container(const String& containerName);
	void delete_container(const String& containerName);
	Ref<GDKAsyncBlock> delete_container_async(const String& containerName);

	Dictionary enumerate_container_info() const;
	Dictionary enumerate_container_info_by_name(const String& prefix) const;
	Dictionary get_container_info(const String& containerName) const;
	int64_t get_remaining_quota() const;
	Ref<GDKAsyncBlock> get_remaining_quota_async() const;
};

class GDKGameSave : public RefCounted {
	GDCLASS(GDKGameSave, RefCounted)

protected:
	static void _bind_methods();

public:
	GDKGameSave() = default;

	static Ref<GDKGameSaveProvider> initialize_game_save_provider(bool syncOnDemand, Ref<GDKUser> user);
	static Ref<GDKAsyncBlock> initialize_game_save_provider_async(bool syncOnDemand, Ref<GDKUser> user);
};
}