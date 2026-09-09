#ifndef LIB_HOST_CONSOLE_H
#define LIB_HOST_CONSOLE_H

#include "lib/base/console.h"

typedef struct host_console_broker host_console_broker;
/* Temporary source-compatibility wrapper. S4 removes this after SoftPC uses
 * the broker API below directly; do not add a new user. */
typedef struct host_console_cooked host_console_cooked;
typedef void (*host_console_cooked_line_sink)(void *context,
    const lib_console_line *line);

typedef enum host_console_mode {
    HOST_CONSOLE_RAW_EVENTS,
    HOST_CONSOLE_COOKED_LINES
} host_console_mode;

/* The broker owns one process-native Console I/O path. The caller owns every
 * logical Console and remains the product-state owner. */
lib_status host_console_broker_create(host_console_broker **out_broker,
    lib_console *initial_console, host_console_mode initial_mode);
/* Replaces Current Console only when expected_current is still current. This
 * is a transaction assertion, not a host-side product-state query. */
lib_status host_console_broker_replace(host_console_broker *broker,
    lib_console *expected_current, lib_console *next_console,
    host_console_mode next_mode);
/* Idempotently arms one cooked line only while expected_current remains the
 * Current Console in cooked mode. */
lib_status host_console_broker_request_cooked_line(host_console_broker *broker,
    lib_console *expected_current);
void host_console_broker_destroy(host_console_broker *broker);

/* Temporary compatibility surface; its only remaining caller is SoftPC's
 * monitor during T47 S3. */
lib_status host_console_cooked_create(host_console_cooked **out_cooked,
    host_console_cooked_line_sink line_sink, void *line_context);
void host_console_cooked_destroy(host_console_cooked *cooked);
lib_status host_console_cooked_activate_raw(host_console_cooked *cooked,
    lib_console *raw_console);
lib_status host_console_cooked_activate_self(host_console_cooked *cooked,
    lib_console *raw_console);
lib_status host_console_cooked_request_line(host_console_cooked *cooked);
lib_status host_console_cooked_write(host_console_cooked *cooked,
    const char *text, lib_size length);

#endif
