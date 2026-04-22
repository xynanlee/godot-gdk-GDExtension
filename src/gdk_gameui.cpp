#include <windows.h>
#include <XGame.h>
#include <variant/typed_array.hpp>
#include <templates/local_vector.hpp>
#include "gdk_gameui.h"
#include "gdk_helpers.h"

using namespace godot;
void GDKXGameUINotificationPositionHint::_bind_methods() {
    BIND_ENUM_CONSTANT(BottomCenter);
    BIND_ENUM_CONSTANT(BottomLeft);
    BIND_ENUM_CONSTANT(BottomRight);
    BIND_ENUM_CONSTANT(TopCenter);
    BIND_ENUM_CONSTANT(TopLeft);
    BIND_ENUM_CONSTANT(TopRight);
}

void GDKXGameUITextEntryInputScope::_bind_methods() {
    BIND_ENUM_CONSTANT(Default);
    BIND_ENUM_CONSTANT(Url);
    BIND_ENUM_CONSTANT(EmailSmtpAddress);
    BIND_ENUM_CONSTANT(Number);
    BIND_ENUM_CONSTANT(Password);
    BIND_ENUM_CONSTANT(TelephoneNumber);
    BIND_ENUM_CONSTANT(Alphanumeric);
    BIND_ENUM_CONSTANT(Search);
    BIND_ENUM_CONSTANT(ChatWithoutEmoji);
}

void GDKXGameUITextEntryPositionHint::_bind_methods() {
    BIND_ENUM_CONSTANT(Bottom);
    BIND_ENUM_CONSTANT(Top);
}

void GDKXGameUITextEntryVisibilityFlags::_bind_methods() {
    BIND_BITFIELD_FLAG(Default);
    BIND_BITFIELD_FLAG(OnlyShowCandidates);
}

void GDKXGameUITextEntryOptions::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_input_scope"), &GDKXGameUITextEntryOptions::get_input_scope);
    ClassDB::bind_method(D_METHOD("get_position_hint"), &GDKXGameUITextEntryOptions::get_position_hint);
    ClassDB::bind_method(D_METHOD("get_visibility_flags"), &GDKXGameUITextEntryOptions::get_visibility_flags);

    ClassDB::bind_method(D_METHOD("set_input_scope", "input_scope"), &GDKXGameUITextEntryOptions::set_input_scope);
    ClassDB::bind_method(D_METHOD("set_position_hint", "position_hint"), &GDKXGameUITextEntryOptions::set_position_hint);
    ClassDB::bind_method(D_METHOD("set_visibility_flags", "visibility_flags"), &GDKXGameUITextEntryOptions::set_visibility_flags);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "input_scope", PROPERTY_HINT_ENUM, "Default,Url,EmailSmtpAddress,Number,Password,TelephoneNumber,Alphanumeric,Search,ChatWithoutEmoji"), "set_input_scope", "get_input_scope");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "position_hint", PROPERTY_HINT_ENUM, "Bottom,Top"), "set_position_hint", "get_position_hint");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "visibility_flags", PROPERTY_HINT_FLAGS, "Default,OnlyShowCandidates"), "set_visibility_flags", "get_visibility_flags");
}

GDKXGameUITextEntryOptions::GDKXGameUITextEntryOptions()
: _visibilityFlags(GDKXGameUITextEntryVisibilityFlags::Default) {
}

void GDKXGameUITextEntryChangeTypeFlags::_bind_methods() {
    BIND_BITFIELD_FLAG(None);
    BIND_BITFIELD_FLAG(TextChanged);
    BIND_BITFIELD_FLAG(Dismissed);
}

void GDKGameUITextEntryHandle::_bind_methods() {
    ClassDB::bind_method(D_METHOD("close"), &GDKGameUITextEntryHandle::close);
    ClassDB::bind_method(D_METHOD("get_extents"), &GDKGameUITextEntryHandle::get_extents);
    ClassDB::bind_method(D_METHOD("get_state", "buffer_size"), &GDKGameUITextEntryHandle::get_state);
    ClassDB::bind_method(D_METHOD("update_position_hint", "position_hint"), &GDKGameUITextEntryHandle::update_position_hint);
    ClassDB::bind_method(D_METHOD("update_visibility", "visibility_flags"), &GDKGameUITextEntryHandle::update_visibility);
}

void GDKGameUITextEntryHandle::_notification(int p_what) {
    if (p_what == NOTIFICATION_PREDELETE) {
        if (_handle != nullptr) {
            XGameUiTextEntryClose(_handle);
        }
    }
}

Ref<GDKGameUITextEntryHandle> GDKGameUITextEntryHandle::create(const XGameUiTextEntryHandle &in_handle) {
    Ref<GDKGameUITextEntryHandle> handle;
    handle.instantiate();
    handle->_handle = in_handle;
    return handle;
}

void GDKGameUITextEntryHandle::close() {
	XGameUiTextEntryClose(_handle);
}

Vector4 GDKGameUITextEntryHandle::get_extents() const {
    XGameUiTextEntryExtents extents;
    HRESULT hr = XGameUiTextEntryGetExtents(_handle, &extents);
	Vector4 return_extents;
    return_extents.x = extents.left;
    return_extents.y = extents.top;
    return_extents.z = extents.right;
    return_extents.w = extents.bottom;
    return return_extents;
}

Dictionary GDKGameUITextEntryHandle::get_state(int buffer_size) const {
    XGameUiTextEntryChangeTypeFlags changeType;
    uint32_t cursorIndex;
    uint32_t imeStart;
    uint32_t imeEnd;
    
    LocalVector<char> buffer;
    buffer.reserve(buffer_size);
    HRESULT hr = XGameUiTextEntryGetState(_handle, &changeType, &cursorIndex, &imeStart, &imeEnd, buffer_size, buffer.ptr());
    
    Dictionary return_state;
    if (SUCCEEDED(hr)) {
        String return_string;
        return_string = String(buffer.ptr());
        return_state["buffer"] = return_string;
    }

    ERR_FAIL_COND_V_MSG(FAILED(hr), Dictionary(), vformat("XGameUiTextEntryGetState Error: 0x%08ux", (uint64_t)hr));

    return_state["hresult"] = hr;
    return_state["change_type"] = (GDKXGameUITextEntryChangeTypeFlags::Enum)changeType;
    return_state["cursor_index"] = (int64_t)cursorIndex;
    return_state["ime_start"] = (int64_t)imeStart;
    return_state["ime_end"] = (int64_t)imeEnd;
    
    return return_state;
}

int GDKGameUITextEntryHandle::update_position_hint(GDKXGameUITextEntryPositionHint::Enum position_hint) {
	HRESULT hr = XGameUiTextEntryUpdatePositionHint(_handle, (XGameUiTextEntryPositionHint)position_hint);
    ERR_FAIL_COND_V_MSG(FAILED(hr), (int64_t)hr, vformat("XGameUiTextEntryUpdatePositionHint Error: 0x%08ux", (uint64_t)hr));
    return (int64_t)hr;
}

int GDKGameUITextEntryHandle::update_visibility(BitField<GDKXGameUITextEntryVisibilityFlags::Enum> visibility_flags) {
	HRESULT hr = XGameUiTextEntryUpdateVisibility(_handle, (XGameUiTextEntryVisibilityFlags)((int64_t)visibility_flags));
    ERR_FAIL_COND_V_MSG(FAILED(hr), (int64_t)hr, vformat("XGameUiTextEntryUpdateVisibility Error: 0x%08ux", (uint64_t)hr));
    return (int64_t)hr;
}

void GDKGameUI::_bind_methods() {
    ClassDB::bind_static_method(get_class_static(), D_METHOD("set_notification_position_hint", "hint"), &GDKGameUI::set_notification_position_hint);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("show_achievements_async", "user"), &GDKGameUI::show_achievements_async);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("show_error_dialog_async", "error_code", "message"), &GDKGameUI::show_error_dialog_async);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("show_message_dialog_async", "title_text", "content_text", "first_button_text", "second_button_text", "third_button_text", "default_button_index", "cancel_button_index"), &GDKGameUI::show_message_dialog_async);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("show_multiplayer_activity_game_invite_async", "user"), &GDKGameUI::show_multiplayer_activity_game_invite_async);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("show_player_profile_card_async", "requesting_user", "target_user"), &GDKGameUI::show_player_profile_card_async);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("show_send_game_invite_async", "requesting_user", "session_template", "session_id", "invitation_text", "custom_activation_context"), &GDKGameUI::show_send_game_invite_async);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("show_state_share_async", "requesting_user", "link_token"), &GDKGameUI::show_state_share_async);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("show_text_entry_async", "input_scope", "max_text_length", "title_text", "description_text", "default_text"), &GDKGameUI::show_text_entry_async);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("show_web_authentication_async", "requesting_user", "request_uri", "completion_uri"), &GDKGameUI::show_web_authentication_async);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("show_web_authentication_with_options_async", "requesting_user", "request_uri", "completion_uri", "is_fullscreen"), &GDKGameUI::show_web_authentication_with_options_async);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("text_entry_open", "options", "max_length", "initial_text", "initial_cursor_index"), &GDKGameUI::text_entry_open);
}

int GDKGameUI::set_notification_position_hint(GDKXGameUINotificationPositionHint::Enum hint) {
    XGameUiNotificationPositionHint position_hint = (XGameUiNotificationPositionHint)hint;
    HRESULT hr = XGameUiSetNotificationPositionHint(position_hint);
    ERR_FAIL_COND_V_MSG(FAILED(hr), (int64_t)hr, vformat("XGameUiSetNotificationPositionHint Error: 0x%08ux", (uint64_t)hr));
    return (int64_t)hr;
}

Ref<GDKAsyncBlock> GDKGameUI::show_achievements_async(Ref<GDKUser> user) {
    Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        HRESULT hr = XGameUiShowAchievementsResult(async);
        Dictionary return_data;
        return_data["hresult"] = hr;
        wrapper->emit(return_data);
    });
    
	uint32_t title_id = 0;
	HRESULT hr = XGameGetXboxTitleId(&title_id);
	ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XGameUiShowAchievementsAsync::XGameGetXboxTitleId Error: 0x%08ux", (uint64_t)hr));

    hr = XGameUiShowAchievementsAsync(asyncBlock->get_block(), user->get_user(), title_id);
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XGameUiShowAchievementsAsync Error: 0x%08ux", (uint64_t)hr));
	return asyncBlock;
}

Ref<GDKAsyncBlock> GDKGameUI::show_error_dialog_async(int error_code, const String &message) {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        HRESULT hr = XGameUiShowErrorDialogResult(async);
        Dictionary return_data;
        return_data["hresult"] = hr;
        wrapper->emit(return_data);
    });

    HRESULT hr = XGameUiShowErrorDialogAsync(asyncBlock->get_block(), (HRESULT)error_code, message.utf8().get_data());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XGameUiShowErrorDialogAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

Ref<GDKAsyncBlock> GDKGameUI::show_message_dialog_async(const String &title_text, 
                                                        const String &content_text, 
                                                        const String &first_button_text, 
                                                        const String &second_button_text, 
                                                        const String &third_button_text, 
                                                        int default_button_index, 
                                                        int cancel_button_index) {
    Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        XGameUiMessageDialogButton resultButton;
        HRESULT hr = XGameUiShowMessageDialogResult(async, &resultButton);    
        Dictionary return_data;
        return_data["hresult"] = hr;
        if (SUCCEEDED(hr)) {
            return_data["result_button"] = (int64_t)resultButton;
        }

        wrapper->emit(return_data);
    });

    HRESULT hr = S_OK;
    if (default_button_index < 0 || default_button_index > 2) {
        hr = E_INVALIDARG;
        ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, "XGameUiShowMessageDialogAsync default_button_index must be >= 0 and <= 2");
    }

    if (cancel_button_index < 0 || cancel_button_index > 2) {
        hr = E_INVALIDARG;
        ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, "XGameUiShowMessageDialogAsync cancel_button_index must be >= 0 and <= 2");
    }

    if (SUCCEEDED(hr)) {
        hr = XGameUiShowMessageDialogAsync( asyncBlock->get_block(),
                                            title_text.utf8().get_data(),
                                            content_text.utf8().get_data(),
                                            first_button_text.utf8().get_data(),
                                            second_button_text.utf8().get_data(),
                                            third_button_text.utf8().get_data(),
                                            (XGameUiMessageDialogButton)default_button_index,
                                            (XGameUiMessageDialogButton)cancel_button_index);
        ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XGameUiShowMessageDialogAsync Error: 0x%08ux", (uint64_t)hr));              
    }

	return asyncBlock;
}

Ref<GDKAsyncBlock> GDKGameUI::show_multiplayer_activity_game_invite_async(Ref<GDKUser> user) {
    Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        HRESULT hr = XGameUiShowMultiplayerActivityGameInviteResult(async);
        Dictionary return_data;
        return_data["hresult"] = hr;
        wrapper->emit(return_data); 
    });

    HRESULT hr = XGameUiShowMultiplayerActivityGameInviteAsync(asyncBlock->get_block(), user->get_user());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XGameUiShowMultiplayerActivityGameInviteAsync Error: 0x%08ux", (uint64_t)hr));
	return asyncBlock;
}

Ref<GDKAsyncBlock> GDKGameUI::show_player_picker_async( Ref<GDKUser> user, 
                                                        const String &prompt_text, 
                                                        PackedInt64Array select_from, 
                                                        int min_selection, 
                                                        int max_selection,
                                                        PackedInt64Array preselected) { 
    Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        uint32_t result_count = 0;
        Dictionary return_data;
        HRESULT hr = XGameUiShowPlayerPickerResultCount(async, &result_count);
        return_data["hresult"] = hr;

        uint64_t* result_players = nullptr;
        uint32_t players_used = 0;
        if (SUCCEEDED(hr)) {
            hr = XGameUiShowPlayerPickerResult(async, result_count, result_players, &players_used);
        }
        
        if (SUCCEEDED(hr)) {
            TypedArray<Ref<GDKUser>> return_results;

            for (uint32_t i = 0; i < players_used; i++) {
                Ref<GDKUser> player = GDKUser::find_user_by_id(result_players[i]);
                return_results.push_back(player);
            }
            return_data["result_players"] = return_results;
        }
        
        return_data["hresult"] = hr;
        wrapper->emit(return_data);
    });

    HRESULT hr = XGameUiShowPlayerPickerAsync(  asyncBlock->get_block(),
                                                user->get_user(),
                                                prompt_text.utf8().get_data(),
                                                select_from.size(),
                                                (uint64_t*)select_from.ptr(),
                                                preselected.size(),
                                                (uint64_t*)preselected.ptr(),
                                                min_selection,
                                                max_selection);

    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XGameUiShowPlayerPickerAsync Error: 0x%08ux", (uint64_t)hr));
	return asyncBlock;
}

Ref<GDKAsyncBlock> GDKGameUI::show_player_profile_card_async(Ref<GDKUser> requesting_user, Ref<GDKUser> target_user) {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        HRESULT hr = XGameUiShowPlayerProfileCardResult(async);
        Dictionary return_data;
        return_data["hresult"] = hr;
        wrapper->emit(return_data);
    });

    const int64_t xbox_id = target_user->get_id();
    HRESULT hr = XGameUiShowPlayerProfileCardAsync(asyncBlock->get_block(), requesting_user->get_user(), (uint64_t)xbox_id);
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XGameUiShowPlayerProfileCardAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

Ref<GDKAsyncBlock> GDKGameUI::show_send_game_invite_async(  Ref<GDKUser> requesting_user, 
                                                            const String &session_template, 
                                                            const String &session_id, 
                                                            const String &invitation_text, 
                                                            const String &custom_activation_context) {
    Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        HRESULT hr = XGameUiShowSendGameInviteResult(async);
        Dictionary return_data;
        return_data["hresult"] = hr;
        wrapper->emit(return_data);
    });

    HRESULT hr = XGameUiShowSendGameInviteAsync(asyncBlock->get_block(),
                                                requesting_user->get_user(),
                                                GDKHelpers::get_scid().utf8().get_data(),
                                                session_template.utf8().get_data(),
                                                session_id.utf8().get_data(),
                                                invitation_text.utf8().get_data(),
                                                custom_activation_context.utf8().get_data());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XGameUiShowSendGameInviteAsync Error: 0x%08ux", (uint64_t)hr));
	return asyncBlock;
}

Ref<GDKAsyncBlock> GDKGameUI::show_state_share_async(Ref<GDKUser> requesting_user, const String &link_token) {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        HRESULT hr = XGameUiShowStateShareResult(async);
        Dictionary return_data;
        return_data["hresult"] = hr;
        wrapper->emit(return_data);
    });

    HRESULT hr = XGameUiShowStateShareAsync(asyncBlock->get_block(), requesting_user->get_user(), link_token.utf8().get_data());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XGameUiShowStateShareAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

Ref<GDKAsyncBlock> GDKGameUI::show_text_entry_async(GDKXGameUITextEntryInputScope::Enum input_scope, int max_text_length, const String &title_text, const String &description_text, const String &default_text) {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        uint32_t bufferSize = 0;
        Dictionary return_data;
        HRESULT hr = XGameUiShowTextEntryResultSize(async, &bufferSize);

        char* resultBuffer = new char[bufferSize];
        if (SUCCEEDED(hr)) {
            hr = XGameUiShowTextEntryResult(async, bufferSize, resultBuffer, nullptr);
        }

        if (SUCCEEDED(hr)) {
            String returnString(resultBuffer);
            return_data["text"] = returnString;
        }

        delete[] resultBuffer;
        resultBuffer = nullptr;

        return_data["hresult"] = hr;
        wrapper->emit(return_data);
    });

    HRESULT hr = XGameUiShowTextEntryAsync( asyncBlock->get_block(),
                                            title_text.utf8().get_data(),
                                            description_text.utf8().get_data(),
                                            default_text.utf8().get_data(),
                                            (XGameUiTextEntryInputScope)input_scope,
                                            max_text_length);
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XGameUiShowTextEntryAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

Ref<GDKAsyncBlock> GDKGameUI::show_web_authentication_async(Ref<GDKUser> requesting_user, const String &request_uri, const String &completion_uri) {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        size_t bufferSize = 0;
        HRESULT hr = XGameUiShowWebAuthenticationResultSize(async, &bufferSize);

        uint8_t* buffer = new uint8_t[bufferSize];
        XGameUiWebAuthenticationResultData* authResult = nullptr;
        if (SUCCEEDED(hr)) {
            hr = XGameUiShowWebAuthenticationResult(async, bufferSize, buffer, &authResult, nullptr);
        }

        Dictionary return_data;
        if (SUCCEEDED(hr)) {
            Dictionary return_auth;
            return_auth["response_status"] = (int64_t)authResult->responseStatus;
            return_auth["response_completion_uri"] = String(authResult->responseCompletionUri);
            return_data["result_data"] = return_auth;
        }

        delete[] buffer;
        buffer = nullptr;

        return_data["hresult"] = hr;
        wrapper->emit(return_data);
    });

    HRESULT hr = XGameUiShowWebAuthenticationAsync( asyncBlock->get_block(), 
                                                    requesting_user->get_user(), 
                                                    request_uri.utf8().get_data(), 
                                                    completion_uri.utf8().get_data());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XGameUiShowWebAuthenticationAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

Ref<GDKAsyncBlock> GDKGameUI::show_web_authentication_with_options_async(Ref<GDKUser> requesting_user, const String &request_uri, const String &completion_uri, bool is_fullscreen) {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        size_t bufferSize = 0;
        HRESULT hr = XGameUiShowWebAuthenticationResultSize(async, &bufferSize);

        uint8_t* buffer = new uint8_t[bufferSize];
        XGameUiWebAuthenticationResultData* authResult = nullptr;
        if (SUCCEEDED(hr)) {
            hr = XGameUiShowWebAuthenticationResult(async, bufferSize, buffer, &authResult, nullptr);
        }

        Dictionary return_data;
        if (SUCCEEDED(hr)) {
            Dictionary return_auth;
            return_auth["response_status"] = (int64_t)authResult->responseStatus;
            return_auth["response_completion_uri"] = String(authResult->responseCompletionUri);
            return_data["result_data"] = return_auth;
        }

        delete[] buffer;
        buffer = nullptr;

        return_data["hresult"] = hr;
        wrapper->emit(return_data);
    });

    XGameUiWebAuthenticationOptions options = is_fullscreen ? XGameUiWebAuthenticationOptions::PreferFullscreen 
                                                            : XGameUiWebAuthenticationOptions::None;
    HRESULT hr = XGameUiShowWebAuthenticationWithOptionsAsync(  asyncBlock->get_block(), 
                                                                requesting_user->get_user(),
                                                                request_uri.utf8().get_data(), 
                                                                completion_uri.utf8().get_data(),
                                                                options);
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XGameUiShowWebAuthenticationAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

Ref<GDKGameUITextEntryHandle> GDKGameUI::text_entry_open(Ref<GDKXGameUITextEntryOptions> options, int max_length, const String &initial_text, int initial_cursor_index) {
	XGameUiTextEntryOptions opts;
    opts.inputScope = (XGameUiTextEntryInputScope)options->get_input_scope();
    opts.positionHint = (XGameUiTextEntryPositionHint)options->get_position_hint();
    opts.visibilityFlags = (XGameUiTextEntryVisibilityFlags)((int64_t)options->get_visibility_flags());

    XGameUiTextEntryHandle handle = nullptr;
    HRESULT hr = XGameUiTextEntryOpen(&opts, (uint32_t)max_length, initial_text.utf8().get_data(), (uint32_t)initial_cursor_index, &handle);
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XGameUiTextEntryOpen Error: 0x%08ux", (uint64_t)hr));
    return GDKGameUITextEntryHandle::create(handle);
}
