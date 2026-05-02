#include "gdk_store.h"
#include "gdk_helpers.h"
#include <templates/local_vector.hpp>

using namespace godot;

void GDKXStoreSku::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_sku_id"), &GDKXStoreSku::get_sku_id);
   ClassDB::bind_method(D_METHOD("get_title"), &GDKXStoreSku::get_title);
   ClassDB::bind_method(D_METHOD("get_description"), &GDKXStoreSku::get_description);
   ClassDB::bind_method(D_METHOD("get_language"), &GDKXStoreSku::get_language);
   ClassDB::bind_method(D_METHOD("get_price"), &GDKXStoreSku::get_price);
   ClassDB::bind_method(D_METHOD("is_trial"), &GDKXStoreSku::is_trial);
   ClassDB::bind_method(D_METHOD("is_in_user_collection"), &GDKXStoreSku::is_in_user_collection);
   ClassDB::bind_method(D_METHOD("get_collection_data"), &GDKXStoreSku::get_collection_data);
   ClassDB::bind_method(D_METHOD("is_subscription"), &GDKXStoreSku::is_subscription);
   ClassDB::bind_method(D_METHOD("get_subscription_info"), &GDKXStoreSku::get_subscription_info);
   ClassDB::bind_method(D_METHOD("get_bundled_skus"), &GDKXStoreSku::get_bundled_skus);
   ClassDB::bind_method(D_METHOD("get_images"), &GDKXStoreSku::get_images);
   ClassDB::bind_method(D_METHOD("get_videos"), &GDKXStoreSku::get_videos);
   ClassDB::bind_method(D_METHOD("get_availabilities"), &GDKXStoreSku::get_availabilities);

   ADD_PROPERTY(PropertyInfo(Variant::STRING, "sku_id"), "", "get_sku_id");
   ADD_PROPERTY(PropertyInfo(Variant::STRING, "title"), "", "get_title");
   ADD_PROPERTY(PropertyInfo(Variant::STRING, "description"), "", "get_description");
   ADD_PROPERTY(PropertyInfo(Variant::STRING, "language"), "", "get_language");
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "price", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT, "GDKXStorePrice"), "", "get_price"); 
   ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_trial"), "", "is_trial");
   ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_in_user_collection"), "", "is_in_user_collection");
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "collection_data", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT, "GDKXStoreCollectionData"), "", "get_collection_data");
   ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_subscription"), "", "is_subscription");
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "subscription_info", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT, "GDKXStoreSubscriptionInfo"), "", "get_subscription_info");
   ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "bundled_skus"), "", "get_bundled_skus");
   ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "images", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT, "GDKXStoreImage"), "", "get_images");
   ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "videos", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT, "GDKXStoreVideo"), "", "get_videos");
   ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "availability", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT, "GDKXStoreAvailability"), "", "get_availabilities");
}

Ref<GDKXStoreSku> GDKXStoreSku::create(const XStoreSku *sku) {
	Ref<GDKXStoreSku> store_sku;
    store_sku.instantiate();
    store_sku->_sku_id = String(sku->skuId);
    store_sku->_title = String(sku->title);
    store_sku->_description = String(sku->description);
    store_sku->_language = String(sku->language);
    store_sku->_price = GDKXStorePrice::create(&sku->price);
    store_sku->_is_trial = sku->isTrial;
    store_sku->_is_in_user_collection = sku->isInUserCollection;
    store_sku->_collection_data = GDKXStoreCollectionData::create(&sku->collectionData);
    store_sku->_is_subscription = sku->isSubscription;
    store_sku->_subscription_info = GDKXStoreSubscriptionInfo::create(&sku->subscriptionInfo);

    PackedStringArray bundled_skus;
    for (uint32_t i = 0; i < sku->bundledSkusCount; i++) {
        bundled_skus.push_back(String(sku->bundledSkus[i]));
    }
    store_sku->_bundled_skus = bundled_skus;

    TypedArray<Ref<GDKXStoreImage>> images;
    for (uint32_t i = 0; i < sku->imagesCount; i++) {
        Ref<GDKXStoreImage> image = GDKXStoreImage::create(&sku->images[i]);
        images.push_back(image);
    }
    store_sku->_images = images;

    TypedArray<Ref<GDKXStoreVideo>> videos;
    for (uint32_t i = 0; i < sku->videosCount; i++) {
        Ref<GDKXStoreVideo> video = GDKXStoreVideo::create(&sku->videos[i]);
        videos.push_back(video);
    }
    store_sku->_videos = videos;

    TypedArray<Ref<GDKXStoreAvailability>> availabilities;
    for (uint32_t i = 0; i < sku->availabilitiesCount; i++) {
        Ref<GDKXStoreAvailability> availability = GDKXStoreAvailability::create(&sku->availabilities[i]);
        availabilities.push_back(availability);
    }
    store_sku->_availabilities = availabilities;
    return store_sku;
}

void GDKXStoreAvailability::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_availability_id"), &GDKXStoreAvailability::get_availability_id);
    ClassDB::bind_method(D_METHOD("get_price"), &GDKXStoreAvailability::get_price);
    ClassDB::bind_method(D_METHOD("get_end_date"), &GDKXStoreAvailability::get_end_date);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "availability_id"), "", "get_availability_id");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "price", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT, "GDKXStorePrice"), "", "get_price");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "end_date"), "", "get_end_date");
}

Ref<GDKXStoreAvailability> GDKXStoreAvailability::create(const XStoreAvailability *availability) {
	Ref<GDKXStoreAvailability> store_availability;
    store_availability.instantiate();
    store_availability->_availability_id = String(availability->availabilityId);
    store_availability->_price = GDKXStorePrice::create(&availability->price);
    store_availability->_end_date = availability->endDate;
    return store_availability;
}

void GDKXStoreImage::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_uri"), &GDKXStoreImage::get_uri);
    ClassDB::bind_method(D_METHOD("get_height"), &GDKXStoreImage::get_height);
    ClassDB::bind_method(D_METHOD("get_width"), &GDKXStoreImage::get_width);
    ClassDB::bind_method(D_METHOD("get_caption"), &GDKXStoreImage::get_caption);
    ClassDB::bind_method(D_METHOD("get_image_purpose_tag"), &GDKXStoreImage::get_image_purpose_tag);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "uri"), "", "get_uri");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "height"), "", "get_height");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "width"), "", "get_width");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "caption"), "", "get_caption");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "image_purpose_tag"), "", "get_image_purpose_tag");
}

Ref<GDKXStoreImage> GDKXStoreImage::create(const XStoreImage *image) {
	Ref<GDKXStoreImage> store_image;
    store_image.instantiate();
    store_image->_uri = String(image->uri);
    store_image->_height = image->height;
    store_image->_width = image->width;
    store_image->_caption = String(image->caption);
    store_image->_image_purpose_tag = String(image->imagePurposeTag);
    return store_image;
}

void GDKXStoreVideo::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_uri"), &GDKXStoreVideo::get_uri);
    ClassDB::bind_method(D_METHOD("get_height"), &GDKXStoreVideo::get_height);
    ClassDB::bind_method(D_METHOD("get_width"), &GDKXStoreVideo::get_width);
    ClassDB::bind_method(D_METHOD("get_caption"), &GDKXStoreVideo::get_caption);
    ClassDB::bind_method(D_METHOD("get_video_purpose_tag"), &GDKXStoreVideo::get_video_purpose_tag);
    ClassDB::bind_method(D_METHOD("get_preview_image"), &GDKXStoreVideo::get_preview_image);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "uri"), "", "get_uri");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "height"), "", "get_height");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "width"), "", "get_width");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "caption"), "", "get_caption");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "video_purpose_tag"), "", "get_video_purpose_tag");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "preview_image", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT, "GDKXStoreImage"), "", "get_preview_image");
}

Ref<GDKXStoreVideo> GDKXStoreVideo::create(const XStoreVideo *video) {
	Ref<GDKXStoreVideo> store_video;
    store_video.instantiate();
    store_video->_uri = String(video->uri);
    store_video->_height = video->height;
    store_video->_width = video->width;
    store_video->_caption = String(video->caption);
    store_video->_video_purpose_tag = String(video->videoPurposeTag);
    store_video->_preview_image = GDKXStoreImage::create(&video->previewImage);
    return store_video;
}

void GDKXStoreSubscriptionInfo::_bind_methods() {
    BIND_ENUM_CONSTANT(Minute);
    BIND_ENUM_CONSTANT(Hour);
    BIND_ENUM_CONSTANT(Day);
    BIND_ENUM_CONSTANT(Week);
    BIND_ENUM_CONSTANT(Month);
    BIND_ENUM_CONSTANT(Year);


    ClassDB::bind_method(D_METHOD("has_trial_period"), &GDKXStoreSubscriptionInfo::has_trial_period);
    ClassDB::bind_method(D_METHOD("get_trial_period_unit"), &GDKXStoreSubscriptionInfo::get_trial_period_unit);
    ClassDB::bind_method(D_METHOD("get_trial_period"), &GDKXStoreSubscriptionInfo::get_trial_period);
    ClassDB::bind_method(D_METHOD("get_billing_period_unit"), &GDKXStoreSubscriptionInfo::get_billing_period_unit);
    ClassDB::bind_method(D_METHOD("get_billing_period"), &GDKXStoreSubscriptionInfo::get_billing_period);

    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "has_trial_period"), "", "has_trial_period");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "trial_period_unit", PROPERTY_HINT_ENUM, "Minute,Hour,Day,Week,Month,Year", PROPERTY_USAGE_DEFAULT, "GDKXStoreSubscriptionInfo::DurationUnit"), "", "get_trial_period_unit");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "trial_period"), "", "get_trial_period");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "billing_period_unit", PROPERTY_HINT_ENUM, "Minute,Hour,Day,Week,Month,Year", PROPERTY_USAGE_DEFAULT, "GDKXStoreSubscriptionInfo::DurationUnit"), "", "get_billing_period_unit");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "billing_period"), "", "get_billing_period");
}

Ref<GDKXStoreSubscriptionInfo> GDKXStoreSubscriptionInfo::create(const XStoreSubscriptionInfo *subscription_info) {
	Ref<GDKXStoreSubscriptionInfo> store_subscription;
    store_subscription.instantiate();
    store_subscription->_has_trial_period = subscription_info->hasTrialPeriod;
    store_subscription->_trial_period_unit = (GDKXStoreSubscriptionInfo::DurationUnit)subscription_info->trialPeriodUnit;
    store_subscription->_trial_period = subscription_info->trialPeriod;
    store_subscription->_billing_period_unit = (GDKXStoreSubscriptionInfo::DurationUnit)subscription_info->billingPeriodUnit;
    store_subscription->_billing_period = subscription_info->billingPeriod;
    return store_subscription;
}

void GDKXStoreCollectionData::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_acquired_date"), &GDKXStoreCollectionData::get_acquired_date);
    ClassDB::bind_method(D_METHOD("get_start_date"), &GDKXStoreCollectionData::get_start_date);
    ClassDB::bind_method(D_METHOD("get_end_date"), &GDKXStoreCollectionData::get_end_date);
    ClassDB::bind_method(D_METHOD("is_trial"), &GDKXStoreCollectionData::is_trial);
    ClassDB::bind_method(D_METHOD("get_trial_time_remaining_in_seconds"), &GDKXStoreCollectionData::get_trial_time_remaining_in_seconds);
    ClassDB::bind_method(D_METHOD("get_quantity"), &GDKXStoreCollectionData::get_quantity);
    ClassDB::bind_method(D_METHOD("get_campaign_id"), &GDKXStoreCollectionData::get_campaign_id);
    ClassDB::bind_method(D_METHOD("get_developer_offer_id"), &GDKXStoreCollectionData::get_developer_offer_id);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "acquired_date"), "", "get_acquired_date");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "start_date"), "", "get_start_date");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "end_date"), "", "get_end_date");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_trial"), "", "is_trial");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "trial_time_remaining_in_seconds"), "", "get_trial_time_remaining_in_seconds");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "quantity"), "", "get_quantity");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "campaign_id"), "", "get_campaign_id");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "developer_offer_id"), "", "get_developer_offer_id");
}

Ref<GDKXStoreCollectionData> GDKXStoreCollectionData::create(const XStoreCollectionData *collection_data) {
	Ref<GDKXStoreCollectionData> store_collection;
    store_collection.instantiate();
    store_collection->_acquired_date = collection_data->acquiredDate;
    store_collection->_start_date = collection_data->startDate;
    store_collection->_end_date = collection_data->endDate;
    store_collection->_is_trial = collection_data->isTrial;
    store_collection->_trial_time_remaining_in_seconds = collection_data->trialTimeRemainingInSeconds;
    store_collection->_quantity = collection_data->quantity;
    store_collection->_campaign_id = String(collection_data->campaignId);
    store_collection->_developer_offer_id = String(collection_data->developerOfferId);
    return store_collection;
}

void GDKXStorePrice::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_base_price"), &GDKXStorePrice::get_base_price);
    ClassDB::bind_method(D_METHOD("get_price"), &GDKXStorePrice::get_price);
    ClassDB::bind_method(D_METHOD("get_recurrence_price"), &GDKXStorePrice::get_recurrence_price);
    ClassDB::bind_method(D_METHOD("get_currency_code"), &GDKXStorePrice::get_currency_code);
    ClassDB::bind_method(D_METHOD("get_formatted_base_price"), &GDKXStorePrice::get_formatted_base_price);
    ClassDB::bind_method(D_METHOD("get_formatted_price"), &GDKXStorePrice::get_formatted_price);
    ClassDB::bind_method(D_METHOD("get_formatted_recurrence_price"), &GDKXStorePrice::get_formatted_recurrence_price);
    ClassDB::bind_method(D_METHOD("is_on_sale"), &GDKXStorePrice::is_on_sale);
    ClassDB::bind_method(D_METHOD("get_sale_end_date"), &GDKXStorePrice::get_sale_end_date);
    
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "base_price"), "", "get_base_price");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "price"), "", "get_price");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "recurrence_price"), "", "get_recurrence_price");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "currency_code"), "", "get_currency_code");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "formatted_base_price"), "", "get_formatted_base_price");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "formatted_price"), "", "get_formatted_price");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "formatted_recurrence_price"), "", "get_formatted_recurrence_price");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_on_sale"), "", "is_on_sale");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "sale_end_date"), "", "get_sale_end_date");
}

Ref<GDKXStorePrice> GDKXStorePrice::create(const XStorePrice *price) {
	Ref<GDKXStorePrice> store_price;
    store_price.instantiate();
    store_price->_base_price = price->basePrice;
    store_price->_price = price->price;
    store_price->_recurrence_price = price->recurrencePrice;
    store_price->_currency_code = String(price->currencyCode);
    store_price->_formatted_base_price = String(price->formattedBasePrice);
    store_price->_formatted_price = String(price->formattedPrice);
    store_price->_formatted_recurrence_price = String(price->formattedRecurrencePrice);
    store_price->_is_on_sale = price->isOnSale;
    store_price->_sale_end_date = price->saleEndDate;
    return store_price;
}

void GDKXStoreProduct::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_store_id"), &GDKXStoreProduct::get_store_id);
    ClassDB::bind_method(D_METHOD("get_title"), &GDKXStoreProduct::get_title);
    ClassDB::bind_method(D_METHOD("get_description"), &GDKXStoreProduct::get_description);
    ClassDB::bind_method(D_METHOD("get_language"), &GDKXStoreProduct::get_language);
    ClassDB::bind_method(D_METHOD("get_in_app_offer_token"), &GDKXStoreProduct::get_in_app_offer_token);
    ClassDB::bind_method(D_METHOD("get_link_uri"), &GDKXStoreProduct::get_link_uri);
    ClassDB::bind_method(D_METHOD("get_product_kind"), &GDKXStoreProduct::get_product_kind);
    ClassDB::bind_method(D_METHOD("get_price"), &GDKXStoreProduct::get_price);
    ClassDB::bind_method(D_METHOD("has_digital_download"), &GDKXStoreProduct::has_digital_download);
    ClassDB::bind_method(D_METHOD("is_in_user_collection"), &GDKXStoreProduct::is_in_user_collection);
    ClassDB::bind_method(D_METHOD("get_keywords"), &GDKXStoreProduct::get_keywords);
    ClassDB::bind_method(D_METHOD("get_skus"), &GDKXStoreProduct::get_skus);
    ClassDB::bind_method(D_METHOD("get_images"), &GDKXStoreProduct::get_images);
    ClassDB::bind_method(D_METHOD("get_videos"), &GDKXStoreProduct::get_videos);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "store_id"), "", "get_store_id");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "title"), "", "get_title");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "description"), "", "get_description");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "language"), "", "get_language");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "in_app_offer_token"), "", "get_in_app_offer_token");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "link_uri"), "", "get_link_uri");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "product_kind", PROPERTY_HINT_ENUM, "None,Consumable,Durable,Game,Pass,UnmanagedConsumable", PROPERTY_USAGE_DEFAULT, "GDKXStoreProductKind::Enum"), "", "get_product_kind");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "price", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT, "GDKXStorePrice"), "", "get_price");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "has_digital_download"), "", "has_digital_download");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_in_user_collection"), "", "is_in_user_collection");
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "keywords"), "", "get_keywords");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "skus", PROPERTY_HINT_ARRAY_TYPE, "", PROPERTY_USAGE_DEFAULT, "GDKXStoreSku"), "", "get_skus");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "images", PROPERTY_HINT_ARRAY_TYPE, "", PROPERTY_USAGE_DEFAULT, "GDKXStoreImage"), "", "get_images");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "videos", PROPERTY_HINT_ARRAY_TYPE, "", PROPERTY_USAGE_DEFAULT, "GDKXStoreVideo"), "", "get_videos");
}

Ref<GDKXStoreProduct> GDKXStoreProduct::create(const XStoreProduct *product) {
    Ref<GDKXStoreProduct> store_product;
    store_product.instantiate();
    store_product->_store_id = String(product->storeId);
    store_product->_title = String(product->title);
    store_product->_description = String(product->description);
    store_product->_language = String(product->language);
    store_product->_in_app_offer_token = String(product->inAppOfferToken);
    store_product->_link_uri = String(product->linkUri);
    store_product->_product_kind = (GDKXStoreProductKind::Enum)product->productKind;
    store_product->_price = GDKXStorePrice::create(&product->price);
    store_product->_has_digital_download = product->hasDigitalDownload;
    store_product->_is_in_user_collection = product->isInUserCollection;
    
    PackedStringArray keywords;
    for (uint32_t i = 0; i < product->keywordsCount; i++) {
        keywords.push_back(String(product->keywords[i]));
    }
    store_product->_keywords = keywords;

    TypedArray<Ref<GDKXStoreSku>> skus;
    for (uint32_t i = 0; i < product->skusCount; i++) {
        skus.push_back(GDKXStoreSku::create(&product->skus[i]));
    }
    store_product->_skus = skus;

    TypedArray<Ref<GDKXStoreImage>> images;
    for (uint32_t i = 0; i < product->imagesCount; i++) {
        images.push_back(GDKXStoreImage::create(&product->images[i]));
    }
    store_product->_images = images;

    TypedArray<Ref<GDKXStoreVideo>> videos;
    for (uint32_t i = 0; i < product->videosCount; i++) {
        videos.push_back(GDKXStoreVideo::create(&product->videos[i]));
    }
    store_product->_videos = videos;
	return store_product;
}

void GDKXStoreProductQueryHandle::_bind_methods() {
    ClassDB::bind_method(D_METHOD("has_more_pages"), &GDKXStoreProductQueryHandle::has_more_pages);
    ClassDB::bind_method(D_METHOD("next_page_async"), &GDKXStoreProductQueryHandle::next_page_async);
    ClassDB::bind_method(D_METHOD("enumerate_products_query", "callback"), &GDKXStoreProductQueryHandle::enumerate_products_query);
}

void GDKXStoreProductQueryHandle::_notification(int p_what) {
	if (p_what == NOTIFICATION_PREDELETE) {
        if (_handle) {
            XStoreCloseProductsQueryHandle(_handle);
            _handle = nullptr;
        }
    }
}

Ref<GDKXStoreProductQueryHandle> GDKXStoreProductQueryHandle::create(XStoreProductQueryHandle handle) {
	Ref<GDKXStoreProductQueryHandle> wrapper;
    if (handle) {
        wrapper.instantiate();
        wrapper->_handle = handle;
    }
    return wrapper;
}

bool GDKXStoreProductQueryHandle::has_more_pages() const {
	return XStoreProductsQueryHasMorePages(_handle);
}

Ref<GDKAsyncBlock> GDKXStoreProductQueryHandle::next_page_async() const {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        Dictionary return_data;

        XStoreProductQueryHandle handle = nullptr;
        HRESULT hr = XStoreProductsQueryNextPageResult(async, &handle);
        return_data["hresult"] = hr;
        if (SUCCEEDED(hr)) {
            return_data["handle"] = GDKXStoreProductQueryHandle::create(handle);
        } else {
            XStoreCloseProductsQueryHandle(handle);
        }
        wrapper->emit(return_data);
    });

    HRESULT hr = XStoreProductsQueryNextPageAsync(_handle, asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreProductsQueryNextPageAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

int GDKXStoreProductQueryHandle::enumerate_products_query(const Callable &callback) const {
    Callable* hCallback = memnew(Callable(callback));
    HRESULT hr = XStoreEnumerateProductsQuery(_handle, hCallback,
                    [](const XStoreProduct* product, void* context) -> bool {
                        Callable* hCallback = reinterpret_cast<Callable*>(context);
                        Ref<GDKXStoreProduct> p = GDKXStoreProduct::create(product);
                        return hCallback->call(p);
                    });
    ERR_FAIL_COND_V_MSG(FAILED(hr), (int64_t)hr, vformat("XStoreEnumerateProductsQuery Error: 0x%08ux", (uint64_t)hr));
    return (int64_t)hr;
}

void GDKXStoreProductKind::_bind_methods() {
    BIND_BITFIELD_FLAG(None);
    BIND_BITFIELD_FLAG(Consumable);
    BIND_BITFIELD_FLAG(Durable);
    BIND_BITFIELD_FLAG(Game);
    BIND_BITFIELD_FLAG(Pass);
    BIND_BITFIELD_FLAG(UnmanagedConsumable);
}

void GDKXStoreCanLicenseStatus::_bind_methods() {
    BIND_ENUM_CONSTANT(NotLicensableToUser);
    BIND_ENUM_CONSTANT(Licensable);
    BIND_ENUM_CONSTANT(LicenseActionNotApplicableToProduct);
}

void GDKXStoreLicenseHandle::_bind_methods() {
    ADD_SIGNAL(MethodInfo("package_license_lost"));
}

void GDKXStoreLicenseHandle::_notification(int p_what) {
    if (p_what == NOTIFICATION_PREDELETE) {
        if (_package_license_changed_token.token != 0) {
            XStoreUnregisterPackageLicenseLost(_handle, _package_license_changed_token, false);
        }

        if (_handle) {
            XStoreCloseLicenseHandle(_handle);
            _handle = nullptr;
        }
    }
}

void PackageLicenseLostCallback(void* context) {
    GDKXStoreLicenseHandle* wrapper = reinterpret_cast<GDKXStoreLicenseHandle*>(context);
    wrapper->call_deferred("emit_signal", "package_license_lost");
}

Ref<GDKXStoreLicenseHandle> GDKXStoreLicenseHandle::create(XStoreLicenseHandle handle) {
    Ref<GDKXStoreLicenseHandle> wrapper;
    if (handle) {
        wrapper.instantiate();
        wrapper->_handle = handle;

        HRESULT hr = XStoreRegisterPackageLicenseLost(handle, GDKHelpers::get_async_queue(), wrapper.ptr(), PackageLicenseLostCallback, &wrapper->_package_license_changed_token);
        ERR_FAIL_COND_V_MSG(FAILED(hr), wrapper, vformat("XStoreRegisterPackageLicenseLost Error: 0x%08ux", (uint64_t)hr));
    }

	return wrapper;
}

bool GDKXStoreLicenseHandle::is_valid_license() const {
	return XStoreIsLicenseValid(_handle);
}

void GDKStore::_bind_methods() {
    ADD_SIGNAL(MethodInfo("game_license_changed"));

    ClassDB::bind_static_method(get_class_static(), D_METHOD("create_store_context", "user"), &GDKStore::create_store_context);
    ClassDB::bind_method(D_METHOD("acquire_license_for_durables_async", "store_id"), &GDKStore::acquire_license_for_durables_async);
    ClassDB::bind_method(D_METHOD("acquire_license_for_package_async", "package_identifier"), &GDKStore::acquire_license_for_package_async);
    ClassDB::bind_method(D_METHOD("can_acquire_license_for_package_async", "package_identifier"), &GDKStore::can_acquire_license_for_package_async);
    ClassDB::bind_method(D_METHOD("can_acquire_license_for_store_id_async", "store_id"), &GDKStore::can_acquire_license_for_store_id_async);
    ClassDB::bind_method(D_METHOD("download_and_install_packages_async", "store_ids"), &GDKStore::download_and_install_packages_async);
    ClassDB::bind_method(D_METHOD("download_and_install_package_updates_async", "package_identifiers"), &GDKStore::download_and_install_package_updates_async);
    ClassDB::bind_method(D_METHOD("download_package_updates_async", "package_identifiers"), &GDKStore::download_package_updates_async);
    ClassDB::bind_method(D_METHOD("get_user_collections_id_async", "service_ticket", "publisher_user_id"), &GDKStore::get_user_collections_id_async);
    ClassDB::bind_method(D_METHOD("get_user_purchase_id_async", "service_ticket", "publisher_user_id"), &GDKStore::get_user_purchase_id_async);
    ClassDB::bind_method(D_METHOD("query_add_on_licenses_async"), &GDKStore::query_add_on_licenses_async);
    ClassDB::bind_method(D_METHOD("query_associated_products_async", "product_kinds", "max_results"), &GDKStore::query_associated_products_async);
    ClassDB::bind_method(D_METHOD("query_associated_products_for_store_id_async", "store_id", "product_kinds", "max_results"), &GDKStore::query_associated_products_for_store_id_async);
    ClassDB::bind_method(D_METHOD("query_consumable_balance_remaining_async", "store_product_id"), &GDKStore::query_consumable_balance_remaining_async);
    ClassDB::bind_method(D_METHOD("query_entitled_products_async", "product_kinds", "max_items_to_retrieve_per_page"), &GDKStore::query_entitled_products_async);
    ClassDB::bind_method(D_METHOD("query_game_and_dlc_package_updates_async"), &GDKStore::query_game_and_dlc_package_updates_async);
    ClassDB::bind_method(D_METHOD("query_game_license_async"), &GDKStore::query_game_license_async);
    ClassDB::bind_method(D_METHOD("query_license_token_async", "product_ids", "custom_developer_string"), &GDKStore::query_license_token_async);
    ClassDB::bind_method(D_METHOD("query_package_identifier", "store_id"), &GDKStore::query_package_identifier);
    ClassDB::bind_method(D_METHOD("query_package_updates_async", "package_identifiers"), &GDKStore::query_package_updates_async);
    ClassDB::bind_method(D_METHOD("query_product_for_current_game_async"), &GDKStore::query_product_for_current_game_async);
    ClassDB::bind_method(D_METHOD("query_product_for_package_async", "product_kinds", "package_identifier"), &GDKStore::query_product_for_package_async);
    ClassDB::bind_method(D_METHOD("query_products_async", "product_kinds", "store_ids", "action_filters"), &GDKStore::query_products_async);
    ClassDB::bind_method(D_METHOD("report_consumable_fulfillment_async", "store_product_id", "quantity", "tracking_id"), &GDKStore::report_consumable_fulfillment_async);
    ClassDB::bind_method(D_METHOD("show_associated_products_ui_async", "store_id", "product_kinds"), &GDKStore::show_associated_products_ui_async);
    ClassDB::bind_method(D_METHOD("show_gifting_ui_async", "store_id", "name", "extended_json_data"), &GDKStore::show_gifting_ui_async);
    ClassDB::bind_method(D_METHOD("show_product_page_ui_async", "store_id"), &GDKStore::show_product_page_ui_async);
    ClassDB::bind_method(D_METHOD("show_purchase_ui_async", "store_id", "name", "extended_json_data"), &GDKStore::show_purchase_ui_async);
    ClassDB::bind_method(D_METHOD("show_rate_and_review_ui_async"), &GDKStore::show_rate_and_review_ui_async);
    ClassDB::bind_method(D_METHOD("show_redeem_token_ui_async", "token", "disallow_csv_redemption", "allowed_store_ids"), &GDKStore::show_redeem_token_ui_async);

	GDREGISTER_CLASS(GDKXStoreProductKind);
	GDREGISTER_CLASS(GDKXStorePrice);
	GDREGISTER_CLASS(GDKXStoreCollectionData);
	GDREGISTER_CLASS(GDKXStoreSubscriptionInfo);
	GDREGISTER_CLASS(GDKXStoreImage);
	GDREGISTER_CLASS(GDKXStoreVideo);
	GDREGISTER_CLASS(GDKXStoreAvailability);
	GDREGISTER_CLASS(GDKXStoreSku);
	GDREGISTER_CLASS(GDKXStoreProduct);
	GDREGISTER_CLASS(GDKXStoreProductQueryHandle);
	GDREGISTER_CLASS(GDKXStoreCanLicenseStatus);
	GDREGISTER_CLASS(GDKXStoreLicenseHandle);
    GDREGISTER_CLASS(GDKXStoreGameLicense);
}

void GDKStore::_notification(int p_what) {
	if (p_what == NOTIFICATION_PREDELETE) {
        if (_game_license_lost_token.token != 0) {
            XStoreUnregisterGameLicenseChanged(_handle, _game_license_lost_token, false);
        }

        if (_handle) {
            XStoreCloseContextHandle(_handle);
            _handle = nullptr;
        }
    }
}

void GameLicenseChangedCallback(void* context) {
    GDKStore* wrapper = reinterpret_cast<GDKStore*>(context);
    wrapper->call_deferred("emit_signal", "game_license_changed");
}

Ref<GDKStore> GDKStore::create_store_context(Ref<GDKUser> user) {
	Ref<GDKStore> wrapper;
    if (user.is_valid()) {
        XStoreContextHandle handle = nullptr;
        HRESULT hr = XStoreCreateContext(user->get_user(), &handle);
        ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreCreateContext Error: 0x%08ux", (uint64_t)hr));
        wrapper.instantiate();
        wrapper->_handle = handle;

        if (SUCCEEDED(hr)) {
            XTaskQueueHandle queue = GDKHelpers::get_async_queue();
            hr = XStoreRegisterGameLicenseChanged(handle, queue, wrapper.ptr(), GameLicenseChangedCallback, &wrapper->_game_license_lost_token);
            ERR_FAIL_COND_V_MSG(FAILED(hr), wrapper, vformat("XStoreRegisterGameLicenseChanged Error: 0x%08ux", (uint64_t)hr));
        }        
    }
	return wrapper;
}

Ref<GDKAsyncBlock> GDKStore::acquire_license_for_durables_async(const String &store_id) const {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        
        XStoreLicenseHandle license = nullptr;
        HRESULT hr = XStoreAcquireLicenseForDurablesResult(async, &license);
        Dictionary return_data;
        return_data["hresult"] = hr;
        if (SUCCEEDED(hr)) {
            return_data["license"] = GDKXStoreLicenseHandle::create(license);
        }
        wrapper->emit(return_data);
    });

    HRESULT hr = XStoreAcquireLicenseForDurablesAsync(_handle, store_id.utf8().get_data(), asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreAcquireLicenseForDurablesAsync Error: 0x%08ux", (uint64_t)hr));
	return asyncBlock;
}

Ref<GDKAsyncBlock> GDKStore::acquire_license_for_package_async(const String &package_identifier) const {
    Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);

        XStoreLicenseHandle license = nullptr;
        HRESULT hr = XStoreAcquireLicenseForPackageResult(async, &license);
        Dictionary return_data;
        return_data["hresult"] = hr;

        if (SUCCEEDED(hr)) {
            return_data["license"] = GDKXStoreLicenseHandle::create(license);
        }
        wrapper->emit(return_data);
    });

    HRESULT hr = XStoreAcquireLicenseForPackageAsync(_handle, package_identifier.utf8().get_data(), asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreAcquireLicenseForPackageAsync Error: 0x%08ux", (uint64_t)hr));
	return asyncBlock;
}

Ref<GDKAsyncBlock> GDKStore::can_acquire_license_for_package_async(const String &package_identifier) const {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);

        Dictionary return_data;
        XStoreCanAcquireLicenseResult result {};
        HRESULT hr = XStoreCanAcquireLicenseForPackageResult(async, &result);
        return_data["hresult"] = hr;

        if (SUCCEEDED(hr)) {
            return_data["status"] = (GDKXStoreCanLicenseStatus::Enum)result.status;
            return_data["licensable_sku"] = String(result.licensableSku);
        }
        wrapper->emit(return_data);
    });

    HRESULT hr = XStoreCanAcquireLicenseForPackageAsync(_handle, package_identifier.utf8().get_data(), asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreCanAcquireLicenseForPackageAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

Ref<GDKAsyncBlock> GDKStore::can_acquire_license_for_store_id_async(const String &store_id) const {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);

        Dictionary return_data;
        XStoreCanAcquireLicenseResult result {};
        HRESULT hr = XStoreCanAcquireLicenseForStoreIdResult(async, &result);
        return_data["hresult"] = hr;

        if (SUCCEEDED(hr)) {
            return_data["status"] = (GDKXStoreCanLicenseStatus::Enum)result.status;
            return_data["licensable_sku"] = String(result.licensableSku);
        }
        wrapper->emit(return_data);
    });

    HRESULT hr = XStoreCanAcquireLicenseForStoreIdAsync(_handle, store_id.utf8().get_data(), asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreCanAcquireLicenseForStoreIdAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

Ref<GDKAsyncBlock> GDKStore::download_and_install_packages_async(const PackedStringArray& store_ids) const {
    Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        uint32_t result_count = 0;
        Dictionary return_data;
        HRESULT hr = XStoreDownloadAndInstallPackagesResultCount(async, &result_count);
        return_data["hresult"] = hr;
        if (FAILED(hr)) {
            wrapper->emit(return_data);
            return;
        }

        auto packageIdentifiers = new char[result_count][XPACKAGE_IDENTIFIER_MAX_LENGTH];
        hr = XStoreDownloadAndInstallPackagesResult(async, result_count, &packageIdentifiers[0]);

        if (SUCCEEDED(hr)) {
            PackedStringArray return_strings;
            for (uint32_t i = 0; i < result_count; i++) {
                const char* s = packageIdentifiers[i];
                return_strings.push_back(String(s));
            }

            return_data["package_identifiers"] = return_strings;
        }

        delete[] packageIdentifiers;
        return_data["hresult"] = hr;
        wrapper->emit(return_data);
    });

    LocalVector<const char*> ids;
    ids.reserve(store_ids.size());
    for (int i = 0; i < store_ids.size(); i++) {
        ids.push_back(store_ids[i].utf8().get_data());
    }

    HRESULT hr = XStoreDownloadAndInstallPackagesAsync( _handle, 
                                                        (ids.size() > 0) ? ids.ptr() : nullptr, 
                                                        ids.size(), 
                                                        asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreDownloadAndInstallPackagesAsync Error: 0x%08ux", (uint64_t)hr));
	return asyncBlock;
}

Ref<GDKAsyncBlock> GDKStore::download_and_install_package_updates_async(const PackedStringArray& package_identifiers) const {
    Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        HRESULT hr = XStoreDownloadAndInstallPackageUpdatesResult(async);
        Dictionary return_data;
        return_data["hresult"] = hr;
        wrapper->emit(return_data);
    });

    LocalVector<const char*> ids;
    ids.reserve(package_identifiers.size());
    for (int i = 0; i < package_identifiers.size(); i++) {
        ids.push_back(package_identifiers[i].utf8().get_data());
    }
    HRESULT hr = XStoreDownloadAndInstallPackageUpdatesAsync(   _handle, 
                                                                (ids.size() > 0) ? ids.ptr() : nullptr, 
                                                                ids.size(), 
                                                                asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreDownloadAndInstallPackageUpdatesAsync Error: 0x%08ux", (uint64_t)hr));
	return asyncBlock;
}

Ref<GDKAsyncBlock> GDKStore::download_package_updates_async(const PackedStringArray &package_identifiers) const {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        HRESULT hr = XStoreDownloadPackageUpdatesResult(async);
        Dictionary return_data;
        return_data["hresult"] = hr;
        wrapper->emit(return_data);
    });
    LocalVector<const char*> ids;
    ids.reserve(package_identifiers.size());
    for (int i = 0; i < package_identifiers.size(); i++) {
        ids.push_back(package_identifiers[i].utf8().get_data());
    }
    HRESULT hr = XStoreDownloadPackageUpdatesAsync( _handle, 
                                                    (ids.size() > 0) ? ids.ptr() : nullptr, 
                                                    ids.size(), 
                                                    asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreDownloadPackageUpdatesAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

Ref<GDKAsyncBlock> GDKStore::get_user_collections_id_async(const String& service_ticket, const String& publisher_user_id) const {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        Dictionary return_data;
        size_t bufferSize = 0;
        HRESULT hr = XStoreGetUserCollectionsIdResultSize(async, &bufferSize);
        return_data["hresult"] = hr;
        if (FAILED(hr)) {
            wrapper->emit(return_data);
            return;
        }

        LocalVector<char> buffer;
        buffer.reserve(bufferSize);
        hr = XStoreGetUserCollectionsIdResult(async, bufferSize, buffer.ptr());
        if (SUCCEEDED(hr)) {
            return_data["collections_id"] = String(buffer.ptr());
        }

        wrapper->emit(return_data);
    });

    HRESULT hr = XStoreGetUserCollectionsIdAsync(   _handle,
                                                    service_ticket.utf8().get_data(),
                                                    publisher_user_id.utf8().get_data(),
                                                    asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreGetUserCollectionsIdAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

Ref<GDKAsyncBlock> GDKStore::get_user_purchase_id_async(const String &service_ticket, const String &publisher_user_id) const {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        Dictionary return_data;
        size_t bufferSize = 0;
        HRESULT hr = XStoreGetUserPurchaseIdResultSize(async, &bufferSize);
        return_data["hresult"] = hr;
        if (FAILED(hr)) {
            wrapper->emit(return_data);
            return;
        }

        LocalVector<char> buffer;
        buffer.reserve(bufferSize);
        hr = XStoreGetUserPurchaseIdResult(async, bufferSize, buffer.ptr());
        if (SUCCEEDED(hr)) {
            return_data["purchase_id"] = String(buffer.ptr());
        }

        wrapper->emit(return_data);
    });

    HRESULT hr = XStoreGetUserCollectionsIdAsync(   _handle, 
                                                    service_ticket.utf8().get_data(),
                                                    publisher_user_id.utf8().get_data(),
                                                    asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreGetUserCollectionsIdAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

Ref<GDKAsyncBlock> GDKStore::query_add_on_licenses_async() const {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        Dictionary return_data;
        uint32_t result_count = 0;
        HRESULT hr = XStoreQueryAddOnLicensesResultCount(async, &result_count);
        return_data["hresult"] = hr;
        
        if (FAILED(hr)) {
            wrapper->emit(return_data);
            return;
        }

        XStoreAddonLicense* licenses = new XStoreAddonLicense[result_count];
        hr = XStoreQueryAddOnLicensesResult(async, result_count, licenses);
        return_data["hresult"] = hr;
        
        if (SUCCEEDED(hr)) {
            Array return_licenses;
            for (uint32_t i = 0; i < result_count; i++) {
                Dictionary license_data;
                license_data["sku_store_id"] = String(licenses[i].skuStoreId);
                license_data["in_app_offer_token"] = String(licenses[i].inAppOfferToken);
                license_data["is_active"] = licenses[i].isActive;
                license_data["expiration_date"] = (int64_t)licenses[i].expirationDate;
                return_licenses.push_back(license_data);
            }

            return_data["licenses"] = return_licenses;
        }

        delete[] licenses;

        wrapper->emit(return_data);
    });

    HRESULT hr = XStoreQueryAddOnLicensesAsync(_handle, asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreQueryAddOnLicensesAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

Ref<GDKAsyncBlock> GDKStore::query_associated_products_async(BitField<GDKXStoreProductKind::Enum> product_kinds, int32_t max_results) const {
	HRESULT hr = S_OK;
    if (max_results < 0) {
        hr = E_INVALIDARG;
        ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreQueryAssociatedProductsAsync max_results must be >= 0 Error: 0x%08ux", (uint64_t)hr));
    }
    
    Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);

        Dictionary return_data;
        XStoreProductQueryHandle result = nullptr;
        HRESULT hr = XStoreQueryAssociatedProductsResult(async, &result);
        return_data["hresult"] = hr;
        
        if (SUCCEEDED(hr)) {
            return_data["query_handle"] = GDKXStoreProductQueryHandle::create(result);
        } else {
            XStoreCloseProductsQueryHandle(result);
        }
        wrapper->emit(return_data);
    });

    hr = XStoreQueryAssociatedProductsAsync(_handle, (XStoreProductKind)((int64_t)product_kinds), max_results, asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreQueryAssociatedProductsAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

Ref<GDKAsyncBlock> GDKStore::query_associated_products_for_store_id_async(const String &store_id, BitField<GDKXStoreProductKind::Enum> product_kinds, int32_t max_results) const {
	HRESULT hr = S_OK;
    if (max_results < 0) {
        hr = E_INVALIDARG;
        ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreQueryAssociatedProductsAsync max_results must be >= 0 Error: 0x%08ux", (uint64_t)hr));
    }

    Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);

        Dictionary return_data;
        XStoreProductQueryHandle result = nullptr;
        HRESULT hr = XStoreQueryAssociatedProductsForStoreIdResult(async, &result);
        return_data["hresult"] = hr;
        
        if (SUCCEEDED(hr)) {
            return_data["query_handle"] = GDKXStoreProductQueryHandle::create(result);
        } else {
            XStoreCloseProductsQueryHandle(result);
        }
        wrapper->emit(return_data);
    });

    hr = XStoreQueryAssociatedProductsForStoreIdAsync(_handle, store_id.utf8().get_data(), (XStoreProductKind)((int64_t)product_kinds), max_results, asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreQueryAssociatedProductsForStoreIdAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

Ref<GDKAsyncBlock> GDKStore::query_consumable_balance_remaining_async(const String &store_product_id) const {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        Dictionary return_data;
        XStoreConsumableResult result {};
        HRESULT hr = XStoreQueryConsumableBalanceRemainingResult(async, &result);
        return_data["hresult"] = hr;
        if (SUCCEEDED(hr)) {
            return_data["balance_remaining"] = (int64_t)result.quantity;
        }

        wrapper->emit(return_data);
    });

    HRESULT hr = XStoreQueryConsumableBalanceRemainingAsync(_handle, store_product_id.utf8().get_data(), asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreQueryConsumableBalanceRemainingAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

Ref<GDKAsyncBlock> GDKStore::query_entitled_products_async(BitField<GDKXStoreProductKind::Enum> product_kinds, int max_items_to_retrieve_per_page) const {
	HRESULT hr = S_OK;
    if (max_items_to_retrieve_per_page < 0) {
        hr = E_INVALIDARG;
        ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreQueryEntitledProductsAsync max_items_to_retrieve_per_page must be >= 0 Error: 0x%08ux", (uint64_t)hr));
    }

    Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        Dictionary return_data;
        XStoreProductQueryHandle handle;
        HRESULT hr = XStoreQueryEntitledProductsResult(async, &handle);
        return_data["hresult"] = hr;
        if (SUCCEEDED(hr)) {
            return_data["query_handle"] = GDKXStoreProductQueryHandle::create(handle);
        } else {
            XStoreCloseProductsQueryHandle(handle);
        }

        wrapper->emit(return_data);
    });

    hr = XStoreQueryEntitledProductsAsync(_handle, (XStoreProductKind)((int64_t)product_kinds), (uint32_t)max_items_to_retrieve_per_page, asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreQueryEntitledProductsAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

Ref<GDKAsyncBlock> GDKStore::query_game_and_dlc_package_updates_async() const {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        Dictionary return_data;
        uint32_t result_count;
        HRESULT hr = XStoreQueryGameAndDlcPackageUpdatesResultCount(async, &result_count);
        return_data["hresult"] = hr;
        if (FAILED(hr)) {
            wrapper->emit(return_data);
            return;
        }

        XStorePackageUpdate* updates = new XStorePackageUpdate[result_count];
        hr = XStoreQueryGameAndDlcPackageUpdatesResult(async, result_count, updates);
        return_data["hresult"] = hr;
        if (SUCCEEDED(hr)) {
            Array return_updates; 
            for (uint32_t i = 0; i < result_count; i++) {
                Dictionary update_data;
                update_data["package_identifier"] = String(updates[i].packageIdentifier);
                update_data["is_mandatory"] = updates[i].isMandatory;

                return_updates.push_back(update_data);
            }

            return_data["updates"] = return_updates;
        }

        delete[] updates;
        wrapper->emit(return_data);
    });

    HRESULT hr = XStoreQueryGameAndDlcPackageUpdatesAsync(_handle, asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreQueryGameAndDlcPackageUpdatesAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

Ref<GDKAsyncBlock> GDKStore::query_game_license_async() const {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        Dictionary return_data;
        XStoreGameLicense license {};
        HRESULT hr = XStoreQueryGameLicenseResult(async, &license);
        return_data["hresult"] = hr;
        if (SUCCEEDED(hr)) {

            Ref<GDKXStoreGameLicense> license_data = GDKXStoreGameLicense::create(&license);
            return_data["license"] = license_data;
        }
        wrapper->emit(return_data);
    });
    
    HRESULT hr = XStoreQueryGameLicenseAsync(_handle, asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreQueryGameLicenseAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

Ref<GDKAsyncBlock> GDKStore::query_license_token_async(const PackedStringArray &product_ids, const String &custom_developer_string) const {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        Dictionary return_data;
        size_t bufferSize = 0;
        HRESULT hr = XStoreQueryLicenseTokenResultSize(async, &bufferSize);
        if (FAILED(hr)) {
            return_data["hresult"] = hr;
            wrapper->emit(return_data);
            return;
        }

        LocalVector<char> buffer;
        buffer.reserve(bufferSize);
        hr = XStoreQueryLicenseTokenResult(async, bufferSize, buffer.ptr());   
        
        if (SUCCEEDED(hr)) {
            return_data["license_token"] = String(buffer.ptr());
        }
        return_data["hresult"] = hr;
        wrapper->emit(return_data);
    });
    LocalVector<const char*> ids;
    ids.reserve(product_ids.size());
    for (int i = 0; i < product_ids.size(); i++) {
        ids.push_back(product_ids[i].utf8().get_data());
    }
    HRESULT hr = XStoreQueryLicenseTokenAsync(  _handle, 
                                                (ids.size() > 0) ? ids.ptr() : nullptr, 
                                                ids.size(), 
                                                custom_developer_string.utf8().get_data(), 
                                                asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreQueryLicenseTokenAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

String GDKStore::query_package_identifier(const String &store_id) const {
	char identifier[XPACKAGE_IDENTIFIER_MAX_LENGTH];
    HRESULT hr = XStoreQueryPackageIdentifier(store_id.utf8().get_data(), XPACKAGE_IDENTIFIER_MAX_LENGTH, identifier);
    ERR_FAIL_COND_V_MSG(FAILED(hr), String(), vformat("XStoreQueryPackageIdentifier Error: 0x%08ux", (uint64_t)hr));
    return String(identifier);
}

Ref<GDKAsyncBlock> GDKStore::query_package_updates_async(const PackedStringArray &package_identifiers) const {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        Dictionary return_data;
        uint32_t result_count = 0;
        HRESULT hr = XStoreQueryPackageUpdatesResultCount(async, &result_count);
        if (FAILED(hr)) {
            return_data["hresult"] = hr;
            wrapper->emit(return_data);
            return;
        }
        XStorePackageUpdate* updates = new XStorePackageUpdate[result_count];
        hr = XStoreQueryPackageUpdatesResult(async, result_count, updates);
        return_data["hresult"] = hr;
        if (SUCCEEDED(hr)) {
            Array update_array;
            for (uint32_t i = 0; i < result_count; i++) {
                Dictionary update_data;
                update_data["package_identifier"] = String(updates[i].packageIdentifier);
                update_data["is_mandatory"] = updates[i].isMandatory;

                update_array.push_back(update_data);
            }

            return_data["updates"] = update_array;
        }

        delete[] updates;
        wrapper->emit(return_data);
    });
    
    LocalVector<const char*> identifiers;
    identifiers.reserve(package_identifiers.size());
    for (int i = 0; i < package_identifiers.size(); i++) {
        identifiers.push_back(package_identifiers[i].utf8().get_data());
    }

    HRESULT hr = XStoreQueryPackageUpdatesAsync(    _handle, 
                                                    (identifiers.size() > 0) ? identifiers.ptr() : nullptr, 
                                                    identifiers.size(), 
                                                    asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreQueryPackageUpdatesAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

Ref<GDKAsyncBlock> GDKStore::query_product_for_current_game_async() const {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        Dictionary return_data;
        XStoreProductQueryHandle handle = nullptr;
        HRESULT hr = XStoreQueryProductForCurrentGameResult(async, &handle);
        return_data["hresult"] = hr;
        if (SUCCEEDED(hr)) {
            return_data["handle"] = GDKXStoreProductQueryHandle::create(handle);
        } else {
            XStoreCloseProductsQueryHandle(handle);
        }
        wrapper->emit(return_data);
    });

    HRESULT hr = XStoreQueryProductForCurrentGameAsync(_handle, asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreQueryProductForCurrentGameAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

Ref<GDKAsyncBlock> GDKStore::query_product_for_package_async(BitField<GDKXStoreProductKind::Enum> product_kinds, const String &package_identifier) const {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        Dictionary return_data;

        XStoreProductQueryHandle handle = nullptr;
        HRESULT hr = XStoreQueryProductForPackageResult(async, &handle);
        if (SUCCEEDED(hr)) {
            return_data["handle"] = GDKXStoreProductQueryHandle::create(handle);
        } else {
            XStoreCloseProductsQueryHandle(handle);
        }
        return_data["hresult"] = hr;
        wrapper->emit(return_data);
    });

    HRESULT hr = XStoreQueryProductForPackageAsync(_handle, (XStoreProductKind)((int64_t)product_kinds), package_identifier.utf8().get_data(), asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreQueryProductForPackageAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

Ref<GDKAsyncBlock> GDKStore::query_products_async(BitField<GDKXStoreProductKind::Enum> product_kinds, const PackedStringArray &store_ids, const PackedStringArray &action_filters) const {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        Dictionary return_data;

        XStoreProductQueryHandle handle = nullptr;
        HRESULT hr = XStoreQueryProductsResult(async, &handle);
        if (SUCCEEDED(hr)) {
            return_data["handle"] = GDKXStoreProductQueryHandle::create(handle);
        } else {
            XStoreCloseProductsQueryHandle(handle);
        }

        return_data["hresult"] = hr;
        wrapper->emit(return_data);
    });
    LocalVector<const char*> local_store_ids;
    local_store_ids.reserve(store_ids.size());
    for (int i = 0; i < store_ids.size(); i++) {
        local_store_ids.push_back(store_ids[i].utf8().get_data());
    }

    LocalVector<const char*> local_action_filters;
    local_action_filters.reserve(action_filters.size());
    for (int i = 0; i < action_filters.size(); i++) {
        local_action_filters.push_back(action_filters[i].utf8().get_data());
    }
    HRESULT hr = XStoreQueryProductsAsync(  _handle, 
                                            (XStoreProductKind)((int64_t)product_kinds), 
                                            (local_store_ids.size() > 0) ? local_store_ids.ptr() : nullptr, 
                                            local_store_ids.size(), 
                                            (local_action_filters.size() > 0) ? local_action_filters.ptr() : nullptr,
                                            local_action_filters.size(),
                                            asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreQueryProductsAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

Ref<GDKAsyncBlock> GDKStore::report_consumable_fulfillment_async(const String &store_product_id, int quantity, const String &tracking_id) const {
	HRESULT hr = S_OK;
    if (quantity < 0) {
        hr = E_INVALIDARG;
        ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreReportConsumableFulfillmentAsync quantity must be >= 0 Error: 0x%08ux", (uint64_t)hr));
    }

    Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        Dictionary return_data;
        XStoreConsumableResult result {};
        HRESULT hr = XStoreReportConsumableFulfillmentResult(async, &result);
        return_data["hresult"] = hr;
        if (SUCCEEDED(hr)) {
            return_data["result"] = (int64_t)result.quantity;
        }
        wrapper->emit(return_data);
    });

    GUID guid;
    hr = CLSIDFromString(tracking_id.wide_string().get_data(), &guid);
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreReportConsumableFulfillmentAsync GUID generation failed Error: 0x%08ux", (uint64_t)hr));

    hr = XStoreReportConsumableFulfillmentAsync(_handle, store_product_id.utf8().get_data(), quantity, guid, asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreReportConsumableFulfillmentAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

Ref<GDKAsyncBlock> GDKStore::show_associated_products_ui_async(const String &store_id, BitField<GDKXStoreProductKind::Enum> product_kinds) const {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        Dictionary return_data;

        HRESULT hr = XStoreShowAssociatedProductsUIResult(async);
        return_data["hresult"] = hr;
        wrapper->emit(return_data);
    });

    HRESULT hr = XStoreShowAssociatedProductsUIAsync(   _handle,
                                                        store_id.utf8().get_data(),
                                                        (XStoreProductKind)((int64_t)product_kinds),
                                                        asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreShowAssociatedProductsUIAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

Ref<GDKAsyncBlock> GDKStore::show_gifting_ui_async(const String &store_id, const String &name, const String &extended_json_data) const {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        Dictionary return_data;
        
        HRESULT hr = XStoreShowGiftingUIResult(async);
        return_data["hresult"] = hr;
        wrapper->emit(return_data);
    });

    HRESULT hr = XStoreShowGiftingUIAsync(  _handle, 
                                            store_id.utf8().get_data(),
                                            name.is_empty() ? nullptr : name.utf8().get_data(),
                                            extended_json_data.is_empty() ? nullptr : extended_json_data.utf8().get_data(),
                                            asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreShowGiftingUIAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

Ref<GDKAsyncBlock> GDKStore::show_product_page_ui_async(const String &store_id) const {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        Dictionary return_data;
        
        HRESULT hr = XStoreShowProductPageUIResult(async);
        return_data["hresult"] = hr;
        wrapper->emit(return_data);
    });

    HRESULT hr = XStoreShowProductPageUIAsync(_handle, store_id.utf8().get_data(), asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreShowProductPageUIAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

Ref<GDKAsyncBlock> GDKStore::show_purchase_ui_async(const String &store_id, const String &name, const String &extended_json_data) const {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        Dictionary return_data;
        
        HRESULT hr = XStoreShowPurchaseUIResult(async);
        return_data["hresult"] = hr;
        wrapper->emit(return_data);
    });

    HRESULT hr = XStoreShowPurchaseUIAsync( _handle, 
                                            store_id.utf8().get_data(), 
                                            name.is_empty() ? nullptr : name.utf8().get_data(), 
                                            extended_json_data.is_empty() ? nullptr : extended_json_data.utf8().get_data(), 
                                            asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreShowPurchaseUIAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

Ref<GDKAsyncBlock> GDKStore::show_rate_and_review_ui_async() const {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        Dictionary return_data;
        
        XStoreRateAndReviewResult result {};
        HRESULT hr = XStoreShowRateAndReviewUIResult(async, &result);
        return_data["hresult"] = hr;
        if (SUCCEEDED(hr)) {
            return_data["was_updated"] = result.wasUpdated;
        }
        wrapper->emit(return_data);
    });

    HRESULT hr = XStoreShowRateAndReviewUIAsync(_handle, asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreShowRateAndReviewUIAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

Ref<GDKAsyncBlock> GDKStore::show_redeem_token_ui_async(const String &token, bool disallow_csv_redemption, const PackedStringArray &allowed_store_ids) const {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        Dictionary return_data;
        
        HRESULT hr = XStoreShowRedeemTokenUIResult(async);
        return_data["hresult"] = hr;
        wrapper->emit(return_data);
    });
    LocalVector<const char*> allowedStoreIds;
    if (allowed_store_ids.size() > 0) {
        allowedStoreIds.reserve(allowed_store_ids.size());
        for (int i = 0; i < allowed_store_ids.size(); i++) {
            allowedStoreIds.push_back(allowed_store_ids[i].utf8().get_data());
        }
    }

    HRESULT hr = XStoreShowRedeemTokenUIAsync(  _handle,
                                                token.utf8().get_data(),
                                                (allowedStoreIds.size() > 0) ? allowedStoreIds.ptr() : nullptr,
                                                allowedStoreIds.size(),
                                                disallow_csv_redemption,
                                                asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XStoreShowRedeemTokenUIAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

void GDKXStoreGameLicense::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_sku_store_id"), &GDKXStoreGameLicense::get_sku_store_id);
    ClassDB::bind_method(D_METHOD("is_active"), &GDKXStoreGameLicense::is_active);
    ClassDB::bind_method(D_METHOD("is_trial_owned_by_this_user"), &GDKXStoreGameLicense::is_trial_owned_by_this_user);
    ClassDB::bind_method(D_METHOD("is_disc_license"), &GDKXStoreGameLicense::is_disc_license);
    ClassDB::bind_method(D_METHOD("is_trial"), &GDKXStoreGameLicense::is_trial);
    ClassDB::bind_method(D_METHOD("get_trial_time_remaining_in_seconds"), &GDKXStoreGameLicense::get_trial_time_remaining_in_seconds);
    ClassDB::bind_method(D_METHOD("get_trial_unique_id"), &GDKXStoreGameLicense::get_trial_unique_id);
    ClassDB::bind_method(D_METHOD("get_expiration_date"), &GDKXStoreGameLicense::get_expiration_date);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "sku_store_id"), "", "get_sku_store_id");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_active"), "", "is_active");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_trial_owned_by_this_user"), "", "is_trial_owned_by_this_user");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_disc_license"), "", "is_disc_license");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_trial"), "", "is_trial");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "trial_time_remaining_in_seconds"), "", "get_trial_time_remaining_in_seconds");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "trial_unique_id"), "", "get_trial_unique_id");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "expiration_date"), "", "get_expiration_date");
}

Ref<GDKXStoreGameLicense> GDKXStoreGameLicense::create(const XStoreGameLicense *license) {
	Ref<GDKXStoreGameLicense> wrapper;
    if (license != nullptr) {
        wrapper.instantiate();
        wrapper->_sku_store_id = String(license->skuStoreId);
        wrapper->_is_active = license->isActive;
        wrapper->_is_trial_owned_by_this_user = license->isTrialOwnedByThisUser;
        wrapper->_is_disc_license = license->isDiscLicense;
        wrapper->_is_trial = license->isTrial;
        wrapper->_trial_time_remaining_in_seconds = (int64_t)license->trialTimeRemainingInSeconds;
        wrapper->_trial_unique_id = String(license->trialUniqueId);
        wrapper->_expiration_date = (int64_t)license->expirationDate;
    }
    return wrapper;
}
