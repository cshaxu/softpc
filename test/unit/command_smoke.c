#include "command.h"

#include <assert.h>
#include <string.h>

static void line(app_command_session *session, const char *input,
    app_reconciler_intent intent)
{
    app_command_effect effect;
    app_command_session_submit_line(session, input, &effect);
    assert(effect.intent == intent);
}

int main(void)
{
    app_command_session session;
    app_command_effect effect;

    app_command_session_initialize(&session, SOFTPC_PRESENTATION_WINDOW);
    app_command_session_open(&session, &effect);
    assert(strstr(effect.text, "Insignia SoftPC") != NULL);
    app_command_session_note_monitor_current(&session, 1, &effect);
    assert(effect.arm_prompt);
    line(&session, "start", APP_RECONCILER_INTENT_START);
    app_command_session_note_runtime(&session, SOFTPC_RUNTIME_RUNNING, &effect);
    assert(app_command_session_state(&session) == APP_MONITOR_RUNNING);
    app_command_session_note_monitor_current(&session, 1, &effect);
    assert(effect.arm_prompt);
    line(&session, "pause", APP_RECONCILER_INTENT_PAUSE);
    app_command_session_note_runtime(&session, SOFTPC_RUNTIME_PAUSED, &effect);
    assert(strstr(effect.text, "Machine paused") != NULL);
    app_command_session_note_monitor_current(&session, 1, &effect);
    assert(effect.arm_prompt);
    line(&session, "resume", APP_RECONCILER_INTENT_RESUME);
    session.state = APP_MONITOR_STOPPED;
    line(&session, "start", APP_RECONCILER_INTENT_START);
    app_command_session_submit_line(&session, "stop", &effect);
    assert(effect.intent == APP_RECONCILER_INTENT_NONE);
    assert(strstr(effect.text, "Machine is stopped") != NULL);
    app_command_session_submit_line(&session, "floppy eject", &effect);
    assert(effect.action == APP_COMMAND_ACTION_EJECT_FLOPPY);
    app_command_session_complete_floppy(&session, effect.action, 1, &effect);
    assert(strstr(effect.text, "Floppy ejected") != NULL);

    app_command_session_initialize(&session, SOFTPC_PRESENTATION_CONSOLE);
    session.state = APP_MONITOR_RUNNING;
    app_command_session_note_runtime(&session, SOFTPC_RUNTIME_RUNNING, &effect);
    app_command_session_note_monitor_current(&session, 1, &effect);
    assert(!effect.arm_prompt);
    app_command_session_note_broker(&session, 0, 1);
    app_command_session_note_monitor_current(&session, 1, &effect);
    assert(effect.arm_prompt);
    return 0;
}
