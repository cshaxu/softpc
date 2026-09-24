#include "machine.h"
#include "insignia.h"
#include "host_def.h"
#include "cpu4.h"
#include "sas.h"

/* Debugger access is external translation, not CPU instruction execution:
 * no #PF, no CR2 update and no page-table accessed/dirty mutation. Preflight
 * every byte (the copied debug ABI permits at most 32) before any bus access.
 * Physical SAS access then preserves A20, ROM and C-VID behavior without
 * translating the address a second time through c_sas_loads/stores. */
extern IBOOL xtrn2phy(LIN_ADDR linear, IUM8 access, PHY_ADDR *physical);
int softpc_machine_debug_memory(IU32 address, IU8 *data, IU32 bytes, int write)
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
        if (physical[index] >= c_sas_memory_size()) return 0;
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
