#include "gdk_game_save_container.h"
#include "gdk_game_save_blob.h"
#include "gdk_game_save_blob_info.h"
#include "gdk_helpers.h"
#include "gdk_game_save_update.h"

using namespace godot;

void GDKGameSaveContainerInfo::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_name"), &GDKGameSaveContainerInfo::get_name);
	ClassDB::bind_method(D_METHOD("get_display_name"), &GDKGameSaveContainerInfo::get_display_name);
	ClassDB::bind_method(D_METHOD("get_blob_count"), &GDKGameSaveContainerInfo::get_blob_count);
	ClassDB::bind_method(D_METHOD("get_total_size"), &GDKGameSaveContainerInfo::get_total_size);
	ClassDB::bind_method(D_METHOD("get_last_modified_time"), &GDKGameSaveContainerInfo::get_last_modified_time);
	ClassDB::bind_method(D_METHOD("get_needs_sync"), &GDKGameSaveContainerInfo::get_needs_sync);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "", "get_name");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "display_name"), "", "get_display_name");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "blob_count"), "", "get_blob_count");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "total_size"), "", "get_total_size");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "last_modified_time"), "", "get_last_modified_time");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "needs_sync"), "", "get_needs_sync");
}

Ref<GDKGameSaveContainerInfo> GDKGameSaveContainerInfo::create(const XGameSaveContainerInfo *info) {
	Ref<GDKGameSaveContainerInfo> wrapper;
	if (info != nullptr) {
		wrapper.instantiate();
		wrapper->_name = info->name;
		wrapper->_display_name = info->displayName;
		wrapper->_blob_count = info->blobCount;
		wrapper->_total_size = info->totalSize;
		wrapper->_last_modified_time = info->lastModifiedTime;
		wrapper->_needs_sync = info->needsSync;
	}
	return wrapper;
}

void GDKGameSaveContainer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("create_update"), &GDKGameSaveContainer::create_update);
	ClassDB::bind_method(D_METHOD("enumerate_blob_info"), &GDKGameSaveContainer::enumerate_blob_info);
	ClassDB::bind_method(D_METHOD("enumerate_blob_info_by_name", "prefix"), &GDKGameSaveContainer::enumerate_blob_info_by_name);
	ClassDB::bind_method(D_METHOD("read_blob_data", "blob_infos"), &GDKGameSaveContainer::read_blob_data);
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

Ref<GDKGameSaveUpdate> GDKGameSaveContainer::create_update() const {
	XGameSaveUpdateHandle updateHandle;
	
	const char* containerName = _name.utf8().get_data();
	HRESULT hr = XGameSaveCreateUpdate(_container_handle, containerName, &updateHandle);
	ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XGameSaveCreateUpdate Error: 0x%08ux", (uint64_t)hr));
	return GDKGameSaveUpdate::create(updateHandle);
}

TypedArray<Ref<GDKGameSaveBlobInfo>> GDKGameSaveContainer::enumerate_blob_info() const {
	TypedArray<Ref<GDKGameSaveBlobInfo>> data;

	HRESULT hr = XGameSaveEnumerateBlobInfo(_container_handle, &data, 
		[](const XGameSaveBlobInfo* info, void* context) -> bool {
			TypedArray<Ref<GDKGameSaveBlobInfo>>* data = reinterpret_cast<TypedArray<Ref<GDKGameSaveBlobInfo>>*>(context);
			data->push_back(GDKGameSaveBlobInfo::create(info));
			return true;
		});
	
	if (FAILED(hr)) {
		ERR_PRINT(vformat("XGameSaveEnumerateBlobInfo Error: 0x%08ux", (uint64_t)hr));
		return TypedArray<Ref<GDKGameSaveBlobInfo>>();
	}

	return data;
}

TypedArray<Ref<GDKGameSaveBlobInfo>> GDKGameSaveContainer::enumerate_blob_info_by_name(const String& prefix) const {
	TypedArray<Ref<GDKGameSaveBlobInfo>> data;

	const char* prefixString = prefix.utf8().get_data();
	HRESULT hr = XGameSaveEnumerateBlobInfoByName(_container_handle, prefixString, &data,
		[](const XGameSaveBlobInfo* info, void* context) -> bool {
			TypedArray<Ref<GDKGameSaveBlobInfo>>* data = reinterpret_cast<TypedArray<Ref<GDKGameSaveBlobInfo>>*>(context);
			data->push_back(GDKGameSaveBlobInfo::create(info));
			return true;
		});

	if (FAILED(hr)) {
		ERR_PRINT(vformat("XGameSaveEnumerateBlobInfoByName Error: 0x%08ux", (uint64_t)hr));
		return TypedArray<Ref<GDKGameSaveBlobInfo>>();
	}

	return data;
}

Dictionary GDKGameSaveContainer::read_blob_data(TypedArray<Ref<GDKGameSaveBlobInfo>> blob_infos) const {
	Dictionary result;
	uint32_t blobCount = blob_infos.size();
	
	int blobSize = 0;
	LocalVector<const char*> blobNameVector;
	blobNameVector.reserve(blobCount);

	for (int i = 0; i < blobCount; i++) {
		Ref<GDKGameSaveBlobInfo> info = blob_infos[i];
		blobNameVector.push_back(info->get_name().utf8().get_data());
		blobSize += sizeof(XGameSaveBlob) + 1 + info->get_name().length() + info->get_size();
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
		for (uint32_t i = 0; i < blobCount; i++) {
			blobArray.push_back(GDKGameSaveBlob::create(&saveBlobs[i]));
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
			for (uint32_t i = 0; i < blobCount; i++) {
				blobArray.push_back(GDKGameSaveBlob::create(&saveBlobs[i]));
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