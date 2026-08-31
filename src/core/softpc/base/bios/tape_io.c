#include "insignia.h"
#include "host_def.h"
#include TypesH

#include "xt.h"
#include CpuH
#include "bios.h"
#include "sas.h"
#include "ios.h"
#include "ica.h"
#ifdef CPU_40_STYLE
#include "c_main.h"
#include "c_bsic.h"
#endif
#include "tape_io.h"

/* The non-PM portion of the original BIOS INT 15h BOP service.  This machine
 * profile does not expose the removed NTVDM protected-mode product path; the
 * standard device-completion and basic AT BIOS calls remain the original
 * service semantics. */
void cassette_io()
{
#ifdef PM
#ifdef CPU_40_STYLE
    CPU_DESCR source_descriptor;
    CPU_DESCR target_descriptor;
#else
    DESCR source_descriptor;
    DESCR target_descriptor;
#endif
    sys_addr descriptor_table;
    sys_addr source;
    sys_addr target;
    sys_addr byte_count;
#endif

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
    case INT15_MOVE_BLOCK:
#ifdef PM
        /* Original AT BIOS service: ES:SI names the descriptor table whose
           source and target descriptors begin at offsets 10h and 18h.  SAS
           already owns physical memory and A20, so retain the original
           descriptor contract while delegating the move to its native API. */
        descriptor_table = effective_addr(getES(), getSI());
        source = descriptor_table + 0x10;
        target = descriptor_table + 0x18;
#ifdef CPU_40_STYLE
        read_descriptor_linear(source, &source_descriptor);
        read_descriptor_linear(target, &target_descriptor);
#else
        read_descriptor(source, &source_descriptor);
        read_descriptor(target, &target_descriptor);
#endif
        byte_count = (sys_addr)getCX() << 1;

        if (byte_count > source_descriptor.limit + 1 ||
            byte_count > target_descriptor.limit + 1) {
            setCF(1);
            setAH(INT15_INVALID);
            break;
        }
        if (sas_twenty_bit_wrapping_enabled()) {
            sas_disable_20_bit_wrapping();
            sas_move_words_forward(source_descriptor.base,
                target_descriptor.base, byte_count >> 1);
            sas_enable_20_bit_wrapping();
        } else {
            sas_move_words_forward(source_descriptor.base,
                target_descriptor.base, byte_count >> 1);
        }
        setAH(0);
        setCF(0);
        setZF(1);
        setIF(1);
#else
        setCF(1);
        setAH(INT15_INVALID);
#endif
        break;
    case INT15_VIRTUAL_MODE:
#ifdef PM
        /* The remainder of this standard AT transition stays in the ROM.
           Restore the original controller and CPU preparation before the
           BOP returns to that Intel code. */
        setIF(0);
        sas_disable_20_bit_wrapping();
        outb(ICA0_PORT_0, (half_word)0x11);
        outb(ICA0_PORT_1, (half_word)getBH());
        outb(ICA0_PORT_1, (half_word)0x04);
        outb(ICA0_PORT_1, (half_word)0x01);
        outb(ICA0_PORT_1, (half_word)0xff);
        outb(ICA1_PORT_0, (half_word)0x11);
        outb(ICA1_PORT_1, (half_word)getBL());
        outb(ICA1_PORT_1, (half_word)0x02);
        outb(ICA1_PORT_1, (half_word)0x01);
        outb(ICA1_PORT_1, (half_word)0xff);
        setDS(getES());
#else
        setCF(1);
        setAH(INT15_INVALID);
#endif
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
