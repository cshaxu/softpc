#ifndef _RS232_H
#define _RS232_H

/*[
	Name:		rs232.h
	Derived From:	Base 2.0
	Author:		Paul Huckle
	Created On:	
	Sccs ID:	05/11/94 @(#)rs232.h	1.14
	Purpose:	Definitions for users of the RS232 Adapter Module

	(c)Copyright Insignia Solutions Ltd., 1990. All rights reserved.

]*/

/*
 * ============================================================================
 * Structure/Data definitions
 * ============================================================================
 */

/* register type definitions follow: */

typedef half_word BUFFER_REG;

#ifdef LITTLEND
typedef union {
   word all;
   struct {
      WORD_BIT_FIELD LSByte:8;
      WORD_BIT_FIELD MSByte:8;
   } byte;
} DIVISOR_LATCH;
#endif
#ifdef BIGEND
typedef union {
   word all;
   struct {
      WORD_BIT_FIELD MSByte:8;
      WORD_BIT_FIELD LSByte:8;
   } byte;
} DIVISOR_LATCH;
#endif

#ifdef BIT_ORDER2
typedef union {
 	half_word all;
	struct {
		 HALF_WORD_BIT_FIELD data_available:1;
		 HALF_WORD_BIT_FIELD tx_holding:1;
		 HALF_WORD_BIT_FIELD rx_line:1;
		 HALF_WORD_BIT_FIELD modem_status:1;
		 HALF_WORD_BIT_FIELD pad:4;
	       } bits;
      } INT_ENABLE_REG;
#endif
#ifdef BIT_ORDER1
typedef union {
 	half_word all;
	struct {
		 HALF_WORD_BIT_FIELD pad:4;
		 HALF_WORD_BIT_FIELD modem_status:1;
		 HALF_WORD_BIT_FIELD rx_line:1;
		 HALF_WORD_BIT_FIELD tx_holding:1;
		 HALF_WORD_BIT_FIELD data_available:1;
	       } bits;
      } INT_ENABLE_REG;
#endif


#ifdef BIT_ORDER2
typedef union {
 	half_word all;
	struct {
		 HALF_WORD_BIT_FIELD no_int_pending:1;
		 HALF_WORD_BIT_FIELD interrupt_ID:2;
		 HALF_WORD_BIT_FIELD pad:5;
	       } bits;
      } INT_ID_REG;
#endif
#ifdef BIT_ORDER1
typedef union {
 	half_word all;
	struct {
		 HALF_WORD_BIT_FIELD pad:5;
		 HALF_WORD_BIT_FIELD interrupt_ID:2;
		 HALF_WORD_BIT_FIELD no_int_pending:1;
	       } bits;
      } INT_ID_REG;
#endif


#ifdef BIT_ORDER2
typedef union {
 	half_word all;
	struct {
		 HALF_WORD_BIT_FIELD word_length:2;
		 HALF_WORD_BIT_FIELD no_of_stop_bits:1;
		 HALF_WORD_BIT_FIELD parity_enabled:1;
		 HALF_WORD_BIT_FIELD even_parity:1;
		 HALF_WORD_BIT_FIELD stick_parity:1;
		 HALF_WORD_BIT_FIELD set_break:1;
		 HALF_WORD_BIT_FIELD DLAB:1;
	       } bits;
      } LINE_CONTROL_REG;
#endif
#ifdef BIT_ORDER1
typedef union {
 	half_word all;
	struct {
		 HALF_WORD_BIT_FIELD DLAB:1;
		 HALF_WORD_BIT_FIELD set_break:1;
		 HALF_WORD_BIT_FIELD stick_parity:1;
		 HALF_WORD_BIT_FIELD even_parity:1;
		 HALF_WORD_BIT_FIELD parity_enabled:1;
		 HALF_WORD_BIT_FIELD no_of_stop_bits:1;
		 HALF_WORD_BIT_FIELD word_length:2;
	       } bits;
      } LINE_CONTROL_REG;
#endif

#ifdef BIT_ORDER2
typedef union {
 	half_word all;
	struct {
		 HALF_WORD_BIT_FIELD DTR:1;
		 HALF_WORD_BIT_FIELD RTS:1;
		 HALF_WORD_BIT_FIELD OUT1:1;
		 HALF_WORD_BIT_FIELD OUT2:1;
		 HALF_WORD_BIT_FIELD loop:1;
		 HALF_WORD_BIT_FIELD pad:3;
	       } bits;
      } MODEM_CONTROL_REG;
#endif
#ifdef BIT_ORDER1
typedef union {
 	half_word all;
	struct {
		 HALF_WORD_BIT_FIELD pad:3;
		 HALF_WORD_BIT_FIELD loop:1;
		 HALF_WORD_BIT_FIELD OUT2:1;
		 HALF_WORD_BIT_FIELD OUT1:1;
		 HALF_WORD_BIT_FIELD RTS:1;
		 HALF_WORD_BIT_FIELD DTR:1;
	       } bits;
      } MODEM_CONTROL_REG;
#endif

#ifdef BIT_ORDER2
typedef union {
 	half_word all;
	struct {
		 HALF_WORD_BIT_FIELD data_ready:1;
		 HALF_WORD_BIT_FIELD overrun_error:1;
		 HALF_WORD_BIT_FIELD parity_error:1;
		 HALF_WORD_BIT_FIELD framing_error:1;
		 HALF_WORD_BIT_FIELD break_interrupt:1;
		 HALF_WORD_BIT_FIELD tx_holding_empty:1;
		 HALF_WORD_BIT_FIELD tx_shift_empty:1;
		 HALF_WORD_BIT_FIELD pad:1;
	       } bits;
      } LINE_STATUS_REG;
#endif
#ifdef BIT_ORDER1
typedef union {
 	half_word all;
	struct {
		 HALF_WORD_BIT_FIELD pad:1;
		 HALF_WORD_BIT_FIELD tx_shift_empty:1;
		 HALF_WORD_BIT_FIELD tx_holding_empty:1;
		 HALF_WORD_BIT_FIELD break_interrupt:1;
		 HALF_WORD_BIT_FIELD framing_error:1;
		 HALF_WORD_BIT_FIELD parity_error:1;
		 HALF_WORD_BIT_FIELD overrun_error:1;
		 HALF_WORD_BIT_FIELD data_ready:1;
	       } bits;
      } LINE_STATUS_REG;
#endif

#ifdef BIT_ORDER2
typedef union {
 	half_word all;
	struct {
		 HALF_WORD_BIT_FIELD delta_CTS:1;
		 HALF_WORD_BIT_FIELD delta_DSR:1;
		 HALF_WORD_BIT_FIELD TERI:1;
		 HALF_WORD_BIT_FIELD delta_RLSD:1;
		 HALF_WORD_BIT_FIELD CTS:1;
		 HALF_WORD_BIT_FIELD DSR:1;
		 HALF_WORD_BIT_FIELD RI:1;
		 HALF_WORD_BIT_FIELD RLSD:1;
	       } bits;
      } MODEM_STATUS_REG;
#endif
#ifdef BIT_ORDER1
typedef union {
 	half_word all;
	struct {
		 HALF_WORD_BIT_FIELD RLSD:1;
		 HALF_WORD_BIT_FIELD RI:1;
		 HALF_WORD_BIT_FIELD DSR:1;
		 HALF_WORD_BIT_FIELD CTS:1;
		 HALF_WORD_BIT_FIELD delta_RLSD:1;
		 HALF_WORD_BIT_FIELD TERI:1;
		 HALF_WORD_BIT_FIELD delta_DSR:1;
		 HALF_WORD_BIT_FIELD delta_CTS:1;
	       } bits;
      } MODEM_STATUS_REG;
#endif


/* register select code definitions follow: */

#define RS232_TX_RX	0
#define RS232_IER	1
#define RS232_IIR	2
#define RS232_LCR	3
#define RS232_MCR	4
#define RS232_LSR	5
#define RS232_MSR	6
#define RS232_SCRATCH	7

#define RS232_COM1_TIMEOUT (BIOS_VAR_START + 0x7c)
#define RS232_COM2_TIMEOUT (BIOS_VAR_START + 0x7d)
#define RS232_COM3_TIMEOUT (BIOS_VAR_START + 0x7e)
#define RS232_COM4_TIMEOUT (BIOS_VAR_START + 0x7f)
#define RS232_PRI_TIMEOUT (BIOS_VAR_START + 0x7c)
#define RS232_SEC_TIMEOUT (BIOS_VAR_START + 0x7d)

#define GO 0           /* We can emulate requested configuration */
#define NO_GO_SPEED 1  /* We can't emulate requested line speed */
#define NO_GO_LINE  2  /* We can't emulate requested line setup */


#define OFF 0
#define ON 1
#define LEAVE_ALONE 2
#define	change_state(external_state, internal_state) \
	((external_state == internal_state) ? LEAVE_ALONE : external_state)


#define RLS_INT 3     /* receiver line status interrupt ID */
#define RDA_INT 2     /* data available interrupt ID */
#define THRE_INT 1    /* tx holding register empty interrupt ID */
#define MS_INT 0      /* modem status interrupt ID */

#define DATA5 0       /* line control setting for five data bits */
#define DATA6 1       /* line control setting for six data bits */
#define DATA7 2       /* line control setting for seven data bits */
#define DATA8 3       /* line control setting for eight data bits */

#define STOP1 0       /* line control setting for one stop bit */
#define STOP2 1       /* line control setting for one and a half or two
                         stop bits */

#define PARITY_ON 1   /* line control setting for parity enabled */
#define PARITY_OFF 0  /* line control setting for parity disabled */

#define PARITY_ODD 0  /* line control setting for odd parity */
#define PARITY_EVEN 1 /* line control setting for even parity */

#define PARITY_STICK 1  /* line control setting for stick(y) parity */

#define PARITY_FIXED 2  /* Internal state setting for fixed parity */

#define COM1 0
#define COM2 1
#if (NUM_SERIAL_PORTS > 2)
#define COM3 2
#define COM4 3
#endif


/*
 * ============================================================================
 * External declarations and macros
 * ============================================================================
 */

extern void com_init IPT1(int, adapter);
extern void com_post IPT1(int, adapter);

extern void com_flush_printer IPT1(int, adapter);

extern void com_inb IPT2(io_addr, port, half_word *, value);
extern void com_outb IPT2(io_addr, port, half_word, value);

extern void com_recv_char IPT1(int, adapter);
extern void recv_char IPT1(long, adapter);
extern void com_modem_change IPT1(int, adapter);
extern void com_save_rxbytes IPT2(int,n, CHAR *,buf);
extern void com_save_txbyte IPT1(CHAR,value);

#ifdef PS_FLUSHING
extern void com_psflush_change IPT2(IU8,hostID, IBOOL,apply);
#endif	/* PS_FLUSHING */


#if (NUM_SERIAL_PORTS > 2)
#define	adapter_for_port(port) \
	(((port & 0x300) == 0x300) ? \
		(((port & 0xf8) == 0xf8) ? COM1 : COM3) \
		        : \
		(((port & 0xf8) == 0xf8) ? COM2 : COM4))

#ifdef SHORT_TRACE
#define	id_for_adapter(adapter)	 	(adapter + '1')
#endif

#else

#define	adapter_for_port(port)	(((port) >= RS232_PRI_PORT_START && (port) <= RS232_PRI_PORT_END) ? COM1 : COM2)


#ifdef SHORT_TRACE
#define	id_for_adapter(adapter)	(adapter == COM1 ? 'P' : 'S')
#endif
#endif /* more than 2 serial ports */

#ifdef IRET_HOOKS
/*
 * A macro we need for IRET hooks, the number of bits in a an async
 * character on a comms line, which is about 8 (for the character)
 * plus two stop bits.
 */
#define BITS_PER_ASYNC_CHAR 10
#endif /* IRET_HOOKS */

/* BCN 2730 define generic macros which can be SVID3 or old style
 * in either case the structure used should be a termios
 */

#ifdef SVID3_TCGET
#define	TCGET TCGETS
#define	TCSET TCSETS
#define	TCSETF TCSETSF
#else
#define	TCGET TCGETA
#define	TCSET TCSETA
#define	TCSETF TCSETAF
#endif	/* SVID3_TCGET */

#endif /* _RS232_H */
