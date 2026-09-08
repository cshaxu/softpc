#ifndef APP_PRESENTATION_H
#define APP_PRESENTATION_H

#include "runtime.h"

#ifdef _WIN32
#define SOFTPC_VM_FRONTEND_STOPPED 0
#define SOFTPC_VM_FRONTEND_ERROR 1
#define SOFTPC_VM_FRONTEND_PAUSED 2

/* Application-owned component supervisor.  It chooses component existence
 * and maps generic UX events to SoftPC actions; neither shared UX leaf has
 * product lifecycle knowledge. */
int app_presentation_run(app_runtime *runtime,
    softpc_presentation presentation);
#endif

#endif
