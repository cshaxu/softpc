#include "insignia.h"
#include "host_def.h"
/*
 * SoftPC Version 3.0
 *
 * Title	: com.c
 *
 * Description	: Asynchronous Adaptor I/O functions.
 *
 * Notes	: Refer to the PC-XT Tech Ref Manual Section 1-185
 *		  For a detailed description of the Asynchronous Adaptor Card.
 *
 */

/* @(#)com.c	1.45 04/26/94 Copyright Insignia Solutions Ltd. */
#ifdef SCCSID
#error "Unsupported SoftPC mirror configuration: SCCSID"
#endif
#line 18

#ifdef SEGMENTATION
#error "Unsupported SoftPC mirror configuration: SEGMENTATION"
#endif
#line 27

/*
 *    O/S include files.
 */
#include <stdio.h>
#include <ctype.h>
#if defined(NTVDM) && defined(MONITOR)
#error "Unsupported SoftPC mirror configuration: excluded conjunction"
#endif
#line 36
#include TypesH
#include StringH

/*
 * SoftPC include files
 */
#include "xt.h"
#include CpuH
#include "sas.h"
#include "bios.h"
#include "ios.h"
#include "rs232.h"
#include "trace.h"
#include "error.h"
#include "config.h"
#include "host_com.h"
#include "ica.h"
#include "debug.h"
#include "timer.h"
#include "quick_ev.h"
#include "idetect.h"
#include "ckmalloc.h"
#include "compat/devices/snapshot.h"
#ifdef GISP_CPU
#error "Unsupported SoftPC mirror configuration: GISP_CPU"
#endif /* GISP_CPU */
#line 61

#ifndef NEC_98
#line 63
LOCAL UTINY selectBits[4] = { 0x1f, 0x3f, 0x7f, 0xff } ;
#endif // NEC_98
#line 65

#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#endif
#line 71


/*
 * =====================================================================
 * The rs232 adaptor state
 * =====================================================================
 */

/*
 * batch_size, current_count
 *	The IRET_HOOKS parameters batch_size and curr_count are used to prevent
 *	the number of interrupts that are emulated in one batch getting too
 *	large. When we reach the batch size we'll unhook the interrupt, and
 *	wait a while.
 * batch_running, qev_running
 *	These variables are used to prevent multiple quick events or batches
 *	running at once on a single adapter.
 */

#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else // NEC_98
#line 127
static struct ADAPTER_STATE
{
	BUFFER_REG tx_buffer;
	BUFFER_REG rx_buffer;
	DIVISOR_LATCH divisor_latch;
	INT_ENABLE_REG int_enable_reg;
	INT_ID_REG int_id_reg;
	LINE_CONTROL_REG line_control_reg;
	MODEM_CONTROL_REG modem_control_reg;
	LINE_STATUS_REG line_status_reg;
	MODEM_STATUS_REG modem_status_reg;
#if defined(NTVDM) && defined(FIFO_ON)
#error "Unsupported SoftPC mirror configuration: NTVDM and FIFO_ON"
#endif
#line 91
	half_word scratch;      /* scratch register */

	int break_state;        /* either OFF or ON */
	int loopback_state;     /* either OFF or ON */
	int dtr_state;          /* either OFF or ON */
	int rts_state;          /* either OFF or ON */
	int out1_state;         /* either OFF or ON */
	int out2_state;         /* either OFF or ON */

	int receiver_line_status_interrupt_state;
	int data_available_interrupt_state;
	int tx_holding_register_empty_interrupt_state;
	int modem_status_interrupt_state;
	int hw_interrupt_priority;
	int com_baud_ind;
	int had_first_read;
#ifdef IRET_HOOKS
#error "Unsupported SoftPC mirror configuration: IRET_HOOKS"
#endif /* IRET_HOOKS */
#line 113
#ifdef NTVDM
#error "Unsupported SoftPC mirror configuration: NTVDM"
#endif
#line 113
} adapter_state[NUM_SERIAL_PORTS];


#ifdef NTVDM
#error "Unsupported SoftPC mirror configuration: NTVDM"
#else
#line 116
#define MODEM_STATE_CHANGE()
#endif
#endif // NEC_98
#line 117


#ifdef IRET_HOOKS
#error "Unsupported SoftPC mirror configuration: IRET_HOOKS"
#endif /* IRET_HOOKS */
#line 191

/*
 * For synchronisation of adapter input.
 * Note this code is essential for the VMS equivalent of the async
 * event manager.  Removing it causes characters to be lost on reception.
 */
static int com_critical[NUM_SERIAL_PORTS];
#define is_com_critical(adapter)	(com_critical[adapter] != 0)
#define com_critical_start(adapter)	(++com_critical[adapter])
#define com_critical_end(adapter)	(--com_critical[adapter])
#define com_critical_reset(adapter)	(com_critical[adapter] = 0)


/*
 * Used to determine whether a flush input is needed for a LCR change
 */
#ifndef NEC_98
#line 208
static LINE_CONTROL_REG LCRFlushMask;
#endif // NEC_98
#line 140

/*
 *	Please note that the following arrays have been made global in order
 *	that they can be accessed from some SUN_VA code. Please do not make
 *	them static.
 */

#if defined(NTVDM) && defined(FIFO_ON)
#error "Unsupported SoftPC mirror configuration: excluded conjunction"
#endif
#line 220

/*
 * The delay needed in microseconds between receiving 2 characters
 * note this time is about 10% less than the time for actual reception.
 *
 * These delays have been heavily fudged and are now based on the idea that
 * most of the comms interrupt handlers can handle 9600 baud.  So as a
 * result the delays between 2 characters are now always set for 9600 baud.
 * Also note the delays of the faster baud rates have been decreased to
 * 1/2 of original delays, again this is to try to empty the host buffers
 * quickly enough to avoid buffer overflows.
 * NB these figures are heuristic.
 *
 * Finally it may be possible that the transmit delays will have to
 * be similarly fudged.
 */
unsigned long RX_delay[] =
{
	34, /* 115200 baud */
	67, /* 57600 baud */
	103, /* 38400 baud */
	900, /* 19200 baud */
	900, /* 9600 baud */
	900, /* 7200 baud */
	900, /* 4800 baud */
	900, /* 3600 baud */
	900, /* 2400 baud */
	900, /* 2000 baud */
	900, /* 1800 baud */
	900, /* 1200 baud */
	900, /* 600 baud */
	900, /* 300 baud */
	900, /* 150 baud */
	900, /* 134 baud */
	900, /* 110 baud */
	900, /* 75 baud */
	900  /* 50 baud */
};

/*
 * the delay needed in microseconds between transmitting 2 characters
 * note this time is about 10% more than the time for actual transmission.
 */
unsigned long TX_delay[] =
{
	83, /* 115200 baud */
	165, /* 57600 baud */
	253, /* 38400 baud */
	495, /* 19200 baud */
	1100, /* 9600 baud */
	1375, /* 7200 baud */
	2063, /* 4800 baud */
	2750, /* 3600 baud */
	4125, /* 2400 baud */
	5042, /* 2000 baud */
	5500, /* 1800 baud */
	8250, /* 1200 baud */
	16500, /* 600 baud */
	33000, /* 300 baud */
	66000, /* 150 baud */
	73920, /* 134 baud */
	89980, /* 110 baud */
	132000, /* 75 baud */
	198000  /* 50 baud */
};

#ifndef PROD
#error "Unsupported SoftPC mirror configuration: non-PROD"
#endif /* !PROD */
#line 221

GLOBAL void recv_char IPT1(long, adapter);
static void do_wait_on_send IPT1(long, adapter);
extern IBOOL tx_pacing_enabled;

/*
 * DIVERGENCE(MVDM-SERIAL-SNAPSHOT-001): export only the selected UART's
 * fixed semantic registers for the standalone snapshot boundary.  Host
 * endpoint buffers belong to compat/serial.c; no host pointer is exported.
 */
int
softpc_device_snapshot_encode_serial_callback(callback, callback_id)
Q_CALLBACK_FN callback;
unsigned long *callback_id;
{
	if (callback_id == NULL) return FALSE;
	if (callback == recv_char) {
		*callback_id = SOFTPC_DEVICE_QUEUE_SERIAL_RECEIVE;
		return TRUE;
	}
	if (callback != do_wait_on_send) return FALSE;
	*callback_id = SOFTPC_DEVICE_QUEUE_SERIAL_SEND;
	return TRUE;
}

Q_CALLBACK_FN
softpc_device_snapshot_decode_serial_callback(callback_id)
unsigned long callback_id;
{
	if (callback_id == SOFTPC_DEVICE_QUEUE_SERIAL_RECEIVE) return recv_char;
	return callback_id == SOFTPC_DEVICE_QUEUE_SERIAL_SEND ? do_wait_on_send : NULL;
}

int
softpc_device_snapshot_capture_serial_controller(state)
softpc_device_serial_controller_state *state;
{
	int adapter;
	struct ADAPTER_STATE *asp;

	if (state == NULL) return FALSE;
	for (adapter = 0; adapter < NUM_SERIAL_PORTS; ++adapter) {
		asp = &adapter_state[adapter];
		state->port[adapter].tx_buffer = asp->tx_buffer;
		state->port[adapter].rx_buffer = asp->rx_buffer;
		state->port[adapter].divisor_latch = asp->divisor_latch.all;
		state->port[adapter].int_enable = asp->int_enable_reg.all;
		state->port[adapter].int_id = asp->int_id_reg.all;
		state->port[adapter].line_control = asp->line_control_reg.all;
		state->port[adapter].modem_control = asp->modem_control_reg.all;
		state->port[adapter].line_status = asp->line_status_reg.all;
		state->port[adapter].modem_status = asp->modem_status_reg.all;
		state->port[adapter].scratch = asp->scratch;
		state->port[adapter].break_state = asp->break_state;
		state->port[adapter].loopback_state = asp->loopback_state;
		state->port[adapter].dtr_state = asp->dtr_state;
		state->port[adapter].rts_state = asp->rts_state;
		state->port[adapter].out1_state = asp->out1_state;
		state->port[adapter].out2_state = asp->out2_state;
		state->port[adapter].receiver_line_status_interrupt =
			asp->receiver_line_status_interrupt_state;
		state->port[adapter].data_available_interrupt =
			asp->data_available_interrupt_state;
		state->port[adapter].tx_empty_interrupt =
			asp->tx_holding_register_empty_interrupt_state;
		state->port[adapter].modem_status_interrupt =
			asp->modem_status_interrupt_state;
		state->port[adapter].interrupt_priority = asp->hw_interrupt_priority;
		state->port[adapter].baud_index = asp->com_baud_ind;
		state->port[adapter].had_first_read = asp->had_first_read;
		state->critical[adapter] = com_critical[adapter];
	}
	state->line_control_flush_mask = LCRFlushMask.all;
	state->transmit_pacing = tx_pacing_enabled;
	return TRUE;
}

int
softpc_device_snapshot_restore_serial_controller(state)
const softpc_device_serial_controller_state *state;
{
	int adapter;
	struct ADAPTER_STATE *asp;

	if (state == NULL) return FALSE;
	for (adapter = 0; adapter < NUM_SERIAL_PORTS; ++adapter) {
		if (state->critical[adapter] < 0 ||
			state->port[adapter].baud_index < 0 ||
			state->port[adapter].baud_index >= (int)(sizeof(TX_delay) / sizeof(TX_delay[0])))
			return FALSE;
	}
	for (adapter = 0; adapter < NUM_SERIAL_PORTS; ++adapter) {
		asp = &adapter_state[adapter];
		asp->tx_buffer = state->port[adapter].tx_buffer;
		asp->rx_buffer = state->port[adapter].rx_buffer;
		asp->divisor_latch.all = state->port[adapter].divisor_latch;
		asp->int_enable_reg.all = state->port[adapter].int_enable;
		asp->int_id_reg.all = state->port[adapter].int_id;
		asp->line_control_reg.all = state->port[adapter].line_control;
		asp->modem_control_reg.all = state->port[adapter].modem_control;
		asp->line_status_reg.all = state->port[adapter].line_status;
		asp->modem_status_reg.all = state->port[adapter].modem_status;
		asp->scratch = state->port[adapter].scratch;
		asp->break_state = state->port[adapter].break_state;
		asp->loopback_state = state->port[adapter].loopback_state;
		asp->dtr_state = state->port[adapter].dtr_state;
		asp->rts_state = state->port[adapter].rts_state;
		asp->out1_state = state->port[adapter].out1_state;
		asp->out2_state = state->port[adapter].out2_state;
		asp->receiver_line_status_interrupt_state =
			state->port[adapter].receiver_line_status_interrupt;
		asp->data_available_interrupt_state =
			state->port[adapter].data_available_interrupt;
		asp->tx_holding_register_empty_interrupt_state =
			state->port[adapter].tx_empty_interrupt;
		asp->modem_status_interrupt_state =
			state->port[adapter].modem_status_interrupt;
		asp->hw_interrupt_priority = state->port[adapter].interrupt_priority;
		asp->com_baud_ind = state->port[adapter].baud_index;
		asp->had_first_read = state->port[adapter].had_first_read;
		com_critical[adapter] = state->critical[adapter];
	}
	LCRFlushMask.all = state->line_control_flush_mask;
	tx_pacing_enabled = state->transmit_pacing;
	return TRUE;
}
/*
 * =====================================================================
 * Other variables
 * =====================================================================
 */

#if !defined(PROD) || defined(SHORT_TRACE)
#error "Unsupported SoftPC mirror configuration: excluded original OR condition"
#endif /* !PROD || SHORT_TRACE */
#line 299

#ifdef PS_FLUSHING
#error "Unsupported SoftPC mirror configuration: PS_FLUSHING"
#endif	/* PS_FLUSHING */
#line 304

/* Control TX pacing */
IBOOL tx_pacing_enabled = FALSE;

GLOBAL void recv_char IPT1(long, adapter);
static void do_wait_on_send IPT1(long, adapter);

/*
 * =====================================================================
 * Static forward declarations
 * =====================================================================
 */
#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else  // NEC_98
#line 348
static void raise_rls_interrupt IPT1(struct ADAPTER_STATE *, asp);
static void raise_rda_interrupt IPT1(struct ADAPTER_STATE *, asp);
static void raise_ms_interrupt IPT1(struct ADAPTER_STATE *,asp);
static void raise_thre_interrupt IPT1(struct ADAPTER_STATE *, asp);
static void generate_iir IPT1(struct ADAPTER_STATE *, asp);
static void raise_interrupt IPT1(struct ADAPTER_STATE *, asp);
static void clear_interrupt IPT1(struct ADAPTER_STATE *, asp);
static void com_flush_input IPT1(int, adapter);
static void com_send_not_finished IPT1(int, adapter);
#ifndef NTVDM
#line 382
static void do_wait_on_send IPT1(long, adapter);
#endif
#line 383
void   com_inb IPT2(io_addr, port, half_word *, value);
void   com_outb IPT2(io_addr, port, half_word, value);
void   com_recv_char IPT1(int, adapter);
GLOBAL void recv_char IPT1(long, adapter);
void   com_modem_change IPT1(int, adapter);
static void modem_change IPT1(int, adapter);
static void set_recv_char_status IPT1(struct ADAPTER_STATE *, asp);
static void set_xmit_char_status IPT1(struct ADAPTER_STATE *, asp);
static void set_break IPT1(int, adapter);
static void set_baud_rate IPT1(int, adapter);
static void set_line_control IPT2(int, adapter, int, value);
static void set_dtr IPT1(int, adapter);
static void set_rts IPT1(int, adapter);
static void set_out1 IPT1(int, adapter);
static void set_out2 IPT1(int, adapter);
static void set_loopback IPT1(int, adapter);
static void super_trace IPT1(char *, string);
void   com1_flush_printer IPT0();
void   com2_flush_printer IPT0();
static void com_reset IPT1(int, adapter);
GLOBAL VOID com_init IPT1(int, adapter);
void   com_post IPT1(int, adapter);
void   com_close IPT1(int, adapter);
LOCAL void next_batch IPT1(long, l_adapter);
#ifdef NTVDM
#error "Unsupported SoftPC mirror configuration: NTVDM"
#endif
#endif // NEC_98
#line 407

/*
 * =====================================================================
 * Subsidiary functions - for interrupt emulation
 * =====================================================================
 */

#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else // NEC_98
#line 455

static void raise_rls_interrupt IFN1(struct ADAPTER_STATE *, asp)
{
	/*
	 * Follow somewhat dubious advice on Page 1-188 of XT Tech Ref
	 * regarding the adapter card sending interrupts to the system.
	 * Apparently confirmed by the logic diagram.
	 */
	if ( asp->modem_control_reg.bits.OUT2 == 0 )
		return;
	
	/*
	 * Check if receiver line status interrupt is enabled
	 */
	if ( asp->int_enable_reg.bits.rx_line == 0 )
		return;
	
	/*
	 * Raise interrupt
	 */
	raise_interrupt(asp);
	asp->receiver_line_status_interrupt_state = ON;
}

static void raise_rda_interrupt IFN1(struct ADAPTER_STATE *, asp)
{
	if (( asp->modem_control_reg.bits.OUT2 == 0 ) &&
		( asp->loopback_state == OFF ))
		return;
	
	/*
	 * Check if data available interrupt is enabled
	 */
	if ( asp->int_enable_reg.bits.data_available == 0 )
		return;
	
	/*
	 * Raise interrupt
	 */
	raise_interrupt(asp);
	asp->data_available_interrupt_state = ON;
}

static void raise_ms_interrupt IFN1(struct ADAPTER_STATE *, asp)
{
	if ( asp->modem_control_reg.bits.OUT2 == 0 )
		return;
	
	/*
	 * Check if modem status interrupt is enabled
	 */
	if ( asp->int_enable_reg.bits.modem_status == 0 )
		return;
	
	/*
	 * Raise interrupt
	 */
	raise_interrupt(asp);
	asp->modem_status_interrupt_state = ON;
}

static void raise_thre_interrupt IFN1(struct ADAPTER_STATE *, asp)
{
	if ( asp->modem_control_reg.bits.OUT2 == 0 )
		return;
	
	/*
	 * Check if tx holding register empty interrupt is enabled
	 */
	if ( asp->int_enable_reg.bits.tx_holding == 0 )
		return;
	
	/*
	 * Raise interrupt
	 */
	raise_interrupt(asp);
	asp->tx_holding_register_empty_interrupt_state = ON;
}

static void generate_iir IFN1(struct ADAPTER_STATE *, asp)
{
	/*
	 * Set up interrupt identification register with highest priority
	 * pending interrupt.
	 */
	
	if ( asp->receiver_line_status_interrupt_state == ON )
	{
		asp->int_id_reg.bits.interrupt_ID = RLS_INT;
		asp->int_id_reg.bits.no_int_pending = 0;
	}
	else if ( asp->data_available_interrupt_state == ON )
	{
		asp->int_id_reg.bits.interrupt_ID = RDA_INT;
		asp->int_id_reg.bits.no_int_pending = 0;
	}
#if defined(NTVDM) && defined(FIFO_ON)
#error "Unsupported SoftPC mirror configuration: excluded conjunction"
#endif
#line 510
	else if ( asp->tx_holding_register_empty_interrupt_state == ON )
	{
		asp->int_id_reg.bits.interrupt_ID = THRE_INT;
		asp->int_id_reg.bits.no_int_pending = 0;
	}
	else if ( asp->modem_status_interrupt_state == ON )
	{
		asp->int_id_reg.bits.interrupt_ID = MS_INT;
		asp->int_id_reg.bits.no_int_pending = 0;
	}
	else
	{
		/* clear interrupt */
		asp->int_id_reg.bits.no_int_pending = 1;
		asp->int_id_reg.bits.interrupt_ID = 0;
	}
}
#endif // NEC_98
#line 527

#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else // NEC_98
#line 612
static void raise_interrupt IFN1(struct ADAPTER_STATE *, asp)
{
	/*
	 * Make sure that some thing else has not raised an interrupt
	 * already.
	 */
	if ( ( asp->receiver_line_status_interrupt_state      == OFF )
	&&   ( asp->data_available_interrupt_state            == OFF )
	&&   ( asp->tx_holding_register_empty_interrupt_state == OFF )
	&&   ( asp->modem_status_interrupt_state              == OFF )
#if defined(NTVDM) && defined(FIFO_ON)
#error "Unsupported SoftPC mirror configuration: NTVDM && FIFO_ON"
#endif
#line 538
       )
	{
#ifndef DELAYED_INTS
		ica_hw_interrupt(0, asp->hw_interrupt_priority, 1);
#else
#error "Unsupported SoftPC mirror configuration: DELAYED_INTS"
#endif
#line 546
	}
}

static void clear_interrupt IFN1(struct ADAPTER_STATE *, asp)
{
	/*
	 * Make sure that some thing else has not raised an interrupt
	 * already.  If so then we cant drop the line.
	 */
	if ( ( asp->receiver_line_status_interrupt_state      == OFF )
	&&   ( asp->data_available_interrupt_state            == OFF )
	&&   ( asp->tx_holding_register_empty_interrupt_state == OFF )
	&&   ( asp->modem_status_interrupt_state              == OFF )
#if defined(NTVDM) && defined(FIFO_ON)
#error "Unsupported SoftPC mirror configuration: NTVDM && FIFO_ON"
#endif
#line 559
       )
	{
		ica_clear_int(0, asp->hw_interrupt_priority);
	}
}
#endif // NEC_98
#line 564

#if defined(NTVDM) && defined(FIFO_ON)
#error "Unsupported SoftPC mirror configuration: NTVDM && FIFO_ON"
#endif
#line 676




/*
 * =====================================================================
 * The Adaptor functions
 * =====================================================================
 */

static void com_flush_input IFN1(int, adapter)
{
	struct ADAPTER_STATE *asp = &adapter_state[adapter];
	int finished, error_mask;
	long input_ready = 0;

	sure_note_trace1(RS232_VERBOSE, "flushing the input for COM%c",
		adapter+'1');
	finished=FALSE;
	while(!finished)
	{
		host_com_ioctl(adapter, HOST_COM_INPUT_READY,
			(intptr_t)&input_ready);
		if (input_ready)
		{
			host_com_read(adapter, (UTINY *)&asp->rx_buffer,
				&error_mask);
		}
		else
		{
			finished=TRUE;
		}
	}
	set_xmit_char_status(asp);
}

#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else // NEC_98
#line 721
static void com_send_not_finished IFN1(int, adapter)
{
	struct ADAPTER_STATE *asp = &adapter_state[adapter];

	asp->line_status_reg.bits.tx_holding_empty=0;
	asp->line_status_reg.bits.tx_shift_empty=0;
}
#endif // NEC_98
#line 607


#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else // NEC_98
#line 743
static void do_wait_on_send IFN1(long, adapter)
{
	extern	void	host_com_send_delay_done IPT2(long, p1, int, p2);
	struct ADAPTER_STATE *asp;

	asp= &adapter_state[adapter];
	set_xmit_char_status(asp);
	host_com_send_delay_done(adapter, TX_delay[asp->com_baud_ind]);
}


#endif // NEC_98
#line 755

#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else // NEC_98
#line 1095
void com_inb IFN2(io_addr, port, half_word *, value)
{
	int adapter = adapter_for_port(port);
	struct ADAPTER_STATE *asp = &adapter_state[adapter];
	long input_ready = 0;
	boolean adapter_was_critical;

#ifdef NTVDM
#error "Unsupported SoftPC mirror configuration: NTVDM"
#endif /* NTVDM */
#line 627

	switch(port & 0x7)
	{
	case RS232_TX_RX:
		IDLE_comlpt();
		if (asp->line_control_reg.bits.DLAB == 0)
		{
			/*
			 * Read of rx buffer
			 */
#ifndef NTVDM
#line 637
			if (!(asp->had_first_read))
			{
				com_flush_input(adapter);
				asp->had_first_read=TRUE;
			}
#else /* NTVDM is defined */
#error "Unsupported SoftPC mirror configuration: NTVDM"
#endif /* !NTVDM */
#line 642
			*value = asp->rx_buffer;
		
			adapter_was_critical =
				(asp->line_status_reg.bits.data_ready == 1);
		
			asp->line_status_reg.bits.data_ready = 0;
			asp->data_available_interrupt_state = OFF;
			clear_interrupt(asp);

			if ( asp->loopback_state == OFF )
			{
				/*
				 * Adapter out of critical region,
				 * check for further input.  For IRET_HOOKS
				 * we don't need to do this, as receipt
				 * of the next character is kicked off
				 * by the IRET, however we do something
				 * else instead.  If this is the first
				 * character of a batch, we kick off a quick
				 * event for what will eventually be the
				 * start of the next batch (assuming there
				 * isn't already a quick event running).
				 * In any case we increment the count of
				 * characters in this batch.
				 */
				if (adapter_was_critical)
				{

#ifndef NTVDM
#line 670
#ifdef IRET_HOOKS
#error "Unsupported SoftPC mirror configuration: IRET_HOOKS"
#else /* IRET_HOOKS */
#line 687
					host_com_ioctl(adapter, HOST_COM_INPUT_READY,
						(intptr_t)&input_ready);
					if (input_ready)
#ifdef DELAYED_INTS
#error "Unsupported SoftPC mirror configuration: DELAYED_INTS"
#else
#line 693
						add_q_event_t(recv_char,
							RX_delay[asp->com_baud_ind],
							adapter);
#endif /* DELAYED_INTS */
					else
						com_critical_reset(adapter);
#endif /* IRET_HOOKS */
#endif /* !NTVDM */
#line 700
				}


			}
			else
			{
				set_xmit_char_status(asp);
			}
#ifdef IRET_HOOKS
#error "Unsupported SoftPC mirror configuration: IRET_HOOKS"
#endif /* IRET_HOOKS */
#line 716
		}
		else
			*value = (IU8)(asp->divisor_latch.byte.LSByte);
#ifdef SHORT_TRACE
#error "Unsupported SoftPC mirror configuration: SHORT_TRACE"
#endif
#line 728
#ifndef PROD
#error "Unsupported SoftPC mirror configuration: !PROD"
#endif
#line 740
		break;
																		
	case RS232_IER:
		if (asp->line_control_reg.bits.DLAB == 0)
			*value = asp->int_enable_reg.all;
		else
			*value = (IU8)(asp->divisor_latch.byte.MSByte);
#ifdef SHORT_TRACE
#error "Unsupported SoftPC mirror configuration: SHORT_TRACE"
#endif
#line 755
#ifndef PROD
#error "Unsupported SoftPC mirror configuration: !PROD"
#endif
#line 766
		break;

	case RS232_IIR:
		generate_iir(asp);
		*value = asp->int_id_reg.all;

		if ( asp->int_id_reg.bits.interrupt_ID == THRE_INT )
		{
			asp->tx_holding_register_empty_interrupt_state = OFF;
			clear_interrupt(asp);
		}

#ifdef SHORT_TRACE
#error "Unsupported SoftPC mirror configuration: SHORT_TRACE"
#endif
#line 786
#ifndef PROD
#error "Unsupported SoftPC mirror configuration: !PROD"
#endif
#line 797
		break;
	
	case RS232_LCR:


		*value = asp->line_control_reg.all;
#ifdef SHORT_TRACE
#error "Unsupported SoftPC mirror configuration: SHORT_TRACE"
#endif
#line 811
#ifndef PROD
#error "Unsupported SoftPC mirror configuration: !PROD"
#endif
#line 822
		break;
	
	case RS232_MCR:
		*value = asp->modem_control_reg.all;
#ifdef SHORT_TRACE
#error "Unsupported SoftPC mirror configuration: SHORT_TRACE"
#endif
#line 834
#ifndef PROD
#error "Unsupported SoftPC mirror configuration: !PROD"
#endif
#line 845
		break;
	
	case RS232_LSR:
		*value = asp->line_status_reg.all;
	
		asp->line_status_reg.bits.overrun_error = 0;
		asp->line_status_reg.bits.parity_error = 0;
		asp->line_status_reg.bits.framing_error = 0;
		asp->line_status_reg.bits.break_interrupt = 0;
		asp->receiver_line_status_interrupt_state = OFF;
		clear_interrupt(asp);
#if defined(NTVDM) && defined(FIFO_ON)
#error "Unsupported SoftPC mirror configuration: excluded conjunction"
#endif
#line 856
	
#ifdef SHORT_TRACE
#error "Unsupported SoftPC mirror configuration: SHORT_TRACE"
#endif
#line 870
#ifndef PROD
#error "Unsupported SoftPC mirror configuration: !PROD"
#endif
#line 897
		break;
	
	case RS232_MSR:

#ifndef NTVDM
#line 901
		if (asp->loopback_state == OFF)
		{
                com_modem_change(adapter);
		}
		else
		{
			asp->modem_status_reg.bits.CTS = asp->modem_control_reg.bits.RTS;
			asp->modem_status_reg.bits.DSR = asp->modem_control_reg.bits.DTR;
			asp->modem_status_reg.bits.RI = asp->modem_control_reg.bits.OUT1;
			asp->modem_status_reg.bits.RLSD = asp->modem_control_reg.bits.OUT2;
		}
		*value = asp->modem_status_reg.all;
		asp->modem_status_reg.bits.delta_CTS = 0;
		asp->modem_status_reg.bits.delta_DSR = 0;
		asp->modem_status_reg.bits.delta_RLSD = 0;
		asp->modem_status_reg.bits.TERI = 0;
		asp->modem_status_interrupt_state = OFF;

		host_com_msr_callback (adapter, asp->modem_status_reg.all);
		clear_interrupt(asp);
#else
#error "Unsupported SoftPC mirror configuration: NTVDM"
#endif /* ifndef NTVDM */
#line 921


#ifdef SHORT_TRACE
#error "Unsupported SoftPC mirror configuration: SHORT_TRACE"
#endif
#line 931
#ifndef PROD
#error "Unsupported SoftPC mirror configuration: !PROD"
#endif
#line 942
		break;

/*
 * Scratch register.  Just output the value stored.
 */
	case RS232_SCRATCH:
		*value = asp->scratch;
		break;
	
	}

#ifndef PROD
#error "Unsupported SoftPC mirror configuration: !PROD"
#endif
#line 966

}


void com_outb IFN2(io_addr, port, half_word, value)
{
	int adapter = adapter_for_port(port);
	struct ADAPTER_STATE *asp = &adapter_state[adapter];
	int	i;



#ifndef PROD
#error "Unsupported SoftPC mirror configuration: !PROD"
#endif
#line 986
	
	switch(port & 0x7)
	{
	case RS232_TX_RX:
		IDLE_comlpt();
		if (asp->line_control_reg.bits.DLAB == 0)
		{
			/*
			 * Write char from tx buffer
			 */
			asp->tx_holding_register_empty_interrupt_state = OFF;
			clear_interrupt(asp);
			asp->tx_buffer = value;
			asp->line_status_reg.bits.tx_holding_empty = 0;
			asp->line_status_reg.bits.tx_shift_empty = 0;
			if ( asp->loopback_state == OFF )
			{
#ifdef PS_FLUSHING
#error "Unsupported SoftPC mirror configuration: PS_FLUSHING"
#endif	/* PS_FLUSHING */
#line 1014
				host_com_write(adapter, asp->tx_buffer);
#if defined (DELAYED_INTS) || defined (NTVDM)
				set_xmit_char_status(asp);
#else
					if(tx_pacing_enabled)
						add_q_event_t(do_wait_on_send,
					            TX_delay[asp->com_baud_ind], adapter);
					else
						do_wait_on_send(adapter);

#endif /* DELAYED_INTS || NTVDM */
#ifdef PS_FLUSHING
#error "Unsupported SoftPC mirror configuration: PS_FLUSHING"
#endif	/* PS_FLUSHING */
#line 1028
			}
			else
			{	/* Loopback case requires masking off */
				/* of bits based upon word length.    */
				asp->rx_buffer = asp->tx_buffer & selectBits[asp->line_control_reg.bits.word_length] ;
				set_xmit_char_status(asp);
				set_recv_char_status(asp);
			}
		}
		else
		{
			asp->divisor_latch.byte.LSByte = value;
			set_baud_rate(adapter);
		}
#ifdef SHORT_TRACE
#error "Unsupported SoftPC mirror configuration: SHORT_TRACE"
#endif
#line 1051
#ifndef PROD
#error "Unsupported SoftPC mirror configuration: !PROD"
#endif
#line 1063
		break;
																		
	case RS232_IER:
		if (asp->line_control_reg.bits.DLAB == 0)
		{
#ifdef NTVDM
#error "Unsupported SoftPC mirror configuration: NTVDM"
#endif
#line 1068
			asp->int_enable_reg.all = value & 0xf;
			/*
			 * Kill off any pending interrupts for those items
			 * which are set now as disabled
			 */
			if ( asp->int_enable_reg.bits.data_available == 0 )
				asp->data_available_interrupt_state = OFF;
			if ( asp->int_enable_reg.bits.tx_holding == 0 )
				asp->tx_holding_register_empty_interrupt_state =
					OFF;
			if ( asp->int_enable_reg.bits.rx_line == 0 )
				asp->receiver_line_status_interrupt_state = OFF;
			if ( asp->int_enable_reg.bits.modem_status == 0 )
				asp->modem_status_interrupt_state = OFF;
			
			/*
			 * Check for immediately actionable interrupts
			 * If you change these, change the code for out2 as well.
			 */
			if ( asp->line_status_reg.bits.data_ready == 1 )
				raise_rda_interrupt(asp);
			if ( asp->line_status_reg.bits.tx_holding_empty == 1 )
				raise_thre_interrupt(asp);

			/* lower int line if no outstanding interrupts */
			clear_interrupt(asp);

#ifdef NTVDM
#error "Unsupported SoftPC mirror configuration: NTVDM"
#endif /* NTVDM */
#line 1095
		}
		else
		{
			asp->divisor_latch.byte.MSByte = value;
			set_baud_rate(adapter);
		}
#ifdef SHORT_TRACE
#error "Unsupported SoftPC mirror configuration: SHORT_TRACE"
#endif
#line 1109
#ifndef PROD
#error "Unsupported SoftPC mirror configuration: !PROD"
#endif
#line 1120
		break;
		
	case RS232_IIR:
		/*
		 * Essentially a READ ONLY register
		 */
#ifdef SHORT_TRACE
#error "Unsupported SoftPC mirror configuration: SHORT_TRACE"
#endif
#line 1134
#ifndef PROD
#error "Unsupported SoftPC mirror configuration: !PROD"
#endif
#line 1145
		break;

	case RS232_LCR:
#ifdef NTVDM
#error "Unsupported SoftPC mirror configuration: NTVDM"
#endif /* NTVDM */
#line 1148

		if ((value & LCRFlushMask.all)
		!= (asp->line_control_reg.all & LCRFlushMask.all))
			com_flush_input(adapter);

		set_line_control(adapter, value);
		set_break(adapter);
#ifdef SHORT_TRACE
#error "Unsupported SoftPC mirror configuration: SHORT_TRACE"
#endif
#line 1163
#ifndef PROD
#error "Unsupported SoftPC mirror configuration: !PROD"
#endif
#line 1174
		break;
		
	case RS232_MCR:
#ifdef SHORT_TRACE
#error "Unsupported SoftPC mirror configuration: SHORT_TRACE"
#endif
#line 1185
		/*
		 * Optimisation - DOS keeps re-writing this register
		 */
		if ( asp->modem_control_reg.all == value )
			break;
		
		asp->modem_control_reg.all = value;
		asp->modem_control_reg.bits.pad = 0;

		/* Must be called before set_dtr */
		set_loopback(adapter);
		set_dtr(adapter);
		set_rts(adapter);
		set_out1(adapter);
		set_out2(adapter);
#ifndef PROD
#error "Unsupported SoftPC mirror configuration: !PROD"
#endif
#line 1211
		break;
		
	case RS232_LSR:
		i = asp->line_status_reg.bits.tx_shift_empty;   /* READ ONLY */
		asp->line_status_reg.all = value;
		asp->line_status_reg.bits.tx_shift_empty = (unsigned char)i;
#ifdef SHORT_TRACE
#error "Unsupported SoftPC mirror configuration: SHORT_TRACE"
#endif
#line 1225
#ifndef PROD
#error "Unsupported SoftPC mirror configuration: !PROD"
#endif
#line 1236
		break;
		
	case RS232_MSR:
		/*
		 * Essentially a READ ONLY register.
		 */
#ifdef SHORT_TRACE
#error "Unsupported SoftPC mirror configuration: SHORT_TRACE"
#endif
#line 1250
#ifndef PROD
#error "Unsupported SoftPC mirror configuration: !PROD"
#endif
#line 1261
		/* DrDOS writes to this reg after setting int on MSR change
		 * and expects to get an interrupt back!!! So we will oblige.
		 * Writing to this reg only seems to affect the delta bits
		 * (bits 0-3) of the reg.
		 */
		if ((value & 0xf) != (asp->modem_status_reg.all & 0xf))
		{
			asp->modem_status_reg.all &= 0xf0;
			asp->modem_status_reg.all |= value & 0xf;
			host_com_msr_callback (adapter, asp->modem_status_reg.all);

			if (asp->loopback_state == OFF)
				raise_ms_interrupt(asp);

			MODEM_STATE_CHANGE();
		}
		break;

/*
 * Scratch register.  Just store the value.
 */
	case RS232_SCRATCH:
		asp->scratch = value;
		break;
	
	}


}
#endif // NEC_98
#line 1290


#ifdef IRET_HOOKS
#error "Unsupported SoftPC mirror configuration: IRET_HOOKS"
#endif /* of ifdef IRET_HOOKS */
#line 2052

/*
 * =====================================================================
 * Subsidiary functions - for transmitting characters
 * =====================================================================
 */


#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else // NEC_98
#line 1417
#ifdef  NTVDM
#error "Unsupported SoftPC mirror configuration: NTVDM"
#else /* NTVDM */
#line 1416

void com_recv_char IFN1(int, adapter)
{
	/*
	 * Character available on input device; process character if adapter
	 * is ready to receive it
	 */

	/* Check adapter not already in critical region */
	if (!is_com_critical(adapter))
	{
		com_critical_start(adapter);
		recv_char((long)adapter);
	}
}
#endif /* NTVDM */
#line 1431

/*
 * BCN 2151 - recv_char must use long param to match add_event function prototype
 */
GLOBAL void
recv_char IFN1(long, adapt_long)
{
	int adapter = adapt_long;

	/*
	 * Character available on input device, read char, format char
	 * checking for parity and overrun errors, raise the appropriate
	 * interrupt.
	 */
	struct ADAPTER_STATE *asp = &adapter_state[adapter];
	int error_mask = 0;
	
	host_com_read(adapter, (UTINY *)&asp->rx_buffer, &error_mask);
	
	if (error_mask)
	{
		/*
		 * Set line status register and raise line status interrupt
		 */
		if (error_mask & HOST_COM_OVERRUN_ERROR)
			asp->line_status_reg.bits.overrun_error = 1;
		
		if (error_mask & HOST_COM_FRAMING_ERROR)
			asp->line_status_reg.bits.framing_error = 1;
		
		if (error_mask & HOST_COM_PARITY_ERROR)
			asp->line_status_reg.bits.parity_error = 1;
		
		if (error_mask & HOST_COM_BREAK_RECEIVED)
			asp->line_status_reg.bits.break_interrupt = 1;
		
		raise_rls_interrupt(asp);
	}
	
	set_recv_char_status(asp);
	
#ifdef DOCUMENTATION
#error "Unsupported SoftPC mirror configuration: DOCUMENTATION"
#endif
#line 1494
}
#endif // NEC_98
#line 1495

#ifdef NTVDM
#error "Unsupported SoftPC mirror configuration: NTVDM"
#endif /* NTVDM */
#line 2311


/*
 * One of the modem control input lines has changed state
 */
void com_modem_change IFN1(int, adapter)
{
	modem_change(adapter);
}

#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else // NEC_98
#line 2340
static void modem_change IFN1(int, adapter)
{
	/*
	 * Update the modem status register after a change to one of the
	 * modem control input lines
	 */
	struct ADAPTER_STATE *asp = &adapter_state[adapter];
	long modem_status = 0;
	int cts_state, dsr_state, rlsd_state, ri_state;
	
	if (asp->loopback_state == OFF)
	{
		/* get current modem input state */
		host_com_ioctl(adapter, HOST_COM_MODEM, (intptr_t)&modem_status);
		cts_state  = (modem_status & HOST_COM_MODEM_CTS)  ? ON : OFF;
		dsr_state  = (modem_status & HOST_COM_MODEM_DSR)  ? ON : OFF;
		rlsd_state = (modem_status & HOST_COM_MODEM_RLSD) ? ON : OFF;
		ri_state   = (modem_status & HOST_COM_MODEM_RI)   ? ON : OFF;
		
		/*
		 * Establish CTS state
		 */
		switch(change_state(cts_state, asp->modem_status_reg.bits.CTS))
		{
		case ON:
			asp->modem_status_reg.bits.CTS = ON;
			asp->modem_status_reg.bits.delta_CTS = ON;
			host_com_msr_callback (adapter, asp->modem_status_reg.all);

			raise_ms_interrupt(asp);
			MODEM_STATE_CHANGE();
			break;
		
		case OFF:
			asp->modem_status_reg.bits.CTS = OFF;
			asp->modem_status_reg.bits.delta_CTS = ON;
			host_com_msr_callback (adapter, asp->modem_status_reg.all);

			raise_ms_interrupt(asp);
			MODEM_STATE_CHANGE();
			break;
		
		case LEAVE_ALONE:
			break;
		}
		
		/*
		 * Establish DSR state
		 */
		switch(change_state(dsr_state, asp->modem_status_reg.bits.DSR))
		{
		case ON:
			asp->modem_status_reg.bits.DSR = ON;
			asp->modem_status_reg.bits.delta_DSR = ON;
			host_com_msr_callback (adapter, asp->modem_status_reg.all);

			raise_ms_interrupt(asp);
			MODEM_STATE_CHANGE();
			break;
		
		case OFF:
			asp->modem_status_reg.bits.DSR = OFF;
			asp->modem_status_reg.bits.delta_DSR = ON;
			host_com_msr_callback (adapter, asp->modem_status_reg.all);

			raise_ms_interrupt(asp);
			MODEM_STATE_CHANGE();
			break;
		
		case LEAVE_ALONE:
			break;
		}
		
		/*
		 * Establish RLSD state
		 */
		switch(change_state(rlsd_state,
			asp->modem_status_reg.bits.RLSD))
		{
		case ON:
			asp->modem_status_reg.bits.RLSD = ON;
			asp->modem_status_reg.bits.delta_RLSD = ON;
			host_com_msr_callback (adapter, asp->modem_status_reg.all);

			raise_ms_interrupt(asp);
			MODEM_STATE_CHANGE();
			break;
		
		case OFF:
			asp->modem_status_reg.bits.RLSD = OFF;
			asp->modem_status_reg.bits.delta_RLSD = ON;
			host_com_msr_callback (adapter, asp->modem_status_reg.all);

			raise_ms_interrupt(asp);
			MODEM_STATE_CHANGE();
			break;
		
		case LEAVE_ALONE:
			break;
		}
		
		/*
		 * Establish RI state
		 */
		switch(change_state(ri_state, asp->modem_status_reg.bits.RI))
		{
		case ON:
			asp->modem_status_reg.bits.RI = ON;
			host_com_msr_callback (adapter, asp->modem_status_reg.all);
			MODEM_STATE_CHANGE();
			break;
		
		case OFF:
			asp->modem_status_reg.bits.RI = OFF;
			asp->modem_status_reg.bits.TERI = ON;
			host_com_msr_callback (adapter, asp->modem_status_reg.all);

			raise_ms_interrupt(asp);
			MODEM_STATE_CHANGE();
			break;
		
		case LEAVE_ALONE:
			break;
		}
	}
}
#endif // NEC_98
#line 1631

#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else // NEC_98
#line 2485
static void set_recv_char_status IFN1(struct ADAPTER_STATE *, asp)
{
	/*
	 * Check for data overrun and set up correct interrupt
	 */
	if ( asp->line_status_reg.bits.data_ready == 1 )
	{
		sure_note_trace0(RS232_VERBOSE, "overrun error in set_recv_char_status");
		asp->line_status_reg.bits.overrun_error = 1;
		raise_rls_interrupt(asp);
	}
	else
	{
		asp->line_status_reg.bits.data_ready = 1;
		raise_rda_interrupt(asp);
	}
}
#endif // NEC_98
#line 2503

static void set_xmit_char_status IFN1(struct ADAPTER_STATE *, asp)
{
	/*
	 * Set line status register and raise interrupt
	 */
#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else // NEC_98
#line 2514
	asp->line_status_reg.bits.tx_holding_empty = 1;
	asp->line_status_reg.bits.tx_shift_empty = 1;
	raise_thre_interrupt(asp);
#endif // NEC_98
#line 1658
}

#ifdef NTVDM
#error "Unsupported SoftPC mirror configuration: NTVDM"
#endif
#line 2542

/*
 * =====================================================================
 * Subsidiary functions - for setting comms parameters
 * =====================================================================
 */

static void set_break IFN1(int, adapter)
{
	/*
	 * Process the set break control bit. Bit 6 of the Line Control
	 * Register.
	 */
	struct ADAPTER_STATE *asp = &adapter_state[adapter];
	
#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else // NEC_98
#line 2561
	switch ( change_state((int)asp->line_control_reg.bits.set_break,
		asp->break_state) )
#endif // NEC_98
#line 1677
	{
	case ON:
		asp->break_state = ON;
		host_com_ioctl(adapter, HOST_COM_SBRK, 0);
		break;
	
	case OFF:
		asp->break_state = OFF;
		host_com_ioctl(adapter, HOST_COM_CBRK, 0);
		break;
	
	case LEAVE_ALONE:
		break;
	}
}

/*
 * The following table is derived from page 1-200 of the XT Tech Ref 1st Ed
 * (except rates above 9600 which are not OFFICIALLY supported on the XT and
 * AT, but are theoretically possible) */

#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else // NEC_98
#line 2633
static word valid_latches[] =
{
	1, 	2, 	3, 	6, 	12, 	16, 	24, 	32,
	48, 	58, 	64, 	96, 	192,	384, 	768, 	857,
	1047, 	1536, 	2304
};
#endif // NEC_98
#line 1704

#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else // NEC_98
#line 2665
#if !defined(PROD) || defined(IRET_HOOKS)
static IUM32 bauds[] =
{
	115200, /* 115200 baud */
	57600, /* 57600 baud */
	38400, /* 38400 baud */
	19200, /* 19200 baud */
	9600, /* 9600 baud */
	7200, /* 7200 baud */
	4800, /* 4800 baud */
	3600, /* 3600 baud */
	2400, /* 2400 baud */
	2000, /* 2000 baud */
	1800, /* 1800 baud */
	1200, /* 1200 baud */
	600, /* 600 baud */
	300, /* 300 baud */
	150, /* 150 baud */
	134, /* 134 baud */
	110, /* 110 baud */
	75, /* 75 baud */
	50  /* 50 baud */
};
#endif /* !PROD or IRET_HOOKS*/
#endif // NEC_98
#line 2690

static word speeds[] =
{
	HOST_COM_B115200,
	HOST_COM_B57600,
	HOST_COM_B38400,
	HOST_COM_B19200,
	HOST_COM_B9600,
	HOST_COM_B7200,
	HOST_COM_B4800,
	HOST_COM_B3600,
	HOST_COM_B2400,
	HOST_COM_B2000,
	HOST_COM_B1800,
	HOST_COM_B1200,
	HOST_COM_B600,
	HOST_COM_B300,
	HOST_COM_B150,
	HOST_COM_B134,
	HOST_COM_B110,
	HOST_COM_B75,
	HOST_COM_B50
};

#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else // NEC_98
#line 2718
static int no_valid_latches =
	(int)(sizeof(valid_latches)/sizeof(valid_latches[0]));
#endif // NEC_98
#line 2721

#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#endif // NEC_98
#line 2761



#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else // NEC_98
#line 2773
static void set_baud_rate IFN1(int, adapter)
{
	/*
	 * Map divisor latch into a valid line speed and set our Unix
	 * device accordingly. Note as the sixteen bit divisor latch is
	 * likely to be written in two eight bit bytes, we ignore illegal
	 * values of the sixteen bit divisor latch - hoping a second
	 * byte will be written to produce a legal value. In addition
	 * the reset value (0) is illegal!
	 *
	 * For IRET hooks, we need to determine the batch size from
	 * the line speed, and an idea of how many quick events
	 * we can get per second. We add one to alow us to catch-up!
	 * Hence
	 * batch size = line_speed (in bits per second)
	 *	/ number of bits in a byte
	 *	* number of quick events ticks per second (normally 1000000)
	 *	/ the length in quick event ticks of a batch
	 *	+ 1
	 */
	struct ADAPTER_STATE *asp = &adapter_state[adapter];
	int i;

	com_flush_input(adapter);


#ifndef NTVDM
#line 1784
	/*
	 * Check for valid divisor latch
	 */
	for (i = 0; i < no_valid_latches && asp->divisor_latch.all !=
		valid_latches[i]; i++)
			;
	
	if (i < no_valid_latches)	/* ie map found */
	{
		host_com_ioctl(adapter, HOST_COM_BAUD, speeds[i]);
		asp->com_baud_ind = i;
		sure_note_trace3(RS232_VERBOSE,
			" delay for baud %d RX:%d TX:%d", bauds[i],
			RX_delay[i], TX_delay[i]);
#ifdef IRET_HOOKS
#error "Unsupported SoftPC mirror configuration: IRET_HOOKS"
#endif /* IRET_HOOKS */
#line 1809
	}
#else /* NTVDM */
#error "Unsupported SoftPC mirror configuration: NTVDM"
#endif /* NTVDM */
#line 1810
}
#endif // NEC_98
#line 1811


#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#endif // NEC_98
#line 2965

#ifndef NEC_98
#line 2967
static void set_line_control IFN2(int, adapter, int, value)
{
	/*
	 * Set Number of data bits
	 *     Parity bits
	 *     Number of stop bits
	 */
	struct ADAPTER_STATE *asp = &adapter_state[adapter];
	LINE_CONTROL_REG newLCR;
	int newParity, parity;
	
	newLCR.all = (unsigned char)value;

	/*
	 * Set up the number of data bits
	 */
	if (asp->line_control_reg.bits.word_length != newLCR.bits.word_length)
		host_com_ioctl(adapter, HOST_COM_DATABITS,
			newLCR.bits.word_length + 5);
	
	/*
	 * Set up the number of stop bits
	 */
	if (asp->line_control_reg.bits.no_of_stop_bits
	!= newLCR.bits.no_of_stop_bits)
		host_com_ioctl(adapter, HOST_COM_STOPBITS,
			newLCR.bits.no_of_stop_bits + 1);

	/* What are new settings to check for a difference */
#ifdef NTVDM
#error "Unsupported SoftPC mirror configuration: NTVDM"
#else
#line 2999
	if (newLCR.bits.parity_enabled == PARITY_OFF)
#endif
#line 3001
	{
		newParity = HOST_COM_PARITY_NONE;
	}
	else if (newLCR.bits.stick_parity == PARITY_STICK)
	{
#ifdef NTVDM
#error "Unsupported SoftPC mirror configuration: NTVDM"
#else
#line 3009
		newParity = newLCR.bits.even_parity == PARITY_ODD ?
#endif
#line 3011
			HOST_COM_PARITY_MARK : HOST_COM_PARITY_SPACE;
	}
	else /* regular parity */
	{
#ifdef NTVDM
#error "Unsupported SoftPC mirror configuration: NTVDM"
#else
#line 3018
		newParity = newLCR.bits.even_parity == PARITY_ODD ?
#endif
#line 3020
			HOST_COM_PARITY_ODD : HOST_COM_PARITY_EVEN;
	}

	/*
	 * Try to make sense of the current parity setting
	 */
#ifdef NTVDM
#error "Unsupported SoftPC mirror configuration: NTVDM"
#else
#line 3029
	if (asp->line_control_reg.bits.parity_enabled == PARITY_OFF)
#endif
#line 3031
	{
		parity = HOST_COM_PARITY_NONE;
	}
	else if (asp->line_control_reg.bits.stick_parity == PARITY_STICK)
	{
#ifdef NTVDM
#error "Unsupported SoftPC mirror configuration: NTVDM"
#else
#line 3039
		parity = asp->line_control_reg.bits.even_parity == PARITY_ODD ?
#endif
#line 3041
			HOST_COM_PARITY_MARK : HOST_COM_PARITY_SPACE;
	}
	else /* regular parity */
	{
#ifdef NTVDM
#error "Unsupported SoftPC mirror configuration: NTVDM"
#else
#line 3048
		parity = asp->line_control_reg.bits.even_parity == PARITY_ODD ?
#endif
#line 3050
			HOST_COM_PARITY_ODD : HOST_COM_PARITY_EVEN;
	}

	if (newParity != parity)
		host_com_ioctl(adapter, HOST_COM_PARITY, newParity);

#ifdef NTVDM
#error "Unsupported SoftPC mirror configuration: NTVDM"
#endif
#line 1878

	/* finally update the current line control settings */
	asp->line_control_reg.all = (unsigned char)value;
}
#endif // NEC_98
#line 1882

#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else // NEC_98
#line 3098
static void set_dtr IFN1(int, adapter)
{
	/*
	 * Process the DTR control bit, Bit 0 of the Modem Control
	 * Register.
	 */
	struct ADAPTER_STATE *asp = &adapter_state[adapter];
	
	switch ( change_state((int)asp->modem_control_reg.bits.DTR,
				asp->dtr_state) )
	{
	case ON:
		asp->dtr_state = ON;
		if (asp->loopback_state == OFF)
		{
			/* set the real DTR modem output */
			host_com_ioctl(adapter, HOST_COM_SDTR, 0);
		}
		else
		{
			/*
			 * loopback the DTR modem output into the
			 * DSR modem input
			 */
			asp->modem_status_reg.bits.DSR = ON;
			asp->modem_status_reg.bits.delta_DSR = ON;
			host_com_msr_callback (adapter, asp->modem_status_reg.all);

			raise_ms_interrupt(asp);
		}
		MODEM_STATE_CHANGE();
		break;
	
	case OFF:
		asp->dtr_state = OFF;
		if (asp->loopback_state == OFF)
		{
			/* clear the real DTR modem output */
			host_com_ioctl(adapter, HOST_COM_CDTR, 0);
		}
		else
		{
			/*
			 * loopback the DTR modem output into the
			 * DSR modem input
			 */
			asp->modem_status_reg.bits.DSR = OFF;
			asp->modem_status_reg.bits.delta_DSR = ON;
			host_com_msr_callback (adapter, asp->modem_status_reg.all);

			raise_ms_interrupt(asp);
		}
		MODEM_STATE_CHANGE();
		break;
	
	case LEAVE_ALONE:
		break;
	}
}
#endif // NEC_98
#line 1942

#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else // NEC_98
#line 3188
static void set_rts IFN1(int, adapter)
{
	/*
	 * Process the RTS control bit, Bit 1 of the Modem Control
	 * Register.
	 */
	struct ADAPTER_STATE *asp = &adapter_state[adapter];
	
	switch ( change_state((int)asp->modem_control_reg.bits.RTS,
				asp->rts_state) )
	{
	case ON:
		asp->rts_state = ON;
		if (asp->loopback_state == OFF)
		{
			/* set the real RTS modem output */
			host_com_ioctl(adapter, HOST_COM_SRTS, 0);
		}
		else
		{
			/* loopback the RTS modem out into the CTS modem in */
			asp->modem_status_reg.bits.CTS = ON;
			asp->modem_status_reg.bits.delta_CTS = ON;
			host_com_msr_callback (adapter, asp->modem_status_reg.all);

			raise_ms_interrupt(asp);
		}
		MODEM_STATE_CHANGE();
		break;
	
	case OFF:
		asp->rts_state = OFF;
		if (asp->loopback_state == OFF)
		{
			/* clear the real RTS modem output */
			host_com_ioctl(adapter, HOST_COM_CRTS, 0);
		}
		else
		{
			/* loopback the RTS modem out into the CTS modem in */
			asp->modem_status_reg.bits.CTS = OFF;
			asp->modem_status_reg.bits.delta_CTS = ON;
			host_com_msr_callback (adapter, asp->modem_status_reg.all);

			raise_ms_interrupt(asp);
		}
		MODEM_STATE_CHANGE();
		break;
	
	case LEAVE_ALONE:
		break;
	}
}
#endif // NEC_98
#line 1996

#ifndef NEC_98
#line 3244
static void set_out1 IFN1(int, adapter)
{
	/*
	 * Process the OUT1 control bit, Bit 2 of the Modem Control
	 * Register.
	 */
	struct ADAPTER_STATE *asp = &adapter_state[adapter];
	
	switch ( change_state((int)asp->modem_control_reg.bits.OUT1,
				asp->out1_state) )
	{
	case ON:
		asp->out1_state = ON;
		if (asp->loopback_state == OFF)
		{
			/*
			 * In the real adapter, this modem control output
			 * signal is not connected; so no real modem
			 * control change is required
			 */
		}
		else
		{
			/* loopback the OUT1 modem out into the RI modem in */
			asp->modem_status_reg.bits.RI = ON;
			host_com_msr_callback (adapter, asp->modem_status_reg.all);
		}
		MODEM_STATE_CHANGE();
		break;
	
	case OFF:
		asp->out1_state = OFF;
		if (asp->loopback_state == OFF)
		{
			/*
			 * In the real adapter, this modem control output
			 * signal is not connected; so no real modem control
			 * change is required
			 */
		}
		else
		{
			/* loopback the OUT1 modem out into the RI modem in */
			asp->modem_status_reg.bits.RI = OFF;
			asp->modem_status_reg.bits.TERI = ON;
			host_com_msr_callback (adapter, asp->modem_status_reg.all);

			raise_ms_interrupt(asp);
		}
		MODEM_STATE_CHANGE();
		break;
	
	case LEAVE_ALONE:
		break;
	}
}

static void set_out2 IFN1(int, adapter)
{
	/*
	 * Process the OUT2 control bit, Bit 3 of the Modem Control
	 * Register.
	 */
	struct ADAPTER_STATE *asp = &adapter_state[adapter];
	
	switch ( change_state((int)asp->modem_control_reg.bits.OUT2,
				asp->out2_state) )
	{
	case ON:
		asp->out2_state = ON;
		if (asp->loopback_state == OFF)
		{
			/*
			 * In the real adapter, this modem control output
			 * signal is used to determine whether the
			 * communications card should send interrupts; so
			 * check for immediately actionable interrupts.
			 * If you change this code, change the equivalent code
			 * for the interrupt enable register.
			 */
			if ( asp->line_status_reg.bits.data_ready == 1 )
				raise_rda_interrupt(asp);
			if ( asp->line_status_reg.bits.tx_holding_empty == 1 )
				raise_thre_interrupt(asp);
		}
		else
		{
			/* loopback the OUT2 modem output into the RLSD modem input */
			asp->modem_status_reg.bits.RLSD = ON;
			asp->modem_status_reg.bits.delta_RLSD = ON;
			host_com_msr_callback (adapter, asp->modem_status_reg.all);

			raise_ms_interrupt(asp);
		}
		MODEM_STATE_CHANGE();
		break;
	
	case OFF:
		asp->out2_state = OFF;
		if (asp->loopback_state == OFF)
		{
			/*
			 * In the real adapter, this modem control output signal
			 * is used to determine whether the communications
			 * card should send interrupts; so no real modem
			 * control change is required
			 */
		}
		else
		{
			/* loopback the OUT2 modem out into the RLSD modem in */
			asp->modem_status_reg.bits.RLSD = OFF;
			asp->modem_status_reg.bits.delta_RLSD = ON;
			host_com_msr_callback (adapter, asp->modem_status_reg.all);

			raise_ms_interrupt(asp);
		}
		MODEM_STATE_CHANGE();
		break;
	
	case LEAVE_ALONE:
		break;
	}
}

static void set_loopback IFN1(int, adapter)
{
	/*
	 * Process the loopback control bit, Bit 4 of the Modem Control
	 * Register.
	 */
	struct ADAPTER_STATE *asp = &adapter_state[adapter];
	
	switch ( change_state((int)asp->modem_control_reg.bits.loop,
				asp->loopback_state) )
	{
		case ON:
		asp->loopback_state = ON;
		/*
		 * Subsequent calls to set_dtr(), set_rts(), set_out1() and
		 * set_out2() will cause the modem control inputs to be set
		 * according to the the modem control outputs
		 */
		break;
	
	case OFF:
		asp->loopback_state = OFF;
		/*
		 * Set the modem control inputs according to the real
		 * modem state
		 */
		modem_change(adapter);
		break;
	
	case LEAVE_ALONE:
		break;
	}
}
#endif // NEC_98
#line 2155

#ifdef SHORT_TRACE
#error "Unsupported SoftPC mirror configuration: SHORT_TRACE"
#endif
#line 3425


void com1_flush_printer IFN0()
{
#ifdef NTVDM
#error "Unsupported SoftPC mirror configuration: NTVDM"
#endif
#line 3432

	host_com_ioctl(COM1, HOST_COM_FLUSH, 0);

#ifdef NTVDM
#error "Unsupported SoftPC mirror configuration: NTVDM"
#endif
#line 2184
}

void com2_flush_printer IFN0()
{
#ifdef NTVDM
#error "Unsupported SoftPC mirror configuration: NTVDM"
#endif
#line 3445

       host_com_ioctl(COM2, HOST_COM_FLUSH, 0);

#ifdef NTVDM
#error "Unsupported SoftPC mirror configuration: NTVDM"
#endif
#line 2191
}


#ifdef SEGMENTATION
#error "Unsupported SoftPC mirror configuration: SEGMENTATION"
#endif
#line 2202

#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else // NEC_98
#line 3526
static void com_reset IFN1(int, adapter)
{
	struct ADAPTER_STATE *asp = &adapter_state[adapter];

	/* setup the LCRFlushMask if it has not already been setup */
	if (!LCRFlushMask.all)
	{
		LCRFlushMask.all = ~0;	 /* turn all bits on */

		/*
		 * Now turn off the bits that should NOT cause the input
		 * to be flushed.  Note set_break is handled seperately by
		 * the set_break() routine.
		 */
		LCRFlushMask.bits.DLAB = 0;
		LCRFlushMask.bits.no_of_stop_bits = 0;
		LCRFlushMask.bits.set_break = 0;
	}
		
	/*
	 * Set default state of all adapter registers
	 */
	asp->int_enable_reg.all = 0;


	
	asp->int_id_reg.all = 0;
	asp->int_id_reg.bits.no_int_pending = 1;
	
	/* make sure a change occurs to 0 */
	asp->line_control_reg.all = ~0;
	
	/*
	 * set up modem control reg so next set_dtr etc.
	 * Will produce required status
	 */
	asp->modem_control_reg.all = 0;
	asp->modem_control_reg.bits.DTR = ON;
	asp->modem_control_reg.bits.RTS = ON;
	asp->modem_control_reg.bits.OUT1 = ON;
	asp->modem_control_reg.bits.OUT2 = ON;
	host_com_ioctl(adapter, HOST_COM_SDTR, 0);
	host_com_ioctl(adapter, HOST_COM_SRTS, 0);

	asp->line_status_reg.all = 0;
	asp->line_status_reg.bits.tx_holding_empty = 1;
	asp->line_status_reg.bits.tx_shift_empty = 1;
	
	asp->modem_status_reg.all = 0;
	MODEM_STATE_CHANGE();
	host_com_msr_callback (adapter, asp->modem_status_reg.all);
	
	/*
	 * Set up default state of our state variables
	 */
	asp->receiver_line_status_interrupt_state = OFF;
	asp->data_available_interrupt_state = OFF;
	asp->tx_holding_register_empty_interrupt_state = OFF;
	asp->modem_status_interrupt_state = OFF;
	asp->break_state = OFF;
	asp->loopback_state = OFF;
	asp->dtr_state = ON;
	asp->rts_state = ON;
	asp->out1_state = ON;
	asp->out2_state = ON;
#if defined(NTVDM) && defined(FIFO_ON)
#error "Unsupported SoftPC mirror configuration: excluded conjunction"
#endif
#line 2268
		
	/*
	 * Reset adapter synchronisation
	 */
	com_critical_reset(adapter);

	/*
	 * Set Unix devices to default state
	 */
	set_baud_rate(adapter);
	set_line_control(adapter, 0);
	set_break(adapter);

	/* Must be called before set_dtr */
	set_loopback(adapter);
	set_dtr(adapter);
	set_rts(adapter);
	set_out1(adapter);
	set_out2(adapter);

#ifdef IRET_HOOKS
#error "Unsupported SoftPC mirror configuration: IRET_HOOKS"
#endif /* IRET_HOOKS */
#line 2304
}
#endif // NEC_98
#line 3642

#ifndef COM3_ADAPTOR
#error "Unsupported SoftPC mirror configuration: !COM3_ADAPTOR"
#endif
#line 3646
#ifndef COM4_ADAPTOR
#error "Unsupported SoftPC mirror configuration: !COM4_ADAPTOR"
#endif
#line 2312

#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else // NEC_98
#line 3657
static IU8 com_adaptor[4] = {COM1_ADAPTOR,COM2_ADAPTOR,
                             COM3_ADAPTOR,COM4_ADAPTOR};
static io_addr port_start[4] = {RS232_COM1_PORT_START,
				RS232_COM2_PORT_START,
				RS232_COM3_PORT_START,
				RS232_COM4_PORT_START};
static io_addr port_end[4] = {RS232_COM1_PORT_END,
                          RS232_COM2_PORT_END,
                          RS232_COM3_PORT_END,
                          RS232_COM4_PORT_END};
static int int_pri[4] = {CPU_RS232_PRI_INT,
                         CPU_RS232_SEC_INT,
                         CPU_RS232_PRI_INT,
                         CPU_RS232_SEC_INT};
static int timeout[4] = {RS232_COM1_TIMEOUT,
                         RS232_COM2_TIMEOUT,
                         RS232_COM3_TIMEOUT,
                         RS232_COM4_TIMEOUT};
#endif // NEC_98
#line 2331


#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else // NEC_98
#line 3777
GLOBAL VOID com_init IFN1(int, adapter)
{
	io_addr i;

#ifdef NTVDM
#error "Unsupported SoftPC mirror configuration: NTVDM"
#endif
#line 2337

	adapter_state[adapter].had_first_read = FALSE;
	
	/* Set up the IO chip select logic for this adaptor */
#ifdef NTVDM
#error "Unsupported SoftPC mirror configuration: NTVDM"
#else
#line 2341
	io_define_inb(com_adaptor[adapter], com_inb);
	io_define_outb(com_adaptor[adapter], com_outb);
#endif /* NTVDM */
#line 2343

	for(i = port_start[adapter]; i <= port_end[adapter]; i++)
		io_connect_port(i, com_adaptor[adapter], IO_READ_WRITE);


	adapter_state[adapter].hw_interrupt_priority = int_pri[adapter];

	/* reset adapter state */
	host_com_reset(adapter);

	/* reset adapter state */
	com_reset(adapter);

	/* Should we enable TX pacing ? */
	tx_pacing_enabled = host_getenv("TX_PACING_ENABLED") ? TRUE : FALSE;

	return;
}
#endif // NEC_98
#line 3826

void com_post IFN1(int, adapter)
{
        /* Set up BIOS data area. */
	sas_storew( BIOS_VAR_START + (2*adapter), port_start[adapter]);
	sas_store(timeout[adapter] , (half_word)1 );
}

void com_close IFN1(int, adapter)
{
#ifdef NTVDM
#error "Unsupported SoftPC mirror configuration: NTVDM"
#endif
#line 2371

#ifndef PROD
#error "Unsupported SoftPC mirror configuration: non-PROD"
#endif
#line 3845
	/* reset host specific communications channel */
	config_activate((UTINY)(C_COM1_NAME + adapter), FALSE);

#ifdef NTVDM
#error "Unsupported SoftPC mirror configuration: NTVDM"
#endif
#line 2380
}

#ifdef NTVDM
#error "Unsupported SoftPC mirror configuration: NTVDM"
#endif /* NTVDM */
#line 3954

#ifdef PS_FLUSHING
#error "Unsupported SoftPC mirror configuration: PS_FLUSHING"
#endif	/* PS_FLUSHING */
#line 3991



/********************************************************/
/* Com debugging shell - Ade Brownlow / Ian Wellock
 * NB: This stuff only works for COM1. It is called from yoda using 'cd'
 * - comdebug - from the yoda command line....
 */
#ifndef PROD
#error "Unsupported SoftPC mirror configuration: !PROD"
#endif /* !PROD */
#line 2882
/********************************************************/
