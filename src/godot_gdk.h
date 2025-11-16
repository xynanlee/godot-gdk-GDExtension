#pragma once

#include <Windows.h>
#include <winapifamily.h>
#include <objbase.h>
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/variant.hpp"
#include <XAsync.h>
#include <XTaskQueue.h>

using namespace godot;

class godot_gdk : public RefCounted {
	GDCLASS(godot_gdk, RefCounted)

private:
	HRESULT Identity_TrySignInDefaultUserSilently(XTaskQueueHandle asyncQueue) const;
	static void Identity_TrySignInDefaultUserSilently_Callback(XAsyncBlock *asyncBlock);

protected:
	static void _bind_methods();

public:
	godot_gdk() = default;
	~godot_gdk() override = default;

	void print_type(const Variant &p_variant) const;
	int InitializeGDK(const String SCID);
};
