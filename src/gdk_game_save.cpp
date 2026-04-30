#include "gdk_game_save.h"
#include <templates/local_vector.hpp>
#include <Windows.h>
#include <XGameSave.h>
#include "gdk_game_save_provider.h"
#include "gdk_helpers.h"

using namespace godot;

void GDKGameSave::_bind_methods() {
	ClassDB::bind_static_method(get_class_static(), D_METHOD("initialize_game_save_provider", "syncOnDemand", "user"), &GDKGameSave::initialize_game_save_provider);
	ClassDB::bind_static_method(get_class_static(), D_METHOD("initialize_game_save_provider_async", "syncOnDemand", "user"), &GDKGameSave::initialize_game_save_provider_async);
}

Ref<GDKGameSaveProvider> GDKGameSave::initialize_game_save_provider(bool syncOnDemand, Ref<GDKUser> user) {
	XGameSaveProviderHandle providerHandle = nullptr;

	const char* scid = GDKHelpers::get_scid().utf8().get_data();
	HRESULT hr = XGameSaveInitializeProvider(user->get_user(), scid, syncOnDemand, &providerHandle);
	ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XGameSaveInitializeProvider Error: 0x%08ux", (uint64_t)hr));

	return GDKGameSaveProvider::create(providerHandle);
}

Ref<GDKAsyncBlock> GDKGameSave::initialize_game_save_provider_async(bool syncOnDemand, Ref<GDKUser> user) {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
	asyncBlock->set_callback([](XAsyncBlock* async) {
		GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);

		XGameSaveProviderHandle providerHandle = nullptr;
		HRESULT hr = XGameSaveInitializeProviderResult(async, &providerHandle);
		Dictionary return_data;
		return_data["hresult"] = hr;
		if (SUCCEEDED(hr)) {
			return_data["provider"] = GDKGameSaveProvider::create(providerHandle);
		}

		wrapper->emit(return_data);
	});

	const char* scid = GDKHelpers::get_scid().utf8().get_data();
	HRESULT hr = XGameSaveInitializeProviderAsync(user->get_user(), scid, syncOnDemand, asyncBlock->get_block());
	ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XGameSaveInitializeProviderAsync Error: 0x%08ux", (uint64_t)hr));
	return asyncBlock;
}