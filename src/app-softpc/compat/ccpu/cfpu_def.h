#ifndef SOFTPC_COMPAT_CFPU_DEF_H
#define SOFTPC_COMPAT_CFPU_DEF_H

#include "../../softpc.new/host/inc/cfpu_def.h"
#include <fenv.h>

/* Keep original FPU types; Compat owns the host floating-point environment. */
#undef HostGetOverflowException
#undef HostGetUnderflowException
#undef HostGetPrecisionException
#undef HostClearExceptions
#undef HostSetRoundToNearest
#undef HostSetRoundDown
#undef HostSetRoundUp
#undef HostSetRoundToZero

#define HostGetOverflowException()      (fetestexcept(FE_OVERFLOW))
#define HostGetUnderflowException()     (fetestexcept(FE_UNDERFLOW))
#define HostGetPrecisionException()     (fetestexcept(FE_INEXACT))
#define HostClearExceptions()           ((VOID)feclearexcept(FE_ALL_EXCEPT))
#define HostSetRoundToNearest()         ((VOID)fesetround(FE_TONEAREST))
#define HostSetRoundDown()              ((VOID)fesetround(FE_DOWNWARD))
#define HostSetRoundUp()                ((VOID)fesetround(FE_UPWARD))
#define HostSetRoundToZero()            ((VOID)fesetround(FE_TOWARDZERO))

#endif
