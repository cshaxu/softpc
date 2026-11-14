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
#include "compat/sb_pcm.h"
#include <windows.h>
#include "insignia.h"
#include "host_def.h"
#include "ios.h"
#include "ica.h"
#include "xt.h"
#include "dma.h"
#include "sas.h"
#include "sndblst.h"
#include "nt_sb.h"
#include "compat/eoi.h"
#include "quick_ev.h"
#include "compat/devices/snapshot.h"

/*****************************************************************************
*
*    Globals
*
*****************************************************************************/

BYTE            IdentByte;              // used with DSP_CARD_IDENTIFY
BYTE            ReservedRegister;       // used with DSP_LOAD_RES_REG and DSP_READ_RES_REG
ULONG           PageSize = 0;           // size of pages for VirtualAlloc
RTL_CRITICAL_SECTION  DspLock;

#define LockDsp()    RtlEnterCriticalSection(&DspLock)
#define UnlockDsp()  RtlLeaveCriticalSection(&DspLock)

//
// Event Globals
//

HANDLE          DspWaveSem;          // used by app to indicate data to write
HANDLE          ThreadStarted;          // signalled when thread starts running
HANDLE          ThreadFinished;         // signalled when thread exits
HANDLE          DspResetEvent;
HANDLE          DspResetDone;
HANDLE          DspWavePlayed;

//
// Wave globals
//

UINT            WaveOutDevice;          // device identifier for open and close devices
HWAVEOUT        HWaveOut = NULL;        // the current open wave output device
PCMWAVEFORMAT   WaveFormat = { { WAVE_FORMAT_PCM, 1, 0, 0, 1}, 8};
DWORD           TimeConstant = 0xA6;    // one byte format
DWORD           SBBlockSize = 0x7ff;    // Block size set by apps, always size of transfer-1

WAVEHDR         *WaveHdrs;              // pointer to allocated wave headers
ULONG           *WaveBlockSizes;
ULONG           BurstSize;

BOOL            bDspActive = FALSE;     // dsp thread currently active
BOOL            bDspPaused = FALSE;     // dsp paused
BOOL            SbAnswerDMAPosition = FALSE;
BOOL            bWriteBurstStarted = FALSE;

//
// To keep track of the waveout volume changes.  So we can restore the volume
// when closing the waveout device.
//

DWORD           OriginalWaveVol;
DWORD           PreviousWaveVol;

//
// bExitThread - terminate DSP DMA thread. This responses to REAL reset cmd.  TO
//               terminate DSP DMA thread, the bDspReset flag should also be set.
// bDspReset - Indicates Dsp reset cmd is received.  In case that reset is used to
//             exit high speed mode the bExitThread will not be set.
//
BOOL            bExitDMAThread= FALSE;  // Exit DSP DMA thread flag
BOOL            bDspReset = FALSE;
BOOL            bExitAuto = FALSE;
BOOL            bHighSpeedMode = FALSE; // Are we in High Speed transfer mode?

ULONG           DspNextRead;
ULONG           DspNextWrite;
ULONG           DspBufferTotalBursts;
ULONG           DspBufferSize;

//
// The following variables maintain the real sound WaveOutPosition while playing
// a SBBlockSize samples.  It gets reset on every SBBlockSize sample.
//

ULONG           StartingWaveOutPos, PreviousWaveOutPos, NextWaveOutPos;
USHORT          StartingDmaAddr, StartingDmaCount;

//
// records # of dma queries made by app while playing a SBBlockSize samples.
// it helps up figuring out how much, on average, the sample played between
// dam queries.
//

ULONG           DspNumberDmaQueries;
ULONG           DspVirtualInc, DspVirtualIncx;

//
// Determine how much samples can be ignored per SBBlockSize samples.
//

ULONG           EndingDmaValue;

//
// DMA globals to speed up dma update
//

VDD_DMA_INFO DmaInfo;

DMA_ADAPT    *pDmaAdp;
DMA_CNTRL    *pDcp;
WORD         Chan;

#define COMPUTE_INTERRUPT_DELAY(sb)   (1000 * (sb + 1) / WaveFormat.wf.nAvgBytesPerSec + 1)

DSP_MODE DspMode;

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
static DWORD blockSize;
static BYTE TableMungeData;
static BOOL TableMungeFirstByte = TRUE; // munging first or second byte
static void DspCompleteBlock(long unused);
static void AutoInitEoiHook(int irq, int count);

VOID
ExitAutoMode(
    VOID
    );

void StopDspTransfer(IBOOL reset)
{
    lib_status status;
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
        sas_PRS(dma_system_address(SbDmaChannel, DmaInfo.page, address), &DspBuffer[i], 1);
        address = (IU16)(address + ((DmaInfo.mode & 0x20) ? -1 : 1));
    }

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
    DspVirtualIncx = DspVirtualInc;     // remember the original virtual incr in case we
                                        // need to fall back to original value.

    dprintf3(("NoQ = %x, Inc = %x\n", DspNumberDmaQueries, DspVirtualInc));
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
    MidiOutPoll,
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

//
// Wave function prototypes
//

BOOL
OpenWaveDevice(
    DWORD
    );

VOID
ResetWaveDevice(
    VOID
    );

VOID
CloseWaveDevice(
    VOID
    );

BOOL
TestWaveFormat(
    DWORD sampleRate
    );

BOOL
SetWaveFormat(
    VOID
    );

VOID
WaitOnWaveOutIdle(
    VOID
    );

VOID
PrepareHeaders(
    VOID
    );

VOID
UnprepareHeaders(
    VOID
    );

VOID
PauseDMA(
    VOID
    );

VOID
ContinueDMA(
    VOID
    );

ULONG
GetDMATransferAddress(
    VOID
    );

BOOL
QueryDMA(
    PVDD_DMA_INFO pDmaInfo
    );

BOOL
SetDMACountAddr(
    PVDD_DMA_INFO pDmaInfo
    );

VOID
SetDMAStatus(
    PVDD_DMA_INFO pDmaInfo,
    BOOL requesting,
    BOOL tc
    );

BOOL
StartDspDmaThread(
    DSP_MODE mode
    );

VOID
StopDspDmaThread(
    BOOL wait
    );

DWORD WINAPI
DspThreadEntry(
    LPVOID context
    );

BOOL
GetWaveOutPosition(
    PULONG pPos
    );

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


IBOOL SbInitializeDMA(void)
{
    if (SbDmaChannel >= DMA_CONTROLLER_CHANNELS) return FALSE;
    pDmaAdp = dmaGetAdaptor();
    pDcp = &pDmaAdp->controller[dma_physical_controller(SbDmaChannel)];
    Chan = dma_physical_channel(SbDmaChannel);
    return TRUE;
}

VOID
DspReadStatus(
    BYTE * data
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

VOID
DspReadData(
    BYTE * data
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
        dprintf0(("rd Reset"));
        break;

    case FirstVersionByte:
        *data = (BYTE)(SB_VERSION / 256);
        DSPReadState = SecondVersionByte;
        dprintf0(("rd 1st Version"));
        break;

    case SecondVersionByte:
        *data = (BYTE)(SB_VERSION % 256);
        DSPReadState = NothingToRead;
        dprintf0(("rd 2nd Version"));
        break;

    case ReadIdent:
        *data = ~IdentByte;
        DSPReadState = NothingToRead;
        dprintf0(("rd Id"));
        break;

    case ReadResReg:
        *data = ReservedRegister;
        DSPReadState = NothingToRead;
        dprintf0(("rd RsvdReg"));
        break;

    default:
        dprintf1(("Unrecognized read state"));
    }

}

VOID
DspResetWrite(
    BYTE data
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

VOID
DspWrite(
    BYTE data
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
        dprintf0(("wt CMD"));
        WriteCommandByte(data);
        break;

    case MidiOutPoll:
        dprintf0(("wt MIDI Byte"));
        DSPWriteState = WriteCommand;
        break;

    case UnsupportedMidiUart:
        break;

    case CardIdent:
        dprintf0(("wt ID"));
        IdentByte = data;
        DSPReadState = ReadIdent;
        DSPWriteState = WriteCommand;
        break;

    case TableMunge:
        dprintf0(("wt TblMunge"));
        TableMunger(data);
        DSPWriteState = WriteCommand;
        break;

    case LoadResReg:
        dprintf0(("wt RsvReg"));
        ReservedRegister = data;
        DSPWriteState = WriteCommand;
        break;

    case SetTimeConstant:
        dprintf0(("wr TmCnst"));
        TimeConstant =  (DWORD)data;
        dprintf2(("Time constant is %X", TimeConstant));
        dprintf2(("Set sampling rate %d", GetSamplingRate()));
        DSPWriteState = WriteCommand;
        break;

    case BlockSizeFirstByte:
        dprintf0(("wt 1st Blksize"));
        blockSize = (DWORD)data;
        DSPWriteState = BlockSizeSecondByte;
        break;

    case BlockSizeSecondByte:
        dprintf0(("wt 2nd Blksize"));
        SBBlockSize = blockSize + (((DWORD)data) << 8);

        DSPWriteState = WriteCommand;
        dprintf2(("Block size = 0x%x", SBBlockSize));
        break;

    case BlockSizeFirstByteWrite:
        dprintf0(("wt 1st Blksize single"));
        blockSize = (DWORD)data;
        DSPWriteState = BlockSizeSecondByteWrite;
        break;

    case BlockSizeSecondByteWrite:
        dprintf0(("wt 2nd Blksize single"));
        SBBlockSize = blockSize + (((DWORD)data) << 8);

        DSPWriteState = WriteCommand;
        dprintf2(("Block size = 0x%x, Single Cycle starting", SBBlockSize));
        if (SBBlockSize <= 0x10) {

            //
            // this is a hack to convince some apps a sb exists
            //

            if (SBBlockSize > 0) {
                VDD_DMA_INFO dmaInfo;

                QueryDMA(&dmaInfo);
                dmaInfo.count -= (WORD)(SBBlockSize + 1);
                if (dmaInfo.mode & 0x20)
                    dmaInfo.addr -= (WORD)(SBBlockSize + 1);
                else
                    dmaInfo.addr += (WORD)(SBBlockSize + 1);

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
        dprintf0(("wt 1st IN Blksize"));
        blockSize = (DWORD)data;
        DSPWriteState = BlockSizeSecondByteRead;
        break;

    case BlockSizeSecondByteRead:
        dprintf0(("wt 2nd IN Blksize"));
        SBBlockSize = blockSize + (((DWORD)data) << 8);

        DSPWriteState = WriteCommand;
        dprintf2(("IN Blksize set to 0x%x", SBBlockSize));
        // this is a hack to convince some apps a sb exists
        if (SBBlockSize <= 0x10) {
            ULONG dMAPhysicalAddress;

            if ((dMAPhysicalAddress=GetDMATransferAddress()) != -1L) {
                BYTE silence = 0x80;
                sas_PWS(dMAPhysicalAddress, &silence, 1);
            }
            GenerateInterrupt(2);
        }
        // NOt implemented
        break;

    case MIDI_READ_POLL:
    case MIDI_READ_INTERRUPT:
    case MIDI_READ_TIMESTAMP_POLL:
    case MIDI_READ_TIMESTAMP_INTERRUPT:
        //
        // These commands we will never return anything for since
        // we have nothing to get MIDI data from.  We simply
        // accept the command, but never respond - as if there
        // were no MIDI hardware connected to a real SB.
        // Since we never respond, we don't have to handle
        // the interrupt or timestamp versions differently.
        //
        dprintf2(("Cmd-Midi non UART read"));
        break;

    case MIDI_READ_POLL_WRITE_POLL_UART:
    case MIDI_READ_INTERRUPT_WRITE_POLL_UART:
    case MIDI_READ_TIMESTAMP_INTERRUPT_WRITE_POLL_UART:
        dprintf2(("Cmd-Midi UART I/O xxx"));
        break;

    case MIDI_WRITE_POLL:
        // Specifies that next byte will be a Midi out data
        dprintf2(("Cmd-MIDI out poll"));
        DSPWriteState = MidiOutPoll;
        break;

    default:
        dprintf1(("Unrecognized DSP write state %x", DSPWriteState));
    }
}

VOID
WriteCommandByte(
    BYTE command
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
    case MIDI_WRITE_POLL:
        DSPWriteState = MidiOutPoll;
        break;

    case MIDI_READ_POLL_WRITE_POLL_UART:
    case MIDI_READ_INTERRUPT_WRITE_POLL_UART:
    case MIDI_READ_TIMESTAMP_POLL_WRITE_POLL_UART:
    case MIDI_READ_TIMESTAMP_INTERRUPT_WRITE_POLL_UART:
        DSPWriteState = UnsupportedMidiUart;
        break;

    case DSP_GET_VERSION:
        dprintf2(("Cmd-GetVer"));
        DSPReadState = FirstVersionByte;
        break;

    case DSP_CARD_IDENTIFY:
        dprintf2(("Cmd-Id"));
        DSPWriteState = CardIdent;
        break;

    case DSP_TABLE_MUNGE:
        dprintf2(("Cmd-Table Munge"));
        DSPWriteState = TableMunge;
        break;

    case DSP_LOAD_RES_REG:
        dprintf2(("Cmd-Wt Res Reg"));
        DSPWriteState = LoadResReg;
        break;

    case DSP_READ_RES_REG:
        dprintf2(("Cmd-Rd Res Reg"));
        DSPReadState = ReadResReg;
        break;

    case DSP_GENERATE_INT:
        dprintf2(("Cmd-GenerateInterrupt"));
        GenerateInterrupt(1);
        break;

    case DSP_SPEAKER_ON:
        dprintf2(("Cmd-Speaker ON"));
        SetSpeaker(TRUE);
        break;

    case DSP_SPEAKER_OFF:
        dprintf2(("Cmd-Speaker OFF"));
        SetSpeaker(FALSE);
        break;

    case DSP_SET_SAMPLE_RATE:
        dprintf2(("Cmd-Set Sample Rate"));
        DSPWriteState = SetTimeConstant;
        break;

    case DSP_SET_BLOCK_SIZE:
        dprintf2(("Cmd-Set Block Size"));
        DSPWriteState =  BlockSizeFirstByte;
        break;

    case DSP_PAUSE_DMA:
        dprintf2(("Cmd-Pause DMA"));
        PauseDMA();
        break;

    case DSP_CONTINUE_DMA:
        dprintf2(("Cmd - Continue DMA"));
        ContinueDMA();
        break;

    case DSP_STOP_AUTO:
        dprintf2(("Cmd- Exit Auto-Init"));
        bExitAuto = TRUE;
        //ExitAutoMode();
        break;

    case DSP_WRITE:
        dprintf2(("Cmd- DSP OUT"));
        DSPWriteState = BlockSizeFirstByteWrite;
        break;

    case DSP_WRITE_HS:
        dprintf2(("Cmd- DSP HS OUT"));
        bHighSpeedMode = TRUE;
        StartDspTransfer(Single);
        break;

    case DSP_WRITE_AUTO:
        dprintf2(("Cmd-DSP OUT Auto"));
        if (SBBlockSize <= 0x10) {

            //
            // this is a hack to convince some apps a sb exists
            //

            if (SBBlockSize > 0) {
                VDD_DMA_INFO dmaInfo;

                QueryDMA(&dmaInfo);
                dmaInfo.count -= (WORD)(SBBlockSize + 1);
                if (dmaInfo.mode & 0x20)
                    dmaInfo.addr -= (WORD)(SBBlockSize + 1);
                else
                    dmaInfo.addr += (WORD)(SBBlockSize + 1);

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
        dprintf2(("Cmd-DSP HS OUT AUTO"));
        bHighSpeedMode = TRUE;
        StartDspTransfer(Auto);
        break;

    case DSP_READ:
        dprintf2(("Cmd- DSP IN - non Auto"));
        DSPWriteState = BlockSizeFirstByteRead;
        break;

    default:
        dprintf2(("Unrecognized DSP command %2X", command));
    }
}

VOID
ResetDSP(
    VOID
    )

/*++

Routine Description:

    This function handles DSP reset command. It resets
    threads/globals/events/state-machines to initial state.

Arguments:

    command - supplies command.

Return Value:

    None.

--*/

{
    StopDspTransfer(bExitDMAThread);

    if (bExitDMAThread) {

        //
        // if this is a real RESET command, not just reset to exit HighSpeed mode
        //

        //
        // Stop any active DMA threads.  Need to wait till the thread exit.
        //


        //
        // Set events and globals to initial state
        //


        HWaveOut = NULL;
        TimeConstant = 0xA6;   //(256 - 1000000/11025)
        WaveFormat.wf.nSamplesPerSec = 0;
        WaveFormat.wf.nAvgBytesPerSec = 0;

        bDspActive = FALSE;
        bExitDMAThread= FALSE;
    }

    DspMode = None;
    SBBlockSize = 0x7ff;
    DspNextRead = DspBufferTotalBursts - 1;
    DspNextWrite = 0;
    bDspPaused = FALSE;
    bDspReset = FALSE;
    NextWaveOutPos = 0;

    //
    // Reset state machines
    //

    DSPReadState = Reset;
    DSPWriteState = WriteCommand;
    ResetState = ResetNotStarted;

    //
    // To start accept command again.
    //

    SbAnswerDMAPosition = FALSE;
    bHighSpeedMode = FALSE;
}

VOID
TableMunger(
    BYTE data
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
    BYTE comp, dataCopy;
    VDD_DMA_INFO dMAInfo;
    ULONG dMAPhysicalAddress;

    if (TableMungeFirstByte) {
        dprintf3(("Munging first byte"));
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
        dprintf3(("Writing first byte"));
        if ((dMAPhysicalAddress=GetDMATransferAddress()) == -1L) {
            dprintf1(("Unable to get dma address"));
            return;
        }
        sas_PWS(dMAPhysicalAddress, &data, 1);

        // Update virtual DMA status
        QueryDMA(&dMAInfo);
        dMAInfo.count = dMAInfo.count - 1;
        dMAInfo.addr += (dMAInfo.mode & 0x20) ? -1 : 1;
        SetDMACountAddr(&dMAInfo);
        TableMungeFirstByte = FALSE;
    } else {
        dprintf3(("Munging second byte"));
        data = data ^ 0xA5;
        data = data + TableMungeData;
        TableMungeData = data;

        // Update memory (code table) with munged data
        dprintf3(("Writing second byte"));
        if ((dMAPhysicalAddress=GetDMATransferAddress()) == -1L) {
            dprintf1(("Unable to get dma address"));
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

DWORD
GetSamplingRate(
    VOID
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
    DWORD samplingRate;

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

VOID
GenerateInterrupt(
    ULONG delay
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
        dprintf2(("Generating interrupt"));
        ica_hw_interrupt(ICA_MASTER, SbInterrupt, 1);
    } else {
        dprintf2(("Generating interrupt with %x ms delay", delay));
        host_DelayHwInterrupt(SbInterrupt, 1, delay * 1000);
    }
}

VOID
SetSpeaker(
    BOOL On
    )

/*++

Routine Description:

    This function sets the speaker on or off.

Arguments:

    On - supplies a boolean value to specify speaker state.

Return Value:

    None.

--*/

{
    if (DspOutputRate == 0) return;
    DspSpeakerEnabled = On;
    SbApplyMixer();
}

VOID
PauseDMA(
    VOID
    )

/*++

Routine Description:

    This function pauses single-cycle or Auto-Init DMA.

    Note, we don't support PauseDMA and ContinueDMA just for PAUSE.
    We expect apps PauseDMA, StartSingleCycle and optionally ContinueDMA.

Arguments:

    None.

Return Value:

    None

--*/

{
    ULONG position;

    bDspPaused = TRUE;

}

VOID
ContinueDMA(
    VOID
    )

/*++

Routine Description:

    This function continues paused single-cycle or Auto-Init DMA.

    Note, we don't support PauseDMA and ContinueDMA just for PAUSE.
    We expect apps PauseDMA, StartSingleCycle and optionally ContinueDMA.

Arguments:

    None.

Return Value:

    None.

--*/

{
    bDspPaused = FALSE;
    Sleep(0);
}

BOOL
FindWaveDevice(
    VOID
    )

/*++

Routine Description:

    This function finds a suitable wave output device.

Arguments:

    None.

Return Value:

    TRUE - if device is found.
    FALSE - no device is found.

--*/

{
    UINT numDev;
    UINT device;
    WAVEOUTCAPS wc;

    numDev = GetNumDevsProc();

    for (device = 0; device < numDev; device++) {
        if (MMSYSERR_NOERROR == GetDevCapsProc(device, &wc, sizeof(wc))) {

            //
            // Need 11025 and 44100 for device
            //

            if ((wc.dwFormats & (WAVE_FORMAT_1M08 | WAVE_FORMAT_4M08)) ==
                (WAVE_FORMAT_1M08 | WAVE_FORMAT_4M08)) {
                WaveOutDevice = device;
                return (TRUE);
            }
        }
    }

    dprintf1(("Wave device not found"));
    return (FALSE);
}

BOOL
OpenWaveDevice(
    DWORD CallbackFunction
    )

/*++

Routine Description:

    This function opens wave device and starts synchronization thread.

Arguments:

    CallbackFunction - specifies the callback function

Return Value:

    TRUE - if success otherwise FALSE.

--*/

{
    UINT rc;
    HANDLE tHandle;
    if (CallbackFunction) {
        rc = OpenProc(&HWaveOut, (UINT)WaveOutDevice, (LPWAVEFORMATEX)
                      &WaveFormat, CallbackFunction, 0, CALLBACK_FUNCTION);
    } else {
        rc = OpenProc(&HWaveOut, (UINT)WaveOutDevice, (LPWAVEFORMATEX)
                      &WaveFormat, 0, 0, CALLBACK_NULL);
    }

    if (rc != MMSYSERR_NOERROR) {
        dprintf1(("Failed to open wave device - code %d", rc));
        return (FALSE);
    } else {

        //
        // Remember the original waveout volume setting.
        // Note we don't care if the device supports it.  If it does not
        // all the setvolume calls will fail and we won't change it anyway.
        //

        GetVolumeProc(HWaveOut, &OriginalWaveVol);
        PreviousWaveVol = OriginalWaveVol;
    }
    NextWaveOutPos = 0;
    return (TRUE);
}

VOID
SetWaveOutVolume(
    DWORD Volume
    )

/*++

Routine Description:

    This function sets WaveOut volume

Arguments:

    Volume - specifies the volume scale

Return Value:

    None.

--*/

{
    DWORD currentVol;

    if (HWaveOut) {
        if (GetVolumeProc(HWaveOut, &currentVol)) {
            if (currentVol != PreviousWaveVol) {
                //
                // SOmeone changed the volume besides NTVDM
                //

                OriginalWaveVol = currentVol;
            }
            PreviousWaveVol = Volume;
            SetVolumeProc(HWaveOut, Volume);
        }
    }
}

VOID
ResetWaveDevice(
    VOID
    )

/*++

Routine Description:

    This function resets wave device.

Arguments:

    None.

Return Value:

    None.

--*/

{

    dprintf3(("Resetting wave device"));
    if (HWaveOut) {
        if (MMSYSERR_NOERROR != ResetProc(HWaveOut)) {
            dprintf1(("Unable to reset wave out device"));
        }
    }
}

VOID
CloseWaveDevice(
    VOID
    )

/*++

Routine Description:

    This function shuts down and closes wave device.

Arguments:

    None.

Return Value:

    None.

--*/

{
    DWORD currentVol;

    //
    // wait till the MM driver is done with the device and then unprepare
    // the prepared headers and close it.
    //

    if (HWaveOut) {

        dprintf2(("Closing wave device"));

        if (GetVolumeProc(HWaveOut, &currentVol)) {
            if (currentVol == PreviousWaveVol) {
                //
                // If we are the last one changed, volume restore it.
                // otherwise leave it alone.
                //
                SetVolumeProc(HWaveOut, OriginalWaveVol);
            }
        }
        WaitOnWaveOutIdle();
        UnprepareHeaders();
        ResetWaveDevice();

        if (MMSYSERR_NOERROR != CloseProc(HWaveOut)) {
            dprintf1(("Unable to close wave out device"));
        } else {
            HWaveOut = NULL;
        }
    }
}

BOOL
TestWaveFormat(
    DWORD sampleRate
    )

/*++

Routine Description:

    This function tests if current wave device supports the sample rate.

Arguments:

    SampleRate - supplies sample rate to be tested.

Return Value:

    TRUE if current wave device supports sample rate, otherwise FALSE.

--*/

{
    PCMWAVEFORMAT format;

    format = WaveFormat;
    format.wf.nSamplesPerSec = sampleRate;
    format.wf.nAvgBytesPerSec = sampleRate;

    return (MMSYSERR_NOERROR == OpenProc(NULL, (UINT)WaveOutDevice,
                                         (LPWAVEFORMATEX) &format,
                                         0, 0, WAVE_FORMAT_QUERY));
}

BOOL
SetWaveFormat(
    VOID
    )

/*++

Routine Description:

    This function makes sure we've got a device that matches the current
    sampling rate. Returns TRUE if device does NOT support current sampling
    rate and wave format has changed, otherwise returns FALSE

Arguments:

    None.

Return Value:

    TRUE if device does NOT support current sampling rate and wave format
    has changed, otherwise returns FALSE

--*/

{
    DWORD sampleRate;
    DWORD testValue;
    UINT i = 0;

    if (TimeConstant != 0xFFFF) {

        //
        // time constant has been reset since last checked
        //

        sampleRate = GetSamplingRate();
        dprintf2(("Requested sample rate is %d", sampleRate));

        if (sampleRate != WaveFormat.wf.nSamplesPerSec) {  // format has changed
            if (!TestWaveFormat(sampleRate)) {
                dprintf3(("Finding closest wave format"));

                //
                // find some format that works and is close to requested
                //

                for (i=0; i<50000; i++) {
                    testValue = sampleRate-i;
                    if (TestWaveFormat(testValue)) {
                        sampleRate = testValue;
                        break;
                    }
                    testValue = sampleRate+i;
                    if (TestWaveFormat(testValue)) {
                        sampleRate = testValue;
                        break;
                    }
                }
                if (sampleRate!=testValue) {
                    dprintf1(("Unable to find suitable wave format"));
                    return (FALSE);
                }
            }

            //
            // Set the new format if it's changed
            //

            if (sampleRate != WaveFormat.wf.nSamplesPerSec) {
                dprintf2(("Setting %d samples per second", sampleRate));
                WaveFormat.wf.nSamplesPerSec = sampleRate;
                WaveFormat.wf.nAvgBytesPerSec = sampleRate;
                TimeConstant = 0xFFFF;
                return (TRUE);
            }
        }
    }

    TimeConstant = 0xFFFF;
    return (FALSE);
}

BOOL
SetDMACountAddr(
    PVDD_DMA_INFO pDmaInfo
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
    pDcp->current_address[Chan][1] = (half_word)HIBYTE(pDmaInfo->addr);
    pDcp->current_address[Chan][0] = (half_word)LOBYTE(pDmaInfo->addr);
    pDcp->current_count[Chan][1] = (half_word)HIBYTE(pDmaInfo->count);
    pDcp->current_count[Chan][0] = (half_word)LOBYTE(pDmaInfo->count);

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

BOOL
QueryDMA(
    PVDD_DMA_INFO DmaInfo
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
    DmaInfo->addr  = ((WORD)pDcp->current_address[Chan][1] << 8)
                     | (WORD)pDcp->current_address[Chan][0];

    DmaInfo->count = ((WORD)pDcp->current_count[Chan][1] << 8)
                     | (WORD)pDcp->current_count[Chan][0];

    DmaInfo->page   = (WORD) pDmaAdp->pages.page[SbDmaChannel];
    DmaInfo->status = (BYTE) pDcp->status.all;
    DmaInfo->mode   = (BYTE) pDcp->mode[Chan].all;
    DmaInfo->mask   = (BYTE) pDcp->mask;

    dprintf3(("DMA Info : addr  %4X, count %4X, page %4X, status %2X, mode %2X, mask %2X",
              DmaInfo->addr, DmaInfo->count, DmaInfo->page, DmaInfo->status,
              DmaInfo->mode, DmaInfo->mask));
    return (TRUE);
}

ULONG
GetDMATransferAddress(
    VOID
    )

/*++

Routine Description:

    This function retrieves and translates a virtual DMA addr to its physical addr.

Arguments:

    None.

Return Value:

    Get DMA transfer address.
    Returns transfer address or -1 on failure.

--*/

{
    ULONG address;
    VDD_DMA_INFO dmaInfo;

    //
    // convert from 20 bit address to 32 bit address
    //

    address = (pDcp->current_address[Chan][1] << 8) | pDcp->current_address[Chan][0];
    address = dma_system_address(SbDmaChannel,
        pDmaAdp->pages.page[SbDmaChannel], address);

    dprintf3(("Physical Transfer address = %8X", (DWORD)address));
    return (address);
}

VOID
SetDMAStatus(
    PVDD_DMA_INFO DmaInfo,
    BOOL requesting,
    BOOL tc
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
        dprintf3(("DMA set as requesting"));
    } else {
        DmaInfo->status &= ~(0x10 << SbDmaChannel); // Not Requesting
        dprintf3(("DMA set as not requesting"));
    }

    if (tc) {
        DmaInfo->status |= (1 << SbDmaChannel); // tc reached
        dprintf3(("DMA set as terminal count reached"));
    } else {
        DmaInfo->status &= ~(1 << SbDmaChannel); // tc not reached
        dprintf3(("DMA set as terminal count not reached"));
    }
    pDcp->status.all = (BYTE) DmaInfo->status;
}

BOOL
GetWaveOutPosition(
    PULONG pPos
    )
/*++

Routine Description:

    This function calls MM low level api to get the current wave out play back
    position.

Arguments:

    pPos - supplies a point to a ULONG to receive the play back position

Return Value:

    TRUE - success.  Otherwise failed.

--*/

{
    MMTIME mmTime;

    mmTime.wType = TIME_SAMPLES;

    if (MMSYSERR_NOERROR == GetPositionProc(HWaveOut, &mmTime, sizeof(MMTIME))) {
        *pPos = mmTime.u.sample;
        return (TRUE);
    }
    dprintf1(("Get Waveout position failed\n"));
    return (FALSE);
}

VOID
SbGetDMAPosition(
    VOID
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
    ULONG position;     // real sound played position
    ULONG offset;       // the amount of sound really played
    ULONG virtOffset;   // the amount of sound virtually/expectedly played

    if (bDspActive && SbAnswerDMAPosition && bWriteBurstStarted) {
        dprintf3(("SbGetDMAPosition"));
        DspNumberDmaQueries++;
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
                dprintf3(("virt addr = %x, count = %x", DmaInfo.addr, DmaInfo.count));
                SetDMACountAddr(&DmaInfo);
            } else {
                Sleep(0);
            }
            return;
        }
#if 1
#line 2184
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
            //Sleep(0);
            DspVirtualInc = 2;
        }

        //
        // Put a limit so we don't overflow the virtual sound position
        //

        if (offset >= SBBlockSize - 0x8) {
            //Sleep(0);
            offset = SBBlockSize - 0x8;
        }

        if (DspVirtualIncx > 0xc0) {
            Sleep(0);
        }
        if (DspVirtualIncx > 0x80) {
            Sleep(0);
#line 2218
#if DESCENT
#error "Unsupported SoftPC mirror configuration: DESCENT"
#endif
#line 2230
        }
        Sleep(0);
#endif
#line 2233
#if 0
#error "Unsupported SoftPC mirror configuration: constant false"
#endif
#line 2268
        dprintf2(("voffset = %x inc = %x\n", offset, DspVirtualInc));

        //
        // Now update the dma controller with our emulation/real position
        //

        DmaInfo.addr = (DmaInfo.mode & 0x20) ?
            StartingDmaAddr - (IU16)offset : StartingDmaAddr + (IU16)offset;
        DmaInfo.count = StartingDmaCount - (IU16)offset;

        SetDMACountAddr(&DmaInfo);
        dprintf3(("INB: AFT Cnt= %x, Addr= %x\n", DmaInfo.count, DmaInfo.addr));


    }
}

VOID
WaitOnWaveOutIdle(
    VOID
    )
{
    ULONG LastBytesPlayedValue = 0;
    ULONG PhysicalBytesPlayed;

    //
    // Allow the device to finish playing current sounds before nuking buffers
    //

    while (GetWaveOutPosition(&PhysicalBytesPlayed)) {
        if (LastBytesPlayedValue == PhysicalBytesPlayed) {
            break;  // no sounds are playing
        }
        LastBytesPlayedValue = PhysicalBytesPlayed;
        Sleep(1);
    }
}

BOOL
WriteBurst(
    ULONG WriteSize
    )

/*++

Routine Description:

    This function sends a bust of wave data to the multimedia device and
    updates DMA controller accordingly.

Arguments:

    WriteSize - supplies the number of bytes to write to Wave device

Return Value:

    return value of WriteProc MM interface.

--*/

{
    MMRESULT mmResult;
    PWAVEHDR header = &WaveHdrs[DspNextRead];

    dprintf2(("read: write burst at block %x", DspNextRead));


    //
    // Send the data to MM Waveout device
    //

    header->dwBufferLength = WriteSize;
    mmResult = WriteProc(HWaveOut, header, sizeof(WAVEHDR));
    return (mmResult == MMSYSERR_NOERROR);
}


VOID
PrepareHeaders(
    VOID
    )

/*++

Routine Description:

    This function calls HWaveOut device to prepare headers.

Arguments:

    None.

Return Value:

    None.

--*/

{
    ULONG i;

    dprintf3(("Prepare Headers"));

    for (i = 0; i < DspBufferTotalBursts; i++) {
        PrepareHeaderProc(HWaveOut, &WaveHdrs[i], sizeof(WAVEHDR));
    }
}

VOID
UnprepareHeaders(
    VOID
    )

/*++

Routine Description:

    This function calls HWaveOut device to unprepare headers.

Arguments:

    None.

Return Value:

    None.

--*/

{
    ULONG i;

    dprintf3(("Unprepare Headers"));

    for (i = 0; i < DspBufferTotalBursts; i++) {
        while (WAVERR_STILLPLAYING ==
                   UnprepareHeaderProc(HWaveOut, &WaveHdrs[i], sizeof(WAVEHDR))) {
            Sleep(1);
        }
        WaveHdrs[i].dwFlags = 0;        // Must be zero to call PrepareHeader
        WaveHdrs[i].dwUser  = 0;
    }
}

#if 0
#error "Unsupported SoftPC mirror configuration: disabled branch"
#endif
#line 2921

VOID
ExitAutoMode (
     void
     )

/*++

Routine Description:

    This function exits DSP auto-init mode

Arguments:

    None.

Return Value:

    None.

--*/

{
    DspMode = None;
    if (bExitAuto) {
        bExitAuto = FALSE;
        dprintf2(("ExitAuto CMD detected in Auto Mode"));
    } else {
        dprintf2(("SingleCycle detected in Auto Mode"));
    }

    //
    // Wait till MM driver finishes playing queued bursts. Then close
    // the waveout device
    //

    RemoveEOIHook(SbInterrupt, AutoInitEoiHook);
    DspEoiRegistered = FALSE;
    //ica_iret_hook_control(ICA_MASTER, SbInterrupt, FALSE);
    NextWaveOutPos = 0;
    dprintf2(("Auto-Init block done"));
}

