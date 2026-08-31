#include "insignia.h"
#include "host_def.h"
/* Original SoftPC ROM BASIC fallback service. */

#include <stdio.h>
#include TypesH

#include "xt.h"
#include CpuH
#include "host.h"
#include "error.h"
#include "sas.h"

void rom_basic()
{
	host_error(EG_NO_ROM_BASIC, ERR_CONT, NULL);
}
