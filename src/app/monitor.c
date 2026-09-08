#include "monitor.h"

#ifdef _WIN32
#include <windows.h>

#include <stdlib.h>
#include <string.h>

#define APP_MONITOR_LINE_QUEUE_CAPACITY 32u

struct app_monitor_console {
    lib_console *console;
    host_console_broker *broker;
    CRITICAL_SECTION lock;
    HANDLE available;
    lib_console_line lines[APP_MONITOR_LINE_QUEUE_CAPACITY];
    unsigned int first;
    unsigned int count;
};

static void app_monitor_receive(void *opaque, const lib_console_event *event)
{
    app_monitor_console *monitor = (app_monitor_console *)opaque;
    if (monitor == NULL || event == NULL ||
        event->kind != LIB_CONSOLE_EVENT_COOKED_LINE) return;
    EnterCriticalSection(&monitor->lock);
    if (monitor->count != APP_MONITOR_LINE_QUEUE_CAPACITY) {
        monitor->lines[(monitor->first + monitor->count) %
            APP_MONITOR_LINE_QUEUE_CAPACITY] = event->value.line;
        ++monitor->count;
        (void)SetEvent(monitor->available);
    }
    LeaveCriticalSection(&monitor->lock);
}

int app_monitor_console_create(app_monitor_console **out_monitor)
{
    app_monitor_console *monitor;
    if (out_monitor == NULL) return 0;
    *out_monitor = NULL;
    monitor = calloc(1u, sizeof(*monitor));
    if (monitor == NULL) return 0;
    InitializeCriticalSection(&monitor->lock);
    monitor->available = CreateEventA(NULL, TRUE, FALSE, NULL);
    if (monitor->available == NULL ||
        lib_console_create(&monitor->console) != LIB_STATUS_OK ||
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
    if (monitor->available != NULL) CloseHandle(monitor->available);
    DeleteCriticalSection(&monitor->lock);
    free(monitor);
}

lib_console *app_monitor_console_object(app_monitor_console *monitor)
{ return monitor == NULL ? NULL : monitor->console; }

host_console_broker *app_monitor_console_broker(app_monitor_console *monitor)
{ return monitor == NULL ? NULL : monitor->broker; }

int app_monitor_console_take_line(app_monitor_console *monitor, char *text,
    size_t capacity, unsigned long timeout_ms)
{
    lib_console_line line;
    DWORD waited;
    if (monitor == NULL || text == NULL || capacity == 0u) return 0;
    waited = WaitForSingleObject(monitor->available, timeout_ms);
    if (waited != WAIT_OBJECT_0) return 0;
    EnterCriticalSection(&monitor->lock);
    if (monitor->count == 0u) {
        (void)ResetEvent(monitor->available);
        LeaveCriticalSection(&monitor->lock);
        return 0;
    }
    line = monitor->lines[monitor->first];
    monitor->first = (monitor->first + 1u) % APP_MONITOR_LINE_QUEUE_CAPACITY;
    --monitor->count;
    if (monitor->count == 0u) (void)ResetEvent(monitor->available);
    LeaveCriticalSection(&monitor->lock);
    if ((size_t)line.length >= capacity) return 0;
    memcpy(text, line.text, line.length);
    text[line.length] = '\0';
    return 1;
}

int app_monitor_console_write(app_monitor_console *monitor, const char *text)
{
    if (monitor == NULL || text == NULL) return 0;
    return lib_console_write_text(monitor->console, text, strlen(text)) ==
        LIB_STATUS_OK;
}
#endif
