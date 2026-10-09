#ifndef VM_APP_INI_INTERFACE_H
#define VM_APP_INI_INTERFACE_H
#include "lib/types/types_interface.h"

#include "ibmpc/nxvm/startup_config_interface.h"

/* Parse one code-owned document or load one NXVM.ini file.  Paths are resolved
 * only against the INI directory; firmware is intentionally absent. */
lib_status vm_app_ini_parse(const lib_u8 *directory, const lib_u8 *name,
    lib_u8 *document, nxvm_startup_config *out_config);
lib_status vm_app_ini_load(const lib_u8 *path, nxvm_startup_config *out_config);

#endif
