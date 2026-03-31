#pragma once

#include <Windows.h>
#include <winapifamily.h>
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/variant.hpp"
#include <XGameSave.h>
#include <list>

using namespace godot;

class GDKGameSaveBlobInfo : public RefCounted {
	GDCLASS(GDKGameSaveBlobInfo, RefCounted)

protected:
	static void _bind_methods();

public:
	GDKGameSaveBlobInfo() = default;
	void setInfo(const XGameSaveBlobInfo* src);

	~GDKGameSaveBlobInfo() override = default;

	String name;
	uint32_t size = 0;

    String get_name() const { return name; }
    uint32_t get_size() const { return size; }
};
