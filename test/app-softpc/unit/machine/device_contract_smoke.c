#include "lib/types/types_interface.h"
#include "compat/sb_pcm.h"
#include "compat/platform.h"
#include "machine/machine.h"
#include "../../fixture_cleanup.h"
#include "insignia.h"
#include "host_def.h"
#include "xt.h"
#include "ios.h"
#include "dma.h"
#include "sndblst.h"
#include "nt_sb.h"
#include "bios.h"
#include "cmos.h"
#include "sas.h"
#define HOOKED_IRETS
#include "ica.h"
#include "compat/eoi.h"
#include "quick_ev.h"

#undef const
#include <assert.h>
#include <stdio.h>

extern void disk_io(void);
extern void c_setAX(IU16 value);
extern void c_setCX(IU16 value);
extern void c_setDX(IU16 value);
extern void c_setCF(IBOOL value);
extern IU16 c_getAX(void);
extern IU16 c_getCX(void);
extern IU16 c_getDX(void);
extern ISM32 c_getCF(void);
extern ISM32 c_setSS(IU16 value);
extern ISM32 c_setDS(IU16 value);
extern void c_setSP(IU16 value);
extern void search_for_roms(void);
extern IU8 *c_GetPhyAdd(IU32 address);
extern ISM32 c_setES(IU16 value);
extern void c_setSI(IU16 value);
extern ISM32 c_getZF(void);
extern ISM32 c_getIF(void);
extern void retrieve_descr_fields(half_word *ar, sys_addr *base,
    sys_addr *limit, sys_addr descriptor_address);

static int eoi_calls, eoi_irq, eoi_count;
static void record_eoi(int irq, int count)
{ ++eoi_calls; eoi_irq = irq; eoi_count = count; }
static void wrong_eoi(int irq, int count)
{ (void)irq; (void)count; assert(0); }

static void verify_eoi(void)
{
    unsigned automatic, specific;
    assert(!RegisterEOIHook(-1, record_eoi));
    assert(!RegisterEOIHook(16, record_eoi));
    assert(!RegisterEOIHook(5, NULL));
    assert(!RemoveEOIHook(-1, record_eoi));
    assert(!RemoveEOIHook(16, record_eoi));
    assert(!RemoveEOIHook(5, NULL));
    assert(RegisterEOIHook(5, record_eoi));
    assert(!RegisterEOIHook(5, wrong_eoi));
    assert(!RemoveEOIHook(5, wrong_eoi));
    host_EOI_hook(-1, 0);
    host_EOI_hook(16, 0);
    assert(eoi_calls == 0);
    for (automatic = 0; automatic != 2; ++automatic) {
        for (specific = 0; specific != 2; ++specific) {
            int before = eoi_calls;
            IU32 hook = 0;
            /* Program the real PIC through its ports, then acknowledge IRQ5. */
            ica_outb(0x20, 0x11);
            ica_outb(0x21, 0x08);
            ica_outb(0x21, 0x04);
            ica_outb(0x21, automatic ? 0x03 : 0x01);
            ica_outb(0x21, 0xdf);
            ica_hw_interrupt(ICA_MASTER, 5, 1);
            assert(ica_intack(&hook) == 0x0d);
            assert(eoi_calls == before + (int)automatic);
            ica_outb(0x20, specific ? 0x65 : 0x20);
            assert(eoi_calls == before + 1 && eoi_irq == 5 && eoi_count == 0);
            ica_outb(0x20, 0x65); /* A second EOI has no in-service owner. */
            assert(eoi_calls == before + 1);
        }
    }
    ica_outb(0x21, 0xff);
    ica_hw_interrupt(ICA_MASTER, 5, 1);
    ica_clear_int(ICA_MASTER, 5);
    assert(eoi_calls == 5 && eoi_count == -1);
    assert(RemoveEOIHook(5, record_eoi));
    assert(!RemoveEOIHook(5, record_eoi));
    ica_hw_interrupt(ICA_MASTER, 5, 1);
    ica_clear_int(ICA_MASTER, 5);
    assert(eoi_calls == 5); /* Teardown removes the only dispatch authority. */
}

static void verify_delayed_irq(void)
{
    unsigned adapter;
    assert(!host_DelayHwInterrupt(-1, 1, 1000));
    assert(!host_DelayHwInterrupt(16, 1, 1000));
    assert(!host_DelayHwInterrupt(5, -1, 1000));
    for (adapter = 0; adapter != 2; ++adapter) {
        int irq = (int)(adapter * 8 + 5);
        io_addr port = adapter ? 0xa0 : 0x20;
        IU32 hook = 0;
        IU8 value;
        ica0_post();
        ica1_post();
        ica_outb(0x21, adapter ? 0xfb : 0xdf);
        ica_outb(0xa1, adapter ? 0xdf : 0xff);
        assert(host_DelayHwInterrupt(irq, 1, 1000));
        ica_outb(port, 0x0a); /* IRR records the request before delivery. */
        ica_inb(port, &value);
        assert(value & 0x20);
        assert(!(ica_scan_irr(adapter) & 0x80));
        assert(host_DelayHwInterrupt(irq, 1, 5000)); /* Accumulate, do not reschedule. */
        dispatch_q_event();
        assert(!(DelayIrqLine & (1ul << irq)));
        assert(ica_scan_irr(adapter) & 0x80);
        assert(ica_intack(&hook) == (adapter ? 0x75 : 0x0d));
        ica_outb(port, 0x65);
        if (adapter) ica_outb(0x20, 0x62);
        assert(ica_intack(&hook) == (adapter ? 0x75 : 0x0d));
        ica_outb(port, 0x65);
        if (adapter) ica_outb(0x20, 0x62);
        assert(!(ica_scan_irr(adapter) & 0x80));
        /* Cancellation removes the scheduled callback, not the PIC request. */
        assert(host_DelayHwInterrupt(irq, 1, 1000));
        assert(host_DelayHwInterrupt(irq, 0, 0xfffffffful));
        assert(!(DelayIrqLine & (1ul << irq)));
        ica_outb(port, 0x0a);
        ica_inb(port, &value);
        assert(value & 0x20);
        ica_clear_int(adapter, 5);
        dispatch_q_event();
        assert(!(ica_scan_irr(adapter) & 0x80));
        assert(host_DelayHwInterrupt(irq, 0, 0xfffffffful));
    }
}

static void verify_descriptor(softpc_machine *machine)
{
    static const struct {
        unsigned char bytes[8];
        sys_addr base, limit;
    } cases[] = {
        { { 0xff, 0xff, 0, 0, 0x10, 0x92, 0, 0 }, 0x100000, 0xffff },
        { { 0x45, 0x23, 0, 0, 0x10, 0x92, 1, 0 }, 0x100000, 0x12345 },
        { { 0xff, 0xff, 0, 0, 0, 0x92, 0x8f, 0 }, 0, 0xffffffff }
    };
    unsigned index, wrapping;
    for (index = 0; index != sizeof(cases) / sizeof(cases[0]); ++index) {
        struct { sys_addr before, value, after; } limit = {
            0x13572468, 0xa5a5a5a5, 0x24681357
        };
        sys_addr base = 0;
        half_word ar = 0;
        assert(softpc_machine_write_physical(machine, 0x600,
            cases[index].bytes, 8) == SOFTPC_MACHINE_OK);
        retrieve_descr_fields(&ar, &base, &limit.value, 0x600);
        assert(ar == 0x92 && base == cases[index].base);
        assert(limit.value == cases[index].limit);
        assert(limit.before == 0x13572468 && limit.after == 0x24681357);
    }
    for (wrapping = 0; wrapping != 2; ++wrapping) {
        unsigned char table[32] = { 0 }, source[32], result[32];
        lib_memory_copy(table + 16, cases[0].bytes, 8);
        lib_memory_copy(table + 24, cases[0].bytes, 8);
        table[27] = 0x10; /* destination base 0x101000 */
        for (index = 0; index != sizeof(source); ++index)
            source[index] = (unsigned char)(index + 0x61);
        sas_disable_20_bit_wrapping();
        assert(softpc_machine_write_physical(machine, 0x600, table, 32) ==
            SOFTPC_MACHINE_OK);
        assert(softpc_machine_write_physical(machine, 0x100000, source, 32) ==
            SOFTPC_MACHINE_OK);
        lib_memory_set(result, 0, sizeof(result));
        assert(softpc_machine_write_physical(machine, 0x101000, result, 32) ==
            SOFTPC_MACHINE_OK);
        if (wrapping) sas_enable_20_bit_wrapping();
        c_setES(0);
        c_setSI(0x600);
        c_setAX(0x8700);
        c_setCX(16);
        c_setCF(1);
        cassette_io();
        assert(c_getAX() == 0 && c_getCF() == 0);
        assert(c_getZF() == 1 && c_getIF() == 1);
        assert(!!sas_twenty_bit_wrapping_enabled() == (int)wrapping);
        sas_disable_20_bit_wrapping();
        assert(softpc_machine_read_physical(machine, 0x101000, result, 32) ==
            SOFTPC_MACHINE_OK);
        assert(lib_memory_compare(source, result, 32) == 0);
    }
}

static void verify_cmos(void)
{
    half_word low, high, value;
    unsigned corrupt;
    /* The fixture has more than 256 KiB of extended memory: comparing a
       byte with the unmasked full KiB value must not be the contract. */
    assert((sas_memory_size() - 0x100000) / 1024 > 255);
    cmos_post();
    assert(cmos_read_byte(CMOS_E_M_S_LO, &low) == 0);
    assert(cmos_read_byte(CMOS_E_M_S_HI, &high) == 0);
    assert((unsigned)low + 256u * high ==
        (sas_memory_size() - 0x100000) / 1024);
    for (corrupt = 0; corrupt != 4; ++corrupt) {
        assert(cmos_write_byte(CMOS_E_M_S_LO,
            (half_word)(low ^ ((corrupt & 1) ? 0x5a : 0))) == 0);
        assert(cmos_write_byte(CMOS_E_M_S_HI,
            (half_word)(high ^ ((corrupt & 2) ? 0xa5 : 0))) == 0);
        cmos_post();
        assert(cmos_read_byte(CMOS_E_M_S_LO, &value) == 0 && value == low);
        assert(cmos_read_byte(CMOS_E_M_S_HI, &value) == 0 && value == high);
    }
}

static void dma_word(io_addr port, IU16 value)
{
    outb(port, (half_word)value);
    outb(port, (half_word)(value >> 8));
}

/* Until the DSP port owner is attached, this fixture supplies its channel. */
extern IU32 TimeConstant;
extern IU32 SBBlockSize;
extern IBOOL bHighSpeedMode, bExitAuto, bDspPaused;
extern IBOOL bDspActive, SbAnswerDMAPosition, bWriteBurstStarted;
extern IU32 PreviousWaveOutPos, DspNumberDmaQueries, DspVirtualInc;
extern IU16 StartingDmaAddr, StartingDmaCount;
extern SB_DMA_INFO DmaInfo;
extern DSP_MODE DspMode;
static lib_u8 sb_pcm_bytes[65536];
static lib_u32 sb_pcm_count, sb_pcm_budget;
static lib_status sb_pcm_status = LIB_STATUS_OK;
static lib_u64 sb_pcm_epoch = 1, sb_pcm_consumed;
static lib_status sb_progress_status = LIB_STATUS_OK;
static lib_u32 sb_output_rate, sb_gain_calls, sb_open_calls;
static lib_u16 sb_output_gain;
static lib_status sb_gain_status = LIB_STATUS_OK;
static lib_status sb_open_status = LIB_STATUS_OK, sb_close_status = LIB_STATUS_OK;
static lib_status sb_active_status = LIB_STATUS_OK;
static lib_bool sb_output_active = LIB_TRUE;
lib_status softpc_sb_pcm_open(lib_u32 rate, lib_u16 gain)
{
    assert(sb_output_rate == 0);
    ++sb_open_calls;
    if (sb_open_status != LIB_STATUS_OK) return sb_open_status;
    sb_output_rate = rate; sb_output_gain = gain; sb_output_active = LIB_TRUE;
    return LIB_STATUS_OK;
}
lib_status softpc_sb_pcm_close(void)
{
    if (sb_close_status != LIB_STATUS_OK) return sb_close_status;
    sb_output_rate = 0;
    return LIB_STATUS_OK;
}
lib_status softpc_sb_pcm_clear(void)
{
    assert(sb_output_rate != 0);
    sb_pcm_count = 0; sb_pcm_consumed = 0; ++sb_pcm_epoch;
    return LIB_STATUS_OK;
}
lib_status softpc_sb_pcm_set_active(lib_bool active)
{
    if (sb_active_status != LIB_STATUS_OK) return sb_active_status;
    if (!active && sb_output_active) softpc_sb_pcm_clear();
    sb_output_active = active;
    return LIB_STATUS_OK;
}
lib_status softpc_sb_pcm_set_gain(lib_u16 gain)
{
    assert(sb_output_rate != 0);
    ++sb_gain_calls;
    if (sb_gain_status != LIB_STATUS_OK) return sb_gain_status;
    sb_output_gain = gain;
    return LIB_STATUS_OK;
}
lib_status softpc_sb_pcm_progress(lib_audio_pcm_progress *progress)
{
    progress->epoch = sb_pcm_epoch;
    progress->accepted_frames = progress->submitted_frames = sb_pcm_count;
    progress->consumed_frames = sb_pcm_consumed;
    return sb_progress_status;
}
lib_status softpc_sb_pcm_write(const lib_u8 *samples, lib_u32 count, lib_u32 *accepted)
{
    assert(count != 0 && count <= LIB_AUDIO_PCM_STREAM_MAX_FRAMES_PER_SUBMISSION);
    *accepted = 0;
    if (sb_pcm_status != LIB_STATUS_OK) return sb_pcm_status;
    if (sb_pcm_budget == 0) return LIB_STATUS_LIMIT_EXCEEDED;
    *accepted = count < sb_pcm_budget ? count : sb_pcm_budget;
    assert(sb_pcm_count + *accepted <= sizeof(sb_pcm_bytes));
    lib_memory_copy(sb_pcm_bytes + sb_pcm_count, samples, *accepted);
    sb_pcm_count += *accepted;
    sb_pcm_budget -= *accepted;
    return LIB_STATUS_OK;
}

static void verify_sb_block(softpc_machine *machine)
{
    static const unsigned lengths[] = { 32, 700, 65536 };
    unsigned direction, length, i;
    SB_DMA_INFO actual;
    SbDmaChannel = 1;
    assert(SbInitializeDMA());
    for (direction = 0; direction != 2; ++direction)
    for (length = 0; length != sizeof(lengths) / sizeof(lengths[0]); ++length) {
        IU16 start = direction ? 1 : 0xfffe;
        outb(0x0c, 0); dma_word(2, start); dma_word(3, 0x1234);
        outb(0x83, 2); outb(0x0b, (half_word)(0x49 | direction * 0x20));
        outb(0x0a, 1);
        for (i = 0; i < lengths[length]; ++i) {
            IU16 address = (IU16)(direction ? start - i : start + i);
            unsigned char sample = (unsigned char)(i * 7 + 3);
            assert(softpc_machine_write_physical(machine, 0x20000u + address,
                &sample, 1) == SOFTPC_MACHINE_OK);
        }
        SBBlockSize = lengths[length] - 1;
        DspNumberDmaQueries = 0;
        assert(DmaDataToDsp(Single));
        assert(DspVirtualInc == lengths[length] / 16);
        /* Modify the source after capture: pending PCM must remain frozen. */
        for (i = 0; i < lengths[length]; ++i) {
            IU16 address = (IU16)(direction ? start - i : start + i);
            unsigned char zero = 0;
            assert(softpc_machine_write_physical(machine, 0x20000u + address,
                &zero, 1) == SOFTPC_MACHINE_OK);
        }
        sb_pcm_count = 0; sb_pcm_budget = 7;
        assert(DspProcessBlock() == LIB_STATUS_OK && sb_pcm_count == 7 && !bWriteBurstStarted);
        assert(QueryDMA(&actual) && actual.addr == start && actual.count == 0x1234);
        assert(DspProcessBlock() == LIB_STATUS_OK && sb_pcm_count == 7);
        sb_pcm_status = LIB_STATUS_IO_ERROR;
        assert(DspProcessBlock() == LIB_STATUS_IO_ERROR && sb_pcm_count == 7);
        sb_pcm_status = LIB_STATUS_OK;
        sb_pcm_budget = lengths[length];
        assert(DspProcessBlock() == LIB_STATUS_OK && bWriteBurstStarted);
        assert(sb_pcm_count == lengths[length]);
        for (i = 0; i < lengths[length]; ++i)
            assert(sb_pcm_bytes[i] == (lib_u8)(i * 7 + 3));
        assert(DspProcessBlock() == LIB_STATUS_OK && sb_pcm_count == lengths[length]);
        assert(QueryDMA(&actual) && actual.addr == start && actual.count == 0x1234);
        outb(0x0a, 5);
        assert(!DmaDataToDsp(Single));
        outb(0x0a, 1);
        SBBlockSize = 65536;
        assert(!DmaDataToDsp(Single));
    }
}


static void verify_sb_output(void)
{
    lib_u32 opens, calls;
    DspCancelTransfer();
    ResetMixer();
    TimeConstant = 0xa6;
    assert(DspPrepareOutput() == LIB_STATUS_OK);
    assert(sb_output_rate == 11025 && sb_output_gain == 16383);
    assert(TimeConstant == 0xffff);
    opens = sb_open_calls;
    assert(DspPrepareOutput() == LIB_STATUS_OK && sb_open_calls == opens);
    DspWrite(DSP_SPEAKER_OFF); assert(sb_output_gain == 0);
    MixerAddrWrite(MIXER_MASTER_VOLUME); MixerDataWrite(14);
    MixerAddrWrite(MIXER_VOICE_VOLUME); MixerDataWrite(6);
    assert(sb_output_gain == 0);
    DspWrite(DSP_SPEAKER_ON);
    assert(sb_output_gain == (lib_u64)57343 * 49151 / 65535);
    MixerDataWrite(0); assert(sb_output_gain == 0);
    DspWrite(DSP_SPEAKER_ON); assert(sb_output_gain == 0);
    ResetMixer(); assert(sb_output_gain == 16383);
    TimeConstant = 0xa5; /* Same rounded rate does not recreate output. */
    assert(DspPrepareOutput() == LIB_STATUS_OK && sb_open_calls == opens);
    sb_close_status = LIB_STATUS_IO_ERROR;
    TimeConstant = 0xd3;
    assert(DspPrepareOutput() == LIB_STATUS_IO_ERROR);
    assert(sb_output_rate == 11025 && sb_open_calls == opens);
    sb_close_status = LIB_STATUS_OK;
    assert(DspPrepareOutput() == LIB_STATUS_OK);
    assert(sb_output_rate == 22050 && sb_open_calls == opens + 1);
    sb_gain_status = LIB_STATUS_IO_ERROR;
    DspWrite(DSP_SPEAKER_OFF);
    assert(DspProcessBlock() == LIB_STATUS_IO_ERROR);
    calls = sb_gain_calls;
    MixerSetVoiceVolume(6);
    assert(sb_gain_calls == calls); /* First failure remains terminal. */
    assert(DspPrepareOutput() == LIB_STATUS_IO_ERROR);
    assert(DspCloseOutput() == LIB_STATUS_OK && sb_output_rate == 0);
    sb_gain_status = LIB_STATUS_OK;
    DspCancelTransfer();
    TimeConstant = 0xa6;
    sb_open_status = LIB_STATUS_UNSUPPORTED;
    assert(DspPrepareOutput() == LIB_STATUS_UNSUPPORTED);
    assert(sb_output_rate == 0 && TimeConstant == 0xa6);
    sb_open_status = LIB_STATUS_OK;
    assert(DspPrepareOutput() == LIB_STATUS_OK && sb_output_rate == 11025);
    assert(DspCloseOutput() == LIB_STATUS_OK);
    assert(DspPrepareOutput() == LIB_STATUS_INVALID_STATE);
}

static IU8 sb_read(void) { IU8 value; DspReadData(&value); return value; }
static IU8 sb_status(void) { IU8 value; DspReadStatus(&value); return value; }

static void verify_sb_interrupt(unsigned delayed)
{
    IU32 hook = 0;
    IU8 value;
    ica_outb(0x20, 0x0a);
    ica_inb(0x20, &value);
    assert(value & 0x20);
    assert(!!(DelayIrqLine & 0x20) == !!delayed);
    if (delayed) {
        assert(!(ica_scan_irr(ICA_MASTER) & 0x80));
        dispatch_q_event();
    }
    assert(ica_scan_irr(ICA_MASTER) & 0x80);
    assert(ica_intack(&hook) == 0x0d);
    ica_outb(0x20, 0x65);
    assert(!(ica_scan_irr(ICA_MASTER) & 0x80));
}

static void verify_sb_transfer(softpc_machine *machine)
{
    unsigned direction, i;
    unsigned char value;
    SB_DMA_INFO actual;
    for (direction = 0; direction != 2; ++direction) {
        StopDspTransfer(TRUE); DspCancelTransfer();
        TimeConstant = 0xa6; ResetMixer();
        ica0_post(); ica_outb(0x21, 0xdf);
        outb(0x0c, 0); dma_word(2, 0x1000); dma_word(3, 63);
        outb(0x83, 2); outb(0x0b, (half_word)(0x59 | direction * 0x20));
        outb(0x0a, 1);
        for (i = 0; i != 64; ++i) {
            value = (unsigned char)i;
            assert(softpc_machine_write_physical(machine,
                0x20000 + (direction ? 0x1000 - i : 0x1000 + i), &value, 1)
                == SOFTPC_MACHINE_OK);
        }
        sb_pcm_budget = 7; sb_pcm_consumed = 0;
        DspWrite(DSP_SET_BLOCK_SIZE); DspWrite(31); DspWrite(0);
        DspWrite(DSP_WRITE_AUTO);
        assert(bDspActive && DspMode == Auto && sb_pcm_count == 7);
        assert(!bWriteBurstStarted && DspServiceTransfer() == LIB_STATUS_OK);
        assert(!(ica_scan_irr(ICA_MASTER) & 0x80));
        sb_pcm_budget = 4096;
        assert(DspServiceTransfer() == LIB_STATUS_OK && sb_pcm_count == 32);
        for (i = 0; i != 32; ++i) assert(sb_pcm_bytes[i] == i);
        dispatch_q_event();
        assert(QueryDMA(&actual));
        assert(actual.addr == (direction ? 0x0fe0 : 0x1020) && actual.count == 31);
        assert(sb_pcm_count == 32); /* IRQ alone does not capture another block. */
        verify_sb_interrupt(0); /* Actual PIC acknowledge + EOI starts block two. */
        assert(sb_pcm_count == 64 && bWriteBurstStarted);
        for (i = 32; i != 64; ++i) assert(sb_pcm_bytes[i] == i);
        DspWrite(DSP_PAUSE_DMA);
        dispatch_q_event(); verify_sb_interrupt(0);
        assert(sb_pcm_count == 64 && !bWriteBurstStarted);
        assert(QueryDMA(&actual) && actual.addr == 0x1000 && actual.count == 63);
        DspWrite(DSP_CONTINUE_DMA);
        assert(sb_pcm_count == 64); /* Continue alone does not invent an EOI. */
        DspWrite(DSP_WRITE_AUTO);
        assert(sb_pcm_count == 96);
        DspWrite(DSP_STOP_AUTO);
        dispatch_q_event(); verify_sb_interrupt(0);
        assert(DspMode == None && sb_pcm_count == 128);
        dispatch_q_event(); verify_sb_interrupt(0);
        assert(sb_pcm_count == 128); /* Last submitted block retains its IRQ. */
        assert(StartDspTransfer(Single));
        assert(sb_pcm_count == 32);
        StopDspTransfer(TRUE);
        dispatch_q_event();
        assert(!bDspActive && sb_output_rate == 0);
        assert(!(ica_scan_irr(ICA_MASTER) & 0x80));
    }
}

static void verify_sb_transfer_failures(void)
{
    half_word pending;
    StopDspTransfer(TRUE); DspCancelTransfer();
    SbDmaChannel = 1; assert(SbInitializeDMA());
    TimeConstant = 0xa6; SBBlockSize = 31;
    ica0_post(); ica_outb(0x21, 0xdf);
    outb(0x0c, 0); dma_word(2, 0x1000); dma_word(3, 63);
    outb(0x0b, 0x59); outb(0x0a, 1);
    sb_pcm_budget = 4096;
    assert(RegisterEOIHook(5, record_eoi));
    assert(!StartDspTransfer(Auto));
    assert(DspServiceTransfer() == LIB_STATUS_INVALID_STATE);
    assert(!bDspActive && sb_output_rate == 0);
    assert(RemoveEOIHook(5, record_eoi)); /* Failed start retains other owner. */
    StopDspTransfer(TRUE); TimeConstant = 0xa6;
    sb_open_status = LIB_STATUS_UNSUPPORTED;
    assert(!StartDspTransfer(Auto));
    assert(DspServiceTransfer() == LIB_STATUS_UNSUPPORTED);
    assert(!bDspActive && sb_output_rate == 0);
    sb_open_status = LIB_STATUS_OK;
    StopDspTransfer(TRUE); TimeConstant = 0xa6;
    assert(StartDspTransfer(Single));
    sb_gain_status = LIB_STATUS_IO_ERROR;
    SetSpeaker(FALSE);
    dispatch_q_event();
    assert(DspServiceTransfer() == LIB_STATUS_IO_ERROR);
    assert(!(ica_scan_irr(ICA_MASTER) & 0x80));
    sb_gain_status = LIB_STATUS_OK;
    StopDspTransfer(TRUE);
    GenerateInterrupt(2);
    assert(DelayIrqLine & 0x20);
    StopDspTransfer(TRUE);
    dispatch_q_event();
    assert(!(DelayIrqLine & 0x20));
    ica_outb(0x20, 0x0a); ica_inb(0x20, &pending);
    assert(!(pending & 0x20));
}

static void verify_sb_ports(softpc_machine *machine)
{
    IU8 value;
    static const IU16 ports[] = { 0x224, 0x225, 0x226, 0x22a, 0x22c, 0x22e };
    unsigned i;
    assert(SbTerminate()); /* Machine reset installed the production device. */
    io_connect_port(0x224, DMA_ADAPTOR, IO_READ_WRITE);
    assert(!SbInitialize());
    assert(Ios_in_adapter_table[0x224] == DMA_ADAPTOR);
    assert(Ios_in_adapter_table[0x226] == EMPTY_ADAPTOR);
    io_disconnect_port(0x224, DMA_ADAPTOR);
    assert(SbInitialize());
    for (i = 0; i < sizeof(ports) / sizeof(ports[0]); ++i) {
        assert(Ios_in_adapter_table[ports[i]] == SNDBLST_ADAPTER);
        assert(Ios_out_adapter_table[ports[i]] == SNDBLST_ADAPTER);
    }
    inb(0x224, &value); assert(value == 0xff);
    inb(0x22c, &value); assert(value == 0x7f);
    outb(0x224, MIXER_MASTER_VOLUME); outb(0x225, 0x0e);
    assert(SbInitialize()); /* Repeated attach must not reset mixer state. */
    inb(0x225, &value); assert(value == 0x0e);
    outb(0x226, 1);
    inb(0x225, &value); assert(value == 0x0e);
    outb(0x226, 0);
    inb(0x22e, &value); assert(value == 0xff);
    inb(0x22a, &value); assert(value == 0xaa);
    inb(0x22e, &value); assert(value == 0x7f);
    outb(0x224, MIXER_MASTER_VOLUME);
    inb(0x225, &value); assert(value == 0x08);
    outb(0x22c, DSP_GET_VERSION);
    inb(0x22a, &value); assert(value == 2);
    inb(0x22a, &value); assert(value == 1);
    outb(0x22c, DSP_SET_SAMPLE_RATE); outb(0x22c, 0xa5);
    outb(0x0a, 5); /* Masked channel: no transfer, but real high-speed state. */
    outb(0x22c, DSP_WRITE_HS);
    assert(bHighSpeedMode);
    inb(0x22c, &value); assert(value == 0xff);
    outb(0x22c, DSP_GET_VERSION); /* Writes ignored until reset. */
    inb(0x22e, &value); assert(value == 0x7f);
    outb(0x224, MIXER_VOICE_VOLUME); outb(0x225, 0x06);
    inb(0x225, &value); assert(value == 0x06);
    outb(0x226, 1); outb(0x226, 0);
    assert(!bHighSpeedMode);
    inb(0x22a, &value); assert(value == 0xaa);
    inb(0x22c, &value); assert(value == 0x7f);
    outb(0x22c, DSP_GET_VERSION);
    inb(0x22a, &value); assert(value == 2);
    inb(0x22a, &value); assert(value == 1);
    outb(0x22c, DSP_SET_SAMPLE_RATE); outb(0x22c, 0xd3);
    outb(0x226, 1); /* Leave a partial reset pulse in the old lifetime. */
    sb_close_status = LIB_STATUS_IO_ERROR;
    assert(softpc_machine_destroy(machine) == SOFTPC_MACHINE_IO_ERROR);
    assert(sb_output_rate != 0);
    assert(softpc_machine_read_physical(machine, 0x20000, &value, 1) ==
        SOFTPC_MACHINE_OK); /* Failed destruction retains the machine and RAM. */
    sb_close_status = LIB_STATUS_OK;
    assert(SbTerminate()); assert(SbTerminate());
    assert(!bDspActive && sb_output_rate == 0);
    for (i = 0; i < sizeof(ports) / sizeof(ports[0]); ++i) {
        assert(Ios_in_adapter_table[ports[i]] == EMPTY_ADAPTOR);
        assert(Ios_out_adapter_table[ports[i]] == EMPTY_ADAPTOR);
    }
    assert(SbInitialize()); /* Teardown permits a new device lifetime. */
    assert(TimeConstant == 0xa6);
    inb(0x22a, &value); assert(value == 0xaa);
    outb(0x226, 0); /* Must not finish the old lifetime's pulse. */
    inb(0x22e, &value); assert(value == 0x7f);
    outb(0x22c, DSP_SET_SAMPLE_RATE); outb(0x22c, 0xd3);
    outb(0x226, 1);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(TimeConstant == 0xa6);
    inb(0x22a, &value); assert(value == 0xaa);
    outb(0x226, 0);
    inb(0x22e, &value); assert(value == 0x7f);
    for (i = 0; i < sizeof(ports) / sizeof(ports[0]); ++i)
        assert(Ios_in_adapter_table[ports[i]] == SNDBLST_ADAPTER);
    assert(SbTerminate());
}

static void verify_sb_executor(softpc_machine *machine)
{
    unsigned fail, i;
    for (fail = 0; fail != 2; ++fail) {
        assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
        outb(0x0c, 0); dma_word(2, 0x1000); dma_word(3, 63);
        outb(0x83, 2); outb(0x0b, 0x49); outb(0x0a, 1);
        for (i = 0; i != 64; ++i) {
            lib_u8 value = (lib_u8)i;
            assert(softpc_machine_write_physical(machine, 0x21000 + i,
                &value, 1) == SOFTPC_MACHINE_OK);
        }
        sb_pcm_budget = 7;
        outb(0x22c, DSP_WRITE); outb(0x22c, 63); outb(0x22c, 0);
        assert(sb_pcm_count == 7 && !bWriteBurstStarted);
        sb_pcm_budget = 4096;
        if (fail) {
            sb_pcm_status = LIB_STATUS_IO_ERROR;
            softpc_platform_request_executor_wake();
            assert(softpc_machine_run(machine, 10000) == SOFTPC_MACHINE_IO_ERROR);
            assert(DspGetStatus() == LIB_STATUS_IO_ERROR && sb_pcm_count == 7);
            sb_pcm_status = LIB_STATUS_OK;
        } else {
            softpc_platform_executor_event();
            assert(sb_pcm_count == 64 && bWriteBurstStarted);
            assert(DspGetStatus() == LIB_STATUS_OK);
            for (i = 0; i != 64; ++i) assert(sb_pcm_bytes[i] == i);
        }
    }
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(DspGetStatus() == LIB_STATUS_OK);
}

static void verify_sb_machine_pause(softpc_machine *machine)
{
    unsigned partial, consumed, i;
    SB_DMA_INFO before, after;
    for (partial = 0; partial != 2; ++partial)
    for (consumed = 0; consumed != 3; ++consumed) {
        lib_u32 played = consumed == 0 ? 0 : consumed == 1 ? 3 : partial ? 7 : 64;
        lib_u64 epoch;
        assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
        outb(0x0c, 0); dma_word(2, 0x1000); dma_word(3, 63);
        outb(0x83, 2); outb(0x0b, 0x49); outb(0x0a, 1);
        for (i = 0; i != 64; ++i) {
            lib_u8 value = (lib_u8)i;
            assert(softpc_machine_write_physical(machine, 0x21000 + i,
                &value, 1) == SOFTPC_MACHINE_OK);
        }
        sb_pcm_budget = partial ? 7 : 64;
        outb(0x22c, DSP_WRITE); outb(0x22c, 63); outb(0x22c, 0);
        sb_pcm_consumed = played;
        assert(QueryDMA(&before));
        softpc_machine_set_heartbeat(machine, 0);
        assert(DspGetStatus() == LIB_STATUS_OK && !sb_output_active);
        assert(sb_pcm_count == 0 && !bWriteBurstStarted);
        epoch = sb_pcm_epoch;
        softpc_machine_set_heartbeat(machine, 0);
        assert(sb_pcm_epoch == epoch); /* Repeated park cannot discard twice. */
        sb_pcm_budget = 4096;
        assert(DspServiceTransfer() == LIB_STATUS_OK && sb_pcm_count == 0);
        assert(QueryDMA(&after) && after.addr == before.addr && after.count == before.count);
        softpc_machine_set_heartbeat(machine, 1);
        assert(DspGetStatus() == LIB_STATUS_OK && sb_output_active && bWriteBurstStarted);
        assert(sb_pcm_count == 64 - played);
        for (i = 0; i < sb_pcm_count; ++i) assert(sb_pcm_bytes[i] == i + played);
        softpc_machine_set_heartbeat(machine, 1);
        assert(sb_pcm_count == 64 - played);
        if (played < 64) {
            sb_pcm_consumed = 2;
            softpc_machine_set_heartbeat(machine, 0);
            assert(DspGetStatus() == LIB_STATUS_OK && !sb_output_active);
            played += 2;
            softpc_machine_set_heartbeat(machine, 1);
            assert(DspGetStatus() == LIB_STATUS_OK && sb_pcm_count == 64 - played);
            for (i = 0; i < sb_pcm_count; ++i) assert(sb_pcm_bytes[i] == i + played);
        }
        assert(QueryDMA(&after) && after.addr == before.addr && after.count == before.count);
        /* Resumption did not complete DMA; only the existing deadline may. */
        dispatch_q_event();
        assert(QueryDMA(&after) && after.addr == 0x1040 && after.count == 0xffff);
    }
    sb_active_status = LIB_STATUS_IO_ERROR;
    softpc_machine_set_heartbeat(machine, 0);
    assert(DspGetStatus() == LIB_STATUS_IO_ERROR && sb_output_active);
    sb_active_status = LIB_STATUS_OK;
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
}

static void verify_sb_commands(softpc_machine *machine)
{
    static const struct { unsigned address, incremented, decremented; } edges[] = {
        { 0x1000, 0x1003, 0x0ffd },
        { 0xfffe, 0x0001, 0xfffb },
        { 0x0001, 0x0004, 0xfffe }
    };
    unsigned i, automatic, decrement, edge;
    SB_DMA_INFO info;
    unsigned char observed;
    SbDmaChannel = 1;
    assert(SbInitializeDMA());
    ica0_post();
    ica_outb(0x21, 0xdf);
    GenerateInterrupt(0);
    verify_sb_interrupt(0);
    assert(sb_status() == 0xff && sb_read() == 0xaa);
    assert(sb_status() == 0x7f && sb_read() == 0xff);
    DspResetWrite(0);
    assert(sb_status() == 0x7f);
    DspResetWrite(1); DspResetWrite(2);
    assert(sb_status() == 0x7f);
    DspResetWrite(0);
    assert(!bDspActive && sb_output_rate == 0);
    assert(sb_status() == 0xff && sb_read() == 0xaa && sb_status() == 0x7f);
    DspWrite(DSP_GET_VERSION);
    assert(sb_status() == 0xff && sb_read() == 2 && sb_status() == 0xff);
    assert(sb_read() == 1 && sb_status() == 0x7f && sb_read() == 0xff);
    for (i = 0x30; i <= 0x38; ++i) {
        DspWrite((IU8)i);
        assert(sb_status() == 0x7f);
        DspWrite(DSP_GET_VERSION); /* Unsupported SB-MIDI never captures data. */
        assert(sb_read() == 2 && sb_read() == 1 && sb_status() == 0x7f);
    }
    for (i = 0; i != 256; ++i) {
        DspWrite(DSP_CARD_IDENTIFY); DspWrite((IU8)i);
        assert(sb_status() == 0xff && sb_read() == (IU8)~i && sb_status() == 0x7f);
        DspWrite(DSP_LOAD_RES_REG); DspWrite((IU8)i); DspWrite(DSP_READ_RES_REG);
        assert(sb_read() == i);
    }
    DspWrite(DSP_SET_SAMPLE_RATE); DspWrite(0xa5);
    assert(TimeConstant == 0xa5 && GetSamplingRate() == 11025);
    DspWrite(DSP_SET_BLOCK_SIZE); DspWrite(0x34); DspWrite(0x12);
    assert(SBBlockSize == 0x1234);
    outb(0x0a, 5); /* Dispatch test: masked DMA does not submit a block. */
    DspWrite(DSP_WRITE_AUTO);
    assert(bDspActive && DspMode == Auto);
    DspWrite(DSP_WRITE); DspWrite(0x21); DspWrite(0);
    assert(DspMode == Single && SBBlockSize == 0x21);
    DspWrite(DSP_WRITE_HS_AUTO);
    assert(DspMode == Auto && bHighSpeedMode);
    DspResetWrite(1); DspResetWrite(0);
    assert(bDspActive && !bHighSpeedMode);
    assert(TimeConstant == 0xffff && sb_output_rate == 11025);
    assert(SBBlockSize == 0x7ff && sb_read() == 0xaa);
    DspWrite(DSP_WRITE_HS);
    assert(DspMode == Single && bHighSpeedMode);
    DspResetWrite(1); DspResetWrite(0); (void)sb_read();
    DspResetWrite(1); DspResetWrite(0); (void)sb_read();
    assert(!bDspActive && sb_output_rate == 0 && TimeConstant == 0xa6);
    for (i = 0; i != 2; ++i) {
        DspMode = i ? Auto : Single;
        DspWrite(DSP_PAUSE_DMA); assert(bDspPaused);
        DspWrite(DSP_PAUSE_DMA); assert(bDspPaused);
        DspWrite(DSP_CONTINUE_DMA); assert(!bDspPaused);
        DspWrite(DSP_CONTINUE_DMA); assert(!bDspPaused);
        DspWrite(DSP_PAUSE_DMA);
        DspResetWrite(1); DspResetWrite(0); (void)sb_read();
        assert(!bDspPaused && DspMode == None);
    }
    {
        lib_u32 calls = sb_gain_calls;
        DspWrite(DSP_SPEAKER_ON); DspWrite(DSP_SPEAKER_OFF);
        assert(sb_gain_calls == calls); /* Closed output ignores speaker commands. */
    }
    DspWrite(DSP_STOP_AUTO); assert(bExitAuto);
    DspWrite(DSP_GENERATE_INT); verify_sb_interrupt(1);
    DspWrite(0xff); assert(sb_status() == 0x7f && !bDspActive);

    for (automatic = 0; automatic != 2; ++automatic)
    for (decrement = 0; decrement != 2; ++decrement)
    for (edge = 0; edge != sizeof(edges) / sizeof(edges[0]); ++edge) {
        outb(0x0c, 0); dma_word(2, edges[edge].address); dma_word(3, 2);
        outb(0x83, 2); outb(0x0b, (half_word)(0x49 | decrement * 0x20));
        DspWrite(automatic ? DSP_SET_BLOCK_SIZE : DSP_WRITE);
        DspWrite(2); DspWrite(0);
        if (automatic) DspWrite(DSP_WRITE_AUTO);
        assert(QueryDMA(&info));
        assert(info.addr == (decrement ? edges[edge].decremented : edges[edge].incremented));
        assert(info.count == 0xffff && info.page == 2);
        assert(info.status & 2);
        assert(!bDspActive);
        verify_sb_interrupt(1);
    }
    /* XP's one-byte probe raises IRQ without advancing the DMA registers. */
    outb(0x83, 0);
    outb(0x0c, 0); dma_word(2, 0x1000); dma_word(3, 2);
    DspWrite(DSP_WRITE); DspWrite(0); DspWrite(0);
    assert(QueryDMA(&info) && info.addr == 0x1000 && info.count == 2);
    verify_sb_interrupt(1);
    DspWrite(DSP_READ); DspWrite(1); DspWrite(0);
    assert(softpc_machine_read_physical(machine, 0x1000, &observed, 1) == SOFTPC_MACHINE_OK);
    assert(observed == 0x80);
    verify_sb_interrupt(1);
    for (decrement = 0; decrement != 2; ++decrement) {
        outb(0x0c, 0); dma_word(2, 0x1000); dma_word(3, 2);
        outb(0x0b, (half_word)(0x49 | decrement * 0x20));
        DspWrite(DSP_TABLE_MUNGE); DspWrite(0);
        DspWrite(DSP_TABLE_MUNGE); DspWrite(0);
        assert(softpc_machine_read_physical(machine, 0x1000, &observed, 1) == SOFTPC_MACHINE_OK && observed == 0x20);
        assert(softpc_machine_read_physical(machine, decrement ? 0xfff : 0x1001,
            &observed, 1) == SOFTPC_MACHINE_OK && observed == 0xc5);
        assert(QueryDMA(&info) && info.addr == (decrement ? 0xffe : 0x1002) && info.count == 0);
    }
}

static int sb_test_encode_event(Q_CALLBACK_FN callback, unsigned long *id)
{
    assert(callback != NULL);
    *id = 1;
    return 1;
}

static void verify_sb_deadline(void)
{
    Q_EVENT_SNAPSHOT_STATE state;
    Q_EVENT_SNAPSHOT_ENTRY quick[32], tick[32];
    unsigned long i, matches = 0;
    assert(q_event_snapshot_capture(&state, quick, 32, tick, 32, sb_test_encode_event));
    for (i = 0; i < state.quick_entries; ++i)
        if (quick[i].original_time == 93000 && quick[i].event_type == 0) ++matches;
    assert(matches == 1); /* 1025 samples at 11025 Hz: 92 + 1 milliseconds, EVENT_TIME. */
}

static void verify_sb_completion(void)
{
    unsigned direction, automatic, scenario, i;
    SB_DMA_INFO actual;
    DspCancelTransfer();
    for (direction = 0; direction != 2; ++direction)
    for (automatic = 0; automatic != 2; ++automatic)
    for (scenario = 0; scenario != 6; ++scenario) {
        ica0_post(); ica_outb(0x21, 0xdf);
        outb(0x0c, 0); dma_word(2, 0x1000); dma_word(3, 0x400);
        outb(0x83, 2); outb(0x0b, (half_word)(0x49 | direction * 0x20 | automatic * 0x10));
        outb(0x0a, 1);
        SBBlockSize = 0x400; DspMode = Single; bDspActive = TRUE;
        sb_pcm_count = 0; sb_pcm_consumed = 0; sb_pcm_budget = 13;
        assert(DmaDataToDsp(Single));
        assert(DspArmCompletion(11025) == LIB_STATUS_INVALID_STATE);
        assert(DspProcessBlock() == LIB_STATUS_OK && !bWriteBurstStarted);
        assert(DspArmCompletion(11025) == LIB_STATUS_INVALID_STATE);
        sb_pcm_budget = 1025;
        assert(DspProcessBlock() == LIB_STATUS_OK && bWriteBurstStarted);
        assert(DspArmCompletion(0) == LIB_STATUS_INVALID_ARGUMENT);
        bHighSpeedMode = TRUE;
        assert(DspArmCompletion(11025) == LIB_STATUS_OK && !bHighSpeedMode);
        verify_sb_deadline();
        assert(DspArmCompletion(11025) == LIB_STATUS_INVALID_STATE);
        assert(!DmaDataToDsp(Single)); /* A live deadline owns this block. */
        assert(QueryDMA(&actual) && actual.addr == 0x1000 && actual.count == 0x400);
        assert(!(ica_scan_irr(ICA_MASTER) & 0x80));
        if (scenario == 1 || scenario == 2) {
            SbGetDMAPosition();
            for (i = 0; i != (scenario == 2 ? 9u : 1u); ++i) {
                dispatch_q_event();
                assert(!(ica_scan_irr(ICA_MASTER) & 0x80));
            }
            if (scenario == 1) sb_pcm_consumed = 1025;
        }
        if (scenario == 3) ++sb_pcm_epoch;
        if (scenario == 4) sb_progress_status = LIB_STATUS_IO_ERROR;
        if (scenario == 5) DspCancelTransfer();
        dispatch_q_event();
        if (scenario >= 3) {
            assert(!(ica_scan_irr(ICA_MASTER) & 0x80));
            assert(QueryDMA(&actual) && actual.addr == 0x1000 && actual.count == 0x400);
            assert(DspProcessBlock() == (scenario == 3 ? LIB_STATUS_INVALID_STATE :
                scenario == 4 ? LIB_STATUS_IO_ERROR : LIB_STATUS_OK));
        } else {
            assert(QueryDMA(&actual) && actual.page == 2);
            assert(actual.addr == (automatic ? 0x1000 : direction ? 0x0bff : 0x1401));
            assert(actual.count == (automatic ? 0x400 : 0xffff));
            assert(actual.status & 2);
            assert(!SbAnswerDMAPosition && !bWriteBurstStarted);
            verify_sb_interrupt(0);
            assert(DspProcessBlock() == LIB_STATUS_OK);
            assert(sb_pcm_count == 1025); /* Completion cannot queue the block again. */
        }
        sb_progress_status = LIB_STATUS_OK;
        DspCancelTransfer();
    }
    bDspActive = FALSE;
}

static void verify_sb_dma_read_ports(void)
{
    unsigned channel, decrement, count;
    half_word low, high;
    for (channel = 1; channel <= 3; channel += 2)
    for (decrement = 0; decrement != 2; ++decrement)
    for (count = 0; count != 2; ++count) {
        io_addr port = (io_addr)(channel * 2 + count);
        SbDmaChannel = (IU16)channel;
        assert(SbInitializeDMA());
        StartingDmaAddr = 0x12f0; StartingDmaCount = 0x1000;
        outb(0x0c, 0); dma_word((io_addr)(channel * 2), StartingDmaAddr);
        dma_word((io_addr)(channel * 2 + 1), StartingDmaCount);
        outb(0x0b, (half_word)(0x48 | channel | decrement * 0x20));
        assert(QueryDMA(&DmaInfo));
        SBBlockSize = 0x400; DspMode = Single; DspVirtualInc = 64;
        DspNumberDmaQueries = PreviousWaveOutPos = 0;
        bDspActive = SbAnswerDMAPosition = bWriteBurstStarted = TRUE;
        outb(0x0c, 0);
        inb(port, &low);
        assert(DspNumberDmaQueries == 1);
        inb(port, &high);
        assert(DspNumberDmaQueries == 1);
        assert(((unsigned)high * 256 + low) ==
            (count ? 0x0fc0u : decrement ? 0x12b0u : 0x1330u));
        /* Another channel and the word-wide controller must not sample SB. */
        outb(0x0c, 0); inb(0, &low); inb(0, &high);
        outb(0xd8, 0); inb(0xc4, &low); inb(0xc4, &high);
        assert(DspNumberDmaQueries == 1);
        bDspActive = FALSE;
        outb(0x0c, 0); inb(port, &low); inb(port, &high);
        assert(DspNumberDmaQueries == 1);
    }
    SbAnswerDMAPosition = bWriteBurstStarted = FALSE;
}

static void verify_sb_position(void)
{
    unsigned decrement, small, automatic, i;
    SB_DMA_INFO actual;
    SbDmaChannel = 1;
    assert(SbInitializeDMA());
    for (decrement = 0; decrement != 2; ++decrement)
    for (small = 0; small != 2; ++small)
    for (automatic = 0; automatic != 2; ++automatic) {
        StartingDmaAddr = decrement ? 0x0010 : 0xfff0;
        StartingDmaCount = 0x1000;
        outb(0x0c, 0); dma_word(2, StartingDmaAddr); dma_word(3, StartingDmaCount);
        outb(0x83, 2); outb(0x0b, (half_word)(0x49 | decrement * 0x20));
        assert(QueryDMA(&DmaInfo));
        SBBlockSize = small ? 0xff : 0x400;
        DspMode = automatic ? Auto : Single;
        DspVirtualInc = small ? 16 : 64;
        PreviousWaveOutPos = DspNumberDmaQueries = 0;
        bDspActive = FALSE; SbAnswerDMAPosition = bWriteBurstStarted = TRUE;
        SbGetDMAPosition();
        bDspActive = TRUE; SbAnswerDMAPosition = FALSE;
        SbGetDMAPosition();
        SbAnswerDMAPosition = TRUE; bWriteBurstStarted = FALSE;
        SbGetDMAPosition();
        assert(DspNumberDmaQueries == 0);
        assert(QueryDMA(&actual) && actual.addr == StartingDmaAddr && actual.count == 0x1000);
        bWriteBurstStarted = TRUE;
        for (i = 0; i != 100; ++i) SbGetDMAPosition();
        assert(DspNumberDmaQueries == 100);
        assert(QueryDMA(&actual) && actual.page == 2);
        if (small) {
            assert(actual.addr == (decrement ? 0xff60 : 0x00a0));
            assert(actual.count == 0x0f50 && DspVirtualInc == 16);
            assert(PreviousWaveOutPos == 0);
        } else {
            assert(actual.addr == (decrement ? 0xfc18 : 0x03e8));
            assert(actual.count == 0x0c08 && DspVirtualInc == 2);
            assert(PreviousWaveOutPos == (automatic ? 0u : 100u));
        }
    }
    bDspActive = SbAnswerDMAPosition = bWriteBurstStarted = FALSE;
}

static void verify_sb_dma(void)
{
    static const struct { IU32 constant, rate, stored; } rates[] = {
        { 0, 3921, 1 }, { 1, 3921, 1 }, { 0x80, 7812, 0x80 },
        { 0x82, 8000, 0x82 }, { 0x83, 8000, 0x83 }, { 0x84, 8000, 0x84 },
        { 0xa5, 11025, 0xa5 }, { 0xa6, 11025, 0xa6 },
        { 0xd2, 22050, 0xd2 }, { 0xd3, 22050, 0xd3 },
        { 0xe9, 44100, 0xe9 }, { 0xea, 44100, 0xea }, { 0xff, 44100, 0xea }
    };
    unsigned i, channel, automatic, decrement;
    SB_DMA_INFO info;
    half_word low, high, status;
    for (i = 0; i != sizeof(rates) / sizeof(rates[0]); ++i) {
        TimeConstant = rates[i].constant;
        assert(GetSamplingRate() == rates[i].rate && TimeConstant == rates[i].stored);
    }
    for (channel = 1; channel <= 3; channel += 2)
    for (automatic = 0; automatic != 2; ++automatic)
    for (decrement = 0; decrement != 2; ++decrement) {
        SbDmaChannel = (IU16)channel;
        assert(SbInitializeDMA());
        outb(0x0c, 0);
        dma_word((io_addr)(channel * 2), 0x9234);
        dma_word((io_addr)(channel * 2 + 1), 0x1234);
        outb(channel == 1 ? 0x83 : 0x82, 0xab);
        outb(0x0b, (half_word)(0x48 | channel | automatic * 0x10 | decrement * 0x20));
        outb(0x0a, (half_word)(4 | channel));
        assert(QueryDMA(&info));
        assert(info.addr == 0x9234 && info.count == 0x1234 && info.page == 0xab);
        assert(info.mask & (1u << channel));
        assert(!!(info.mode & 0x10) == automatic && !!(info.mode & 0x20) == decrement);
        assert(GetDMATransferAddress() == 0xab9234);
        outb(0x0a, (half_word)channel);
        assert(QueryDMA(&info) && !(info.mask & (1u << channel)));
        info.addr = 0x4321; info.count = 0x0201;
        assert(SetDMACountAddr(&info));
        outb(0x0c, 0);
        inb((io_addr)(channel * 2), &low); inb((io_addr)(channel * 2), &high);
        assert(low == 0x21 && high == 0x43);
        inb((io_addr)(channel * 2 + 1), &low); inb((io_addr)(channel * 2 + 1), &high);
        assert(low == 1 && high == 2);
        info.count = 0xffff;
        assert(SetDMACountAddr(&info) && QueryDMA(&info));
        assert(info.addr == (automatic ? 0x9234 : 0x4321));
        assert(info.count == (automatic ? 0x1234 : 0xffff));
        SetDMAStatus(&info, TRUE, TRUE);
        inb(0x08, &status);
        assert((status & (0x11u << channel)) == (0x11u << channel));
        assert(QueryDMA(&info) && !(info.status & (1u << channel)));
        SetDMAStatus(&info, FALSE, FALSE);
        assert(QueryDMA(&info) && !(info.status & (0x11u << channel)));
    }
    SbDmaChannel = 4;
    assert(!SbInitializeDMA()); /* SB2's byte-wide DMA must not bind the cascade. */
    SbDmaChannel = 1;
    assert(SbInitializeDMA());
    verify_sb_position();
    verify_sb_dma_read_ports();
}

static void verify_dma_transfers(softpc_machine *machine)
{
    static unsigned char original[0x20000], actual[0x20000], expected[0x20000];
    static const IU16 starts[] = { 0x2100, 0, 1, 0xffff, 0xfffe };
    unsigned controller, decrement, type, start, index;

    for (controller = 0; controller != 2; ++controller)
    for (decrement = 0; decrement != 2; ++decrement)
    for (type = 0; type != 3; ++type)
    for (start = 0; start != sizeof(starts) / sizeof(starts[0]); ++start) {
        const unsigned unit = controller ? 2 : 1;
        const unsigned bytes = 0x10000 * unit;
        const half_word channel = controller ? 5 : 1;
        unsigned char guarded[24], wanted[24];
        sys_addr address;
        word count;
        IU16 offset = starts[start];
        unsigned transfer, lane;

        for (index = 0; index != bytes; ++index)
            original[index] = (unsigned char)(index ^ (index >> 8) ^ 0x5a);
        lib_memory_copy(expected, original, bytes);
        lib_memory_set(guarded, 0xa5, sizeof(guarded));
        for (index = 0; index != 4 * unit; ++index)
            guarded[8 + index] = (unsigned char)(index + 0x31);
        lib_memory_copy(wanted, guarded, sizeof(wanted));
        for (transfer = 0; transfer != 4; ++transfer) {
            for (lane = 0; lane != unit; ++lane) {
                unsigned position = (unsigned)offset * unit + lane;
                if (type == DMA_WRITE_TRANSFER)
                    expected[position] = guarded[8 + transfer * unit + lane];
                if (type == DMA_READ_TRANSFER)
                    wanted[8 + transfer * unit + lane] = original[position];
            }
            offset = (IU16)(decrement ? offset - 1 : offset + 1);
        }
        assert(softpc_machine_write_physical(machine, 0x40000,
            original, bytes) == SOFTPC_MACHINE_OK);
        dma_post();
        outb(controller ? 0xd8 : 0x0c, 0);
        dma_word(controller ? 0xc4 : 0x02, starts[start]);
        dma_word(controller ? 0xc6 : 0x03, 3);
        /* Word channels ignore page bit zero; both cases address bank 4. */
        outb(controller ? 0x8b : 0x83, controller ? 5 : 4);
        outb(controller ? 0xd6 : 0x0b,
            (half_word)(0x41 | (type << 2) | (decrement << 5)));
        outb(controller ? 0xd4 : 0x0a, 1);
        /* Zero length and a disabled controller must not touch either side. */
        assert(dma_request(channel, (char *)guarded + 8, 0) == TRUE);
        outb(controller ? 0xd0 : 0x08, 4);
        assert(dma_request(channel, (char *)guarded + 8, 4) == TRUE);
        outb(controller ? 0xd0 : 0x08, 0);
        dma_enquire(channel, &address, &count);
        assert(address == 0x40000 + (unsigned)starts[start] * unit);
        assert(count == 3);
        for (index = 0; index != sizeof(guarded); ++index)
            assert(guarded[index] == (index >= 8 && index < 8 + 4 * unit ?
                (unsigned char)(index - 8 + 0x31) : 0xa5));
        /* Split the device request too: partial progress, then terminal count. */
        assert(dma_request(channel, (char *)guarded + 8, 1) == TRUE);
        assert(dma_request(channel, (char *)guarded + 8 + unit, 3) == FALSE);
        for (index = 0; index != sizeof(guarded); ++index)
            assert(guarded[index] == wanted[index]);
        assert(softpc_machine_read_physical(machine, 0x40000,
            actual, bytes) == SOFTPC_MACHINE_OK);
        for (index = 0; index != bytes; ++index)
            assert(actual[index] == expected[index]);
        dma_enquire(channel, &address, &count);
        assert(address == 0x40000 + (unsigned)offset * unit);
        assert(count == 0xffff);
        /* Reading status acknowledges TC; the automatically masked channel
           must leave the complete guarded buffer unchanged on another call. */
        {
            half_word status;
            inb(controller ? 0xd0 : 0x08, &status);
            assert((status & 2) != 0);
            inb(controller ? 0xd0 : 0x08, &status);
            assert((status & 2) == 0);
        }
        assert(dma_request(channel, (char *)guarded + 8, 4) == TRUE);
        for (index = 0; index != sizeof(guarded); ++index)
            assert(guarded[index] == wanted[index]);
        if (type == DMA_WRITE_TRANSFER) {
            /* SAS remains the authority for ROM writes in either direction.
               Reuse this private RAM bank as a temporary protected fixture. */
            sas_connect_memory(0x40000, 0x40000 + bytes - 1, SAS_ROM);
            outb(controller ? 0xd8 : 0x0c, 0);
            dma_word(controller ? 0xc4 : 0x02, starts[start]);
            dma_word(controller ? 0xc6 : 0x03, 3);
            outb(controller ? 0xd4 : 0x0a, 1);
            for (index = 8; index != 8 + 4 * unit; ++index)
                guarded[index] ^= 0xff;
            assert(dma_request(channel, (char *)guarded + 8, 4) == FALSE);
            sas_connect_memory(0x40000, 0x40000 + bytes - 1, SAS_RAM);
            assert(softpc_machine_read_physical(machine, 0x40000,
                actual, bytes) == SOFTPC_MACHINE_OK);
            for (index = 0; index != bytes; ++index)
                assert(actual[index] == expected[index]);
        }
    }
}

static void verify_dma(softpc_machine *machine)
{
    unsigned controller, decrement, automatic, round, index;
    for (controller = 0; controller != 2; ++controller)
    for (decrement = 0; decrement != 2; ++decrement)
    for (automatic = 0; automatic != 2; ++automatic) {
        const half_word channel = controller ? 5 : 1;
        const io_addr address_port = controller ? 0xc4 : 0x02;
        const io_addr count_port = controller ? 0xc6 : 0x03;
        const io_addr clear_port = controller ? 0xd8 : 0x0c;
        const unsigned unit = controller ? 2 : 1;
        const IU16 initial = 0x2100;
        const lib_u32 physical = (lib_u32)initial * unit;
        unsigned char memory[32], output[8];
        sys_addr address;
        word count;

        for (index = 0; index < sizeof(memory); ++index)
            memory[index] = (unsigned char)(index + 0x31);
        assert(softpc_machine_write_physical(machine, physical - 16,
            memory, sizeof(memory)) == SOFTPC_MACHINE_OK);
        dma_post();
        outb(clear_port, 0);
        dma_word(address_port, initial);
        dma_word(count_port, 3);
        outb(controller ? 0x8b : 0x83, 0);
        /* Real reads also verify the reloaded address on the second round. */
        outb(controller ? 0xd6 : 0x0b,
            (half_word)(0x49 |
                (decrement << 5) | (automatic << 4)));
        outb(controller ? 0xd4 : 0x0a, 1);
        for (round = 0; round != 2; ++round) {
            lib_memory_set(output, 0, sizeof(output));
            assert(dma_request(channel, (char *)output, 4) == FALSE);
            for (index = 0; index != 4; ++index) {
                unsigned byte;
                for (byte = 0; byte != unit; ++byte)
                    assert(output[index * unit + byte] ==
                        memory[16 + (decrement ? -index * unit : index * unit) + byte]);
            }
            dma_enquire(channel, &address, &count);
            if (automatic) {
                assert(address == physical);
                assert(count == 3);
            } else {
                assert(address == physical + (decrement ? -4 * unit : 4 * unit));
                assert(count == 0xffff);
                lib_memory_set(output, 0x5a, sizeof(output));
                /* Terminal count masks the channel: no second transfer. */
                assert(dma_request(channel, (char *)output, 4) == TRUE);
                for (index = 0; index < sizeof(output); ++index)
                    assert(output[index] == 0x5a);
                break;
            }
        }
    }
    verify_dma_transfers(machine);
}

static void verify_disk(softpc_machine *machine)
{
    /* Guest-owned INT 41h parameter table: no real disk or assets required.
       Keep the original reserved-cylinder convention; test only register split. */
    static const struct {
        IU16 cylinders;
        IU8 heads, sectors;
        IU16 high, low;
    } cases[] = {
        { 18, 4, 17, 0, 1156 },
        { 513, 4, 32, 1, 0 },
        { 1024, 16, 63, 15, 0xbc10 }
    };
    const unsigned char vector[4] = { 0, 6, 0, 0 };
    unsigned index;
    assert(softpc_machine_write_physical(machine, 0x104, vector, 4) ==
        SOFTPC_MACHINE_OK);
    for (index = 0; index < sizeof(cases) / sizeof(cases[0]); ++index) {
        unsigned char parameters[16] = { 0 };
        parameters[0] = (unsigned char)cases[index].cylinders;
        parameters[1] = (unsigned char)(cases[index].cylinders >> 8);
        parameters[2] = cases[index].heads;
        parameters[14] = cases[index].sectors;
        assert(softpc_machine_write_physical(machine, 0x600, parameters, 16) ==
            SOFTPC_MACHINE_OK);
        c_setAX(0x1500);
        c_setCX(0xffff);
        c_setDX(0x0080);
        c_setCF(1);
        assert(BIOS[BIOS_DISK_IO] == disk_io);
        BIOS[BIOS_DISK_IO]();
        assert((c_getAX() >> 8) == 3 && c_getCF() == 0);
        assert(c_getCX() == cases[index].high);
        assert(c_getDX() == cases[index].low);
    }
    /* The public dispatcher rejects absent D: before read_dasd is called. */
    c_setAX(0x1500);
    c_setCX(0x5a5a);
    c_setDX(0x0081);
    BIOS[BIOS_DISK_IO]();
    assert(c_getCF() == 1);
    assert(c_getAX() == 0x1500);
    assert(c_getCX() == 0x5a5a && c_getDX() == 0x0081);
}

static void verify_rom(softpc_machine *machine)
{
    /* Minimal option ROM: mov ax,cs; mov [0500],ax; retf. The original
       scanner calls it and returns via the existing BIOS BOP FE trampoline. */
    unsigned char rom[512] = { 0x55, 0xaa, 1, 0x8c, 0xc8,
        0xa3, 0x00, 0x05, 0xcb };
    unsigned char observed[2];
    unsigned sum = 0, index;
    for (index = 0; index != sizeof(rom) - 1; ++index)
        sum += rom[index];
    rom[511] = (unsigned char)(0u - sum);
    /* Firmware installation is a host write, not a guest bus write to ROM. */
    assert(c_GetPhyAdd(0xc8000) != NULL);
    lib_memory_copy(c_GetPhyAdd(0xc8000), rom, sizeof(rom));
    c_setSS(0);
    c_setSP(0x7000);
    c_setDS(0);
    search_for_roms();
    assert(softpc_machine_read_physical(machine, 0x500, observed, 2) ==
        SOFTPC_MACHINE_OK);
    assert(observed[0] == 0 && observed[1] == 0xc0);
}

int main(int argc, char **argv)
{
    const char *path = "device-contract.img";
    softpc_machine_options options = { path, NULL };
    softpc_machine *machine = NULL;
    unsigned char boot[512] = { 0xeb, 0xfe };
    FILE *file;
    assert(argc == 2);
    boot[510] = 0x55;
    boot[511] = 0xaa;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(boot, 1, sizeof(boot), file) == sizeof(boot));
    assert(fclose(file) == 0);
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    if (lib_text_compare(argv[1], "dma") == 0)
        verify_dma(machine);
    else if (lib_text_compare(argv[1], "rom") == 0)
        verify_rom(machine);
    else if (lib_text_compare(argv[1], "descriptor") == 0)
        verify_descriptor(machine);
    else if (lib_text_compare(argv[1], "cmos") == 0)
        verify_cmos();
    else if (lib_text_compare(argv[1], "eoi") == 0)
        verify_eoi();
    else if (lib_text_compare(argv[1], "irq") == 0)
        verify_delayed_irq();
    else if (lib_text_compare(argv[1], "sb-dma") == 0)
        verify_sb_dma();
    else if (lib_text_compare(argv[1], "sb-commands") == 0)
    {
        verify_sb_commands(machine);
        verify_sb_block(machine);
        verify_sb_completion();
        verify_sb_output();
        verify_sb_transfer(machine);
        verify_sb_transfer_failures();
        verify_sb_ports(machine);
        verify_sb_executor(machine);
        verify_sb_machine_pause(machine);
    }
    else {
        assert(lib_text_compare(argv[1], "disk") == 0);
        verify_disk(machine);
    }
    assert(softpc_machine_destroy(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_test_remove_image(path));
    puts("App SoftPC device contract: OK");
    return 0;
}
