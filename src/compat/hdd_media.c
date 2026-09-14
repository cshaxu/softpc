#include "insignia.h"
#include "host_def.h"

#include "fdisk.h"
#include "lib/storage/medium_interface.h"
#include "machine.h"
#include "hdd_media.h"

typedef struct softpc_disk_media
{
    lib_storage_medium *medium;
    IU32 total_sectors;
} softpc_disk_media;

static softpc_disk_media softpc_hdd_media[2];
static const CHAR *softpc_hdd_config_paths[2];
#define SOFTPC_DISK_SECTOR_BYTES 512u

const CHAR *softpc_hdd_media_config_path(unsigned int index)
{
    if (index >= 2u)
        return NULL;
    return softpc_hdd_config_paths[index];
}

static int softpc_hdd_attach_media(softpc_disk_media *media, const char *path,
                                   softpc_media_mode mode)
{
    lib_storage_medium_mode storage_mode;
    size_t bytes;

    media->medium = NULL;
    media->total_sectors = 0u;
    if (path == NULL)
        return 1;
    storage_mode = mode == SOFTPC_MEDIA_DIRECT ? LIB_STORAGE_MEDIUM_DIRECT :
        mode == SOFTPC_MEDIA_READONLY ? LIB_STORAGE_MEDIUM_READONLY :
        LIB_STORAGE_MEDIUM_OVERLAY;
    if (lib_storage_medium_open(path, storage_mode, &media->medium) !=
        LIB_STATUS_OK) return 0;
    bytes = lib_storage_medium_byte_count(media->medium);
    if (bytes < SOFTPC_DISK_SECTOR_BYTES) goto attach_failed;
    media->total_sectors = (IU32)(bytes / SOFTPC_DISK_SECTOR_BYTES);
    return 1;
attach_failed:
    lib_storage_medium_destroy(&media->medium);
    return 0;
}

int softpc_platform_hdd_attach(const char *hard_disk_path, softpc_media_mode mode)
{
    unsigned int index;
    for (index = 0u; index < 2u; ++index)
    {
        lib_storage_medium_destroy(&softpc_hdd_media[index].medium);
        softpc_hdd_media[index].total_sectors = 0u;
    }
    /* Fixed disks belong solely to the original fixed-disk controller.
       Removable media is attached separately through original FLA/GFI/FDC. */
    softpc_hdd_config_paths[0] = hard_disk_path;
    softpc_hdd_config_paths[1] = NULL;
    if (!softpc_hdd_attach_media(&softpc_hdd_media[0], softpc_hdd_config_paths[0], mode))
        return 0;
    if (!softpc_hdd_attach_media(&softpc_hdd_media[1], softpc_hdd_config_paths[1], mode))
    {
        lib_storage_medium_destroy(&softpc_hdd_media[0].medium);
        softpc_hdd_config_paths[0] = NULL;
        softpc_hdd_config_paths[1] = NULL;
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
    }
    softpc_hdd_config_paths[0] = NULL;
    softpc_hdd_config_paths[1] = NULL;
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

int host_fdisk_rd(driveid, offset, sectors, buffer)
int driveid;
int offset;
int sectors;
char *buffer;
{
    softpc_disk_media *media;
    size_t bytes;
    if (driveid < 0 || driveid >= 2 || offset < 0 || sectors < 0)
        return 0;
    media = &softpc_hdd_media[driveid];
    bytes = (size_t)sectors * SOFTPC_DISK_SECTOR_BYTES;
    if (media->medium == NULL || (IU32)offset > media->total_sectors * SOFTPC_DISK_SECTOR_BYTES || bytes > (size_t)(media->total_sectors * SOFTPC_DISK_SECTOR_BYTES - (IU32)offset))
    {
        return 0;
    }
    if (lib_storage_medium_read_at(media->medium, (size_t)offset, buffer,
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
    softpc_disk_media *media;
    size_t bytes;
    if (driveid < 0 || driveid >= 2 || offset < 0 || sectors < 0)
        return 0;
    media = &softpc_hdd_media[driveid];
    bytes = (size_t)sectors * SOFTPC_DISK_SECTOR_BYTES;
    if (media->medium == NULL || (IU32)offset > media->total_sectors * SOFTPC_DISK_SECTOR_BYTES || bytes > (size_t)(media->total_sectors * SOFTPC_DISK_SECTOR_BYTES - (IU32)offset))
    {
        return 0;
    }
    if (lib_storage_medium_write_at(media->medium, (size_t)offset, buffer,
            bytes) != LIB_STATUS_OK)
        return 0;
    return 1;
}

void host_fdisk_seek0(driveid) int driveid;
{
    UNUSED(driveid);
}
