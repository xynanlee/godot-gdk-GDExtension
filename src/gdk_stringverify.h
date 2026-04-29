#pragma once

#include <classes/ref_counted.hpp>
#include "gdk_asyncblock.h"
#include "gdk_user.h"

extern "C" {
    #include <xsapi-c/string_verify_c.h>
}

namespace godot {
class GDKXblStringVerifyResult: public RefCounted {
    GDCLASS(GDKXblStringVerifyResult, RefCounted);

public:
    enum Enum : uint32_t {
        Success = XblVerifyStringResultCode::Success,
        Offensive = XblVerifyStringResultCode::Offensive,
        TooLong = XblVerifyStringResultCode::TooLong,
        UnknownError = XblVerifyStringResultCode::UnknownError
    };

private:
    GDKXblStringVerifyResult::Enum result = Enum::Success;
    String first_offending_substring;

protected:
    static void _bind_methods();

public:
    static Ref<GDKXblStringVerifyResult> create(XblVerifyStringResult* in_result);
    inline GDKXblStringVerifyResult::Enum get_result() const { return result; }
    inline String get_first_offending_substring() const { return first_offending_substring; }
};


class GDKStringVerify : public RefCounted {
    GDCLASS(GDKStringVerify, RefCounted);

protected:
    static void _bind_methods();

public:
    static Ref<GDKAsyncBlock> verify_string_async(Ref<GDKUser> user, const String& string_to_verify);
    static Ref<GDKAsyncBlock> verify_strings_async(Ref<GDKUser> user, PackedStringArray strings_to_verify);
};
}

VARIANT_ENUM_CAST(GDKXblStringVerifyResult::Enum);