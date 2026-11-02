#include "insignia.h"
#include "host_def.h"
/*[
	Name:		illegal_op.c
	Derived From:	Base 2.0
	Author:		William Gulland
	Created On:	Unknown
	Sccs ID:	@(#)illegal_op.c	1.19 07/04/95
	Notes:		Called from the CPU.
	Purpose:	The CPU has encountered an illegal op code.

	(c)Copyright Insignia Solutions Ltd., 1990. All rights reserved.

]*/

#ifdef SEGMENTATION
#error "Unsupported SoftPC mirror configuration: SEGMENTATION"
#endif
#line 24


/*
 *    O/S include files.
 */
#include <stdio.h>
#include TypesH

/*
 * SoftPC include files
 */
#include "xt.h"
#include "sas.h"
#include CpuH
#include "bios.h"
#include "error.h"
#include "config.h"
#include "debug.h"
#include "yoda.h"

#ifndef PROD
#error "Unsupported SoftPC mirror configuration: non-PROD"
#endif
#line 47

/* Routine to produce human readable form of where an illegal instruction occured */
LOCAL VOID where IFN3(CHAR *, string, word, cs, LIN_ADDR, ip)
{
	double_word ea = effective_addr(cs, ip);

	sprintf(string,
#ifdef	PROD
		"CS:%04x IP:%04x OP:%02x %02x %02x %02x %02x",
#else	/* PROD */
#error "Unsupported SoftPC mirror configuration: !PROD"
#endif	/* PROD */
#line 59
		cs, ip,
		sas_hw_at(ea), sas_hw_at(ea+1), sas_hw_at(ea+2),
		sas_hw_at(ea+3),sas_hw_at(ea+4));
}



#if defined(NTVDM) && defined(MONITOR)
#error "Unsupported SoftPC mirror configuration: NTVDM and MONITOR"
#endif
#line 68

void illegal_op()
{
#ifndef	PROD
#error "Unsupported SoftPC mirror configuration: non-PROD"
#endif
#line 74
}

void illegal_op_int()
{
	CHAR string[100];
	word cs, ip;

#ifdef NTVDM
#error "Unsupported SoftPC mirror configuration: NTVDM"
#endif
#line 88

	/* the cs and ip of the faulting instruction should be on the top of the stack */
	sys_addr stack;

	stack=effective_addr(getSS(),getESP());

	ip = sas_hw_at(stack) + (sas_hw_at(stack+1)<<8);
	cs = sas_hw_at(stack+2) + (sas_hw_at(stack+3)<<8);

	where(string, cs, ip);

#ifndef NTVDM
#line 92
#ifdef PROD
	host_error(EG_BAD_OP, ERR_QU_CO_RE, string);
#else  /* PROD */
#error "Unsupported SoftPC mirror configuration: !PROD"
#endif /* PROD */
#line 98

#else /* NTVDM */
#error "Unsupported SoftPC mirror configuration: NTVDM"
#endif /* NTVDM */
#line 121



	/* the user has requested a `continue` */
	/* we don't know how many bytes this instr should be, so guess 1 */
	if (ip == 0xffff) {
		cs ++;
		sas_store (stack+2, (IU8)(cs & 0xff));
		sas_store (stack+3, (IU8)((cs >> 8) & 0xff));
	}
	ip ++;
	sas_store (stack , (IU8)(ip & 0xff));
	sas_store (stack+1, (IU8)((ip >> 8) & 0xff));
	unexpected_int();
}


void illegal_dvr_bop IFN0()
{
#ifndef NTVDM
#line 118
	sys_addr bop_addr;
	CHAR buf[256];

	/* This is called when an Insignia Intel driver decides that
	 * this (old) SoftWindows is not compatible.
	 *
	 * We should:
	 * a) Put up a localised panel complaining that
	 *    the named driver CS:[eIP] with decimal
	 *    version AX is incompatible with the SoftWindows.
	 *    N.B. The most compatible way to pass the
	 *    name of the driver is by embedded bytes just
	 *    after the BOP. The problem is caused by the
	 *    fact that the driver may be either 16-bit RM
	 *    or a 32-bit flat VxD so the address of the
	 *    string can be either 16/32 bits, and we
	 *    need to be able to execute (and do nothing)
	 *    on the shipping SoftPC 1.xx which prevents
	 *    us doing anything with 32-bit registers!
	 *
	 *	BOP	driver_incompat
	 *	jmp	SHORT over_name
	 *	db	'somename.drv', 0
	 * over_name:
	 *
	 * b) setCF(0)
	 */

	buf[0] = '\0';
	bop_addr = effective_addr(getCS(), GetInstructionPointer());
	if (sas_hw_at(bop_addr) == 0xEB)
	{
		IU8 data;
		char *p;

		p = buf;
		bop_addr += 2;	/* Skip the xEB xXX */
		do {
			data = sas_hw_at(bop_addr++);
			*p++ = data;
		} while (data != 0);
		sprintf(p-1, " v%d.%02d", getAX() / 100, getAX() % 100);
	}
	host_error(EG_DRIVER_MISMATCH, ERR_CONT, buf);
	setCF(0);
#endif /* ! NTVDM */
#line 163
}


#ifndef PROD
#error "Unsupported SoftPC mirror configuration: non-PROD"
#endif /* ! PROD */
#line 372
