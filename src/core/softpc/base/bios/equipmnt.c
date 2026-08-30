#include "insignia.h"
#include "host_def.h"
/*
 * SoftPC Revision 3.0
 *
 * Title        : equipment.c
 *
 * Description  : BIOS equipment function.  Returns a word containing
 *                a bit pattern representing the equipment supported
 *                by the virtual bios.
 *
 * Author       : Henry Nash / David Rees
 *
 * Notes        : Now reads the word from the appropriate place within
 *                the BIOS data area (40:10H).
 */

#include <stdio.h>
#include TypesH

#include "xt.h"
#include CpuH
#include "bios.h"
#include "sas.h"
#include "equip.h"

void equipment()
{
    EQUIPMENT_WORD equip_flag;

    sas_loadw(EQUIP_FLAG, &equip_flag.all);
    setAX(equip_flag.all);
}
