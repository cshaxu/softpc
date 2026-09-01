#ifndef SOFTPC_VM_CONSOLE_H
#define SOFTPC_VM_CONSOLE_H

#include "runtime.h"

#define SOFTPC_VM_FRONTEND_STOPPED 0
#define SOFTPC_VM_FRONTEND_ERROR 1
#define SOFTPC_VM_FRONTEND_PAUSED 2

int softpc_vm_run_console(softpc_runtime *runtime);

#endif
