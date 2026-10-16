#include "sb_midi.h"
#include "ccpu/lifecycle.h"
#include "lib/base/sync_interface.h"
#include "lib/types/file.h"

void softpc_sb_midi_report_oversize(void)
{
    lib_c_fprintf(lib_c_stderr, "SoftPC MIDI: discarded SysEx larger than %u bytes\n",
        (unsigned)LIB_AUDIO_MIDI_SYSEX_MAX_BYTES);
}

/* The guard protects publication/destruction against cancellation only.
 * Host waits run outside it. Guest state remains with nt_sbmid.c. */
static base_sync_mutex *softpc_sb_midi_guard;
static lib_audio_midi_output *softpc_sb_midi_output;
static lib_status softpc_sb_midi_cancel_failure;
static lib_u16 softpc_sb_midi_original_left, softpc_sb_midi_original_right;
static lib_u16 softpc_sb_midi_last_gain;
static lib_bool softpc_sb_midi_restore_volume;
static lib_bool softpc_sb_midi_changed_volume;

lib_status softpc_sb_midi_prepare(void)
{
    if (softpc_sb_midi_guard != LIB_NULL) return LIB_STATUS_OK;
    return base_sync_mutex_create(&softpc_sb_midi_guard);
}

lib_status softpc_sb_midi_gain(lib_u16 gain)
{
    lib_status status;
    if (softpc_sb_midi_output == LIB_NULL) return LIB_STATUS_INVALID_STATE;
    if (softpc_sb_midi_restore_volume && softpc_sb_midi_changed_volume) {
        lib_u16 left, right;
        status = lib_audio_midi_output_get_volume(softpc_sb_midi_output, &left, &right);
        if (status != LIB_STATUS_OK && status != LIB_STATUS_UNSUPPORTED) return status;
        if (status == LIB_STATUS_OK && (left != softpc_sb_midi_last_gain ||
            right != softpc_sb_midi_last_gain)) {
            softpc_sb_midi_original_left = left;
            softpc_sb_midi_original_right = right;
        }
    }
    status = lib_audio_midi_output_set_volume(softpc_sb_midi_output, gain, gain);
    if (status == LIB_STATUS_OK) {
        softpc_sb_midi_last_gain = gain;
        softpc_sb_midi_changed_volume = LIB_TRUE;
    }
    return status;
}

lib_status softpc_sb_midi_open(lib_u16 gain)
{
    lib_audio_midi_output *output = LIB_NULL;
    lib_status status;
    if (softpc_sb_midi_guard == LIB_NULL || softpc_sb_midi_output != LIB_NULL)
        return LIB_STATUS_INVALID_STATE;
    status = lib_audio_midi_output_create(&output);
    if (status != LIB_STATUS_OK) return status;
    base_sync_mutex_lock(softpc_sb_midi_guard);
    softpc_sb_midi_output = output;
    softpc_sb_midi_cancel_failure = LIB_STATUS_OK;
    base_sync_mutex_unlock(softpc_sb_midi_guard);
    softpc_sb_midi_changed_volume = LIB_FALSE;
    status = lib_audio_midi_output_get_volume(output,
        &softpc_sb_midi_original_left, &softpc_sb_midi_original_right);
    softpc_sb_midi_restore_volume = status == LIB_STATUS_OK;
    if (status == LIB_STATUS_UNSUPPORTED) status = LIB_STATUS_OK;
    if (status == LIB_STATUS_OK) {
        status = softpc_sb_midi_gain(gain);
        if (status == LIB_STATUS_UNSUPPORTED) status = LIB_STATUS_OK;
    }
    if (status != LIB_STATUS_OK) {
        lib_status cleanup = softpc_sb_midi_close();
        if (cleanup != LIB_STATUS_OK) return cleanup;
    }
    return status;
}

lib_status softpc_sb_midi_cancel(void)
{
    lib_status status = LIB_STATUS_OK;
    /* The owner must quiesce cancellation before disposing the guard. */
    if (softpc_sb_midi_guard == LIB_NULL) return LIB_STATUS_OK;
    base_sync_mutex_lock(softpc_sb_midi_guard);
    if (softpc_sb_midi_output != LIB_NULL)
        status = lib_audio_midi_output_cancel_wait(softpc_sb_midi_output);
    if (status != LIB_STATUS_OK && softpc_sb_midi_cancel_failure == LIB_STATUS_OK)
        softpc_sb_midi_cancel_failure = status;
    base_sync_mutex_unlock(softpc_sb_midi_guard);
    return status;
}

lib_status softpc_sb_midi_status(void)
{
    lib_status status;
    if (softpc_sb_midi_guard == LIB_NULL) return LIB_STATUS_OK;
    base_sync_mutex_lock(softpc_sb_midi_guard);
    status = softpc_sb_midi_cancel_failure;
    base_sync_mutex_unlock(softpc_sb_midi_guard);
    return status;
}

lib_status softpc_sb_midi_write(const lib_u8 *bytes, lib_u32 count)
{
    if (bytes == LIB_NULL || count == 0u) return LIB_STATUS_INVALID_ARGUMENT;
    if (softpc_sb_midi_output == LIB_NULL) return LIB_STATUS_INVALID_STATE;
    for (;;) {
        lib_status status;
        if (softpc_ccpu_lifecycle_exit_requested()) return LIB_STATUS_INVALID_STATE;
        if (bytes[0] == 0xf0) {
            status = lib_audio_midi_output_send_sysex(softpc_sb_midi_output, bytes, count);
            if (status == LIB_STATUS_LIMIT_EXCEEDED) {
                status = lib_audio_midi_output_wait_writable(softpc_sb_midi_output);
                if (status == LIB_STATUS_OK) continue;
            }
        } else {
            status = lib_audio_midi_output_send_short(softpc_sb_midi_output, bytes, count);
        }
        /* An auto-reset cancellation signal may survive the previous run.
         * The existing CCPU exit request, not another flag, is authoritative. */
        if (status == LIB_STATUS_INVALID_STATE && !softpc_ccpu_lifecycle_exit_requested())
            continue;
        /* Capability admission already succeeded. Rejection of a valid
         * complete message is a host failure, not an oversized guest SysEx. */
        if (status == LIB_STATUS_UNSUPPORTED) return LIB_STATUS_IO_ERROR;
        return status;
    }
}

lib_status softpc_sb_midi_reset(void)
{
    lib_status status;
    if (softpc_sb_midi_output == LIB_NULL) return LIB_STATUS_OK;
    do {
        status = lib_audio_midi_output_reset(softpc_sb_midi_output);
    } while (status == LIB_STATUS_INVALID_STATE && !softpc_ccpu_lifecycle_exit_requested());
    return status;
}

lib_status softpc_sb_midi_close(void)
{
    lib_status status, reset_status, volume_status = LIB_STATUS_OK;
    if (softpc_sb_midi_output == LIB_NULL) return LIB_STATUS_OK;
    /* Drain the reset/EOX handshake while cancellation can still reach it.
     * A successful reset leaves no outstanding native header for destroy.
     * Terminal faults still need Lib's explicit destruction/cleanup path. */
    reset_status = softpc_sb_midi_reset();
    if (reset_status == LIB_STATUS_INVALID_STATE) return reset_status;
    if (reset_status == LIB_STATUS_OK && softpc_sb_midi_restore_volume &&
        softpc_sb_midi_changed_volume) {
        lib_u16 left, right;
        volume_status = lib_audio_midi_output_get_volume(softpc_sb_midi_output, &left, &right);
        if (volume_status == LIB_STATUS_OK && left == softpc_sb_midi_last_gain &&
            right == softpc_sb_midi_last_gain)
            volume_status = lib_audio_midi_output_set_volume(softpc_sb_midi_output,
                softpc_sb_midi_original_left, softpc_sb_midi_original_right);
        if (volume_status == LIB_STATUS_UNSUPPORTED) volume_status = LIB_STATUS_OK;
    }
    base_sync_mutex_lock(softpc_sb_midi_guard);
    status = lib_audio_midi_output_destroy(&softpc_sb_midi_output);
    base_sync_mutex_unlock(softpc_sb_midi_guard);
    if (status != LIB_STATUS_OK) return status;
    softpc_sb_midi_restore_volume = softpc_sb_midi_changed_volume = LIB_FALSE;
    return reset_status != LIB_STATUS_OK ? reset_status : volume_status;
}

lib_status softpc_sb_midi_dispose(void)
{
    lib_status status = softpc_sb_midi_close();
    if (status != LIB_STATUS_OK) return status;
    base_sync_mutex_destroy(softpc_sb_midi_guard);
    softpc_sb_midi_guard = LIB_NULL;
    softpc_sb_midi_cancel_failure = LIB_STATUS_OK;
    return LIB_STATUS_OK;
}
