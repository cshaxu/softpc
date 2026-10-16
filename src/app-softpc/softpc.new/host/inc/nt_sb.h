//
// Mixer prototypes
//

/* T93: host PCM/status vocabulary is supplied by the Compat boundary. */
#include "compat/sb_pcm.h"

void ResetMixer(void);
void
MixerDataRead(
    IU8 *pData
    );

void
MixerDataWrite(
    IU8 data
    );

void
MixerAddrWrite(
    IU8 data
    );

void
MixerSetMidiVolume(
    IU8 level
    );

void
MixerSetMasterVolume(
    IU8 level
    );

void
MixerSetVoiceVolume(
    IU8 level
    );

/* T93: one device-side recomputation after register, speaker or output changes. */
void SbApplyMixer(void);
IU16 MixerGetVoiceGain(void);
IU16 MixerGetMidiGain(void);

/* T93: copied controller observation, not a second DMA controller or VDD ABI. */
typedef struct {
    IU16 addr, count, page;
    IU8 status, mode, mask;
} SB_DMA_INFO;

IBOOL SbInitializeDMA(void);
IU32 GetSamplingRate(void);
IBOOL QueryDMA(SB_DMA_INFO *DmaInfo);
IBOOL SetDMACountAddr(SB_DMA_INFO *DmaInfo);
IU32 GetDMATransferAddress(void);
void SetDMAStatus(SB_DMA_INFO *DmaInfo, IBOOL requesting, IBOOL tc);

typedef enum {
    Auto,
    Single,
    None
} DSP_MODE;

void DspReadStatus(IU8 *data);
void DspReadData(IU8 *data);
void DspResetWrite(IU8 data);
void DspWrite(IU8 data);
void WriteCommandByte(IU8 command);
void ResetDSP(void);
void ResetAll(void);
void VsbByteIn(IU16 port, IU8 *data);
void VsbByteOut(IU16 port, IU8 data);
void TableMunger(IU8 data);
void GenerateInterrupt(IU32 delay);
void SetSpeaker(IBOOL on);
void PauseDMA(void);
void ContinueDMA(void);
IBOOL StartDspTransfer(DSP_MODE mode);
IBOOL DmaDataToDsp(DSP_MODE mode);
lib_status DspProcessBlock(void);
lib_status DspServiceTransfer(void);
lib_status DspGetStatus(void);
void DspSetRunning(IBOOL running);
lib_status DspPrepareOutput(void);
lib_status DspCloseOutput(void);
lib_status DspArmCompletion(IU32 rate);
void DspCancelTransfer(void);
void StopDspTransfer(IBOOL reset);
