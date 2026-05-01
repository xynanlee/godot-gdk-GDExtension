#pragma once

#include <classes/ref_counted.hpp>
#include <variant/typed_array.hpp>
#include <xsapi-c/profile_c.h>
#include "gdk_asyncblock.h"
#include "gdk_user.h"

namespace godot {
class GDKProfile: public RefCounted {
    GDCLASS(GDKProfile, RefCounted);

private:
    int64_t xbox_user_id = 0;
    String app_display_name;
    String app_display_picture_resize_uri;
    String game_display_name;
    String game_display_picture_resize_uri;
    String gamerscore;
    String gamertag;
    String modern_gamertag;
    String modern_gamertag_suffix;
    String unique_modern_gamertag;

protected:
    static void _bind_methods();

public:
    static Ref<GDKProfile> create(XblUserProfile* profile);
    static Ref<GDKAsyncBlock> get_user_profile_async(Ref<GDKUser> caller_user, int64_t xbox_user_id);
    static Ref<GDKAsyncBlock> get_user_profiles_async(Ref<GDKUser> caller_user, PackedInt64Array xbox_user_ids);
    static Ref<GDKAsyncBlock> get_user_profiles_for_social_group_async(Ref<GDKUser> caller_user, const String& social_group);

    inline int64_t get_xbox_user_id() const { return xbox_user_id; }
    inline String get_app_display_name() const { return app_display_name; }
    inline String get_app_display_picture_resize_uri() const { return app_display_picture_resize_uri; }
    inline String get_game_display_name() const { return game_display_name; }
    inline String get_game_display_picture_resize_uri() const { return game_display_picture_resize_uri; }
    inline String get_gamerscore() const { return gamerscore; }
    inline String get_gamertag() const { return gamertag; }
    inline String get_modern_gamertag() const { return modern_gamertag; }
    inline String get_modern_gamertag_suffix() const { return modern_gamertag_suffix; }
    inline String get_unique_modern_gamertag() const { return unique_modern_gamertag; }
};
}