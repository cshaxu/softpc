#include "insignia.h"
#include "host_def.h"
#include "ios.h"

#include "sndblst.h"
#include "nt_sb.h"
/* T93: XP default binding; native WinMM/NTVDM loading is not a device path. */
static IU16 BasePort = 0x220;
extern IBOOL bHighSpeedMode;
extern IBOOL bExitDMAThread;
IU16 SbInterrupt = VSB_INTERRUPT;
IU16 SbDmaChannel = VSB_DMA_CHANNEL;
static IBOOL bSBAttached;
/* T93: selected DSP/Mixer ports; FM/MPU remain with their device admission. */
static IU8 SbPorts[] = { MIXER_ADDRESS, MIXER_DATA, RESET_PORT,
    READ_DATA, WRITE_PORT, READ_STATUS };

IBOOL SbInitialize(void)
{
    IU32 i;
    if (bSBAttached) return TRUE;
    /* The existing I/O tables are the binding authority, not a second map. */
    for (i = 0; i < sizeof(SbPorts) / sizeof(SbPorts[0]); ++i) {
        IU16 port = BasePort + SbPorts[i];
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
        io_connect_port(BasePort + SbPorts[i], SNDBLST_ADAPTER, IO_READ_WRITE);
    bSBAttached = TRUE;
    return TRUE;
}

IBOOL SbTerminate(void)
{
    IU32 i;
    if (!bSBAttached) return DspCloseOutput() == LIB_STATUS_OK;
    StopDspTransfer(TRUE);
    for (i = 0; i < sizeof(SbPorts) / sizeof(SbPorts[0]); ++i)
        io_disconnect_port(BasePort + SbPorts[i], SNDBLST_ADAPTER);
    bSBAttached = FALSE;
    return DspServiceTransfer() == LIB_STATUS_OK;
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
    ResetMixer();

    //
    // Close WaveOut device after we finish reset mixer.
    //
    ResetDSP();
}
