#include "insignia.h"
#include "bios.h"

/*
 * The original ROM communicates with machine-resident C services through
 * C4 C4..C7 BIOS Operations.  This is deliberately only a machine device
 * table: it carries no product/session dispatcher and no fallback service.
 * A ROM call can reach a handler only after that handler has been restored
 * and registered below.
 */
IBOOL softpc_device_bop_dispatch IFN2(IU8, number, IU32, argument)
{
    void (*handler)();

    UNUSED(argument);
    handler = BIOS[number];
    if (handler == 0)
        return FALSE;
    (*handler)();
    return TRUE;
}
