#include "gdk_game_save_files.h"
#include "gdk_helpers.h"
#include <XGameSaveFiles.h>

using namespace godot;

void GDKGameSaveFiles::_bind_methods() {
    ClassDB::bind_static_method("GDKGameSaveFiles", D_METHOD("get_folder_with_ui_async", "user"), &GDKGameSaveFiles::get_folder_with_ui_async);
    ClassDB::bind_static_method("GDKGameSaveFiles", D_METHOD("get_remaining_quota", "user"), &GDKGameSaveFiles::get_remaining_quota);
}

Ref<GDKAsyncBlock> GDKGameSaveFiles::get_folder_with_ui_async(Ref<GDKUser> user) {
    XTaskQueueHandle queue = GDKHelpers::get_async_queue();
    Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(queue);
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);

        size_t folderSize = 0;
        XAsyncGetResultSize(async, &folderSize);
        char folderResult[MAX_PATH]{};
        HRESULT hr = XGameSaveFilesGetFolderWithUiResult(async, folderSize, folderResult);
        Dictionary return_data;
        return_data["hresult"] = hr;
        if (SUCCEEDED(hr)) {
            return_data["folder"] = String(folderResult);
        }

        wrapper->emit(return_data);
    });

    const char* scid = GDKHelpers::get_scid().utf8().get_data();
    HRESULT hr = XGameSaveFilesGetFolderWithUiAsync(user->get_user(), scid, asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XGameSaveFilesGetFolderWithUiAsync Error: 0x%08ux", (uint64_t)hr));

	return asyncBlock;
}

int64_t GDKGameSaveFiles::get_remaining_quota(Ref<GDKUser> user) {
    int64_t remainingQuota = 0;
    const char* scid = GDKHelpers::get_scid().utf8().get_data();
    HRESULT hr = XGameSaveFilesGetRemainingQuota(user->get_user(), scid, &remainingQuota);
    ERR_FAIL_COND_V_MSG(FAILED(hr), 0, vformat("XGameSaveFilesGetRemainingQuota Error: 0x%08ux", (uint64_t)hr));

	return remainingQuota;
}
