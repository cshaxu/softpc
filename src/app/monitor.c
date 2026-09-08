#include "monitor.h"

#ifdef _WIN32
#include <windows.h>

#include <stdlib.h>
#include <string.h>

struct app_monitor_console {
    lib_console *console;
    host_console_broker *broker;
    app_control_queue *control_queue;
};

static void app_monitor_receive(void *opaque, const lib_console_event *event)
{
    app_monitor_console *monitor = (app_monitor_console *)opaque;
    if (monitor == NULL || event == NULL ||
        event->kind != LIB_CONSOLE_EVENT_COOKED_LINE) return;
    (void)app_control_queue_push_monitor_line(monitor->control_queue,
        &event->value.line);
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
    if (lib_console_create(&monitor->console) != LIB_STATUS_OK ||
        lib_console_set_event_sink(monitor->console, app_monitor_receive,
            monitor) != LIB_STATUS_OK ||
        host_console_broker_create(&monitor->broker, monitor->console,
            HOST_CONSOLE_COOKED_LINES) != LIB_STATUS_OK) {
        app_monitor_console_destroy(monitor);
        return 0;
    }
    *out_monitor = monitor;
    return 1;
}

void app_monitor_console_destroy(app_monitor_console *monitor)
{
    if (monitor == NULL) return;
    host_console_broker_destroy(monitor->broker);
    if (monitor->console != NULL) {
        (void)lib_console_set_event_sink(monitor->console, NULL, NULL);
        lib_console_release(monitor->console);
    }
    free(monitor);
}

lib_console *app_monitor_console_object(app_monitor_console *monitor)
{ return monitor == NULL ? NULL : monitor->console; }

host_console_broker *app_monitor_console_broker(app_monitor_console *monitor)
{ return monitor == NULL ? NULL : monitor->broker; }

int app_monitor_console_write(app_monitor_console *monitor, const char *text)
{
    if (monitor == NULL || text == NULL) return 0;
    return lib_console_write_text(monitor->console, text, strlen(text)) ==
        LIB_STATUS_OK;
}
#endif
