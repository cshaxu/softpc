#include "insignia.h"
#include "host_def.h"
#include TypesH

#include "xt.h"
#include CpuH
#include "bios.h"
#include "sas.h"
#include "tape_io.h"

/* The non-PM portion of the original BIOS INT 15h BOP service.  This machine
 * profile does not expose the removed NTVDM protected-mode product path; the
 * standard device-completion and basic AT BIOS calls remain the original
 * service semantics. */
void cassette_io()
{
    switch (getAH()) {
    case INT15_DEVICE_OPEN:
    case INT15_DEVICE_CLOSE:
    case INT15_PROGRAM_TERMINATION:
    case INT15_REQUEST_KEY:
    case INT15_DEVICE_BUSY:
        setAH(0);
        setCF(0);
        break;
    case INT15_EMS_DETERMINE:
        setAX(0);
        break;
    case INT15_INTERRUPT_COMPLETE:
        break;
    case INT15_CONFIGURATION:
        setES(getCS());
        setBX(CONF_TABLE_OFFSET);
        setAH(0);
        setCF(0);
        break;
    case INT15_JOYSTICK:
    case 0x24:
    case 0xd8:
    case 0x41:
    default:
        setCF(1);
        setAH(INT15_INVALID);
        break;
    }
}
