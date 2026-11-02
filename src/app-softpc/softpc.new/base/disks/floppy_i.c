#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#endif // NEC_98
#line 7
#include "insignia.h"
#include "host_def.h"

extern void host_simulate();
/*
 * SoftPC Revision 3.0
 *
 *
 * Title	: Primary SFD BIOS floppy diskette functions
 *
 *
 * Description	: This module defines the floppy diskette BIOS functions
 *		  that are invoked directly from BOP instructions:
 *
 *		  diskette_io()		floppy diskette access functions
 *
 *		  diskette_post()	floppy diskette POST function
 *
 *		  diskette_int()	floppy diskette interrupt handler
 *
 *
 * Author	: Ross Beresford
 *
 *
 * Notes	:
 *
 */


/*
 * static char SccsID[]="@(#)floppy_io.c	1.14 03/02/94 Copyright Insignia Solutions Ltd.";
 */


#ifdef SEGMENTATION
#error "Unsupported SoftPC mirror configuration: SEGMENTATION"
#endif
#line 49

#include <stdio.h>
#include TypesH
#include "xt.h"
#include "bios.h"
#include "ios.h"
#include CpuH
#include "error.h"
#include "config.h"
#include "gfi.h"
#include "fla.h"
#include "sas.h"
#include "floppy.h"
#include "equip.h"
#include "trace.h"
#include "ica.h"
#include "debug.h"

#include "tape_io.h"

#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#endif // NEC_98
#line 71

#ifdef NTVDM
#error "Unsupported SoftPC mirror configuration: NTVDM"
#endif	/* NTVDM */
#line 76

#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#endif // NEC_98
#line 163

#ifndef	PROD
#error "Unsupported SoftPC mirror configuration: !PROD"
#endif /* nPROD */
#line 507


void diskette_io()
{
#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else  // !NEC_98
#line 524
	/*
	 *	Check for valid call and use secondary functions to
	 *	perform the required operation
	 *
	 *	Register inputs:
	 *		AH	operation required
	 *		DL	drive number
	 */
	half_word diskette_status;
	int op = getAH(), drive = getDL();
#endif // !NEC_98
#line 304

#ifndef	PROD
#error "Unsupported SoftPC mirror configuration: non-PROD"
#endif
#line 309

#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#endif // NEC_98
#line 553
#ifndef	JOKER
	/*
	 *	Enable interrupts
	 */

	setIF(1);	
#endif	/* JOKER */

	/*
	 *	Check operation required, using known invalid function
	 *	if operation is out of range
	 */

#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else  // !NEC_98
#line 571
	if (!fl_operation_in_range(op))
		op = FL_FNC_ERR;
#endif // !NEC_98
#line 325

	/*
	 *	If the drive number is applicable in the operation, check it
	 */

#ifndef NEC_98
#line 580
	if (op != FL_DISK_RESET && op != FL_DISK_STATUS && op != FL_DISK_PARMS)
		if (drive >= MAX_FLOPPY)
			op = FL_FNC_ERR;
#endif // !NEC_98
#line 333


#ifndef NEC_98
#line 335
	/*
	 *	Save previous diskette status, initialise current diskette
	 *	status to OK
	 */

	sas_load(FLOPPY_STATUS, &diskette_status);
	setAH(diskette_status);
	sas_store(FLOPPY_STATUS, FS_OK);
#endif // !NEC_98
#line 343

	/*
	 *	Do the operation
	 */

#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else  // !NEC_98
#line 608
	(*fl_fnc_tab[op])(drive);
#endif // !NEC_98
#line 349

#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#endif // NEC_98
#line 618



#ifndef	PROD
#error "Unsupported SoftPC mirror configuration: non-PROD"
#endif
#line 623
}


#ifndef	JOKER		/* Floppies handled v. weirdly on JOKER */

void diskette_int()
{
	/*
	 * The diskette interrupt service routine.  Mark the Seek Status to say
	 * the interrupt has occurred and terminate the interrupt.
	 */
	half_word seek_status;
	word savedAX, savedCS, savedIP;

	note_trace0(FLOPBIOS_VERBOSE, "diskette_int()");
	sas_load(SEEK_STATUS, &seek_status);
	sas_store(SEEK_STATUS, (IU8)(seek_status | SS_INT_OCCURRED));

	outb(ICA0_PORT_0, END_INTERRUPT);

	/*
	 *	Enable interrupts
	 */

	setIF(1);	

	/*
	 *	Notify OS that BIOS has completed a "wait" for
	 *	a diskette interrupt
	 */
	savedAX = getAX();
	savedCS = getCS();
	savedIP = getIP();

	setAH(INT15_INTERRUPT_COMPLETE);
	setAL(INT15_DEVICE_FLOPPY);
#ifdef NTVDM
#error "Unsupported SoftPC mirror configuration: NTVDM"
#else
#line 392
	setCS(RCPU_INT15_SEGMENT);
	setIP(RCPU_INT15_OFFSET);
#endif /* NTVDM */
#line 394

	host_simulate();

	setAX(savedAX);
	setCS(savedCS);
	setIP(savedIP);
}

#endif	/* ndef JOKER */


#ifdef SEGMENTATION
#error "Unsupported SoftPC mirror configuration: SEGMENTATION"
#endif
#line 685

void diskette_post()
{
	/*
	 *	This routine performs the diskette BIOS initialisation
	 *	functionality in the POST.
	 */
#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else  // !NEC_98
#line 696
	half_word diskette_id_reg, hf_cntrl, interrupt_mask;
	half_word disk_state;
	EQUIPMENT_WORD equip_flag;
#endif // !NEC_98
#line 702

	note_trace0(FLOPBIOS_VERBOSE, "diskette_post()");

#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else  // !NEC_98
#line 728
	/*
	 *	Set the diskette data rate to 250 kbs (low density)
	 */

	outb(DISKETTE_DCR_REG, DCR_RATE_250);

	/*
	 *	If there are any diskettes installed, check whether
	 *	they are accessed via a DUAL card or the old-style
	 *	adapter card
	 */

	sas_loadw(EQUIP_FLAG, &equip_flag.all);
	if (equip_flag.bits.diskette_present)
	{
		/*
		 *	Enable diskette interrupts
		 */

		inb(ICA0_PORT_1, &interrupt_mask);
		interrupt_mask &= ~(1 << CPU_DISKETTE_INT);
		outb(ICA0_PORT_1, interrupt_mask);

		/*
		 *	If a DUAL diskette/fixed disk adapter is fitted,
		 *	set the drive indicator for drive 0 accordingly
		 */

		inb(DISKETTE_ID_REG, &diskette_id_reg);
		sas_load(DRIVE_CAPABILITY, &hf_cntrl);
		hf_cntrl &= ~DC_DUAL;
		if ((diskette_id_reg & IDR_ID_MASK) == DUAL_CARD_ID)
			hf_cntrl |= DC_DUAL;
		sas_store(DRIVE_CAPABILITY, hf_cntrl);

#ifndef NTVDM	/* prevent floppies showing up in bios data area */
#line 461
		/*
		 *	Setup the diskette BIOS state according to the
		 *	types of drives installed
		 */
		fl_diskette_setup();

		/*
		 *	If a second drive was discovered, update the
		 *	equipment flag accordingly
		 */

		sas_load(FDD_STATUS+1, &disk_state);
		if (disk_state != 0)
		{
			sas_loadw(EQUIP_FLAG, &equip_flag.all);
			equip_flag.bits.max_diskette = 1;
			sas_storew(EQUIP_FLAG, equip_flag.all);
		}
#endif  /* NTVDM */
#line 479
	}
#endif // !NEC_98
#line 480
}

#if defined(NEC_98)
#error "Unsupported SoftPC mirror configuration: NEC_98"
#endif // NEC_98
#line 1180
