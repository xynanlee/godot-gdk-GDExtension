#include "gdk_system.h"

using namespace godot;

void GDKXVersion::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_int_value"), &GDKXVersion::get_int_value);
    ClassDB::bind_method(D_METHOD("get_readable_text"), &GDKXVersion::get_readable_text);
}

Ref<GDKXVersion> GDKXVersion::create(const XVersion *version) {
    Ref<GDKXVersion> wrapper;
    if (version != nullptr) {
        wrapper.instantiate();
        wrapper->_version = *version;
    }
	return wrapper;
}

int GDKXVersion::get_int_value() const {
	return _version.Value;
}

String GDKXVersion::get_readable_text() const {
    return vformat("%05d.%05d.%05d.%05d", _version.major, _version.minor, _version.build, _version.revision);
}
