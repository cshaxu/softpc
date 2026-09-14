#ifndef SOFTPC_HOST_STATUS_H
#define SOFTPC_HOST_STATUS_H

#include "lib/types/types_interface.h"
#include <stdio.h>
#include <stdlib.h>

/* Terminal host infrastructure failure: never unwind into callers which may
 * free a still-running worker's dependencies. Expected API outcomes are handled
 * by the caller before reaching this application boundary. */
static inline void softpc_host_require_status(lib_status status, const char *operation)
{
    if (status == LIB_STATUS_OK) return;
    fprintf(stderr, "softpcvm: %s failed (status=%d); terminating.\n", operation, status);
    _Exit(EXIT_FAILURE);
}

#endif
