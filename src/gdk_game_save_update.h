#pragma once

#include <classes/ref_counted.hpp>
#include "gdk_asyncblock.h"
#include <XGameSave.h>

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
}