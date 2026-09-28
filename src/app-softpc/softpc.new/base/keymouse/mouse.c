#include "insignia.h"
#include "host_def.h"
/*
 * SoftPC Version 2.0
 *
 * Title	: Microsoft Bus Mouse Adapter
 *
 * Description	: This package contains a group of functions that provide
 *               an interface between the Microsoft Bus Mouse Card and the cpu.
 *
 *		mouse_init()	Initialise the bus mouse adapter
 *		mouse_inb() 	Supports IN's from ports in the bus mouse range
 *		mouse_outb()	Supports OUT's to ports in the bus mouse range
 *		mouse_send()	Queues data sent from the host mouse
 *
 * Author       : Henry Nash
 *
 * Notes        : See the Microsoft Inport Technical Reference Manual for
 *                further information on the hardware interface.
 *                The Bus Mouse ports are jumper selectable on the real card
 *                between (1) 023C - 023F & (2) 0238 - 023B. We only support
 *                the primary range 023C - 023F.
 *						
 *       (r3.5) : The system directory /usr/include/sys is not available
 *                on a Mac running Finder and MPW. Bracket references to
 *                such include files by "#ifdef macintosh <Mac file> #else
 *                <Unix file> #endif".
 */

#ifdef SCCSID
static char SccsID[]="@(#)mouse.c	1.17+ 07/10/95 Copyright Insignia Solutions Ltd.";
#endif

#ifdef SEGMENTATION
/*
 * The following #include specifies the code segment into which this
 * module will by placed by the MPW C compiler on the Mac II running
 * MultiFinder.
 */
#include "SOFTPC_MOUSE.seg"
#endif


/*
 *    O/S include files.
 */
#include <stdio.h>
#include TypesH

/*
 * SoftPC include files
 */
#include "xt.h"
#include CpuH
#include "sas.h"
#include "cga.h"
#include "bios.h"
#include "error.h"
#include "ios.h"
#include "ica.h"
#include "trace.h"
#include "video.h"
#include "mouse.h"
#include "quick_ev.h"
#include "compat/devices/snapshot.h"
#include "mouse_io.h"


/*
 * The following globals are also used by xxx_input.c,
 * where xxx is the host machine name.
 */

int button_left  = 0;	/* Current state of left button */
int button_right = 0;	/* Current state of right button */
int delta_x = 0, delta_y = 0;	/* Current mouse delta moves */

#ifndef PROD
static char buff[132];
#endif

static half_word data1_reg = 0,
		 data2_reg = 0,
		 mouse_status_reg = 0;

static half_word 
		 last_button_left  = 0,
		 last_button_right = 0;

static half_word
                 mouse_mode_reg          = 0, /* mode register 		*/
		 address_reg       = 0; /* address pointer register	*/

/* T85: one original quick event models the InPort timer, not driver-specific
 * interrupt bursts. Device state and scheduling stay with this controller. */
static q_ev_handle mouse_timer = 0;


static int mouse_inb_toggle = 0;

LOCAL unsigned long mouse_period IFN0()
{
    static const unsigned long periods[] = { 0, 33333, 20000, 10000, 5000 };
    unsigned int rate = mouse_mode_reg & 7;

    return (mouse_mode_reg & 0x10) && rate <= 4 ? periods[rate] : 0;
}

LOCAL void mouse_timer_tick IFN1(long, unused)
{
    unsigned long period = mouse_period();

    UNUSED(unused);
    mouse_timer = 0;
    if (period == 0) return;
    if (!(mouse_mode_reg & HOLD))
        ica_hw_interrupt(AT_CPU_MOUSE_ADAPTER, AT_CPU_MOUSE_INT, 1);
    mouse_timer = add_q_event_t(mouse_timer_tick, period, 0);
}

int softpc_device_snapshot_encode_mouse_callback(callback, callback_id)
Q_CALLBACK_FN callback;
unsigned long *callback_id;
{
    if (callback != mouse_timer_tick || callback_id == NULL) return FALSE;
    *callback_id = SOFTPC_DEVICE_QUEUE_MOUSE_TIMER;
    return TRUE;
}

Q_CALLBACK_FN softpc_device_snapshot_decode_mouse_callback(callback_id)
unsigned long callback_id;
{
    return callback_id == SOFTPC_DEVICE_QUEUE_MOUSE_TIMER ? mouse_timer_tick : NULL;
}

int
softpc_device_snapshot_capture_inport_mouse(state)
softpc_device_inport_mouse_state *state;
{
    if (state == NULL) return FALSE;
    state->button_left = button_left;
    state->button_right = button_right;
    state->delta_x = delta_x;
    state->delta_y = delta_y;
    state->data_1 = data1_reg;
    state->data_2 = data2_reg;
    state->status = mouse_status_reg;
    state->last_button_left = last_button_left;
    state->last_button_right = last_button_right;
    state->mode = mouse_mode_reg;
    state->address = address_reg;
    state->timer_handle = (uint32_t)mouse_timer;
    state->id_toggle = mouse_inb_toggle;
    return TRUE;
}

int
softpc_device_snapshot_restore_inport_mouse(state)
const softpc_device_inport_mouse_state *state;
{
    if (state == NULL ||
        (state->button_left != 0 && state->button_left != 1) ||
        (state->button_right != 0 && state->button_right != 1) ||
        (state->last_button_left != 0 && state->last_button_left != 1) ||
        (state->last_button_right != 0 && state->last_button_right != 1) ||
        state->delta_x < -128 || state->delta_x > 127 ||
        state->delta_y < -128 || state->delta_y > 127 ||
        state->timer_handle >= 0xffff ||
        (state->id_toggle != 0 && state->id_toggle != 1))
        return FALSE;
    button_left = state->button_left;
    button_right = state->button_right;
    delta_x = state->delta_x;
    delta_y = state->delta_y;
    data1_reg = state->data_1;
    data2_reg = state->data_2;
    mouse_status_reg = state->status;
    last_button_left = state->last_button_left;
    last_button_right = state->last_button_right;
    mouse_mode_reg = state->mode;
    address_reg = state->address;
    mouse_timer = state->timer_handle;
    mouse_inb_toggle = state->id_toggle;
    return TRUE;
}

void mouse_inb IFN2(io_addr, port, half_word *, value)
{
    *value = 0xff;
    if (port == MOUSE_PORT_1) {		/* data register */

	/*
	 * Internal registers
	 */

	switch (address_reg & 0x07) {
	    case 0x00 :  /* mouse status register */
		*value = mouse_status_reg;
		break;
	    case 0x01 : /* data 1 register horizontal */
		*value = data1_reg;
		break;
	    case 0x02 : /* data 2 register vertical */
		*value = data2_reg;
		break;
	    case 0x07 : /* mode register */
		*value = mouse_mode_reg;
		break;
	    default :
#ifndef PROD
		if (io_verbose & MOUSE_VERBOSE) {
			sprintf(buff, "mouse_inb() :  Bad P");
			trace(buff,DUMP_NONE);
		}
#endif
		break;
	}
    }

	/*
	 * ID register, alternates between value 1 and value 2
	 * value 1 is the chip signature: DE hex
	 * value 2 is the chip revision: 1  and version: 0
	 */

    else
    if (port == MOUSE_PORT_2) {
	if (mouse_inb_toggle = 1 - mouse_inb_toggle)
		*value = 0xDE;
	else
		*value = 0x10;
    }

#ifndef PROD
    if (io_verbose & MOUSE_VERBOSE) {
	sprintf(buff, "mouse_inb() : port %x value %x", port, *value);
	trace(buff,DUMP_NONE);
    }
#endif
}


void mouse_outb IFN2(io_addr, port, half_word, value)
{
#ifndef PROD
	if (io_verbose & MOUSE_VERBOSE) {
		if ((port == MOUSE_PORT_0)
		 || (port == MOUSE_PORT_1)
		 || (port == MOUSE_PORT_2))
			sprintf(buff, "mouse_outb() : port %x value %x", port, value);
		else
			sprintf(buff, "mouse_outb() : bad port: %x value %x", port, value);
		trace(buff,DUMP_NONE);
	}
#endif

	/*
	 * Out to an internal register
	 */

	if (port == MOUSE_PORT_1) {	/* data register */
		/*
		 * Out to Mode register
		 */
		if ( (address_reg & 0x07) == INTERNAL_MODE_REG) {

			/*
			 * Check hold bit (5) for 0 to 1 transition
			 * - Data Interrupt Enable bit is set (Mode 0)
			 * - counter values saved in Data1 & Data2 registers (Mode 0)
			 * - counters cleared
			 * - status register updated
			 */

			if ((value & 0x20) && ((mouse_mode_reg & 0x20) == 0)) {
#ifndef PROD
				if (io_verbose & MOUSE_VERBOSE) {
					sprintf(buff, "mouse_outb() : hold bit 0 -> 1");
					trace(buff,DUMP_NONE);
				}
#endif
				/* clear the interrupt */
				ica_clear_int(AT_CPU_MOUSE_ADAPTER, AT_CPU_MOUSE_INT);

				/* 
				 * read next mouse deltas & buttons 
				 * into the inport registers 
				 */
				data1_reg = delta_x;
				data2_reg = delta_y;
				mouse_status_reg = 0;
				mouse_status_reg = (button_left << 2) + (button_right);
				if (delta_x!=0 || delta_y!=0) {
					mouse_status_reg |= MOVEMENT;
				}
				if (last_button_right != button_right) {
					mouse_status_reg |= RIGHT_BUTTON_CHANGE;
					last_button_right = button_right;
				}

				if (last_button_left != button_left) {
					mouse_status_reg |= LEFT_BUTTON_CHANGE;
					last_button_left = button_left;
				}
				delta_x = delta_y = 0;
			}
			/* 1 -> 0 transition on mode register's hold bit */
			/* ready for next read from queue, so send interrupt */
			else if ((mouse_mode_reg & 0x20) && ((value & 0x20)==0)){
#ifndef PROD
				if (io_verbose & MOUSE_VERBOSE) {
					sprintf(buff, "mouse_outb() : hold bit 1 -> 0");
					trace(buff,DUMP_NONE);
				}
#endif
			}
			/*
			 * Check timer select value (bits 210)
			 */

			switch (value & 0x7) {
			case 0x0 :		/* 0 Hz INTR low */
#ifndef PROD
				if (io_verbose & MOUSE_VERBOSE) {
					sprintf(buff, "mouse_outb() : INTR low"); trace(buff,DUMP_NONE);
				}
#endif
				ica_clear_int(AT_CPU_MOUSE_ADAPTER, AT_CPU_MOUSE_INT);
				break;

			case 0x1: /* 30 Hz */
			case 0x2: /* 50 Hz */
			case 0x3: /* 100 Hz */
			case 0x4: /* 200 Hz */
				break;
			case 0x5: /* reserved */
#ifndef PROD
				if (io_verbose & MOUSE_VERBOSE) {
					sprintf(buff, "mouse: reserved"); trace(buff,DUMP_NONE);
					}
#endif
				break;
			case 0x6 :		/* 0 Hz INTR hi */
#ifndef PROD
				if (io_verbose & MOUSE_VERBOSE) {
					sprintf(buff, "mouse_outb() : INTR hi"); trace(buff,DUMP_NONE);
				}
#endif
				if (value & 0x10)
					ica_hw_interrupt(AT_CPU_MOUSE_ADAPTER,AT_CPU_MOUSE_INT,1);
				break;
			case 0x7: /* externally controlled */
				break;
			default:
#ifndef PROD
				if (io_verbose & MOUSE_VERBOSE) {
					sprintf(buff,"mouse_outb() : bad mode");
					trace(buff,DUMP_NONE);
				}
#endif
				break;

			}
		/* HOLD changes do not restart the free-running timer. */
		if ((mouse_mode_reg ^ value) & 0x17) {
			if (mouse_timer != 0) delete_q_event(mouse_timer);
			mouse_timer = 0;
		}
		mouse_mode_reg = value;
		if (mouse_timer == 0 && mouse_period() != 0)
			mouse_timer = add_q_event_t(mouse_timer_tick, mouse_period(), 0);
		if (!(value & 0x18))
			ica_clear_int(AT_CPU_MOUSE_ADAPTER, AT_CPU_MOUSE_INT);
		else if ((value & 0x08) && !(value & HOLD) &&
			(delta_x || delta_y || button_left != last_button_left ||
			button_right != last_button_right))
			ica_hw_interrupt(AT_CPU_MOUSE_ADAPTER, AT_CPU_MOUSE_INT, 1);

		/*
	 	 * Interface control register
	 	 */
		}
		else
		if ((address_reg & 0x07) == INTERFACE_CONTROL_REG){
#ifndef PROD
				if (io_verbose & MOUSE_VERBOSE) {
					sprintf(buff, "mouse_outb() :  interface control reg port %x value %x",port,value);
					trace(buff,DUMP_NONE);
				}
#endif
		}

	}
	else if(port == MOUSE_PORT_0)	/* address pointer register */
	{
	    if (value & 0x80)  /* is it  a reset */
	    {
		if (mouse_timer != 0) delete_q_event(mouse_timer);
		mouse_timer = 0;
		mouse_mode_reg = 0;
		delta_x = delta_y = 0;
		data1_reg = data2_reg = mouse_status_reg = 0;
		last_button_left = button_left;
		last_button_right = button_right;
		address_reg = value & 0x7F;	/* clear reset bit*/
		ica_clear_int( AT_CPU_MOUSE_ADAPTER, AT_CPU_MOUSE_INT );
	    }
	    else
		address_reg = value;
	}
}

#ifdef SEGMENTATION
/*
 * The following #include specifies the code segment into which this
 * module will by placed by the MPW C compiler on the Mac II running
 * MultiFinder.
 */
#include "SOFTPC_INPUT.seg"
#endif

void mouse_send(Delta_x,Delta_y,left,right)
int	Delta_x,Delta_y,left,right;
{
	if(Delta_x != 0 || Delta_y != 0 || button_left != left || button_right != right)
	{
		delta_x += Delta_x;
		delta_y += Delta_y;

		/***
		Mouse inport registers can only handle one byte
		***/
		if (delta_x < -128)
			delta_x = -128;
		else if (delta_x > 127)
			delta_x = 127;

		if (delta_y < -128)
			delta_y = -128;
		else if (delta_y > 127)
			delta_y = 127;

		button_left = left;
		button_right = right;
		if ((mouse_mode_reg & 0x08) && !(mouse_mode_reg & HOLD))
			ica_hw_interrupt(AT_CPU_MOUSE_ADAPTER,AT_CPU_MOUSE_INT,1);
	}
}

#ifdef SEGMENTATION
/*
 * The following #include specifies the code segment into which this
 * module will by placed by the MPW C compiler on the Mac II running
 * MultiFinder.
 */
#include "SOFTPC_INIT.seg"
#endif


void mouse_init IFN0()
{
    int p;

#ifndef PROD
    if (io_verbose & MOUSE_VERBOSE) {
	sprintf(buff, "mouse_init()");
	trace(buff,DUMP_NONE);
    }
#endif

    mouse_inb_toggle = 0;
    /* Cold initialization follows q_event_init: no old handle remains valid. */
    mouse_timer = 0;
    button_left = button_right = 0;
    mouse_outb(MOUSE_PORT_0, 0x80);

    io_define_inb(MOUSE_ADAPTOR, mouse_inb);
    io_define_outb(MOUSE_ADAPTOR, mouse_outb);

    for(p = MOUSE_PORT_START; p <= MOUSE_PORT_END; p++) {
	io_connect_port(p, MOUSE_ADAPTOR, IO_READ_WRITE);

#ifdef KIPPER
#ifdef CPU_40_STYLE
  /* Enable iret hooks on mouse interrupts */
  ica_iret_hook_control(AT_CPU_MOUSE_ADAPTER, AT_CPU_MOUSE_INT, TRUE);
#endif
#endif

#ifndef PROD
	if (io_verbose & MOUSE_VERBOSE) {
	    sprintf(buff, "Mouse Port connected: %x", p);
	    trace(buff,DUMP_NONE);
	}  
#endif
    }
    host_deinstall_host_mouse();
}
