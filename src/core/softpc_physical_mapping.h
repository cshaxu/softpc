#ifndef SOFTPC_PHYSICAL_MAPPING_H
#define SOFTPC_PHYSICAL_MAPPING_H

#include <stdint.h>

/* A machine-local physical mapping port.  It has no session, VDM, guest
 * pointer, DOS page-alias or host-resource semantics.  Normal SoftPC RAM is
 * resolved directly by SAS when neither hook claims an address. */
int softpc_physical_mapping_translate(uint32_t address, uint32_t *mapped_address);
int softpc_physical_mapping_resolve(uint32_t address, uint8_t **host_byte);

#endif
