/*
 * VPC-XT Revision 2.0
 *
 * Title	: PPI Adpator definitions
 *
 * Description	: Definitions for users of the PPI Adaptor 
 *
 * Author	: Henry Nash
 *
 * Notes	: None
 */

/* SccsID[]="@(#)ppi.h	1.4 08/10/92 Copyright Insignia Solutions Ltd."; */

/*
 * ============================================================================
 * Structure/Data definitions
 * ============================================================================
 */

/* None */

/*
 * ============================================================================
 * External declarations and macros
 * ============================================================================
 */

#ifdef ANSI
#error "Unsupported SoftPC mirror configuration: ANSI"
#else
#line 34
extern void ppi_init();
extern void ppi_inb();
extern void ppi_outb();
#endif
