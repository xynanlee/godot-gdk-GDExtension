#include "gdk_package.h"
#include "gdk_helpers.h"
#include "gdk_system.h"
#include <templates/local_vector.hpp>

using namespace godot;

void GDKXPackageMount::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_mount_path"), &GDKXPackageMount::get_mount_path);
}

void GDKXPackageMount::_notification(int p_what) {
    if (p_what == NOTIFICATION_PREDELETE) {
        if (_mount != nullptr) {
            XPackageCloseMountHandle(_mount);
        }
    }
}

Ref<GDKXPackageMount> GDKXPackageMount::create(const XPackageMountHandle mount) {
    Ref<GDKXPackageMount> wrapper;
    if (mount != nullptr) {
        wrapper.instantiate();
        wrapper->_mount = mount;
    }
	return wrapper;
}

String GDKXPackageMount::get_mount_path() const {
    size_t bufferSize;
    HRESULT hr = XPackageGetMountPathSize(_mount, &bufferSize);
    ERR_FAIL_COND_V_MSG(FAILED(hr), String(), vformat("XPackageGetMountPathSize Error: 0x%08ux", (uint64_t)hr));
    
    LocalVector<char> buffer;
    buffer.reserve(bufferSize);
    hr = XPackageGetMountPath(_mount, bufferSize, buffer.ptr());
    ERR_FAIL_COND_V_MSG(FAILED(hr), String(), vformat("XPackageGetMountPath Error: 0x%08ux", (uint64_t)hr));
    return String(buffer.ptr());
}

void GDKXPackageEnumerationScope::_bind_methods() {
    BIND_ENUM_CONSTANT(ThisOnly);
    BIND_ENUM_CONSTANT(ThisAndRelated);
    BIND_ENUM_CONSTANT(ThisPublisher);
}

void GDKXPackageKind::_bind_methods() {
    BIND_ENUM_CONSTANT(Game);
    BIND_ENUM_CONSTANT(Content);
}

void GDKXPackageChunkAvailability::_bind_methods() {
    BIND_ENUM_CONSTANT(Ready);
    BIND_ENUM_CONSTANT(Pending);
    BIND_ENUM_CONSTANT(Installable);
    BIND_ENUM_CONSTANT(Unavailable);
}

void GDKXPackageChunkSelector::_bind_methods() {
    BIND_ENUM_CONSTANT(Language);
    BIND_ENUM_CONSTANT(Tag);
    BIND_ENUM_CONSTANT(Feature);
    BIND_ENUM_CONSTANT(Chunk);

    ClassDB::bind_static_method(get_class_static(), D_METHOD("create_langauge", "language"), &GDKXPackageChunkSelector::create_language);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("create_tag", "tag"), &GDKXPackageChunkSelector::create_tag);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("create_feature", "feature"), &GDKXPackageChunkSelector::create_feature);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("create_chunk_id", "chunk_id"), &GDKXPackageChunkSelector::create_chunk_id);
}

Ref<GDKXPackageChunkSelector> GDKXPackageChunkSelector::create_language(const String &language) {
	Ref<GDKXPackageChunkSelector> selector;
    selector.instantiate();
    selector->_selector.type = XPackageChunkSelectorType::Language;
    selector->_selector.language = language.utf8().get_data();
    return selector;
}

Ref<GDKXPackageChunkSelector> GDKXPackageChunkSelector::create_tag(const String &tag) {
	Ref<GDKXPackageChunkSelector> selector;
    selector.instantiate();
    selector->_selector.type = XPackageChunkSelectorType::Tag;
    selector->_selector.tag = tag.utf8().get_data();
    return selector;
}

Ref<GDKXPackageChunkSelector> godot::GDKXPackageChunkSelector::create_feature(const String &feature) {
	Ref<GDKXPackageChunkSelector> selector;
    selector.instantiate();
    selector->_selector.type = XPackageChunkSelectorType::Feature;
    selector->_selector.feature = feature.utf8().get_data();
    return selector;
}

Ref<GDKXPackageChunkSelector> GDKXPackageChunkSelector::create_chunk_id(int chunk_id) {
    ERR_FAIL_COND_V_MSG(chunk_id < 0, nullptr, "Chunk ID must be >= 0.");

	Ref<GDKXPackageChunkSelector> selector;
    selector.instantiate();
    selector->_selector.type = XPackageChunkSelectorType::Chunk;
    selector->_selector.chunkId = (uint32_t)chunk_id;
    return selector;
}

Ref<GDKXPackageChunkSelector> GDKXPackageChunkSelector::create_from_xpackage(const XPackageChunkSelector *selector) {
	Ref<GDKXPackageChunkSelector> wrapper;
    if (selector != nullptr) {
        wrapper.instantiate();
        wrapper->_selector = *selector;
    }
    
    return wrapper;
}

void GDKXPackageInstallationMonitor::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_installation_progress"), &GDKXPackageInstallationMonitor::get_installation_progress);
    ClassDB::bind_method(D_METHOD("update_installation_progress"), &GDKXPackageInstallationMonitor::update_installation_progress);

    ADD_SIGNAL(MethodInfo("progress_changed", 
        PropertyInfo(Variant::INT, "total_bytes"),
        PropertyInfo(Variant::INT, "installed_bytes"),
        PropertyInfo(Variant::INT, "launch_bytes"),
        PropertyInfo(Variant::BOOL, "launchable"),
        PropertyInfo(Variant::BOOL, "completed")));
}

void GDKXPackageInstallationMonitor::_notification(int p_what) {
	if (p_what == NOTIFICATION_PREDELETE) {
        if (_handle != nullptr) {
            XPackageCloseInstallationMonitorHandle(_handle);
        }        
    }
}

void InstallationProgressChangedCallback(void* context, XPackageInstallationMonitorHandle handle) {
    XPackageInstallationProgress progress;
    XPackageGetInstallationProgress(handle, &progress);

    GDKXPackageInstallationMonitor* monitor = (GDKXPackageInstallationMonitor*)context;
    monitor->call_deferred("emit_signal", "progress_changed", progress.totalBytes, progress.installedBytes, progress.launchBytes, progress.launchable, progress.completed);

    if (progress.completed) {
        XPackageUnregisterInstallationProgressChanged(handle, monitor->get_token(), false);
    }
}

Ref<GDKXPackageInstallationMonitor> GDKXPackageInstallationMonitor::create(XPackageInstallationMonitorHandle handle) {
	Ref<GDKXPackageInstallationMonitor> wrapper;
    if (handle != nullptr) {
        wrapper.instantiate();
        wrapper->_handle = handle;
        HRESULT hr = XPackageRegisterInstallationProgressChanged(handle, wrapper.ptr(), InstallationProgressChangedCallback, &wrapper->_token);
        ERR_FAIL_COND_V_MSG(FAILED(hr), wrapper, vformat("XPackageRegisterInstallationProgressChanged Error: 0x%08ux", (uint64_t)hr));
    }
    return wrapper;
}

Dictionary GDKXPackageInstallationMonitor::get_installation_progress() const {
	Dictionary return_data;

    XPackageInstallationProgress progress;
    XPackageGetInstallationProgress(_handle, &progress);
    return_data["total_bytes"] = progress.totalBytes;
    return_data["installed_bytes"] = progress.installedBytes;
    return_data["launch_bytes"] = progress.launchBytes;
    return_data["launchable"] = progress.launchable;
    return_data["completed"] = progress.completed;
    return return_data;
}

bool GDKXPackageInstallationMonitor::update_installation_progress() const {
	return XPackageUpdateInstallationMonitor(_handle);
}

void GDKPackage::_bind_methods() {
    ClassDB::bind_static_method(get_class_static(), D_METHOD("get_current_process_package_identifier"), &GDKPackage::get_current_process_package_identifier);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("change_chunk_install_order", "package_identifier", "selectors"), &GDKPackage::change_chunk_install_order);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("find_chunk_availability", "package_identifier", "selectors"), &GDKPackage::find_chunk_availability);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("enumerate_chunk_availability", "package_identifier", "selector_type", "callback"), &GDKPackage::enumerate_chunk_availability);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("enumerate_features", "package_identifier", "callback"), &GDKPackage::enumerate_features);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("enumerate_packages", "kind", "scope", "callback"), &GDKPackage::enumerate_packages);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("create_installation_monitor", "package_identifier", "selectors", "minimum_update_interval_ms"), &GDKPackage::create_installation_monitor);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("estimate_download_size", "package_identifier", "selectors"), &GDKPackage::estimate_download_size);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("mount_with_ui_async", "package_identifier"), &GDKPackage::mount_with_ui_async);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("get_user_locale"), &GDKPackage::get_user_locale);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("get_write_stats"), &GDKPackage::get_write_stats);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("install_chunks", "package_identifier", "selectors", "minimum_update_interval_ms", "suppres_user_confirmation"), &GDKPackage::install_chunks);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("install_chunks_async", "package_identifier", "selectors", "minimum_update_interval_ms", "suppres_user_confirmation"), &GDKPackage::install_chunks_async);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("is_packaged_process"), &GDKPackage::is_packaged_process);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("uninstall_package", "package_identifier"), &GDKPackage::uninstall_package);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("uninstall_chunks", "package_identifier", "selectors"), &GDKPackage::uninstall_chunks);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("uninstall_uwp_instance", "package_name"), &GDKPackage::uninstall_uwp_instance);
}

Dictionary GDKPackage::get_current_process_package_identifier() {
	Dictionary return_data;
    char id[XPACKAGE_IDENTIFIER_MAX_LENGTH];
    HRESULT hr = XPackageGetCurrentProcessPackageIdentifier(_countof(id), id);
    return_data["hresult"] = hr;
    ERR_FAIL_COND_V_MSG(FAILED(hr), return_data, vformat("XPackageGetCurrentProcessPackageIdentifier Error: 0x%08ux", (uint64_t)hr));
    return_data["package_identifier"] = String(id);
    return return_data;
}

int GDKPackage::change_chunk_install_order(const String &package_identifier, TypedArray<Ref<GDKXPackageChunkSelector>> selectors) {
	LocalVector<XPackageChunkSelector> native_selectors;
    native_selectors.reserve(selectors.size());
    for (int i = 0; i < selectors.size(); i++) {
        Ref<GDKXPackageChunkSelector> s = selectors[i];
        native_selectors.push_back(s->get_native_selector());
    }

    HRESULT hr = XPackageChangeChunkInstallOrder(package_identifier.utf8().get_data(), selectors.size(), native_selectors.ptr());
    ERR_FAIL_COND_V_MSG(FAILED(hr), (int64_t)hr, vformat("XPackageChangeChunkInstallOrder Error: 0x%08ux", (uint64_t)hr));
    return (int64_t)hr;
}

Dictionary GDKPackage::find_chunk_availability(const String &package_identifier, TypedArray<Ref<GDKXPackageChunkSelector>> selectors) {
	LocalVector<XPackageChunkSelector> native_selectors;
    native_selectors.reserve(selectors.size());
    for (int i = 0; i < selectors.size(); i++) {
        Ref<GDKXPackageChunkSelector> s = selectors[i];
        native_selectors.push_back(s->get_native_selector());
    }
    Dictionary return_data;
    XPackageChunkAvailability availability;
    HRESULT hr = XPackageFindChunkAvailability(package_identifier.utf8().get_data(), selectors.size(), native_selectors.ptr(), &availability);
    return_data["hresult"] = hr;
    ERR_FAIL_COND_V_MSG(FAILED(hr), return_data, vformat("XPackageFindChunkAvailability Error: 0x%08ux", (uint64_t)hr));
    
    return_data["availability"] = (GDKXPackageChunkAvailability::Enum)availability;
    return return_data;
}

int GDKPackage::enumerate_chunk_availability(const String &package_identifier, GDKXPackageChunkSelector::Enum selector_type, const Callable &callback) {
    Callable* hCallback = memnew(Callable(callback));
    HRESULT hr = XPackageEnumerateChunkAvailability(package_identifier.utf8().get_data(), (XPackageChunkSelectorType)selector_type, hCallback, 
                    [](void* context, const XPackageChunkSelector* selector, XPackageChunkAvailability availability) -> bool{
                        Ref<GDKXPackageChunkSelector> s = GDKXPackageChunkSelector::create_from_xpackage(selector);
                        GDKXPackageChunkAvailability::Enum a = (GDKXPackageChunkAvailability::Enum)availability;
                        Callable* c = static_cast<Callable*>(context);
                        return c->call(s, a);
                    });
    
    memdelete(hCallback);
    ERR_FAIL_COND_V_MSG(FAILED(hr), (int64_t)hr, vformat("XPackageEnumerateChunkAvailability Error: 0x%08ux", (uint64_t)hr));
    return (int64_t)hr;
}

int GDKPackage::enumerate_features(const String &package_identifier, const Callable &callback) {
    Callable* hCallback = memnew(Callable(callback));
    HRESULT hr = XPackageEnumerateFeatures(package_identifier.utf8().get_data(), hCallback, 
                    [](void* context, const XPackageFeature* feature) -> bool {
                        Dictionary feature_data;
                        feature_data["id"] = String(feature->id);
                        feature_data["display_name"] = String(feature->displayName);
                        feature_data["tags"] = String(feature->tags);
                        feature_data["hidden"] = feature->hidden;
                        
                        PackedStringArray storeIds;
                        storeIds.resize(feature->storeIdCount);
                        for (int i = 0; i < feature->storeIdCount; i++) {
                            storeIds.push_back(String(feature->storeIds[i]));
                        }
                        feature_data["store_ids"] = storeIds;
                        
                        Callable* c = static_cast<Callable*>(context);
                        return c->call(feature_data);
                    });

    memdelete(hCallback);
    ERR_FAIL_COND_V_MSG(FAILED(hr), (int64_t)hr, vformat("XPackageEnumerateFeatures Error: 0x%08ux", (uint64_t)hr));
	return (int64_t)hr;
}

int GDKPackage::enumerate_packages(GDKXPackageKind::Enum kind, GDKXPackageEnumerationScope::Enum scope, const Callable& callback) {
	Callable* hCallback = memnew(Callable(callback));
    HRESULT hr = XPackageEnumeratePackages((XPackageKind)kind, (XPackageEnumerationScope)scope, hCallback,
                    [](void* context, const XPackageDetails* details) -> bool {
                        Dictionary package_details;
                        package_details["package_identifier"] = String(details->packageIdentifier);
                        package_details["version"] = GDKXVersion::create(&details->version);
                        package_details["kind"] = (GDKXPackageKind::Enum)details->kind;
                        package_details["display_name"] = String(details->displayName);
                        package_details["description"] = String(details->description);
                        package_details["publisher"] = String(details->publisher);
                        package_details["store_id"] = String(details->storeId);
                        package_details["installing"] = details->installing;
                        package_details["index"] = details->index;
                        package_details["count"] = details->count;
                        package_details["age_restricted"] = details->ageRestricted;
                        package_details["title_id"] = String(details->titleId);

                        Callable* c = static_cast<Callable*>(context);
                        return c->call(package_details);
                    });

    memdelete(hCallback);
    ERR_FAIL_COND_V_MSG(FAILED(hr), (int64_t)hr, vformat("XPackageEnumeratePackages Error: 0x%08ux", (uint64_t)hr));
    return (int64_t)hr;
}

Dictionary GDKPackage::create_installation_monitor(const String &package_identifier, TypedArray<Ref<GDKXPackageChunkSelector>> selectors, int32_t minimum_update_interval_ms) {
    Dictionary return_data;
    XPackageInstallationMonitorHandle handle = nullptr;
    HRESULT hr = S_OK;

    if (minimum_update_interval_ms < 0) {
        hr = E_INVALIDARG;
        return_data["hresult"] = hr;
        ERR_FAIL_COND_V_MSG(FAILED(hr), return_data, vformat("XPackageCreateInstallationMonitor minimum_update_interval_ms must be >= 0: 0x%08ux", (uint64_t)hr));
    }

    if (SUCCEEDED(hr)) {
        LocalVector<XPackageChunkSelector> native_selectors;
        native_selectors.reserve(selectors.size());
        for (int i = 0; i < selectors.size(); i++) {
            Ref<GDKXPackageChunkSelector> s = selectors[i];
            native_selectors.push_back(s->get_native_selector());
        }

        hr = XPackageCreateInstallationMonitor( package_identifier.utf8().get_data(),
                                                selectors.size(),
                                                native_selectors.ptr(),
                                                minimum_update_interval_ms,
                                                GDKHelpers::get_async_queue(),
                                                &handle);
        return_data["hresult"] = hr;
        ERR_FAIL_COND_V_MSG(FAILED(hr), return_data, vformat("XPackageCreateInstallationMonitor Error: 0x%08ux", (uint64_t)hr));
    }

    if (SUCCEEDED(hr)) {
        return_data["monitor"] = GDKXPackageInstallationMonitor::create(handle);
    }

    return return_data;
}

Dictionary GDKPackage::estimate_download_size(const String &package_identifier, TypedArray<Ref<GDKXPackageChunkSelector>> selectors) {
	LocalVector<XPackageChunkSelector> native_selectors;
    native_selectors.reserve(selectors.size());
    for (int i = 0; i < selectors.size(); i++) {
        Ref<GDKXPackageChunkSelector> s = selectors[i];
        native_selectors.push_back(s->get_native_selector());
    }

    Dictionary return_data;
    uint64_t downloadSize = 0;
    bool shouldPresentUserConfirmation = false;
    HRESULT hr = XPackageEstimateDownloadSize(package_identifier.utf8().get_data(), selectors.size(), native_selectors.ptr(), &downloadSize, &shouldPresentUserConfirmation);
    return_data["hresult"] = hr;
    ERR_FAIL_COND_V_MSG(FAILED(hr), return_data, vformat("XPackageEstimateDownloadSize Error: 0x%08ux", (uint64_t)hr));

    return_data["download_size"] = downloadSize;
    return_data["should_present_user_confirmation"] = shouldPresentUserConfirmation;
    return return_data;
}

Ref<GDKAsyncBlock> GDKPackage::mount_with_ui_async(const String& package_identifier) {
	Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        XPackageMountHandle handle;
        HRESULT hr = XPackageMountWithUiResult(async, &handle);
        Dictionary return_data;
        return_data["hresult"] = hr;
        if (SUCCEEDED(hr)) {
            return_data["mount_handle"] = GDKXPackageMount::create(handle);
        }
        wrapper->emit(return_data);
    });

    HRESULT hr = XPackageMountWithUiAsync(package_identifier.utf8().get_data(), asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XPackageMountWithUiAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

Dictionary GDKPackage::get_user_locale() {
    Dictionary return_data;
    char gameLocale[LOCALE_NAME_MAX_LENGTH];
    HRESULT hr = XPackageGetUserLocale(_countof(gameLocale), gameLocale);
    return_data["hresult"] = hr;
    
    ERR_FAIL_COND_V_MSG(FAILED(hr), Dictionary(), vformat("XPackageGetUserLocale Error: 0x%08ux", (uint64_t)hr));
    return_data["game_locale"] = String(gameLocale);
	return return_data;
}

Dictionary GDKPackage::get_write_stats() {
    Dictionary return_data;
    XPackageWriteStats stats{};
	HRESULT hr = XPackageGetWriteStats(&stats);
    return_data["hresult"] = hr;
    ERR_FAIL_COND_V_MSG(FAILED(hr), return_data, vformat("XPackageGetWriteStats Error: 0x%08ux", (uint64_t)hr));

    return_data["interval"] = stats.interval;
    return_data["budget"] = stats.budget;
    return_data["elapsed"] = stats.elapsed;
    return_data["bytes_written"] = stats.bytesWritten;
    return return_data;
}

Ref<GDKXPackageInstallationMonitor> GDKPackage::install_chunks( const String &package_identifier, 
                                                                TypedArray<Ref<GDKXPackageChunkSelector>> selectors, 
                                                                int minimum_update_interval_ms, 
                                                                bool suppress_user_confirmation) {
    HRESULT hr = S_OK;
    if (minimum_update_interval_ms < 0) {
        hr = E_INVALIDARG;
        ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XPackageInstallChunks minimum_update_interval_ms must be >= 0: 0x%08ux", (uint64_t)hr));
    }

    XPackageInstallationMonitorHandle handle = nullptr;
    if (SUCCEEDED(hr)) {
        LocalVector<XPackageChunkSelector> native_selectors;
        native_selectors.reserve(selectors.size());
        for (int i = 0; i < selectors.size(); i++) {
            Ref<GDKXPackageChunkSelector> s = selectors[i];
            native_selectors.push_back(s->get_native_selector());
        }

        hr = XPackageInstallChunks( package_identifier.utf8().get_data(), 
                                    selectors.size(), 
                                    native_selectors.ptr(), 
                                    minimum_update_interval_ms, 
                                    suppress_user_confirmation, 
                                    GDKHelpers::get_async_queue(), 
                                    &handle);
        ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XPackageInstallChunks Error: 0x%08ux", (uint64_t)hr));
    }

	return Ref<GDKXPackageInstallationMonitor>(handle);
}

Ref<GDKAsyncBlock> GDKPackage::install_chunks_async(const String &package_identifier, TypedArray<Ref<GDKXPackageChunkSelector>> selectors, int minimum_update_interval_ms, bool suppress_user_confirmation) {
	HRESULT hr = S_OK;
    if (minimum_update_interval_ms < 0) {
        hr = E_INVALIDARG;
        ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XPackageInstallChunks minimum_update_interval_ms must be >= 0: 0x%08ux", (uint64_t)hr));
    }
    
    Ref<GDKAsyncBlock> asyncBlock = GDKAsyncBlock::create(GDKHelpers::get_async_queue());
    asyncBlock->set_callback([](XAsyncBlock* async) {
        GDKAsyncBlock* wrapper = reinterpret_cast<GDKAsyncBlock*>(async->context);
        XPackageInstallationMonitorHandle handle;
        HRESULT hr = XPackageInstallChunksResult(async, &handle);
        Dictionary return_data;
        return_data["hresult"] = hr;
        if (SUCCEEDED(hr)) {
            return_data["monitor"] = GDKXPackageInstallationMonitor::create(handle);
        }
        wrapper->emit(return_data);
    });

    LocalVector<XPackageChunkSelector> native_selectors;
    native_selectors.reserve(selectors.size());
    for (int i = 0; i < selectors.size(); i++) {
        Ref<GDKXPackageChunkSelector> s = selectors[i];
        native_selectors.push_back(s->get_native_selector());
    }
    hr = XPackageInstallChunksAsync(package_identifier.utf8().get_data(), 
                                            selectors.size(), 
                                            native_selectors.ptr(), 
                                            minimum_update_interval_ms, 
                                            suppress_user_confirmation, 
                                            asyncBlock->get_block());
    ERR_FAIL_COND_V_MSG(FAILED(hr), nullptr, vformat("XPackageInstallChunksAsync Error: 0x%08ux", (uint64_t)hr));
    return asyncBlock;
}

bool GDKPackage::is_packaged_process() {
	return XPackageIsPackagedProcess();
}

bool GDKPackage::uninstall_package(const String &package_identifier) {
    return XPackageUninstallPackage(package_identifier.utf8().get_data());
}

int GDKPackage::uninstall_chunks(const String &package_identifier, TypedArray<Ref<GDKXPackageChunkSelector>> selectors) {
    LocalVector<XPackageChunkSelector> native_selectors;
    native_selectors.reserve(selectors.size());
    for (int i = 0; i < selectors.size(); i++) {
        Ref<GDKXPackageChunkSelector> s = selectors[i];
        native_selectors.push_back(s->get_native_selector());
    }

    HRESULT hr = XPackageUninstallChunks(package_identifier.utf8().get_data(), selectors.size(), native_selectors.ptr());
    ERR_FAIL_COND_V_MSG(FAILED(hr), (int64_t)hr, vformat("XPackageUninstallChunks Error: 0x%08ux", (uint64_t)hr));
    return (int64_t)hr;
}

int GDKPackage::uninstall_uwp_instance(const String &package_name) {
    HRESULT hr = XPackageUninstallUWPInstance(package_name.utf8().get_data());
    ERR_FAIL_COND_V_MSG(FAILED(hr), (int64_t)hr, vformat("XPackageUninstallUwpInstance Error: 0x%08ux", (uint64_t)hr));
    return (int64_t)hr;
}
