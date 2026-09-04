#ifndef SOFTPC_VM_WIN32_WINDOW_H
#define SOFTPC_VM_WIN32_WINDOW_H

#include "runtime.h"
#include "console.h"

int app_vm_run_window(app_runtime *runtime);
/* Console routing variant: return to the console if the original guest changes
 * from graphics back to text. Fixed `display=window' never performs this
 * transfer. */
int app_vm_run_console_window(app_runtime *runtime);

#endif
