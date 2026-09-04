#ifndef SOFTPC_VM_WIN32_WINDOW_H
#define SOFTPC_VM_WIN32_WINDOW_H

#include "../../runtime/runtime.h"
#include "../console/console.h"

int softpc_vm_run_window(softpc_runtime *runtime);
/* Console routing variant: return to the console if the original guest changes
 * from graphics back to text. Fixed `display=window' never performs this
 * transfer. */
int softpc_vm_run_console_window(softpc_runtime *runtime);

#endif
