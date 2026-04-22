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

Ref<GDKPromise> GDKGameSaveFiles::get_remaining_quota(Ref<GDKUser> user) {
    Ref<GDKPromise> promise;
    promise.instantiate();
    
    Array dataArray;
    dataArray.push_back(user);
    dataArray.push_back(GDKHelpers::get_scid());

    promise->start(dataArray);

	return promise;
}

void GDKPromise::_bind_methods() {
	ADD_SIGNAL(MethodInfo("get_future", PropertyInfo(Variant::DICTIONARY, "result")));
}

void GDKPromise::_notification(int p_what) {
    switch (p_what)
    {
    case NOTIFICATION_PREDELETE:
        {
            if (_asyncThread.is_valid()) {
                _asyncThread->wait_to_finish();
            }

            _asyncThread.unref();
        }
        break;
    }
}

void GDKPromise::start(Array inputData, Thread::Priority priority) {
    _inputData = inputData;

    _asyncThread.instantiate();
    _asyncThread->start(callable_mp(this, &GDKPromise::_threaded_function), Thread::PRIORITY_NORMAL);
}

void GDKPromise::_threaded_function() {
    Ref<GDKUser> user = _inputData[0];
    String scid = _inputData[1];

    int64_t remainingQuota = 0;
    HRESULT hr = XGameSaveFilesGetRemainingQuota(user->get_user(), scid.utf8().get_data(), &remainingQuota);
    Dictionary return_data;
    return_data["hresult"] = hr;

    if (SUCCEEDED(hr)) {
        return_data["remaining_quota"] = remainingQuota;
    }

    call_deferred("emit_signal", "get_future", return_data);
}
