#ifndef SOFTPC_GDP_RULE_ACCESS_H
#define SOFTPC_GDP_RULE_ACCESS_H

#include "softpc_gdp_state.h"

#include <stdint.h>

/* The generated C-VID rules identify fields by the original fixed 32-bit GDP
 * byte offset.  The side table owns native-width storage on both x86 and x64. */
void *softpc_gdp_rule_slot(void *state, unsigned int original_offset,
    size_t native_width);
void *softpc_gdp_rule_address(void *state, uintptr_t original_address,
    size_t native_width);

#endif
