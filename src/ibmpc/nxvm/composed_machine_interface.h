#ifndef IBMPC_NXVM_COMPOSED_MACHINE_INTERFACE_H
#define IBMPC_NXVM_COMPOSED_MACHINE_INTERFACE_H

#include "ibmpc/product/machine_interface.h"
#include "ibmpc/product/command_interface.h"
#include "ibmpc/product/composition_interface.h"
#include "ibmpc/nxvm/startup_config_interface.h"
#include "ibmpc/machine/input_interface.h"

/* Fixed composition values and assets outlive Product. NXVM App code
 * interprets its private startup configuration before handing Product a fully
 * composed machine. */
typedef struct vm_app_machine_binding {
    const char *name;
    core_machine_cpu_profile cpu;
    x86_fpu_profile fpu;
    vm_machine_floppy_format floppy_format;
    lib_size bios_count;
    const vm_machine_assets *firmware;
    lib_status (*prepare)(const vm_machine_config *config,
        const vm_machine_assets *assets, vm_machine_construction *out_construction);
} vm_app_machine_binding;

typedef struct vm_app_machine_composition {
    const vm_app_machine_binding *binding;
    const nxvm_startup_config *startup;
} vm_app_machine_composition;

lib_status vm_app_build_machine_config(const vm_app_machine_binding *binding,
    const nxvm_startup_config *startup, vm_machine_config *out_config);
lib_status vm_app_nxvm_compose_machine(
    const vm_app_machine_composition *composition,
    app_composed_machine *out_machine);

/* NXVM-family commands are registered by each App; shared Product dispatch
 * has no INFO, SPEED or floppy cases. */
lib_status vm_app_configure_standard_extensions(vm_app *app,
    app_command_extensions *out_extensions);

#endif
