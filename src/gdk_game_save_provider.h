#pragma once

#include <classes/ref_counted.hpp>
#include <variant/typed_array.hpp>
#include "gdk_asyncblock.h"
#include <XGameSave.h>

namespace godot {
class GDKGameSaveContainer;
class GDKGameSaveContainerInfo;

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

	TypedArray<Ref<GDKGameSaveContainerInfo>> enumerate_container_info() const;
	TypedArray<Ref<GDKGameSaveContainerInfo>> enumerate_container_info_by_name(const String& prefix) const;
	Ref<GDKGameSaveContainerInfo> get_container_info(const String& containerName) const;
	int64_t get_remaining_quota() const;
	Ref<GDKAsyncBlock> get_remaining_quota_async() const;
};
}