#include "lib/audio/stream.h"
#include "lib/types/win32/audio.h"

#include <assert.h>

static lib_win32_mmresult open_result = LIB_WIN32_MMSYSERR_NOERROR;
static lib_win32_mmresult prepare_result = LIB_WIN32_MMSYSERR_NOERROR;
static lib_win32_mmresult write_result = LIB_WIN32_MMSYSERR_NOERROR;
static lib_win32_mmresult reset_result = LIB_WIN32_MMSYSERR_NOERROR;
static lib_win32_mmresult unprepare_result = LIB_WIN32_MMSYSERR_NOERROR;
static lib_win32_mmresult close_result = LIB_WIN32_MMSYSERR_NOERROR;
static unsigned open_calls, prepare_calls, write_calls, reset_calls;
static unsigned unprepare_calls, close_calls;

static lib_win32_mmresult fake_open(lib_win32_wave_output *out_output,
    lib_win32_uint mapper, const lib_win32_wave_format *format,
    lib_win32_dword_ptr callback, lib_win32_dword_ptr instance,
    lib_win32_dword flags)
{
    ++open_calls;
    assert(out_output != LIB_NULL && mapper == LIB_WIN32_WAVE_MAPPER);
    assert(format != LIB_NULL && callback == 0u && instance == 0u);
    assert(flags == LIB_WIN32_CALLBACK_NULL);
    *out_output = open_result == LIB_WIN32_MMSYSERR_NOERROR ?
        (lib_win32_wave_output)(lib_iptr)1 : LIB_NULL;
    return open_result;
}

static lib_win32_mmresult fake_prepare(lib_win32_wave_output output,
    lib_win32_wave_header *header, lib_win32_uint header_size)
{
    ++prepare_calls;
    assert(output != LIB_NULL && header != LIB_NULL && header_size == sizeof(*header));
    return prepare_result;
}

static lib_win32_mmresult fake_write(lib_win32_wave_output output,
    lib_win32_wave_header *header, lib_win32_uint header_size)
{
    ++write_calls;
    assert(output != LIB_NULL && header != LIB_NULL && header_size == sizeof(*header));
    return write_result;
}

static lib_win32_mmresult fake_reset(lib_win32_wave_output output)
{
    ++reset_calls;
    assert(output != LIB_NULL);
    return reset_result;
}

static lib_win32_mmresult fake_unprepare(lib_win32_wave_output output,
    lib_win32_wave_header *header, lib_win32_uint header_size)
{
    ++unprepare_calls;
    assert(output != LIB_NULL && header != LIB_NULL && header_size == sizeof(*header));
    return unprepare_result;
}

static lib_win32_mmresult fake_close(lib_win32_wave_output output)
{
    ++close_calls;
    assert(output != LIB_NULL);
    return close_result;
}

#undef lib_win32_wave_out_open
#undef lib_win32_wave_out_prepare_header
#undef lib_win32_wave_out_write
#undef lib_win32_wave_out_reset
#undef lib_win32_wave_out_unprepare_header
#undef lib_win32_wave_out_close
#define lib_win32_wave_out_open fake_open
#define lib_win32_wave_out_prepare_header fake_prepare
#define lib_win32_wave_out_write fake_write
#define lib_win32_wave_out_reset fake_reset
#define lib_win32_wave_out_unprepare_header fake_unprepare
#define lib_win32_wave_out_close fake_close
#include "lib/audio/win32/stream.c"

static void reset_fake(void)
{
    open_result = LIB_WIN32_MMSYSERR_NOERROR;
    prepare_result = LIB_WIN32_MMSYSERR_NOERROR;
    write_result = LIB_WIN32_MMSYSERR_NOERROR;
    reset_result = LIB_WIN32_MMSYSERR_NOERROR;
    unprepare_result = LIB_WIN32_MMSYSERR_NOERROR;
    close_result = LIB_WIN32_MMSYSERR_NOERROR;
    open_calls = prepare_calls = write_calls = reset_calls = 0u;
    unprepare_calls = close_calls = 0u;
}

int main(void)
{
    lib_audio_stream_options options = { 48000u, 2u };
    audio_stream_platform *platform = LIB_NULL;
    lib_i16 samples[2] = { 1, -1 };
    lib_u32 accepted = 0u;
    lib_u32 queued = 0u;
    lib_u32 writable = 0u;

    reset_fake();
    assert(audio_stream_platform_create(&options, &platform) == LIB_STATUS_OK);
    assert(open_calls == 1u && prepare_calls == 4u);
    assert(audio_stream_platform_enqueue(platform, samples, 1u, &accepted) == LIB_STATUS_OK);
    assert(accepted == 1u && write_calls == 1u);
    assert(audio_stream_platform_query(platform, &queued, &writable) == LIB_STATUS_OK);
    assert(queued == 1u && writable == 1536u);
    reset_result = 1u;
    assert(audio_stream_platform_clear(platform) == LIB_STATUS_IO_ERROR);
    reset_result = LIB_WIN32_MMSYSERR_NOERROR;
    assert(audio_stream_platform_clear(platform) == LIB_STATUS_OK);
    unprepare_result = 1u;
    assert(audio_stream_platform_destroy(&platform) == LIB_STATUS_IO_ERROR && platform != LIB_NULL);
    unprepare_result = LIB_WIN32_MMSYSERR_NOERROR;
    close_result = 1u;
    assert(audio_stream_platform_destroy(&platform) == LIB_STATUS_IO_ERROR && platform != LIB_NULL);
    close_result = LIB_WIN32_MMSYSERR_NOERROR;
    assert(audio_stream_platform_destroy(&platform) == LIB_STATUS_OK && platform == LIB_NULL);

    reset_fake();
    open_result = 1u;
    assert(audio_stream_platform_create(&options, &platform) == LIB_STATUS_IO_ERROR);
    assert(platform == LIB_NULL && open_calls == 1u);
    reset_fake();
    prepare_result = 1u;
    assert(audio_stream_platform_create(&options, &platform) == LIB_STATUS_IO_ERROR);
    assert(platform == LIB_NULL && reset_calls == 1u && close_calls == 1u);
    reset_fake();
    assert(audio_stream_platform_create(&options, &platform) == LIB_STATUS_OK);
    write_result = 1u;
    assert(audio_stream_platform_enqueue(platform, samples, 1u, &accepted) == LIB_STATUS_IO_ERROR);
    assert(accepted == 0u);
    assert(audio_stream_platform_destroy(&platform) == LIB_STATUS_OK && platform == LIB_NULL);
    return 0;
}
