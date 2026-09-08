#ifndef LIB_HOST_CONSOLE_H
#define LIB_HOST_CONSOLE_H

#include "lib/base/console.h"

typedef struct host_console_broker host_console_broker;
typedef struct host_console_cooked host_console_cooked;

typedef void (*host_console_cooked_line_sink)(void *context,
    const lib_console_line *line);

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

/* A product shell owns command meaning, while host owns the cooked logical
 * Console and its Current-Console registration. */
lib_status host_console_cooked_create(host_console_cooked **out_cooked,
    host_console_cooked_line_sink line_sink, void *line_context);
void host_console_cooked_destroy(host_console_cooked *cooked);
lib_status host_console_cooked_activate_raw(host_console_cooked *cooked,
    lib_console *raw_console);
lib_status host_console_cooked_activate_self(host_console_cooked *cooked,
    lib_console *raw_console);
/* Idempotently arms one native cooked line while this is Current Console.
 * It does nothing when a line is already pending. */
lib_status host_console_cooked_request_line(host_console_cooked *cooked);
lib_status host_console_cooked_write(host_console_cooked *cooked,
    const char *text, lib_size length);

#endif
