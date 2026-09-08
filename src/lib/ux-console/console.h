#ifndef UX_CONSOLE_H
#define UX_CONSOLE_H

#include "lib/base/console.h"
#include "lib/ux-base/component.h"

typedef struct ux_console ux_console;

typedef ux_component_options ux_console_options;

lib_status ux_console_create(ux_console **out_console,
    const ux_console_options *options);
ux_component *ux_console_component(ux_console *console);
/* Borrowed logical Console object. SoftPC passes it to host for Current
 * Console registration before it permits raw VM input. */
lib_console *ux_console_get_console(const ux_console *console);

#endif
