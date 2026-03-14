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
#include <XGame.h>
#include <XGameInvite.h>
#include <xsapi-c/types_c.h>

#include <string>

using namespace godot;

namespace godot { class GDKUser; }

class GodotGDK : public RefCounted {
	GDCLASS(GodotGDK, RefCounted)

private:
	static GodotGDK* _instance;
	bool _initialized = false;
	XTaskQueueHandle _async_queue = nullptr;
	XTaskQueueRegistrationToken _invite_token = {};
	bool _invite_registered = false;

	static HRESULT Identity_TrySignInDefaultUserSilently(XTaskQueueHandle asyncQueue, Callable cb);
	static void Identity_TrySignInDefaultUserSilently_Callback(XAsyncBlock *asyncBlock);
	static void OnGameInviteReceived(void* context, const char* inviteUri);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	static GodotGDK* get_singleton();
	GodotGDK() = default;
	~GodotGDK() override = default;

	inline XTaskQueueHandle get_async_queue() { return _async_queue; }

	int InitializeGDK(Callable, String);
	static bool CreateContextHandle(XblContextHandle* handle);
	static XTaskQueueHandle GetQueueHandle();

	static XUserLocalId GetUserId();
	static XUserHandle GetUserHandle();
	static XAsyncBlock* GodotGDK::CreateAsyncBlock();
	static const char* GetSCID();
	static bool CheckResult(HRESULT result, std::string succeedMessage, std::string errorMessage);
	static char* CopyStringToChar(String string);

	// XGame
	static int get_xbox_title_id();
	static void launch_new_game(const String &exe_path, const String &args, Ref<GDKUser> default_user);
	static int launch_restart_on_crash(const String &args);
};
