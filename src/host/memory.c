#include "insignia.h"
#include "host_def.h"

#include <stdlib.h>
#include <string.h>

#include "cpu4.h"
#include "sas.h"

/* This is the sole host allocation backing the original SAS machine memory.
   All guest-visible physical reads and writes below remain routed through the
   recovered SAS/C-VID implementation rather than exposing this buffer. */
static UTINY *softpc_ram;
static sys_addr softpc_ram_size;

UTINY *host_sas_init(sys_addr size)
{
    softpc_ram = (UTINY *)calloc((size_t)size + 0x2000u, 1u);
    softpc_ram_size = softpc_ram == NULL ? 0 : size;
    return softpc_ram;
}

UTINY *host_sas_term(void)
{
    free(softpc_ram);
    softpc_ram = NULL;
    softpc_ram_size = 0;
    return NULL;
}

PVOID softpc_xms_physical_pointer(ULONG address, ULONG length)
{
    if (softpc_ram == NULL || address > softpc_ram_size ||
        length > softpc_ram_size - address)
        return NULL;
    return softpc_ram + address;
}

PVOID softpc_xms_guest_pointer(USHORT segment, USHORT offset)
{
    return softpc_xms_physical_pointer(((ULONG)segment << 4) + offset, 1u);
}

BOOL softpc_xms_copy_physical(ULONG destination, ULONG source, ULONG length)
{
    PVOID destination_pointer = softpc_xms_physical_pointer(destination, length);
    PVOID source_pointer = softpc_xms_physical_pointer(source, length);
    if (destination_pointer == NULL || source_pointer == NULL)
        return FALSE;
    memmove(destination_pointer, source_pointer, length);
    sas_overwrite_memory(destination, length);
    return TRUE;
}

int softpc_platform_write_physical(IU32 address, const IU8 *bytes, IU32 length)
{
    if (bytes == NULL || address > softpc_ram_size ||
        length > softpc_ram_size - address)
        return 0;
    /* A physical bus write must take the same original SAS route as reads.
       Direct host-RAM copying bypasses SAS_VIDEO, so it fails to update
       C-VID's EGA planes and its original dirty-marking algorithm. */
    c_sas_stores(address, (IU8 *)bytes, length);
    return 1;
}

int softpc_platform_read_physical(IU32 address, IU8 *bytes, IU32 length)
{
    if (bytes == NULL || address > softpc_ram_size ||
        length > softpc_ram_size - address)
        return 0;
    /* A physical bus read must honour SAS_VIDEO and other original mapping
       types. Reading host_sas memory directly bypasses C-VID's EGA planes,
       making B8000h appear empty even after a guest has written text. */
    c_sas_loads(address, bytes, length);
    return 1;
}
