#include "softpc_physical_mapping.h"

int softpc_physical_mapping_translate(uint32_t address, uint32_t *mapped_address)
{
    if (mapped_address != 0) *mapped_address = address;
    return 0;
}

int softpc_physical_mapping_resolve(uint32_t address, uint8_t **host_byte)
{
    (void)address;
    if (host_byte != 0) *host_byte = 0;
    return 0;
}
