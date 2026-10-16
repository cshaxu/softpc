#include "lib/types/test.h"
#include "lib/types/file.h"
#include "compat/sb_midi.h"
#include "insignia.h"
#include "host_def.h"
#include "nt_sb.h"

/* The executor is quiescent here; parser, Compat and native Lib output are
 * real. This test does not emulate CPU execution or measure acoustic output. */
int softpc_ccpu_lifecycle_exit_requested(void) { return 0; }

static void send_bytes(const lib_u8 *bytes, lib_size count)
{
    for (lib_size i = 0; i < count; ++i)
        lib_test_assert(BufferMidi(bytes[i]) == LIB_STATUS_OK);
}

int main(void)
{
    const lib_u8 note[] = {0x90, 60, 1};
    const lib_u8 off[] = {0x80, 60, 0};
    /* Universal identity request: no instrument/program reset. */
    const lib_u8 identity[] = {0xf0, 0x7e, 0x7f, 0x06, 0x01, 0xf7};
    lib_status status;
    lib_test_assert(softpc_sb_midi_prepare() == LIB_STATUS_OK);
    status = softpc_sb_midi_open(0);
    if (status == LIB_STATUS_UNSUPPORTED) {
        lib_test_assert(softpc_sb_midi_dispose() == LIB_STATUS_OK);
        lib_c_printf("SoftPC MIDI native capability unavailable\n");
        return 77;
    }
    lib_test_assert(status == LIB_STATUS_OK);
    ResetMidiParser();
    send_bytes(note, sizeof(note));
    lib_test_assert(MidiSetRunning(FALSE) == LIB_STATUS_OK);
    lib_test_assert(MidiSetRunning(TRUE) == LIB_STATUS_OK);
    send_bytes(off, sizeof(off));
    send_bytes(identity, sizeof(identity));
    /* Exercise the cancelled long-message ordering wait, if still pending. */
    lib_test_assert(softpc_sb_midi_cancel() == LIB_STATUS_OK);
    send_bytes(note, sizeof(note));
    send_bytes(off, sizeof(off));
    lib_test_assert(softpc_sb_midi_close() == LIB_STATUS_OK);
    lib_test_assert(softpc_sb_midi_dispose() == LIB_STATUS_OK);
    lib_c_printf("SoftPC parser/Compat/native MIDI output: OK\n");
    return 0;
}
