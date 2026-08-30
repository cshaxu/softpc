#ifndef SOFTPC_GDP_STATE_H_H
#define SOFTPC_GDP_STATE_H_H

#include <stddef.h>

/*
 * Private to the selected CCPU/CVIDC mirror.  GDP fields are native SoftPC
 * controller state, not guest addresses or externally visible identities.
 */
void *softpc_gdp_create(void);
void softpc_gdp_destroy(void *state);
void *softpc_gdp_slot(const void *state, unsigned int original_offset,
    size_t native_width);

#endif
