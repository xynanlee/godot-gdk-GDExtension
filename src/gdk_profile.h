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
    int64_t _xbox_user_id = 0;
    String _app_display_name;
    String _app_display_picture_resize_uri;
    String _game_display_name;
    String _game_display_picture_resize_uri;
    String _gamerscore;
    String _gamertag;
    String _modern_gamertag;
    String _modern_gamertag_suffix;
    String _unique_modern_gamertag;

protected:
    static void _bind_methods();

public:
    static Ref<GDKProfile> create(XblUserProfile* profile);
    static Ref<GDKAsyncBlock> get_user_profile_async(Ref<GDKUser> caller_user, int64_t xbox_user_id);
    static Ref<GDKAsyncBlock> get_user_profiles_async(Ref<GDKUser> caller_user, PackedInt64Array xbox_user_ids);
    static Ref<GDKAsyncBlock> get_user_profiles_for_social_group_async(Ref<GDKUser> caller_user, const String& social_group);

    int64_t get_xbox_user_id() const { return _xbox_user_id; }
    String get_app_display_name() const { return _app_display_name; }
    String get_app_display_picture_resize_uri() const { return _app_display_picture_resize_uri; }
    String get_game_display_name() const { return _game_display_name; }
    String get_game_display_picture_resize_uri() const { return _game_display_picture_resize_uri; }
    String get_gamerscore() const { return _gamerscore; }
    String get_gamertag() const { return _gamertag; }
    String get_modern_gamertag() const { return _modern_gamertag; }
    String get_modern_gamertag_suffix() const { return _modern_gamertag_suffix; }
    String get_unique_modern_gamertag() const { return _unique_modern_gamertag; }
};
}