#include "gdk_helpers.h"
#include "godot_gdk.h"

using namespace godot;

XTaskQueueHandle GDKHelpers::get_async_queue() {
	return GodotGDK::get_singleton()->get_async_queue();
}