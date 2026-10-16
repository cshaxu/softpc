/***************************************************************************
*
*    nt_sbmixer.c
*
*    Copyright (c) 1991-1996 Microsoft Corporation.  All Rights Reserved.
*
*    This code provides VDD support for SB 2.0 sound output, specifically:
*        Mixer Chip CT1335 (not strictly part of SB 2.0, but apps seem to like it)
*
***************************************************************************/

#include "insignia.h"
#include "host_def.h"
#include "ios.h"

/* T93: use the existing core scalar ABI, not WinMM host types. */
#include "sndblst.h"
#include "nt_sb.h"

void MixerSetMasterVolume(IU8 level);
void MixerSetVoiceVolume(IU8 level);
void MixerSetMidiVolume(IU8 level);

//
// Mixer globals
//

struct {
    IU8 MasterVolume;     // current master volume
    IU8 FMVolume;         // current volume of fm device
    IU8 CDVolume;         // current volume of cd
    IU8 VoiceVolume;      // current volume of wave device
} MixerSettings;

//
// Mixer State Machine
//

enum {
    MixerReset = 1,        // initial state and after reset
    MixerMasterVolume,
    MixerFMVolume,
    MixerCDVolume,
    MixerVoiceVolume
} MixerState = MixerReset; // state of current command/data being set

void
MixerDataRead(
    IU8 * data
    )

/*++

Routine Description:

    This function handles reading back of preselected volume level.

Arguments:

    data - supplies the address to receive the level data

Return Value:

    None.

--*/

{
    switch(MixerState) {
    case MixerReset:
        ResetMixer();
        break;

    case MixerMasterVolume:
        *data = MixerSettings.MasterVolume;
        break;

    case MixerFMVolume:
        *data = MixerSettings.FMVolume;
        break;

    case MixerCDVolume:
        *data = MixerSettings.CDVolume;
        break;

    case MixerVoiceVolume:
        *data = MixerSettings.VoiceVolume;
        break;
    }
}

void
MixerAddrWrite(
    IU8 data
    )

/*++

Routine Description:

    This function handles setting register index to the addr register.

Arguments:

    data - register index

Return Value:

    None.

--*/

{
    switch(data) {
    case MIXER_RESET:
        MixerState = MixerReset;
        break;

    case MIXER_MASTER_VOLUME:
        MixerState = MixerMasterVolume;
        break;

    case MIXER_FM_VOLUME:
        MixerState = MixerFMVolume;
        break;

    case MIXER_CD_VOLUME:
        MixerState = MixerCDVolume;
        break;

    case MIXER_VOICE_VOLUME:
        MixerState = MixerVoiceVolume;
        break;
    }
}

void
MixerDataWrite(
    IU8 data
    )

/*++

Routine Description:

    This function sets mixer volume

Arguments:

    data - specifies the level of reset index register

Return Value:

    None.

--*/

{
    switch(MixerState) {
    case MixerReset:
        ResetMixer();
        break;

    case MixerMasterVolume:
        MixerSetMasterVolume(data);
        break;

    case MixerFMVolume:
        MixerSetMidiVolume(data);
        break;

    case MixerCDVolume:
        MixerSettings.CDVolume = data;
        break;

    case MixerVoiceVolume:
        MixerSetVoiceVolume(data);
        break;
    }
}

void
ResetMixer(
    void
    )

/*++

Routine Description:

    This function resets mixer to its default states.

Arguments:

    None.

Return Value:

    None

--*/

{
    MixerSetMasterVolume(0x08);
    MixerSetMidiVolume(0x08);
    MixerSettings.CDVolume = 0x00; // set to level 0
    MixerSetVoiceVolume(0x04);

    MixerState = MixerReset;
}

void
MixerSetMasterVolume(
    IU8 level
    )

/*++

Routine Description:

    This function sets master volume level.

Arguments:

    level - only bit 1, 2, 3

Return Value:

    None.

--*/

{
    MixerSettings.MasterVolume = level;
    SbApplyMixer();
}

void
MixerSetVoiceVolume(
    IU8 level
    )

/*++

Routine Description:

    This function sets mixer volume of wave out device.

Arguments:

    level - wave out volume level. (only bit 1 and 2)

Return Value:

    None.

--*/

{
    MixerSettings.VoiceVolume = level;
    SbApplyMixer();
}

void
MixerSetMidiVolume(
    IU8 level
    )

/*++

Routine Description:

    This function sets mixer volume of FM/MIDI out device.

Arguments:

    level - wave out volume level. (only bit 1 and 2)

Return Value:

    None.

--*/

{
    MixerSettings.FMVolume = level;
    SbApplyMixer();
}


/* T93: retain XP's positive-level scale, but zero is silent. Compose the
 * existing device registers here; the host must not own another mixer. */
static IU32 MixerGain(IU8 level, IU8 mask, IU32 step)
{
    IU32 decoded = (level >> 1) & mask;
    return decoded ? decoded * step - 1 : 0;
}

IU16 MixerGetVoiceGain(void)
{
    IU32 master = MixerGain(MixerSettings.MasterVolume, 7, 0x2000);
    IU32 voice = MixerGain(MixerSettings.VoiceVolume, 3, 0x4000);
    return (IU16)((master * voice) / 65535);
}

IU16 MixerGetMidiGain(void)
{
    IU32 master = MixerGain(MixerSettings.MasterVolume, 7, 0x2000);
    IU32 midi = MixerGain(MixerSettings.FMVolume, 7, 0x2000);
    return (IU16)((master * midi) / 65535);
}
