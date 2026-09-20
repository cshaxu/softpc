#include "lib/kvm-base/mailbox_interface.h"
#include "lib/kvm-window/frame_interface.h"
#include <assert.h>

static kvm_window_frame source, destination, storage;
static kvm_component_mailboxes mailbox;

static void check_copy(void)
{
    lib_size bytes = kvm_window_frame_size_bytes(&source);
    lib_memory_set(&destination, 0xa5, sizeof(destination));
    assert(kvm_window_frame_copy(&destination, &source));
    assert(lib_memory_compare(&destination, &source, bytes) == 0);
    for (lib_size i = bytes; i < sizeof(destination); ++i)
        assert(((const lib_u8 *)&destination)[i] == 0xa5);
    assert(kvm_window_frame_copy(&source, &source));
}

int main(void)
{
    lib_u32 generation, old;
    lib_memory_set(&source, 0x3c, sizeof(source));
    source.valid = 1u;
    source.graphics = 0u;
    source.text.base.text_columns = 80u;
    source.text.base.text_rows = 25u;
    check_copy(); /* No inactive graphics payload is copied. */
    assert(kvm_component_mailboxes_create(&mailbox, &storage, sizeof(storage)) == LIB_STATUS_OK);
    lib_memory_set(&storage, 0x96, sizeof(storage));
    assert(kvm_component_mailboxes_publish_frame(&mailbox, &source,
        kvm_window_frame_size_bytes(&source), NULL) == LIB_STATUS_OK);
    assert(kvm_component_mailboxes_capture_frame(&mailbox, &generation, &destination, sizeof(destination)));
    assert(destination.text.base.text[1999] == source.text.base.text[1999]);
    for (lib_size i = kvm_window_frame_size_bytes(&source); i < sizeof(source); ++i) {
        assert(((const lib_u8 *)&storage)[i] == 0x96);
        assert(((const lib_u8 *)&destination)[i] == 0xa5);
    }
    old = generation;
    assert(kvm_component_mailboxes_publish_frame(&mailbox, &source, sizeof(storage) + 1u, NULL) == LIB_STATUS_LIMIT_EXCEEDED);
    assert(mailbox.frame_generation == old);
    assert(!kvm_component_mailboxes_capture_frame(&mailbox, &generation, &destination, 1u));
    source.graphics = 1u;
    source.image.width = 3u;
    source.image.stride = 7u;
    source.image.height = 5u;
    check_copy(); /* Copy row padding, not just width*height. */
    assert(kvm_component_mailboxes_publish_frame(&mailbox, &source,
        kvm_window_frame_size_bytes(&source), NULL) == LIB_STATUS_OK);
    kvm_component_mailboxes_acknowledge_frame(&mailbox, old);
    assert(kvm_component_mailboxes_capture_frame(&mailbox, &generation, &destination, sizeof(destination)));
    assert(generation != old);
    assert(lib_memory_compare(destination.image.pixels, source.image.pixels, 35u) == 0);
    assert(destination.image.pixels[35] == 0xa5);
    source.image.width = source.image.stride = KVM_WINDOW_GRAPHICS_MAX_WIDTH;
    source.image.height = KVM_WINDOW_GRAPHICS_MAX_HEIGHT;
    check_copy();
    source.graphics = 0u;
    source.text.base.text_columns = 80u; source.text.base.text_rows = 25u;
    check_copy(); /* Graphics -> text never exposes the old pixels. */
    assert(kvm_component_mailboxes_publish_frame(&mailbox, &source,
        kvm_window_frame_size_bytes(&source), NULL) == LIB_STATUS_OK);
    assert(kvm_component_mailboxes_capture_frame(&mailbox, &generation, &destination, sizeof(destination)));
    assert(!destination.graphics && ((const lib_u8 *)&destination)[kvm_window_frame_size_bytes(&source)] == 0xa5);
    kvm_component_mailboxes_acknowledge_frame(&mailbox, generation);
    assert(!kvm_component_mailboxes_capture_frame(&mailbox, &generation, &destination, sizeof(destination)));
    source.graphics = 1u;
    source.image.stride = KVM_WINDOW_GRAPHICS_MAX_WIDTH + 1u;
    lib_memory_set(&destination, 0xa5, sizeof(destination));
    assert(!kvm_window_frame_copy(&destination, &source));
    assert(!kvm_window_frame_copy(&destination, LIB_NULL));
    assert(!kvm_window_frame_copy(LIB_NULL, &source));
    for (lib_size i = 0u; i < sizeof(destination); ++i)
        assert(((const lib_u8 *)&destination)[i] == 0xa5);
    kvm_component_mailboxes_destroy(&mailbox);
    return 0;
}
