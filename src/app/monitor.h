#ifndef APP_MONITOR_H
#define APP_MONITOR_H

#include "lib/base/console.h"
#include "lib/host/console.h"

#include <stddef.h>

/* SoftPC's cooked monitor is an application object.  Host binds it to the
 * one process Console; it never parses a line or calls lifecycle APIs. */
typedef struct app_monitor_console app_monitor_console;

int app_monitor_console_create(app_monitor_console **out_monitor);
void app_monitor_console_destroy(app_monitor_console *monitor);
lib_console *app_monitor_console_object(app_monitor_console *monitor);
host_console_broker *app_monitor_console_broker(app_monitor_console *monitor);
int app_monitor_console_take_line(app_monitor_console *monitor, char *text,
    size_t capacity, unsigned long timeout_ms);
int app_monitor_console_write(app_monitor_console *monitor, const char *text);

#endif
