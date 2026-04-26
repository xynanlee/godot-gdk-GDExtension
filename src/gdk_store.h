#pragma once 

#include <classes/ref_counted.hpp>
#include "gdk_asyncblock.h"
#include "gdk_user.h"
#include <XStore.h>
#include <variant/typed_array.hpp>

namespace godot {
class GDKXStoreProductKind: public RefCounted {
    GDCLASS(GDKXStoreProductKind, RefCounted);

protected:
    static void _bind_methods();

public:
    enum Enum: uint32_t {
        None = XStoreProductKind::None,
        Consumable = XStoreProductKind::Consumable,
        Durable = XStoreProductKind::Durable,
        Game = XStoreProductKind::Game,
        Pass = XStoreProductKind::Pass,
        UnmanagedConsumable = XStoreProductKind::UnmanagedConsumable
    };
};

class GDKXStorePrice: public RefCounted {
    GDCLASS(GDKXStorePrice, RefCounted);

private:
    float _base_price;
    float _price;
    float _recurrence_price;
    String _currency_code;
    String _formatted_base_price;
    String _formatted_price;
    String _formatted_recurrence_price;
    bool _is_on_sale;
    int64_t _sale_end_date;

protected:
    static void _bind_methods();

public:
    static Ref<GDKXStorePrice> create(const XStorePrice* price);

    inline float get_base_price() const { return _base_price; }
    inline float get_price() const { return _price; }
    inline float get_recurrence_price() const { return _recurrence_price; }
    inline String get_currency_code() const { return _currency_code; }
    inline String get_formatted_base_price() const { return _formatted_base_price; }
    inline String get_formatted_price() const { return _formatted_price; }
    inline String get_formatted_recurrence_price() const { return _formatted_recurrence_price; }
    inline bool is_on_sale() const { return _is_on_sale; }
    inline int64_t get_sale_end_date() const { return _sale_end_date; }
};

class GDKXStoreCollectionData: public RefCounted {
    GDCLASS(GDKXStoreCollectionData, RefCounted);

private:
    int64_t _acquired_date;
    int64_t _start_date;
    int64_t _end_date;
    bool _is_trial;
    int64_t _trial_time_remaining_in_seconds;
    int64_t _quantity;
    String _campaign_id;
    String _developer_offer_id;

protected:
    static void _bind_methods();

public:
    static Ref<GDKXStoreCollectionData> create(const XStoreCollectionData* collection_data);

    inline int64_t get_acquired_date() const { return _acquired_date; }
    inline int64_t get_start_date() const { return _start_date; }
    inline int64_t get_end_date() const { return _end_date; }
    inline bool is_trial() const { return _is_trial; }
    inline int64_t get_trial_time_remaining_in_seconds() const { return _trial_time_remaining_in_seconds; }
    inline int64_t get_quantity() const { return _quantity; }
    inline String get_campaign_id() const { return _campaign_id; }
    inline String get_developer_offer_id() const { return _developer_offer_id; }
};

class GDKXStoreSubscriptionInfo: public RefCounted {
    GDCLASS(GDKXStoreSubscriptionInfo, RefCounted);

public:
    enum DurationUnit: uint32_t {
        Minute = XStoreDurationUnit::Minute,
        Hour = XStoreDurationUnit::Hour,
        Day = XStoreDurationUnit::Day,
        Week = XStoreDurationUnit::Week,
        Month = XStoreDurationUnit::Month,
        Year = XStoreDurationUnit::Year
    };

private:
    bool _has_trial_period;
    DurationUnit _trial_period_unit;
    int64_t _trial_period;
    DurationUnit _billing_period_unit;
    int64_t _billing_period;

protected:
    static void _bind_methods();

public:
    static Ref<GDKXStoreSubscriptionInfo> create(const XStoreSubscriptionInfo* subscription_info);

    inline bool has_trial_period() const { return _has_trial_period; }
    inline DurationUnit get_trial_period_unit() const { return _trial_period_unit; }
    inline int64_t get_trial_period() const { return _trial_period; }
    inline DurationUnit get_billing_period_unit() const { return _billing_period_unit; }
    inline int64_t get_billing_period() const { return _billing_period; }
};

class GDKXStoreImage: public RefCounted {
    GDCLASS(GDKXStoreImage, RefCounted);

private:
    String _uri;
    int64_t _height;
    int64_t _width;
    String _caption;
    String _image_purpose_tag;

protected:
    static void _bind_methods();

public:
    static Ref<GDKXStoreImage> create(const XStoreImage* image);

    inline String get_uri() const { return _uri; }
    inline int64_t get_height() const { return _height; }
    inline int64_t get_width() const { return _width; }
    inline String get_caption() const { return _caption; }
    inline String get_image_purpose_tag() const { return _image_purpose_tag; }
};

class GDKXStoreVideo: public RefCounted {
    GDCLASS(GDKXStoreVideo, RefCounted);

private:
    String _uri;
    int64_t _height;
    int64_t _width;
    String _caption;
    String _video_purpose_tag;
    Ref<GDKXStoreImage> _preview_image;

protected:
    static void _bind_methods();

public:
    static Ref<GDKXStoreVideo> create(const XStoreVideo* video);

    inline String get_uri() const { return _uri; }
    inline int64_t get_height() const { return _height; }
    inline int64_t get_width() const { return _width; }
    inline String get_caption() const { return _caption; }
    inline String get_video_purpose_tag() const { return _video_purpose_tag; }
    inline Ref<GDKXStoreImage> get_preview_image() const { return _preview_image; }
};

class GDKXStoreAvailability: public RefCounted {
    GDCLASS(GDKXStoreAvailability, RefCounted);

private:
    String _availability_id;
    Ref<GDKXStorePrice> _price;
    int64_t _end_date;

protected:
    static void _bind_methods();

public:
    static Ref<GDKXStoreAvailability> create(const XStoreAvailability* availability);

    inline String get_availability_id() const { return _availability_id; }
    inline Ref<GDKXStorePrice> get_price() const { return _price; }
    inline int64_t get_end_date() const { return _end_date; }
};

class GDKXStoreSku: public RefCounted {
    GDCLASS(GDKXStoreSku, RefCounted);

private:
    String _sku_id;
    String _title;
    String _description;
    String _language;
    Ref<GDKXStorePrice> _price;
    bool _is_trial;
    bool _is_in_user_collection;
    Ref<GDKXStoreCollectionData> _collection_data;
    bool _is_subscription;
    Ref<GDKXStoreSubscriptionInfo> _subscription_info;
    PackedStringArray _bundled_skus;
    TypedArray<Ref<GDKXStoreImage>> _images;
    TypedArray<Ref<GDKXStoreVideo>> _videos;
    TypedArray<Ref<GDKXStoreAvailability>> _availabilities;

protected:
    static void _bind_methods();

public:
    static Ref<GDKXStoreSku> create(const XStoreSku* sku);

    inline String get_sku_id() const { return _sku_id; }
    inline String get_title() const { return _title; }
    inline String get_description() const { return _description; }
    inline String get_language() const { return _language; }
    inline Ref<GDKXStorePrice> get_price() const { return _price; }
    inline bool is_trial() const { return _is_trial; }
    inline bool is_in_user_collection() const { return _is_in_user_collection; }
    inline Ref<GDKXStoreCollectionData> get_collection_data() const { return _collection_data; }
    inline bool is_subscription() const { return _is_subscription; }
    inline Ref<GDKXStoreSubscriptionInfo> get_subscription_info() const { return _subscription_info; }
    inline PackedStringArray get_bundled_skus() const { return _bundled_skus; }
    inline TypedArray<Ref<GDKXStoreImage>> get_images() const { return _images; }
    inline TypedArray<Ref<GDKXStoreVideo>> get_videos() const { return _videos; }
    inline TypedArray<Ref<GDKXStoreAvailability>> get_availabilities() const { return _availabilities; }
};

class GDKXStoreProduct: public RefCounted {
    GDCLASS(GDKXStoreProduct, RefCounted);

private:
    String _store_id;
    String _title;
    String _description;
    String _language;
    String _in_app_offer_token;
    String _link_uri;
    BitField<GDKXStoreProductKind::Enum> _product_kind = GDKXStoreProductKind::Enum::None;
    Ref<GDKXStorePrice> _price;
    bool _has_digital_download;
    bool _is_in_user_collection;
    PackedStringArray _keywords;
    TypedArray<Ref<GDKXStoreSku>> _skus;
    TypedArray<Ref<GDKXStoreImage>> _images;
    TypedArray<Ref<GDKXStoreVideo>> _videos;

protected:
    static void _bind_methods();

public:
    static Ref<GDKXStoreProduct> create(const XStoreProduct* product);
    inline String get_store_id() const { return _store_id; }
    inline String get_title() const { return _title; }
    inline String get_description() const { return _description; }
    inline String get_language() const { return _language; }
    inline String get_in_app_offer_token() const { return _in_app_offer_token; }
    inline String get_link_uri() const { return _link_uri; }
    inline BitField<GDKXStoreProductKind::Enum> get_product_kind() const { return _product_kind; }
    inline Ref<GDKXStorePrice> get_price() const { return _price; }
    inline bool has_digital_download() const { return _has_digital_download; }
    inline bool is_in_user_collection() const { return _is_in_user_collection; }
    inline PackedStringArray get_keywords() const { return _keywords; }
    inline TypedArray<Ref<GDKXStoreSku>> get_skus() const { return _skus; }
    inline TypedArray<Ref<GDKXStoreImage>> get_images() const { return _images; }
    inline TypedArray<Ref<GDKXStoreVideo>> get_videos() const { return _videos; }
};

class GDKXStoreProductQueryHandle: public RefCounted {
    GDCLASS(GDKXStoreProductQueryHandle, RefCounted);

private:
    XStoreProductQueryHandle _handle;

protected:
    static void _bind_methods();
    void _notification(int p_what);

public:
    static Ref<GDKXStoreProductQueryHandle> create(XStoreProductQueryHandle handle);
    bool has_more_pages() const;
    Ref<GDKAsyncBlock> next_page_async() const;
    int enumerate_products_query(const Callable& callback) const;
    inline XStoreProductQueryHandle get_handle() const { return _handle; }
};

class GDKXStoreCanLicenseStatus: public RefCounted {
    GDCLASS(GDKXStoreCanLicenseStatus, RefCounted);

protected:
    static void _bind_methods();

public:
    enum Enum: uint32_t {
        NotLicensableToUser = XStoreCanLicenseStatus::NotLicensableToUser,
        Licensable = XStoreCanLicenseStatus::Licensable,
        LicenseActionNotApplicableToProduct = XStoreCanLicenseStatus::LicenseActionNotApplicableToProduct
    };
};

class GDKXStoreLicenseHandle: public RefCounted {
    GDCLASS(GDKXStoreLicenseHandle, RefCounted);

private:
    XStoreLicenseHandle _handle = nullptr;
    XTaskQueueRegistrationToken _package_license_changed_token = {};
    
protected:
    static void _bind_methods();
    void _notification(int p_what);

public:
    static Ref<GDKXStoreLicenseHandle> create(XStoreLicenseHandle handle);
    bool is_valid_license() const;
};

class GDKStore : public RefCounted {
    GDCLASS(GDKStore, RefCounted);

private:
    XStoreContextHandle _handle = nullptr;
    XTaskQueueRegistrationToken _game_license_lost_token = {};

protected:
    static void _bind_methods();
    void _notification(int p_what);

public:
    static Ref<GDKStore> create_store_context(Ref<GDKUser> user);
    Ref<GDKAsyncBlock> acquire_license_for_durables_async(const String& store_id) const;
    Ref<GDKAsyncBlock> acquire_license_for_package_async(const String& package_identifier) const;
    Ref<GDKAsyncBlock> can_acquire_license_for_package_async(const String& package_identifier) const;
    Ref<GDKAsyncBlock> can_acquire_license_for_store_id_async(const String& store_id) const;

    Ref<GDKAsyncBlock> download_and_install_packages_async(const PackedStringArray& store_ids) const;
    Ref<GDKAsyncBlock> download_and_install_package_updates_async(const PackedStringArray& package_identifiers) const;
    Ref<GDKAsyncBlock> download_package_updates_async(const PackedStringArray& package_identifiers) const;

    Ref<GDKAsyncBlock> get_user_collections_id_async(const String& service_ticket, const String& publisher_user_id) const;
    Ref<GDKAsyncBlock> get_user_purchase_id_async(const String& service_ticket, const String& publisher_user_id) const;
    Ref<GDKAsyncBlock> query_add_on_licenses_async() const;
    Ref<GDKAsyncBlock> query_associated_products_async(BitField<GDKXStoreProductKind::Enum> product_kinds, int32_t max_results) const;
    Ref<GDKAsyncBlock> query_associated_products_for_store_id_async(const String& store_id, BitField<GDKXStoreProductKind::Enum> product_kinds, int32_t max_results) const;
    Ref<GDKAsyncBlock> query_consumable_balance_remaining_async(const String& store_product_id) const;
    Ref<GDKAsyncBlock> query_entitled_products_async(BitField<GDKXStoreProductKind::Enum> product_kinds, int max_items_to_retrieve_per_page) const;
    Ref<GDKAsyncBlock> query_game_and_dlc_package_updates_async() const;
    Ref<GDKAsyncBlock> query_game_license_async() const;
    Ref<GDKAsyncBlock> query_license_token_async(const PackedStringArray& product_ids, const String& custom_developer_string) const;
    String query_package_identifier(const String& store_id) const;
    Ref<GDKAsyncBlock> query_package_updates_async(const PackedStringArray& package_identifiers) const;
    Ref<GDKAsyncBlock> query_product_for_current_game_async() const;
    Ref<GDKAsyncBlock> query_product_for_package_async(BitField<GDKXStoreProductKind::Enum> product_kinds, const String& package_identifier) const;
    Ref<GDKAsyncBlock> query_products_async(BitField<GDKXStoreProductKind::Enum> product_kinds, const PackedStringArray& store_ids, const PackedStringArray& action_filters) const;
    Ref<GDKAsyncBlock> report_consumable_fulfillment_async(const String& store_product_id, int quantity, const String& tracking_id) const;
    Ref<GDKAsyncBlock> show_associated_products_ui_async(const String& store_id, BitField<GDKXStoreProductKind::Enum> product_kinds) const;
    Ref<GDKAsyncBlock> show_gifting_ui_async(const String& store_id, const String& name = "", const String& extended_json_data = "") const;
    Ref<GDKAsyncBlock> show_product_page_ui_async(const String& store_id) const;
    Ref<GDKAsyncBlock> show_purchase_ui_async(const String& store_id, const String& name = "", const String& extended_json_data = "") const;
    Ref<GDKAsyncBlock> show_rate_and_review_ui_async() const;
    Ref<GDKAsyncBlock> show_redeem_token_ui_async(const String& token, bool disallow_csv_redemption, const PackedStringArray& allowed_store_ids = PackedStringArray()) const;
};
}

VARIANT_ENUM_CAST(GDKXStoreCanLicenseStatus::Enum);
VARIANT_BITFIELD_CAST(GDKXStoreProductKind::Enum);
VARIANT_ENUM_CAST(GDKXStoreSubscriptionInfo::DurationUnit);