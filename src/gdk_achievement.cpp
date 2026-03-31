#include "gdk_achievement.h"

#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/dictionary.hpp"
#include "godot_cpp/variant/variant.hpp"

#include <xsapi-c/services_c.h>

namespace godot
{
	void GDKXblAchievementType::_bind_methods() {
		BIND_ENUM_CONSTANT(Unknown);
		BIND_ENUM_CONSTANT(All);
		BIND_ENUM_CONSTANT(Persistent);
		BIND_ENUM_CONSTANT(Challenge);
	}

	void GDKXblAchievementOrderBy::_bind_methods() {
		BIND_ENUM_CONSTANT(DefaultOrder);
		BIND_ENUM_CONSTANT(TitleId);
		BIND_ENUM_CONSTANT(UnlockTime);
	}

	void GDKXblAchievementProgressState::_bind_methods() {
		BIND_ENUM_CONSTANT(Unknown);
		BIND_ENUM_CONSTANT(Achieved);
		BIND_ENUM_CONSTANT(NotStarted);
		BIND_ENUM_CONSTANT(InProgress);
	}

	void GDKXblAchievementMediaAssetType::_bind_methods() {
		BIND_ENUM_CONSTANT(Unknown);
		BIND_ENUM_CONSTANT(Icon);
		BIND_ENUM_CONSTANT(Art);
	}

	void GDKXblAchievementParticipationType::_bind_methods() {
		BIND_ENUM_CONSTANT(Unknown);
		BIND_ENUM_CONSTANT(Individual);
		BIND_ENUM_CONSTANT(Group);
	}

	void GDKXblAchievementRewardType::_bind_methods() {
		BIND_ENUM_CONSTANT(Unknown);
		BIND_ENUM_CONSTANT(Gamerscore);
		BIND_ENUM_CONSTANT(InApp);
		BIND_ENUM_CONSTANT(Art);
	}

	void GDKXblAchievementRarityCategory::_bind_methods() {
		BIND_ENUM_CONSTANT(Unset);
		BIND_ENUM_CONSTANT(Rare);
		BIND_ENUM_CONSTANT(Common);
	}

	GDKAchievement::GDKAchievement(const XblAchievement src)
	{
		id = src.id;
		scid = src.serviceConfigurationId;
		name = src.name;

		for (size_t i = 0; i < src.titleAssociationsCount; i++) {
			Ref<GDKXblAchievementTitleAssociation> titleAssociationInfo = memnew(GDKXblAchievementTitleAssociation(src.titleAssociations[i]));
			title_associations.push_back(titleAssociationInfo);
		}

		progress_state = static_cast<GDKXblAchievementProgressState::Enum>(src.progressState);

		progression.instantiate();
		progression->time_unlocked = src.progression.timeUnlocked;

		for(size_t i = 0; i < src.progression.requirementsCount; i++) {
			Ref<GDKXblAchievementRequirement> requirement = memnew(GDKXblAchievementRequirement(src.progression.requirements[i]));
			progression->requirements.push_back(requirement);
		}

		for (size_t i = 0; i < src.mediaAssetsCount; i++) {
			Ref<GDKXblAchievementMediaAsset> mediaAssetInfo = memnew(GDKXblAchievementMediaAsset(src.mediaAssets[i]));
			media_assets.push_back(mediaAssetInfo);
		}

		for (size_t i = 0; i < src.platformsAvailableOnCount; i++) {
			String platform = String(src.platformsAvailableOn[i]);
			platforms.push_back(platform);
		}

		is_secret = src.isSecret;
		unlocked_description = src.unlockedDescription;
		locked_description = src.lockedDescription;
		product_id = src.productId;
		type = static_cast<GDKXblAchievementType::Enum>(src.type);
		participation_type = static_cast<GDKXblAchievementParticipationType::Enum>(src.participationType);
		available = Ref<GDKXblAchievementTimeWindow>(memnew(GDKXblAchievementTimeWindow(src.available)));

		for (size_t i = 0; i < src.rewardsCount; i++) {
			Ref<GDKXblAchievementReward> reward = memnew(GDKXblAchievementReward(src.rewards[i]));
			rewards.push_back(reward);
		}

		estimated_unlock_time = src.estimatedUnlockTime;
		deep_link = src.deepLink;
		is_revoked = src.isRevoked;
	}

	void GDKAchievement::_bind_methods () {
		// Godot-visible properties via getters
		ClassDB::bind_method(D_METHOD("get_id"), &GDKAchievement::get_id);
		ClassDB::bind_method(D_METHOD("get_scid"), &GDKAchievement::get_scid);
		ClassDB::bind_method(D_METHOD("get_name"), &GDKAchievement::get_name);
		ClassDB::bind_method(D_METHOD("get_title_association"), &GDKAchievement::get_title_association);
		ClassDB::bind_method(D_METHOD("get_progress_state"), &GDKAchievement::get_progress_state);
		ClassDB::bind_method(D_METHOD("get_progression"), &GDKAchievement::get_progression);
		ClassDB::bind_method(D_METHOD("get_media_assets"), &GDKAchievement::get_media_assets);
		ClassDB::bind_method(D_METHOD("get_platforms"), &GDKAchievement::get_platforms);
		ClassDB::bind_method(D_METHOD("get_is_secret"), &GDKAchievement::get_is_secret);
		ClassDB::bind_method(D_METHOD("get_unlocked_desc"), &GDKAchievement::get_unlocked_desc);
		ClassDB::bind_method(D_METHOD("get_locked_desc"), &GDKAchievement::get_locked_desc);
		ClassDB::bind_method(D_METHOD("get_product_id"), &GDKAchievement::get_product_id);
		ClassDB::bind_method(D_METHOD("get_type"), &GDKAchievement::get_type);
		ClassDB::bind_method(D_METHOD("get_participation_type"), &GDKAchievement::get_participation_type);
		ClassDB::bind_method(D_METHOD("get_estimated_time"), &GDKAchievement::get_estimated_time);
		ClassDB::bind_method(D_METHOD("get_deep_link"), &GDKAchievement::get_deep_link);
		ClassDB::bind_method(D_METHOD("get_is_revoked"), &GDKAchievement::get_is_revoked);

		ADD_PROPERTY(PropertyInfo(Variant::STRING, "id"), "", "get_id");
		ADD_PROPERTY(PropertyInfo(Variant::STRING, "scid"), "", "get_scid");
		ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "", "get_name");
		ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "title_association"), "", "get_title_association");
		ADD_PROPERTY(PropertyInfo(Variant::INT, "progress_state"), "", "get_progress_state");
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "progression"), "", "get_progression");
		ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "media_assets"), "", "get_media_assets");
		ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "platforms"), "", "get_platforms");
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_secret"), "", "get_is_secret");
		ADD_PROPERTY(PropertyInfo(Variant::STRING, "unlocked_desc"), "", "get_unlocked_desc");
		ADD_PROPERTY(PropertyInfo(Variant::STRING, "locked_desc"), "", "get_locked_desc");
		ADD_PROPERTY(PropertyInfo(Variant::STRING, "product_id"), "", "get_product_id");
		ADD_PROPERTY(PropertyInfo(Variant::INT, "type"), "", "get_type");
		ADD_PROPERTY(PropertyInfo(Variant::INT, "participation_type"), "", "get_participation_type");
		ADD_PROPERTY(PropertyInfo(Variant::INT, "estimated_time"), "", "get_estimated_time");
		ADD_PROPERTY(PropertyInfo(Variant::STRING, "deep_link"), "", "get_deep_link");
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_revoked"), "", "get_is_revoked");

		GDREGISTER_CLASS(GDKXblAchievementType);
		GDREGISTER_CLASS(GDKXblAchievementOrderBy);
		GDREGISTER_CLASS(GDKXblAchievementProgressState);
		GDREGISTER_CLASS(GDKXblAchievementMediaAssetType);
		GDREGISTER_CLASS(GDKXblAchievementParticipationType);
		GDREGISTER_CLASS(GDKXblAchievementRewardType);
		GDREGISTER_CLASS(GDKXblAchievementRarityCategory);
		GDREGISTER_CLASS(GDKXblAchievementTitleAssociation);
		GDREGISTER_CLASS(GDKXblAchievementRequirement);
		GDREGISTER_CLASS(GDKXblAchievementProgression);
		GDREGISTER_CLASS(GDKXblAchievementTimeWindow);
		GDREGISTER_CLASS(GDKXblAchievementMediaAsset);
		GDREGISTER_CLASS(GDKXblAchievementReward);
		GDREGISTER_CLASS(GDKXblAchievementProgressChangeEntry);
	}
}
