#ifndef APP_MONITOR_H
#define APP_MONITOR_H

#include "lib/host/console.h"
#include "control.h"

#include <stddef.h>

/* SoftPC's cooked monitor is an application object.  Host binds it to the
 * one process Console; it never parses a line or calls lifecycle APIs. */
typedef struct app_monitor_console app_monitor_console;

int app_monitor_console_create(app_monitor_console **out_monitor,
    app_control_queue *control_queue);
void app_monitor_console_destroy(app_monitor_console *monitor);
struct ux_console;
int app_monitor_console_activate_vm(app_monitor_console *monitor,
    struct ux_console *console);
int app_monitor_console_activate_self(app_monitor_console *monitor,
    struct ux_console *console);
int app_monitor_console_write(app_monitor_console *monitor, const char *text);

#endif
