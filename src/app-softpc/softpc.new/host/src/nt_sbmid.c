#include "insignia.h"
#include "host_def.h"
#include "sndblst.h"
#include "nt_sb.h"
#include "compat/sb_midi.h"
#include "compat/devices/snapshot.h"

/* T93: complete copied messages replace native CMDHDR/MIDIHDR ring framing.
 * Guest parsing remains here; Compat owns only the host output. No pointer
 * is narrowed to a buffer index and no following command flushes a tail. */
static IU8 MidiBuffer[LIB_AUDIO_MIDI_SYSEX_MAX_BYTES];
static IU32 NextData;
static IU32 BytesLeft;
static IU32 LastCommandLength;
static IU8 RunningStatus;
static IU8 RealtimePending;
static IBOOL DiscardSysEx;

/* T93: logical sounding notes survive a host pause, not native handles or
 * envelopes. One voice per channel/key is the deliberate host-MIDI limit. */
typedef struct { IU8 velocity, held, sostenuto; } MIDI_NOTE;
static MIDI_NOTE MidiNotes[16][128];
static IU8 MidiSustain[16], MidiSostenuto[16];
static IBOOL MidiRunning = TRUE;

/* T93: copied parser/voice state; native output is rebuilt by the SB owner. */
void softpc_device_snapshot_capture_sb_midi(softpc_device_sb_midi_state *state)
{
    IU32 channel, key;
    _Static_assert(sizeof(state->buffer) == sizeof(MidiBuffer), "MIDI archive capacity");
    memcpy(state->buffer, MidiBuffer, sizeof(MidiBuffer));
    state->next_data = NextData;
    state->bytes_left = BytesLeft;
    state->last_length = LastCommandLength;
    state->running_status = RunningStatus;
    state->realtime = RealtimePending;
    state->discard_sysex = DiscardSysEx;
    memcpy(state->sustain, MidiSustain, sizeof(MidiSustain));
    memcpy(state->sostenuto, MidiSostenuto, sizeof(MidiSostenuto));
    for (channel = 0; channel < 16; ++channel)
        for (key = 0; key < 128; ++key) {
            state->notes[channel][key][0] = MidiNotes[channel][key].velocity;
            state->notes[channel][key][1] = MidiNotes[channel][key].held;
            state->notes[channel][key][2] = MidiNotes[channel][key].sostenuto;
        }
}

int softpc_device_snapshot_restore_sb_midi(const softpc_device_sb_midi_state *state)
{
    IU32 channel, key, i, length = 0;
    IU8 command = state->buffer[0], running = state->running_status;
    if (state->next_data > sizeof(MidiBuffer) || state->bytes_left > 2 ||
        state->discard_sysex > 1 ||
        (running && (running < 0x80 || running > 0xf0)) ||
        (state->realtime && (state->realtime < 0xf8 ||
            state->realtime == 0xf9 || state->realtime == 0xfd))) return FALSE;
    if (running >= 0x80 && running < 0xf0)
        length = (running >= 0xc0 && running < 0xe0) ? 2 : 3;
    if (state->last_length != length) return FALSE;
    if (running == 0xf0 || (state->next_data && command == 0xf0)) {
        if (!state->next_data || command != 0xf0 || state->bytes_left ||
            (running == 0xf0 && state->next_data >= sizeof(MidiBuffer)) ||
            (running != 0xf0 && (running || state->next_data < 2 ||
                state->buffer[state->next_data - 1] != 0xf7)) ||
            (state->discard_sysex && (running != 0xf0 ||
                state->next_data != sizeof(MidiBuffer) - 1))) return FALSE;
        for (i = 1; i < state->next_data; ++i)
            if (state->buffer[i] >= 0x80 &&
                !(running == 0 && i == state->next_data - 1)) return FALSE;
    } else {
        if (state->discard_sysex) return FALSE;
        if (state->next_data) {
            if (command < 0x80 || command >= 0xf7 ||
                command == 0xf4 || command == 0xf5) return FALSE;
            length = command == 0xf6 ? 1 :
                ((command >= 0xc0 && command < 0xe0) ||
                command == 0xf1 || command == 0xf3) ? 2 : 3;
            if (state->next_data + state->bytes_left != length ||
                (command < 0xf0 ? running != command : running != 0)) return FALSE;
            for (i = 1; i < state->next_data; ++i)
                if (state->buffer[i] >= 0x80) return FALSE;
        } else if (state->bytes_left) return FALSE;
    }
    for (channel = 0; channel < 16; ++channel) {
        if (state->sustain[channel] > 1 || state->sostenuto[channel] > 1)
            return FALSE;
        for (key = 0; key < 128; ++key)
            if (state->notes[channel][key][0] > 127 ||
                state->notes[channel][key][1] > 1 ||
                state->notes[channel][key][2] > 1) return FALSE;
    }
    memcpy(MidiBuffer, state->buffer, sizeof(MidiBuffer));
    NextData = state->next_data;
    BytesLeft = state->bytes_left;
    LastCommandLength = state->last_length;
    RunningStatus = running;
    RealtimePending = state->realtime;
    DiscardSysEx = state->discard_sysex;
    memcpy(MidiSustain, state->sustain, sizeof(MidiSustain));
    memcpy(MidiSostenuto, state->sostenuto, sizeof(MidiSostenuto));
    for (channel = 0; channel < 16; ++channel)
        for (key = 0; key < 128; ++key) {
            MidiNotes[channel][key].velocity = state->notes[channel][key][0];
            MidiNotes[channel][key].held = state->notes[channel][key][1];
            MidiNotes[channel][key].sostenuto = state->notes[channel][key][2];
        }
    MidiRunning = FALSE;
    return TRUE;
}

static void PruneMidiNotes(IU32 channel)
{
    IU32 key;
    for (key = 0; key < 128; ++key) {
        MIDI_NOTE *note = &MidiNotes[channel][key];
        if (!note->held && !MidiSustain[channel] && !note->sostenuto)
            note->velocity = 0;
    }
}

static void RememberMidiMessage(const IU8 *bytes)
{
    IU32 channel = bytes[0] & 15, key;
    IU8 kind = bytes[0] & 0xf0;
    MIDI_NOTE *note;
    if (kind == 0x80 || kind == 0x90) {
        note = &MidiNotes[channel][bytes[1]];
        if (kind == 0x90 && bytes[2]) {
            note->velocity = bytes[2];
            note->held = TRUE;
        } else {
            note->held = FALSE;
            PruneMidiNotes(channel);
        }
    } else if (kind == 0xb0) {
        IU8 control = bytes[1], value = bytes[2];
        if (control == 64) MidiSustain[channel] = value >= 64;
        if (control == 66) {
            if (!MidiSostenuto[channel] && value >= 64)
                for (key = 0; key < 128; ++key)
                    MidiNotes[channel][key].sostenuto = MidiNotes[channel][key].held;
            MidiSostenuto[channel] = value >= 64;
        }
        if (control == 121) MidiSustain[channel] = MidiSostenuto[channel] = FALSE;
        for (key = 0; key < 128; ++key) {
            note = &MidiNotes[channel][key];
            if (!MidiSostenuto[channel]) note->sostenuto = FALSE;
            if (control == 120) note->velocity = note->held = note->sostenuto = 0;
            if (control >= 123) note->held = FALSE;
        }
        PruneMidiNotes(channel);
    }
}

static lib_status RestoreMidiNotes(void)
{
    IU32 channel, key, phase;
    lib_status status;
    IU8 message[3];
    for (channel = 0; channel < 16; ++channel) {
        message[0] = (IU8)(0xb0 | channel); message[2] = 0;
        message[1] = 64;
        status = softpc_sb_midi_write(message, 3);
        if (status != LIB_STATUS_OK) return status;
        message[1] = 66;
        status = softpc_sb_midi_write(message, 3);
        if (status != LIB_STATUS_OK) return status;
        /* Rebuild the sostenuto capture before playing later notes. Restore
         * sustain before releasing keys which were pedal-held at pause. */
        for (phase = 0; phase < 2; ++phase) {
            for (key = 0; key < 128; ++key) {
                MIDI_NOTE *note = &MidiNotes[channel][key];
                if (!note->velocity || (note->sostenuto != 0) != (phase == 0)) continue;
                message[0] = (IU8)(0x90 | channel);
                message[1] = (IU8)key; message[2] = note->velocity;
                status = softpc_sb_midi_write(message, 3);
                if (status != LIB_STATUS_OK) return status;
            }
            if ((phase == 0 && MidiSostenuto[channel]) ||
                (phase == 1 && MidiSustain[channel])) {
                message[0] = (IU8)(0xb0 | channel);
                message[1] = phase == 0 ? 66 : 64; message[2] = 127;
                status = softpc_sb_midi_write(message, 3);
                if (status != LIB_STATUS_OK) return status;
            }
        }
        for (key = 0; key < 128; ++key) {
            if (!MidiNotes[channel][key].velocity || MidiNotes[channel][key].held) continue;
            message[0] = (IU8)(0x80 | channel);
            message[1] = (IU8)key; message[2] = 0;
            status = softpc_sb_midi_write(message, 3);
            if (status != LIB_STATUS_OK) return status;
        }
    }
    return LIB_STATUS_OK;
}

lib_status MidiSetRunning(IBOOL running)
{
    lib_status status;
    if (!running) {
        if (!MidiRunning) return LIB_STATUS_OK;
        MidiRunning = FALSE;
        return softpc_sb_midi_reset();
    }
    if (MidiRunning) return LIB_STATUS_OK;
    /* A cancelled/failed restore may have submitted a prefix. Start from
     * silence on every actual re-entry, never duplicate that prefix. */
    status = softpc_sb_midi_reset();
    if (status == LIB_STATUS_OK) status = RestoreMidiNotes();
    if (status == LIB_STATUS_OK) MidiRunning = TRUE;
    return status;
}

void ResetMidiParser(void)
{
    NextData = 0;
    BytesLeft = 0;
    LastCommandLength = 0;
    RunningStatus = 0;
    RealtimePending = 0;
    DiscardSysEx = FALSE;
    memset(MidiNotes, 0, sizeof(MidiNotes));
    memset(MidiSustain, 0, sizeof(MidiSustain));
    memset(MidiSostenuto, 0, sizeof(MidiSostenuto));
    MidiRunning = TRUE;
}

lib_status SendMidiRequest(void)
{
    lib_status result;
    /* A stop can release the paused executor before its CCPU exit signal.
     * Servicing retained output while paused is idle, not a host fault. */
    if (!MidiRunning) return LIB_STATUS_OK;
    if (RealtimePending) {
        result = softpc_sb_midi_write(&RealtimePending, 1);
        if (result != LIB_STATUS_OK) return result;
        if (RealtimePending == 0xff) {
            memset(MidiNotes, 0, sizeof(MidiNotes));
            memset(MidiSustain, 0, sizeof(MidiSustain));
            memset(MidiSostenuto, 0, sizeof(MidiSostenuto));
        }
        RealtimePending = 0;
    }
    if (NextData && !BytesLeft && RunningStatus != 0xf0) {
        result = softpc_sb_midi_write(MidiBuffer, NextData);
        if (result != LIB_STATUS_OK) return result;
        RememberMidiMessage(MidiBuffer);
        NextData = 0;
    }
    return LIB_STATUS_OK;
}

lib_status BufferMidi(IU8 data)
{
    lib_status result;
    if (!MidiRunning) return LIB_STATUS_INVALID_STATE;
    result = SendMidiRequest();
    if (result != LIB_STATUS_OK) return result; /* No new byte accepted. */

    if (data >= 0xf8) {
        /* System realtime does not affect running status or partial data.
         * Undefined F9/FD are not valid host messages. */
        if (data == 0xf9 || data == 0xfd) return LIB_STATUS_OK;
        RealtimePending = data;
        return SendMidiRequest();
    }

    if (RunningStatus == 0xf0) {
        if (data == 0xf7) {
            RunningStatus = 0;
            if (DiscardSysEx) {
                NextData = 0;
                DiscardSysEx = FALSE;
                return LIB_STATUS_UNSUPPORTED;
            }
            MidiBuffer[NextData++] = data;
            return SendMidiRequest();
        }
        if (data < 0x80) {
            /* Reserve one byte for EOX; never emit a truncated SysEx. */
            if (NextData < sizeof(MidiBuffer) - 1)
                MidiBuffer[NextData++] = data;
            else
                DiscardSysEx = TRUE;
            return LIB_STATUS_OK;
        }
        /* A new non-realtime status aborts the incomplete SysEx. */
        NextData = 0;
        DiscardSysEx = FALSE;
        RunningStatus = 0;
    }

    if (data >= 0x80) {
        /* A new status discards an incomplete command, not a valid prefix
         * to be submitted as if it were a complete short message. */
        NextData = 0;
        BytesLeft = 0;
        if (data >= 0xf0) {
            // This is a system common message. It cancels any running status.
            RunningStatus = 0;
            LastCommandLength = 0;
            switch (data) {
            case 0xf0:
                RunningStatus = data;
                MidiBuffer[NextData++] = data;
                return LIB_STATUS_OK;
            case 0xf1:
            case 0xf3:
                BytesLeft = 2;
                break;
            case 0xf2:
                BytesLeft = 3;
                break;
            case 0xf6:
                BytesLeft = 1;
                break;
            default: /* Undefined common status or unpaired EOX. */
                return LIB_STATUS_OK;
            }
        } else {
            // This is the start of a standard midi command.
            // Track the running status.
            RunningStatus = data;
            if (data < 0xc0 || data > 0xdf) {
                BytesLeft = 3;
            } else {
                BytesLeft = 2;
            }
            LastCommandLength = BytesLeft;
        }
    } else if (!BytesLeft) {
        if (!RunningStatus) return LIB_STATUS_OK;
        /* Lib receives complete messages, never implicit running status. */
        MidiBuffer[NextData++] = RunningStatus;
        BytesLeft = LastCommandLength - 1;
    }

    MidiBuffer[NextData++] = data;
    --BytesLeft;
    if (!BytesLeft) return SendMidiRequest();
    return LIB_STATUS_OK;
}
