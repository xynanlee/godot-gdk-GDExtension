#include "gdk_stringverify.h"
#include "gdk_helpers.h"
#include <templates/local_vector.hpp>
#include <variant/typed_array.hpp>

using namespace godot;

void GDKXblStringVerifyResult::_bind_methods() {
    BIND_ENUM_CONSTANT(Success);
    BIND_ENUM_CONSTANT(Offensive);
    BIND_ENUM_CONSTANT(TooLong);
    BIND_ENUM_CONSTANT(UnknownError);

    ClassDB::bind_method(D_METHOD("get_result"), &GDKXblStringVerifyResult::get_result);
    ClassDB::bind_method(D_METHOD("get_first_offending_substring"), &GDKXblStringVerifyResult::get_first_offending_substring);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "result", PROPERTY_HINT_ENUM, "Success,Offensive,TooLong,UnknownError", PROPERTY_USAGE_DEFAULT, "GDKXblStringVerifyResult::Enum"), "", "get_result");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "first_offending_substring"), "", "get_first_offending_substring");
}

Ref<GDKXblStringVerifyResult> GDKXblStringVerifyResult::create(XblVerifyStringResult *in_result) {
	Ref<GDKXblStringVerifyResult> verify_result;
    
    if (in_result != nullptr) {
        verify_result.instantiate();
        verify_result->result = (GDKXblStringVerifyResult::Enum) in_result->resultCode;
        verify_result->first_offending_substring = String(in_result->firstOffendingSubstring);
    }
    
    return verify_result;
}

void GDKStringVerify::_bind_methods() {
    ClassDB::bind_static_method(get_class_static(), D_METHOD("verify_string_async", "user", "string_to_verify"), &GDKStringVerify::verify_string_async);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("verify_strings_async", "user", "strings_to_verify"), &GDKStringVerify::verify_strings_async);
}

Ref<GDKAsyncBlock> GDKStringVerify::verify_string_async(Ref<GDKUser> user, const String &string_to_verify) {
    Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        size_t bufferSize = 0;
        HRESULT hr = XblStringVerifyStringResultSize(async, &bufferSize);
        
        LocalVector<char> buffer;
        XblVerifyStringResult* result = nullptr;
        Dictionary return_data;
        if (SUCCEEDED(hr)) {
            buffer.reserve(bufferSize);
            hr = XblStringVerifyStringResult(async, bufferSize, buffer.ptr(), &result, nullptr);
        }
        
        if (SUCCEEDED(hr) && result != nullptr) {
           return_data["result"] = GDKXblStringVerifyResult::create(result); 
        }

        return_data["hresult"] = hr;
        wrapper->emit(return_data);
    });

    HRESULT hr = XblStringVerifyStringAsync(user->get_xbox_live_context(), string_to_verify.utf8().get_data(), asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XblStringVerifyStringAsync Error: 0x%08ux", (uint64_t)hr));
	return asyncBlock;
}

Ref<GDKAsyncBlock> GDKStringVerify::verify_strings_async(Ref<GDKUser> user, PackedStringArray strings_to_verify) {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        size_t bufferSize = 0;
        HRESULT hr = XblStringVerifyStringsResultSize(async, &bufferSize);

        LocalVector<char> buffer;
        XblVerifyStringResult* results = nullptr;
        size_t resultCount = 0;
        Dictionary return_data;
        if (SUCCEEDED(hr)) {
            buffer.reserve(bufferSize);
            hr = XblStringVerifyStringsResult(async, bufferSize, buffer.ptr(), &results, &resultCount, nullptr);
        }

        if (SUCCEEDED(hr) && resultCount > 0) {
            TypedArray<Ref<GDKXblStringVerifyResult>> return_results;
            for (size_t i = 0; i < resultCount; i++) {
                return_results.push_back(GDKXblStringVerifyResult::create(&results[i]));
            }

            return_data["results"] = return_results;
        }

        return_data["hresult"] = hr;
        wrapper->emit(return_data);
    });
    
    LocalVector<const char*> strings;
    strings.reserve(strings_to_verify.size());

    for (int i = 0; i < strings_to_verify.size(); i++) {
        strings.push_back(strings_to_verify[i].utf8().get_data());
    }

    HRESULT hr = XblStringVerifyStringsAsync(user->get_xbox_live_context(), strings.ptr(), strings_to_verify.size(), asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XblStringVerifyStringsAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}
