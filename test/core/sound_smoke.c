#include "core/machine/machine.h"
#include "core/compat/audio.h"
#include "core/compat/devices/snapshot.h"
#include "../lib/cleanup.h"

#include <assert.h>
#include <stdio.h>

#include "insignia.h"
#include "host_def.h"
#include "ios.h"
#include "timer.h"
#include "lib/audio/stream.h"

#ifdef _WIN32
#include <windows.h>

/* This smoke links the real Core, Compat and Lib Audio worker.  Only the
   final platform leaf is replaced so it records actual Lib delivery for two
   complete AUDIO.COM-equivalent runs without opening a host audio device. */
struct audio_stream_platform { lib_bool live; };
static struct audio_stream_platform audio_probe_platform;
static volatile LONG audio_probe_phase;
static volatile LONG audio_probe_first_delivery;
static volatile LONG audio_probe_second_delivery;
static lib_u32 audio_probe_first_frame_count;
static lib_u32 audio_probe_second_frame_count;
static lib_bool audio_probe_first_positive;
static lib_bool audio_probe_first_negative;
static lib_bool audio_probe_second_positive;
static lib_bool audio_probe_second_negative;

lib_status audio_stream_platform_create(const lib_audio_stream_options *options,
    audio_stream_platform **out_platform)
{
    assert(options != NULL && out_platform != NULL);
    assert(options->sample_rate == 48000u && options->channel_count == 1u);
    audio_probe_platform.live = LIB_TRUE;
    *out_platform = &audio_probe_platform;
    return LIB_STATUS_OK;
}

lib_status audio_stream_platform_enqueue(audio_stream_platform *platform,
    const lib_i16 *samples, lib_u32 frame_count, lib_u32 *accepted)
{
    lib_u32 index;
    lib_bool non_silent = LIB_FALSE;
    LONG phase;

    assert(platform == &audio_probe_platform && platform->live != LIB_FALSE);
    assert(samples != NULL && accepted != NULL);
    for (index = 0u; index < frame_count; ++index)
        if (samples[index] != 0) non_silent = LIB_TRUE;
    phase = InterlockedCompareExchange(&audio_probe_phase, 0, 0);
    if (non_silent != LIB_FALSE && phase == 1) {
        if (InterlockedCompareExchange(&audio_probe_first_delivery, 0, 0) == 0) {
            audio_probe_first_frame_count = frame_count;
            for (index = 0u; index < frame_count; ++index) {
                assert(samples[index] == 12000 || samples[index] == -12000);
                if (samples[index] > 0) audio_probe_first_positive = LIB_TRUE;
                if (samples[index] < 0) audio_probe_first_negative = LIB_TRUE;
            }
        }
        (void)InterlockedIncrement(&audio_probe_first_delivery);
    }
    if (non_silent != LIB_FALSE && phase == 2) {
        if (InterlockedCompareExchange(&audio_probe_second_delivery, 0, 0) == 0) {
            audio_probe_second_frame_count = frame_count;
            for (index = 0u; index < frame_count; ++index) {
                assert(samples[index] == 12000 || samples[index] == -12000);
                if (samples[index] > 0) audio_probe_second_positive = LIB_TRUE;
                if (samples[index] < 0) audio_probe_second_negative = LIB_TRUE;
            }
        }
        (void)InterlockedIncrement(&audio_probe_second_delivery);
    }
    *accepted = frame_count;
    return LIB_STATUS_OK;
}

lib_status audio_stream_platform_wait_writable(audio_stream_platform *platform)
{
    assert(platform == &audio_probe_platform);
    /* 1024 mono frames at 48kHz take about 21ms.  This keeps the real Lib
       worker under the same bounded four-slot backpressure it sees at WaveOut
       instead of letting a fake endpoint drain all PCM immediately. */
    Sleep(20u);
    return LIB_STATUS_OK;
}

lib_status audio_stream_platform_cancel_wait(audio_stream_platform *platform)
{ assert(platform == &audio_probe_platform); return LIB_STATUS_OK; }

lib_status audio_stream_platform_clear(audio_stream_platform *platform)
{ assert(platform == &audio_probe_platform); return LIB_STATUS_OK; }

lib_status audio_stream_platform_destroy(audio_stream_platform **platform)
{
    assert(platform != NULL && *platform == &audio_probe_platform);
    audio_probe_platform.live = LIB_FALSE;
    *platform = NULL;
    return LIB_STATUS_OK;
}

static void audio_probe_wait_for_blocks(volatile LONG *delivered)
{
    DWORD deadline = GetTickCount() + 2000u;
    while (InterlockedCompareExchange(delivered, 0, 0) < 3 &&
        (LONG)(GetTickCount() - deadline) < 0)
        Sleep(1u);
    assert(InterlockedCompareExchange(delivered, 0, 0) >= 3);
}

static void audio_probe_run_audio_com(void)
{
    outb(TIMER_MODE_REG, 0xb6u);
    outb(TIMER2_REG, 0x98u);
    outb(TIMER2_REG, 0x0au);
    outb(PPI_GENERAL, 0x03u);
}
#endif

extern ULONG FreqT2;
extern BOOL PpiState;
extern BOOL T2State;
extern ULONG BeepLastFreq;
extern ULONG BeepLastDuration;
extern void host_timer2_waveform(int delay, ULONG loclocks, ULONG hiclocks,
    int lohi, int repeat);

static void make_boot_disk(const char *path)
{
    unsigned char sector[512] = { 0 };
    FILE *file = fopen(path, "wb");
    assert(file != NULL);
    sector[510] = 0x55u;
    sector[511] = 0xaau;
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fseek(file, 1474560L - 1L, SEEK_SET) == 0);
    assert(fputc(0, file) == 0);
    assert(fclose(file) == 0);
}

int main(void)
{
    const char *path = "softpc-sound-smoke.img";
    softpc_machine_options options = { path, NULL };
    softpc_machine *machine = NULL;
    softpc_device_pit_state pit;

    make_boot_disk(path);
    /* The production VM starts Compat's host-audio sink around the recovered
       machine.  This direct machine smoke must do the same before it can
       observe the real PPI -> Compat -> Lib handoff. */
    assert(softpc_platform_audio_start() == LIB_STATUS_OK);
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_device_snapshot_capture_pit(&pit));
    assert(pit.counter[2].gate == GATE_SIGNAL_LOW);

    /* These are AUDIO.COM's PIT writes: channel 2, square wave, divisor
       0x0a98. The later PPI write is therefore its first guest-visible gate
       transition, not a test-only direct host callback. */
#ifdef _WIN32
    InterlockedExchange(&audio_probe_phase, 1);
    audio_probe_run_audio_com();
    audio_probe_wait_for_blocks(&audio_probe_first_delivery);
#else
    outb(TIMER_MODE_REG, 0xb6u);
    outb(TIMER2_REG, 0x98u);
    outb(TIMER2_REG, 0x0au);
    outb(PPI_GENERAL, 0x03u);
#endif
    assert(softpc_device_snapshot_capture_pit(&pit));
    assert(pit.counter[2].gate == GATE_SIGNAL_RISE);
    assert(PpiState == TRUE);
    assert(T2State == TRUE);
    assert(FreqT2 > 10u && FreqT2 < 20000u);
    assert(BeepLastFreq == FreqT2);
    assert(BeepLastDuration == INFINITE);

    /* A second original PIT waveform changes only the requested playback
       tone; the host worker remains a presentation sink and never feeds a
       synthesized timing event back into the guest. */
    host_timer2_waveform(0, 298u, 299u, 0, 1);
    assert(FreqT2 > 10u && FreqT2 < 20000u);
    assert(BeepLastFreq == FreqT2);
    assert(BeepLastDuration == INFINITE);

    /* Clearing both original PPI bits stops the same state machine. */
    outb(PPI_GENERAL, 0x00u);
    assert(PpiState == FALSE);
    assert(T2State == FALSE);
    assert(BeepLastFreq == 0u);
    assert(BeepLastDuration == 0u);

#ifdef _WIN32
    /* A second guest invocation must reach the same Lib delivery boundary.
       Repeat the full original port sequence rather than calling the Compat
       sink directly. */
    InterlockedExchange(&audio_probe_phase, 2);
    audio_probe_run_audio_com();
    audio_probe_wait_for_blocks(&audio_probe_second_delivery);
    outb(PPI_GENERAL, 0x00u);
    assert(PpiState == FALSE);
    assert(T2State == FALSE);
    assert(BeepLastFreq == 0u);
    assert(BeepLastDuration == 0u);
    assert(audio_probe_first_frame_count == audio_probe_second_frame_count);
    assert(audio_probe_first_positive != LIB_FALSE && audio_probe_first_negative != LIB_FALSE);
    assert(audio_probe_second_positive != LIB_FALSE && audio_probe_second_negative != LIB_FALSE);
#endif

    /* Original reset.c clears the same Timer-2/PPI path. A standalone audio
       worker must therefore have no tone or guest state to retain across a
       cold reset. */
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(BeepLastFreq == 0u);
    assert(BeepLastDuration == 0u);

    softpc_machine_destroy(machine);
    softpc_platform_audio_shutdown();
    assert(softpc_test_remove_image(path));
    return 0;
}
