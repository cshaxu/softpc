#ifndef VM_APP_ENTRY_INTERFACE_H
#define VM_APP_ENTRY_INTERFACE_H

#include "ibmpc/product/composition_interface.h"
#include "ibmpc/product/command_interface.h"

/* App has already interpreted its configuration. Product sees only the two
 * choices required to compose Common Session/UI; this is not a Common UI
 * instance and Product remains its sole creator. */
typedef struct app_composed_ui {
    common_session_display display;
    lib_bool console_control;
} app_composed_ui;

typedef lib_status (*vm_app_extensions_configure)(vm_app *app,
    app_command_extensions *out_extensions);

/* Both App-composed values are transferred to Product. It does not receive an
 * App configuration object, loader callback, or machine-construction API. */
typedef struct vm_app_definition {
    const char *name;
    app_composed_machine machine;
    app_composed_ui ui;
    vm_app_extensions_configure configure_extensions;
} vm_app_definition;

/* Sole PC process body. Returns the existing process success/failure code. */
lib_i32 vm_app_run(const vm_app_definition *definition);

#endif
