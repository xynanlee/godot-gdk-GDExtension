#pragma once

#include <Windows.h>
#include <winapifamily.h>
#include <objbase.h>
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/variant.hpp"
#include <XAsync.h>
#include <XTaskQueue.h>
#include <list>
#include <xsapi-c/services_c.h>
using namespace godot;

class gdk_achievement : public RefCounted {
    GDCLASS(gdk_achievement, RefCounted)

public:
	String id;
	String scid;
	String name;
	Array title_associations;
	Array media_assets;
	Array platforms;
	bool is_secret = false;
	String unlocked_description;
	String locked_description;
	String product_id;
	int type = 0;
	int participation_type = 0;
	Dictionary available;
	Array rewards;
	uint64_t estimated_unlock_time = 0;
	String deep_link;
	bool is_revoked = false;

    gdk_achievement() = default; // required by Godot

    gdk_achievement(const XblAchievement* src);

protected:
	static void _bind_methods();

public:
    // Getters Godot uses
    String get_id() const { return id; }
    String get_scid() const { return scid; }
    String get_name() const { return name; }
    bool get_is_secret() const { return is_secret; }
    String get_unlocked_desc() const { return unlocked_description; }
    String get_locked_desc() const { return locked_description; }
    String get_product_id() const { return product_id; }
    uint64_t get_estimated_time() const { return estimated_unlock_time; }
    String get_deep_link() const { return deep_link; }
    bool get_is_revoked() const { return is_revoked; }
};
