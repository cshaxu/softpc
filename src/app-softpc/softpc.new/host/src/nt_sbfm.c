/***************************************************************************
*
*    nt_sbfm.c
*
*    Copyright (c) 1991-1996 Microsoft Corporation.  All Rights Reserved.
*
*    This code provides VDD support for SB 2.0 sound output, specifically:
*        FM Chip OPL2 (a.k.a. Adlib)
*
***************************************************************************/
#include "insignia.h"
#include "host_def.h"
#include "sndblst.h"
#include "nt_sb.h"

/* T93: one no-host device state; native output capability belongs to Compat.
 * No native port batches, timer thread or synthetic audio backend is retained. */
static IU8 AdlibRegister = 0x00;
static IU8 Status = 0x06;

void
ResetFM(
    void
    )
{
    AdlibRegister = 0x00; // register currently selected
    Status = 0x06;
}

void
FMStatusRead(
    IU8 *data
    )
{
    *data = Status;
}

void
FMRegisterSelect(
    IU8 data
    )
{
    AdlibRegister = data;
}

void
FMDataWrite(
    IU8 data
    )
{
    /* T93: match XP's active no-hardware fallback. Note registers also
     * update immediate-expiry flags; only AD_MASK can clear them. This is
     * status compatibility, not a claim of a running OPL timer/synthesizer. */
    if ((AdlibRegister >= 0xB0 && AdlibRegister <= 0xBD) ||
        AdlibRegister == AD_MASK) {
        if (AdlibRegister == AD_MASK && (data & 0x80))
            Status = 0x00;
        if ((data & 1) && !(Status & 0x40))
            Status |= 0xC0; // simulate immediate expiry of timer1
        if ((data & 2) && !(Status & 0x20))
            Status |= 0xA0; // simulate immediate expiry of timer2
    }
}
