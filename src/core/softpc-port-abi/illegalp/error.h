/*
 * The original illegal-instruction module expects the base error-code enum.
 * Its historic build searched base/inc before the NT host headers; the
 * standalone target otherwise finds host/inc/error.h first.  This include
 * overlay restores only that original declaration lookup, with no error
 * policy or machine behavior of its own.
 */
#include "../../mvdm/softpc.new/base/inc/error.h"
