/*[
 *   Name:		host_npx.h
 *
 *   SCCS ID:		@(#)cfpu_def.h	1.13 10/13/94
 *
 *   Purpose:		More dodgy, non-portable structures!
 *
 * (C) Copyright Insignia Solutions, 1994.
]*/

typedef double 		FPH;

#define FPHOST FP64
#define HOST_MAX_EXP 2047
#if !defined(HOST_BIAS)
#define HOST_BIAS 1023
#endif
/* T59 host ABI: word and bitfield order must match the selected host byte
 * order. The CCPU arithmetic and Intel-memory conversion remain unchanged. */
typedef struct {
#ifdef LITTLEND
IU32	mant:23;
IU32	exp:8;
IU32	sign:1;
#else
#error "Unsupported SoftPC mirror configuration: !LITTLEND"
#endif
#line 30
} FP32;

typedef struct {
#ifdef LITTLEND
IU32	mant_hi:20;
IU32	exp:11;
IU32	sign:1;
#else
#error "Unsupported SoftPC mirror configuration: !LITTLEND"
#endif
#line 42
} FP64HI;

typedef struct {
#ifdef LITTLEND
IU16	exp:15;
IU16	sign:1;
#else
#error "Unsupported SoftPC mirror configuration: !LITTLEND"
#endif
#line 52
} FP80SE;

//#ifdef BIGEND

typedef struct {
#ifdef LITTLEND
IU32	mant_lo;
FP64HI	hiword;
#else
#error "Unsupported SoftPC mirror configuration: !LITTLEND"
#endif
#line 64
} FP64;


typedef struct {
#ifdef LITTLEND
/* fpu.c's little-endian BCD tables initialize low word before high word. */
IU32 low_word;
IS32 high_word;
#else
#error "Unsupported SoftPC mirror configuration: !LITTLEND"
#endif
#line 76
} FPU_I64;

/*
 * The format of an FP80 has to be 
 * struct { IU64 mant; IU16 sign_exp }
 * to match the Acpu.
 */

typedef struct {
#ifdef LITTLEND
IU32	mant_lo;
IU32	mant_hi;
#else
#error "Unsupported SoftPC mirror configuration: !LITTLEND"
#endif
#line 57
FP80SE	sign_exp;
} FP80;

//#endif

/* HOST_ values are based on a byte ordering where 0 represents the first
 * byte.  They are used to copy n-byte values represented in bigendian-order
 * to a host memory location in host-order. The definition is therefore
 * determined by whether the host is bigendian or littleendian.
 */

#ifdef BIGEND
#error "Unsupported SoftPC mirror configuration: BIGEND"
#endif	/* BIGEND */
#line 96

#ifdef LITTLEND
/* Format for double precision on littleendian host */
#define HOST_R64_BYTE_0 7
#define HOST_R64_BYTE_1 6
#define HOST_R64_BYTE_2 5
#define HOST_R64_BYTE_3 4
#define HOST_R64_BYTE_4 3
#define HOST_R64_BYTE_5 2
#define HOST_R64_BYTE_6 1
#define HOST_R64_BYTE_7 0

/* Format for internal representation of an R80 on littleendian host */
#define HOST_R80_BYTE_0 9
#define HOST_R80_BYTE_1 8
#define HOST_R80_BYTE_2 7
#define HOST_R80_BYTE_3 6
#define HOST_R80_BYTE_4 5
#define HOST_R80_BYTE_5 4
#define HOST_R80_BYTE_6 3
#define HOST_R80_BYTE_7 2
#define HOST_R80_BYTE_8 1
#define HOST_R80_BYTE_9 0

#define NPX_LOW_32_BITS  0
#define NPX_HIGH_32_BITS 1
#endif /* LITTLEND */


#ifdef NTVDM
#error "Unsupported SoftPC mirror configuration: NTVDM"
#else	/* !NTVDM */
#line 187

#define HostGetOverflowException()      (fpgetsticky() & FP_X_OFL)
#define HostGetUnderflowException()     (fpgetsticky() & FP_X_UFL)
#define HostGetPrecisionException()     (fpgetsticky() & FP_X_IMP)

#define HostClearExceptions()           ((VOID)fpsetsticky(FP_X_CLEAR))

#define HostSetRoundToNearest()         ((VOID)fpsetround(FP_RN))
#define HostSetRoundDown()              ((VOID)fpsetround(FP_RM))
#define HostSetRoundUp()                ((VOID)fpsetround(FP_RP))
#define HostSetRoundToZero()            ((VOID)fpsetround(FP_RZ))

#endif	/* !NTVDM */

#ifndef _Fpu_c_h

/* These definitions are for the Ccpu NPX only: in circumstances
 * where both the C NPX and the Assembler NPX are accessed, these
 * definitions should be excluded in favour of the generated ones in
 * Fpu_c.h, hence the #ifndef above.
 */

typedef struct {
FPH fpvalue;
IUH padding;    /* To allow at least 10 bytes for unconverted formats */
IUH tagvalue;
} FPSTACKENTRY;

#endif
