/*
 * File: egamode.h
 *
 * SccsID = @(#)egamode.h	1.3 08/10/92 Copyright Insignia Solutions Ltd.
 *
 */


extern	boolean	(*choose_display_mode)();
extern	boolean	choose_ega_display_mode();
#ifdef VGG
#line 12
#if defined(NEC_98)         
#error "Unsupported SoftPC mirror configuration: NEC_98"
#else  // !NEC_98
#line 17
extern	boolean	choose_vga_display_mode();
#endif // !NEC_98
#line 19
#endif

