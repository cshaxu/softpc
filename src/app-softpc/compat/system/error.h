/*
 * The original illegal-instruction module expects the base error-code enum.
 * Its historic build searched base/inc before the NT host headers; the
 * standalone target otherwise finds host/inc/error.h first.  This include
 * overlay restores only that original declaration lookup, with no error
 * policy or machine behavior of its own.
 */
/* insignia.h has already selected host/inc/error.h, whose historic include
 * guard is shared with base/inc/error.h.  This translation unit needs the
 * original Base enum, so deliberately replace that earlier header here. */
#undef _INS_ERROR_H
#include "../../softpc.new/base/inc/error.h"
