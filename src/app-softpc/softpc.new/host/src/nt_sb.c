#include <windows.h>
#include "host_def.h"
#include "insignia.h"
#include "ios.h"

#include "sndblst.h"
#include "nt_sb.h"
#include "compat/ccpu/lifecycle.h"
#include "compat/devices/snapshot.h"
#include <stdio.h>

//
// Definitions for MM api entry points. The functions will be linked
// dynamically to avoid bringing winmm.dll in before wow32.
//

SETVOLUMEPROC            SetVolumeProc;
GETVOLUMEPROC            GetVolumeProc;
GETNUMDEVSPROC           GetNumDevsProc;
GETDEVCAPSPROC           GetDevCapsProc;
OPENPROC                 OpenProc;
PAUSEPROC                PauseProc;
RESTARTPROC              RestartProc;
RESETPROC                ResetProc;
CLOSEPROC                CloseProc;
GETPOSITIONPROC          GetPositionProc;
WRITEPROC                WriteProc;
PREPAREHEADERPROC        PrepareHeaderProc;
UNPREPAREHEADERPROC      UnprepareHeaderProc;

SETMIDIVOLUMEPROC        SetMidiVolumeProc;
GETMIDIVOLUMEPROC        GetMidiVolumeProc;
MIDIGETNUMDEVSPROC       MidiGetNumDevsProc;
MIDIGETDEVCAPSPROC       MidiGetDevCapsProc;
MIDIOPENPROC             MidiOpenProc;
MIDIRESETPROC            MidiResetProc;
MIDICLOSEPROC            MidiCloseProc;
MIDILONGMSGPROC          MidiLongMsgProc;
MIDISHORTMSGPROC         MidiShortMsgProc;
MIDIPREPAREHEADERPROC    MidiPrepareHeaderProc;
MIDIUNPREPAREHEADERPROC  MidiUnprepareHeaderProc;

//
// Misc. globals
//

BOOL bSBAttached;
BOOL bDevicesActive = FALSE;    // Are MM sound devices initialized?
HINSTANCE hWinmm;               // module handle to winmm.dll
WORD BasePort, MpuBasePort;     // Where the card is mapped, ie the base I/O address
WORD MixerBasePort;
WORD MpuMode = MPU_INTELLIGENT_MODE;
USHORT SbInterrupt;
USHORT SbDmaChannel;
UCHAR MpuInData = 0xFF;

//
// Forward references
//

BOOL InitializeIoAddresses(void);
BOOL InstallIoHook(void);
VOID DeInstallIoHook(void);
VOID VsbByteIn(WORD port, BYTE * data);
VOID VsbByteOut(WORD port, BYTE data);
BOOL LoadWinmm(VOID);
BOOL InitDevices(VOID);

extern IBOOL bExitDMAThread;
static lib_status MidiStatus;


int softpc_device_snapshot_capture_sb(softpc_device_sb_state *state)
{
    if (SbGetStatus() != LIB_STATUS_OK) return FALSE;
    state->attached = bSBAttached;
    state->mpu_present = MpuBasePort != 0;
    state->mpu_mode = MpuMode;
    state->mpu_input = MpuInData;
    softpc_device_snapshot_capture_sb_mixer(&state->mixer);
    softpc_device_snapshot_capture_sb_fm(&state->fm);
    softpc_device_snapshot_capture_sb_midi(&state->midi);
    return softpc_device_snapshot_capture_sb_dsp(&state->dsp);
}

int softpc_device_snapshot_prepare_sb(const softpc_device_sb_state *state)
{
    if (state->attached != bSBAttached || state->mpu_present > 1 ||
        state->mpu_present != (MpuBasePort != 0) ||
        (state->mpu_mode != MPU_INTELLIGENT_MODE && state->mpu_mode != MPU_UART_MODE))
        return FALSE;

    StopDspTransfer(TRUE);
    if (DspGetStatus() != LIB_STATUS_OK) return FALSE;
    MidiStatus = softpc_sb_midi_reset();
    return MidiStatus == LIB_STATUS_OK;
}

int softpc_device_snapshot_restore_sb(const softpc_device_sb_state *state)
{
    if (!softpc_device_snapshot_restore_sb_mixer(&state->mixer) ||
        !softpc_device_snapshot_restore_sb_fm(&state->fm) ||
        !softpc_device_snapshot_restore_sb_midi(&state->midi) ||
        !softpc_device_snapshot_restore_sb_dsp(&state->dsp)) return FALSE;
    MpuMode = state->mpu_mode;
    MpuInData = state->mpu_input;
    SbApplyMixer();
    return SbGetStatus() == LIB_STATUS_OK;
}


static void MidiResult(lib_status status)
{
    if (status != LIB_STATUS_OK && status != LIB_STATUS_UNSUPPORTED &&
        !(status == LIB_STATUS_INVALID_STATE && softpc_ccpu_lifecycle_exit_requested()))
        MidiStatus = status;
}

lib_status SbGetStatus(void)
{
    lib_status status = DspGetStatus();
    if (status == LIB_STATUS_OK) status = softpc_sb_midi_status();
    return status != LIB_STATUS_OK ? status : MidiStatus;
}

void MidiApplyMixer(void)
{
    if (MpuBasePort && MidiStatus == LIB_STATUS_OK)
        MidiResult(softpc_sb_midi_gain(MixerGetMidiGain()));
}

lib_status SbService(void)
{
    lib_status status = DspServiceTransfer();
    if (status != LIB_STATUS_OK) return status;
    if (MpuBasePort && MidiStatus == LIB_STATUS_OK) MidiResult(SendMidiRequest());
    return MidiStatus;
}

void SbSetRunning(IBOOL running)
{
    DspSetRunning(running);
    if (!MpuBasePort) return;
    MidiResult(softpc_sb_midi_status());
    if (MidiStatus == LIB_STATUS_OK) {
        MidiResult(MidiSetRunning(running));
        if (running && MidiStatus == LIB_STATUS_OK) MidiResult(SendMidiRequest());
    }

    if (!running && MidiStatus != LIB_STATUS_OK) MidiResult(softpc_sb_midi_close());
}

static const IU16 SbPorts[] = { 0x224, 0x225, 0x226, 0x228, 0x229,
    0x22a, 0x22c, 0x22e, ADLIB_REGISTER_SELECT_PORT, ADLIB_DATA_PORT };

BOOL
SbInitialize(
    void
    )

/*++

Routine Description:

    This function performs SB initialization by installing I/O port and handler hooks.

Arguments:

    None.

Return Value:

    TRUE -  Initialization successful.
    FALSE - Otherwise.

--*/

{
    IU32 i;
    lib_status status;
    if (bSBAttached) return SbGetStatus() == LIB_STATUS_OK;

    for (i = 0; i < sizeof(SbPorts) / sizeof(SbPorts[0]); ++i) {
        IU16 port = SbPorts[i];
        if (Ios_in_adapter_table[port] != EMPTY_ADAPTOR ||
            Ios_out_adapter_table[port] != EMPTY_ADAPTOR) return FALSE;
    }
    BasePort = 0x220;
    SbInterrupt = VSB_INTERRUPT;
    SbDmaChannel = VSB_DMA_CHANNEL;
    if (!SbInitializeDMA()) return FALSE;
    bExitDMAThread = TRUE;
    ResetAll();
    if (DspServiceTransfer() != LIB_STATUS_OK) return FALSE;
    io_define_inb(SNDBLST_ADAPTER, VsbByteIn);
    io_define_outb(SNDBLST_ADAPTER, VsbByteOut);
    for (i = 0; i < sizeof(SbPorts) / sizeof(SbPorts[0]); ++i)
        io_connect_port(SbPorts[i], SNDBLST_ADAPTER, IO_READ_WRITE);
    bSBAttached = TRUE;

    if (Ios_in_adapter_table[MPU401_DATA_PORT] == EMPTY_ADAPTOR &&
        Ios_out_adapter_table[MPU401_DATA_PORT] == EMPTY_ADAPTOR &&
        Ios_in_adapter_table[MPU401_COMMAND_PORT] == EMPTY_ADAPTOR &&
        Ios_out_adapter_table[MPU401_COMMAND_PORT] == EMPTY_ADAPTOR) {
        status = softpc_sb_midi_open(MixerGetMidiGain());
        if (status == LIB_STATUS_OK) {
            MpuBasePort = MPU401_DATA_PORT;
            io_connect_port(MpuBasePort, SNDBLST_ADAPTER, IO_READ_WRITE);
            io_connect_port(MpuBasePort + 1, SNDBLST_ADAPTER, IO_READ_WRITE);
        } else if (status != LIB_STATUS_UNSUPPORTED) {
            MidiStatus = status;
            return FALSE;
        }
    }
    return TRUE;

}

IBOOL
SbTerminate(
    void
    )

/*++

Routine Description:

    This function performs SB cleanup.

Arguments:

    None.

Return Value:

    None.

--*/

{
    IU32 i;
    lib_status pcm_status, midi_status;
    if (bSBAttached) {
        StopDspTransfer(TRUE);
        for (i = 0; i < sizeof(SbPorts) / sizeof(SbPorts[0]); ++i)
            io_disconnect_port(SbPorts[i], SNDBLST_ADAPTER);
    }
    if (MpuBasePort) {
        io_disconnect_port(MpuBasePort, SNDBLST_ADAPTER);
        io_disconnect_port(MpuBasePort + 1, SNDBLST_ADAPTER);
        MpuBasePort = 0;
    }
    bSBAttached = FALSE;
    ResetMidiParser();
    pcm_status = DspCloseOutput();
    midi_status = softpc_sb_midi_close();
    return pcm_status == LIB_STATUS_OK && midi_status == LIB_STATUS_OK;
}

BOOL
InitializeIoAddresses(
    VOID
    )

/*++

Routine Description:

    This function reads environment variables to initialize IO addresses for
    DSP, MIXER, MIDI and ADLIB.  For the IO addresses which are not specified
    in the environment variable, we will use our default values.

Arguments:

    None.

Return Value:

    TRUE - initialization success.
    FALSE - failed.

--*/

{
    //
    // The SB is initialized before the env. variables are processed.
    // So, here we simply use the default value.
    //

    BasePort = 0x220;
    MpuBasePort = MPU401_DATA_PORT;
    SbInterrupt = VSB_INTERRUPT;
    SbDmaChannel = VSB_DMA_CHANNEL;

    return TRUE;
}

BOOL
LoadWinmm(
    VOID
    )

/*++

Routine Description:

    This function dynamically loads the "waveOutxxx" entry points. This
    is done because there is code in WINMM which does certain things in a
    WOW vdm. If we do static links, then winmm may get loaded way before
    WOW32, in which case it can't do the things it should.

Arguments:

    None.

Return Value:

    TRUE -  successful.
    FALSE - Otherwise.

--*/

{

    //
    // Load the Winmm.dll and grab all the desired function addresses.
    //

    if (!(hWinmm = LoadLibrary("WINMM.DLL"))) {
        return FALSE;
    }

    SetVolumeProc = (SETVOLUMEPROC) GetProcAddress(hWinmm, "waveOutSetVolume");
    GetVolumeProc = (GETVOLUMEPROC) GetProcAddress(hWinmm, "waveOutGetVolume");
    GetNumDevsProc = (GETNUMDEVSPROC) GetProcAddress(hWinmm, "waveOutGetNumDevs");
    GetDevCapsProc = (GETDEVCAPSPROC) GetProcAddress(hWinmm, "waveOutGetDevCapsA");
    OpenProc = (OPENPROC) GetProcAddress(hWinmm, "waveOutOpen");
    PauseProc = (PAUSEPROC) GetProcAddress(hWinmm, "waveOutPause");
    RestartProc = (RESTARTPROC) GetProcAddress(hWinmm, "waveOutRestart");
    ResetProc = (RESETPROC) GetProcAddress(hWinmm, "waveOutReset");
    CloseProc = (CLOSEPROC) GetProcAddress(hWinmm, "waveOutClose");
    GetPositionProc = (GETPOSITIONPROC) GetProcAddress(hWinmm, "waveOutGetPosition");
    WriteProc = (WRITEPROC) GetProcAddress(hWinmm, "waveOutWrite");
    PrepareHeaderProc = (PREPAREHEADERPROC) GetProcAddress(hWinmm, "waveOutPrepareHeader");
    UnprepareHeaderProc = (UNPREPAREHEADERPROC) GetProcAddress(hWinmm, "waveOutUnprepareHeader");

    SetMidiVolumeProc = (SETMIDIVOLUMEPROC) GetProcAddress(hWinmm, "midiOutSetVolume");
    GetMidiVolumeProc = (GETMIDIVOLUMEPROC) GetProcAddress(hWinmm, "midiOutGetVolume");
    MidiGetNumDevsProc = (MIDIGETNUMDEVSPROC) GetProcAddress(hWinmm, "midiOutGetNumDevs");
    MidiGetDevCapsProc = (MIDIGETDEVCAPSPROC) GetProcAddress(hWinmm, "midiOutGetDevCapsA");
    MidiOpenProc = (MIDIOPENPROC) GetProcAddress(hWinmm, "midiOutOpen");
    MidiResetProc = (MIDIRESETPROC) GetProcAddress(hWinmm, "midiOutReset");
    MidiCloseProc = (MIDICLOSEPROC) GetProcAddress(hWinmm, "midiOutClose");
    MidiLongMsgProc = (MIDILONGMSGPROC) GetProcAddress(hWinmm, "midiOutLongMsg");
    MidiShortMsgProc = (MIDISHORTMSGPROC) GetProcAddress(hWinmm, "midiOutShortMsg");
    MidiPrepareHeaderProc = (MIDIPREPAREHEADERPROC) GetProcAddress(hWinmm, "midiOutPrepareHeader");
    MidiUnprepareHeaderProc = (MIDIUNPREPAREHEADERPROC) GetProcAddress(hWinmm, "midiOutUnprepareHeader");

    //
    // Check to see if everyone is OK
    //

    if (SetVolumeProc && GetVolumeProc && GetNumDevsProc && GetDevCapsProc && OpenProc && ResetProc &&
        CloseProc && GetPositionProc && WriteProc && PrepareHeaderProc && SetMidiVolumeProc &&
        UnprepareHeaderProc && MidiGetNumDevsProc && MidiGetDevCapsProc && MidiOpenProc &&
        MidiResetProc && MidiCloseProc && MidiLongMsgProc && MidiShortMsgProc &&
        MidiPrepareHeaderProc && MidiUnprepareHeaderProc && GetMidiVolumeProc) {

        return TRUE;
    } else {
        dprintf1(("Can not get all the MM api entries"));
        return FALSE;
    }
}

VOID
DeInstallIoHook(
    VOID
    )

/*++

Routine Description:

    This function unhooks I/O ports that we connected during initialization.
    It is called when ntvdm is being terminated.

Arguments:

    None.

Return Value:

    None.

--*/

{
    DisconnectPorts(BasePort + 0x4, BasePort + 0x6);
    DisconnectPorts(BasePort + 0x8, BasePort + 0xA);
    DisconnectPorts(BasePort + 0xC, BasePort + 0xC);
    DisconnectPorts(BasePort + 0xE, BasePort + 0xE);
    DisconnectPorts(0x388, 0x389);
    if (MpuBasePort != 0) {
        DisconnectPorts(MpuBasePort, MpuBasePort + 1);
    }
}

VOID
VsbByteIn(
    WORD port,
    BYTE * data
    )

/*++

Routine Description:

    Gets called when the application reads from port.
    Returns results to application in data.

Arguments:

    port - the trapped I/O port address.

    data - Supplies the address of byte buffer to return data read from specified port.

Return Value:

    None.

--*/

{
    //
    // as if we fail simulate nothing at the port
    //

    *data = 0xFF;

    switch (port - BasePort) {
    case READ_STATUS:
        DspReadStatus(data);
        dprintf0(("r-RdSta %x", *data));
        break;

    case READ_DATA:
        DspReadData(data);
        dprintf0(("r-RdDt %x", *data));
        break;

    case WRITE_STATUS:

        //
        // If we are in High Speed DMA mode, DSP will not accept any
        // command/data.  So, in this case, we return not-ready.
        //
        if (bHighSpeedMode) {
            *data = 0xFF;
        } else {
            *data = 0x7F;
        }
        dprintf0(("r-WtSta %x", *data));
        break;

    case MIXER_ADDRESS:
        // apps sometimes read from this port??
        break;

    case MIXER_DATA:
        MixerDataRead(data);
        break;

    case 0x8:
        // remap to ADLIB_STATUS_PORT
        port = ADLIB_STATUS_PORT;
        break;
    }

    switch(port) {
    case MPU401_DATA_PORT:
    case ALT_MPU401_DATA_PORT:
        //
        // Don't support MIDI read.  Except that we support MPU reset in this case
        // we return 0xFE to indicate MPU is present
        //

        *data = MpuInData;
        MpuInData = 0xFF;
        dprintf0(("r-MPU RdDt %x", *data));
        break;

    case MPU401_COMMAND_PORT:
    case ALT_MPU401_COMMAND_PORT:

        //
        // always return data ready for reading and ready for command/data
        //
        if (MpuInData != 0xFF) {
            *data = 0x00;
        } else {
            *data = 0x80;
        }
        dprintf0(("r-MPU RdSt %x", *data));
        break;

    case ADLIB_STATUS_PORT:
        FMStatusRead(data);
        break;
    }

    dprintf4(("Read  %4X, <= %2X", port, *data));
}

VOID
VsbByteOut(
    WORD port,
    BYTE data
    )

/*++

Routine Description:

    Gets called when the application writes data to port.

Arguments:

    port - the trapped I/O port address.

    data - Supplies the data to be written to the specified port.

Return Value:

    None.

--*/

{
    dprintf4(("Write %4X, => %2X", port, data));

    switch (port - BasePort) {
    case RESET_PORT:
        dprintf0(("w-Reset %x", data));
        DspResetWrite(data);
        break;

    case WRITE_PORT:
        //
        // The DSP accepts command/data only when it is NOT in High Speed
        // DMA mode.
        //
        if (!bHighSpeedMode) {
            dprintf0(("w-wt %x", data));
            DspWrite(data);
        }
        break;

    case MIXER_ADDRESS:
        MixerAddrWrite(data);
        break;

    case MIXER_DATA:
        MixerDataWrite(data);
        break;

    case 0x8:
        // remap to ADLIB_REGISTER_SELECT_PORT
        port = ADLIB_REGISTER_SELECT_PORT;
        break;

    case 0x9:
        // remap to ADLIB_DATA_PORT
        port = ADLIB_DATA_PORT;
        break;
    }

    switch(port) {
    case MPU401_DATA_PORT:
    case ALT_MPU401_DATA_PORT:

        dprintf0(("w-MPU wtDt %x", data));
        switch (MpuMode) {
        case MPU_UART_MODE:
            if (MidiStatus == LIB_STATUS_OK) {
                lib_status status = BufferMidi(data);
                if (status == LIB_STATUS_UNSUPPORTED) softpc_sb_midi_report_oversize();
                MidiResult(status);
            }
            break;

        case MPU_INTELLIGENT_MODE:
            dprintf1(("App sending MPU data when in INTELLIGENT mode!  Data dumped!"));
            break;

        default:
            dprintf1(("Invalid MPU mode!"));
            break;
        }
        break;

    case MPU401_COMMAND_PORT:
    case ALT_MPU401_COMMAND_PORT:


        dprintf0(("r-MPU wtCmd %x", data));

        if (data == MPU_RESET || data == MPU_PASSTHROUGH_MODE) {
            MpuInData = 0xFE;
        }

        switch (MpuMode) {
        case MPU_UART_MODE:
            switch (data) {
            case MPU_RESET:
                dprintf2(("App Reseting MPU while in UART mode, switching to intelligent mode."));
                MpuMode=MPU_INTELLIGENT_MODE;
                ResetMidiParser();
                MidiResult(softpc_sb_midi_reset());
                dprintf2(("MPU Reset done."));
                break;

            default:
               // While in UART mode all the other commands are ignored.
               break;
            }
            break;

        case MPU_INTELLIGENT_MODE:
            switch (data) {
            case MPU_RESET:
                // Does nothing here.  While app read data port, we will return 0xfe.
                dprintf2(("Reseting MPU while in intelligent mode."));
                break;

            case MPU_PASSTHROUGH_MODE:
                dprintf2(("Switching MPU to UART (dumb) mode."));

                MpuMode = MPU_UART_MODE;
                break;

            default:
                // We don't recognize any other commands.
                dprintf2(("Unknown MPU401 command 0x%x sent while in intelligent mode!", data));
                break;
            }
            break;

        default:
            dprintf1(("Invalid MPU mode!"));
            break;
        }
        break;

    case ADLIB_REGISTER_SELECT_PORT:
        FMRegisterSelect(data);
        break;

    case ADLIB_DATA_PORT:
        FMDataWrite(data);
        break;
    }
}

VOID
ResetAll(
    VOID
    )

/*++

Routine Description:

    This function resets all devices.

Arguments:

    None.

Return Value:

    None.

--*/

{
    dprintf2(("Resetting"));
    //ResetMidiDevice();
    ResetMidiParser();
    MpuMode = MPU_INTELLIGENT_MODE;
    MpuInData = 0xFF;
    MidiStatus = softpc_sb_midi_reset();
    ResetFM();
    ResetMixer();

    //
    // Close WaveOut device after we finish reset mixer.
    //
    ResetDSP();
}

//
// Debugging stuff
//

//#if DBG

int DebugLevel = 2;
int DebugCount = 0;

#define DEBUG_START 0

void DbgOut(LPSTR lpszFormat, ...)

/*++

Routine Description:

    This function Generates debug output in printf type format.

Arguments:

    lpszFormat - supplies a pointer to a printf type format string.

    ... - other parameters for the format string.

Return Value:

    None.

--*/

{
    char buf[256];
    char buf2[300] = "VSB: ";
    va_list va;

    if (!IsDebuggerPresent() || (++DebugCount < DEBUG_START)) {
        return;
    }

    va_start(va, lpszFormat);
    wvsprintfA(buf, lpszFormat, va);
    va_end(va);

    strcat(buf2, buf);
    strcat(buf2, "\r\n");
    OutputDebugStringA(buf2);

}

//#endif  // DBG


