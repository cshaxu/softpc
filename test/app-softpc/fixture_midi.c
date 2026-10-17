#include "lib/audio/midi/output_interface.h"
#include "lib/types/test.h"

/* Non-MIDI tests use a synchronous memory sink, never a host device. Tests of
 * MIDI failure/backpressure retain their own scripted fixtures instead. */
struct lib_audio_midi_output { lib_u16 left, right; };
lib_status lib_audio_midi_output_create(lib_audio_midi_output **out)
{
    *out = lib_allocate_zero(1, sizeof(**out));
    lib_test_assert(*out);
    return LIB_STATUS_OK;
}
lib_status lib_audio_midi_output_destroy(lib_audio_midi_output **out)
{ lib_release(*out); *out = NULL; return LIB_STATUS_OK; }
lib_status lib_audio_midi_output_send_short(lib_audio_midi_output *out,
    const lib_u8 *bytes, lib_u32 count)
{ lib_test_assert(out && bytes && count >= 1 && count <= 3); return LIB_STATUS_OK; }
lib_status lib_audio_midi_output_send_sysex(lib_audio_midi_output *out,
    const lib_u8 *bytes, lib_u32 count)
{
    lib_test_assert(out && bytes && count >= 2 && bytes[0] == 0xf0 && bytes[count - 1] == 0xf7);
    return LIB_STATUS_OK;
}
lib_status lib_audio_midi_output_query_capacity(lib_audio_midi_output *out, lib_u32 *slots)
{ lib_test_assert(out); *slots = LIB_AUDIO_MIDI_SYSEX_CAPACITY; return LIB_STATUS_OK; }
lib_status lib_audio_midi_output_wait_writable(lib_audio_midi_output *out)
{ lib_test_assert(out); return LIB_STATUS_OK; }
lib_status lib_audio_midi_output_cancel_wait(lib_audio_midi_output *out)
{ lib_test_assert(out); return LIB_STATUS_OK; }
lib_status lib_audio_midi_output_reset(lib_audio_midi_output *out)
{ lib_test_assert(out); return LIB_STATUS_OK; }
lib_status lib_audio_midi_output_get_volume(lib_audio_midi_output *out, lib_u16 *left, lib_u16 *right)
{ lib_test_assert(out); *left = out->left; *right = out->right; return LIB_STATUS_OK; }
lib_status lib_audio_midi_output_set_volume(lib_audio_midi_output *out, lib_u16 left, lib_u16 right)
{ lib_test_assert(out); out->left = left; out->right = right; return LIB_STATUS_OK; }
