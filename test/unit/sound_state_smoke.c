#include <assert.h>
#include <windows.h>

static DWORD ticks;
static ULONG calls, frequency, duration;
static unsigned bells;
static SHORT sound_enabled;
static DWORD WINAPI clock_tick(void) { return ticks; }
ULONG GetPerfCounter(VOID) { return (ULONG)(ticks * 10u); }
static BOOL WINAPI bell(UINT type) { assert(type == MB_OK); ++bells; return TRUE; }
void softpc_standalone_audio_set_tone(ULONG hz, ULONG ms)
{ ++calls; frequency = hz; duration = ms; }

#define GetTickCount clock_tick
#define MessageBeep bell
#include "mvdm/softpc.new/host/src/nt_sound.c"
#undef MessageBeep
#undef GetTickCount

SHORT host_runtime_inquire(what)
UTINY what;
{ assert(what == C_SOUND_ON); return sound_enabled; }

int main(void)
{
    unsigned i;
    LazyBeep(440, INFINITE);
    LazyBeep(440, INFINITE);
    assert(calls == 1 && frequency == 440 && duration == INFINITE);
    LazyBeep(100, 1);
    assert(calls == 2 && BeepLastFreq == 0 && BeepLastDuration == 0);
    LazyBeep(0, 0);
    assert(calls == 2);

    ticks = 100; ET2TicCount = 99;
    PulsePpi();
    assert(PpiCounting == 1000 && FreqPpi == 0);
    for (i = 1; i <= 200; ++i) { ticks = 100 + i; PulsePpi(); }
    assert(FreqPpi == 0);
    ticks = 301; PulsePpi();
    assert(FreqPpi == 1008 && LastPpi == 1008 && PpiCounting == 3010);
    PlayContinuousTone();
    /* The original equal-tick branch expires counting; retain that behavior. */
    assert(PpiCounting == 0 && FreqPpi == 0);
    ticks = 400; ET2TicCount = 399; PulsePpi();
    ticks = 602; PlayContinuousTone();
    assert(PpiCounting == 0 && LastPpi == 0 && FreqPpi == 0);

    ticks = 700; ET2TicCount = 699;
    HostPpiState(3);
    host_timer2_waveform(0, 596, 597, 0, 1);
    assert(FreqT2 == 1000 && BeepLastFreq == 1000);
    i = calls;
    host_timer2_waveform(0, 596, 597, 0, 1);
    assert(calls == i);
    softpc_standalone_sound_timer2_gate(GATE_SIGNAL_LOW);
    assert(!T2State && BeepLastFreq == 0);
    softpc_standalone_sound_timer2_gate(GATE_SIGNAL_RISE);
    assert(T2State && BeepLastFreq == 1000);
    host_disable_timer2_sound();
    assert(!PpiState && BeepLastFreq == 0);
    ++ticks; host_enable_timer2_sound();
    assert(PpiState && BeepLastFreq == 1000);
    host_timer2_waveform(0, INFINITE, 1, 0, 0);
    assert(FreqT2 == 0);
    host_timer2_waveform(0, 0, 0, 0, 1);
    assert(FreqT2 == 0);
    host_timer2_waveform(0, 100000, 100000, 0, 1);
    assert(FreqT2 == 0);
    host_timer2_waveform(0, 596, 597, 0, 1);
    InitSound(FALSE);
    assert(BeepLastFreq == 0 && BeepLastDuration == 0);
    i = calls; InitSound(TRUE); assert(calls == i);
    sound_enabled = 0; host_ring_bell(1); assert(bells == 0);
    sound_enabled = 1; host_ring_bell(1); assert(bells == 1);
    host_alarm(1); assert(bells == 2);
    return 0;
}
