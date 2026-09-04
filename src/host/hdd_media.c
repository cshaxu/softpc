#include "insignia.h"
#include "host_def.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fdisk.h"
#include "machine.h"
#include "hdd_media.h"

typedef struct softpc_disk_media
{
    FILE *file;
    unsigned char *data;
    size_t data_bytes;
    softpc_media_mode mode;
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
    FILE *file;
    long bytes;

    media->file = NULL;
    media->data = NULL;
    media->data_bytes = 0u;
    media->total_sectors = 0u;
    if (path == NULL)
        return 1;
    file = fopen(path, mode == SOFTPC_MEDIA_DIRECT ? "rb+" : "rb");
    if (file == NULL)
        return 0;
    if (fseek(file, 0L, SEEK_END) != 0)
        goto attach_failed;
    bytes = ftell(file);
    if (bytes < (long)SOFTPC_DISK_SECTOR_BYTES ||
        fseek(file, 0L, SEEK_SET) != 0)
        goto attach_failed;
    if (mode == SOFTPC_MEDIA_OVERLAY)
    {
        media->data = malloc((size_t)bytes);
        if (media->data == NULL || fread(media->data, 1u, (size_t)bytes, file) !=
                                       (size_t)bytes)
            goto attach_failed;
        fclose(file);
        media->data_bytes = (size_t)bytes;
    }
    else
    {
        media->file = file;
    }
    media->mode = mode;
    media->total_sectors = (IU32)((unsigned long)bytes /
                                  SOFTPC_DISK_SECTOR_BYTES);
    return 1;
attach_failed:
    fclose(file);
    free(media->data);
    media->data = NULL;
    media->data_bytes = 0u;
    return 0;
}

int softpc_platform_hdd_attach(const char *hard_disk_path, softpc_media_mode mode)
{
    unsigned int index;
    for (index = 0u; index < 2u; ++index)
    {
        if (softpc_hdd_media[index].file != NULL)
            fclose(softpc_hdd_media[index].file);
        free(softpc_hdd_media[index].data);
        softpc_hdd_media[index].file = NULL;
        softpc_hdd_media[index].data = NULL;
        softpc_hdd_media[index].data_bytes = 0u;
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
        if (softpc_hdd_media[0].file != NULL)
            fclose(softpc_hdd_media[0].file);
        free(softpc_hdd_media[0].data);
        softpc_hdd_media[0].file = NULL;
        softpc_hdd_media[0].data = NULL;
        softpc_hdd_media[0].data_bytes = 0u;
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
        if (softpc_hdd_media[index].file != NULL)
            fclose(softpc_hdd_media[index].file);
        free(softpc_hdd_media[index].data);
        softpc_hdd_media[index].file = NULL;
        softpc_hdd_media[index].data = NULL;
        softpc_hdd_media[index].data_bytes = 0u;
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
    if ((media->data == NULL && media->file == NULL) || (IU32)offset > media->total_sectors * SOFTPC_DISK_SECTOR_BYTES || bytes > (size_t)(media->total_sectors * SOFTPC_DISK_SECTOR_BYTES - (IU32)offset))
    {
        return 0;
    }
    if (media->mode == SOFTPC_MEDIA_OVERLAY)
        memcpy(buffer, media->data + offset, bytes);
    else if (fseek(media->file, (long)offset, SEEK_SET) != 0 ||
             fread(buffer, 1u, bytes, media->file) != bytes)
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
    if ((media->data == NULL && media->file == NULL) ||
        media->mode == SOFTPC_MEDIA_READONLY || (IU32)offset > media->total_sectors * SOFTPC_DISK_SECTOR_BYTES || bytes > (size_t)(media->total_sectors * SOFTPC_DISK_SECTOR_BYTES - (IU32)offset))
    {
        return 0;
    }
    if (media->mode == SOFTPC_MEDIA_OVERLAY)
        memcpy(media->data + offset, buffer, bytes);
    else if (fseek(media->file, (long)offset, SEEK_SET) != 0 ||
             fwrite(buffer, 1u, bytes, media->file) != bytes ||
             fflush(media->file) != 0)
        return 0;
    return 1;
}

void host_fdisk_seek0(driveid) int driveid;
{
    UNUSED(driveid);
}
