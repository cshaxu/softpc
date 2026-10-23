/***************************************************************************
*
*    dsp.c
*
*    Copyright (c) 1991-1996 Microsoft Corporation.  All Rights Reserved.
*
*    This code provides VDD support for SB 2.0 sound output, specifically:
*        DSP 2.01+ (excluding SB-MIDI port)
*
***************************************************************************/
/* Declare the host PCM contract before the original core's const macro. */
#include "compat/sb_pcm.h"
#include "insignia.h"
#include "host_def.h"
#include "xt.h"
#include "dma.h"
#include "sas.h"
#include "sndblst.h"
#include "nt_sb.h"
#include "ica.h"
#include "compat/eoi.h"
#include "quick_ev.h"
#include "compat/devices/snapshot.h"

IU32           TimeConstant = 0xA6;    // one byte format

DMA_ADAPT    *pDmaAdp;
DMA_CNTRL    *pDcp;
IU16         Chan;

IU8            IdentByte;              // used with DSP_CARD_IDENTIFY
IU8            ReservedRegister;       // used with DSP_LOAD_RES_REG and DSP_READ_RES_REG
IU32           SBBlockSize = 0x7ff;    // Block size set by apps, always size of transfer-1
IBOOL            bDspActive = FALSE;     // dsp thread currently active
IBOOL            bDspPaused = FALSE;     // dsp paused
IBOOL            SbAnswerDMAPosition = FALSE;
IBOOL            bWriteBurstStarted = FALSE;
IBOOL            bExitDMAThread= FALSE;  // Exit DSP DMA thread flag
IBOOL            bDspReset = FALSE;
IBOOL            bExitAuto = FALSE;
IBOOL            bHighSpeedMode = FALSE; // Are we in High Speed transfer mode?
DSP_MODE DspMode;
IU32 PreviousWaveOutPos;
IU16 StartingDmaAddr, StartingDmaCount;
IU32 DspNumberDmaQueries, DspVirtualInc;
SB_DMA_INFO DmaInfo;
/* T93: copied guest block replaces native WaveHdr storage. Only the executor
 * advances its accepted prefix; DMA progress remains the original controller. */
static IU8 DspBuffer[65536];
static IU32 DspTransferSize, DspTransferQueued, DspTransferConsumed;
static q_ev_handle DspCompletionEvent;
static IU32 DspCompletionRate, DspCompletionChecks;
static lib_u64 DspHostOrigin, DspHostEpoch;
static DSP_MODE DspTransferMode;
static lib_status DspTransferStatus;
static IBOOL DspResetOutput;
static IU32 DspOutputRate;
static IBOOL DspSpeakerEnabled = TRUE;
static IBOOL DspEoiRegistered;
static IBOOL DspOutputSuspended;
/* T93: parser-owned partial commands must survive a portable capture. */
static IU32 blockSize;
static IU8 TableMungeData;
static IBOOL TableMungeFirstByte = TRUE;
static void DspCompleteBlock(long unused);
static void AutoInitEoiHook(int irq, int count);

static void ExitAutoMode(void)
{
    DspMode = None;
    bExitAuto = FALSE;
    if (DspEoiRegistered) {
        RemoveEOIHook(SbInterrupt, AutoInitEoiHook);
        DspEoiRegistered = FALSE;
    }
}

void StopDspTransfer(IBOOL reset)
{
    lib_status status;
    /* Remove the owner before PIC withdrawal can invoke its EOI callback. */
    ExitAutoMode();
    DspCancelTransfer();
    host_DelayHwInterrupt(SbInterrupt, 0, 0xfffffffful);
    ica_clear_int(ICA_MASTER, SbInterrupt);
    status = reset ? DspCloseOutput() :
        (DspOutputRate != 0 ? softpc_sb_pcm_clear() : LIB_STATUS_OK);
    DspTransferStatus = status;
    DspResetOutput = FALSE;
    if (reset) bDspActive = FALSE;
}

lib_status DspGetStatus(void)
{
    return DspTransferStatus;
}

lib_status DspServiceTransfer(void)
{
    if (DspTransferStatus != LIB_STATUS_OK) return DspTransferStatus;
    /* T95: returning host capacity must not resume a guest-paused DMA block. */
    if (DspOutputSuspended || bDspPaused || !bDspActive || DspTransferSize == 0 || DspCompletionEvent != 0)
        return LIB_STATUS_OK;
    DspTransferStatus = DspProcessBlock();
    if (DspTransferStatus == LIB_STATUS_OK && bWriteBurstStarted) {
        DspTransferStatus = DspArmCompletion(DspOutputRate);
        if (DspTransferStatus == LIB_STATUS_OK && DspMode == Auto && bExitAuto)
            ExitAutoMode();
    }
    return DspTransferStatus;
}

IBOOL StartDspTransfer(DSP_MODE mode)
{
    lib_status status;
    if ((mode != Single && mode != Auto) || DspTransferStatus != LIB_STATUS_OK)
        return FALSE;
    if (!bDspActive) bExitAuto = FALSE;
    if (mode == Single) ExitAutoMode();
    DspCancelTransfer();
    status = DspPrepareOutput();
    if (status == LIB_STATUS_OK && (mode == Single || DspMode != Auto))
        status = softpc_sb_pcm_clear();
    if (status == LIB_STATUS_OK && mode == Auto && !DspEoiRegistered) {
        if (RegisterEOIHook(SbInterrupt, AutoInitEoiHook)) DspEoiRegistered = TRUE;
        else status = LIB_STATUS_INVALID_STATE;
    }
    if (status != LIB_STATUS_OK) {
        StopDspTransfer(TRUE);
        DspTransferStatus = status;
        return FALSE;
    }
    bDspActive = TRUE;
    bDspPaused = FALSE;
    DspMode = mode;
    /* A masked or exhausted DMA channel supplies no block, as in XP. */
    if (DmaDataToDsp(mode)) return DspServiceTransfer() == LIB_STATUS_OK;
    return TRUE;
}

static void AutoInitEoiHook(int irq, int count)
{
    if (irq != SbInterrupt || count < 0 || DspMode != Auto ||
        bDspPaused || bDspReset || DspTransferStatus != LIB_STATUS_OK ||
        DspTransferSize != 0) return;
    DspTransferStatus = DspPrepareOutput();
    if (DspTransferStatus == LIB_STATUS_OK && DmaDataToDsp(Auto))
        DspServiceTransfer();
}

/* T93: retain one output owner and the current format, replacing WaveOut's
 * handle/format and device-wide volume with a stream-local PCM contract. */
lib_status DspCloseOutput(void)
{
    lib_status status = softpc_sb_pcm_close();
    if (status == LIB_STATUS_OK) {
        DspOutputRate = 0;
        DspSpeakerEnabled = TRUE;
        DspOutputSuspended = FALSE;
    }
    return status;
}

/* T93: only the executor parks this device. Keep the frozen guest block;
 * inactive host output discards samples, not guest DMA/IRQ obligations. */
void DspSetRunning(IBOOL running)
{
    lib_audio_pcm_progress progress;
    lib_u64 origin, played;
    IU32 queued;
    if (DspOutputRate == 0 || DspTransferStatus != LIB_STATUS_OK ||
        DspOutputSuspended == !running) return;
    if (!running) {
        queued = DspTransferQueued - DspTransferConsumed;
        if (queued != 0) {
            DspTransferStatus = softpc_sb_pcm_progress(&progress);
            if (DspTransferStatus != LIB_STATUS_OK) return;
            if (progress.accepted_frames < queued ||
                (DspCompletionEvent != 0 && progress.epoch != DspHostEpoch)) {
                DspTransferStatus = LIB_STATUS_INVALID_STATE;
                return;
            }
            origin = progress.accepted_frames - queued;
            played = progress.consumed_frames > origin ?
                progress.consumed_frames - origin : 0;
            DspTransferConsumed += played < queued ? (IU32)played : queued;
        }
        DspTransferStatus = softpc_sb_pcm_set_active(LIB_FALSE);
        if (DspTransferStatus != LIB_STATUS_OK) return;
        if (DspCompletionEvent != 0) delete_q_event(DspCompletionEvent);
        DspCompletionEvent = 0;
        DspTransferQueued = DspTransferConsumed;
        bWriteBurstStarted = FALSE;
        DspOutputSuspended = TRUE;
    } else {
        DspTransferStatus = softpc_sb_pcm_set_active(LIB_TRUE);
        if (DspTransferStatus != LIB_STATUS_OK) return;
        DspOutputSuspended = FALSE;
        DspServiceTransfer();
    }
}

lib_status DspPrepareOutput(void)
{
    IU32 rate = TimeConstant == 0xffff ? DspOutputRate : GetSamplingRate();
    lib_status status;
    if (rate == 0) return LIB_STATUS_INVALID_STATE;
    if (DspTransferStatus != LIB_STATUS_OK) return DspTransferStatus;
    if (rate != DspOutputRate) {
        /* Also resolves a retained resource from a failed host open. */
        status = DspCloseOutput();
        if (status != LIB_STATUS_OK) return status;
        status = softpc_sb_pcm_open(rate, MixerGetVoiceGain());
        if (status != LIB_STATUS_OK) return status;
        DspOutputRate = rate;
    }
    TimeConstant = 0xffff;
    if (DspResetOutput) {
        status = softpc_sb_pcm_clear();
        if (status != LIB_STATUS_OK) return status;
        DspResetOutput = FALSE;
    }
    return LIB_STATUS_OK;
}

void SbApplyMixer(void)
{
    MidiApplyMixer();
    if (DspOutputRate != 0 && DspTransferStatus == LIB_STATUS_OK)
        DspTransferStatus = softpc_sb_pcm_set_gain(
            DspSpeakerEnabled ? MixerGetVoiceGain() : 0);
}

void SetSpeaker(IBOOL on)
{
    /* XP ignores speaker commands until an output device exists. */
    if (DspOutputRate == 0) return;
    DspSpeakerEnabled = on;
    SbApplyMixer();
}

static lib_status DspScheduleCompletion(IU32 samples_minus_one)
{
    lib_u64 delay = ((1000ull * (samples_minus_one + 1ull)) /
        DspCompletionRate + 1ull) * 1000ull;
    if (delay > 0xffffffffull) return LIB_STATUS_LIMIT_EXCEEDED;
    DspCompletionEvent = add_q_event_t(DspCompleteBlock, (unsigned long)delay, 0);
    if (DspCompletionEvent == 0 || DspCompletionEvent == 0xffff) {
        DspCompletionEvent = 0;
        return LIB_STATUS_IO_ERROR;
    }
    return LIB_STATUS_OK;
}

lib_status DspArmCompletion(IU32 rate)
{
    lib_audio_pcm_progress progress;
    lib_status status;
    IU32 size;
    if (!bWriteBurstStarted || DspTransferSize == 0 || DspCompletionEvent != 0)
        return LIB_STATUS_INVALID_STATE;
    if (rate == 0) return LIB_STATUS_INVALID_ARGUMENT;
    status = softpc_sb_pcm_progress(&progress);
    if (status != LIB_STATUS_OK) return status;
    size = DspTransferSize - DspTransferConsumed;
    if (progress.accepted_frames < size) return LIB_STATUS_INVALID_STATE;
    DspHostOrigin = progress.accepted_frames - size;
    DspHostEpoch = progress.epoch;
    DspCompletionRate = rate;
    DspCompletionChecks = PreviousWaveOutPos = 0;
    size = size != 0 ? size - 1 : 0;
    if (DspTransferMode == Auto) {
        if (size < 0x400) size += size / 8;
        else if (size > 0x1800) size -= AUTO_BLOCK_SIZE;
    } else {
        bHighSpeedMode = FALSE;
    }
    return DspScheduleCompletion(size);
}

void DspCancelTransfer(void)
{
    if (DspCompletionEvent != 0) delete_q_event(DspCompletionEvent);
    DspCompletionEvent = 0;
    DspTransferSize = DspTransferQueued = DspTransferConsumed = 0;
    DspTransferStatus = LIB_STATUS_OK;
    SbAnswerDMAPosition = bWriteBurstStarted = FALSE;
}

static void DspCompleteBlock(long unused)
{
    lib_audio_pcm_progress progress;
    lib_u64 played;
    IU32 remaining;
    UNUSED(unused);
    DspCompletionEvent = 0;
    if (DspTransferStatus != LIB_STATUS_OK) return;
    DspTransferStatus = softpc_sb_pcm_progress(&progress);
    if (DspTransferStatus != LIB_STATUS_OK) return;
    if (progress.epoch != DspHostEpoch) {
        DspTransferStatus = LIB_STATUS_INVALID_STATE;
        return;
    }
    /* Original timeout policy: physical lag matters when DMA was queried.
     * Clamp to this copied range, not a different stream epoch or block. */
    if (PreviousWaveOutPos != 0) {
        played = progress.consumed_frames > DspHostOrigin ?
            progress.consumed_frames - DspHostOrigin : 0;
        played += DspTransferConsumed;
        remaining = played >= DspTransferSize ? 0 : DspTransferSize - (IU32)played;
        if (remaining > (DspTransferMode == Single ? 4u : 0x20u)) {
            if (++DspCompletionChecks < 10) {
                DspTransferStatus = DspScheduleCompletion(remaining);
                return;
            }
            DspResetOutput = TRUE;
        }
    }
    DmaInfo.count = (IU16)(StartingDmaCount - DspTransferSize);
    DmaInfo.addr = (IU16)((DmaInfo.mode & 0x20) ?
        StartingDmaAddr - DspTransferSize : StartingDmaAddr + DspTransferSize);
    SetDMACountAddr(&DmaInfo);
    if (DmaInfo.count == 0xffff) SetDMAStatus(&DmaInfo, TRUE, TRUE);
    GenerateInterrupt(0);
    SbAnswerDMAPosition = bWriteBurstStarted = FALSE;
    DspTransferSize = DspTransferQueued = DspTransferConsumed = 0;
}

IBOOL DmaDataToDsp(DSP_MODE mode)
{
    IU32 i;
    IU16 address;
    if (DspCompletionEvent != 0) return FALSE;
    if (mode == Auto && bDspReset) return FALSE;
    QueryDMA(&DmaInfo);
    if (DmaInfo.count == 0xffff || DmaInfo.count == 0 ||
        (DmaInfo.mask & (1u << SbDmaChannel))) return FALSE;
    if (SBBlockSize >= sizeof(DspBuffer)) return FALSE;
    StartingDmaAddr = DmaInfo.addr;
    StartingDmaCount = DmaInfo.count;
    DspTransferSize = SBBlockSize + 1;
    DspTransferQueued = 0;
    DspTransferConsumed = 0;
    DspTransferMode = mode;
    DspTransferStatus = LIB_STATUS_OK;
    SbAnswerDMAPosition = SBBlockSize > 0xff;
    bWriteBurstStarted = FALSE;
    address = StartingDmaAddr;
    for (i = 0; i < DspTransferSize; ++i) {
        /* T94: DMA addresses are physical, independent of guest paging. */
        sas_PRS(dma_system_address(SbDmaChannel, DmaInfo.page, address), &DspBuffer[i], 1);
        address = (IU16)(address + ((DmaInfo.mode & 0x20) ? -1 : 1));
    }

    /* Original DspProcessBlock query-count adaptation, before host submission. */
    if (DspNumberDmaQueries == 0) {
        DspVirtualInc = DspTransferSize >> 4;
    } else {
        DspVirtualInc = DspTransferSize / DspNumberDmaQueries;
        if (DspTransferSize > 0x400) {
            if (DspVirtualInc < 5) DspVirtualInc = 5;
            else if (DspVirtualInc > 0x200) DspVirtualInc = 0x200;
        } else if (DspVirtualInc > DspTransferSize / 4) {
            DspVirtualInc = DspTransferSize / 4;
        }
    }
    DspNumberDmaQueries = 0;
    return TRUE;
}

lib_status DspProcessBlock(void)
{
    if (DspTransferStatus != LIB_STATUS_OK) return DspTransferStatus;
    while (DspTransferQueued < DspTransferSize) {
        IU32 count = DspTransferSize - DspTransferQueued;
        lib_u32 accepted = 0;
        lib_status status;
        if (count > LIB_AUDIO_PCM_STREAM_MAX_FRAMES_PER_SUBMISSION)
            count = LIB_AUDIO_PCM_STREAM_MAX_FRAMES_PER_SUBMISSION;
        status = softpc_sb_pcm_write(DspBuffer + DspTransferQueued, count, &accepted);
        if (accepted > count) return LIB_STATUS_IO_ERROR;
        DspTransferQueued += accepted;
        if (status == LIB_STATUS_LIMIT_EXCEEDED) return LIB_STATUS_OK;
        if (status != LIB_STATUS_OK) return status;
        if (accepted == 0) return LIB_STATUS_IO_ERROR;
    }
    if (DspTransferSize != 0) bWriteBurstStarted = TRUE;
    return LIB_STATUS_OK;
}

//
//
// DSP State Machines
//

//
// Reset State Machine
//

enum {
    ResetNotStarted = 1,
    Reset1Written
}
ResetState = ResetNotStarted; // state of current reset

//
// Write State Machine
//

enum {
    WriteCommand = 1, // Initial state and after reset
    CardIdent,
    TableMunge,
    LoadResReg,
    SetTimeConstant,
    BlockSizeFirstByte,
    BlockSizeSecondByte,
    BlockSizeFirstByteWrite,
    BlockSizeSecondByteWrite,
    BlockSizeFirstByteRead,
    BlockSizeSecondByteRead,
    UnsupportedMidiByte,
    UnsupportedMidiUart
}
DSPWriteState = WriteCommand; // state of current command/data being written

//
// Read State Machine
//

enum {
    NothingToRead = 1, // initial state and after reset
    Reset,
    FirstVersionByte,
    SecondVersionByte,
    ReadIdent,
    ReadResReg
}
DSPReadState = NothingToRead; // state of current command/data being read

/* T93: archive only device values. Native counters are reduced to the consumed
 * prefix of the frozen block; restoration starts a new paused host output. */
int softpc_device_snapshot_capture_sb_dsp(softpc_device_sb_dsp_state *state)
{
    lib_audio_pcm_progress progress;
    lib_u64 origin, played;
    IU32 queued = DspTransferQueued - DspTransferConsumed;
    if (DspTransferStatus != LIB_STATUS_OK) return FALSE;
    state->TimeConstant = TimeConstant;
    state->SBBlockSize = SBBlockSize;
    state->blockSize = blockSize;
    state->DspTransferSize = DspTransferSize;
    state->DspTransferConsumed = DspTransferConsumed;
    state->DspCompletionEvent = DspCompletionEvent;
    state->DspOutputRate = DspOutputRate;
    state->PreviousWaveOutPos = PreviousWaveOutPos;
    state->DspNumberDmaQueries = DspNumberDmaQueries;
    state->DspVirtualInc = DspVirtualInc;
    state->StartingDmaAddr = StartingDmaAddr;
    state->StartingDmaCount = StartingDmaCount;
    state->IdentByte = IdentByte;
    state->ReservedRegister = ReservedRegister;
    state->TableMungeData = TableMungeData;
    state->TableMungeFirstByte = TableMungeFirstByte;
    state->ResetState = ResetState;
    state->DSPWriteState = DSPWriteState;
    state->DSPReadState = DSPReadState;
    state->DspMode = DspMode;
    state->DspTransferMode = DspTransferMode;
    state->bDspActive = bDspActive;
    state->bDspPaused = bDspPaused;
    state->SbAnswerDMAPosition = SbAnswerDMAPosition;
    state->bExitDMAThread = bExitDMAThread;
    state->bDspReset = bDspReset;
    state->bExitAuto = bExitAuto;
    state->bHighSpeedMode = bHighSpeedMode;
    state->DspSpeakerEnabled = DspSpeakerEnabled;
    state->DspEoiRegistered = DspEoiRegistered;
    state->dma_addr = DmaInfo.addr;
    state->dma_count = DmaInfo.count;
    state->dma_page = DmaInfo.page;
    state->dma_status = DmaInfo.status;
    state->dma_mode = DmaInfo.mode;
    state->dma_mask = DmaInfo.mask;
    memcpy(state->buffer, DspBuffer, sizeof(DspBuffer));
    if (queued) {
        if (softpc_sb_pcm_progress(&progress) != LIB_STATUS_OK ||
            progress.accepted_frames < queued ||
            (DspCompletionEvent && progress.epoch != DspHostEpoch)) return FALSE;
        origin = progress.accepted_frames - queued;
        played = progress.consumed_frames > origin ? progress.consumed_frames - origin : 0;
        state->DspTransferConsumed += played < queued ? (IU32)played : queued;
    }
    return TRUE;
}

int softpc_device_snapshot_restore_sb_dsp(const softpc_device_sb_dsp_state *state)
{
    if ((state->TimeConstant > 255 && state->TimeConstant != 0xffff) ||
        state->SBBlockSize >= sizeof(DspBuffer) || state->blockSize > 0xffff ||
        state->DspTransferSize > sizeof(DspBuffer) ||
        state->DspTransferConsumed > state->DspTransferSize ||
        state->DspCompletionEvent > 0xfffe ||
        (state->DspOutputRate && (state->DspOutputRate < 3921 || state->DspOutputRate > 44100)) ||
        (state->DspTransferSize && !state->DspOutputRate) ||
        state->ResetState < ResetNotStarted || state->ResetState > Reset1Written ||
        state->DSPWriteState < WriteCommand || state->DSPWriteState > UnsupportedMidiUart ||
        state->DSPReadState < NothingToRead || state->DSPReadState > ReadResReg ||
        state->DspMode > None || state->DspTransferMode > None ||
        state->TableMungeFirstByte > 1 || state->bDspActive > 1 ||
        state->bDspPaused > 1 || state->SbAnswerDMAPosition > 1 ||
        state->bExitDMAThread > 1 || state->bDspReset > 1 ||
        state->bExitAuto > 1 || state->bHighSpeedMode > 1 ||
        state->DspSpeakerEnabled > 1 || state->DspEoiRegistered > 1 ||
        (state->DspEoiRegistered && state->DspMode != Auto)) return FALSE;
    /* The archive prepares old output before restoring PIC and event queues.
     * Retire only this saved host-progress event from the restored queue. */
    if (state->DspCompletionEvent)
        delete_q_event((q_ev_handle)state->DspCompletionEvent);
    TimeConstant = state->TimeConstant;
    SBBlockSize = state->SBBlockSize;
    blockSize = state->blockSize;
    DspTransferSize = state->DspTransferSize;
    DspTransferConsumed = state->DspTransferConsumed;
    PreviousWaveOutPos = state->PreviousWaveOutPos;
    DspNumberDmaQueries = state->DspNumberDmaQueries;
    DspVirtualInc = state->DspVirtualInc;
    StartingDmaAddr = state->StartingDmaAddr;
    StartingDmaCount = state->StartingDmaCount;
    IdentByte = state->IdentByte;
    ReservedRegister = state->ReservedRegister;
    TableMungeData = state->TableMungeData;
    TableMungeFirstByte = state->TableMungeFirstByte;
    ResetState = state->ResetState;
    DSPWriteState = state->DSPWriteState;
    DSPReadState = state->DSPReadState;
    DspMode = state->DspMode;
    DspTransferMode = state->DspTransferMode;
    bDspActive = state->bDspActive;
    bDspPaused = state->bDspPaused;
    SbAnswerDMAPosition = state->SbAnswerDMAPosition;
    bExitDMAThread = state->bExitDMAThread;
    bDspReset = state->bDspReset;
    bExitAuto = state->bExitAuto;
    bHighSpeedMode = state->bHighSpeedMode;
    DspSpeakerEnabled = state->DspSpeakerEnabled;
    DmaInfo.addr = state->dma_addr;
    DmaInfo.count = state->dma_count;
    DmaInfo.page = state->dma_page;
    DmaInfo.status = state->dma_status;
    DmaInfo.mode = state->dma_mode;
    DmaInfo.mask = state->dma_mask;
    memcpy(DspBuffer, state->buffer, sizeof(DspBuffer));
    if (!SbInitializeDMA()) return FALSE;
    DspTransferQueued = DspTransferConsumed;
    DspCompletionEvent = 0;
    DspCompletionRate = DspCompletionChecks = 0;
    DspHostOrigin = DspHostEpoch = 0;
    bWriteBurstStarted = DspResetOutput = FALSE;
    DspOutputRate = state->DspOutputRate;
    DspOutputSuspended = DspOutputRate != 0;
    DspTransferStatus = LIB_STATUS_OK;
    if (DspOutputRate) {
        DspTransferStatus = softpc_sb_pcm_open(DspOutputRate,
            DspSpeakerEnabled ? MixerGetVoiceGain() : 0);
        if (DspTransferStatus == LIB_STATUS_OK)
            DspTransferStatus = softpc_sb_pcm_set_active(LIB_FALSE);
        if (DspTransferStatus != LIB_STATUS_OK) return FALSE;
    }
    DspEoiRegistered = FALSE;
    if (state->DspEoiRegistered) {
        if (!RegisterEOIHook(SbInterrupt, AutoInitEoiHook)) return FALSE;
        DspEoiRegistered = TRUE;
    }
    return TRUE;
}

int softpc_device_snapshot_encode_sb_callback(Q_CALLBACK_FN callback,
    unsigned long *callback_id)
{
    if (callback != DspCompleteBlock) return FALSE;
    *callback_id = SOFTPC_DEVICE_QUEUE_SB_COMPLETION;
    return TRUE;
}

Q_CALLBACK_FN softpc_device_snapshot_decode_sb_callback(unsigned long callback_id)
{
    return callback_id == SOFTPC_DEVICE_QUEUE_SB_COMPLETION ? DspCompleteBlock : NULL;
}

void
GenerateInterrupt(
    IU32 delay
    )

/*++

Routine Description:

    This function generates device interrupt on dma channel SM_INTERRUPT
    on ICA_MASTER device.  The interrupt will be dispatched before the control
    returns to the emulation thread.

Arguments:

    delay - specifies the delay to generate delayed interrupt.

Return Value:

    None.

--*/

{
    //
    // Generate an interrupt on the master controller
    //

    if (delay == 0) {
        ica_hw_interrupt(ICA_MASTER, SbInterrupt, 1);
    } else {
        host_DelayHwInterrupt(SbInterrupt, 1, delay * 1000);
    }
}

void
SbGetDMAPosition(
    void
    )

/*++

Routine Description:

    This function is the handler for the SB DMA position IO read instruction.  It
    maintains s real waveout position by calling MM api to get the current play back
    position.  It also maintains a virtual waveout position to simulate sound playing
    position in case real sound is not played or too slow.

Arguments:

    None.

Return Value:

    None.

--*/

{
    IU32 offset;       // virtual progress within the current DMA block

    /* T93: this runs on the executor; an unsubmitted block cannot spin here. */
    if (bDspActive && SbAnswerDMAPosition && bWriteBurstStarted) {
        DspNumberDmaQueries++;
        /* T93: distance is modulo the 16-bit address, in DMA direction. */
        offset = (IU16)((DmaInfo.mode & 0x20) ?
            StartingDmaAddr - DmaInfo.addr : DmaInfo.addr - StartingDmaAddr);
        offset += DspVirtualInc;
        if (SBBlockSize < 0x400) {

            //
            // If block size is smal, do not call GetWaveOutPosition and
            // do not update PreviousWaveOutPos .
            //

            if (offset < (SBBlockSize * 3 / 4)) {
                if (DmaInfo.mode & 0x20)
                    DmaInfo.addr -= (IU16)DspVirtualInc;
                else
                    DmaInfo.addr += (IU16)DspVirtualInc;
                DmaInfo.count -= (IU16)DspVirtualInc;
                SetDMACountAddr(&DmaInfo);
            }
            return;
        }
        if (DspMode == Single) {
            PreviousWaveOutPos++;
        }


        //
        // The algorithm used here is weird but it seems to work.
        // We will come back and revise it later.
        //

        //
        // Virtual sound is approaching to the end.  Slow down
        //

        if (offset >= (SBBlockSize - 0x50)) {
            DspVirtualInc = 2;
        }

        //
        // Put a limit so we don't overflow the virtual sound position
        //

        if (offset >= SBBlockSize - 0x8) {
            offset = SBBlockSize - 0x8;
        }

        //
        // Now update the dma controller with our emulation/real position
        //

        DmaInfo.addr = (DmaInfo.mode & 0x20) ?
            StartingDmaAddr - (IU16)offset : StartingDmaAddr + (IU16)offset;
        DmaInfo.count = StartingDmaCount - (IU16)offset;

        SetDMACountAddr(&DmaInfo);


    }
}

/* T93: keep the original controller owner; no VDD handle or host pointer. */
IBOOL SbInitializeDMA(void)
{
    if (SbDmaChannel >= DMA_CONTROLLER_CHANNELS) return FALSE;
    pDmaAdp = dmaGetAdaptor();
    pDcp = &pDmaAdp->controller[dma_physical_controller(SbDmaChannel)];
    Chan = dma_physical_channel(SbDmaChannel);
    return TRUE;
}

void
DspReadStatus(
    IU8 * data
    )

/*++

Routine Description:

    This function determines the status based on the read state machine and
    returns either Ready or Busy.

Arguments:

    data - supplies a pointer to a byte to receive the status.

Return Value:

    None.

--*/

{
    //
    // See if we think there is something to read
    //

    *data = DSPReadState != NothingToRead ? 0xFF : 0x7F;
}

void
DspReadData(
    IU8 * data
    )

/*++

Routine Description:

    This function returns the desired data based on the read state machine and
    updates the read state machine.

Arguments:

    data - supplies a pointer to a byte to receive the data.

Return Value:

    None.

--*/

{
    switch (DSPReadState) {
    case NothingToRead:
        *data = 0xFF;
        break;

    case Reset:
        *data = 0xAA;
        DSPReadState = NothingToRead;
        break;

    case FirstVersionByte:
        *data = (IU8)(SB_VERSION / 256);
        DSPReadState = SecondVersionByte;
        break;

    case SecondVersionByte:
        *data = (IU8)(SB_VERSION % 256);
        DSPReadState = NothingToRead;
        break;

    case ReadIdent:
        *data = ~IdentByte;
        DSPReadState = NothingToRead;
        break;

    case ReadResReg:
        *data = ReservedRegister;
        DSPReadState = NothingToRead;
        break;

    default:
        break;
    }

}

void
DspResetWrite(
    IU8 data
    )

/*++

Routine Description:

    This function resets the sound blaster.  If reset was received
    in HighSpeed mode, the reset is used to exit high speed mode.

Arguments:

    data - supplies data to control how the reset should be done.

Return Value:

    None.

--*/

{
    if (data == 1) {
        ResetState = Reset1Written;
    } else if (ResetState == Reset1Written && data == 0) {
        ResetState = ResetNotStarted;
        bDspReset = TRUE;

        //
        // Some games reset DSP on every single cycle out.
        //

        if (!bHighSpeedMode) {
            bExitDMAThread = TRUE;
        }
        ResetAll(); // OK - reset everything
    }
}

void
DspWrite(
    IU8 data
    )

/*++

Routine Description:

    This function handles apps write data to dsp write port.

Arguments:

    data - supplies data to write to dsp write port.

Return Value:

    None.

--*/

{
    switch (DSPWriteState) {
    case WriteCommand:
        WriteCommandByte(data);
        break;

    case UnsupportedMidiByte:
        DSPWriteState = WriteCommand;
        break;

    case UnsupportedMidiUart:
        break;

    case CardIdent:
        IdentByte = data;
        DSPReadState = ReadIdent;
        DSPWriteState = WriteCommand;
        break;

    case TableMunge:
        TableMunger(data);
        DSPWriteState = WriteCommand;
        break;

    case LoadResReg:
        ReservedRegister = data;
        DSPWriteState = WriteCommand;
        break;

    case SetTimeConstant:
        TimeConstant =  (IU32)data;
        DSPWriteState = WriteCommand;
        break;

    case BlockSizeFirstByte:
        blockSize = (IU32)data;
        DSPWriteState = BlockSizeSecondByte;
        break;

    case BlockSizeSecondByte:
        SBBlockSize = blockSize + (((IU32)data) << 8);

        DSPWriteState = WriteCommand;
        break;

    case BlockSizeFirstByteWrite:
        blockSize = (IU32)data;
        DSPWriteState = BlockSizeSecondByteWrite;
        break;

    case BlockSizeSecondByteWrite:
        SBBlockSize = blockSize + (((IU32)data) << 8);

        DSPWriteState = WriteCommand;
        if (SBBlockSize <= 0x10) {

            //
            // this is a hack to convince some apps a sb exists
            //

            if (SBBlockSize > 0) {
                SB_DMA_INFO dmaInfo;

                QueryDMA(&dmaInfo);
                dmaInfo.count -= (IU16)(SBBlockSize + 1);
                /* T93: honor the existing controller's decrement bit. */
                if (dmaInfo.mode & 0x20)
                    dmaInfo.addr -= (IU16)(SBBlockSize + 1);
                else
                    dmaInfo.addr += (IU16)(SBBlockSize + 1);

                SetDMACountAddr(&dmaInfo);

                if (dmaInfo.count == 0xffff) {
                    SetDMAStatus(&dmaInfo, TRUE, TRUE);
                }
            }
            GenerateInterrupt(2);  // 2ms to play the 0x10 bytes data
            break;
        }
        StartDspTransfer(Single);
        break;

    case BlockSizeFirstByteRead:
        blockSize = (IU32)data;
        DSPWriteState = BlockSizeSecondByteRead;
        break;

    case BlockSizeSecondByteRead:
        SBBlockSize = blockSize + (((IU32)data) << 8);

        DSPWriteState = WriteCommand;
        // this is a hack to convince some apps a sb exists
        if (SBBlockSize <= 0x10) {
            IU32 dMAPhysicalAddress;

            if ((dMAPhysicalAddress=GetDMATransferAddress()) != -1L) {
                IU8 silence = 0x80;
                sas_PWS(dMAPhysicalAddress, &silence, 1);
            }
            GenerateInterrupt(2);
        }
        // NOt implemented
        break;

    default:
        break;
    }
}

void
WriteCommandByte(
    IU8 command
    )

/*++

Routine Description:

    This function handles command sent to DSP. Mainly, it dispatches to
    its worker functions.

Arguments:

    command - supplies command.

Return Value:

    None.

--*/

{
    switch (command) {
    /* T99 S11: consume unsupported MIDI framing, never PCM commands. */
    case MIDI_WRITE_POLL:
        DSPWriteState = UnsupportedMidiByte;
        break;

    case MIDI_READ_POLL_WRITE_POLL_UART:
    case MIDI_READ_INTERRUPT_WRITE_POLL_UART:
    case MIDI_READ_TIMESTAMP_POLL_WRITE_POLL_UART:
    case MIDI_READ_TIMESTAMP_INTERRUPT_WRITE_POLL_UART:
        DSPWriteState = UnsupportedMidiUart;
        break;

    case DSP_GET_VERSION:
        DSPReadState = FirstVersionByte;
        break;

    case DSP_CARD_IDENTIFY:
        DSPWriteState = CardIdent;
        break;

    case DSP_TABLE_MUNGE:
        DSPWriteState = TableMunge;
        break;

    case DSP_LOAD_RES_REG:
        DSPWriteState = LoadResReg;
        break;

    case DSP_READ_RES_REG:
        DSPReadState = ReadResReg;
        break;

    case DSP_GENERATE_INT:
        GenerateInterrupt(1);
        break;

    case DSP_SPEAKER_ON:
        SetSpeaker(TRUE);
        break;

    case DSP_SPEAKER_OFF:
        SetSpeaker(FALSE);
        break;

    case DSP_SET_SAMPLE_RATE:
        DSPWriteState = SetTimeConstant;
        break;

    case DSP_SET_BLOCK_SIZE:
        DSPWriteState =  BlockSizeFirstByte;
        break;

    case DSP_PAUSE_DMA:
        PauseDMA();
        break;

    case DSP_CONTINUE_DMA:
        ContinueDMA();
        break;

    case DSP_STOP_AUTO:
        bExitAuto = TRUE;
        //ExitAutoMode();
        break;

    case DSP_WRITE:
        DSPWriteState = BlockSizeFirstByteWrite;
        break;

    case DSP_WRITE_HS:
        bHighSpeedMode = TRUE;
        StartDspTransfer(Single);
        break;

    case DSP_WRITE_AUTO:
        if (SBBlockSize <= 0x10) {

            //
            // this is a hack to convince some apps a sb exists
            //

            if (SBBlockSize > 0) {
                SB_DMA_INFO dmaInfo;

                QueryDMA(&dmaInfo);
                dmaInfo.count -= (IU16)(SBBlockSize + 1);
                /* T93: honor the existing controller's decrement bit. */
                if (dmaInfo.mode & 0x20)
                    dmaInfo.addr -= (IU16)(SBBlockSize + 1);
                else
                    dmaInfo.addr += (IU16)(SBBlockSize + 1);

                SetDMACountAddr(&dmaInfo);

                if (dmaInfo.count == 0xffff) {
                    SetDMAStatus(&dmaInfo, TRUE, TRUE);
                }
            }
            GenerateInterrupt(2);  // 2ms to play the 0x10 bytes
            break;
        }
        StartDspTransfer(Auto);
        break;

    case DSP_WRITE_HS_AUTO:
        bHighSpeedMode = TRUE;
        StartDspTransfer(Auto);
        break;

    case DSP_READ:
        DSPWriteState = BlockSizeFirstByteRead;
        break;

    default:
        break;
    }
}
/* T93: the existing executor stops the transfer; native worker/event
 * ownership remains in Lib. Keep the original ordinary/high-speed reset split. */
void ResetDSP(void)
{
    StopDspTransfer(bExitDMAThread);
    if (bExitDMAThread) {
        TimeConstant = 0xA6;
        bDspActive = FALSE;
        bExitDMAThread = FALSE;
    }
    DspMode = None;
    SBBlockSize = 0x7ff;
    bDspPaused = FALSE;
    bDspReset = FALSE;
    DSPReadState = Reset;
    DSPWriteState = WriteCommand;
    ResetState = ResetNotStarted;
    SbAnswerDMAPosition = FALSE;
    bHighSpeedMode = FALSE;
}

void
TableMunger(
    IU8 data
    )

/*++

Routine Description:

    This function munges (changes) a jump table in apps code,
    Algorithm from sbvirt.asm in MMSNDSYS.

Arguments:

    data - supplies data.

Return Value:

    None.

--*/

{
    IU8 comp, dataCopy;
    SB_DMA_INFO dMAInfo;
    IU32 dMAPhysicalAddress;

    if (TableMungeFirstByte) {
        dataCopy = data;
        dataCopy = dataCopy & 0x06;
        dataCopy = dataCopy << 1;
        if (data & 0x10) {
            comp = 0x40;
        } else {
            comp = 0x20;
        }
        comp = comp - dataCopy;
        data = data + comp;
        TableMungeData = data;

        // Update memory (code table) with munged data
        if ((dMAPhysicalAddress=GetDMATransferAddress()) == -1L) {
            return;
        }
        sas_PWS(dMAPhysicalAddress, &data, 1);

        // Update virtual DMA status
        QueryDMA(&dMAInfo);
        dMAInfo.count = dMAInfo.count - 1;
        /* T93: this transfer obeys the same controller direction as PCM. */
        dMAInfo.addr += (dMAInfo.mode & 0x20) ? -1 : 1;
        SetDMACountAddr(&dMAInfo);
        TableMungeFirstByte = FALSE;
    } else {
        data = data ^ 0xA5;
        data = data + TableMungeData;
        TableMungeData = data;

        // Update memory (code table) with munged data
        if ((dMAPhysicalAddress=GetDMATransferAddress()) == -1L) {
            return;
        }
        sas_PWS(dMAPhysicalAddress, &data, 1);

        // Update virtual DMA status
        QueryDMA(&dMAInfo);
        dMAInfo.count = dMAInfo.count - 1;
        dMAInfo.addr += (dMAInfo.mode & 0x20) ? -1 : 1;
        SetDMACountAddr(&dMAInfo);
        if (dMAInfo.count==0xFFFF) {
            SetDMAStatus(&dMAInfo, FALSE, TRUE);
        }
        TableMungeFirstByte = TRUE;
    }
}

IU32
GetSamplingRate(
    void
    )

/*++

Routine Description:

    This function gets sampling rate from time constant.
    Sampling rate = 1000000 / (256 - Time constant)

Arguments:

    None.

Return Value:

    Sampling rate.

--*/

{
    IU32 samplingRate;

    if (TimeConstant == 0) {
        TimeConstant = 1;
    }
    if (TimeConstant > 0xea) {
        TimeConstant = 0xea;
    }

    switch (TimeConstant) {

    //
    // Now we set all time constants that set sample rates that
    // are just above and just below the standard sample rates
    // to the standard rates.  This will prevent unnecessary
    // sample rate conversions/searches.
    //

    //
    // All of these can be interpreted as "8000"
    //

    case 0x82: // 7936 Hz
    case 0x83: // 8000
    case 0x84: // 8065 Hz
        samplingRate = 8000;
        break;

    //
    // Both of these can be interpreted as "11025"
    //

    case 0xA5: // 10989
    case 0xA6: // 11111 Hz
        samplingRate = 11025;
        break;

    //
    // Both of these can be interpreted as "22050"
    //

    case 0xD2: // 21739 Hz
    case 0xD3: // 22222 Hz
        samplingRate = 22050;
        break;

    //
    // Both of these can be interpreted as "44100"
    //

    case 0xE9: // 43478 Hz
    case 0xEA: // 45454 Hz
        samplingRate = 44100;
        break;

    //
    // A very non standard rate is desired.  So give them what they
    // asked for.
    //

    default:
        samplingRate = 1000000 / (256 - TimeConstant);
        break;
    }
    return samplingRate;
}

IBOOL
SetDMACountAddr(
    SB_DMA_INFO * pDmaInfo
    )

/*++

Routine Description:

    This function updates DMA controller Count and Addr fields.

Arguments:

    DmaInfo - supplies a pointer to a DMA info structure

Return Value:

    TRUE - success.
    FALSE - failure

--*/
{
    pDcp->current_address[Chan][1] = (half_word)(pDmaInfo->addr >> 8);
    pDcp->current_address[Chan][0] = (half_word)pDmaInfo->addr;
    pDcp->current_count[Chan][1] = (half_word)(pDmaInfo->count >> 8);
    pDcp->current_count[Chan][0] = (half_word)pDmaInfo->count;

    //
    // If DMA count is 0xffff and autoinit is enabled, we need to
    // reload the count and address.
    //

    if ((pDcp->current_count[Chan][1] == (half_word) 0xff) &&
        (pDcp->current_count[Chan][0] == (half_word) 0xff)) {

        if (pDcp->mode[Chan].bits.auto_init != 0) {
            pDcp->current_count[Chan][0] = pDcp->base_count[Chan][0];
            pDcp->current_count[Chan][1] = pDcp->base_count[Chan][1];

            pDcp->current_address[Chan][0] = pDcp->base_address[Chan][0];
            pDcp->current_address[Chan][1] = pDcp->base_address[Chan][1];
        }
    }

    return TRUE;
}

IBOOL
QueryDMA(
    SB_DMA_INFO * DmaInfo
    )

/*++

Routine Description:

    This function retrieves virtual DMA states and returns DmaInfo.

Arguments:

    DmaInfo - supplies a pointer to a structure to receive DMA information.

Return Value:

    TRUE - success.
    FALSE - failure

--*/

{
    DmaInfo->addr  = ((IU16)pDcp->current_address[Chan][1] << 8)
                     | (IU16)pDcp->current_address[Chan][0];

    DmaInfo->count = ((IU16)pDcp->current_count[Chan][1] << 8)
                     | (IU16)pDcp->current_count[Chan][0];

    DmaInfo->page   = (IU16) pDmaAdp->pages.page[SbDmaChannel];
    DmaInfo->status = (IU8) pDcp->status.all;
    DmaInfo->mode   = (IU8) pDcp->mode[Chan].all;
    DmaInfo->mask   = (IU8) pDcp->mask;

    return (TRUE);
}

IU32
GetDMATransferAddress(
    void
    )

/*++

Routine Description:

    This function retrieves and translates a virtual DMA addr to its physical addr.

Arguments:

    None.

Return Value:

    Get DMA transfer address.
    Returns guest physical address; host memory access is performed by SAS.

--*/

{
    IU32 address;

    //
    // convert from 20 bit address to 32 bit address
    //

    address = (pDcp->current_address[Chan][1] << 8) | pDcp->current_address[Chan][0];
    /* T93: return a guest physical address, never a truncated host pointer. */
    address = dma_system_address(SbDmaChannel,
        pDmaAdp->pages.page[SbDmaChannel], address);
    return (address);
}


void
SetDMAStatus(
    SB_DMA_INFO * DmaInfo,
    IBOOL requesting,
    IBOOL tc
    )

/*++

Routine Description:

    Update the virtual DMA terminal count and request status.
    Terminal count (tc) is set when DMA count loops to 0xFFFF.
    Request status is set when DMA has data to transfer
    (ignored in auto-init DMA).

Arguments:

    DmaInfo - supplies the dam information

    Requesting - sepcifies if the REQUEST flag should be set.

    tc - specifies if the TC flag should be set

Return Value:

    None.

--*/

{

    if (requesting) {
        DmaInfo->status |= (0x10 << SbDmaChannel); // Requesting
    } else {
        DmaInfo->status &= ~(0x10 << SbDmaChannel); // Not Requesting
    }

    if (tc) {
        DmaInfo->status |= (1 << SbDmaChannel); // tc reached
    } else {
        DmaInfo->status &= ~(1 << SbDmaChannel); // tc not reached
    }
    pDcp->status.all = (IU8) DmaInfo->status;
}

/* XP guest DMA pause gates the next EOI transfer; it does not pause or clear
 * host output. The original position query served only a host Sleep(0),
 * which is unnecessary on the sole executor. */
void PauseDMA(void)
{
    bDspPaused = TRUE;
}

void ContinueDMA(void)
{
    bDspPaused = FALSE;
}
