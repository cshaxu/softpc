#include "lib/types/test.h"
#include "lib/base/sync_interface.h"
#include "compat/sb_midi.h"

struct base_sync_mutex { lib_bool held; };
struct lib_audio_midi_output { lib_u32 token; };
static base_sync_mutex guard;
static lib_audio_midi_output output;
static lib_status create_status, volume_status, reset_status, destroy_status;
static lib_status send_status, wait_status, cancel_status;
static lib_u16 current_left = 12000, current_right = 23000;
static lib_u32 creates, destroys, resets, shorts, longs, waits, cancels, sets;
static lib_bool exit_requested, inject_cancel, stale_cancel;

int softpc_ccpu_lifecycle_exit_requested(void) { return exit_requested; }
lib_status base_sync_mutex_create(base_sync_mutex **value)
{ *value = &guard; return LIB_STATUS_OK; }
void base_sync_mutex_destroy(base_sync_mutex *value)
{ lib_test_assert(!value || (value == &guard && !guard.held)); }
void base_sync_mutex_lock(base_sync_mutex *value)
{ lib_test_assert(value == &guard && !guard.held); guard.held = LIB_TRUE; }
void base_sync_mutex_unlock(base_sync_mutex *value)
{ lib_test_assert(value == &guard && guard.held); guard.held = LIB_FALSE; }
lib_status lib_audio_midi_output_create(lib_audio_midi_output **value)
{ ++creates; *value = create_status == LIB_STATUS_OK ? &output : LIB_NULL; return create_status; }
lib_status lib_audio_midi_output_get_volume(lib_audio_midi_output *value,
    lib_u16 *left, lib_u16 *right)
{ lib_test_assert(value == &output); *left = current_left; *right = current_right; return volume_status; }
lib_status lib_audio_midi_output_set_volume(lib_audio_midi_output *value,
    lib_u16 left, lib_u16 right)
{
    lib_test_assert(value == &output); ++sets;
    if (volume_status == LIB_STATUS_OK) { current_left = left; current_right = right; }
    return volume_status;
}
lib_status lib_audio_midi_output_cancel_wait(lib_audio_midi_output *value)
{ lib_test_assert(value == &output && guard.held); ++cancels; return cancel_status; }
lib_status lib_audio_midi_output_reset(lib_audio_midi_output *value)
{
    lib_test_assert(value == &output && !guard.held); ++resets;
    if (stale_cancel) { stale_cancel = LIB_FALSE; return LIB_STATUS_INVALID_STATE; }
    return reset_status;
}
lib_status lib_audio_midi_output_destroy(lib_audio_midi_output **value)
{
    lib_test_assert(*value == &output && guard.held); ++destroys;
    if (destroy_status == LIB_STATUS_OK) *value = LIB_NULL;
    return destroy_status;
}
lib_status lib_audio_midi_output_send_short(lib_audio_midi_output *value,
    const lib_u8 *bytes, lib_u32 count)
{
    lib_test_assert(value == &output && !guard.held && count == 3 && bytes[0] == 0x90);
    ++shorts;
    if (stale_cancel) { stale_cancel = LIB_FALSE; return LIB_STATUS_INVALID_STATE; }
    return send_status;
}
lib_status lib_audio_midi_output_send_sysex(lib_audio_midi_output *value,
    const lib_u8 *bytes, lib_u32 count)
{
    lib_test_assert(value == &output && !guard.held && count == 3 && bytes[0] == 0xf0);
    ++longs; return send_status;
}
lib_status lib_audio_midi_output_wait_writable(lib_audio_midi_output *value)
{
    lib_test_assert(value == &output && !guard.held); ++waits;
    if (inject_cancel) {
        exit_requested = LIB_TRUE;
        lib_test_assert(softpc_sb_midi_cancel() == LIB_STATUS_OK);
        return LIB_STATUS_INVALID_STATE;
    }
    send_status = LIB_STATUS_OK;
    return wait_status;
}

int main(void)
{
    const lib_u8 note[] = {0x90, 60, 127}, sysex[] = {0xf0, 1, 0xf7};
    lib_test_assert(softpc_sb_midi_open(32000) == LIB_STATUS_INVALID_STATE);
    lib_test_assert(softpc_sb_midi_prepare() == LIB_STATUS_OK);
    lib_test_assert(softpc_sb_midi_prepare() == LIB_STATUS_OK);
    create_status = LIB_STATUS_UNSUPPORTED;
    lib_test_assert(softpc_sb_midi_open(32000) == LIB_STATUS_UNSUPPORTED && creates == 1);
    create_status = LIB_STATUS_OK;
    lib_test_assert(softpc_sb_midi_open(32000) == LIB_STATUS_OK && sets == 1);
    lib_test_assert(softpc_sb_midi_open(32000) == LIB_STATUS_INVALID_STATE && creates == 2);
    lib_test_assert(softpc_sb_midi_write(note, 3) == LIB_STATUS_OK && shorts == 1);
    stale_cancel = LIB_TRUE;
    lib_test_assert(softpc_sb_midi_write(note, 3) == LIB_STATUS_OK && shorts == 3);
    send_status = LIB_STATUS_LIMIT_EXCEEDED;
    lib_test_assert(softpc_sb_midi_write(sysex, 3) == LIB_STATUS_OK && longs == 2 && waits == 1);
    send_status = LIB_STATUS_LIMIT_EXCEEDED; inject_cancel = LIB_TRUE;
    lib_test_assert(softpc_sb_midi_write(sysex, 3) == LIB_STATUS_INVALID_STATE);
    lib_test_assert(cancels == 1 && longs == 3 && waits == 2);
    lib_test_assert(softpc_sb_midi_write(note, 3) == LIB_STATUS_INVALID_STATE && shorts == 3);
    exit_requested = inject_cancel = LIB_FALSE; send_status = LIB_STATUS_OK;
    lib_test_assert(softpc_sb_midi_write(sysex, 3) == LIB_STATUS_OK && longs == 4);
    stale_cancel = LIB_TRUE;
    lib_test_assert(softpc_sb_midi_reset() == LIB_STATUS_OK && resets == 2);
    lib_test_assert(softpc_sb_midi_gain(40000) == LIB_STATUS_OK);
    lib_test_assert(softpc_sb_midi_close() == LIB_STATUS_OK && destroys == 1);
    lib_test_assert(current_left == 12000 && current_right == 23000);
    lib_test_assert(softpc_sb_midi_cancel() == LIB_STATUS_OK && cancels == 1);

    lib_test_assert(softpc_sb_midi_open(32000) == LIB_STATUS_OK);
    current_left = 111; current_right = 222; /* Another client owns newer volume. */
    lib_test_assert(softpc_sb_midi_close() == LIB_STATUS_OK);
    lib_test_assert(current_left == 111 && current_right == 222);
    lib_test_assert(softpc_sb_midi_open(32000) == LIB_STATUS_OK);
    current_left = 333; current_right = 444;
    lib_test_assert(softpc_sb_midi_gain(16000) == LIB_STATUS_OK);
    lib_test_assert(softpc_sb_midi_close() == LIB_STATUS_OK);
    lib_test_assert(current_left == 333 && current_right == 444);
    volume_status = LIB_STATUS_UNSUPPORTED;
    lib_test_assert(softpc_sb_midi_open(32000) == LIB_STATUS_OK);
    lib_test_assert(softpc_sb_midi_write(note, 3) == LIB_STATUS_OK);
    lib_test_assert(softpc_sb_midi_gain(16000) == LIB_STATUS_UNSUPPORTED);
    destroy_status = LIB_STATUS_IO_ERROR;
    lib_test_assert(softpc_sb_midi_dispose() == LIB_STATUS_IO_ERROR);
    lib_test_assert(softpc_sb_midi_cancel() == LIB_STATUS_OK && cancels == 2);
    destroy_status = LIB_STATUS_OK;
    lib_test_assert(softpc_sb_midi_dispose() == LIB_STATUS_OK);
    lib_test_assert(softpc_sb_midi_open(32000) == LIB_STATUS_INVALID_STATE);

    lib_test_assert(softpc_sb_midi_prepare() == LIB_STATUS_OK);
    volume_status = LIB_STATUS_IO_ERROR; reset_status = LIB_STATUS_IO_ERROR;
    lib_test_assert(softpc_sb_midi_open(32000) == LIB_STATUS_IO_ERROR);
    /* A terminal Lib failure must not prevent its explicit destruction path. */
    volume_status = reset_status = LIB_STATUS_OK;
    lib_test_assert(softpc_sb_midi_open(32000) == LIB_STATUS_OK);
    send_status = LIB_STATUS_IO_ERROR;
    lib_test_assert(softpc_sb_midi_write(note, 3) == LIB_STATUS_IO_ERROR);
    send_status = LIB_STATUS_UNSUPPORTED;
    lib_test_assert(softpc_sb_midi_write(note, 3) == LIB_STATUS_IO_ERROR);
    send_status = LIB_STATUS_LIMIT_EXCEEDED; wait_status = LIB_STATUS_IO_ERROR;
    {
        lib_u32 before_waits = waits, before_longs = longs;
        lib_test_assert(softpc_sb_midi_write(sysex, 3) == LIB_STATUS_IO_ERROR);
        lib_test_assert(waits == before_waits + 1 && longs == before_longs + 1);
    }
    wait_status = LIB_STATUS_OK; send_status = LIB_STATUS_OK;
    /* Cancellation during teardown must not release the output or its guard.
     * Once the executor resumes, the same owner finishes cleanup. */
    exit_requested = LIB_TRUE; reset_status = LIB_STATUS_INVALID_STATE;
    {
        lib_u32 before_destroys = destroys, before_cancels = cancels;
        lib_test_assert(softpc_sb_midi_dispose() == LIB_STATUS_INVALID_STATE);
        lib_test_assert(destroys == before_destroys);
        lib_test_assert(softpc_sb_midi_open(32000) == LIB_STATUS_INVALID_STATE);
        lib_test_assert(softpc_sb_midi_cancel() == LIB_STATUS_OK);
        lib_test_assert(cancels == before_cancels + 1);
    }
    exit_requested = LIB_FALSE; reset_status = LIB_STATUS_OK;
    cancel_status = LIB_STATUS_IO_ERROR;
    lib_test_assert(softpc_sb_midi_cancel() == LIB_STATUS_IO_ERROR);
    lib_test_assert(softpc_sb_midi_status() == LIB_STATUS_IO_ERROR);
    cancel_status = LIB_STATUS_OK;
    lib_test_assert(softpc_sb_midi_cancel() == LIB_STATUS_OK);
    lib_test_assert(softpc_sb_midi_status() == LIB_STATUS_IO_ERROR);
    lib_test_assert(softpc_sb_midi_dispose() == LIB_STATUS_OK);
    lib_test_assert(softpc_sb_midi_status() == LIB_STATUS_OK);
    lib_test_assert(softpc_sb_midi_dispose() == LIB_STATUS_OK);
    return 0;
}
