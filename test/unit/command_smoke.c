#include "command.h"
#include "reconciler.h"

#include <assert.h>
#include <string.h>

typedef struct command_case {
    const char *text;
} command_case;

static const command_case lifecycle_commands[] = {
    { "start" }, { "pause" }, { "resume" }, { "reset" }, { "stop" }
};

static app_reconciler_intent expected_intent(app_monitor_state state,
    const char *command)
{
    if (strcmp(command, "start") == 0)
        return state == APP_MONITOR_INIT || state == APP_MONITOR_STOPPED ?
            APP_RECONCILER_INTENT_START : APP_RECONCILER_INTENT_NONE;
    if (strcmp(command, "pause") == 0)
        return state == APP_MONITOR_RUNNING ? APP_RECONCILER_INTENT_PAUSE :
            APP_RECONCILER_INTENT_NONE;
    if (strcmp(command, "resume") == 0)
        return state == APP_MONITOR_PAUSED ? APP_RECONCILER_INTENT_RESUME :
            APP_RECONCILER_INTENT_NONE;
    if (strcmp(command, "reset") == 0)
        return APP_RECONCILER_INTENT_RESET;
    if (strcmp(command, "stop") == 0)
        return state == APP_MONITOR_RUNNING || state == APP_MONITOR_PAUSED ?
            APP_RECONCILER_INTENT_STOP : APP_RECONCILER_INTENT_NONE;
    assert(0);
    return APP_RECONCILER_INTENT_NONE;
}

static void arm_window_monitor(app_command_session *session)
{
    app_command_effect effect;
    app_command_session_note_monitor_current(session, 1, &effect);
    assert(effect.arm_prompt);
}

static void complete_intent(app_command_session *session,
    app_reconciler_intent intent)
{
    app_command_effect effect;

    switch (intent) {
    case APP_RECONCILER_INTENT_START:
        /* Cold start has an internal stopped stage.  It must not rearm the
         * monitor before the eventual running completion owns the turn. */
        app_command_session_note_runtime(session, SOFTPC_RUNTIME_STOPPED,
            &effect);
        assert(!effect.arm_prompt && effect.text[0] == '\0');
        app_command_session_note_runtime(session, SOFTPC_RUNTIME_RUNNING,
            &effect);
        assert(app_command_session_state(session) == APP_MONITOR_RUNNING);
        arm_window_monitor(session);
        break;
    case APP_RECONCILER_INTENT_PAUSE:
        app_command_session_note_runtime(session, SOFTPC_RUNTIME_PAUSED,
            &effect);
        assert(app_command_session_state(session) == APP_MONITOR_PAUSED);
        assert(strstr(effect.text, "Machine paused") != NULL);
        arm_window_monitor(session);
        break;
    case APP_RECONCILER_INTENT_RESUME:
        app_command_session_note_runtime(session, SOFTPC_RUNTIME_RUNNING,
            &effect);
        assert(app_command_session_state(session) == APP_MONITOR_RUNNING);
        arm_window_monitor(session);
        break;
    case APP_RECONCILER_INTENT_RESET:
        app_command_session_note_runtime(session, SOFTPC_RUNTIME_STOPPED,
            &effect);
        assert(!effect.arm_prompt && effect.text[0] == '\0');
        /* Reset is a cold start followed by a pause.  RUNNING here is an
         * internal runtime fact, not a stable monitor state: accepting a
         * prompt at this point permits a command to race the required pause. */
        app_command_session_note_runtime(session, SOFTPC_RUNTIME_RUNNING,
            &effect);
        assert(app_command_session_state(session) == APP_MONITOR_STOPPED);
        assert(!effect.arm_prompt && effect.text[0] == '\0');
        app_command_session_note_runtime(session, SOFTPC_RUNTIME_PAUSED,
            &effect);
        assert(app_command_session_state(session) == APP_MONITOR_PAUSED);
        assert(strstr(effect.text, "Machine reset and paused") != NULL);
        arm_window_monitor(session);
        break;
    case APP_RECONCILER_INTENT_STOP:
        app_command_session_note_runtime(session, SOFTPC_RUNTIME_STOPPED,
            &effect);
        assert(app_command_session_state(session) == APP_MONITOR_STOPPED);
        assert(strstr(effect.text, "Machine stopped") != NULL);
        arm_window_monitor(session);
        break;
    case APP_RECONCILER_INTENT_NONE:
        break;
    default:
        assert(0);
    }
}

static void run_sequence(const command_case *sequence, size_t count)
{
    app_command_session session;
    app_command_effect effect;
    size_t index;

    app_command_session_initialize(&session, SOFTPC_PRESENTATION_WINDOW);
    app_command_session_open(&session, &effect);
    assert(strstr(effect.text, "Insignia SoftPC") != NULL);
    arm_window_monitor(&session);
    app_command_session_submit_line(&session, "start", &effect);
    assert(app_command_session_take_intent(&session) ==
        APP_RECONCILER_INTENT_START);
    complete_intent(&session, APP_RECONCILER_INTENT_START);

    for (index = 0u; index < count; ++index) {
        app_monitor_state before = app_command_session_state(&session);
        app_command_session_submit_line(&session, sequence[index].text, &effect);
        {
            app_reconciler_intent intent =
                app_command_session_take_intent(&session);
            assert(intent == expected_intent(before, sequence[index].text));
            if (intent == APP_RECONCILER_INTENT_NONE) {
                assert(app_command_session_state(&session) == before);
                assert(effect.text[0] != '\0');
                arm_window_monitor(&session);
            } else {
                complete_intent(&session, intent);
            }
        }
    }
}

static void run_all_three_command_sequences(void)
{
    size_t first;
    size_t second;
    size_t third;
    command_case sequence[3];

    /* Every ordered triple after the initial start: 5^3 = 125 paths.  This
     * includes start-reset-resume, start-resume-start, and
     * start-pause-reset-start, plus their rejected-command variants. */
    for (first = 0u; first < sizeof(lifecycle_commands) /
            sizeof(lifecycle_commands[0]); ++first) {
        for (second = 0u; second < sizeof(lifecycle_commands) /
                sizeof(lifecycle_commands[0]); ++second) {
            for (third = 0u; third < sizeof(lifecycle_commands) /
                    sizeof(lifecycle_commands[0]); ++third) {
                sequence[0] = lifecycle_commands[first];
                sequence[1] = lifecycle_commands[second];
                sequence[2] = lifecycle_commands[third];
                run_sequence(sequence, 3u);
            }
        }
    }
}

static void submit_with_reconciler(app_command_session *session,
    app_reconciler *reconciler, const char *text,
    app_reconciler_intent expected)
{
    app_command_effect effect;

    app_command_session_submit_line(session, text, &effect);
    assert(app_command_session_take_intent(session) == expected);
    app_reconciler_note_intent(reconciler, expected);
}

static void note_pair(app_command_session *session,
    app_reconciler *reconciler, app_runtime_state state,
    app_reconciler_action next)
{
    app_command_effect effect;

    app_command_session_note_runtime(session, state, &effect);
    app_reconciler_note_runtime(reconciler, state);
    assert(app_reconciler_take_action(reconciler) == next);
}

static void test_start_stop_start_reset_chain(void)
{
    app_command_session session;
    app_reconciler reconciler;

    app_command_session_initialize(&session, SOFTPC_PRESENTATION_WINDOW);
    app_reconciler_initialize(&reconciler, SOFTPC_PRESENTATION_WINDOW, 1);

    submit_with_reconciler(&session, &reconciler, "start",
        APP_RECONCILER_INTENT_START);
    assert(app_reconciler_take_action(&reconciler) ==
        APP_RECONCILER_ACTION_RUNTIME_START);
    note_pair(&session, &reconciler, SOFTPC_RUNTIME_RUNNING,
        APP_RECONCILER_ACTION_CREATE_WINDOW);
    app_reconciler_note_window(&reconciler, 1);

    submit_with_reconciler(&session, &reconciler, "stop",
        APP_RECONCILER_INTENT_STOP);
    assert(app_reconciler_take_action(&reconciler) ==
        APP_RECONCILER_ACTION_RUNTIME_STOP);
    note_pair(&session, &reconciler, SOFTPC_RUNTIME_STOPPED,
        APP_RECONCILER_ACTION_DESTROY_WINDOW);
    app_reconciler_note_window(&reconciler, 0);

    submit_with_reconciler(&session, &reconciler, "start",
        APP_RECONCILER_INTENT_START);
    assert(app_reconciler_take_action(&reconciler) ==
        APP_RECONCILER_ACTION_RUNTIME_START);
    note_pair(&session, &reconciler, SOFTPC_RUNTIME_RUNNING,
        APP_RECONCILER_ACTION_CREATE_WINDOW);
    app_reconciler_note_window(&reconciler, 1);

    submit_with_reconciler(&session, &reconciler, "reset",
        APP_RECONCILER_INTENT_RESET);
    assert(app_reconciler_take_action(&reconciler) ==
        APP_RECONCILER_ACTION_RUNTIME_STOP);
    note_pair(&session, &reconciler, SOFTPC_RUNTIME_STOPPED,
        APP_RECONCILER_ACTION_RUNTIME_START);
    note_pair(&session, &reconciler, SOFTPC_RUNTIME_RUNNING,
        APP_RECONCILER_ACTION_RUNTIME_PAUSE);
    note_pair(&session, &reconciler, SOFTPC_RUNTIME_PAUSED,
        APP_RECONCILER_ACTION_NONE);
    assert(app_command_session_state(&session) == APP_MONITOR_PAUSED);
}

int main(void)
{
    app_command_session session;
    app_command_effect effect;

    run_all_three_command_sequences();
    test_start_stop_start_reset_chain();

    app_command_session_initialize(&session, SOFTPC_PRESENTATION_WINDOW);
    session.state = APP_MONITOR_STOPPED;
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
