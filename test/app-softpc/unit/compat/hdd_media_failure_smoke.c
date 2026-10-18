#include "media_fixture.h"

static unsigned transfers;
static lib_size transfer_offset, transfer_bytes;

static lib_status read_medium(const lib_storage_medium *medium, lib_size offset,
    void *buffer, lib_size bytes)
{
    assert(medium != NULL && buffer != NULL);
    ++transfers; transfer_offset = offset; transfer_bytes = bytes;
    return LIB_STATUS_OK;
}

static lib_status write_medium(lib_storage_medium *medium, lib_size offset,
    const void *buffer, lib_size bytes)
{ return read_medium(medium, offset, (void *)buffer, bytes); }

#define lib_storage_medium_open open_medium
#define lib_storage_medium_destroy close_medium
#define lib_storage_medium_byte_count medium_bytes
#define lib_storage_medium_read_at read_medium
#define lib_storage_medium_write_at write_medium
#include "compat/hdd_media.c"
#undef lib_storage_medium_write_at
#undef lib_storage_medium_read_at
#undef lib_storage_medium_byte_count
#undef lib_storage_medium_destroy
#undef lib_storage_medium_open

static void check_hdd(void)
{
    lib_storage_medium *replacement = NULL;
    softpc_media_view view;
    unsigned calls;
    assert(softpc_platform_hdd_attach("first", LIB_STORAGE_MEDIUM_OVERLAY));
    assert(lib_storage_medium_create_zero_overlay(512u, &replacement) == LIB_STATUS_OK);
    assert(softpc_hdd_media_restore(1u, "other", LIB_STORAGE_MEDIUM_OVERLAY,
        &replacement) == LIB_STATUS_OK);
    fail_close = 1; calls = opens;
    assert(!softpc_platform_hdd_attach("second", LIB_STORAGE_MEDIUM_OVERLAY));
    assert(opens == calls); /* Both consumed leases cleared before returning. */
    softpc_hdd_media_view(0u, &view);
    assert(view.medium == NULL && view.path[0] == '\0');
    softpc_hdd_media_view(1u, &view);
    assert(view.medium == NULL && view.path[0] == '\0');
    fail_close = 0;
    assert(softpc_platform_hdd_attach("first", LIB_STORAGE_MEDIUM_OVERLAY));
    assert(lib_storage_medium_create_zero_overlay(512u, &replacement) == LIB_STATUS_OK);
    fail_close = 1;
    assert(softpc_hdd_media_restore(0u, "replacement", LIB_STORAGE_MEDIUM_OVERLAY,
        &replacement) == LIB_STATUS_IO_ERROR);
    softpc_hdd_media_view(0u, &view);
    assert(replacement == NULL && view.medium != NULL);
    assert(lib_text_compare(view.path, "replacement") == 0);
    assert(softpc_hdd_media_restore(0u, NULL, LIB_STORAGE_MEDIUM_OVERLAY,
        &replacement) == LIB_STATUS_IO_ERROR);
    softpc_hdd_media_view(0u, &view);
    assert(view.medium == NULL && view.path[0] == '\0');
    calls = closes;
    assert(softpc_hdd_media_restore(0u, NULL, LIB_STORAGE_MEDIUM_OVERLAY,
        &replacement) == LIB_STATUS_OK && closes == calls);
    fail_close = 0;
}

static void check_hdd_bounds(void)
{
    const lib_size limit = (lib_size)INT_MAX + 1u;
    lib_storage_medium *replacement = NULL;
    char buffer[512];
    unsigned calls;
    opened_bytes = limit;
    assert(softpc_platform_hdd_attach("limit", LIB_STORAGE_MEDIUM_OVERLAY));
    assert(open_medium("restored-limit", LIB_STORAGE_MEDIUM_OVERLAY, &replacement) == LIB_STATUS_OK);
    assert(softpc_hdd_media_restore(0u, "restored-limit", LIB_STORAGE_MEDIUM_OVERLAY,
        &replacement) == LIB_STATUS_OK && replacement == NULL);
    assert(host_fdisk_rd(0, 0, 1, buffer));
    assert(host_fdisk_wt(0, INT_MAX - 511, 1, buffer));
    assert(transfer_offset == limit - 512u && transfer_bytes == 512u);
    calls = transfers;
    assert(!host_fdisk_rd(0, INT_MAX - 511, 2, buffer));
    assert(!host_fdisk_wt(0, 0, INT_MAX, buffer));
    assert(!host_fdisk_rd(0, -1, 1, buffer));
    assert(!host_fdisk_rd(0, 0, -1, buffer));
    assert(transfers == calls);
    assert(open_medium("too-large", LIB_STORAGE_MEDIUM_OVERLAY, &replacement) == LIB_STATUS_OK);
    reported_bytes = limit + 512u;
    assert(softpc_hdd_media_restore(0u, "too-large", LIB_STORAGE_MEDIUM_OVERLAY,
        &replacement) == LIB_STATUS_INVALID_ARGUMENT && replacement != NULL);
    assert(softpc_hdd_media[0].total_sectors == 4194304u);
    assert(close_medium(&replacement) == LIB_STATUS_OK);
    opened_bytes = limit + 512u;
    assert(!softpc_platform_hdd_attach("too-large", LIB_STORAGE_MEDIUM_OVERLAY));
    assert(softpc_hdd_media[0].medium == NULL);
    if (sizeof(lib_size) > 4u) {
        opened_bytes = (lib_size)((lib_u64)1u << 32);
        assert(!softpc_platform_hdd_attach("4-GiB", LIB_STORAGE_MEDIUM_OVERLAY));
    }
    opened_bytes = 511u;
    assert(!softpc_platform_hdd_attach("short", LIB_STORAGE_MEDIUM_OVERLAY));
    opened_bytes = 512u;
    assert(softpc_platform_hdd_attach("small", LIB_STORAGE_MEDIUM_OVERLAY));
    assert(host_fdisk_rd(0, 0, 1, buffer));
    assert(!host_fdisk_rd(0, 512, 1, buffer));
    softpc_platform_hdd_detach();
}

int main(void)
{
    check_hdd();
    check_hdd_bounds();
    puts("App-SoftPC HDD media failure and capacity: OK");
    return 0;
}
