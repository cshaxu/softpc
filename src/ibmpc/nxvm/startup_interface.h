#ifndef VM_APP_STARTUP_INTERFACE_H
#define VM_APP_STARTUP_INTERFACE_H
#include "lib/types/types_interface.h"
#include "ibmpc/product/entry_interface.h"
#include "ibmpc/nxvm/startup_config_interface.h"


/* Compose the App-selected configuration name beside its executable. */
lib_status vm_app_ini_executable_path(const char *name, lib_u8 *path,
    lib_size capacity);

/* NXVM App maps its already-read private configuration to the two Product UI
 * inputs. Product never reads this configuration or this INI helper. */
lib_status vm_app_nxvm_compose_ui(const nxvm_startup_config *config,
    app_composed_ui *out_ui);

#endif
