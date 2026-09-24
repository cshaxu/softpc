#include "insignia.h"
#include "host_def.h"
#include "virtual.h"

#include <stdlib.h>

/* One standalone machine has one original mouse-driver instance payload. */
static IHP softpc_instance_data;

IHP *NIDDB_Allocate_Instance_Data(size, create_callback, terminate_callback)
int size;
NIDDB_CR_CALLBACK create_callback;
NIDDB_TM_CALLBACK terminate_callback;
{
    UNUSED(terminate_callback);
    if (size <= 0)
        return NULL;
    free(softpc_instance_data);
    softpc_instance_data = calloc(1u, (size_t)size);
    if (softpc_instance_data != NULL && create_callback != NULL)
        (*create_callback)(&softpc_instance_data);
    return &softpc_instance_data;
}

void NIDDB_Deallocate_Instance_Data(handle)
    IHP *handle;
{
    if (handle == NULL)
        return;
    free(*handle);
    *handle = NULL;
}
