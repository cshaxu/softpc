#include "command.h"

#include <assert.h>
#include <string.h>

typedef struct command_case { const char *text; } command_case;
static const command_case commands[] = {
    { "start" }, { "pause" }, { "resume" }, { "reset" }, { "stop" }
};

static app_lifecycle_request expected(app_monitor_state state,
    const char *command)
{
    if (!strcmp(command, "start"))
        return state == APP_MONITOR_INIT || state == APP_MONITOR_STOPPED ?
            APP_LIFECYCLE_REQUEST_START : APP_LIFECYCLE_REQUEST_NONE;
    if (!strcmp(command, "pause"))
        return state == APP_MONITOR_RUNNING ? APP_LIFECYCLE_REQUEST_PAUSE :
            APP_LIFECYCLE_REQUEST_NONE;
    if (!strcmp(command, "resume"))
        return state == APP_MONITOR_PAUSED ? APP_LIFECYCLE_REQUEST_RESUME :
            APP_LIFECYCLE_REQUEST_NONE;
    if (!strcmp(command, "reset")) return APP_LIFECYCLE_REQUEST_RESET;
    if (!strcmp(command, "stop"))
        return state == APP_MONITOR_RUNNING || state == APP_MONITOR_PAUSED ?
            APP_LIFECYCLE_REQUEST_STOP : APP_LIFECYCLE_REQUEST_NONE;
    assert(0); return APP_LIFECYCLE_REQUEST_NONE;
}

static void arm(app_command_session *session, const char *outcome)
{
    app_command_effect effect;
    app_command_session_note_monitor_current(session, 1, &effect);
    assert(effect.arm_prompt);
    if (outcome != NULL) assert(strstr(effect.text, outcome) != NULL);
}

static void complete(app_command_session *session, app_monitor_state *state,
    app_lifecycle_request request)
{
    app_command_effect effect;
    app_runtime_state fact = SOFTPC_RUNTIME_ERROR;
    const char *outcome = NULL;
    switch (request) {
    case APP_LIFECYCLE_REQUEST_START:
        fact = SOFTPC_RUNTIME_RUNNING; outcome = "Machine started"; break;
    case APP_LIFECYCLE_REQUEST_PAUSE:
        fact = SOFTPC_RUNTIME_PAUSED; outcome = "Machine paused"; break;
    case APP_LIFECYCLE_REQUEST_RESUME:
        fact = SOFTPC_RUNTIME_RUNNING; outcome = "Machine resumed"; break;
    case APP_LIFECYCLE_REQUEST_STOP:
        fact = SOFTPC_RUNTIME_STOPPED; outcome = "Machine stopped"; break;
    case APP_LIFECYCLE_REQUEST_RESET:
        fact = SOFTPC_RUNTIME_RESET_COMPLETED;
        outcome = "Machine reset and paused"; break;
    case APP_LIFECYCLE_REQUEST_NONE:
        return;
    }
    app_command_session_note_runtime(session, *state, fact, &effect);
    if (fact == SOFTPC_RUNTIME_RESET_COMPLETED || fact == SOFTPC_RUNTIME_PAUSED)
        *state = APP_MONITOR_PAUSED;
    else if (fact == SOFTPC_RUNTIME_RUNNING)
        *state = APP_MONITOR_RUNNING;
    else if (fact == SOFTPC_RUNTIME_STOPPED || fact == SOFTPC_RUNTIME_ERROR)
        *state = APP_MONITOR_STOPPED;
    arm(session, outcome);
}

static void run_matrix(void)
{
    size_t state_index;
    for (state_index = APP_MONITOR_INIT; state_index <= APP_MONITOR_RUNNING;
         ++state_index) {
        size_t command_index;
        for (command_index = 0u; command_index < sizeof(commands) / sizeof(commands[0]);
             ++command_index) {
            app_command_session session;
            app_command_effect effect;
            app_lifecycle_request request;
            app_monitor_state state = (app_monitor_state)state_index;
            app_command_session_initialize(&session, SOFTPC_PRESENTATION_WINDOW);
            app_command_session_submit_line(&session, state,
                commands[command_index].text, &effect);
            request = app_command_session_take_request(&session);
            assert(request == expected(state, commands[command_index].text));
            if (request == APP_LIFECYCLE_REQUEST_NONE) {
                assert(effect.text[0] != '\0');
                arm(&session, NULL);
            } else complete(&session, &state, request);
        }
    }
}

static void test_hotkey_completion_is_not_command_provenance(void)
{
    app_command_session session;
    app_command_effect effect;
    app_monitor_state state = APP_MONITOR_PAUSED;
    app_command_session_initialize(&session, SOFTPC_PRESENTATION_WINDOW);
    /* A Window CAP produces no monitor command request; the completion still
       classifies its monitor outcome from the prior stable state. */
    app_command_session_note_runtime(&session, state, SOFTPC_RUNTIME_RUNNING,
        &effect);
    state = APP_MONITOR_RUNNING;
    assert(state == APP_MONITOR_RUNNING);
    arm(&session, "Machine resumed");
}

static void test_raw_route_discards_running_outcome(void)
{
    app_command_session session;
    app_command_effect effect;

    app_command_session_initialize(&session, SOFTPC_PRESENTATION_CONSOLE);
    app_command_session_submit_line(&session, APP_MONITOR_STOPPED, "start",
        &effect);
    assert(app_command_session_take_request(&session) ==
        APP_LIFECYCLE_REQUEST_START);
    app_command_session_note_runtime(&session, APP_MONITOR_STOPPED,
        SOFTPC_RUNTIME_RUNNING, &effect);
    /* The raw broker completion owns the decision to discard a monitor-only
       running outcome. A later monitor current fact cannot replay it. */
    app_command_session_note_broker(&session, APP_MONITOR_RUNNING, 1, 0);
    app_command_session_note_monitor_current(&session, 1, &effect);
    assert(!effect.arm_prompt && effect.text[0] == '\0');
}

static void test_error_returns_monitor_transaction(void)
{
    app_command_session session;
    app_command_effect effect;

    app_command_session_initialize(&session, SOFTPC_PRESENTATION_WINDOW);
    app_command_session_note_runtime(&session, APP_MONITOR_RUNNING,
        SOFTPC_RUNTIME_ERROR, &effect);
    arm(&session, "Machine error");
}

int main(void)
{
    app_command_session session;
    app_command_effect effect;
    run_matrix();
    test_hotkey_completion_is_not_command_provenance();
    test_raw_route_discards_running_outcome();
    test_error_returns_monitor_transaction();
    app_command_session_initialize(&session, SOFTPC_PRESENTATION_WINDOW);
    assert(strstr(app_command_hotkey_help(),
        "While the guest is running in a raw VM Console:") != NULL);
    app_command_session_open(&session, &effect);
    assert(strstr(effect.text, "cold-reset and run the machine") != NULL);
    assert(strstr(effect.text, app_command_hotkey_help()) != NULL);
    app_command_session_submit_line(&session, APP_MONITOR_STOPPED,
        "floppy eject", &effect);
    assert(effect.action == APP_COMMAND_ACTION_EJECT_FLOPPY);
    app_command_session_complete_floppy(&session, effect.action, 1, &effect);
    assert(strstr(effect.text, "Floppy ejected") != NULL);
    return 0;
}
