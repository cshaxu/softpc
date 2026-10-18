#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include "lib/storage/medium_interface.h"

static unsigned closes, opens;
static int fail_close;
static lib_size opened_bytes = 512u, reported_bytes;
static lib_storage_medium *sized_medium;

/* Real small leases, scripted close status and reported size: no host media,
   native audio device or multi-gigabyte allocation is needed by these units. */
static lib_status close_medium(lib_storage_medium **medium)
{
    int fail = *medium != NULL && fail_close;
    if (*medium != NULL) {
        ++closes;
        if (*medium == sized_medium) sized_medium = NULL;
    }
    assert(lib_storage_medium_destroy(medium) == LIB_STATUS_OK);
    return fail ? LIB_STATUS_IO_ERROR : LIB_STATUS_OK;
}

static lib_status open_medium(const char *path, lib_storage_medium_mode mode,
    lib_storage_medium **medium)
{
    (void)path; (void)mode;
    ++opens;
    assert(lib_storage_medium_create_zero_overlay(512u, medium) == LIB_STATUS_OK);
    sized_medium = *medium;
    reported_bytes = opened_bytes;
    return LIB_STATUS_OK;
}

static lib_size medium_bytes(const lib_storage_medium *medium)
{
    return medium == sized_medium ? reported_bytes :
        lib_storage_medium_byte_count(medium);
}
