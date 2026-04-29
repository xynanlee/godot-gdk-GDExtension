#include "gdk_game_save_provider.h"
#include "gdk_game_save_container.h"
#include "gdk_helpers.h"

using namespace godot;

void GDKGameSaveProvider::_bind_methods() {
	ClassDB::bind_method(D_METHOD("create_container", "container_name"), &GDKGameSaveProvider::create_container);
	ClassDB::bind_method(D_METHOD("delete_container", "container_name"), &GDKGameSaveProvider::delete_container);
	ClassDB::bind_method(D_METHOD("delete_container_async", "container_name"), &GDKGameSaveProvider::delete_container_async);
	ClassDB::bind_method(D_METHOD("enumerate_container_info"), &GDKGameSaveProvider::enumerate_container_info);
	ClassDB::bind_method(D_METHOD("enumerate_container_info_by_name", "prefix"), &GDKGameSaveProvider::enumerate_container_info_by_name);
	ClassDB::bind_method(D_METHOD("get_container_info", "container_name"), &GDKGameSaveProvider::get_container_info);
	ClassDB::bind_method(D_METHOD("get_remaining_quota"), &GDKGameSaveProvider::get_remaining_quota);
	ClassDB::bind_method(D_METHOD("get_remaining_quota_async"), &GDKGameSaveProvider::get_remaining_quota_async);
}

void GDKGameSaveProvider::_notification(int p_what) {
	if (p_what == NOTIFICATION_PREDELETE) {
		if (_save_provider_handle) {
			XGameSaveCloseProvider(_save_provider_handle);
			_save_provider_handle = nullptr;
		}
	}
}

Ref<GDKGameSaveProvider> GDKGameSaveProvider::create(XGameSaveProviderHandle in_handle) {
	Ref<GDKGameSaveProvider> wrapper;
	if (in_handle) {
		wrapper.instantiate();
		wrapper->_save_provider_handle = in_handle;
	}
	return wrapper;
}

Ref<GDKGameSaveContainer> GDKGameSaveProvider::create_container(const String & containerName){
	XGameSaveContainerHandle containerHandle;
	HRESULT hr = XGameSaveCreateContainer(_save_provider_handle, containerName.utf8().get_data(), &containerHandle);
	ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XGameSaveCreateContainer Error: 0x%08ux", (uint64_t)hr));
	return GDKGameSaveContainer::create(containerHandle, containerName);
}

void GDKGameSaveProvider::delete_container(const String &containerName) {
	HRESULT hr = XGameSaveDeleteContainer(_save_provider_handle, containerName.utf8().get_data());
	ERR_FAIL_COND_MSG(FAILED(hr), vformat("XGameSaveDeleteContainer Error: 0x%08ux", (uint64_t)hr));
}

Ref<GDKAsyncBlock> GDKGameSaveProvider::delete_container_async(const String &containerName) {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
	asyncBlock->set_callback([](XAsyncBlock* async) {
		GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
		HRESULT hr = XGameSaveDeleteContainerResult(async);
		Dictionary return_data;
		return_data["hresult"] = hr;
		wrapper->emit(return_data);
	});

	HRESULT hr = XGameSaveDeleteContainerAsync(_save_provider_handle, containerName.utf8().get_data(), asyncBlock->get_block());
	return asyncBlock;
}

Dictionary GDKGameSaveProvider::enumerate_container_info() const {
	Dictionary data;
	HRESULT hr = XGameSaveEnumerateContainerInfo(_save_provider_handle, &data,
		[](const XGameSaveContainerInfo* info, void* context) -> bool {
			Dictionary containerData;
			containerData["name"] = String(info->name);
			containerData["display_name"] = String(info->displayName);
			containerData["blob_count"] = info->blobCount;
			containerData["total_size"] = String::num_uint64(info->totalSize);
			containerData["last_modified_time"] = info->lastModifiedTime;
			containerData["needs_sync"] = info->needsSync;
			
			Dictionary* data = reinterpret_cast<Dictionary*>(context);
			data->set(String(info->name), containerData);
			return true;
		});

	if (FAILED(hr)) {
		ERR_PRINT(vformat("XGameSaveEnumerateContainerInfo Error: 0x%08ux", (uint64_t)hr));
		return Dictionary();
	}

	return data;
}

Dictionary GDKGameSaveProvider::enumerate_container_info_by_name(const String &prefix) const {
	Dictionary data;
	HRESULT hr = XGameSaveEnumerateContainerInfoByName(_save_provider_handle, prefix.utf8().get_data(), &data,
		[](const XGameSaveContainerInfo* info, void* context) -> bool {
			Dictionary containerData;
			containerData["name"] = String(info->name);
			containerData["display_name"] = String(info->displayName);
			containerData["blob_count"] = info->blobCount;
			containerData["total_size"] = String::num_uint64(info->totalSize);
			containerData["last_modified_time"] = info->lastModifiedTime;
			containerData["needs_sync"] = info->needsSync;

			Dictionary* data = reinterpret_cast<Dictionary*>(context);
			data->set(String(info->name), containerData);
			return true;
		});

	if (FAILED(hr)) {
		ERR_PRINT(vformat("XGameSaveEnumerateContainerInfoByName Error: 0x%08ux", (uint64_t)hr));
		return Dictionary();
	}

	return data;
}

Dictionary GDKGameSaveProvider::get_container_info(const String& containerName) const {
	Dictionary data;
	HRESULT hr = XGameSaveGetContainerInfo(_save_provider_handle, containerName.utf8().get_data(), &data,
		[](const XGameSaveContainerInfo* info, void* context) -> bool {
			Dictionary containerData;
			containerData["name"] = String(info->name);
			containerData["display_name"] = String(info->displayName);
			containerData["blob_count"] = info->blobCount;
			containerData["total_size"] = String::num_uint64(info->totalSize);
			containerData["last_modified_time"] = info->lastModifiedTime;
			containerData["needs_sync"] = info->needsSync;

			Dictionary* data = reinterpret_cast<Dictionary*>(context);
			*data = containerData;
			return true;
		});

	if (FAILED(hr)) {
		ERR_PRINT(vformat("XGameSaveGetContainerInfo Error: 0x%08ux", (uint64_t)hr));
		return Dictionary();
	}

	return data;
}

int64_t GDKGameSaveProvider::get_remaining_quota() const {
	int64_t remainingQuota = 0;
	HRESULT hr = XGameSaveGetRemainingQuota(_save_provider_handle, &remainingQuota);
	ERR_FAIL_COND_V_MSG(FAILED(hr), 0, vformat("XGameSaveGetRemainingQuota Error: 0x%08ux", (uint64_t)hr));
	return remainingQuota;
}

Ref<GDKAsyncBlock> GDKGameSaveProvider::get_remaining_quota_async() const {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
	asyncBlock->set_callback([](XAsyncBlock* async) {
		GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
		int64_t remainingQuota = 0;
		HRESULT hr = XGameSaveGetRemainingQuotaResult(async, &remainingQuota);
		Dictionary return_data;
		return_data["hresult"] = hr;

		if (SUCCEEDED(hr)) {
			return_data["remaining_quota"] = remainingQuota;
		}

		wrapper->emit(return_data);
	});

	HRESULT hr = XGameSaveGetRemainingQuotaAsync(_save_provider_handle, asyncBlock->get_block());
	ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XGameSaveGetRemainingQuotaAsync Error: 0x%08ux", (uint64_t)hr));
	return asyncBlock;
}