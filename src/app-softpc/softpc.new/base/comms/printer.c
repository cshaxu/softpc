#include "insignia.h"
#include "host_def.h"
#ifdef PRINTER

/*
 * VPC-XT Revision 1.0
 *
 * Title:	Parallel Printer Port Emulation
 *
 * Description:	Emulates the IBM || Printer card as used in the original
 *		IBM XT, which is itself a H/W emulation of an Intel 8255.
 *
 * Author:	Henry Nash
 *
 * Notes:	None
 *
 * Mods:
 *		<chrisP 11Sep91>
 *		Allow transition to NOTBUSY in the OUTA state as well as the READY
 *		state.  i.e. at the leading edge of the ACK pulse after just one
 *		INB (STATUS) rather than two.  Our printer port emulation relies on
 *		these INB's to toggle the ACK line and set NOTBUSY true again.  So
 *		the port could be left in the BUSY condition at the end of an app's
 *		print job (which can confuse the next print request).  NOTE we could
 *		still have a problem if the PC app bypasses the BIOS and is too lazy
 *		to do even one INB(STATUS) after the last print byte.
 */



/* for NTVDM port -- williamh
 * There are such things called Dongles which many software companies have
 * used for copy protection. Each software comes with its dedicated Dongle
 * that records necessary indentification inforamtion. It is required
 * to plug the device onto the parallel port in order to run the software
 * correctly. The device has an outlet which can be connectted to parallel
 * port printer so the the user doesn't sacrifice his parallel port when
 * the device in plugged on the original connector.
 * There are several Dongle vendors and each one of them provides their
 * propietary library or driver for the applications to link to. These
 * drivers know how to read/WRITE the Dongle to verify a legitmate copy.
 * Since it has to maintain compatiblility with standard PC parallel port,
 * the devices are designed in a way that it can be programmed without
 * disturbing ordinary parallel port operation. To do this, it usually does
 * this:
 * (1) Turn off strobe.
 * (2) output data pattern to data port
 * (3) delay a little bit(looping in instructions) and then go to (2)
 *     until the chunk of data has been sent. NOTE THAT THE STROBE LINE
 *     IS NEVER "strobe"
 * (4). Read status port and by interpreting the line differently,
 *     the driver decodes any id information it is looking for.
 *
 * In order to support these devices, we have to do these:
 * (1). We can not fake printer status. We have to get the real
 *	status line states.
 * (2). we have to output data to the printer without waiting the data
 *	to be qualified(strobing).
 * (3). We must be smart enough to detect the application is done with
 *	its Dongle things and wants everything goes back to normal.
 *	We must adjust ourselves under this circumestances.
 * (4). Down level printer driver must provide function that we can call
 *	to control the port directly.
 * (5). Printer h/w interrupt is not allowed to be enabled under this
 *	circumstance --and how can we make sure of that?????
 *
 */

/* @(#)printer.c	1.19 11/14/94 Copyright Insignia Solutions Ltd. */
#ifdef SCCSID
#error "Unsupported SoftPC mirror configuration: SCCSID"
#endif
#line 72

#ifdef SEGMENTATION
#error "Unsupported SoftPC mirror configuration: SEGMENTATION"
#endif
#line 81


/*
 *    O/S include files.
 */
#include <stdio.h>
#include TypesH
#include TimeH
#ifdef SYSTEMV
#error "Unsupported SoftPC mirror configuration: SYSTEMV"
#endif
#line 94

/*
 * SoftPC include files
 */
#include "xt.h"
#include CpuH
#include "sas.h"
#include "ios.h"
#include "bios.h"
#include "printer.h"
#include "error.h"
#include "config.h"
#include "host_lpt.h"
#include "ica.h"
#include "quick_ev.h"
#include "compat/devices/snapshot.h"

#include "debug.h"
#ifndef PROD
#error "Unsupported SoftPC mirror configuration: !PROD"
#endif
#line 114


/*
 * ============================================================================
 * Global data
 * ============================================================================
 */


/*
 * ============================================================================
 * Static data and defines
 * ============================================================================
 */

#line 129
#ifndef NEC_98
#line 130
#define PRINTER_BIT_MASK	0x3	/* bits decoded from address bus */
#define CONTROL_REG_MASK	0xE0;	/* unused bits drift to HIGH */
#define STATUS_REG_MASK		0x07;	/* unused bits drift to HIGH */
#endif // !NEC_98
#line 134

#define DATA_OFFSET	0		/* ouput register */
#define STATUS_OFFSET	1		/* status register */
#define CONTROL_OFFSET	2		/* control register */

#ifdef	ERROR
#undef	ERROR
#endif

#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else  // !NEC_98
#line 167
static half_word output_reg[NUM_PARALLEL_PORTS];
static half_word control_reg[NUM_PARALLEL_PORTS];
#define NOTBUSY		(IU8)0x80
#define ACK		(IU8)0x40
#define PEND		(IU8)0x20
#define SELECT		(IU8)0x10
#define ERROR		(IU8)0x08

static half_word status_reg[NUM_PARALLEL_PORTS];
#define IRQ		(IU8)0x10
#define SELECT_IN	(IU8)0x08
#define INIT_P		(IU8)0x04
#define AUTO_FEED	(IU8)0x02
#define STROBE		(IU8)0x01

LOCAL IU8 retryErrorCount = 0;   /* num status inb before clearing ERROR */

static IU8 state[NUM_PARALLEL_PORTS]; /* state control variable */
/*
 * set up arrays of all port addresses
 */
static io_addr port_start[] = {LPT1_PORT_START,LPT2_PORT_START,LPT3_PORT_START};
static io_addr port_end[] = {LPT1_PORT_END, LPT2_PORT_END, LPT3_PORT_END};
static int port_no[] = {LPT1_PORT_START & LPT_MASK, LPT2_PORT_START & LPT_MASK,
			LPT3_PORT_START & LPT_MASK };
static half_word lpt_adapter[] = {LPT1_ADAPTER, LPT2_ADAPTER, LPT3_ADAPTER};
static sys_addr port_address[] = {LPT1_PORT_ADDRESS, LPT2_PORT_ADDRESS, LPT3_PORT_ADDRESS};
static sys_addr timeout_address[] = {LPT1_TIMEOUT_ADDRESS, LPT2_TIMEOUT_ADDRESS, LPT3_TIMEOUT_ADDRESS};
static q_ev_handle handle_for_out_event[NUM_PARALLEL_PORTS];
static q_ev_handle handle_for_outa_event[NUM_PARALLEL_PORTS];
#endif // !NEC_98
#line 172


#line 199
#ifndef NEC_98
#line 200
#if defined(NTVDM) && defined(MONITOR)
#error "Unsupported SoftPC mirror configuration: NTVDM and MONITOR"
#endif
#endif // !NEC_98
#line 209
#define STATE_READY     0
#define STATE_OUT       1
#define STATE_OUTA      2

#line 212
#if defined(NTVDM)
#error "Unsupported SoftPC mirror configuration: NTVDM"
#endif
#line 217
/*
 * State transitions:
 *
 *	    +->	 STATE_READY
 *	    |	  |
 *          |     | ........ write char to output_reg, print on low-high strobe
 *	    |	  V          set NOTBUSY to false
 *	    |	 STATE_OUT
 *	    |	  |
 *	    |	  | ........ (read status) set ACK low
 *	    |	  V
 *	    |	 STATE_OUTA
 *	    |	  |
 *	    |	  | ........ (read status) set ACK high
 *	    +-----+
 *
 *	Caveat: if the control register interrupt request bit is set,
 *	we assume that the application isn't interested in getting the
 *	ACKs and just wants to know when the printer state changes back
 *	to NOTBUSY. I'm not sure to want extent you can get away with
 *	this: however, applications using the BIOS printer services
 *	should be OK.
 */

#ifdef PS_FLUSHING
#error "Unsupported SoftPC mirror configuration: PS_FLUSHING"
#endif	/* PS_FLUSHING */
#line 206

static void lpr_state_outa_event IPT1(long, adapter);
static void lpr_state_out_event IPT1(long, adapter);

/*
 * DIVERGENCE(MVDM-PARALLEL-SNAPSHOT-001): export the selected parallel
 * controller's finite register and delayed-event state.  The standalone
 * virtual printer buffer is owned separately by compat/parallel.c.
 */
int
softpc_device_snapshot_encode_parallel_callback(callback, callback_id)
Q_CALLBACK_FN callback;
unsigned long *callback_id;
{
	if (callback_id == NULL) return FALSE;
	if (callback == lpr_state_out_event) {
		*callback_id = SOFTPC_DEVICE_QUEUE_PRINTER_OUT;
		return TRUE;
	}
	if (callback != lpr_state_outa_event) return FALSE;
	*callback_id = SOFTPC_DEVICE_QUEUE_PRINTER_OUT_ACK;
	return TRUE;
}

Q_CALLBACK_FN
softpc_device_snapshot_decode_parallel_callback(callback_id)
unsigned long callback_id;
{
	if (callback_id == SOFTPC_DEVICE_QUEUE_PRINTER_OUT)
		return lpr_state_out_event;
	return callback_id == SOFTPC_DEVICE_QUEUE_PRINTER_OUT_ACK ?
		lpr_state_outa_event : NULL;
}

int
softpc_device_snapshot_capture_parallel_controller(state_out)
softpc_device_parallel_controller_state *state_out;
{
	int adapter;
	if (state_out == NULL) return FALSE;
	for (adapter = 0; adapter < NUM_PARALLEL_PORTS; ++adapter) {
		state_out->output[adapter] = output_reg[adapter];
		state_out->control[adapter] = control_reg[adapter];
		state_out->status[adapter] = status_reg[adapter];
		state_out->state[adapter] = state[adapter];
		state_out->out_event[adapter] = handle_for_out_event[adapter];
		state_out->out_ack_event[adapter] = handle_for_outa_event[adapter];
	}
	state_out->retry_error_count = retryErrorCount;
	return TRUE;
}

int
softpc_device_snapshot_restore_parallel_controller(state_in)
const softpc_device_parallel_controller_state *state_in;
{
	int adapter;
	if (state_in == NULL) return FALSE;
	for (adapter = 0; adapter < NUM_PARALLEL_PORTS; ++adapter) {
		if (state_in->state[adapter] < STATE_READY ||
			state_in->state[adapter] > STATE_OUTA ||
			state_in->out_event[adapter] < 0 ||
			state_in->out_ack_event[adapter] < 0)
			return FALSE;
	}
	for (adapter = 0; adapter < NUM_PARALLEL_PORTS; ++adapter) {
		output_reg[adapter] = state_in->output[adapter];
		control_reg[adapter] = state_in->control[adapter];
		status_reg[adapter] = state_in->status[adapter];
		state[adapter] = state_in->state[adapter];
		handle_for_out_event[adapter] = (q_ev_handle)state_in->out_event[adapter];
		handle_for_outa_event[adapter] =
			(q_ev_handle)state_in->out_ack_event[adapter];
	}
	retryErrorCount = state_in->retry_error_count;
	return TRUE;
}


/*
 * ============================================================================
 * Internal functions & macros
 * ============================================================================
 */

#define set_low(val, bit)		val &= ~bit
#define set_high(val, bit)		val |=  bit
#define low_high(val1, val2, bit)	(!(val1 & bit) && (val2 & bit))
#define high_low(val1, val2, bit)	((val1 & bit) && !(val2 & bit))
#define toggled(val1, val2, bit)	((val1 & bit) != (val2 & bit))
#define negate(val, bit)		val ^= bit

/*
 * Defines and variables to handle tables stored in 16-bit code for NT
 * monitors.
 */

#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else  // !NEC_98
#if defined(NTVDM) && defined(MONITOR)
#error "Unsupported SoftPC mirror configuration: NTVDM and MONITOR"
#else /* NTVDM && MONITOR */
#line 288
#define get_status(adap)	(status_reg[adapter])
#define set_status(adap,val)	(status_reg[adapter] = (val))

#define get_control(adap)	(control_reg[adapter])
#define set_control(adap,val)	(control_reg[adapter] = (val))

#define get_state(adap)		(state[adapter])
#define set_state(adap,val)	(state[adapter] = (val))
#endif /* NTVDM && MONITOR */
#line 297


static void printer_inb IPT2(io_addr, port, half_word *, value);
static void printer_outb IPT2(io_addr, port, half_word, value);
static void notbusy_check IPT1(int,adapter);
#endif // !NEC_98
#line 303

/*
 * ============================================================================
 * External functions
 * ============================================================================
 */

void printer_post IFN1(int,adapter)
{
#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else  // !NEC_98
#line 315
	/*
	 * Set up BIOS data area.
	 */
	sas_storew(port_address[adapter],(word)port_start[adapter]);
	sas_store(timeout_address[adapter], (half_word)0x14 );		/* timeout */
#endif // !NEC_98
#line 321
}


#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else  // !NEC_98
#if defined(NTVDM) && defined(MONITOR)
#error "Unsupported SoftPC mirror configuration: NTVDM and MONITOR"
#else	/* NTVDM && MONITOR */
#line 351
static void lpr_state_outa_event IFN1(long,adapter)
{
	set_high(status_reg[adapter],ACK);
	state[adapter]=STATE_READY;
}

static void lpr_state_out_event IFN1(long,adapter)
{
	set_low(status_reg[adapter], ACK);
	state[adapter]=STATE_OUTA;
	handle_for_outa_event[adapter]=add_q_event_t(lpr_state_outa_event,HOST_PRINTER_DELAY,adapter);
}
#endif	/* NTVDM && MONITOR */
#endif // !NEC_98
#line 345

#line 366
#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else
#line 369
static void printer_inb IFN2(io_addr,port, half_word *,value)
#endif
#line 371
{
#ifndef NEC_98
#line 373
	int	adapter, i;

	note_trace1(PRINTER_VERBOSE,"inb from printer port %#x ",port);
	/*
	** Scan the ports to find out which one is used. NB the
	** port must be valid one because we only used io_define_inb()
	** for the valid ports
	*/
	for(i=0; i < NUM_PARALLEL_PORTS; i++)
		if((port & LPT_MASK) == port_no[i])
			break;
        adapter = i % NUM_PARALLEL_PORTS;
		
	port = port & PRINTER_BIT_MASK;		/* clear unused bits */
#endif // !NEC_98
#line 388

	switch(port)
	{
#line 391
#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else
#line 395
	case DATA_OFFSET:
                *value = output_reg[adapter];
#endif
#line 398
		break;

#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else  // !NEC_98
#line 407
	case STATUS_OFFSET:
		switch(get_state(adapter))
		{

#line 410
#if defined(NTVDM)
#error "Unsupported SoftPC mirror configuration: #if defined(NTVDM)"
#endif
#line 420
		case STATE_READY:
			notbusy_check(adapter);
                        *value = get_status(adapter) | STATUS_REG_MASK;


                        /* Clear ERROR as it will be set if we fail on the print. */
                        /* Clear after two inbs as DOS seems to require this. */
                        if (retryErrorCount > 0)
                            retryErrorCount--;
                        else
                            set_status(adapter, (IU8)(get_status(adapter) | ERROR));
			break;
    	case STATE_OUT:
			*value = get_status(adapter) | STATUS_REG_MASK;
#ifndef DELAYED_INTS
			delete_q_event(handle_for_out_event[adapter]);
                        lpr_state_out_event(adapter);
#else
#error "Unsupported SoftPC mirror configuration: DELAYED_INTS"
#endif /* DELAYED INTS */
#line 441
			break;
    	case STATE_OUTA:
			notbusy_check(adapter);		/* <chrisP 11Sep91> */
			*value = get_status(adapter) | STATUS_REG_MASK;
#ifndef DELAYED_INTS
			delete_q_event(handle_for_outa_event[adapter]);
			lpr_state_outa_event(adapter);
#else
#error "Unsupported SoftPC mirror configuration: DELAYED_INTS"
#endif
#line 452
			break;
    	default:	
			note_trace1(PRINTER_VERBOSE,
			            "<pinb() - unknown state %x>",
			            get_state(adapter));
			break;
		}
#endif // !NEC_98
#line 460
		break;
#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else  // !NEC_98
#line 465
	case CONTROL_OFFSET:
		*value = get_control(adapter) | CONTROL_REG_MASK;
		negate(*value, STROBE);
		negate(*value, AUTO_FEED);
		negate(*value, SELECT_IN);
#endif // !NEC_98
#line 471
		break;
	}
#ifndef NEC_98
#line 474
	note_trace3(PRINTER_VERBOSE, "<pinb() %x, ret %x, state %x>",
		    port, *value, get_state(adapter));
#endif // !NEC_98
#line 477
}

#line 479
#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else
#line 482
static void printer_outb IFN2(io_addr,port, half_word,value)
#endif
#line 484
{
#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else  // !NEC_98
#line 488
	int	adapter, i;
	half_word old_control;
#ifdef PC_CONFIG
#error "Unsupported SoftPC mirror configuration: PC_CONFIG"
#endif
#line 498


	note_trace2(PRINTER_VERBOSE,"outb to printer port %#x with value %#x",
	            port, value);

	/*
	** Scan the ports to find out which one is used. NB the
	** port must be valid one because we only used io_define_inb()
	** for the valid ports
	*/
	for(i=0; i < NUM_PARALLEL_PORTS; i++)
		if((port & LPT_MASK) == port_no[i])
			break;
	adapter = i % NUM_PARALLEL_PORTS; 			

	note_trace3(PRINTER_VERBOSE, "<poutb() %x, val %x, state %x>",
		    port, value, get_state(adapter));

	port = port & PRINTER_BIT_MASK;		/* clear unused bits */

	switch(get_state(adapter))
	{
#line 520
#if defined(NTVDM)
#error "Unsupported SoftPC mirror configuration: #if defined(NTVDM)"
#endif
#line 554
	case STATE_OUT:
	case STATE_OUTA:
	case STATE_READY:
#endif // !NEC_98
#line 558
		switch(port)
		{
#line 560
#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else // !NEC_98
#line 564
		case DATA_OFFSET:
#line 568
#line 565
#if defined(NTVDM)
#error "Unsupported SoftPC mirror configuration: #if defined(NTVDM)"
#endif
#line 568
			/* Write char to internal buffer */
			output_reg[adapter] = value;
#endif // !NEC_98
#line 571
			break;
#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else  // !NEC_98
#line 614

		case STATUS_OFFSET:
			/* Not possible */
			break;

		case CONTROL_OFFSET:
			/* Write control bits */
			old_control = get_control(adapter);	/* Save old value to see what's changed */
			set_control(adapter, value);
#endif // !NEC_98
#line 624
#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else  // !NEC_98
#line 628
			if (low_high(old_control, value, INIT_P))
#ifdef PC_CONFIG
#error "Unsupported SoftPC mirror configuration: PC_CONFIG"
#else
#line 635
				/* this was a call to host_print_doc - <chrisP 28Aug91> */
				host_reset_print(adapter);
#endif

			if (toggled(old_control, value, AUTO_FEED))
				host_print_auto_feed(adapter,
					((value & AUTO_FEED) != 0));

			if (low_high(old_control, value, STROBE))
#endif // !NEC_98
#line 645
			{

#line 646
#ifndef NEC_98
#line 647
#if defined(NTVDM)
#error "Unsupported SoftPC mirror configuration: NTVDM"
#endif
#endif // !NEC_98
#line 660
#ifdef PS_FLUSHING
#error "Unsupported SoftPC mirror configuration: PS_FLUSHING"
#endif	/* PS_FLUSHING */
#line 670
				       /*
				 	* Send the stored internal buffer to
				 	* the printer
				 	*/
#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else  // !NEC_98
#line 682
                                	if(host_print_byte(adapter,output_reg[adapter]) == FALSE)
					{
				    		set_status(adapter, (IU8)(get_status(adapter) & ~ERROR)); /* active Low */
				    		/* NTVDM had here(?): set_status(adapter, ACK|PEND|SELECT|ERROR); */
				    		/* two status inbs before we clear ERROR */
				    		retryErrorCount = 2;
					}
					else
					{
                                    		/* clear ERROR condition */
                                    		set_status(adapter, (IU8)(get_status(adapter) | ERROR));
#if defined(NTVDM) && !defined(MONITOR)
#error "Unsupported SoftPC mirror configuration: defined(NTVDM) && !defined(MONITOR)"
#else /* NTVDM && !MONITOR */
#line 696
				    		set_status(adapter,
					       	(IU8)(get_status(adapter) & ~NOTBUSY));
#endif /* NTVDM && !MONITOR */
#line 699
				    		set_state(adapter, STATE_OUT);
#ifndef DELAYED_INTS
				    		handle_for_out_event[adapter]=add_q_event_t(lpr_state_out_event,HOST_PRINTER_DELAY,adapter);
#endif /* DELAYED_INTS */
					}
#endif // !NEC_98
#line 705
#ifdef PS_FLUSHING
#error "Unsupported SoftPC mirror configuration: PS_FLUSHING"
#endif	/* PS_FLUSHING */
#line 708
			}
#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else  // !NEC_98
#line 713
			else if (high_low(old_control, value, STROBE)
				 	&& get_state(adapter) == STATE_OUT)
#endif // !NEC_98
#line 716
			{
#line 717
#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else
#line 720
				if (value & IRQ)
#endif
#line 722
				{
					/*
					 * Application is using
					 * interrupts, so we can't
					 * rely on INBs being
					 * used to check the
					 * printer status.
					 */
#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else  // !NEC_98
#line 735
					set_state(adapter, STATE_READY);
					notbusy_check(adapter);
#endif // !NEC_98
#line 738
				}
			}


#line 741
#ifndef NEC_98
#line 742
#if defined(NTVDM)
#error "Unsupported SoftPC mirror configuration: NTVDM"
#endif
#endif // !NEC_98
#line 753
#ifndef NEC_98
#line 754
#ifndef	PROD
#error "Unsupported SoftPC mirror configuration: !PROD"
#endif
#endif // !NEC_98
#line 760
			break;
		}
#ifndef NEC_98
#line 763
		break;
	default:	
		note_trace1(PRINTER_VERBOSE, "<poutb() - unknown state %x>",
		            get_state(adapter));
		break;
	}
#endif // !NEC_98
#line 770
}

void printer_status_changed IFN1(int,adapter)
{
	note_trace1(PRINTER_VERBOSE, "<printer_status_changed() adapter %d>",
	            adapter);

	/* check whether the printer has just changed state to NOTBUSY */
#line 778
#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else
#line 781
	notbusy_check(adapter);
#endif
#line 783
}

/*
 * ============================================================================
 * Internal functions
 * ============================================================================
 */

#line 791
#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else
#line 794
static void notbusy_check IFN1(int,adapter)
#endif
#line 796
{
	/*
	 *	This function is used to detect when the printer
	 *	state transition to NOTBUSY occurs.
	 *
	 *	If the parallel port is being polled, the port
	 *	emulation will stop this transition occurring
	 *	until the application has detected the ACK
	 *	pulse. notbusy_check() is then called each time the
	 *	port status is read using the INB; when the host
	 *	says the printer is HOST_LPT_BUSY, the port status
	 *	returns to the NOTBUSY state.
	 *
	 *	If the parallel port is interrupt driven, we cannot
	 *	rely on the application using the INB: so we first
	 *	check the host printer status immediately after
	 *	outputting the character. If the host printer isn't
	 *	HOST_LPT_BUSY, then we interrupt immediately;
	 *	otherwise, we rely on the printer_status_changed()
	 *	call to notify us of when HOST_LPT_BUSY is cleared.
	 */

	/* <chrisP 11Sep91> allow not busy at leading edge of ack pulse too */
#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else  // !NEC_98
#line 848
	if (	 (get_state(adapter) == STATE_READY ||
#line 849
#if defined(NTVDM)
#error "Unsupported SoftPC mirror configuration: #if defined(NTVDM)"
#endif
#line 852
		  get_state(adapter) == STATE_OUTA)
	     &&	!(get_status(adapter) & NOTBUSY)
	     &&	!(host_lpt_status(adapter) & HOST_LPT_BUSY))
	{
#line 856
#if defined(NTVDM) && !defined(MONITOR)
#error "Unsupported SoftPC mirror configuration: #if defined(NTVDM) && !defined(MONITOR)"
#else /* NTVDM && !MONITOR */
#line 859
		set_status(adapter, (IU8)(get_status(adapter) | NOTBUSY));
#endif /* NTVDM && !MONITOR */
#line 861

#ifndef	PROD
#error "Unsupported SoftPC mirror configuration: !PROD"
#endif
#line 866

		if (get_control(adapter) & IRQ)
                {
			ica_hw_interrupt(0, CPU_PRINTER_INT, 1);
		}
	}
#endif // !NEC_98
#line 873
}
#ifdef SEGMENTATION
#error "Unsupported SoftPC mirror configuration: SEGMENTATION"
#endif
#line 882

/*
** Initialise the printer port required.
*/
void printer_init IFN1(int,adapter)
{
#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else  // !NEC_98
#line 903
	io_addr i;

	io_define_inb( lpt_adapter[adapter], printer_inb );
	io_define_outb( lpt_adapter[adapter], printer_outb );
	for(i = port_start[adapter]; i < port_end[adapter]; i++)
		io_connect_port(i,lpt_adapter[adapter],IO_READ_WRITE);

#if defined(NTVDM) && defined(MONITOR)
#error "Unsupported SoftPC mirror configuration: excluded conjunction"
#endif /* NTVDM && MONITOR */
#line 922
	{
	    control_reg[adapter] = 0xEC;
	    status_reg[adapter] = 0xDF;
	}
        output_reg[adapter] = 0xAA;

	/*
	 * The call to host_print_doc has been removed since it is
	 * sensible to distinguish between a hard flush (on ctl-alt-del)
	 * or menu reset and a soft flush under user control or at end
	 * of PC application. The calls to host_lpt_close() followed
	 * by host_lpt_open() should already cause a flush to occur,
	 * so no functionality is lost. The first time printer_init is
	 * called host_lpt_close() is not called, but this cannot
	 * matter since host_print_doc() can only be a no-op.
	 */
	/* host_print_doc(adapter); */
	host_print_auto_feed(adapter, FALSE);

#if defined(NTVDM) && defined(MONITOR)
#error "Unsupported SoftPC mirror configuration: excluded conjunction"
#endif /* NTVDM && MONITOR */
#line 946
	    state[adapter] = STATE_READY;

#endif // !NEC_98
#line 949
} /* end of printer_init() */

#if defined(NTVDM) && defined(MONITOR)
#error "Unsupported SoftPC mirror configuration: excluded conjunction"
#endif /* NTVDM && MONITOR */
#line 1008

#endif

#ifndef NEC_98
#ifdef NTVDM
#error "Unsupported SoftPC mirror configuration: NTVDM"
#endif
#endif // !NEC_98
#line 667

#ifdef PS_FLUSHING
#error "Unsupported SoftPC mirror configuration: PS_FLUSHING"
#endif	/* PS_FLUSHING */
#line 1063

#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#endif // NEC_98
#line 1102
