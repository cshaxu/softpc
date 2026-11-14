#include <mmsystem.h>
#include "compat/sb_midi.h"
#line 2
//
// Definitions for MM api entry points. The functions will be linked
// dynamically to avoid bringing winmm.dll in before wow32.
// BUGBUG - shielint.  Should be moved to H file
//

typedef MMRESULT (WINAPI* SETVOLUMEPROC)(HWAVEOUT, DWORD);
typedef MMRESULT (WINAPI* GETVOLUMEPROC)(HWAVEOUT, LPDWORD);
typedef UINT     (WINAPI* GETNUMDEVSPROC)(VOID);
typedef MMRESULT (WINAPI* GETDEVCAPSPROC)(UINT, LPWAVEOUTCAPSA, UINT);
typedef MMRESULT (WINAPI* OPENPROC)(LPHWAVEOUT, UINT, LPCWAVEFORMATEX, DWORD, DWORD, DWORD);
typedef MMRESULT (WINAPI* PAUSEPROC)(HWAVEOUT);
typedef MMRESULT (WINAPI* RESTARTPROC)(HWAVEOUT);
typedef MMRESULT (WINAPI* RESETPROC)(HWAVEOUT);
typedef MMRESULT (WINAPI* CLOSEPROC)(HWAVEOUT);
typedef MMRESULT (WINAPI* GETPOSITIONPROC)(HWAVEOUT, LPMMTIME, UINT);
typedef MMRESULT (WINAPI* WRITEPROC)(HWAVEOUT, LPWAVEHDR, UINT);
typedef MMRESULT (WINAPI* PREPAREHEADERPROC)(HWAVEOUT, LPWAVEHDR, UINT);
typedef MMRESULT (WINAPI* UNPREPAREHEADERPROC)(HWAVEOUT, LPWAVEHDR, UINT);

typedef MMRESULT (WINAPI* SETMIDIVOLUMEPROC)(HMIDIOUT, DWORD);
typedef MMRESULT (WINAPI* GETMIDIVOLUMEPROC)(HMIDIOUT, LPDWORD);
typedef UINT     (WINAPI* MIDIGETNUMDEVSPROC)(VOID);
typedef MMRESULT (WINAPI* MIDIGETDEVCAPSPROC)(UINT, LPMIDIOUTCAPSA, UINT);
typedef MMRESULT (WINAPI* MIDIOPENPROC)(LPHMIDIOUT, UINT, DWORD, DWORD, DWORD);
typedef MMRESULT (WINAPI* MIDIRESETPROC)(HMIDIOUT);
typedef MMRESULT (WINAPI* MIDICLOSEPROC)(HMIDIOUT);
typedef MMRESULT (WINAPI* MIDILONGMSGPROC)(HMIDIOUT, LPMIDIHDR, UINT);
typedef MMRESULT (WINAPI* MIDISHORTMSGPROC)(HMIDIOUT, DWORD);
typedef MMRESULT (WINAPI* MIDIPREPAREHEADERPROC)(HMIDIOUT, LPMIDIHDR, UINT);
typedef MMRESULT (WINAPI* MIDIUNPREPAREHEADERPROC)(HMIDIOUT, LPMIDIHDR, UINT);

extern SETVOLUMEPROC            SetVolumeProc;
extern GETVOLUMEPROC            GetVolumeProc;
extern GETNUMDEVSPROC           GetNumDevsProc;
extern GETDEVCAPSPROC           GetDevCapsProc;
extern OPENPROC                 OpenProc;
extern PAUSEPROC                PauseProc;
extern RESTARTPROC              RestartProc;
extern RESETPROC                ResetProc;
extern CLOSEPROC                CloseProc;
extern GETPOSITIONPROC          GetPositionProc;
extern WRITEPROC                WriteProc;
extern PREPAREHEADERPROC        PrepareHeaderProc;
extern UNPREPAREHEADERPROC      UnprepareHeaderProc;

extern SETMIDIVOLUMEPROC        SetMidiVolumeProc;
extern GETMIDIVOLUMEPROC        GetMidiVolumeProc;
extern MIDIGETNUMDEVSPROC       MidiGetNumDevsProc;
extern MIDIGETDEVCAPSPROC       MidiGetDevCapsProc;
extern MIDIOPENPROC             MidiOpenProc;
extern MIDIRESETPROC            MidiResetProc;
extern MIDICLOSEPROC            MidiCloseProc;
extern MIDILONGMSGPROC          MidiLongMsgProc;
extern MIDISHORTMSGPROC         MidiShortMsgProc;
extern MIDIPREPAREHEADERPROC    MidiPrepareHeaderProc;
extern MIDIUNPREPAREHEADERPROC  MidiUnprepareHeaderProc;

extern BOOL     bHighSpeedMode;
extern HMIDIOUT HMidiOut;
extern HMIDIOUT HFM;
extern HWAVEOUT HWaveOut;
extern BOOL     bExitMidiThread;
extern WORD     MpuBasePort;
extern WORD     BasePort;
extern BOOL     bDevicesActive;

#define DisconnectPorts(First, Last) {                  \
            WORD i;                                      \
            for (i = First; i <= Last; i++) {           \
                io_disconnect_port(i, SNDBLST_ADAPTER); \
            }                                           \
        }

//
// Function Prototypes
//


//
// DSP prototypes
//

VOID
SbCloseDevices(
    VOID
    );

VOID
DspReadData(
    BYTE * data
    );

VOID
DspReadStatus(
    BYTE * data
    );

VOID
DspResetWrite(
    BYTE data
    );

VOID
DspWrite(
    BYTE data
    );

VOID
WriteCommandByte(
    BYTE command
    );

VOID
ResetDSP(
    VOID
    );

VOID
TableMunger(
    BYTE data
    );

DWORD
GetSamplingRate(
    VOID
    );

VOID
GenerateInterrupt(
    DWORD
    );

VOID
SetSpeaker(
    BOOL
    );

VOID
PauseWaveDevice(
    VOID
    );

VOID
RestartWaveDevice(
    VOID
    );

BOOL
FindWaveDevice(
    VOID
    );

BOOL
PrepareWaveInitialization(
    VOID
    );

VOID
CleanUpWave(
    VOID
    );

VOID
SetWaveOutVolume(
    DWORD Volume
    );

//
// FM Synth prototypes
//

VOID
ResetFM(
    VOID
    );

BOOL
OpenFMDevice(
    VOID
    );

VOID
CloseFMDevice(
    VOID
    );

VOID
FMStatusRead(
    BYTE *data
    );
VOID
FMDataWrite(
    BYTE data
    );

VOID
FMRegisterSelect(
    BYTE data
    );

//
// Mixer prototypes
//

void ResetMixer(void);
VOID
MixerDataRead(
    BYTE *pData
    );

VOID
MixerDataWrite(
    BYTE data
    );

VOID
MixerAddrWrite(
    BYTE data
    );

VOID
MixerSetMidiVolume(
    BYTE level
    );

VOID
MixerSetMasterVolume(
    BYTE level
    );

VOID
MixerSetVoiceVolume(
    BYTE level
    );

//
// MIDI prototypes
//

BOOL
InitializeMidi(
    VOID
    );

lib_status
BufferMidi(
    BYTE data
    );

VOID
CloseMidiDevice(
    VOID
    );

BOOL
OpenMidiDevice(
    HANDLE CallbackEvent
    );

VOID
StopMidiThread(
    BOOL wait
    );

VOID
DetachMidi(
    VOID
    );

VOID
ResetMidiDevice(
    VOID
    );

VOID CALLBACK
MidiBlockPlayedCallback (
    HMIDIOUT HMidiOut,
    UINT uMsg,
    DWORD dwInstance,
    DWORD dwParam1,
    DWORD dwParam2
    );

VOID
SetMidiOutVolume(
    DWORD Volume
    );

/*****************************************************************************
*
*    #defines
*
*****************************************************************************/

void DbgOut(LPSTR lpszFormat, ...);

/*****************************************************************************
*
*    Debugging
*    Levels:
*    bit 0 - port usage
*    bit 1 - errors only
*    bit 2 - significant events
*    bit 3 - regular events
*    bit 4 - heaps o' information
*
*****************************************************************************/

#if DBG
#error "Unsupported SoftPC mirror configuration: NTVDM sound debugging"
#else
#line 321

    #define dprintf(x)
    #define dprintf0(x)
    #define dprintf1(x)
    #define dprintf2(x)
    #define dprintf3(x)
    #define dprintf4(x)

#endif // DBG

//
// Temp stuff
//

#line 336

#if REPORT_SB_MODE
#error "Unsupported SoftPC mirror configuration: NTVDM sound mode diagnostics"
#endif
#line 350



void ResetMidiParser(void);
lib_status SendMidiRequest(void);
lib_status SbGetStatus(void);
lib_status SbService(void);
void MidiApplyMixer(void);

void SbApplyMixer(void);
IU16 MixerGetVoiceGain(void);
IU16 MixerGetMidiGain(void);

typedef struct _VDD_DMA_INFO {
    WORD    addr;
    WORD    count;
    WORD    page;
    BYTE    status;
    BYTE    mode;
    BYTE    mask;
} VDD_DMA_INFO, *PVDD_DMA_INFO;

IBOOL SbInitializeDMA(void);
BOOL QueryDMA(PVDD_DMA_INFO DmaInfo);
BOOL SetDMACountAddr(PVDD_DMA_INFO DmaInfo);
IU32 GetDMATransferAddress(void);
VOID SetDMAStatus(PVDD_DMA_INFO DmaInfo, BOOL requesting, BOOL tc);

typedef enum {
    Auto,
    Single,
    None
} DSP_MODE;

void ResetAll(void);
void VsbByteIn(IU16 port, IU8 *data);
void VsbByteOut(IU16 port, IU8 data);
void PauseDMA(void);
void ContinueDMA(void);
IBOOL StartDspTransfer(DSP_MODE mode);
IBOOL DmaDataToDsp(DSP_MODE mode);
lib_status DspProcessBlock(void);
lib_status DspServiceTransfer(void);
lib_status DspGetStatus(void);
void DspSetRunning(IBOOL running);
void SbSetRunning(IBOOL running);
lib_status MidiSetRunning(IBOOL running);
lib_status DspPrepareOutput(void);
lib_status DspCloseOutput(void);
lib_status DspArmCompletion(IU32 rate);
void DspCancelTransfer(void);
void StopDspTransfer(IBOOL reset);
