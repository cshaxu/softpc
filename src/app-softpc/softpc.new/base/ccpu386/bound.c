/*[

bound.c

LOCAL CHAR SccsID[]="@(#)bound.c	1.6 03/28/94";

BOUND CPU functions.
--------------------

]*/


#include <insignia.h>

#include <host_def.h>
#include <xt.h>
#include <c_main.h>
#include <c_addr.h>
#include <c_bsic.h>
#include <c_prot.h>
#include <c_seg.h>
#include <c_stack.h>
#include <c_xcptn.h>
#include	<c_reg.h>
#include <bound.h>


/*
   =====================================================================
   EXTERNAL ROUTINES STARTS HERE.
   =====================================================================
 */


GLOBAL VOID
BOUND
#ifdef ANSI
#error "Unsupported SoftPC mirror configuration: ANSI"
#else
#line 44
   (op1, op2, op_sz)
   IU32 op1;
   IU32 op2[2];
   IUM8 op_sz;
#endif
   {
   IS32 value;
   IS32 lower;
   IS32 upper;

   /* transfer to local signed variables */
   value = op1;
   lower = op2[0];
   upper = op2[1];

   if ( op_sz == 16 )
      {
      /* sign extend operands */
      if ( value & BIT15_MASK )
	 value |= ~WORD_MASK;

      if ( lower & BIT15_MASK )
	 lower |= ~WORD_MASK;

      if ( upper & BIT15_MASK )
	 upper |= ~WORD_MASK;
      }

   op_sz = op_sz / 8;   /* determine number of bytes in operand */

   if ( value < lower || value > upper )
      Int5();
   }
