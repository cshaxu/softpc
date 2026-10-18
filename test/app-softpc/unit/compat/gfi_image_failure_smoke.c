#include "media_fixture.h"

#define lib_storage_medium_open open_medium
#define lib_storage_medium_destroy close_medium
#define lib_storage_medium_byte_count medium_bytes
#include "compat/gfi_image.c"
#undef lib_storage_medium_byte_count
#undef lib_storage_medium_destroy
#undef lib_storage_medium_open

int main(void)
{
    lib_storage_medium *replacement = NULL;
    softpc_media_view view;
    unsigned calls;
    assert(softpc_platform_floppy_attach("first", LIB_STORAGE_MEDIUM_OVERLAY));
    fail_close = 1;
    assert(!softpc_platform_floppy_attach("second", LIB_STORAGE_MEDIUM_OVERLAY));
    softpc_floppy_media_view(0u, &view);
    assert(view.medium != NULL && lib_text_compare(view.path, "second") == 0);
    assert(lib_storage_medium_create_zero_overlay(512u, &replacement) == LIB_STATUS_OK);
    assert(softpc_floppy_media_restore(0u, "third", LIB_STORAGE_MEDIUM_OVERLAY,
        &replacement, 31u) == LIB_STATUS_IO_ERROR);
    softpc_floppy_media_view(0u, &view);
    assert(replacement == NULL && view.medium != NULL && view.cylinder == 31u);
    assert(lib_text_compare(view.path, "third") == 0);
    assert(softpc_floppy_media_restore(0u, NULL, LIB_STORAGE_MEDIUM_OVERLAY,
        &replacement, 0u) == LIB_STATUS_IO_ERROR);
    softpc_floppy_media_view(0u, &view);
    assert(view.medium == NULL && view.path[0] == '\0');
    assert(softpc_gfi_drives[0].drive_type != GFI_DRIVE_TYPE_NULL);
    calls = closes;
    assert(softpc_floppy_media_restore(0u, NULL, LIB_STORAGE_MEDIUM_OVERLAY,
        &replacement, 0u) == LIB_STATUS_OK && closes == calls);
    fail_close = 0;
    assert(open_medium("oversize", LIB_STORAGE_MEDIUM_OVERLAY, &replacement) == LIB_STATUS_OK);
    reported_bytes = (lib_size)LONG_MAX + 1u;
    assert(softpc_floppy_media_restore(0u, "oversize", LIB_STORAGE_MEDIUM_OVERLAY,
        &replacement, 0u) == LIB_STATUS_INVALID_ARGUMENT && replacement != NULL);
    assert(close_medium(&replacement) == LIB_STATUS_OK);
    puts("App-SoftPC GFI media failure and capacity: OK");
    return 0;
}
