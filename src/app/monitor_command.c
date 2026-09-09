#include "monitor_command.h"

#include <string.h>

static app_monitor_command_result app_monitor_command_intent(
    app_reconciler_intent intent)
{
    return (app_monitor_command_result) { intent, NULL };
}

static app_monitor_command_result app_monitor_command_reject(const char *message)
{
    return (app_monitor_command_result) { APP_RECONCILER_INTENT_NONE, message };
}

app_monitor_lifecycle_command app_monitor_command_parse(const char *text)
{
    if (text == NULL) return APP_MONITOR_COMMAND_NONE;
    if (strcmp(text, "start") == 0) return APP_MONITOR_COMMAND_START;
    if (strcmp(text, "pause") == 0) return APP_MONITOR_COMMAND_PAUSE;
    if (strcmp(text, "resume") == 0) return APP_MONITOR_COMMAND_RESUME;
    if (strcmp(text, "reset") == 0) return APP_MONITOR_COMMAND_RESET;
    if (strcmp(text, "stop") == 0) return APP_MONITOR_COMMAND_STOP;
    return APP_MONITOR_COMMAND_NONE;
}

app_monitor_command_result app_monitor_command_resolve(app_monitor_state state,
    app_monitor_lifecycle_command command)
{
    switch (command) {
    case APP_MONITOR_COMMAND_START:
        if (state == APP_MONITOR_INIT || state == APP_MONITOR_STOPPED)
            return app_monitor_command_intent(APP_RECONCILER_INTENT_START);
        if (state == APP_MONITOR_PAUSED)
            return app_monitor_command_reject(
                "Machine is paused; use resume, reset, or stop.\r\n");
        return app_monitor_command_reject(
            "Machine is already running; use pause, reset, or stop.\r\n");
    case APP_MONITOR_COMMAND_PAUSE:
        if (state == APP_MONITOR_RUNNING)
            return app_monitor_command_intent(APP_RECONCILER_INTENT_PAUSE);
        if (state == APP_MONITOR_PAUSED)
            return app_monitor_command_reject(
                "Machine is paused; use resume, reset, or stop.\r\n");
        return app_monitor_command_reject(state == APP_MONITOR_INIT ?
            "Machine has not started; use start or reset.\r\n" :
            "Machine is stopped; use start or reset.\r\n");
    case APP_MONITOR_COMMAND_RESUME:
        if (state == APP_MONITOR_PAUSED)
            return app_monitor_command_intent(APP_RECONCILER_INTENT_RESUME);
        if (state == APP_MONITOR_RUNNING)
            return app_monitor_command_reject(
                "Machine is already running; use pause, reset, or stop.\r\n");
        return app_monitor_command_reject(state == APP_MONITOR_INIT ?
            "Machine has not started; use start or reset.\r\n" :
            "Machine is stopped; use start or reset.\r\n");
    case APP_MONITOR_COMMAND_RESET:
        return app_monitor_command_intent(APP_RECONCILER_INTENT_RESET);
    case APP_MONITOR_COMMAND_STOP:
        if (state == APP_MONITOR_RUNNING || state == APP_MONITOR_PAUSED)
            return app_monitor_command_intent(APP_RECONCILER_INTENT_STOP);
        return app_monitor_command_reject(state == APP_MONITOR_INIT ?
            "Machine has not started; use start or reset.\r\n" :
            "Machine is stopped; use start or reset.\r\n");
    case APP_MONITOR_COMMAND_NONE:
        break;
    }
    return app_monitor_command_reject("Unknown command.\r\n");
}
