#include "godot_gdk.h"

#include "godot_cpp/core/class_db.hpp"

#include <Windows.h>
#include <winapifamily.h>
#include <objbase.h>
#include <XGameRuntimeInit.h>
#include <XTaskQueue.h>
#include <xsapi-c/services_c.h>
#include <XUser.h>

#include <iomanip>
#include <sstream>
#include <string>

static XTaskQueueHandle queue;
static XUserLocalId xboxUserId;
static XUserHandle xboxUserHandle;
static const char* SCID;

GodotGDK* GodotGDK::_instance = nullptr;

void GodotGDK::_bind_methods() {
	ClassDB::bind_method(D_METHOD("InitializeGDK", "callback", "SCID"), &GodotGDK::InitializeGDK);
}

void GodotGDK::_notification(int p_what) {
	if (p_what == NOTIFICATION_PREDELETE) {
		if (_initialized) {
			XTaskQueueCloseHandle(queue);
			XGameRuntimeUninitialize();
		}
	}
}

GodotGDK *GodotGDK::get_singleton() {
	if (_instance) {
		return _instance;
	}

	_instance = memnew(GodotGDK);
	return _instance;
}

int GodotGDK::InitializeGDK(Callable cb, String scid) {

	HRESULT hr = XGameRuntimeInitialize();
	if (!CheckResult(hr,"Successfully initialized GDK", "Failed to initialize Xbox Game Runtime (GDK).")) {
		return hr;
	}

	XblInitArgs xblArgs = {};
	xblArgs.queue = queue;

	SCID = GodotGDK::CopyStringToChar(scid);
	xblArgs.scid = SCID;

	hr = XblInitialize(&xblArgs);
	if (!CheckResult(hr, "Successfully initialized xbox services", "Failed to initialize Xbox services"))
	{
		return hr;
	}

	queue = XblGetAsyncQueue();

	hr = Identity_TrySignInDefaultUserSilently(queue, cb);
	CheckResult(hr, "Login successfully started", "Failed to start login");

	return hr;
}


HRESULT GodotGDK::Identity_TrySignInDefaultUserSilently(_In_ XTaskQueueHandle asyncQueue, Callable cb) {
	XAsyncBlock* asyncBlock = new XAsyncBlock();
	asyncBlock->queue = asyncQueue;
	Callable* cb_ptr = new Callable(cb);
	asyncBlock->context = cb_ptr;
	asyncBlock->callback = Identity_TrySignInDefaultUserSilently_Callback;

	// Request to silently sign in the default user.
	HRESULT hr = XUserAddAsync(XUserAddOptions::AddDefaultUserSilently, asyncBlock);

	if (!CheckResult(hr, "Successfully started login", "Failed to start login"))
	{
		delete asyncBlock;
	}

	return hr;
}

void CALLBACK GodotGDK::Identity_TrySignInDefaultUserSilently_Callback(_In_ XAsyncBlock* asyncBlock)
{
	HRESULT hr = XUserAddResult(asyncBlock, &xboxUserHandle);

	if (!CheckResult(hr, "Login successful", "Login failed")) {
		delete asyncBlock;
		return;
	}

	XblContextHandle handle;

	if (FAILED(XblContextCreateHandle(GodotGDK::GetUserHandle(), &handle))) {
		delete asyncBlock;
		return;
	}

	hr = XUserGetLocalId(xboxUserHandle, &xboxUserId);

	if (!CheckResult(hr, "Successfully retrieved userid", "Failed to retrieve userid")) {
		delete asyncBlock;
		return;
	}

	Callable* callback = static_cast<Callable*>(asyncBlock->context);

	if (callback && callback->is_valid()) {
		callback->call_deferred();
	}

	delete callback;
	delete asyncBlock;
}

XTaskQueueHandle GodotGDK::GetQueueHandle() {
	return queue;
}

XUserLocalId  GodotGDK::GetUserId() {
	return xboxUserId;
}

XUserHandle GodotGDK::GetUserHandle() {
	return xboxUserHandle;
}

XAsyncBlock* GodotGDK::CreateAsyncBlock() {
	XAsyncBlock* asyncBlock = new XAsyncBlock();
	XTaskQueueHandle queue_handle = GodotGDK::GetQueueHandle();
	asyncBlock->queue = queue_handle;
	return asyncBlock;
}

const char* GodotGDK::GetSCID() {
	return SCID;
}

bool GodotGDK::CheckResult(HRESULT result, String succeedMessage, String errorMessage) {
	if (FAILED(result)) {
		String resultString = "HRESULT=0x%X";
		ERR_PRINT(errorMessage + ", " + resultString % (int64_t)(unsigned int)result);
		return false;
	}

	if(succeedMessage != "") {
		print_line(succeedMessage);
	}

	return true;
}

char* GodotGDK::CopyStringToChar(String string) {
	CharString cs = string.utf8();
	int len = cs.length();

	char* out = new char[len + 1];
	memcpy(out, cs.get_data(), len);
	out[len] = '\0';

	return out;
}
