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
		static void _bind_methods()	{ }

	public:
		GDKXblAchievementTitleAssociation() = default; // required by Godot
		GDKXblAchievementTitleAssociation(XblAchievementTitleAssociation* src) {
			name = src->name;
			titleId = src->titleId;
		}

		String name;
		uint32_t titleId;
};

class GDKXblAchievementRequirement : public RefCounted {
	GDCLASS(GDKXblAchievementRequirement, RefCounted);

	protected:
		static void _bind_methods() { }

	public:
		GDKXblAchievementRequirement() = default; // required by Godot
		GDKXblAchievementRequirement(XblAchievementRequirement* src) {
			id = src->id;
			current_progress_value = src->currentProgressValue;
			target_progress_value = src->targetProgressValue;
		}

		String id;
		String current_progress_value;
		String target_progress_value;
};

class GDKXblAchievementProgression : public RefCounted {
	GDCLASS(GDKXblAchievementProgression, RefCounted);

	protected:
		static void _bind_methods() { }

	public:
		GDKXblAchievementProgression() = default; // required by Godot
		GDKXblAchievementProgression(XblAchievementProgression* src) {
			for(size_t i = 0; i < src->requirementsCount; i++) {
				GDKXblAchievementRequirement requirement(&src->requirements[i]);
				requirements.push_back(&requirement);
			}

			timeUnlocked = src->timeUnlocked;
		}
		Array requirements;
		time_t timeUnlocked;
};

class GDKXblAchievementTimeWindow : public RefCounted {
	GDCLASS(GDKXblAchievementTimeWindow, RefCounted);

	protected:
		static void _bind_methods() { }

	public:
		GDKXblAchievementTimeWindow() = default; // required by Godot
		GDKXblAchievementTimeWindow(XblAchievementTimeWindow* src) {
			startDate = src->startDate;
			endDate = src->endDate;
		}
		time_t startDate;
		time_t endDate;
};

class GDKXblAchievementMediaAsset : public RefCounted {
	GDCLASS(GDKXblAchievementMediaAsset, RefCounted);

	protected:
		static void _bind_methods() { }

	public:
		GDKXblAchievementMediaAsset() = default; // required by Godot
		GDKXblAchievementMediaAsset(XblAchievementMediaAsset* src) {
			name = src->name;
			media_asset_type = static_cast<GDKXblAchievementMediaAssetType::Enum>(src->mediaAssetType);
			url = src->url;
		}

		String name;
		GDKXblAchievementMediaAssetType::Enum media_asset_type;
		String url;
};

class GDKXblAchievementReward : public RefCounted {
	GDCLASS(GDKXblAchievementReward, RefCounted);

	protected:
		static void _bind_methods() { }

	public:
		GDKXblAchievementReward() = default; // required by Godot
		GDKXblAchievementReward(const XblAchievementReward* src) {
			name = src->name;
			description = src->description;
			value = src->value;
			rewardType = static_cast<GDKXblAchievementRewardType::Enum>(src->rewardType);
			valueType = src->valueType;
			mediaAsset = memnew(GDKXblAchievementMediaAsset(src->mediaAsset));
		}

		String name;
		String description;
		String value;
		GDKXblAchievementRewardType::Enum rewardType;
		String valueType;
		GDKXblAchievementMediaAsset* mediaAsset;
};

class GDKXblAchievementProgressChangeEntry  : public RefCounted {
	GDCLASS(GDKXblAchievementProgressChangeEntry, RefCounted);

	protected:
		static void _bind_methods() { }

	public:
		GDKXblAchievementProgressChangeEntry() = default; // required by Godot
		String achievementId;
		GDKXblAchievementProgressState progressState;
		GDKXblAchievementProgression progression;
};

class GDKAchievement : public RefCounted {
	GDCLASS(GDKAchievement, RefCounted)

	public:
		GDKAchievement() = default; // required by Godot
		GDKAchievement(const XblAchievement* src);

		String id;
		String scid;
		String name;
		Array title_associations;
		GDKXblAchievementProgressState::Enum progress_state;
		Ref<GDKXblAchievementProgression> progression;
		Array media_assets;
		Array platforms;
		bool is_secret = false;
		String unlocked_description;
		String locked_description;
		String product_id;
		GDKXblAchievementType::Enum type;
		GDKXblAchievementParticipationType::Enum participation_type;
		Dictionary available;
		Array rewards;
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
		Array get_title_association() const { return title_associations; }
		GDKXblAchievementProgressState::Enum get_progress_state() const { return progress_state; }
		Ref<GDKXblAchievementProgression> get_progression() const { return progression; }
		Array get_media_assets() const { return media_assets; }
		Array get_platforms() const { return platforms; }
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
