#include "gdk_game_save_container.h"
#include "gdk_game_save_blob.h"
#include "gdk_helpers.h"
#include "gdk_game_save_update.h"

using namespace godot;
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

Ref<GDKGameSaveUpdate> GDKGameSaveContainer::create_update() const {
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