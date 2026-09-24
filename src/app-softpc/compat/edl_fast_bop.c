/*
 * Historical CCPU fast-BOP host contract.
 *
 * The selected ROM set contains only C4 C4 BIOS Operations.  C4 C4 FE is
 * consumed by the original executor's BOP-FE return path, so no selected-ROM
 * execution reaches this provider.  The historical source nevertheless
 * declares EDL_fast_bop implicitly for C4 C5..C7 fast operations.  Keep the
 * unresolved product contract outside the machine and fail explicitly if a
 * different ROM attempts to use it.
 */

#include <stdio.h>
#include <stdlib.h>

void EDL_fast_bop(unsigned long immediate)
{
    fprintf(stderr,
        "SoftPC: unsupported fast BOP 0x%08lx from this firmware profile\n",
        immediate);
    abort();
}
