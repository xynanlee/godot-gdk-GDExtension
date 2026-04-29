#pragma once

#include <classes/ref_counted.hpp>
#include "gdk_user.h"
#include "gdk_asyncblock.h"

namespace godot {
class GDKGameSaveProvider;
class GDKGameSave : public RefCounted {
	GDCLASS(GDKGameSave, RefCounted)

protected:
	static void _bind_methods();

public:
	static Ref<GDKGameSaveProvider> initialize_game_save_provider(bool syncOnDemand, Ref<GDKUser> user);
	static Ref<GDKAsyncBlock> initialize_game_save_provider_async(bool syncOnDemand, Ref<GDKUser> user);
};
}