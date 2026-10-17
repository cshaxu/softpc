#include "../../fixture_time.h"
#include "machine/machine.h"
#include "compat/audio.h"
#include "compat/devices/snapshot.h"
#include "../../fixture_cleanup.h"

#include <assert.h>
#include <stdio.h>

#include "insignia.h"
#include "host_def.h"
#include "ios.h"
#include "timer.h"
#include "quick_ev.h"
#include "lib/audio/pcm/stream.h"

#ifdef _WIN32
#include <windows.h>

extern BOOL PpiState;
extern ULONG FreqT2;
extern BOOL T2State;
extern ULONG BeepLastFreq;
extern ULONG BeepLastDuration;
/* This smoke links the real Core, Compat and Lib Audio worker.  Only the
   final platform leaf is replaced so it records actual Lib delivery for two
   complete AUDIO.COM-equivalent runs without opening a host audio device. */
struct audio_pcm_stream_platform { lib_bool live; lib_u64 submitted; };
static struct audio_pcm_stream_platform audio_probe_platform;
static volatile LONG audio_probe_phase;
static volatile LONG audio_probe_first_frame_count;
static volatile LONG audio_probe_second_frame_count;
static lib_bool audio_probe_first_positive;
static lib_bool audio_probe_first_negative;
static lib_bool audio_probe_second_positive;
static lib_bool audio_probe_second_negative;
static volatile LONG audio_probe_program_first_delivery;
static volatile LONG audio_probe_program_second_delivery;
static volatile LONG audio_probe_program_first_frames;
static volatile LONG audio_probe_program_second_frames;
static volatile LONG audio_probe_program_fast_frames;
static volatile LONG audio_probe_sb_frames;
static lib_u32 audio_probe_rate;

lib_status audio_pcm_stream_platform_create(const lib_audio_pcm_stream_options *options,
    audio_pcm_stream_platform **out_platform)
{
    assert(options != NULL && out_platform != NULL);
    assert((options->sample_rate == 48000u || options->sample_rate == 11025u) &&
        options->channel_count == 1u);
    assert(!audio_probe_platform.live);
    audio_probe_rate = options->sample_rate;
    audio_probe_platform.live = LIB_TRUE;
    audio_probe_platform.submitted = 0u;
    *out_platform = &audio_probe_platform;
    return LIB_STATUS_OK;
}

lib_status audio_pcm_stream_platform_worker_attach(audio_pcm_stream_platform *platform)
{
    assert(platform == &audio_probe_platform);
    return LIB_STATUS_OK;
}

void audio_pcm_stream_platform_worker_detach(audio_pcm_stream_platform *platform)
{ assert(platform == &audio_probe_platform); }

lib_status audio_pcm_stream_platform_enqueue(audio_pcm_stream_platform *platform,
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
    if (non_silent != LIB_FALSE && (phase == 1 || phase == 2)) {
        volatile LONG *frames = phase == 1 ? &audio_probe_first_frame_count : &audio_probe_second_frame_count;
        lib_bool *positive = phase == 1 ? &audio_probe_first_positive : &audio_probe_second_positive;
        lib_bool *negative = phase == 1 ? &audio_probe_first_negative : &audio_probe_second_negative;
        LONG remaining = 512 - InterlockedCompareExchange(frames, 0, 0);
        lib_u32 count = frame_count < (lib_u32)remaining ? frame_count : (lib_u32)remaining;
        /* Compare a fixed sample prefix, independent of worker dequeue batches. */
        for (index = 0u; index < count; ++index) {
            assert(samples[index] == 12000 || samples[index] == -12000);
            if (samples[index] > 0) *positive = LIB_TRUE;
            if (samples[index] < 0) *negative = LIB_TRUE;
        }
        if (count != 0u) (void)InterlockedAdd(frames, (LONG)count);
    }
    if (non_silent != LIB_FALSE && phase == 3) {
        /* Compare the same three complete blocks, not extra output delivered
           while the following machine reset waits for host cleanup. */
        if (InterlockedCompareExchange(&audio_probe_program_first_delivery, 0, 0) < 3)
            (void)InterlockedAdd(&audio_probe_program_first_frames, (LONG)frame_count);
        (void)InterlockedIncrement(&audio_probe_program_first_delivery);
    }
    if (non_silent != LIB_FALSE && phase == 4) {
        if (InterlockedCompareExchange(&audio_probe_program_second_delivery, 0, 0) < 3)
            (void)InterlockedAdd(&audio_probe_program_second_frames, (LONG)frame_count);
        (void)InterlockedIncrement(&audio_probe_program_second_delivery);
    }
    if (non_silent != LIB_FALSE && phase == 5)
        (void)InterlockedAdd(&audio_probe_program_fast_frames, (LONG)frame_count);
    if (phase == 6) {
        LONG offset = InterlockedCompareExchange(&audio_probe_sb_frames, 0, 0);
        assert(audio_probe_rate == 11025u && offset + frame_count <= 64u);
        for (index = 0u; index != frame_count; ++index)
            assert(samples[index] == ((lib_i32)(offset + index) * 4 - 128) * 256);
        (void)InterlockedAdd(&audio_probe_sb_frames, (LONG)frame_count);
    }
    *accepted = frame_count;
    platform->submitted += frame_count;
    return LIB_STATUS_OK;
}

lib_status audio_pcm_stream_platform_wait_writable(audio_pcm_stream_platform *platform)
{
    assert(platform == &audio_probe_platform);
    /* 1024 mono frames at 48kHz take about 21ms.  This keeps the real Lib
       worker under the same bounded four-slot backpressure it sees at WaveOut
       instead of letting a fake endpoint drain all PCM immediately. */
    softpc_test_sleep_milliseconds(20u);
    return LIB_STATUS_OK;
}

lib_status audio_pcm_stream_platform_cancel_wait(audio_pcm_stream_platform *platform)
{ assert(platform == &audio_probe_platform); return LIB_STATUS_OK; }

lib_status audio_pcm_stream_platform_clear(audio_pcm_stream_platform *platform)
{
    assert(platform == &audio_probe_platform);
    platform->submitted = 0u;
    return LIB_STATUS_OK;
}

lib_status audio_pcm_stream_platform_progress(audio_pcm_stream_platform *platform,
    lib_u64 *submitted, lib_u64 *consumed)
{
    assert(platform == &audio_probe_platform);
    *submitted = *consumed = platform->submitted;
    return LIB_STATUS_OK;
}

lib_status audio_pcm_stream_platform_wait_progress(audio_pcm_stream_platform *platform)
{
    (void)platform;
    assert(0); /* This fixture consumes each accepted block immediately. */
    return LIB_STATUS_INVALID_STATE;
}

lib_status audio_pcm_stream_platform_set_gain(audio_pcm_stream_platform *platform,
    const lib_u16 *gains, lib_u32 channel_count)
{
    assert(platform == &audio_probe_platform && audio_probe_rate == 11025u);
    assert(gains != NULL && channel_count == 1u);
    return LIB_STATUS_OK;
}

lib_status audio_pcm_stream_platform_destroy(audio_pcm_stream_platform **platform)
{
    assert(platform != NULL && *platform == &audio_probe_platform);
    audio_probe_platform.live = LIB_FALSE;
    *platform = NULL;
    return LIB_STATUS_OK;
}

static void audio_probe_wait_for_count(volatile LONG *delivered, LONG required)
{
    lib_u64 deadline = softpc_test_clock_milliseconds() + 2000u;
    while (InterlockedCompareExchange(delivered, 0, 0) < required &&
        softpc_test_clock_milliseconds() < deadline)
        softpc_test_sleep_milliseconds(1u);
    assert(InterlockedCompareExchange(delivered, 0, 0) >= required);
}

static void audio_probe_run_audio_com(void)
{
    outb(TIMER_MODE_REG, 0xb6u);
    outb(TIMER2_REG, 0x98u);
    outb(TIMER2_REG, 0x0au);
    outb(PPI_GENERAL, 0x03u);
}

static void audio_probe_sb_blocks(softpc_machine *machine)
{
    unsigned iteration, i;
    /* Real ports -> DSP -> Compat -> Lib worker, without an explicit flush or
       a later producer burst to make the first short block reach the host. */
    for (iteration = 0; iteration != 2; ++iteration) {
        lib_u64 deadline;
        assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
        for (i = 0; i != 64; ++i) {
            lib_u8 value = (lib_u8)(i * 4);
            assert(softpc_machine_write_physical(machine, 0x21000 + i,
                &value, 1) == SOFTPC_MACHINE_OK);
        }
        InterlockedExchange(&audio_probe_sb_frames, 0);
        InterlockedExchange(&audio_probe_phase, 6);
        outb(0x0c, 0); outb(2, 0); outb(2, 0x10);
        outb(3, 63); outb(3, 0); outb(0x83, 2);
        outb(0x0b, 0x49); outb(0x0a, 1);
        outb(0x22c, 0x14); outb(0x22c, 63); outb(0x22c, 0);
        deadline = softpc_test_clock_milliseconds() + 2000u;
        while (InterlockedCompareExchange(&audio_probe_sb_frames, 0, 0) < 64 &&
            softpc_test_clock_milliseconds() < deadline)
            softpc_test_sleep_milliseconds(1u);
        assert(InterlockedCompareExchange(&audio_probe_sb_frames, 0, 0) == 64);
        dispatch_q_event();
    }
}

/* This is AUDIO.COM's tone setup: program Timer 2 to 439Hz, enable the PPI
   speaker gate, then hold that state. */
static void audio_probe_execute_audio_com(softpc_machine *machine,
    lib_bool yield_host)
{
    lib_u64 deadline = softpc_test_clock_milliseconds() + 3000u;

    while (softpc_test_clock_milliseconds() < deadline) {
        assert(softpc_machine_run(machine, 5000u) == SOFTPC_MACHINE_OK);
        if (PpiState != FALSE) return;
        if (yield_host != LIB_FALSE) softpc_test_sleep_milliseconds(1u);
    }
    assert(!"AUDIO.COM did not enable its PPI speaker gate");
}

#endif

extern void host_timer2_waveform(int delay, ULONG loclocks, ULONG hiclocks,
    int lohi, int repeat);

static void make_boot_disk(const char *path)
{
    static const unsigned char program[] = {
        0xb0u,0xb6u,0xe6u,0x43u,0xb8u,0x98u,0x0au,0xe6u,
        0x42u,0x88u,0xe0u,0xe6u,0x42u,0xe4u,0x61u,0x0cu,
        0x03u,0xe6u,0x61u,0xebu,0xfeu
    };
    unsigned char sector[512] = { 0 };
    FILE *file = fopen(path, "wb");
    assert(file != NULL);
    lib_memory_copy(sector, program, sizeof(program));
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
    softpc_device_ppi_state ppi;

    make_boot_disk(path);
    /* The production VM starts Compat's host-audio sink around the recovered
       machine.  This direct machine smoke must do the same before it can
       observe the real PPI -> Compat -> Lib handoff. */
    assert(softpc_platform_audio_start() == LIB_STATUS_OK);
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_device_snapshot_capture_pit(&pit));
    softpc_device_snapshot_capture_ppi(&ppi);
    assert(ppi.register_value == 0u);
    assert(ppi.gate_2_was_low != 0u);

#ifdef _WIN32
    /* Both executions are the actual DOS fixture program, not direct calls
       to the presentation boundary. */
    InterlockedExchange(&audio_probe_phase, 3);
    audio_probe_execute_audio_com(machine, LIB_TRUE);
    audio_probe_wait_for_count(&audio_probe_program_first_delivery, 3);
    assert(InterlockedCompareExchange(&audio_probe_program_first_delivery, 0, 0) != 0);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    InterlockedExchange(&audio_probe_phase, 4);
    audio_probe_execute_audio_com(machine, LIB_TRUE);
    audio_probe_wait_for_count(&audio_probe_program_second_delivery, 3);
    assert(InterlockedCompareExchange(&audio_probe_program_second_delivery, 0, 0) != 0);
    assert(InterlockedCompareExchange(&audio_probe_program_first_frames, 0, 0) * 4 >=
        InterlockedCompareExchange(&audio_probe_program_second_frames, 0, 0) * 3);
    assert(InterlockedCompareExchange(&audio_probe_program_second_frames, 0, 0) * 4 >=
        InterlockedCompareExchange(&audio_probe_program_first_frames, 0, 0) * 3);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);

    /* The production executor runs continuous slices.  It must not need this
       test's cooperative yield for the existing Audio producer to deliver a
       complete guest tone. */
    InterlockedExchange(&audio_probe_phase, 5);
    audio_probe_execute_audio_com(machine, LIB_FALSE);
    softpc_test_sleep_milliseconds(250u);
    assert(InterlockedCompareExchange(&audio_probe_program_fast_frames, 0, 0) >= 2048);
#endif

    /* These are AUDIO.COM's PIT writes: channel 2, square wave, divisor
       0x0a98. The later PPI write is therefore its first guest-visible gate
       transition, not a test-only direct host callback. */
#ifdef _WIN32
    InterlockedExchange(&audio_probe_phase, 1);
    audio_probe_run_audio_com();
    audio_probe_wait_for_count(&audio_probe_first_frame_count, 512);
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
    audio_probe_wait_for_count(&audio_probe_second_frame_count, 512);
    outb(PPI_GENERAL, 0x00u);
    assert(PpiState == FALSE);
    assert(T2State == FALSE);
    assert(BeepLastFreq == 0u);
    assert(BeepLastDuration == 0u);
    assert(InterlockedCompareExchange(&audio_probe_first_frame_count, 0, 0) == 512);
    assert(InterlockedCompareExchange(&audio_probe_second_frame_count, 0, 0) == 512);
    assert(audio_probe_first_positive != LIB_FALSE && audio_probe_first_negative != LIB_FALSE);
    assert(audio_probe_second_positive != LIB_FALSE && audio_probe_second_negative != LIB_FALSE);
#endif

    /* Original reset.c clears the same Timer-2/PPI path. A standalone audio
       worker must therefore have no tone or guest state to retain across a
       cold reset. */
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(BeepLastFreq == 0u);
    assert(BeepLastDuration == 0u);

    assert(softpc_platform_audio_shutdown() == LIB_STATUS_OK);
#ifdef _WIN32
    audio_probe_sb_blocks(machine);
#endif
    assert(softpc_machine_destroy(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_test_remove_image(path));
    return 0;
}
