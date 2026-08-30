#include "insignia.h"
#include "host_def.h"

/* Original SoftPC CMOS BIOS access bridge.  It deliberately talks through
   the original CMOS controller's I/O ports rather than accessing its state
   directly. */
#include "xt.h"
#include "bios.h"
#include "sas.h"
#include "cmos.h"
#include "cmosbios.h"
#include "ios.h"
#include "rtc_bios.h"

#ifndef NTVDM
#define BCD_TO_BIN(n) ((n & 0x0f) + (((n >> 4) & 0x0f) * 10))

void set_tod(void)
{
    half_word value;
    DOUBLE_TIME timer_tics;
    double_word tics_temp;

    sas_storew(TIMER_LOW, 0);
    sas_storew(TIMER_HIGH, 0);
    sas_store(TIMER_OVFL, 0);

    value = cmos_read(CMOS_SECONDS + NMI_DISABLE);
    if (value > 0x59) {
        value = cmos_read(CMOS_DIAG + NMI_DISABLE);
        cmos_write(CMOS_DIAG + NMI_DISABLE, value | CMOS_CLK_FAIL);
        return;
    }
    tics_temp = BCD_TO_BIN(value) * 73;
    tics_temp /= 4;
    timer_tics.total = tics_temp;

    value = cmos_read(CMOS_MINUTES + NMI_DISABLE);
    if (value > 0x59) {
        value = cmos_read(CMOS_DIAG + NMI_DISABLE);
        cmos_write(CMOS_DIAG + NMI_DISABLE, value | CMOS_CLK_FAIL);
        return;
    }
    tics_temp = BCD_TO_BIN(value) * 2185;
    tics_temp /= 2;
    timer_tics.total += tics_temp;

    value = cmos_read(CMOS_HOURS + NMI_DISABLE);
    if (value > 0x23) {
        value = cmos_read(CMOS_DIAG + NMI_DISABLE);
        cmos_write(CMOS_DIAG + NMI_DISABLE, value | CMOS_CLK_FAIL);
        return;
    }
    timer_tics.total += BCD_TO_BIN(value) * 65543L;
    sas_storew(TIMER_LOW, timer_tics.half.low);
    sas_storew(TIMER_HIGH, timer_tics.half.high);
}
#endif

half_word cmos_read(table_address)
half_word table_address;
{
    half_word value;
    outb(CMOS_PORT, table_address | NMI_DISABLE);
    inb(CMOS_DATA, &value);
    outb(CMOS_PORT, CMOS_SHUT_DOWN | (table_address & NMI_DISABLE));
    return value;
}

void cmos_write(table_address, value)
half_word table_address;
half_word value;
{
    outb(CMOS_PORT, table_address | NMI_DISABLE);
    outb(CMOS_DATA, value);
    outb(CMOS_PORT, CMOS_SHUT_DOWN | (table_address & NMI_DISABLE));
}
