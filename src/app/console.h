#ifndef SOFTPC_VM_CONSOLE_H
#define SOFTPC_VM_CONSOLE_H

#include "runtime.h"

#define SOFTPC_VM_FRONTEND_STOPPED 0
#define SOFTPC_VM_FRONTEND_ERROR 1
#define SOFTPC_VM_FRONTEND_PAUSED 2
/* Presentation routing outcomes.  They deliberately do not stop/pause the
 * runtime: auto mode transfers the same running machine to another shell. */
#define SOFTPC_VM_FRONTEND_SWITCH_WINDOW 3
#define SOFTPC_VM_FRONTEND_SWITCH_CONSOLE 4

int app_vm_run_console(app_runtime *runtime);

#endif
