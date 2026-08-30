#include "insignia.h"
#include "host_def.h"
/*
 * VPC-XT Revision 1.0
 *
 * Title        : memory_size
 *
 * Description  : Returns the memory size of the virtual PC memory
 */

#include TypesH

#include "xt.h"
#include CpuH
#include "bios.h"
#include "sas.h"

void memory_size()
{
    word memory_size;

    /* Read the BIOS data area: applications are allowed to change it. */
    sas_loadw(MEMORY_VAR, &memory_size);
    setAX(memory_size);
}
