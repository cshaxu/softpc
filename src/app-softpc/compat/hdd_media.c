#include "lib/types/types_interface.h"
#include "insignia.h"
#include "host_def.h"
#include "platform.h"

#include "fdisk.h"
#include "lib/storage/medium_interface.h"
#include "hdd_media.h"
#include "media_snapshot.h"

typedef struct softpc_disk_media
{
    lib_storage_medium *medium;
    lib_storage_medium_mode mode;
    IU32 total_sectors;
    char path[SOFTPC_MEDIA_ARCHIVE_PATH_MAX];
} softpc_disk_media;

static softpc_disk_media softpc_hdd_media[2];
#define SOFTPC_DISK_SECTOR_BYTES 512u

const CHAR *softpc_hdd_media_config_path(unsigned int index)
{
    if (index >= 2u)
        return NULL;
    return softpc_hdd_media[index].path[0] == '\0' ? NULL :
        softpc_hdd_media[index].path;
}

static int softpc_hdd_attach_media(softpc_disk_media *media, const char *path,
                                   lib_storage_medium_mode mode)
{
    lib_size bytes;

    media->medium = NULL;
    media->mode = mode;
    media->total_sectors = 0u;
    media->path[0] = '\0';
    if (path == NULL)
        return 1;
    if (lib_text_length(path) >= sizeof(media->path)) return 0;
    if (lib_storage_medium_open(path, mode, &media->medium) !=
        LIB_STATUS_OK) return 0;
    bytes = lib_storage_medium_byte_count(media->medium);
    if (bytes < SOFTPC_DISK_SECTOR_BYTES) goto attach_failed;
    media->total_sectors = (IU32)(bytes / SOFTPC_DISK_SECTOR_BYTES);
    lib_memory_copy(media->path, path, lib_text_length(path) + 1u);
    return 1;
attach_failed:
    lib_storage_medium_destroy(&media->medium);
    return 0;
}

int softpc_platform_hdd_attach(const char *hard_disk_path, lib_storage_medium_mode mode)
{
    unsigned int index;
    for (index = 0u; index < 2u; ++index)
    {
        lib_storage_medium_destroy(&softpc_hdd_media[index].medium);
        softpc_hdd_media[index].total_sectors = 0u;
        softpc_hdd_media[index].path[0] = '\0';
    }
    /* Fixed disks belong solely to the original fixed-disk controller.
       Removable media is attached separately through original FLA/GFI/FDC. */
    if (!softpc_hdd_attach_media(&softpc_hdd_media[0], hard_disk_path, mode))
        return 0;
    if (!softpc_hdd_attach_media(&softpc_hdd_media[1], NULL, mode))
    {
        lib_storage_medium_destroy(&softpc_hdd_media[0].medium);
        softpc_hdd_media[0].path[0] = '\0';
        return 0;
    }
    return 1;
}

void softpc_platform_hdd_detach(void)
{
    unsigned int index;
    fdisk_iodetach();
    for (index = 0u; index < 2u; ++index)
    {
        lib_storage_medium_destroy(&softpc_hdd_media[index].medium);
        softpc_hdd_media[index].total_sectors = 0u;
        softpc_hdd_media[index].path[0] = '\0';
    }
}

void host_fdisk_get_params(driveid, cylinders, heads, sectors) int driveid;
int *cylinders;
int *heads;
int *sectors;
{
    IU32 total_sectors = 0u;
    if (driveid >= 0 && driveid < 2)
        total_sectors = softpc_hdd_media[driveid].total_sectors;
    *heads = 16;
    *sectors = 63;
    *cylinders = (int)(total_sectors / ((IU32)*heads * (IU32)*sectors));
    if (*cylinders < 1)
        *cylinders = 1;
    if (*cylinders > 16383)
        *cylinders = 16383;
}

static softpc_disk_media *softpc_hdd_transfer(int driveid, int offset,
    int sectors, lib_size *bytes)
{
    softpc_disk_media *media;
    if (driveid < 0 || driveid >= 2 || offset < 0 || sectors < 0)
        return NULL;
    media = &softpc_hdd_media[driveid];
    *bytes = (lib_size)sectors * SOFTPC_DISK_SECTOR_BYTES;
    if (media->medium == NULL || (IU32)offset > media->total_sectors * SOFTPC_DISK_SECTOR_BYTES || *bytes > (lib_size)(media->total_sectors * SOFTPC_DISK_SECTOR_BYTES - (IU32)offset))
        return NULL;
    return media;
}

int host_fdisk_rd(driveid, offset, sectors, buffer)
int driveid;
int offset;
int sectors;
char *buffer;
{
    lib_size bytes;
    softpc_disk_media *media = softpc_hdd_transfer(driveid, offset, sectors, &bytes);
    if (media == NULL) return 0;
    if (lib_storage_medium_read_at(media->medium, (lib_size)offset, buffer,
            bytes) != LIB_STATUS_OK)
        return 0;
    return 1;
}

int host_fdisk_wt(driveid, offset, sectors, buffer)
int driveid;
int offset;
int sectors;
char *buffer;
{
    lib_size bytes;
    softpc_disk_media *media = softpc_hdd_transfer(driveid, offset, sectors, &bytes);
    if (media == NULL) return 0;
    if (lib_storage_medium_write_at(media->medium, (lib_size)offset, buffer,
            bytes) != LIB_STATUS_OK)
        return 0;
    return 1;
}

void host_fdisk_seek0(driveid) int driveid;
{
    UNUSED(driveid);
}

void softpc_hdd_media_view(unsigned slot, softpc_media_view *view)
{
    *view = (softpc_media_view){0};
    if (slot >= 2u) return;
    view->medium = softpc_hdd_media[slot].medium;
    view->path = softpc_hdd_media[slot].path;
    view->mode = softpc_hdd_media[slot].mode;
}

lib_status softpc_hdd_media_restore(unsigned slot, const char *path,
    lib_storage_medium_mode mode, lib_storage_medium **replacement)
{
    lib_storage_medium *retired = NULL;
    lib_status status;
    softpc_disk_media *media;
    if (slot >= 2u || replacement == NULL || mode > LIB_STORAGE_MEDIUM_OVERLAY ||
        (path != NULL && lib_text_length(path) >= sizeof(softpc_hdd_media[slot].path)))
        return LIB_STATUS_INVALID_ARGUMENT;
    media = &softpc_hdd_media[slot];
    if (path == NULL) {
        if (*replacement != NULL) return LIB_STATUS_INVALID_ARGUMENT;
        (void)lib_storage_medium_destroy(&media->medium);
        *media = (softpc_disk_media){0};
        return LIB_STATUS_OK;
    }
    if (*replacement != NULL) {
        if (lib_storage_medium_byte_count(*replacement) < SOFTPC_DISK_SECTOR_BYTES)
            return LIB_STATUS_INVALID_ARGUMENT;
        status = lib_storage_medium_replace(&media->medium, *replacement, &retired);
        if (status != LIB_STATUS_OK) return status;
        *replacement = NULL;
        (void)lib_storage_medium_destroy(&retired);
        media->mode = mode;
        media->total_sectors = (IU32)(lib_storage_medium_byte_count(media->medium) /
            SOFTPC_DISK_SECTOR_BYTES);
        lib_memory_copy(media->path, path, lib_text_length(path) + 1u);
        return LIB_STATUS_OK;
    }
    if (media->medium == NULL || media->mode != mode || lib_text_compare(media->path, path) != 0)
        return LIB_STATUS_INVALID_ARGUMENT;
    return LIB_STATUS_OK;
}
