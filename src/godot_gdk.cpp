#include "godot_gdk.h"

#include <Windows.h>
#include <winapifamily.h>
#include <objbase.h>
#include <XGameRuntimeInit.h>
#include <XTaskQueue.h>
#include <xsapi-c/services_c.h>
#include <XUser.h>

#include <iomanip>
#include <ios>
#include <sstream>
#include <string>

XTaskQueueHandle queue = nullptr;
uint64_t xboxUserId;

void godot_gdk::_bind_methods() {
	godot::ClassDB::bind_method(D_METHOD("print_type", "variant"), &godot_gdk::print_type);
	godot::ClassDB::bind_method(D_METHOD("InitializeGDK"), &godot_gdk::InitializeGDK);
}

void godot_gdk::print_type(const Variant &p_variant) const {
	print_line(vformat("Type: %d", p_variant.get_type()));
}

int godot_gdk::InitializeGDK(const String SCID) {

	HRESULT hr = XGameRuntimeInitialize();
	if (FAILED(hr)) {
		ERR_PRINT("Failed to initialize Xbox Game Runtime (GDK).");
		return hr;
	}

	print_line("Successfully initialized GDK");

	hr = XTaskQueueCreate(
		XTaskQueueDispatchMode::ThreadPool,
		XTaskQueueDispatchMode::ThreadPool,
		&queue);

	if (FAILED(hr))
	{
		ERR_PRINT("Failed to initialize Task Queue");
		return hr;
	}
	print_line("Successfully initialized Task Queue");

	std::string native = SCID.utf8().get_data();

	XblInitArgs xblArgs = {};
	xblArgs.queue = queue;
	xblArgs.scid = native.c_str();

	hr = XblInitialize(&xblArgs);
	if (FAILED(hr))
	{
		ERR_PRINT("Failed to initialize Xbox services");
		return hr;
	}

	print_line("Successfully initialized xbox services");
	hr = Identity_TrySignInDefaultUserSilently(queue);
	if (FAILED(hr)) {
		ERR_PRINT("Failed to start login");
	}else {
		print_line("Login started");
	}

	return hr;
}


HRESULT godot_gdk::Identity_TrySignInDefaultUserSilently(_In_ XTaskQueueHandle asyncQueue) const
{
	XAsyncBlock* asyncBlock = new XAsyncBlock();
	asyncBlock->queue = asyncQueue;
	asyncBlock->callback = Identity_TrySignInDefaultUserSilently_Callback;

	// Request to silently sign in the default user.
	HRESULT hr = XUserAddAsync(XUserAddOptions::AddDefaultUserSilently, asyncBlock);

	if (FAILED(hr))
	{
		ERR_PRINT("Failed to start login");
		delete asyncBlock;
	}

	return hr;
}

void CALLBACK godot_gdk::Identity_TrySignInDefaultUserSilently_Callback(_In_ XAsyncBlock* asyncBlock)
{
	// NOTE: XUserAddResult will add a Ref to the passed-in XUserHandle.
	XUserHandle newUser = nullptr;
	HRESULT hr = XUserAddResult(asyncBlock, &newUser);

	if (SUCCEEDED(hr))
	{
		print_line("Login successful");

		XblContextHandle handle;
		hr = XblContextCreateHandle(newUser, &handle);

		if (SUCCEEDED(hr)) {
			printf("Successfully created handle");

			XUserLocalId userID;
			hr = XUserGetLocalId(newUser, &userID);

			if (SUCCEEDED(hr)) {
				printf("Successfully retrieved userid");

			}else {
				std::ostringstream oss;
				oss << "Login failed. HRESULT=0x"
					<< std::hex << std::setw(8) << std::setfill('0') << hr;

				ERR_PRINT(oss.str().c_str());
			}
		}else {
			std::ostringstream oss;
			oss << "Login failed. HRESULT=0x"
				<< std::hex << std::setw(8) << std::setfill('0') << hr;

			ERR_PRINT(oss.str().c_str());
		}
	}
	else
	{
		std::ostringstream oss;
		oss << "Login failed. HRESULT=0x"
			<< std::hex << std::setw(8) << std::setfill('0') << hr;

		ERR_PRINT(oss.str().c_str());
	}

	delete asyncBlock;
}
