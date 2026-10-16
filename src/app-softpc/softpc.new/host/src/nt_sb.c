#include "insignia.h"
#include "host_def.h"
#include "ios.h"

#include "sndblst.h"
#include "nt_sb.h"
#include "compat/ccpu/lifecycle.h"
/* T93: XP default binding; native WinMM/NTVDM loading is not a device path. */
static IU16 BasePort = 0x220;
extern IBOOL bHighSpeedMode;
extern IBOOL bExitDMAThread;
IU16 SbInterrupt = VSB_INTERRUPT;
IU16 SbDmaChannel = VSB_DMA_CHANNEL;
static IBOOL bSBAttached;
static IU16 MpuBasePort;
static IU16 MpuMode = MPU_INTELLIGENT_MODE;
static IU8 MpuInData = 0xFF;
static lib_status MidiStatus;

/* T93: rejected/oversized guest input is not a host transport failure.
 * Cancellation retains the parser's message until the same executor resumes. */
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
    /* T93: ordinary reset rejects a faulted host output; explicit close still
     * performs its native cleanup. A fault must not leave old notes sounding. */
    if (!running && MidiStatus != LIB_STATUS_OK) MidiResult(softpc_sb_midi_close());
}
/* T93: one binding list for DSP/Mixer and the original FM aliases. */
static const IU16 SbPorts[] = { 0x224, 0x225, 0x226, 0x228, 0x229,
    0x22a, 0x22c, 0x22e, ADLIB_REGISTER_SELECT_PORT, ADLIB_DATA_PORT };

IBOOL SbInitialize(void)
{
    IU32 i;
    lib_status status;
    if (bSBAttached) return SbGetStatus() == LIB_STATUS_OK;
    /* The existing I/O tables are the binding authority, not a second map. */
    for (i = 0; i < sizeof(SbPorts) / sizeof(SbPorts[0]); ++i) {
        IU16 port = SbPorts[i];
        if (Ios_in_adapter_table[port] != EMPTY_ADAPTOR ||
            Ios_out_adapter_table[port] != EMPTY_ADAPTOR) return FALSE;
    }
    SbInterrupt = VSB_INTERRUPT;
    SbDmaChannel = VSB_DMA_CHANNEL;
    if (!SbInitializeDMA()) return FALSE;
    bExitDMAThread = TRUE; /* A new machine lifetime requires the full reset. */
    ResetAll();
    if (DspServiceTransfer() != LIB_STATUS_OK) return FALSE;
    io_define_inb(SNDBLST_ADAPTER, VsbByteIn);
    io_define_outb(SNDBLST_ADAPTER, VsbByteOut);
    for (i = 0; i < sizeof(SbPorts) / sizeof(SbPorts[0]); ++i)
        io_connect_port(SbPorts[i], SNDBLST_ADAPTER, IO_READ_WRITE);
    bSBAttached = TRUE;
    /* XP treats MPU admission as optional, independent of DSP/FM. */
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

IBOOL SbTerminate(void)
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

void
VsbByteIn(
    IU16 port,
    IU8 * data
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
        break;

    case READ_DATA:
        DspReadData(data);
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

    switch (port) {
    case MPU401_DATA_PORT:
        *data = MpuInData;
        MpuInData = 0xFF;
        break;
    case MPU401_COMMAND_PORT:
        *data = MpuInData != 0xFF ? 0x00 : 0x80;
        break;
    case ADLIB_STATUS_PORT:
        FMStatusRead(data);
        break;
    }
}
void
VsbByteOut(
    IU16 port,
    IU8 data
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
    switch (port - BasePort) {
    case RESET_PORT:
        DspResetWrite(data);
        break;

    case WRITE_PORT:
        //
        // The DSP accepts command/data only when it is NOT in High Speed
        // DMA mode.
        //
        if (!bHighSpeedMode) {
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

    switch (port) {
    case MPU401_DATA_PORT:
        if (MpuMode == MPU_UART_MODE && MidiStatus == LIB_STATUS_OK) {
            lib_status status = BufferMidi(data);
            if (status == LIB_STATUS_UNSUPPORTED) softpc_sb_midi_report_oversize();
            MidiResult(status);
        }
        break;
    case MPU401_COMMAND_PORT:
        if (data == MPU_RESET || data == MPU_PASSTHROUGH_MODE)
            MpuInData = 0xFE;
        switch (MpuMode) {
        case MPU_UART_MODE:
            if (data == MPU_RESET) {
                MpuMode = MPU_INTELLIGENT_MODE;
                ResetMidiParser();
                MidiResult(softpc_sb_midi_reset());
            }
            break;
        case MPU_INTELLIGENT_MODE:
            if (data == MPU_PASSTHROUGH_MODE) MpuMode = MPU_UART_MODE;
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
void
ResetAll(
    void
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
