#include "gdk_game_save_update.h"
#include "gdk_helpers.h"
#include <Windows.h>

using namespace godot;

void GDKGameSaveUpdate::_bind_methods() {
	ClassDB::bind_method(D_METHOD("delete_blob", "blob_name"), &GDKGameSaveUpdate::delete_blob);
	ClassDB::bind_method(D_METHOD("write_blob", "blob_name", "data"), &GDKGameSaveUpdate::write_blob);
	ClassDB::bind_method(D_METHOD("submit_update"), &GDKGameSaveUpdate::submit_update);
	ClassDB::bind_method(D_METHOD("submit_update_async"), &GDKGameSaveUpdate::submit_update_async);
}

void GDKGameSaveUpdate::_notification(int p_what) {
	if (p_what == NOTIFICATION_PREDELETE) {
		if (_update_handle) {
			XGameSaveCloseUpdate(_update_handle);
			_update_handle = nullptr;
		}
	}
}

Ref<GDKGameSaveUpdate> GDKGameSaveUpdate::create(XGameSaveUpdateHandle updateHandle) {
	Ref<GDKGameSaveUpdate> wrapper;
	if (updateHandle) {
		wrapper.instantiate();
		wrapper->_update_handle = updateHandle;
	}
	return wrapper;
}

int GDKGameSaveUpdate::delete_blob(const String& blobName) const {
	const char* blobNameString = blobName.utf8().get_data();
	HRESULT hr = XGameSaveSubmitBlobDelete(_update_handle, blobNameString);
	ERR_FAIL_COND_V_MSG(FAILED(hr), hr, vformat("XGameSaveSubmitBlobDelete Error: 0x%08ux", (uint64_t)hr));
	return hr;
}

int GDKGameSaveUpdate::write_blob(const String &blobName, PackedByteArray data) const {
	const char* blobNameString = blobName.utf8().get_data();
	HRESULT hr = XGameSaveSubmitBlobWrite(_update_handle, blobNameString, data.ptr(), data.size());
	ERR_FAIL_COND_V_MSG(FAILED(hr), hr, vformat("XGameSaveSubmitBlobWrite Error: 0x%08ux", (uint64_t)hr));
	return hr;
}

int GDKGameSaveUpdate::submit_update() const {
	HRESULT hr = XGameSaveSubmitUpdate(_update_handle);
	ERR_FAIL_COND_V_MSG(FAILED(hr), hr, vformat("XGameSaveSubmitUpdate Error: 0x%08ux", (uint64_t)hr));
	return hr;
}

Ref<GDKAsyncBlock> GDKGameSaveUpdate::submit_update_async() const {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
	asyncBlock->set_callback([](XAsyncBlock* async){
		GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
		Dictionary return_data;
		HRESULT hr = XGameSaveSubmitUpdateResult(async);
		return_data["hresult"] = hr;
		wrapper->emit(return_data);
	});

	HRESULT hr = XGameSaveSubmitUpdateAsync(_update_handle, asyncBlock->get_block());
	ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XGameSaveSubmitUpdateAsync Error: 0x%08ux", (uint64_t)hr));
	return asyncBlock;
}