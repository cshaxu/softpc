#ifndef LIB_HOST_CONSOLE_H
#define LIB_HOST_CONSOLE_H

#include "lib/base/console.h"

typedef struct host_console_broker host_console_broker;

typedef enum host_console_mode {
    HOST_CONSOLE_RAW_EVENTS,
    HOST_CONSOLE_COOKED_LINES
} host_console_mode;

/* Creates the process-local broker with an already-created initial object.
 * There is no unregister operation: replacement preserves one current object;
 * destruction is the sole normal zero-object transition. */
lib_status host_console_broker_create(host_console_broker **out_broker,
    lib_console *initial_console, host_console_mode initial_mode);
lib_status host_console_replace_active(host_console_broker *broker,
    lib_console *old_console, lib_console *next_console,
    host_console_mode next_mode);
void host_console_broker_destroy(host_console_broker *broker);

#endif
