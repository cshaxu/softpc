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
#include <windows.h>
#include "host_def.h"
#include "insignia.h"

#include "sndblst.h"
#include "nt_sb.h"
#include "compat/devices/snapshot.h"

BOOL FMPortWrite(void);

/*****************************************************************************
*
*    Globals
*
*****************************************************************************/

HMIDIOUT HFM = NULL;     // current open FM device.  If no zero, it means we
                       // successfully open the FM synth device and apps
                       // have the direct IO to adlib port.
BOOL FMActive = FALSE; // TRUE: kernel is emulating or apps have full access
                       // FALSE: User mode (us) is doint the emulation

BYTE AdlibRegister = 0x00; // register currently selected
int Position = 0; // position in PortData array
SYNTH_DATA PortData[BATCH_SIZE]; // batched data to be written to OPL2 device
BOOL Timer1Started = FALSE; // if a timer interrupts then it's stopped
BOOL Timer2Started = FALSE; // if a timer interrupts then it's stopped
BYTE Status = 0x06; // or 0x00, see sb programming book page xi


/****************************************************************************
*
*    FM device routines
*
****************************************************************************/

void softpc_device_snapshot_capture_sb_fm(softpc_device_sb_fm_state *state)
{
    state->selected = AdlibRegister;
    state->status = Status;
}

int softpc_device_snapshot_restore_sb_fm(const softpc_device_sb_fm_state *state)
{
    IU8 flags = state->status & 0xe0;
    if ((state->status & 0x1f) != 0 && (state->status & 0x1f) != 6)
        return FALSE;
    if (flags != 0 && flags != 0xa0 && flags != 0xc0 && flags != 0xe0)
        return FALSE;
    AdlibRegister = state->selected;
    Status = state->status;
    return TRUE;
}


VOID
ResetFM(
    VOID
    )
{
    AdlibRegister = 0x00; // register currently selected
    Position = 0;
    Timer1Started = FALSE;
    Timer2Started = FALSE;
    Status = 0x06;
}

VOID
FMStatusRead(
    BYTE *data
    )
{
#if 0 // This should work but doesn't (ReadFile fails)
#error "Unsupported SoftPC mirror configuration: disabled branch"
#endif
#line 102
        *data = Status;
}

VOID
FMRegisterSelect(
    BYTE data
    )
{
    AdlibRegister = data;
}

VOID
FMDataWrite(
    BYTE data
    )
{
    if(AdlibRegister==AD_NEW) {
        data &=0xFE; // don't enter opl3 mode
    }

    // put data in PortData array
    if(Position <= BATCH_SIZE-2) {
        PortData[Position].IoPort = ADLIB_REGISTER_SELECT_PORT;
        PortData[Position].PortData = AdlibRegister;
        PortData[Position + 1].IoPort = ADLIB_DATA_PORT;
        PortData[Position + 1].PortData = data;
        Position += 2;
    } else {
        dprintf1(("Attempting to write beyond end of PortData array"));
    }

    if (Position == BATCH_SIZE ||
        AdlibRegister>=0xB0 && AdlibRegister<=0xBD ||
        AdlibRegister == AD_MASK) {
        // PortData full or note-on/off command or changing status
        if (!FMPortWrite()) {
            dprintf1(("Failed to write to device!"));
        } else {
            // Work out what status change may have occurred
            if (AdlibRegister == AD_MASK) {
                // Look for RST and starting timers
                if (data & 0x80) {
                    Status = 0x00; // reset both timers
            }

                // We ignore starting of timers if their interrupt
                // flag is set because the timer status will have to
                // be set again to make the status for this timer change

                if ((data & 1) && !(Status & 0x40)) {
                    dprintf2(("Timer 1 started"));
#if 0
#error "Unsupported SoftPC mirror configuration: disabled branch"
#else
#line 156
                    Status |= 0xC0; // simulate immediate expiry of timer1
#endif
#line 158
                } else {
                    Timer1Started = FALSE;
                }

                if ((data & 2) && !(Status & 0x20)) {
                    dprintf2(("Timer 2 started"));
#if 0
#error "Unsupported SoftPC mirror configuration: disabled branch"
#else
#line 167
                    Status |= 0xA0; // simulate immediate expiry of timer2
#endif
#line 169
                    Timer2Started = TRUE;
                } else {
                    Timer2Started = FALSE;
                }
           }
        }
    }
}


/***************************************************************************/

/*
*    Sends FM data to the card.
*    Returns TRUE on success.
*/

BOOL
FMPortWrite(
    VOID
    )
{
    DWORD bytesWritten = 0;
    LPVOID lpMsgBuf;

    if(FMActive) {
        dprintf4(("Writing %d bytes of data to port",
          Position * sizeof(PortData[0])));
        if(!WriteFile(HFM, &PortData, Position * sizeof(PortData[0]),
          &bytesWritten, NULL)) {
#if DBG
#error "Unsupported SoftPC mirror configuration: native FM debugging"
#endif //DBG
#line 209
             return FALSE;
        }
    }
    Position = 0;
    return TRUE;
}

BOOL
FindFMSynthDevice(
    PUINT FMDevice
    )

/*++

Routine Description:

    This function finds a FM synth output device.

Arguments:

    FMDevice - supplies a pointer to a variable to receive the FMDevice number.

Return Value:

    TRUE - if device is found.
    FALSE - no device is found.

--*/

{
    UINT numDev;
    UINT device;
    MIDIOUTCAPS mc;

    numDev = MidiGetNumDevsProc();

    for (device = 0; device < numDev; device++) {
        if (MMSYSERR_NOERROR == MidiGetDevCapsProc(device, &mc, sizeof(mc))) {

            //
            // Need FM Synth device
            //

            if (mc.wTechnology == MOD_FMSYNTH) {
                *FMDevice = device;
                return (TRUE);
            }
        }
    }

    dprintf1(("FM Synth device not found"));
    return (FALSE);
}





