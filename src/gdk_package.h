#pragma once

#include <classes/ref_counted.hpp>
#include <variant/typed_array.hpp>
#include "gdk_asyncblock.h"
#include "gdk_system.h"
#include <XTaskQueue.h>

extern "C" {
    #include <XPackage.h>
}

namespace godot {

class GDKXPackageFeature: public RefCounted {
    GDCLASS(GDKXPackageFeature, RefCounted);

private:
    String _id;
    String _display_name;
    String _tags;
    bool _hidden;
    PackedStringArray _store_ids;

protected:
    static void _bind_methods();

public:
    static Ref<GDKXPackageFeature> create(const XPackageFeature* feature);

    inline String get_id() const { return _id; }
    inline String get_display_name() const { return _display_name; }
    inline String get_tags() const { return _tags; }
    inline bool is_hidden() const { return _hidden; }
    inline PackedStringArray get_store_ids() const { return _store_ids; }
};

class GDKXPackageMount: public RefCounted {
    GDCLASS(GDKXPackageMount, RefCounted);

private:
    XPackageMountHandle _mount;

protected:
    static void _bind_methods();
    void _notification(int p_what);

public:
    static Ref<GDKXPackageMount> create(const XPackageMountHandle mount);
    String get_mount_path() const;
};

class GDKXPackageEnumerationScope: public RefCounted {
    GDCLASS(GDKXPackageEnumerationScope, RefCounted);

protected:
    static void _bind_methods();

public:
    enum Enum: uint32_t {
        ThisOnly = XPackageEnumerationScope::ThisOnly,
        ThisAndRelated = XPackageEnumerationScope::ThisAndRelated,
        ThisPublisher = XPackageEnumerationScope::ThisPublisher
    };
};

class GDKXPackageKind: public RefCounted {
    GDCLASS(GDKXPackageKind, RefCounted);

protected:
    static void _bind_methods();

public:
    enum Enum: uint32_t {
        Game = XPackageKind::Game,
        Content = XPackageKind::Content
    };
};

class GDKXPackageChunkAvailability: public RefCounted {
    GDCLASS(GDKXPackageChunkAvailability, RefCounted);

protected:
    static void _bind_methods();

public:
    enum Enum: uint32_t {
        Ready = XPackageChunkAvailability::Ready,
        Pending = XPackageChunkAvailability::Pending,
        Installable = XPackageChunkAvailability::Installable,
        Unavailable = XPackageChunkAvailability::Unavailable
    };
};

class GDKXPackageChunkSelector: public RefCounted {
    GDCLASS(GDKXPackageChunkSelector, RefCounted);

private:
    XPackageChunkSelector _selector;

protected:
    static void _bind_methods();

public:
    enum Enum: uint32_t {
        Language = XPackageChunkSelectorType::Language,
        Tag = XPackageChunkSelectorType::Tag,
        Feature = XPackageChunkSelectorType::Feature,
        Chunk = XPackageChunkSelectorType::Chunk
    };

    static Ref<GDKXPackageChunkSelector> create_language(const String& language);
    static Ref<GDKXPackageChunkSelector> create_tag(const String& tag);
    static Ref<GDKXPackageChunkSelector> create_feature(const String& feature);
    static Ref<GDKXPackageChunkSelector> create_chunk_id(int chunk_id);

    static Ref<GDKXPackageChunkSelector> create_from_xpackage(const XPackageChunkSelector* selector);
    inline XPackageChunkSelector get_native_selector() const { return _selector; }
};

class GDKXPackageInstallationMonitor: public RefCounted {
    GDCLASS(GDKXPackageInstallationMonitor, RefCounted);

private:
    XPackageInstallationMonitorHandle _handle;
    XTaskQueueRegistrationToken _token;

protected:
    static void _bind_methods();
    void _notification(int p_what);

public:
    static Ref<GDKXPackageInstallationMonitor> create(XPackageInstallationMonitorHandle handle);
    Dictionary get_installation_progress() const;
    bool update_installation_progress() const;
    inline XTaskQueueRegistrationToken get_token() const { return _token; }
};

class GDKXPackageDetails: public RefCounted {
    GDCLASS(GDKXPackageDetails, RefCounted);

private:
    String _package_identifier;
    Ref<GDKXVersion> _version;
    GDKXPackageKind::Enum _kind;
    String _display_name;
    String _description;
    String _publisher;
    String _store_id;
    bool _installing;
    int _index;
    int _count;
    bool _age_restricted;
    String _title_id;

protected:
    static void _bind_methods();

public:
    static Ref<GDKXPackageDetails> create(const XPackageDetails* details);

    inline String get_package_identifier() const { return _package_identifier; }
    inline Ref<GDKXVersion> get_version() const { return _version; }
    inline GDKXPackageKind::Enum get_kind() const { return _kind; }
    inline String get_display_name() const { return _display_name; }
    inline String get_description() const { return _description; }
    inline String get_publisher() const { return _publisher; }
    inline String get_store_id() const { return _store_id; }
    inline bool is_installing() const { return _installing; }
    inline int get_index() const { return _index; }
    inline int get_count() const { return _count; }
    inline bool is_age_restricted() const { return _age_restricted; }
    inline String get_title_id() const { return _title_id; }
};

class GDKPackage: public RefCounted {
    GDCLASS(GDKPackage, RefCounted);

protected:
    static void _bind_methods();

public:
    static Dictionary get_current_process_package_identifier();

    static int change_chunk_install_order(const String& package_identifier, TypedArray<Ref<GDKXPackageChunkSelector>> selectors);
    static Dictionary find_chunk_availability(const String& package_identifier, TypedArray<Ref<GDKXPackageChunkSelector>> selectors);
    static int enumerate_chunk_availability(const String& package_identifier, GDKXPackageChunkSelector::Enum selector_type, const Callable& callback);
    static int enumerate_features(const String& package_identifier, const Callable& callback);
    static int enumerate_packages(GDKXPackageKind::Enum kind, GDKXPackageEnumerationScope::Enum scope, const Callable& callback);
    static Dictionary create_installation_monitor(  const String& package_identifier, 
                                                    TypedArray<Ref<GDKXPackageChunkSelector>> selectors,
                                                    int32_t minimum_update_interval_ms);
    static Dictionary estimate_download_size(const String& package_identifier, TypedArray<Ref<GDKXPackageChunkSelector>> selectors);
    static Ref<GDKAsyncBlock> mount_with_ui_async(const String& package_identifier);
    static Dictionary get_user_locale();
    static Dictionary get_write_stats();
    static Ref<GDKXPackageInstallationMonitor> install_chunks(  const String& package_identifier, 
                                                                TypedArray<Ref<GDKXPackageChunkSelector>> selectors, 
                                                                int minimum_update_interval_ms, 
                                                                bool suppress_user_confirmation);
    static Ref<GDKAsyncBlock> install_chunks_async( const String& package_identifier, 
                                                    TypedArray<Ref<GDKXPackageChunkSelector>> selectors, 
                                                    int minimum_update_interval_ms, 
                                                    bool suppress_user_confirmation);
    static bool is_packaged_process();
    static bool uninstall_package(const String& package_identifier);    
    static int uninstall_chunks(const String& package_identifier, TypedArray<Ref<GDKXPackageChunkSelector>> selectors);
    static int uninstall_uwp_instance(const String& package_name);
};
}

VARIANT_ENUM_CAST(GDKXPackageChunkSelector::Enum);
VARIANT_ENUM_CAST(GDKXPackageChunkAvailability::Enum);
VARIANT_ENUM_CAST(GDKXPackageKind::Enum);
VARIANT_ENUM_CAST(GDKXPackageEnumerationScope::Enum);