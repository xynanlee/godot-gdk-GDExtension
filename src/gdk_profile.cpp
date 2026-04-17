#include <xsapi-c/types_c.h>
#include "gdk_profile.h"
#include "gdk_helpers.h"

using namespace godot;

void GDKProfile::_bind_methods() {
    ClassDB::bind_static_method(get_class_static(), D_METHOD("get_user_profile_async", "caller_user", "xbox_user_id"), &GDKProfile::get_user_profile_async);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("get_user_profiles_async", "caller_user", "xbox_user_ids"), &GDKProfile::get_user_profiles_async);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("get_user_profiles_for_social_group_async", "caller_user", "social_group"), &GDKProfile::get_user_profiles_for_social_group_async);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "xbox_user_id"), "", "get_xbox_user_id");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "app_display_name"), "", "get_app_display_name");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "app_display_picture_resize_uri"), "", "get_app_display_picture_resize_uri");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "game_display_name"), "", "get_game_display_name");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "game_display_picture_resize_uri"), "", "get_game_display_picture_resize_uri");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "gamerscore"), "", "get_gamerscore");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "gamertag"), "", "get_gamertag");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "modern_gamertag"), "", "get_modern_gamertag");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "modern_gamertag_suffix"), "", "get_modern_gamertag_suffix");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "unique_modern_gamertag"), "", "get_unique_modern_gamertag");
}

Ref<GDKProfile> GDKProfile::create(XblUserProfile *profile) {
    Ref<GDKProfile> wrapper;
    wrapper.instantiate();
    wrapper->_xbox_user_id = profile->xboxUserId;
    wrapper->_app_display_name = profile->appDisplayName;
    wrapper->_app_display_picture_resize_uri = profile->appDisplayPictureResizeUri;
    wrapper->_game_display_name = profile->gameDisplayName;
    wrapper->_game_display_picture_resize_uri = profile->gameDisplayPictureResizeUri;
    wrapper->_gamerscore = profile->gamerscore;
    wrapper->_gamertag = profile->gamertag;
    wrapper->_modern_gamertag = profile->modernGamertag;
    wrapper->_modern_gamertag_suffix = profile->modernGamertagSuffix;
    wrapper->_unique_modern_gamertag = profile->uniqueModernGamertag;
	return wrapper;
}

Ref<GDKAsyncBlock> GDKProfile::get_user_profile_async(Ref<GDKUser> caller_user, int64_t xbox_user_id) {
	Ref<GDKAsyncBlock> async_block = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    async_block->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        Dictionary return_data;

        XblUserProfile profile;
        HRESULT hr = XblProfileGetUserProfileResult(async, &profile);
        return_data["hresult"] = hr;
        if (SUCCEEDED(hr)) {
            return_data["profile"] = GDKProfile::create(&profile);
        }

        wrapper->emit(return_data);
    });

    HRESULT hr = XblProfileGetUserProfileAsync(caller_user->get_xbox_live_context(), (uint64_t)xbox_user_id, async_block->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XblProfileGetUserProfileAsync Error: 0x%08ux", (uint64_t)hr));
    return async_block;
}

Ref<GDKAsyncBlock> GDKProfile::get_user_profiles_async(Ref<GDKUser> caller_user, PackedInt64Array xbox_user_ids) {
    Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        Dictionary return_data;
        size_t profileCount = 0;
        size_t bufferSize = 0;
        XblUserProfile* profiles = nullptr;

        HRESULT hr = XAsyncGetResultSize(async, &bufferSize);
        if (SUCCEEDED(hr)) {
            hr = XblProfileGetUserProfilesResultCount(async, &profileCount);
        }

        if (SUCCEEDED(hr)) {
            profiles = reinterpret_cast<XblUserProfile*>(malloc(bufferSize));
            hr = XblProfileGetUserProfilesResult(async, profileCount, profiles);
        }

        if (SUCCEEDED(hr)) {
            TypedArray<Ref<GDKProfile>> profileArray;
            for (size_t i = 0; i < profileCount; i++) {
                Ref<GDKProfile> profile = GDKProfile::create(&profiles[i]);
                profileArray.push_back(profile);
            }

            return_data["profiles"] = profileArray;
        }

        if (profiles != nullptr) {
            ::free(profiles);
        }

        return_data["hresult"] = hr;
        wrapper->emit(return_data);
    });

    LocalVector<uint64_t> userIds;
    userIds.reserve(xbox_user_ids.size());
    for (int i = 0; i < xbox_user_ids.size(); i++) {
        userIds.push_back((uint64_t)xbox_user_ids[i]);
    }
    
    HRESULT hr = XblProfileGetUserProfilesAsync(caller_user->get_xbox_live_context(), userIds.ptr(), userIds.size(), asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XblProfileGetUserProfilesAsync Error: 0x%08ux", (uint64_t)hr));
	return asyncBlock;
}

Ref<GDKAsyncBlock> GDKProfile::get_user_profiles_for_social_group_async(Ref<GDKUser> caller_user, const String &social_group) {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        Dictionary return_data;
        size_t profileCount = 0;
        size_t bufferSize = 0;
        XblUserProfile* profiles = nullptr;

        HRESULT hr = XAsyncGetResultSize(async, &bufferSize);
        if (SUCCEEDED(hr)) {
            hr = XblProfileGetUserProfilesForSocialGroupResultCount(async, &profileCount);
        }

        if (SUCCEEDED(hr)) {
            profiles = reinterpret_cast<XblUserProfile*>(malloc(bufferSize));
            hr = XblProfileGetUserProfilesForSocialGroupResult(async, profileCount, profiles);
        }

        if (SUCCEEDED(hr)) {
            TypedArray<Ref<GDKProfile>> profileArray;
            for (size_t i = 0; i < profileCount; i++) {
                Ref<GDKProfile> profile = GDKProfile::create(&profiles[i]);
                profileArray.push_back(profile);
            }

            return_data["profiles"] = profileArray;
        }

        if (profiles != nullptr) {
            ::free(profiles);
        }

        return_data["hresult"] = hr;
        wrapper->emit(return_data);
    });

    const char* social_group_cstr = social_group.utf8().get_data();
    HRESULT hr = XblProfileGetUserProfilesForSocialGroupAsync(caller_user->get_xbox_live_context(), social_group_cstr, asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XblProfileGetUserProfilesForSocialGroupAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}
