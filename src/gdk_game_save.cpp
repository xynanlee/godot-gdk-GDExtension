#include "gdk_game_save.h"

#include "gdk_achievement.h"
#include "gdk_game_save_blob.h"
#include "godot_gdk.h"
#include "../godot-cpp/include/godot_cpp/godot.hpp"
#include "../godot-cpp/include/godot_cpp/core/memory.hpp"
#include "../godot-cpp/include/godot_cpp/core/print_string.hpp"

#include <Windows.h>
#include <winapifamily.h>
#include <objbase.h>
#include <XGameRuntimeInit.h>
#include <XTaskQueue.h>
#include <xsapi-c/services_c.h>
#include <XUser.h>
#include <XGame.h>
#include <XGameSave.h>

#include <iomanip>
#include <list>
#include <map>
#include <unordered_map>
#include <vector>

static XGameSaveProviderHandle *saveProviderHandle;
static char *containerName = nullptr;

void gdk_game_save::_bind_methods() {
	godot::ClassDB::bind_method(D_METHOD("InitializeGameSaveProvider", "syncOnDemand", "gameName"), &gdk_game_save::InitializeGameSaveProvider);
	godot::ClassDB::bind_method(D_METHOD("InitializeGameSaveProviderAsync", "callback", "syncOnDemand", "gameName"), &gdk_game_save::InitializeGameSaveProviderAsync);
	godot::ClassDB::bind_method(D_METHOD("ReadBlobData", "containerName", "fileNames"), &gdk_game_save::ReadBlobData);
	godot::ClassDB::bind_method(D_METHOD("WriteBlobDataString", "fileName", "data"), &gdk_game_save::WriteBlobDataString);
	godot::ClassDB::bind_method(D_METHOD("WriteBlobDataByte", "fileName", "data"), &gdk_game_save::WriteBlobDataByte);
}

void gdk_game_save::InitializeGameSaveProvider(bool syncOnDemand, godot::String gameName) {
	containerName = godot_gdk::CopyStringToChar(gameName);
	XUserHandle userHandle = godot_gdk::GetUserHandle();

	saveProviderHandle = new XGameSaveProviderHandle();

	HRESULT hr = XGameSaveInitializeProvider(userHandle, godot_gdk::GetSCID(), syncOnDemand, saveProviderHandle);

	godot_gdk::CheckResult(hr, "Successfully initialized GameSaveProvider", "Failed to initialize GameSaveProvider");
}

void gdk_game_save::InitializeGameSaveProviderAsync(godot::Callable callback, bool syncOnDemand, godot::String gameName) {
	containerName = godot_gdk::CopyStringToChar(gameName);
	XUserHandle userHandle = godot_gdk::GetUserHandle();

	XAsyncBlock *asyncBlock = godot_gdk::CreateAsyncBlock();
	godot::Callable *callbackPointer = new godot::Callable(callback);
	asyncBlock->context = callbackPointer;
	asyncBlock->callback = GameSaveProviderAsync_Callback;

	HRESULT hr = XGameSaveInitializeProviderAsync(userHandle, godot_gdk::GetSCID(), syncOnDemand, asyncBlock);

	godot_gdk::CheckResult(hr, "Successfully started initialization of GameSaveProvider async", "Failed to start initialization of GameSaveProvider async");
}

void gdk_game_save::SetGameName(godot::String gameName) {
	const char *raw = gameName.utf8();

	size_t len = std::strlen(raw);
	containerName = new char[len + 1];
	std::memcpy(containerName, raw, len + 1);
}

void gdk_game_save::GameSaveProviderAsync_Callback(_In_ XAsyncBlock *asyncBlock) {
	godot::Callable *callback = static_cast<godot::Callable *>(asyncBlock->context);
	HRESULT hr = XAsyncGetStatus(asyncBlock, false);

	if (godot_gdk::CheckResult(hr, "Successfully initialized GameSaveProvider async", "Failed to initialize GameSaveProvider async")) {
		saveProviderHandle = new XGameSaveProviderHandle();
		XGameSaveInitializeProviderResult(asyncBlock, saveProviderHandle);
		callback->call_deferred();
	}

	delete callback;
	delete asyncBlock;
}

XGameSaveContainerHandle *gdk_game_save::CreateContainerHandle() {
	XGameSaveContainerHandle *containerHandle = new XGameSaveContainerHandle();

	HRESULT hr = XGameSaveCreateContainer(*saveProviderHandle, containerName, containerHandle);

	godot_gdk::CheckResult(hr, "Successfully created Save Container Handle", "Failed to create Save Container Handle");

	return containerHandle;
}

XGameSaveUpdateHandle *gdk_game_save::CreateUpdateHandle(const char *displayName) {
	XGameSaveContainerHandle *containerHandle = CreateContainerHandle();

	XGameSaveUpdateHandle *updateHandle = new XGameSaveUpdateHandle();

	HRESULT hr = XGameSaveCreateUpdate(*containerHandle, containerName, updateHandle);

	godot_gdk::CheckResult(hr, "Successfully created Update Handle", "Failed to create Update Handle");

	return updateHandle;
}

godot::Array gdk_game_save::ReadBlobData(godot::String containerName, godot::Array fileNames) {
	print_line("Start Read BLob data");
	uint32_t size = fileNames.size();
	const char **paths = new const char *[size];

	for (int i = 0; i < size; i++) {
		paths[i] = godot_gdk::CopyStringToChar(fileNames[i]);
	}

	XGameSaveContainerHandle *containerHandle = CreateContainerHandle();

	print_line("Pre callback");
	std::vector<const XGameSaveBlobInfo *> *blobInfos = new std::vector<const XGameSaveBlobInfo *>();

	if (size == 0 || (size == 1 && paths[0] == "")) {
		GetBlobInfoFinal(nullptr, nullptr, blobInfos);
	} else {
		for (int i = 0; i < size; i++) {
			print_line(paths[i]);
			GetBlobInfoFinal(nullptr, paths[i], blobInfos);
		}
	}

	godot::print_line("Post callback");

	godot::Array* godotBlobs = memnew(godot::Array);

	XGameSaveBlob *blobData;

	if (blobInfos->size() == 0) {
		ERR_PRINT("There is no save files with given path.");
	} else {
		auto newSize = static_cast<uint32_t>(blobInfos->size());
		size_t allBlobsSize = 0;
		const char **paths2 = new const char *[newSize];

		for (int i = 0; i < newSize; i++) {
			paths2[i] = blobInfos->at(i)->name;
			allBlobsSize += sizeof(XGameSaveBlob) + strlen(blobInfos->at(i)->name) + 1 + blobInfos->at(i)->size;
		}

		blobData = static_cast<XGameSaveBlob*>(malloc(allBlobsSize));
		HRESULT hr = XGameSaveReadBlobData(*containerHandle, paths2, &newSize, allBlobsSize, blobData);

		if (godot_gdk::CheckResult(hr, "Successfully retrieved save blob data", "Failed to read save blob data")) {
			for (int i = 0; i < newSize; i++) {
				godotBlobs->push_back(memnew(gdk_game_save_blob(&blobData[i])));
			}
		}
	}

	return *godotBlobs;
}


void gdk_game_save::WriteBlobDataString(godot::String fileName, godot::String data) {
	WriteBlobDataByte(fileName, data.to_utf8_buffer());
}

void gdk_game_save::WriteBlobDataByte(godot::String fileName, godot::PackedByteArray data) {
	XGameSaveUpdateHandle *updateHandle = CreateUpdateHandle(fileName.utf8().ptrw());

	HRESULT hr = XGameSaveSubmitBlobWrite(*updateHandle, fileName.utf8(), data.ptrw(), data.size());

	godot::print_line(data);
	if (!godot_gdk::CheckResult(hr, "Successfully created blob data", "Failed to create blob data")) {
		return;
	}

	hr = XGameSaveSubmitUpdate(*updateHandle);
	godot_gdk::CheckResult(hr, "Successfully saved blob data", "Failed to save blob data");

	delete updateHandle;
}

void gdk_game_save::GetBlobInfo(godot::String containerName, godot::Callable callback) {
	XGameSaveContainerHandle *containerHandle = CreateContainerHandle();
	//GetBlobInfo(containerHandle, nullptr, *callback);
	XGameSaveCloseContainer(*containerHandle);
}

void gdk_game_save::GetBlobInfo(godot::String containerName, godot::String path, godot::Callable callback) {
	XGameSaveContainerHandle *containerHandle = CreateContainerHandle();
	//GetBlobInfo(containerHandle, path.utf8(), *callback);
	XGameSaveCloseContainer(*containerHandle);
}

void gdk_game_save::GetBlobInfoFinal(XGameSaveContainerHandle *containerHandle, const char *path, std::vector<const XGameSaveBlobInfo *> *blobArray) {
	HRESULT hr;
	bool closeContainerHandle = false;

	if (containerHandle == nullptr) {
		containerHandle = CreateContainerHandle();
		closeContainerHandle = true;
	}

	if (path == nullptr || !path[0]) {
		godot::print_line("Get blob info");
		hr = XGameSaveEnumerateBlobInfo(*containerHandle, blobArray, GetBlobInfoCallback);
	} else {
		godot::print_line("Get blob info by name");
		hr = XGameSaveEnumerateBlobInfoByName(*containerHandle, path, blobArray, GetBlobInfoCallback);
	}

	godot_gdk::CheckResult(hr, "Successfully started retrieving Blob Info By Name", "Failed to start retrieving Blob Info By Name");

	if (closeContainerHandle) {
		XGameSaveCloseContainer(*containerHandle);
	}
}

bool CALLBACK gdk_game_save::GetBlobInfoCallback(const XGameSaveBlobInfo *info, void *context) {
	print_line("Callback");
	std::vector<const XGameSaveBlobInfo *> *blobArray = static_cast<std::vector<const XGameSaveBlobInfo *> *>(context);
	blobArray->push_back(info);
	return true;
}
