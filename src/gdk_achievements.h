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

class gdk_achievements : public RefCounted {
	GDCLASS(gdk_achievements, RefCounted)

	static void CALLBACK GetAchievementsCallback(_Inout_ XAsyncBlock* asyncBlock);
protected:
	static void _bind_methods();

public:
	gdk_achievements() = default;
	~gdk_achievements() override = default;

	void GetAchievements();
};
