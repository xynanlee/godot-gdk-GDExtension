#pragma once

#include <Windows.h>
#include <winapifamily.h>
#include <objbase.h>
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/variant.hpp"
#include <XAsync.h>
#include <XTaskQueue.h>
#include <XUser.h>
#include <xsapi-c/types_c.h>

#include <string>

using namespace godot;

class GodotGDK : public RefCounted {
	GDCLASS(GodotGDK, RefCounted)

private:
	static HRESULT Identity_TrySignInDefaultUserSilently(XTaskQueueHandle asyncQueue, Callable cb);
	static void Identity_TrySignInDefaultUserSilently_Callback(XAsyncBlock *asyncBlock);

protected:
	static void _bind_methods();

public:
	GodotGDK() = default;
	~GodotGDK() override = default;

	int InitializeGDK(Callable, String);
	static bool CreateContextHandle(XblContextHandle* handle);
	static XTaskQueueHandle GetQueueHandle();

	static XUserLocalId GetUserId();
	static XUserHandle GetUserHandle();
	static XAsyncBlock* GodotGDK::CreateAsyncBlock();
	static const char* GetSCID();
	static bool CheckResult(HRESULT result, std::string succeedMessage, std::string errorMessage);
	static char* CopyStringToChar(String string);
};
