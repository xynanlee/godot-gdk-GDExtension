#include "gdk_asyncblock.h"

using namespace godot;

GDKAsyncBlock::GDKAsyncBlock() {
    _block = new XAsyncBlock();
    ZeroMemory(_block, sizeof(XAsyncBlock));

    _block->context = this;
}

void GDKAsyncBlock::_bind_methods() {
    ADD_SIGNAL(MethodInfo("completed", PropertyInfo(Variant::DICTIONARY, "result")));
}

void GDKAsyncBlock::_notification(int p_what) {
    if (p_what == NOTIFICATION_PREDELETE && _block) {
        delete _block;
        _block = nullptr;
    }
}

Ref<GDKAsyncBlock> GDKAsyncBlock::create(XTaskQueueHandle queue) {
    Ref<GDKAsyncBlock> wrapper;
    wrapper.instantiate();

    XAsyncBlock* b = wrapper->_block;
    b->queue = queue;
    return wrapper;
}

void GDKAsyncBlock::set_callback(XAsyncCompletionRoutine* callback) {
    _block->callback = callback;
}

void GDKAsyncBlock::emit(Dictionary data) {
    emit_signal("completed", data);
}
