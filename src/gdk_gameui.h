#pragma once

#include <classes/ref_counted.hpp>
#include <XGameUI.h>
#include "gdk_asyncblock.h"
#include "gdk_user.h"

namespace godot {

class GDKXGameUINotificationPositionHint: public Object {
    GDCLASS(GDKXGameUINotificationPositionHint, Object)

protected:
    static void _bind_methods();

public:
    enum Enum: uint32_t {
        BottomCenter = XGameUiNotificationPositionHint::BottomCenter,
        BottomLeft = XGameUiNotificationPositionHint::BottomLeft,
        BottomRight = XGameUiNotificationPositionHint::BottomRight,
        TopCenter = XGameUiNotificationPositionHint::TopCenter,
        TopLeft = XGameUiNotificationPositionHint::TopLeft,
        TopRight = XGameUiNotificationPositionHint::TopRight
    };
};

class GDKXGameUITextEntryInputScope: public Object {
    GDCLASS(GDKXGameUITextEntryInputScope, Object);

protected:
    static void _bind_methods();

public:
    enum Enum: uint32_t {
        Default = XGameUiTextEntryInputScope::Default,
        Url = XGameUiTextEntryInputScope::Url,
        EmailSmtpAddress = XGameUiTextEntryInputScope::EmailSmtpAddress,
        Number = XGameUiTextEntryInputScope::Number,
        Password = XGameUiTextEntryInputScope::Password,
        TelephoneNumber = XGameUiTextEntryInputScope::TelephoneNumber,
        Alphanumeric = XGameUiTextEntryInputScope::Alphanumeric,
        Search = XGameUiTextEntryInputScope::Search,
        ChatWithoutEmoji = XGameUiTextEntryInputScope::ChatWithoutEmoji
    }; 
};

class GDKXGameUITextEntryPositionHint: public Object {
    GDCLASS(GDKXGameUITextEntryPositionHint, Object);

protected:
    static void _bind_methods();

public:
    enum Enum: uint32_t {
        Bottom = XGameUiTextEntryPositionHint::Bottom,
        Top = XGameUiTextEntryPositionHint::Top
    };
};

class GDKXGameUITextEntryVisibilityFlags: public Object {
    GDCLASS(GDKXGameUITextEntryVisibilityFlags, Object);

protected:
    static void _bind_methods();

public:
    enum Enum: uint32_t {
        Default = XGameUiTextEntryVisibilityFlags::Default,
        OnlyShowCandidates = XGameUiTextEntryVisibilityFlags::OnlyShowCandidates
    };
};

class GDKXGameUITextEntryOptions: public RefCounted {
    GDCLASS(GDKXGameUITextEntryOptions, RefCounted);

private:
    GDKXGameUITextEntryInputScope::Enum _inputScope;
    GDKXGameUITextEntryPositionHint::Enum _positionHint;
    BitField<GDKXGameUITextEntryVisibilityFlags::Enum> _visibilityFlags;

protected:
    static void _bind_methods();

public:
    GDKXGameUITextEntryOptions();

    inline GDKXGameUITextEntryInputScope::Enum get_input_scope() const { return _inputScope; }
    inline void set_input_scope(GDKXGameUITextEntryInputScope::Enum inputScope) { _inputScope = inputScope; }

    inline GDKXGameUITextEntryPositionHint::Enum get_position_hint() const { return _positionHint; }
    inline void set_position_hint(GDKXGameUITextEntryPositionHint::Enum positionHint) { _positionHint = positionHint; }

    inline BitField<GDKXGameUITextEntryVisibilityFlags::Enum> get_visibility_flags() const { return _visibilityFlags; }
    inline void set_visibility_flags(BitField<GDKXGameUITextEntryVisibilityFlags::Enum> visibilityFlags) { _visibilityFlags = visibilityFlags; }
};


class GDKXGameUITextEntryChangeTypeFlags: public Object {
    GDCLASS(GDKXGameUITextEntryChangeTypeFlags, Object);

protected:
    static void _bind_methods();

public:
    enum Enum: uint32_t {
        None = XGameUiTextEntryChangeTypeFlags::None,
        TextChanged = XGameUiTextEntryChangeTypeFlags::TextChanged,
        Dismissed = XGameUiTextEntryChangeTypeFlags::Dismissed
    };
};

class GDKGameUITextEntryHandle: public RefCounted {
    GDCLASS(GDKGameUITextEntryHandle, RefCounted);

private:
    XGameUiTextEntryHandle _handle;

protected:
    static void _bind_methods();
    void _notification(int p_what);

public:
    static Ref<GDKGameUITextEntryHandle> create(const XGameUiTextEntryHandle& in_handle);

    void close();
    Vector4 get_extents() const;
    Dictionary get_state(int buffer_size) const;
    int update_position_hint(GDKXGameUITextEntryPositionHint::Enum position_hint);
    int update_visibility(BitField<GDKXGameUITextEntryVisibilityFlags::Enum> visibility_flags);

    inline XGameUiTextEntryHandle get_handle() const { return _handle; }
};

class GDKGameUI: public RefCounted {
    GDCLASS(GDKGameUI, RefCounted);

protected:
    static void _bind_methods();

public:
    static int set_notification_position_hint(GDKXGameUINotificationPositionHint::Enum hint);
    static Ref<GDKAsyncBlock> show_achievements_async(Ref<GDKUser> user);
    static Ref<GDKAsyncBlock> show_error_dialog_async(int error_code, const String& message);
    static Ref<GDKAsyncBlock> show_message_dialog_async(const String& title_text,
                                                        const String& content_text,
                                                        const String& first_button_text,
                                                        const String& second_button_text,
                                                        const String& third_button_text,
                                                        int default_button_index,
                                                        int cancel_button_index);
    static Ref<GDKAsyncBlock> show_multiplayer_activity_game_invite_async(Ref<GDKUser> user);
    static Ref<GDKAsyncBlock> show_player_picker_async( Ref<GDKUser> user,
                                                        const String& prompt_text,
                                                        PackedInt64Array select_from,
                                                        int min_selection,
                                                        int max_selection,
                                                        PackedInt64Array preselected = PackedInt64Array());
    static Ref<GDKAsyncBlock> show_player_profile_card_async(Ref<GDKUser> requesting_user, Ref<GDKUser> target_user);
    static Ref<GDKAsyncBlock> show_send_game_invite_async(  Ref<GDKUser> requesting_user,
                                                            const String& session_template,
                                                            const String& session_id,
                                                            const String& invitation_text = String(),
                                                            const String& custom_activation_context = String());
    static Ref<GDKAsyncBlock> show_state_share_async(Ref<GDKUser> requesting_user, const String& link_token);
    static Ref<GDKAsyncBlock> show_text_entry_async(GDKXGameUITextEntryInputScope::Enum input_scope,
                                                    int max_text_length,
                                                    const String& title_text = String(),
                                                    const String& description_text = String(),
                                                    const String& default_text = String());
    static Ref<GDKAsyncBlock> show_web_authentication_async(Ref<GDKUser> requesting_user, const String& request_uri, const String& completion_uri);
    static Ref<GDKAsyncBlock> show_web_authentication_with_options_async(Ref<GDKUser> requesting_user, const String& request_uri, const String& completion_uri, bool is_fullscreen); 
    static Ref<GDKGameUITextEntryHandle> text_entry_open(Ref<GDKXGameUITextEntryOptions> options, int max_length, const String& initial_text, int initial_cursor_index);
};
}

VARIANT_ENUM_CAST(GDKXGameUINotificationPositionHint::Enum);
VARIANT_ENUM_CAST(GDKXGameUITextEntryInputScope::Enum);
VARIANT_ENUM_CAST(GDKXGameUITextEntryPositionHint::Enum);
VARIANT_BITFIELD_CAST(GDKXGameUITextEntryVisibilityFlags::Enum);
VARIANT_BITFIELD_CAST(GDKXGameUITextEntryChangeTypeFlags::Enum);