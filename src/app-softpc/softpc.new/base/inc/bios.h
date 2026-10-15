/*
 * VPC-XT Revision 1.0
 *
 * Title	: BIOS definitions
 *
 * Description	: Defintions for users of the BIOS
 *
 * Author	: Henry Nash
 *
 * Notes	: This is a copy of bios.h from henry/kernel taken on
 *		  17 dec 86. Several lines have been added to support
 *		  the hard disk bios (marked HD-dr). See also bios.c.
 *
 * Mods: (r2.21): Added an external declaration of rom_basic().
 */

/* SccsID[]="@(#)bios.h	1.47 06/28/95 Copyright Insignia Solutions Ltd."; */

/*
 * ============================================================================
 * Structure/Data definitions
 * ============================================================================
 */

/*
 * The following define the BOP call numbers for the BIOS functions.
 */

#define BIOS_RESET		0x0
#define BIOS_DUMMY_INT		0x1
#define BIOS_UNEXP_INT		0x2
#define BIOS_DRIVER_INCOMPAT	0x7
#define BIOS_TIMER_INT		0x8
#define BIOS_KB_INT		0x9
#define BIOS_DISK_INT		0xD	
#define BIOS_DISKETTE_INT	0xE
#define BIOS_PRINT_SCREEN	0x5
#define BIOS_VIDEO_IO		0x10
#define BIOS_EQUIPMENT		0x11
#define BIOS_MEMORY_SIZE	0x12
#define BIOS_DISK_IO		0x13
#define BIOS_RS232_IO		0x14
#define BIOS_CASSETTE_IO	0x15
#define BIOS_KEYBOARD_IO	0x16
#define BIOS_PRINTER_IO		0x17
#define BIOS_BASIC		0x18
#define BIOS_BOOT_STRAP		0x19
#define BIOS_TIME_OF_DAY	0x1A
#define BIOS_KEYBOARD_BREAK	0x1B
#define BIOS_USER_TIMER_INT	0x1C
#define BIOS_EXTEND_CHAR	0x1F
#define BIOS_DISKETTE_IO	0x40
#define EGA_FONT_INT		0x43	/* Pointer for the EGA graphics font */

/*
 * Private bootstrap functions
 */

#define BIOS_BOOTSTRAP_1	0x90
#define BIOS_BOOTSTRAP_2	0x91
#define BIOS_BOOTSTRAP_3	0x92

/*
 * Private diskette functions
 */

#define BIOS_FL_OPERATION_1	0xA0
#define BIOS_FL_OPERATION_2	0xA1
#define BIOS_FL_OPERATION_3	0xA2
#define BIOS_FL_RESET_2		0xA3

/*
 * Private hard disk function
 */

#define BIOS_HDA_COMMAND_CHECK	0xB0

/*
 * Mouse driver functions
 */

#define BIOS_MOUSE_INSTALL1	0xB8
#define BIOS_MOUSE_INSTALL2	0xB9
#define BIOS_MOUSE_INT1	0xBA
#define BIOS_MOUSE_INT2		0xBB
#define BIOS_MOUSE_IO_LANGUAGE	0xBC
#define BIOS_MOUSE_IO_INTERRUPT	0xBD
#define BIOS_MOUSE_VIDEO_IO	0xBE

/*
 * Get date function
 */

#define BIOS_GETDATE		0xBF

/*
 * Re-entrant CPU return function
 */

#define BIOS_CPU_RETURN		0xFE

/*
 * The following defines the structure of the Bios internal storage area.
 */

#define BIOS_VAR_SEGMENT	0x40
#define BIOS_VAR_START		((sys_addr)BIOS_VAR_SEGMENT * 16L)

/*
 * Address of memory size bios word variable.
 */

#define MEMORY_VAR            (BIOS_VAR_START + 0x13)

/*
 * The Bios FDC result data storage area
 */

#define BIOS_FDC_STATUS_BLOCK	BIOS_VAR_START + 0x42

/*
 * The BIOS hard disk data area. These variables are updated to their correct
 * values before every exit from disk_io.c/disk.c to the cpu.
*/

#define CMD_BLOCK	BIOS_VAR_START + 0x42

/*
 * The Bios Reset Flag
 */

#define RESET_FLAG	BIOS_VAR_START + 0x72

#define DISK_STATUS	BIOS_VAR_START + 0x74
#define HF_NUM		BIOS_VAR_START + 0x75
#define CONTROL_BYTE	BIOS_VAR_START + 0x76
#define PORT_OFF	BIOS_VAR_START + 0x77

/*
 * Bios Buffer space
 */

typedef union {
                 word wd;
                 struct {
                          half_word scan;
                          half_word ch;
                        } byte;
               } KEY_OCCURRENCE;

#define BIOS_KB_BUFFER_START    (BIOS_VAR_START + 0x80)
#define BIOS_KB_BUFFER_END      (BIOS_VAR_START + 0x82)
#define BIOS_KB_BUFFER_HEAD     (BIOS_VAR_START + 0x1a)
#define BIOS_KB_BUFFER_TAIL     (BIOS_VAR_START + 0x1c)
#define BIOS_KB_BUFFER          0x1e
#define BIOS_KB_BUFFER_SIZE     16

#define BIOS_VIRTUALISING_BYTE     (BIOS_VAR_START + 0xAC)

#define SOFT_FLAG	0x1234 		/* value indicating a soft reset */

/*
 * The number of diskettes supported by the Bios
 */

#define MAX_DISKETTES	4


#define	MODEL_BYTE		0xfc	/* system model byte		*/
#define SUB_MODEL_BYTE		02	/* system sub model type byte	*/
#define BIOS_LEVEL		0	/* BIOS revision level		*/

/*
 * ============================================================================
 * External declarations and macros
 * ============================================================================
 */

extern void (*BIOS[]) IPT0();

#ifndef bop
#define bop(n) (*BIOS[n])()
#endif /* bop */

#ifdef PTY
extern void com_bop_pty IPT0();
#endif

#ifdef GEN_DRVR
extern void gen_driver_io IPT0();
#endif

#ifdef LIM
extern void emm_init IPT0();
extern void emm_io IPT0();
extern void return_from_call IPT0();
#endif

#ifdef SUSPEND
extern void send_script IPT0();
#endif

#ifdef CDROM
extern void cdrom_ord_fns IPT0();
extern void cdrom_ext_fns IPT0();
extern void bcdrom_io IPT0();
#endif /* CDROM */

extern void worm_init IPT0();
extern void worm_io IPT0();

extern void cmd_install IPT0();
extern void cmd_load IPT0();

#ifdef DPMI
extern void DPMI_2F IPT0();
extern void DPMI_31 IPT0();
extern void DPMI_general IPT0();
extern void DPMI_int IPT0();
extern void DPMI_r0_int IPT0();
extern void DPMI_exc IPT0();
extern void DPMI_4B IPT0();
#endif

extern void reset IPT0();
extern void illegal_bop IPT0();
extern void illegal_op_int IPT0();
extern void illegal_dvr_bop IPT0();
extern void video_io IPT0();
extern void equipment IPT0();
extern void memory_size IPT0();
extern void diskette_io IPT0();
extern void diskette_int IPT0();
extern void disk_int IPT0();
extern void not_supported IPT0();
extern void rom_basic IPT0();
extern void keyboard_io IPT0();
extern void keyboard_int IPT0();
extern void printer_io IPT0();
extern void bootstrap IPT0();
extern void time_of_day IPT0();
extern void kb_idle_poll IPT0();
extern void time_int IPT0();
extern void disk_io IPT0();
extern void print_screen IPT0();
extern void rs232_io IPT0();
extern void dummy_int IPT0();
extern void unexpected_int IPT0();
extern void Get_build_id IPT0();

#ifdef WIN_VTD
extern void VtdTickSync IPT0();
#endif /* WIN_VTD */

#ifdef EGG
extern void ega_video_io IPT0();
#endif

extern void re_direct IPT0();
extern void D11_int IPT0();
extern void int_287 IPT0();

extern void mouse_install1 IPT0();
extern void mouse_install2 IPT0();
extern void mouse_int1 IPT0();
extern void mouse_int2 IPT0();
extern void mouse_io_interrupt IPT0();
extern void mouse_io_language IPT0();
extern void mouse_video_io IPT0();
extern void mouse_EM_callback IPT0();

extern void host_mouse_install1 IPT0();
extern void host_mouse_install2 IPT0();

extern void time_of_day_init IPT0();
extern void keyboard_init IPT0();

extern void cassette_io IPT0();

extern void fl_operation1 IPT0();
extern void fl_operation2 IPT0();
extern void fl_operation3 IPT0();
extern void fl_reset2 IPT0();
extern void fl_dummy IPT0();

extern void hda_command_check IPT0();	/* HD-dr */

extern void host_unsimulate IPT0();

extern void bootstrap1 IPT0();
extern void bootstrap2 IPT0();
extern void bootstrap3 IPT0();

#ifdef NOVELL
extern void DriverInitialize IPT0();
extern void DriverReadPacket IPT0();
extern void DriverSendPacket IPT0();
extern void DriverMulticastChange IPT0();
extern void DriverCloseSocket IPT0();
extern void DriverReset IPT0();
extern void DriverShutdown IPT0();
extern void DriverAddProtocol IPT0();
extern void DriverChangePromiscuous IPT0() ;
extern void DriverOpenSocket IPT0() ;
extern void DriverCloseSocket IPT0() ;
#ifdef NOVELL_CFM
extern void DriverCheckForMore IPT0() ;
#endif
#ifdef V4CLIENT
extern void DriverChangeIntStatus IPT0() ;
#endif	/* V4CLIENT */
#endif

#ifdef	MSWDVR
extern void ms_windows IPT0();
extern void msw_mouse IPT0();
extern void msw_copy IPT0();
extern void msw_copyInit IPT0();
extern void msw_keybd IPT0();
#endif

#ifdef	NOVELL_IPX
extern void IPXResInit  IPT0();
extern void IPXResEntry  IPT0();
extern void IPXResInterrupt  IPT0();
#endif

#ifdef	NOVELL_TCPIP
extern void TCPResInit  IPT0();
extern void TCPResEntry  IPT0();
#endif

#ifdef WINSOCK
extern void ISWSEntry IPT0();
#endif /* WINSOCK */

#ifdef SWINAPI
extern void User_call IPT0();
extern void Gdi_call IPT0();
extern void Swinapi_bop IPT0();
#endif /* SWINAPI */

#if	defined(SOFTWIN_API) || defined(SWIN_HFX)
extern void SoftWindowsInit IPT0();
extern void SoftWindowsTerm IPT0();
#endif	/* SOFTWIN_API or SWIN_HFX */

#if	defined(SOFTWIN_API)
extern void SoftWindowsApi IPT0();
#endif	/* SOFTWIN_API */

#ifdef SWIN_HAW
extern void msw_sound IPT0();
#endif	/* SWIN_HAW */

#ifdef ANSI
extern void host_reset(void);
extern void reboot(void);
extern void display_string(char *);
extern void clear_string(void);
#else
extern void host_reset IPT0();
extern void reboot IPT0();
extern void display_string IPT0();
extern void clear_string IPT0();
#endif /* ANSI */

#ifdef ANSI
extern void smeg_collect_data(void);
extern void smeg_freeze_data(void);
#else
extern void smeg_collect_data IPT0();
extern void smeg_freeze_data IPT0();
#endif /* ANSI */

extern void softpc_version IPT0();


#ifdef GISP_CPU
extern void hg_bop_handler IPT0();
#endif	/* GISP_CPU */

#ifdef HFX
extern void test_for_us IPT0();
#endif /* HFX */

#ifdef DOS_APP_LIC
extern void DOS_AppLicense IPT0();
#endif

IMPORT int soft_reset;

#if	defined(SPC386) && defined(WDCTRL_BOP)
extern void wdctrl_bop IPT0();
#endif	/* SPC386 && WDCTRL_BOP */

