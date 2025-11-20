#include "gdk_achievement.h"

#include <xsapi-c/services_c.h>
using namespace godot;

gdk_achievement::gdk_achievement(const XblAchievement* src)
{
	if (!src) return;

	id = src->id;
	scid = src->serviceConfigurationId;
	name = src->name;

	for (size_t i = 0; i < src->titleAssociationsCount; i++) {
		const auto& t = src->titleAssociations[i];
		godot::Dictionary d;
		d["name"] = t.name;
		d["id"] = t.titleId;
		title_associations.push_back(d);
	}

	for (size_t i = 0; i < src->mediaAssetsCount; i++) {
		const auto& m = src->mediaAssets[i];
		godot::Dictionary d;
		d["name"] = m.name;
		d["type"] = static_cast<uint32_t>(m.mediaAssetType);
		d["url"] = m.url;
		media_assets.push_back(d);
	}

	for (size_t i = 0; i < src->platformsAvailableOnCount; i++) {
		platforms.push_back(godot::String(src->platformsAvailableOn[i]));
	}

	is_secret = src->isSecret;
	unlocked_description = src->unlockedDescription;
	locked_description = src->lockedDescription;
	product_id = src->productId;
	type = static_cast<int>(src->type);
	participation_type = static_cast<int>(src->participationType);
	estimated_unlock_time = src->estimatedUnlockTime;
	deep_link = src->deepLink;
	is_revoked = src->isRevoked;
}

void gdk_achievement::_bind_methods() {
	// Godot-visible properties via getters
	ClassDB::bind_method(D_METHOD("get_id"), &gdk_achievement::get_id);
	ClassDB::bind_method(D_METHOD("get_scid"), &gdk_achievement::get_scid);
	ClassDB::bind_method(D_METHOD("get_name"), &gdk_achievement::get_name);
	ClassDB::bind_method(D_METHOD("get_is_secret"), &gdk_achievement::get_is_secret);
	ClassDB::bind_method(D_METHOD("get_unlocked_desc"), &gdk_achievement::get_unlocked_desc);
	ClassDB::bind_method(D_METHOD("get_locked_desc"), &gdk_achievement::get_locked_desc);
	ClassDB::bind_method(D_METHOD("get_product_id"), &gdk_achievement::get_product_id);
	ClassDB::bind_method(D_METHOD("get_estimated_time"), &gdk_achievement::get_estimated_time);
	ClassDB::bind_method(D_METHOD("get_deep_link"), &gdk_achievement::get_deep_link);
	ClassDB::bind_method(D_METHOD("get_is_revoked"), &gdk_achievement::get_is_revoked);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "id"), "", "get_id");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "scid"), "", "get_scid");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "", "get_name");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_secret"), "", "get_is_secret");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "unlocked_desc"), "", "get_unlocked_desc");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "locked_desc"), "", "get_locked_desc");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "product_id"), "", "get_product_id");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "estimated_time"), "", "get_estimated_time");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "deep_link"), "", "get_deep_link");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_revoked"), "", "get_is_revoked");
}
