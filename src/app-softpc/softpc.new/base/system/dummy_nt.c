#include "insignia.h"
#include "host_def.h"
/*
 * VPC Version 1.0
 *
 * Title         : dummy_int.c
 *
 * Decription    : Provide a function which emulates the dummy interrupt
 *                 within the IBM PC BIOS.
 *
 * Author        :
 *
 * Notes         :
 */

/* @(#)dummy_int.c	1.4 08/10/92 Copyright Insignia Solutions Ltd. */
#ifdef SCCSID
#error "Unsupported SoftPC mirror configuration: SCCSID"
#endif
#line 19

#ifdef SEGMENTATION
#error "Unsupported SoftPC mirror configuration: SEGMENTATION"
#endif
#line 28

void dummy_int()
{
    ;
}
