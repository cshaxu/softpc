#include "lib/types/test.h"
#include "insignia.h"
#include "host_def.h"
#include "nt_sb.h"
#include "compat/sb_midi.h"

static lib_u8 messages[64][LIB_AUDIO_MIDI_SYSEX_MAX_BYTES];
static lib_u32 lengths[64], sent, attempts, resets, fail_attempt;
static lib_status host_status;
lib_status softpc_sb_midi_reset(void) { ++resets; return host_status; }

lib_status softpc_sb_midi_write(const lib_u8 *bytes, lib_u32 count)
{
    ++attempts;
    if (fail_attempt && attempts == fail_attempt) return LIB_STATUS_INVALID_STATE;
    if (host_status != LIB_STATUS_OK) return host_status;
    lib_test_assert(sent < 64 && count <= sizeof(messages[0]));
    lib_memory_copy(messages[sent], bytes, count);
    lengths[sent++] = count;
    return LIB_STATUS_OK;
}

static void reset(void)
{
    ResetMidiParser();
    sent = attempts = 0;
    host_status = LIB_STATUS_OK;
}

static void feed_byte(IU8 value)
{
    lib_test_assert(BufferMidi(value) == LIB_STATUS_OK);
}

static void expect(lib_u32 index, const lib_u8 *bytes, lib_u32 count)
{
    lib_test_assert(index < sent && lengths[index] == count);
    for (lib_u32 i = 0; i < count; ++i)
        lib_test_assert(messages[index][i] == bytes[i]);
}

int main(void)
{
    static const lib_u8 channel_lengths[] = {3, 3, 3, 3, 2, 2, 3};
    static const lib_u8 realtime[] = {0xf8, 0xfa, 0xfb, 0xfc, 0xfe, 0xff};
    static const lib_u8 note[] = {0x90, 60, 127};
    static const lib_u8 sysex[] = {0xf0, 1, 2, 0xf7};
    for (lib_u32 status = 0x80; status < 0xf0; ++status) {
        lib_u32 length = channel_lengths[(status - 0x80) / 16];
        lib_u8 expected[] = {(lib_u8)status, 60, 127};
        reset();
        feed_byte((IU8)status); feed_byte(60);
        if (length == 3) { lib_test_assert(sent == 0); feed_byte(127); }
        lib_test_assert(sent == 1); /* First complete message, no next byte. */
        expect(0, expected, length);
        feed_byte(60); if (length == 3) feed_byte(127);
        lib_test_assert(sent == 2);
        expect(1, expected, length); /* Running status materialized once. */
        lib_test_assert(SendMidiRequest() == LIB_STATUS_OK && sent == 2);
    }
    for (lib_u32 i = 0; i < sizeof(realtime); ++i) {
        reset();
        feed_byte(0x90); feed_byte(60); feed_byte(realtime[i]);
        lib_test_assert(sent == 1); expect(0, realtime + i, 1);
        feed_byte(127); expect(1, note, sizeof(note));
        feed_byte(0xf0); feed_byte(1); feed_byte(realtime[i]); feed_byte(2); feed_byte(0xf7);
        expect(2, realtime + i, 1); expect(3, sysex, sizeof(sysex));
        lib_test_assert(sent == 4);
    }
    for (lib_u32 i = 0; i != 4; ++i) {
        static const lib_u8 statuses[] = {0xf1, 0xf2, 0xf3, 0xf6};
        static const lib_u8 sizes[] = {2, 3, 2, 1};
        lib_u8 expected[] = {statuses[i], 1, 2};
        reset(); feed_byte(0x90); feed_byte(60); /* Abort partial channel message. */
        feed_byte(statuses[i]);
        for (lib_u32 j = 1; j < sizes[i]; ++j) feed_byte((IU8)j);
        expect(0, expected, sizes[i]);
        feed_byte(60); feed_byte(127); lib_test_assert(sent == 1); /* No running status. */
    }
    reset(); feed_byte(0x90); feed_byte(60); feed_byte(0xc0); feed_byte(1);
    { const lib_u8 expected[] = {0xc0, 1}; expect(0, expected, 2); }
    feed_byte(0xf4); feed_byte(1); feed_byte(0xf5); feed_byte(1); feed_byte(0xf7);
    feed_byte(0xf9); feed_byte(0xfd); lib_test_assert(sent == 1);
    feed_byte(0xf0); feed_byte(1); feed_byte(0x90); feed_byte(60); feed_byte(127);
    expect(1, note, sizeof(note)); /* Interrupted SysEx is not submitted. */

    reset(); feed_byte(0xf0);
    for (lib_u32 i = 0; i < LIB_AUDIO_MIDI_SYSEX_MAX_BYTES - 2; ++i)
        feed_byte((IU8)(i % 128));
    feed_byte(0xf7);
    lib_test_assert(sent == 1 && lengths[0] == LIB_AUDIO_MIDI_SYSEX_MAX_BYTES);
    lib_test_assert(messages[0][0] == 0xf0 &&
        messages[0][LIB_AUDIO_MIDI_SYSEX_MAX_BYTES - 1] == 0xf7);
    for (lib_u32 i = 0; i < LIB_AUDIO_MIDI_SYSEX_MAX_BYTES - 2; ++i)
        lib_test_assert(messages[0][i + 1] == i % 128);
    for (lib_u32 overflow = 1; overflow != 4; ++overflow) {
        reset(); feed_byte(0xf0);
        for (lib_u32 i = 0; i < LIB_AUDIO_MIDI_SYSEX_MAX_BYTES * overflow; ++i)
            feed_byte(1);
        feed_byte(0xf8); lib_test_assert(sent == 1); /* Realtime survives overflow. */
        lib_test_assert(BufferMidi(0xf7) == LIB_STATUS_UNSUPPORTED && sent == 1);
        feed_byte(0x90); feed_byte(60); feed_byte(127); expect(1, note, sizeof(note));
    }

    reset(); feed_byte(0x90); feed_byte(60); host_status = LIB_STATUS_LIMIT_EXCEEDED;
    lib_test_assert(BufferMidi(127) == LIB_STATUS_LIMIT_EXCEEDED && sent == 0);
    host_status = LIB_STATUS_OK;
    lib_test_assert(SendMidiRequest() == LIB_STATUS_OK);
    expect(0, note, sizeof(note)); /* Drain tail without another guest byte. */
    lib_test_assert(SendMidiRequest() == LIB_STATUS_OK && sent == 1);

    reset(); feed_byte(0x90); feed_byte(60); host_status = LIB_STATUS_INVALID_STATE;
    lib_test_assert(BufferMidi(0xf8) == LIB_STATUS_INVALID_STATE);
    host_status = LIB_STATUS_OK;
    lib_test_assert(SendMidiRequest() == LIB_STATUS_OK);
    feed_byte(127); expect(1, note, sizeof(note));
    lib_test_assert(sent == 2 && messages[0][0] == 0xf8);

    reset(); feed_byte(0xf0); feed_byte(1); feed_byte(2);
    host_status = LIB_STATUS_LIMIT_EXCEEDED;
    lib_test_assert(BufferMidi(0xf7) == LIB_STATUS_LIMIT_EXCEEDED);
    lib_test_assert(BufferMidi(0x90) == LIB_STATUS_LIMIT_EXCEEDED && sent == 0);
    host_status = LIB_STATUS_OK;
    lib_test_assert(SendMidiRequest() == LIB_STATUS_OK);
    expect(0, sysex, sizeof(sysex));
    feed_byte(60); feed_byte(127); lib_test_assert(sent == 1); /* Rejected 90 not parsed. */

    reset(); feed_byte(0x90); feed_byte(60); host_status = LIB_STATUS_IO_ERROR;
    lib_test_assert(BufferMidi(127) == LIB_STATUS_IO_ERROR);
    ResetMidiParser(); host_status = LIB_STATUS_OK;
    lib_test_assert(SendMidiRequest() == LIB_STATUS_OK && sent == 0);
    feed_byte(60); feed_byte(127); lib_test_assert(sent == 0); /* No stale running status. */
    reset(); feed_byte(0x90); feed_byte(60); feed_byte(100);
    feed_byte(0xb0); feed_byte(66); feed_byte(127); /* Capture 60, not later 62. */
    feed_byte(0x90); feed_byte(62); feed_byte(110);
    feed_byte(0x80); feed_byte(60); feed_byte(0);
    sent = 0; resets = 0;
    lib_test_assert(MidiSetRunning(FALSE) == LIB_STATUS_OK && resets == 1);
    lib_test_assert(MidiSetRunning(FALSE) == LIB_STATUS_OK && resets == 1);
    lib_test_assert(SendMidiRequest() == LIB_STATUS_OK && sent == 0);
    lib_test_assert(BufferMidi(0x90) == LIB_STATUS_INVALID_STATE && sent == 0);
    lib_test_assert(MidiSetRunning(TRUE) == LIB_STATUS_OK && resets == 2);
    {
        const lib_u8 expected[][3] = {
            {0xb0,64,0}, {0xb0,66,0}, {0x90,60,100}, {0xb0,66,127},
            {0x90,62,110}, {0x80,60,0}
        };
        for (lib_u32 i = 0; i < 6; ++i) expect(i, expected[i], 3);
        lib_test_assert(sent == 36);
        lib_test_assert(MidiSetRunning(TRUE) == LIB_STATUS_OK && sent == 36);
    }
    /* Releasing sostenuto retires only its released key; held 62 survives. */
    feed_byte(0xb0); feed_byte(66); feed_byte(0);
    lib_test_assert(MidiSetRunning(FALSE) == LIB_STATUS_OK);
    sent = 0; host_status = LIB_STATUS_INVALID_STATE;
    lib_test_assert(MidiSetRunning(TRUE) == LIB_STATUS_INVALID_STATE && sent == 0);
    host_status = LIB_STATUS_OK;
    lib_test_assert(MidiSetRunning(TRUE) == LIB_STATUS_OK && sent == 33);
    { const lib_u8 held[] = {0x90,62,110}; expect(2, held, 3); }
    /* All Sound Off clears the continuation, including pedal-held notes. */
    feed_byte(0xb0); feed_byte(120); feed_byte(0);
    lib_test_assert(MidiSetRunning(FALSE) == LIB_STATUS_OK);
    sent = 0;
    lib_test_assert(MidiSetRunning(TRUE) == LIB_STATUS_OK && sent == 32);
    reset(); feed_byte(0x90); feed_byte(60); feed_byte(100);
    feed_byte(0xb0); feed_byte(64); feed_byte(127);
    feed_byte(123); feed_byte(0); /* All Notes Off still respects sustain. */
    lib_test_assert(MidiSetRunning(FALSE) == LIB_STATUS_OK);
    sent = 0;
    lib_test_assert(MidiSetRunning(TRUE) == LIB_STATUS_OK && sent == 35);
    {
        const lib_u8 expected[][3] = {{0x90,60,100},{0xb0,64,127},{0x80,60,0}};
        for (lib_u32 i = 0; i < 3; ++i) expect(i + 2, expected[i], 3);
    }
    feed_byte(0xb0); feed_byte(121); feed_byte(0); /* Reset controllers releases pedal. */
    lib_test_assert(MidiSetRunning(FALSE) == LIB_STATUS_OK);
    sent = 0;
    lib_test_assert(MidiSetRunning(TRUE) == LIB_STATUS_OK && sent == 32);
    reset(); feed_byte(0x90); feed_byte(60); feed_byte(100);
    lib_test_assert(MidiSetRunning(FALSE) == LIB_STATUS_OK);
    sent = attempts = resets = 0; fail_attempt = 4;
    lib_test_assert(MidiSetRunning(TRUE) == LIB_STATUS_INVALID_STATE && sent == 3);
    fail_attempt = 0; sent = 0;
    lib_test_assert(MidiSetRunning(TRUE) == LIB_STATUS_OK && sent == 33 && resets == 2);
    { const lib_u8 expected[] = {0x90,60,100}; expect(2, expected, 3); }
    return 0;
}
