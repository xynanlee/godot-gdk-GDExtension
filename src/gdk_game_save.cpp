#include <core/class_db.hpp>
#include <templates/local_vector.hpp>
#include "gdk_game_save.h"
#include "gdk_game_save_blob.h"
#include "gdk_helpers.h"

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

void GDKGameSaveContainer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("create_update"), &GDKGameSaveContainer::create_update);
	ClassDB::bind_method(D_METHOD("enumerate_blob_info"), &GDKGameSaveContainer::enumerate_blob_info);
	ClassDB::bind_method(D_METHOD("enumerate_blob_info_by_name", "prefix"), &GDKGameSaveContainer::enumerate_blob_info_by_name);
	ClassDB::bind_method(D_METHOD("read_blob_data", "blob_names", "blob_size"), &GDKGameSaveContainer::read_blob_data);
	ClassDB::bind_method(D_METHOD("read_blob_data_async", "blob_names"), &GDKGameSaveContainer::read_blob_data_async);
}

void GDKGameSaveContainer::_notification(int p_what) {
	if (p_what == NOTIFICATION_PREDELETE) {
		if (_container_handle) {
			XGameSaveCloseContainer(_container_handle);
			_container_handle = nullptr;
		}
	}
}

Ref<GDKGameSaveContainer> GDKGameSaveContainer::create(XGameSaveContainerHandle containerHandle, const String& name) {
	Ref<GDKGameSaveContainer> wrapper;
	if (containerHandle) {
		wrapper.instantiate();
		wrapper->_container_handle = containerHandle;
		wrapper->_name = name;
	}
	return wrapper;
}

Ref<GDKGameSaveUpdate> GDKGameSaveContainer::create_update() {
	XGameSaveUpdateHandle updateHandle;
	
	const char* containerName = _name.utf8().get_data();
	HRESULT hr = XGameSaveCreateUpdate(_container_handle, containerName, &updateHandle);
	ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XGameSaveCreateUpdate Error: 0x%08ux", (uint64_t)hr));
	return GDKGameSaveUpdate::create(updateHandle);
}

TypedDictionary<String, int> GDKGameSaveContainer::enumerate_blob_info() const {
	TypedDictionary<String, int> data;

	HRESULT hr = XGameSaveEnumerateBlobInfo(_container_handle, &data, 
		[](const XGameSaveBlobInfo* info, void* context) -> bool {
			TypedDictionary<String, int>* data = reinterpret_cast<TypedDictionary<String, int>*>(context);
			String objectName = String(info->name);
			data->set(objectName, info->size);
			return true;
		});
	
	if (FAILED(hr)) {
		ERR_PRINT(vformat("XGameSaveEnumerateBlobInfo Error: 0x%08ux", (uint64_t)hr));
		return TypedDictionary<String, int>();
	}

	return data;
}

TypedDictionary<String, int> godot::GDKGameSaveContainer::enumerate_blob_info_by_name(const String& prefix) const {
	TypedDictionary<String, int> data;

	const char* prefixString = prefix.utf8().get_data();
	HRESULT hr = XGameSaveEnumerateBlobInfoByName(_container_handle, prefixString, &data,
		[](const XGameSaveBlobInfo* info, void* context) -> bool {
			TypedDictionary<String, int>* data = reinterpret_cast<TypedDictionary<String, int>*>(context);
			String objectName = String(info->name);
			data->set(objectName, info->size);
			return true;
		});

	if (FAILED(hr)) {
		ERR_PRINT(vformat("XGameSaveEnumerateBlobInfoByName Error: 0x%08ux", (uint64_t)hr));
		return TypedDictionary<String, int>();
	}

	return data;
}

Dictionary GDKGameSaveContainer::read_blob_data(PackedStringArray blobNames, int blobSize) {
	Dictionary result;
	uint32_t blobCount = blobNames.size();
	
	LocalVector<const char*> blobNameVector;
	blobNameVector.reserve(blobCount);
	for (int i = 0; i < blobCount; i++) {
		blobNameVector.push_back(blobNames.get(i).utf8().get_data());
	}

	HRESULT hr = S_OK;
	XGameSaveBlob* saveBlobs = reinterpret_cast<XGameSaveBlob*>(malloc(blobSize));
	if (saveBlobs == nullptr) {
		hr = E_OUTOFMEMORY;
	}

	if (SUCCEEDED(hr)) {
		hr = XGameSaveReadBlobData(_container_handle, blobNameVector.ptr(), &blobCount, blobSize, saveBlobs);	
	}
	
	TypedArray<GDKGameSaveBlob> blobArray;
	if (SUCCEEDED(hr)) {
		blobArray.resize(blobCount);
		for (uint32_t i = 0; i < blobCount; i++) {
			blobArray.push_back(memnew(GDKGameSaveBlob(&saveBlobs[i])));
		}
	}

	switch (hr) {
		case E_GS_INVALID_CONTAINER_NAME:
			ERR_PRINT(vformat("XGameSaveReadBlobData Error: Invalid container name (0x%08ux)", (uint64_t)hr));
			break;
		case E_GS_PROVIDED_BUFFER_TOO_SMALL:
			ERR_PRINT(vformat("XGameSaveReadBlobData Error: Provided buffer too small (0x%08ux)", (uint64_t)hr));
			break;
		case E_GS_BLOB_NOT_FOUND:
			ERR_PRINT(vformat("XGameSaveReadBlobData Error: Blob not found (0x%08ux)", (uint64_t)hr));
			break;
		case E_GS_CONTAINER_NOT_IN_SYNC:
			WARN_PRINT(vformat("XGameSaveReadBlobData Error: Container not in sync (0x%08ux)", (uint64_t)hr));
			break;
		case E_GS_CONTAINER_SYNC_FAILED:
			ERR_PRINT(vformat("XGameSaveReadBlobData Error: Container sync failed (0x%08ux)", (uint64_t)hr));
			break;
		case E_GS_HANDLE_EXPIRED:
			ERR_PRINT(vformat("XGameSaveReadBlobData Error: Handle expired (0x%08ux)", (uint64_t)hr));
			break;
		case S_OK:
			break;
		default:
			ERR_PRINT(vformat("XGameSaveReadBlobData Error: 0x%08ux", (uint64_t)hr));
	}

	result["hresult"] = hr;

	if (saveBlobs != nullptr) {
		::free(saveBlobs);
	}
	
	if (SUCCEEDED(hr)){
		result["blobs"] = blobArray;
	}

	return result;
}

Ref<GDKAsyncBlock> GDKGameSaveContainer::read_blob_data_async(PackedStringArray blobNames) {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
	asyncBlock->set_callback([](XAsyncBlock* async) {
		GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
		Dictionary return_data;
		size_t allocatedSize = 0;
		XGameSaveBlob* saveBlobs = nullptr;
		TypedArray<GDKGameSaveBlob> blobArray;
		uint32_t blobCount = 0;

		HRESULT hr = XAsyncGetResultSize(async, &allocatedSize);
		return_data["hresult"] = hr;
		if (SUCCEEDED(hr) && allocatedSize > 0) {
			saveBlobs = reinterpret_cast<XGameSaveBlob*>(malloc(allocatedSize));
			if (saveBlobs == nullptr) {
				hr = E_OUTOFMEMORY;
			}
		}

		if (SUCCEEDED(hr)) {
			hr = XGameSaveReadBlobDataResult(async, allocatedSize, saveBlobs, &blobCount);
		}
		
		if (SUCCEEDED(hr)) {
			blobArray.resize(blobCount);
			for (uint32_t i = 0; i < blobCount; i++) {
				blobArray.push_back(memnew(GDKGameSaveBlob(&saveBlobs[i])));
			}
		}

		switch (hr) {
			case E_GS_INVALID_CONTAINER_NAME:
				ERR_PRINT(vformat("XGameSaveReadBlobDataAsyncResult Error: Invalid container name (0x%08ux)", (uint64_t)hr));
				break;
			case E_GS_PROVIDED_BUFFER_TOO_SMALL:
				ERR_PRINT(vformat("XGameSaveReadBlobDataAsyncResult Error: Provided buffer too small (0x%08ux)", (uint64_t)hr));
				break;
			case E_GS_BLOB_NOT_FOUND:
				ERR_PRINT(vformat("XGameSaveReadBlobDataAsyncResult Error: Blob not found (0x%08ux)", (uint64_t)hr));
				break;
			case E_GS_CONTAINER_NOT_IN_SYNC:
				WARN_PRINT(vformat("XGameSaveReadBlobDataAsyncResult Error: Container not in sync (0x%08ux)", (uint64_t)hr));
				break;
			case E_GS_CONTAINER_SYNC_FAILED:
				ERR_PRINT(vformat("XGameSaveReadBlobDataAsyncResult Error: Container sync failed (0x%08ux)", (uint64_t)hr));
				break;
			case E_GS_HANDLE_EXPIRED:
				ERR_PRINT(vformat("XGameSaveReadBlobDataAsyncResult Error: Handle expired (0x%08ux)", (uint64_t)hr));
				break;
			case S_OK:
				break;
			default:
				ERR_PRINT(vformat("XGameSaveReadBlobDataAsyncResult Error: 0x%08ux", (uint64_t)hr));
		}

		if (saveBlobs != nullptr) {
			::free(saveBlobs);
		}

		return_data["hresult"] = hr;
		if (SUCCEEDED(hr)) {
			return_data["blobs"] = blobArray;
		}
		wrapper->emit(return_data);
	});

	uint32_t initialBlobCount = blobNames.size();
	LocalVector<const char*> blobNameVector;
	blobNameVector.reserve(initialBlobCount);
	for (int i = 0; i < initialBlobCount; i++) {
		blobNameVector.push_back(blobNames.get(i).utf8().get_data());
	}

	HRESULT hr = XGameSaveReadBlobDataAsync(_container_handle, blobNameVector.ptr(), initialBlobCount, asyncBlock->get_block());
	ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XGameSaveReadBlobDataAsync Error: 0x%08ux", (uint64_t)hr));
	return asyncBlock;
}

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

Ref<GDKAsyncBlock> godot::GDKGameSaveProvider::get_remaining_quota_async() const {
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