#include "monitor.h"

#ifdef _WIN32
#include "lib/ux-console/console.h"
#include <windows.h>

#include <stdlib.h>
#include <string.h>

struct app_monitor_console {
    host_console_cooked *cooked;
    app_control_queue *control_queue;
};

static void app_monitor_receive(void *opaque, const lib_console_line *line)
{
    app_monitor_console *monitor = (app_monitor_console *)opaque;
    if (monitor == NULL || line == NULL) return;
    (void)app_control_queue_push_monitor_line(monitor->control_queue,
        line);
}

int app_monitor_console_create(app_monitor_console **out_monitor,
    app_control_queue *control_queue)
{
    app_monitor_console *monitor;
    if (out_monitor == NULL || control_queue == NULL) return 0;
    *out_monitor = NULL;
    monitor = calloc(1u, sizeof(*monitor));
    if (monitor == NULL) return 0;
    monitor->control_queue = control_queue;
    if (host_console_cooked_create(&monitor->cooked, app_monitor_receive,
            monitor) != LIB_STATUS_OK) {
        app_monitor_console_destroy(monitor);
        return 0;
    }
    *out_monitor = monitor;
    return 1;
}

void app_monitor_console_destroy(app_monitor_console *monitor)
{
    if (monitor == NULL) return;
    host_console_cooked_destroy(monitor->cooked);
    free(monitor);
}

int app_monitor_console_activate_vm(app_monitor_console *monitor,
    ux_console *console)
{
    return monitor != NULL && console != NULL &&
        host_console_cooked_activate_raw(monitor->cooked,
            ux_console_get_console(console)) == LIB_STATUS_OK;
}

int app_monitor_console_activate_self(app_monitor_console *monitor,
    ux_console *console)
{
    return monitor != NULL && console != NULL &&
        host_console_cooked_activate_self(monitor->cooked,
            ux_console_get_console(console)) == LIB_STATUS_OK;
}

int app_monitor_console_write(app_monitor_console *monitor, const char *text)
{
    if (monitor == NULL || text == NULL) return 0;
    return host_console_cooked_write(monitor->cooked, text, strlen(text)) ==
        LIB_STATUS_OK;
}
#endif
