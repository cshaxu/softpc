#include "insignia.h"
#include "host_def.h"
#include "platform.h"

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

/* Debugger access is external translation, not CPU instruction execution:
 * no #PF, no CR2 update and no page-table accessed/dirty mutation. Preflight
 * every byte (the copied debug ABI permits at most 32) before any bus access.
 * Physical SAS access then preserves A20, ROM and C-VID behavior without
 * translating the address a second time through c_sas_loads/stores. */
extern IBOOL xtrn2phy(LIN_ADDR linear, IUM8 access, PHY_ADDR *physical);
int softpc_platform_debug_memory(IU32 address, IU8 *data, IU32 bytes, int write)
{
    PHY_ADDR physical[32];
    IU32 index;
    SAS_MEM_TYPE type;
    if (data == NULL || bytes > 32u || bytes == 0u || address > 0xffffffffu - (bytes - 1u))
        return 0;
    for (index = 0; index < bytes; ++index) {
        physical[index] = address + index;
        if ((c_getCR0() & 0x80000000u) &&
            !xtrn2phy(address + index, write ? 1 : 0, &physical[index])) return 0;
        if (c_sas_twenty_bit_wrapping_enabled()) physical[index] &= 0xfffffu;
        if (physical[index] >= softpc_ram_size) return 0;
        type = c_sas_memory_type(physical[index]);
        if (type != SAS_RAM && type != SAS_VIDEO && !(type == SAS_ROM && !write))
            return 0;
    }
    for (index = 0; index < bytes; ++index) {
        if (write) phy_w8(physical[index], data[index]);
        else data[index] = phy_r8(physical[index]);
    }
    return 1;
}
