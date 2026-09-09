#include "monitor_command.h"

#include <assert.h>
#include <string.h>

typedef struct monitor_command_case {
    app_monitor_state state;
    app_monitor_lifecycle_command command;
    app_reconciler_intent intent;
    const char *message;
} monitor_command_case;

int main(void)
{
    static const monitor_command_case cases[] = {
        { APP_MONITOR_INIT, APP_MONITOR_COMMAND_START,
            APP_RECONCILER_INTENT_START, NULL },
        { APP_MONITOR_INIT, APP_MONITOR_COMMAND_PAUSE,
            APP_RECONCILER_INTENT_NONE, "Machine has not started; use start or reset.\r\n" },
        { APP_MONITOR_INIT, APP_MONITOR_COMMAND_RESUME,
            APP_RECONCILER_INTENT_NONE, "Machine has not started; use start or reset.\r\n" },
        { APP_MONITOR_INIT, APP_MONITOR_COMMAND_RESET,
            APP_RECONCILER_INTENT_RESET, NULL },
        { APP_MONITOR_INIT, APP_MONITOR_COMMAND_STOP,
            APP_RECONCILER_INTENT_NONE, "Machine has not started; use start or reset.\r\n" },
        { APP_MONITOR_STOPPED, APP_MONITOR_COMMAND_START,
            APP_RECONCILER_INTENT_START, NULL },
        { APP_MONITOR_STOPPED, APP_MONITOR_COMMAND_PAUSE,
            APP_RECONCILER_INTENT_NONE, "Machine is stopped; use start or reset.\r\n" },
        { APP_MONITOR_STOPPED, APP_MONITOR_COMMAND_RESUME,
            APP_RECONCILER_INTENT_NONE, "Machine is stopped; use start or reset.\r\n" },
        { APP_MONITOR_STOPPED, APP_MONITOR_COMMAND_RESET,
            APP_RECONCILER_INTENT_RESET, NULL },
        { APP_MONITOR_STOPPED, APP_MONITOR_COMMAND_STOP,
            APP_RECONCILER_INTENT_NONE, "Machine is stopped; use start or reset.\r\n" },
        { APP_MONITOR_PAUSED, APP_MONITOR_COMMAND_START,
            APP_RECONCILER_INTENT_NONE, "Machine is paused; use resume, reset, or stop.\r\n" },
        { APP_MONITOR_PAUSED, APP_MONITOR_COMMAND_PAUSE,
            APP_RECONCILER_INTENT_NONE, "Machine is paused; use resume, reset, or stop.\r\n" },
        { APP_MONITOR_PAUSED, APP_MONITOR_COMMAND_RESUME,
            APP_RECONCILER_INTENT_RESUME, NULL },
        { APP_MONITOR_PAUSED, APP_MONITOR_COMMAND_RESET,
            APP_RECONCILER_INTENT_RESET, NULL },
        { APP_MONITOR_PAUSED, APP_MONITOR_COMMAND_STOP,
            APP_RECONCILER_INTENT_STOP, NULL },
        { APP_MONITOR_RUNNING, APP_MONITOR_COMMAND_START,
            APP_RECONCILER_INTENT_NONE, "Machine is already running; use pause, reset, or stop.\r\n" },
        { APP_MONITOR_RUNNING, APP_MONITOR_COMMAND_PAUSE,
            APP_RECONCILER_INTENT_PAUSE, NULL },
        { APP_MONITOR_RUNNING, APP_MONITOR_COMMAND_RESUME,
            APP_RECONCILER_INTENT_NONE, "Machine is already running; use pause, reset, or stop.\r\n" },
        { APP_MONITOR_RUNNING, APP_MONITOR_COMMAND_RESET,
            APP_RECONCILER_INTENT_RESET, NULL },
        { APP_MONITOR_RUNNING, APP_MONITOR_COMMAND_STOP,
            APP_RECONCILER_INTENT_STOP, NULL }
    };
    unsigned int index;

    assert(app_monitor_command_parse("start") == APP_MONITOR_COMMAND_START);
    assert(app_monitor_command_parse("pause") == APP_MONITOR_COMMAND_PAUSE);
    assert(app_monitor_command_parse("resume") == APP_MONITOR_COMMAND_RESUME);
    assert(app_monitor_command_parse("reset") == APP_MONITOR_COMMAND_RESET);
    assert(app_monitor_command_parse("stop") == APP_MONITOR_COMMAND_STOP);
    assert(app_monitor_command_parse("other") == APP_MONITOR_COMMAND_NONE);
    for (index = 0u; index < sizeof(cases) / sizeof(cases[0]); ++index) {
        app_monitor_command_result result = app_monitor_command_resolve(
            cases[index].state, cases[index].command);
        assert(result.intent == cases[index].intent);
        if (cases[index].message == NULL) assert(result.message == NULL);
        else assert(result.message != NULL &&
            strcmp(result.message, cases[index].message) == 0);
    }
    return 0;
}
