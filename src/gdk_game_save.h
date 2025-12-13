#pragma once

#include "gdk_game_save_blob.h"
#include "gdk_game_save_blob_info.h"

#include <Windows.h>
#include <winapifamily.h>
#include <objbase.h>
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/variant.hpp"
#include <XAsync.h>
#include <XGameSave.h>
#include <XTaskQueue.h>
#include <list>
#include <xsapi-c/services_c.h>

#include <vector>

using namespace godot;

class gdk_game_save : public RefCounted {
	GDCLASS(gdk_game_save, RefCounted)

	void SetGameName(String gameName);

	static void GameSaveProviderAsync_Callback(XAsyncBlock* asyncBlock);
	static XGameSaveContainerHandle* CreateContainerHandle();
	XGameSaveUpdateHandle *CreateUpdateHandle(const char *displayName);
	static bool CALLBACK GetBlobInfoCallback(const XGameSaveBlobInfo* info, void* context);
	void EnumerateBlobInfoByName();

	static void GetBlobInfoFinal(XGameSaveContainerHandle* containerHandle, const char* path, std::vector<const XGameSaveBlobInfo*>* blobArray);
	static void test(XGameSaveBlobInfo blobInfo, void *context);

protected:
	static void _bind_methods();

public:
	gdk_game_save() = default;
	~gdk_game_save() override = default;

	void InitializeGameSaveProvider(bool syncOnDemand, String containerName);
	void InitializeGameSaveProviderAsync(Callable callback, bool syncOnDemand, godot::String containerName);
	godot::Array ReadBlobData(String containerName, godot::Array fileNames);
	void WriteBlobDataString(String fileName, String data);
	void WriteBlobDataByte(String fileName, PackedByteArray data);
	void GetBlobInfo(String containerName, Callable callback);
	void GetBlobInfo(String containerName, String path, Callable callback);

};
