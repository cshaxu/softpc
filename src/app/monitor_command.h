#ifndef APP_MONITOR_COMMAND_H
#define APP_MONITOR_COMMAND_H

#include "reconciler.h"

/* Stable monitor state is product policy, separate from transient runtime
 * completions and native presentation details. */
typedef enum app_monitor_state {
    APP_MONITOR_INIT,
    APP_MONITOR_STOPPED,
    APP_MONITOR_PAUSED,
    APP_MONITOR_RUNNING
} app_monitor_state;

typedef enum app_monitor_lifecycle_command {
    APP_MONITOR_COMMAND_NONE,
    APP_MONITOR_COMMAND_START,
    APP_MONITOR_COMMAND_PAUSE,
    APP_MONITOR_COMMAND_RESUME,
    APP_MONITOR_COMMAND_RESET,
    APP_MONITOR_COMMAND_STOP
} app_monitor_lifecycle_command;

typedef struct app_monitor_command_result {
    app_reconciler_intent intent;
    const char *message;
} app_monitor_command_result;

app_monitor_lifecycle_command app_monitor_command_parse(const char *text);
app_monitor_command_result app_monitor_command_resolve(app_monitor_state state,
    app_monitor_lifecycle_command command);

#endif
