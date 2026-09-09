#ifndef LIB_HOST_INTERNAL_CONSOLE_H
#define LIB_HOST_INTERNAL_CONSOLE_H

/* Current-Console transaction machinery belongs solely to host.  Product
 * bindings use host_console_cooked_* from lib/host/console.h instead. */
#include "lib/host/console.h"

typedef struct host_console_broker host_console_broker;

lib_status host_console_broker_create(host_console_broker **out_broker,
    lib_console *initial_console, host_console_mode initial_mode);
lib_status host_console_replace_active(host_console_broker *broker,
    lib_console *old_console, lib_console *next_console,
    host_console_mode next_mode);
/* Foregrounding is an explicit product request, independent of binding a
 * Current Console for native I/O.  It is deliberately best effort. */
void host_console_broker_request_focus(host_console_broker *broker);
void host_console_broker_destroy(host_console_broker *broker);

#endif
