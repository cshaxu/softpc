#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else  // !NEC_98
#line 3

#include "insignia.h"
#include "host_def.h"
/*
 * SoftPC-AT Revision 2.0
 *
 * Title	: Parallel Printer BIOS functions.
 *
 * Description	: The Bios functions for printing a char, initializing the
 *		  printer and getting the printer status.  The low
 *		  level printer emulation is in printer.c.
 *
 * Author	: Henry Nash
 *
 * Mods: (r3.2) : The system directory /usr/include/sys is not available
 *                on a Mac running Finder and MPW. Bracket references to
 *                such include files by "#ifdef macintosh <Mac file> #else
 *                <Unix file> #endif".
 *
 *	(r3.3)	: Implement the real code, inside compile switch.
 */

/* @(#)printer_io.c	1.11 08/25/93 Copyright Insignia Solutions Ltd. */
#ifdef SCCSID
#error "Unsupported SoftPC mirror configuration: SCCSID"
#endif
#line 28

#ifdef SEGMENTATION
#error "Unsupported SoftPC mirror configuration: SEGMENTATION"
#endif
#line 37


/*
 *    O/S include files.
 */
#ifdef PRINTER
#include <stdio.h>

#include TypesH

#ifdef SYSTEMV
#error "Unsupported SoftPC mirror configuration: SYSTEMV"
#endif
#line 50

/*
 * SoftPC include files
 */
#include "xt.h"
#include CpuH
#include "sas.h"
#include "ios.h"
#include "bios.h"
#include "printer.h"
#include "trace.h"
#include "error.h"
#include "config.h"
#include "host_lpt.h"
#include "idetect.h"



#line 66
#if defined(NTVDM) && defined(MONITOR)
#error "Unsupported SoftPC mirror configuration: #if defined(NTVDM) && defined(MONITOR)"
#endif
#line 70
#if defined(NTVDM)
#error "Unsupported SoftPC mirror configuration: #if defined(NTVDM)"
#endif
#line 74
/*
 * Set up arrays of addresses in BIOS data area, pointing to LPT ports and timeout values
 */
static sys_addr port_address[] = {
			LPT1_PORT_ADDRESS,
			LPT2_PORT_ADDRESS,
			LPT3_PORT_ADDRESS };
static sys_addr timeout_address[] = {
			LPT1_TIMEOUT_ADDRESS,
			LPT2_TIMEOUT_ADDRESS,
			LPT3_TIMEOUT_ADDRESS };

#endif /* PRINTER */

#if defined(NTVDM) && defined(MONITOR)
#error "Unsupported SoftPC mirror configuration: excluded conjunction"
#endif
#line 91
/*
 * The printer bios consists of three functions:
 *
 * 	AH == 0		print char in AL
 *	AH == 1		initialize the printer
 *	AH == 2		get printer status
 *
 * The bios only supports programmed IO operations to the printer, although
 * the printer adaptor does support interrupts if Bit 4 is set in the control
 * register.
 */

void printer_io()
{
#ifdef PRINTER

    half_word time_out, status;
    word printer_io_address, printer_io_reg, printer_status_reg, printer_control_reg;
    boolean printer_busy = TRUE;
    unsigned long time_count;
    int adapter;

#ifdef NTVDM
#error "Unsupported SoftPC mirror configuration: NTVDM"
#endif
#line 131

    setIF(1);
    adapter = getDX() % NUM_PARALLEL_PORTS;
    sas_loadw(port_address[adapter], &printer_io_address);
    printer_io_reg = printer_io_address;
    printer_status_reg = printer_io_address + 1;
    printer_control_reg = printer_io_address + 2;

    sas_load(timeout_address[adapter], &time_out);
    time_count = time_out * 0xFFFF;

    if (printer_io_address != 0)
    {
		IDLE_comlpt ();

        switch(getAH())
        {
        case 0:
		/* Check the port status for busy before sending the character*/
		while(printer_busy && time_count > 0)
		{
		    /* The host_lpt_status() should check for status changes */
		    /* possibly by calling AsyncEventManager() if it's using */
		    /* XON /XOFF flow control. */
		    inb(printer_status_reg, &status);
		    if (status & 0x80)
			printer_busy = FALSE;
		    else
			time_count--;
		}

		if (printer_busy)
		{
		    status &= 0xF8;			/* clear bottom unused bits */
		    status |= 1;			/* set error flag	    */
		}
		else
		{
                    /* Only send the character if the port isn't still busy */
                    outb(printer_io_reg, getAL());
		    outb(printer_control_reg, 0x0D);	/* strobe low-high  	    */
		    outb(printer_control_reg, 0x0C);	/* strobe high-low  	    */
		    inb(printer_status_reg, &status);
		    status &= 0xF8;			/* clear unused bits	    */
		}

		status ^= 0x48;				/* flip the odd bit	    */
		setAH(status);
	        break;

        case 1: outb(printer_control_reg, 0x08);	/* set init line low	    */
                outb(printer_control_reg, 0x0C);	/* set init line high	    */
		inb(printer_status_reg, &status);
		status &= 0xF8;				/* clear unused bits	    */
		status ^= 0x48;				/* flip the odd bit	    */
		setAH(status);
	        break;

        case 2: inb(printer_status_reg, &status);
		status &= 0xF8;				/* clear unused bits	    */
		status ^= 0x48;				/* flip the odd bit	    */
		setAH(status);
	        break;

        default:
	         break;
	}
    }
#endif
}

#line 205
#if defined(NTVDM) && defined(MONITOR)
#error "Unsupported SoftPC mirror configuration: NTVDM and MONITOR"
#endif
#line 228
#if defined(NTVDM)
#error "Unsupported SoftPC mirror configuration: NTVDM"
#endif
#line 246
#endif // !NEC_98
