#include "insignia.h"
#include "host_def.h"

#include "xt.h"
#include "ios.h"
#include "sas.h"

/* Original slave-ICA BIOS services.  The standalone machine keeps this
 * controller code intact; only the former product BOP table is replaced. */

#define intr_flag 0x46b

void D11_int()
{
    half_word level, level2, mask;

    outb(ICA0_PORT_0, 0x0B);
    inb(ICA0_PORT_0, &level);

    if (level == 0)
    {
        level = 0xff;
    }
    else
    {
        outb(ICA1_PORT_0, 0x0B);
        inb(ICA1_PORT_0, &level2);

        if (level2 == 0)
        {
            inb(ICA0_PORT_1, &mask);
            level &= 0xfb;
            mask |= level;
            outb(ICA0_PORT_1, mask);
        }
        else
        {
            inb(ICA1_PORT_1, &mask);
            mask |= level2;
            outb(ICA1_PORT_1, mask);
            outb(ICA1_PORT_1, 0x20);
        }
        outb(ICA0_PORT_0, 0x20);
    }
    sas_store(intr_flag, level);
}

void re_direct()
{
    outb(ICA1_PORT_0, 0x20);
}

void int_287()
{
    outb(0xf0, 0);
    outb(ICA1_PORT_0, 0x20);
    outb(ICA0_PORT_0, 0x20);
}
