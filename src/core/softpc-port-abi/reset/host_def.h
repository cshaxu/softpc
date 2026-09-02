#ifndef SOFTPC_RESET_HOST_DEF_H
#define SOFTPC_RESET_HOST_DEF_H

/* Keep original reset.c pristine: standalone excludes only the absent LIM
   product layer after importing the selected original host definition. */
#include "../../softpc/host/inc/host_def.h"
#undef LIM

#endif
