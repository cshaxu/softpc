/*
 * VPC-XT Revision 1.0
 *
 * Title        : Sparc 2.0 Definitions for the CPU
 *
 * Description  : Structures, macros and definitions for access to the
 *                CPU registers
 *
 * Author       : Andrew Guthrie
 *
 * Notes        : This file is included by cpu.h and should NOT
 *                be included directly by any other module.
 */

/* SccsID[]="@(#)host_cpu.h     1.22 4/15/91 Copyright Insignia Solutions Ltd."; */

#ifdef CPU_30_STYLE
#error "Unsupported SoftPC mirror configuration: CPU_30_STYLE"
#else /* CPU_30_STYLE */
#line 113

/*
 * ============================================================================
 * Structure/Data definitions
 * ============================================================================
 */

/*
 * Union representing different ways of accessing a register
 */

typedef union
    {
        word    X;
    struct
        {                               /* as two bytes */
#ifdef LITTLEND
        half_word low;
        half_word high;
#else
#error "Unsupported SoftPC mirror configuration: !LITTLEND"
#endif
#line 136
        word pad;
        } byte;
    struct
        {                               /* as 4 nibbles */
        word n3:4;
        word n2:4;
        word n1:4;
        word n0:4;
        word pad;
        } nibble;
    struct
        {                               /* as 16 bits   */
        word b15:1;
        word b14:1;
        word b13:1;
        word b12:1;
        word b11:1;
        word b10:1;
        word b9:1;
        word b8:1;
        word b7:1;
        word b6:1;
        word b5:1;
        word b4:1;
        word b3:1;
        word b2:1;
        word b1:1;
        word b0:1;
        word pad;
        } bit;
    } reg;

#ifdef A2CPU
#error "Unsupported SoftPC mirror configuration: A2CPU"
#endif A2CPU
#line 184

/*
 * ============================================================================
 * External declarations and macros
 * ============================================================================
 */
extern word m_s_w;

#ifndef EGATEST
#ifdef A2CPU
#error "Unsupported SoftPC mirror configuration: A2CPU"
#endif A2CPU
#line 199


/*  common READ functions  */
#define  getAX()        ((*getAX_func) ())
#define  getAH()        ((*getAH_func) ())
#define  getAL()        ((*getAL_func) ())
#define  getBX()        ((*getBX_func) ())
#define  getBH()        ((*getBH_func) ())
#define  getBL()        ((*getBL_func) ())
#define  getCX()        ((*getCX_func) ())
#define  getCH()        ((*getCH_func) ())
#define  getCL()        ((*getCL_func) ())
#define  getDX()        ((*getDX_func) ())
#define  getDH()        ((*getDH_func) ())
#define  getDL()        ((*getDL_func) ())
#define  getSP()        ((*getSP_func) ())
#define  getBP()        ((*getBP_func) ())
#define  getSI()        ((*getSI_func) ())
#define  getDI()        ((*getDI_func) ())
#define  getIP()        ((*getIP_func) ())
#define  getCS()        ((*getCS_func) ())
#define  getDS()        ((*getDS_func) ())
#define  getES()        ((*getES_func) ())
#define  getSS()        ((*getSS_func) ())
#define  getMSW()       ((*getMSW_func) ())
#define  getDF()        ((*getDF_func) ())
#define  getIF()        ((*getIF_func) ())
#define  getTF()        ((*getTF_func) ())
#define  getPF()        ((*getPF_func) ())
#define  getAF()        ((*getAF_func) ())
#define  getSF()        ((*getSF_func) ())
#define  getZF()        ((*getZF_func) ())
#define  getOF()        ((*getOF_func) ())
#define  getCF()        ((*getCF_func) ())

#ifdef CCPU
/*  CCPU-specific READ functions */
#define getCPL()                ((*getCPL_func) ())
#define getGDTR_base()          ((*getGDTR_base_func) ())
#define getGDTR_limit()         ((*getGDTR_limit_func) ())
#define getIDTR_base()          ((*getIDTR_base_func) ())
#define getIDTR_limit()         ((*getIDTR_limit_func) ())
#define getLDTR()               ((*getLDTR_func) ())
#define getTR()                 ((*getTR_func) ())
#define getMSW_reserved()       ((*getMSW_reserved_func) ())
#define getTS()         ((*getTS_func) ())
#define getEM()         ((*getEM_func) ())
#define getMP()         ((*getMP_func) ())
#define getPE()         ((*getPE_func) ())
#define getNT()         ((*getNT_func) ())
#define getIOPL()       ((*getIOPL_func) ())
#define  getSTATUS()    ((*getSTATUS_func) ())
#endif CCPU

#ifdef A2CPU
#error "Unsupported SoftPC mirror configuration: A2CPU"
#endif A2CPU
#line 261

/*  common WRITE functions  */
#define  setAX(val)     ((*setAX_func) (val))
#define  setAH(val)     ((*setAH_func) (val))
#define  setAL(val)     ((*setAL_func) (val))
#define  setBX(val)     ((*setBX_func) (val))
#define  setBH(val)     ((*setBH_func) (val))
#define  setBL(val)     ((*setBL_func) (val))
#define  setCX(val)     ((*setCX_func) (val))
#define  setCH(val)     ((*setCH_func) (val))
#define  setCL(val)     ((*setCL_func) (val))
#define  setDX(val)     ((*setDX_func) (val))
#define  setDH(val)     ((*setDH_func) (val))
#define  setDL(val)     ((*setDL_func) (val))
#define  setSP(val)     ((*setSP_func) (val))
#define  setBP(val)     ((*setBP_func) (val))
#define  setSI(val)     ((*setSI_func) (val))
#define  setDI(val)     ((*setDI_func) (val))
#define  setIP(val)     ((*setIP_func) (val))
#define  setCS(val)     ((*setCS_func) (val))
#define  setDS(val)     ((*setDS_func) (val))
#define  setES(val)     ((*setES_func) (val))
#define  setSS(val)     ((*setSS_func) (val))
#define  setMSW(val)    ((*setMSW_func) (val))
#define  setDF(val)     ((*setDF_func) (val))
#define  setIF(val)     ((*setIF_func) (val))
#define  setTF(val)     ((*setTF_func) (val))
#define  setPF(val)     ((*setPF_func) (val))
#define  setAF(val)     ((*setAF_func) (val))
#define  setSF(val)     ((*setSF_func) (val))
#define  setZF(val)     ((*setZF_func) (val))
#define  setOF(val)     ((*setOF_func) (val))
#define  setCF(val)     ((*setCF_func) (val))

#ifdef CCPU
/*  CCPU-specific WRITE functions */
#define setCPL(val)             ((*setCPL_func) (val))
#define setGDTR_base(val)       ((*setGDTR_base_func) (val))
#define setGDTR_limit(val)      ((*setGDTR_limit_func) (val))
#define setIDTR_base(val)       ((*setIDTR_base_func) (val))
#define setIDTR_limit(val)      ((*setIDTR_limit_func) (val))
#define setLDTR(val)            ((*setLDTR_func) (val))
#define setTR(val)              ((*setTR_func) (val))
#define setMSW_reserved(val)    ((*setMSW_reserved_func) (val))
#define setTS(val)              ((*setTS_func) (val))
#define setEM(val)              ((*setEM_func) (val))
#define setMP(val)              ((*setMP_func) (val))
#define setPE(val)              ((*setPE_func) (val))
#define setNT(val)              ((*setNT_func) (val))
#define setIOPL(val)    ((*setIOPL_func) (val))
#endif CCPU

#ifdef A2CPU
#error "Unsupported SoftPC mirror configuration: A2CPU"
#endif A2CPU
#line 320
#endif EGATEST



/*  HOST_SIMULATE function  */
#define  host_simulate()        ((*host_simulate_func) ())

/*  common access functions. Load at boot and cpu switch times. */
/*  common READ functions  */
extern word             (*getAX_func) ();
extern half_word        (*getAH_func) ();
extern half_word        (*getAL_func) ();
extern word             (*getBX_func) ();
extern half_word        (*getBH_func) ();
extern half_word        (*getBL_func) ();
extern word             (*getCX_func) ();
extern half_word        (*getCH_func) ();
extern half_word        (*getCL_func) ();
extern word             (*getDX_func) ();
extern half_word        (*getDH_func) ();
extern half_word        (*getDL_func) ();
extern word             (*getSP_func) ();
extern word             (*getBP_func) ();
extern word             (*getSI_func) ();
extern word             (*getDI_func) ();
extern word             (*getIP_func) ();
extern word             (*getCS_func) ();
extern word             (*getDS_func) ();
extern word             (*getES_func) ();
extern word             (*getSS_func) ();
extern word             (*getMSW_func) ();
extern word             (*getDF_func) ();
extern word             (*getIF_func) ();
extern word             (*getTF_func) ();
extern word             (*getPF_func) ();
extern word             (*getAF_func) ();
extern word             (*getSF_func) ();
extern word             (*getZF_func) ();
extern word             (*getOF_func) ();
extern word             (*getCF_func) ();
extern word             (*getSTATUS_func) ();

#ifdef CCPU
/* CCPU-specific READ functions */
extern int              (*getCPL_func) ();
extern sys_addr (*getGDTR_base_func) ();
extern word             (*getGDTR_limit_func) ();
extern sys_addr (*getIDTR_base_func) ();
extern word             (*getIDTR_limit_func) ();
extern word             (*getLDTR_func) ();
extern word             (*getTR_func) ();
extern word             (*getMSW_reserved_func) ();
extern word             (*getTS_func) ();
extern word             (*getEM_func) ();
extern word             (*getMP_func) ();
extern word             (*getPE_func) ();
extern word             (*getNT_func) ();
extern word             (*getIOPL_func) ();
#endif CCPU

#ifdef A2CPU
#error "Unsupported SoftPC mirror configuration: A2CPU"
#endif A2CPU
#line 388



/*  common WRITE functions  */
/* DIVERGENCE: MVDM-HOST-DIV-102 keeps original common vector names and
 * selection macros, but exposes the selected generated CCPU contracts.
 * Empty NT4 parameter lists do not prove the x86/x64 call ABI. */
extern VOID (*setAX_func)(IU16); extern VOID (*setAH_func)(IU8); extern VOID (*setAL_func)(IU8);
extern VOID (*setBX_func)(IU16); extern VOID (*setBH_func)(IU8); extern VOID (*setBL_func)(IU8);
extern VOID (*setCX_func)(IU16); extern VOID (*setCH_func)(IU8); extern VOID (*setCL_func)(IU8);
extern VOID (*setDX_func)(IU16); extern VOID (*setDH_func)(IU8); extern VOID (*setDL_func)(IU8);
extern VOID (*setSP_func)(IU16); extern VOID (*setBP_func)(IU16); extern VOID (*setSI_func)(IU16);
extern VOID (*setDI_func)(IU16); extern VOID (*setIP_func)(IU16);
extern ISM32 (*setCS_func)(IU16); extern ISM32 (*setDS_func)(IU16);
extern ISM32 (*setES_func)(IU16); extern ISM32 (*setSS_func)(IU16);
extern VOID (*setMSW_func)(IU16);
extern VOID (*setDF_func)(IBOOL); extern VOID (*setIF_func)(IBOOL); extern VOID (*setTF_func)(IBOOL);
extern VOID (*setPF_func)(IBOOL); extern VOID (*setAF_func)(IBOOL); extern VOID (*setSF_func)(IBOOL);
extern VOID (*setZF_func)(IBOOL); extern VOID (*setOF_func)(IBOOL); extern VOID (*setCF_func)(IBOOL);
#line 419
#ifdef CCPU
/* CCPU-specific WRITE functions */
extern void             (*setCPL_func) ();
extern void             (*setGDTR_base_func) ();
extern void             (*setGDTR_limit_func) ();
extern void             (*setIDTR_base_func) ();
extern void             (*setIDTR_limit_func) ();
extern void             (*setLDTR_func) ();
extern void             (*setTR_func) ();
extern void             (*setMSW_reserved_func) ();
extern void             (*setTS_func) ();
extern void             (*setEM_func) ();
extern void             (*setMP_func) ();
extern void             (*setPE_func) ();
extern void             (*setNT_func) ();
extern void             (*setIOPL_func) ();
#endif CCPU

#ifdef A2CPU
#error "Unsupported SoftPC mirror configuration: A2CPU"
#endif A2CPU
#line 467


/*  HOST_SIMULATE function  */
extern void             (*host_simulate_func) ();

/*
 *
 *******************************************************************
 * The Second Assembler cpu register access functions.             *
 *******************************************************************
 *
 */
#ifdef A2CPU
#error "Unsupported SoftPC mirror configuration: A2CPU"
#endif A2CPU
#line 528

#endif /* CPU_30_STYLE */
