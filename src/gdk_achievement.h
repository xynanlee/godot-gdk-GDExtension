#pragma once

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/typed_array.hpp"
#include "godot_cpp/variant/typed_dictionary.hpp"
#include "godot_cpp/variant/variant.hpp"
#include <xsapi-c/services_c.h>

namespace godot {
class GDKXblAchievementType : public Object {
	GDCLASS(GDKXblAchievementType, Object);

	protected:
		static void _bind_methods();

	public:
		enum Enum: uint32_t {
			Unknown,
			All,
			Persistent,
			Challenge
		};
};

class GDKXblAchievementOrderBy : public Object {
	GDCLASS(GDKXblAchievementOrderBy, Object);

	protected:
		static void _bind_methods();

	public:
	enum Enum: uint32_t {
			DefaultOrder,
			TitleId,
			UnlockTime
		};
};

class GDKXblAchievementProgressState : public Object {
	GDCLASS(GDKXblAchievementProgressState, Object);

	protected:
		static void _bind_methods();

	public:
		enum Enum: uint32_t {
			Unknown,
			Achieved,
			NotStarted,
			InProgress
		};
};

class GDKXblAchievementMediaAssetType : public Object {
	GDCLASS(GDKXblAchievementMediaAssetType, Object);

	protected:
		static void _bind_methods();

	public:
		enum Enum: uint32_t {
			Unknown,
			Icon,
			Art
		};
};

class GDKXblAchievementParticipationType : public Object {
	GDCLASS(GDKXblAchievementParticipationType, Object);

	protected:
		static void _bind_methods();

	public:
		enum Enum: uint32_t {
			Unknown,
			Individual,
			Group
		};
};

class GDKXblAchievementRewardType : public Object {
	GDCLASS(GDKXblAchievementRewardType, Object);

	protected:
		static void _bind_methods();

	public:
		enum Enum: uint32_t {
			Unknown,
			Gamerscore,
			InApp,
			Art
		};
};

class GDKXblAchievementRarityCategory : public Object {
	GDCLASS(GDKXblAchievementRarityCategory, Object);

	protected:
		static void _bind_methods();

	public:
		enum Enum: uint32_t {
			Unset = 0,
			Rare = 1,
			Common = 2
		};
};

class GDKXblAchievementTitleAssociation : public RefCounted {
	GDCLASS(GDKXblAchievementTitleAssociation, RefCounted);

	protected:
	static void _bind_methods() {
			ClassDB::bind_method(D_METHOD("get_name"), &GDKXblAchievementTitleAssociation::get_name);
			ClassDB::bind_method(D_METHOD("get_title_id"), &GDKXblAchievementTitleAssociation::get_title_id);

			ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "", "get_name");
			ADD_PROPERTY(PropertyInfo(Variant::INT, "title_id"), "", "get_title_id");
		}

	public:
		GDKXblAchievementTitleAssociation() = default; // required by Godot
		GDKXblAchievementTitleAssociation(XblAchievementTitleAssociation src) {
			name = src.name;
			title_id = src.titleId;
		}

		String name;
		uint32_t title_id;

		String get_name() const { return name; }
		uint32_t get_title_id() const { return title_id; }
};

class GDKXblAchievementRequirement : public RefCounted {
	GDCLASS(GDKXblAchievementRequirement, RefCounted);

	protected:
		static void _bind_methods() {
			ClassDB::bind_method(D_METHOD("get_id"), &GDKXblAchievementRequirement::get_id);
			ClassDB::bind_method(D_METHOD("get_current_progress_value"), &GDKXblAchievementRequirement::get_current_progress_value);
			ClassDB::bind_method(D_METHOD("get_target_progress_value"), &GDKXblAchievementRequirement::get_target_progress_value);

			ADD_PROPERTY(PropertyInfo(Variant::STRING, "id"), "", "get_id");
			ADD_PROPERTY(PropertyInfo(Variant::STRING, "current_progress_value"), "", "get_current_progress_value");
			ADD_PROPERTY(PropertyInfo(Variant::STRING, "target_progress_value"), "", "get_target_progress_value");
		}

	public:
		GDKXblAchievementRequirement() = default; // required by Godot
		GDKXblAchievementRequirement(XblAchievementRequirement src) {
			id = src.id;
			current_progress_value = src.currentProgressValue;
			target_progress_value = src.targetProgressValue;
		}

		String id;
		String current_progress_value;
		String target_progress_value;

		String get_id() const { return id; }
		String get_current_progress_value() const { return current_progress_value; }
		String get_target_progress_value() const { return target_progress_value; }
};

class GDKXblAchievementProgression : public RefCounted {
	GDCLASS(GDKXblAchievementProgression, RefCounted);

	protected:
		static void _bind_methods() {
			ClassDB::bind_method(D_METHOD("get_requirements"), &GDKXblAchievementProgression::get_requirements);
			ClassDB::bind_method(D_METHOD("get_time_unlocked"), &GDKXblAchievementProgression::get_time_unlocked);

			ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "requirements"), "", "get_requirements");
			ADD_PROPERTY(PropertyInfo(Variant::INT, "time_unlocked"), "", "get_time_unlocked");
		}

	public:
		GDKXblAchievementProgression() = default; // required by Godot
		GDKXblAchievementProgression(XblAchievementProgression src) {
			for(size_t i = 0; i < src.requirementsCount; i++) {
				Ref<GDKXblAchievementRequirement> requirement = memnew(GDKXblAchievementRequirement(src.requirements[i]));
				requirements.push_back(requirement);
			}

			time_unlocked = src.timeUnlocked;
		}
		TypedArray<Ref<GDKXblAchievementRequirement>> requirements;
		int64_t time_unlocked;

		TypedArray<Ref<GDKXblAchievementRequirement>> get_requirements() const { return requirements; }
		int64_t get_time_unlocked() const { return time_unlocked; }
};

class GDKXblAchievementTimeWindow : public RefCounted {
	GDCLASS(GDKXblAchievementTimeWindow, RefCounted);

	protected:
		static void _bind_methods() {
			ClassDB::bind_method(D_METHOD("get_startDate"), &GDKXblAchievementTimeWindow::get_startDate);
			ClassDB::bind_method(D_METHOD("get_endDate"), &GDKXblAchievementTimeWindow::get_endDate);

			ADD_PROPERTY(PropertyInfo(Variant::INT, "start_date"), "", "get_startDate");
			ADD_PROPERTY(PropertyInfo(Variant::INT, "end_date"), "", "get_endDate");
		}

	public:
		GDKXblAchievementTimeWindow() = default; // required by Godot
		GDKXblAchievementTimeWindow(const XblAchievementTimeWindow src) {
			start_date = src.startDate;
			end_date = src.endDate;
		}
		int64_t start_date;
		int64_t end_date;

		int64_t get_startDate() const { return start_date; }
		int64_t get_endDate() const { return end_date; }
};

class GDKXblAchievementMediaAsset : public RefCounted {
	GDCLASS(GDKXblAchievementMediaAsset, RefCounted);

	protected:
		static void _bind_methods() {
			ClassDB::bind_method(D_METHOD("get_name"), &GDKXblAchievementMediaAsset::get_name);
			ClassDB::bind_method(D_METHOD("get_media_asset_type"), &GDKXblAchievementMediaAsset::get_media_asset_type);
			ClassDB::bind_method(D_METHOD("get_url"), &GDKXblAchievementMediaAsset::get_url);

			ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "", "get_name");
			ADD_PROPERTY(PropertyInfo(Variant::INT, "media_asset_type"), "", "get_media_asset_type");
			ADD_PROPERTY(PropertyInfo(Variant::STRING, "url"), "", "get_url");
		}

	public:
		GDKXblAchievementMediaAsset() = default; // required by Godot
		GDKXblAchievementMediaAsset(XblAchievementMediaAsset src) {
			name = src.name;
			media_asset_type = static_cast<GDKXblAchievementMediaAssetType::Enum>(src.mediaAssetType);
			url = src.url;
		}

		String name;
		GDKXblAchievementMediaAssetType::Enum media_asset_type;
		String url;

		String get_name () const { return name; }
		GDKXblAchievementMediaAssetType::Enum get_media_asset_type () const { return media_asset_type; }
		String get_url () const { return url; }
};

class GDKXblAchievementReward : public RefCounted {
	GDCLASS(GDKXblAchievementReward, RefCounted);

	protected:
		static void _bind_methods() {
			ClassDB::bind_method(D_METHOD("get_name"), &GDKXblAchievementReward::get_name);
			ClassDB::bind_method(D_METHOD("get_description"), &GDKXblAchievementReward::get_description);
			ClassDB::bind_method(D_METHOD("get_value"), &GDKXblAchievementReward::get_value);
			ClassDB::bind_method(D_METHOD("get_reward_type"), &GDKXblAchievementReward::get_reward_type);
			ClassDB::bind_method(D_METHOD("get_value_type"), &GDKXblAchievementReward::get_value_type);
			ClassDB::bind_method(D_METHOD("get_media_asset"), &GDKXblAchievementReward::get_media_asset);

			ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "", "get_name");
			ADD_PROPERTY(PropertyInfo(Variant::STRING, "description"), "", "get_description");
			ADD_PROPERTY(PropertyInfo(Variant::STRING, "value"), "", "get_value");
			ADD_PROPERTY(PropertyInfo(Variant::INT, "reward_type"), "", "get_reward_type");
			ADD_PROPERTY(PropertyInfo(Variant::STRING, "value_type"), "", "get_value_type");
			ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "media_asset"), "", "get_media_asset");
		}

	public:
		GDKXblAchievementReward() = default; // required by Godot
		GDKXblAchievementReward(const XblAchievementReward src) {
			name = src.name;
			description = src.description;
			value = src.value;
			reward_type = static_cast<GDKXblAchievementRewardType::Enum>(src.rewardType);
			value_type = src.valueType;
			media_asset = Ref<GDKXblAchievementMediaAsset>(memnew(GDKXblAchievementMediaAsset(*src.mediaAsset)));
		}

		String name;
		String description;
		String value;
		GDKXblAchievementRewardType::Enum reward_type;
		String value_type;
		Ref<GDKXblAchievementMediaAsset> media_asset;

		String get_name () const { return name; }
		String get_description () const { return description; }
		String get_value () const { return value; }
		GDKXblAchievementRewardType::Enum get_reward_type () const { return reward_type; }
		String get_value_type () const { return value_type; }
		Ref<GDKXblAchievementMediaAsset> get_media_asset () const { return media_asset; }
};

class GDKXblAchievementProgressChangeEntry  : public RefCounted {
	GDCLASS(GDKXblAchievementProgressChangeEntry, RefCounted);

	protected:
		static void _bind_methods() {
			ClassDB::bind_method(D_METHOD("get_achievement_id"), &GDKXblAchievementProgressChangeEntry::get_achievement_id);
			ClassDB::bind_method(D_METHOD("get_progress_state"), &GDKXblAchievementProgressChangeEntry::get_progress_state);
			ClassDB::bind_method(D_METHOD("get_progression"), &GDKXblAchievementProgressChangeEntry::get_progression);

			ADD_PROPERTY(PropertyInfo(Variant::STRING, "achievement_id"), "", "get_achievement_id");
			ADD_PROPERTY(PropertyInfo(Variant::INT, "progress_state"), "", "get_progress_state");
			ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "progression"), "", "get_progression");
		}

	public:
		GDKXblAchievementProgressChangeEntry() = default; // required by Godot
		String achievement_id;
		GDKXblAchievementProgressState::Enum progress_state;
		Ref<GDKXblAchievementProgression> progression;

		String get_achievement_id () const { return achievement_id; };
		GDKXblAchievementProgressState::Enum get_progress_state () const { return progress_state; };
		Ref<GDKXblAchievementProgression> get_progression () const { return progression; };
};

class GDKAchievement : public RefCounted {
	GDCLASS(GDKAchievement, RefCounted)

	public:
		GDKAchievement() = default; // required by Godot
		GDKAchievement(const XblAchievement src);

		String id;
		String scid;
		String name;
		TypedArray<Ref<GDKXblAchievementTitleAssociation>> title_associations;
		GDKXblAchievementProgressState::Enum progress_state;
		Ref<GDKXblAchievementProgression> progression;
		TypedArray<Ref<GDKXblAchievementMediaAsset>> media_assets;
		TypedArray<String> platforms;
		bool is_secret = false;
		String unlocked_description;
		String locked_description;
		String product_id;
		GDKXblAchievementType::Enum type;
		GDKXblAchievementParticipationType::Enum participation_type;
		Ref<GDKXblAchievementTimeWindow> available;
		TypedArray<Ref<GDKXblAchievementReward>> rewards;
		uint64_t estimated_unlock_time = 0;
		String deep_link;
		bool is_revoked = false;

	protected:
		static void _bind_methods();

	public:
		// Getters Godot uses
		String get_id() const { return id; }
		String get_scid() const { return scid; }
		String get_name() const { return name; }
		TypedArray<Ref<GDKXblAchievementTitleAssociation>> get_title_association() const { return title_associations; }
		GDKXblAchievementProgressState::Enum get_progress_state() const { return progress_state; }
		Ref<GDKXblAchievementProgression> get_progression() const { return progression; }
		TypedArray<Ref<GDKXblAchievementMediaAsset>> get_media_assets() const { return media_assets; }
		TypedArray<String> get_platforms() const { return platforms; }
		bool get_is_secret() const { return is_secret; }
		String get_unlocked_desc() const { return unlocked_description; }
		String get_locked_desc() const { return locked_description; }
		String get_product_id() const { return product_id; }
		GDKXblAchievementType::Enum get_type() const { return type; }
		GDKXblAchievementParticipationType::Enum get_participation_type() const { return participation_type; }
		uint64_t get_estimated_time() const { return estimated_unlock_time; }
		String get_deep_link() const { return deep_link; }
		bool get_is_revoked() const { return is_revoked; }
};
}


VARIANT_ENUM_CAST(GDKXblAchievementType::Enum);
VARIANT_ENUM_CAST(GDKXblAchievementOrderBy::Enum);
VARIANT_ENUM_CAST(GDKXblAchievementProgressState::Enum);
VARIANT_ENUM_CAST(GDKXblAchievementMediaAssetType::Enum);
VARIANT_ENUM_CAST(GDKXblAchievementParticipationType::Enum);
VARIANT_ENUM_CAST(GDKXblAchievementRewardType::Enum);
VARIANT_ENUM_CAST(GDKXblAchievementRarityCategory::Enum);
