#include "command.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

static const char HELP_COMMANDS[] =
    "Insignia SoftPC\r\n"
    "===============\r\n"
    "  start                 cold-reset and run the machine\r\n"
    "  resume                continue a paused machine\r\n"
    "  pause                 request machine pause\r\n"
    "  stop                  stop execution\r\n"
    "  reset                 cold-reset and pause at firmware entry\r\n"
    "  floppy insert <image> insert drive A media while stopped/paused\r\n"
    "  floppy eject          eject drive A media while stopped/paused\r\n"
    "  help                  show this help\r\n"
    "  debug                 enter debugger (q returns to monitor)\r\n"
    "  exit                  quit\r\n";
static const char HELP_HOTKEYS[] =
    "While the guest is running:\r\n"
    "  Ctrl+Alt+P            pause or resume\r\n"
    "  Ctrl+Alt+D            send Ctrl+Alt+Del to the guest\r\n"
    "  Ctrl+Alt+F            send Alt+Enter to the guest\r\n"
    "  Ctrl+Alt+M            release captured mouse\r\n";

static void clear(app_command_effect *e) { memset(e, 0, sizeof(*e)); }
static void text(app_command_effect *e, const char *s) { (void)snprintf(e->text, sizeof(e->text), "%s", s); }
static void message(char *out, size_t capacity, const char *value)
{
    (void)snprintf(out, capacity, "%s\r\n\r\n", value);
}
static void help(app_command_effect *e)
{
    (void)snprintf(e->text, sizeof(e->text), "%s\r\n%s\r\n",
                   HELP_COMMANDS, HELP_HOTKEYS);
}
/* Product readiness remains true until a command reserves a transition.
 * Common alone admits one pending line; callbacks never consume readiness. */
static void prompt(app_command_session *s) { s->prompt_due = 1; }
static void outcome(app_command_session *s, const char *value)
{
    message(s->pending_monitor_text, sizeof(s->pending_monitor_text), value);
    prompt(s);
}
static void reject(app_command_session *s, app_command_effect *e, const char *value)
{
    message(e->text, sizeof(e->text), value);
    prompt(s);
}
static char *trim(char *s)
{
    char *e;
    while (*s && isspace((unsigned char)*s))
        ++s;
    e = s + strlen(s);
    while (e != s && isspace((unsigned char)e[-1]))
        --e;
    *e = 0;
    return s;
}
static void lower(char *s)
{
    while (*s)
    {
        *s = (char)tolower((unsigned char)*s);
        ++s;
    }
}

static void accept(app_command_session *session,
                   app_lifecycle_request request)
{
    session->pending_request = request;
    session->dispatch_pending = 1;
    session->transition_pending = 1;
    session->prompt_due = 0;
}

static void lifecycle(app_command_session *s, app_monitor_state state,
                      const char *c, app_command_effect *e)
{
    if (s->transition_pending || s->dispatch_pending)
    {
        reject(s, e, "Machine state transition is in progress.");
        return;
    }
    if (!strcmp(c, "start"))
    {
        if (state == APP_MONITOR_INIT || state == APP_MONITOR_STOPPED)
        {
            accept(s, APP_LIFECYCLE_REQUEST_START);
        }
        else
            reject(s, e, state == APP_MONITOR_PAUSED ? "Machine is paused; use resume, reset, or stop." : "Machine is already running; use pause, reset, or stop.");
    }
    else if (!strcmp(c, "pause"))
    {
        if (state == APP_MONITOR_RUNNING)
            accept(s, APP_LIFECYCLE_REQUEST_PAUSE);
        else
            reject(s, e, state == APP_MONITOR_PAUSED ? "Machine is paused; use resume, reset, or stop." : state == APP_MONITOR_INIT ? "Machine has not started; use start or reset."
                                                                                                                                    : "Machine is stopped; use start or reset.");
    }
    else if (!strcmp(c, "resume"))
    {
        if (state == APP_MONITOR_PAUSED)
            accept(s, APP_LIFECYCLE_REQUEST_RESUME);
        else
            reject(s, e, state == APP_MONITOR_RUNNING ? "Machine is already running; use pause, reset, or stop." : state == APP_MONITOR_INIT ? "Machine has not started; use start or reset."
                                                                                                                                             : "Machine is stopped; use start or reset.");
    }
    else if (!strcmp(c, "reset"))
    {
        accept(s, APP_LIFECYCLE_REQUEST_RESET);
    }
    else if (!strcmp(c, "stop"))
    {
        if (state == APP_MONITOR_RUNNING || state == APP_MONITOR_PAUSED)
        {
            accept(s, APP_LIFECYCLE_REQUEST_STOP);
        }
        else
            reject(s, e, state == APP_MONITOR_INIT ? "Machine has not started; use start or reset." : "Machine is stopped; use start or reset.");
    }
    else
        reject(s, e, "Unknown command.");
}

void app_command_session_initialize(app_command_session *s, common_session_display display)
{
    memset(s, 0, sizeof(*s));
    s->display = display;
}
const char *app_command_hotkey_help(void) { return HELP_HOTKEYS; }
void app_command_session_open(app_command_session *s, app_command_effect *e)
{
    clear(e);
    help(e);
    prompt(s);
}
void app_command_session_reject_line(app_command_session *s, app_command_effect *e)
{
    clear(e);
    reject(s, e, "Command is too long.");
}
void app_command_session_submit_line(app_command_session *s, app_monitor_state state,
                                     const char *line, app_command_effect *e)
{
    char b[APP_COMMAND_TEXT_CAPACITY], *c, *a, *p;
    size_t n;
    clear(e);
    if (!line || (n = strlen(line)) >= sizeof(b))
    {
        reject(s, e, line ? "Command is too long." : "Unknown command.");
        return;
    }
    memcpy(b, line, n + 1);
    c = trim(b);
    a = c;
    while (*a && !isspace((unsigned char)*a))
        ++a;
    if (*a)
        *a++ = 0;
    a = trim(a);
    lower(c);
    if (!*c)
    {
        prompt(s);
        return;
    }
    if (!strcmp(c, "help"))
    {
        help(e);
        prompt(s);
        return;
    }
    if (!strcmp(c, "exit"))
    {
        e->exit_requested = 1;
        return;
    }
    if (!strcmp(c, "debug") && !*a)
    {
        e->action = APP_COMMAND_ACTION_DEBUG;
        prompt(s);
        return;
    }
    /* The parser does not own machine state.  It receives control's current
       stable fact for the one command validation below. */
    if (strcmp(c, "floppy"))
    {
        lifecycle(s, state, c, e);
        return;
    }
    p = a;
    while (*p && !isspace((unsigned char)*p))
        ++p;
    if (*p)
        *p++ = 0;
    p = trim(p);
    lower(a);
    if (!strcmp(a, "eject") && !*p)
        e->action = APP_COMMAND_ACTION_EJECT_FLOPPY;
    else if (!strcmp(a, "insert") && *p && strlen(p) < sizeof(e->path))
    {
        e->action = APP_COMMAND_ACTION_INSERT_FLOPPY;
        memcpy(e->path, p, strlen(p) + 1);
    }
    else
        reject(s, e, "Usage: floppy insert <image> | eject");
}
app_lifecycle_request app_command_session_take_request(app_command_session *s)
{
    app_lifecycle_request request;
    if (s == NULL || !s->dispatch_pending)
        return APP_LIFECYCLE_REQUEST_NONE;
    request = s->pending_request;
    s->pending_request = APP_LIFECYCLE_REQUEST_NONE;
    s->dispatch_pending = 0;
    return request;
}
int app_command_session_begin_external(app_command_session *s,
                                       app_monitor_state state, app_lifecycle_request request)
{
    if (s == NULL || s->transition_pending ||
        request == APP_LIFECYCLE_REQUEST_NONE)
        return 0;
    if ((request == APP_LIFECYCLE_REQUEST_PAUSE &&
         state != APP_MONITOR_RUNNING) ||
        (request == APP_LIFECYCLE_REQUEST_RESUME &&
         state != APP_MONITOR_PAUSED) ||
        (request == APP_LIFECYCLE_REQUEST_STOP &&
         state != APP_MONITOR_RUNNING && state != APP_MONITOR_PAUSED))
        return 0;
    s->transition_pending = 1;
    s->prompt_due = 0;
    return 1;
}
void app_command_session_complete_floppy(app_command_session *s, app_command_action a, int ok, app_command_effect *e)
{
    clear(e);
    message(e->text, sizeof(e->text),
        a == APP_COMMAND_ACTION_EJECT_FLOPPY ?
            (ok ? "Floppy ejected." : "Cannot eject floppy.") :
            (ok ? "Floppy inserted." : "Cannot insert floppy."));
    prompt(s);
}
void app_command_session_note_runtime(app_command_session *s,
                                      app_monitor_state prior, common_machine_state state, app_command_effect *e)
{
    if (s == NULL || e == NULL)
        return;
    clear(e);
    if (state == COMMON_MACHINE_RESET_COMPLETED)
    {
        s->transition_pending = 0;
        outcome(s, "Machine reset and paused.");
        return;
    }
    if (state == COMMON_MACHINE_PAUSED && prior != APP_MONITOR_PAUSED)
    {
        s->transition_pending = 0;
        outcome(s, "Machine paused.");
    }
    else if (state == COMMON_MACHINE_RUNNING)
    {
        s->transition_pending = 0;
        if (prior == APP_MONITOR_INIT || prior == APP_MONITOR_STOPPED)
            outcome(s, "Machine started.");
        else if (prior == APP_MONITOR_PAUSED)
            outcome(s, "Machine resumed.");
        else if (s->display == COMMON_SESSION_DISPLAY_WINDOW)
            prompt(s);
    }
    else if (state == COMMON_MACHINE_STOPPED &&
             prior != APP_MONITOR_STOPPED)
    {
        s->transition_pending = 0;
        outcome(s, "Machine stopped.");
    }
    else if (state == COMMON_MACHINE_ERROR)
    {
        s->transition_pending = 0;
        outcome(s, "Machine error.");
    }
}

void app_command_session_note_broker(app_command_session *s,
                                     app_monitor_state state, int vm,
                                     int monitor_running_surface)
{
    if (s == NULL)
        return;
    if (vm && state == APP_MONITOR_RUNNING)
    {
        s->prompt_due = 0;
        s->pending_monitor_text[0] = '\0';
    }
    else if (!vm && state == APP_MONITOR_RUNNING &&
             monitor_running_surface)
        prompt(s);
}

void app_command_session_note_monitor_current(app_command_session *s,
                                              int current, app_command_effect *e)
{
    if (s == NULL || e == NULL)
        return;
    clear(e);
    if (!s->prompt_due || !current)
        return;
    text(e, s->pending_monitor_text);
    s->pending_monitor_text[0] = '\0';
    e->arm_prompt = 1;
}

_Static_assert(COMMON_SESSION_TEXT_CAPACITY >= 2u * COMMON_DEBUG_TEXT_CAPACITY,
    "Session text must hold debugger output with CRLF expansion");
_Static_assert(COMMON_SESSION_PROMPT_CAPACITY >= COMMON_DEBUG_PROMPT_CAPACITY,
    "Session prompt must hold debugger continuation prompts");

/* app/ owns product command policy. common/session owns its neutral copied
 * completion facts; convert explicitly at this one composition boundary.
 * The numeric enum values are deliberately not a cross-component contract. */
static common_machine_state app_machine_completed_state(
    common_session_machine_state state)
{
    switch (state) {
    case COMMON_SESSION_MACHINE_RUNNING: return COMMON_MACHINE_RUNNING;
    case COMMON_SESSION_MACHINE_PAUSED: return COMMON_MACHINE_PAUSED;
    case COMMON_SESSION_MACHINE_ERROR: return COMMON_MACHINE_ERROR;
    case COMMON_SESSION_MACHINE_RESET_COMPLETED:
        return COMMON_MACHINE_RESET_COMPLETED;
    default: return COMMON_MACHINE_STOPPED;
    }
}

static app_monitor_state app_command_state(common_session_machine_state state)
{
    switch (state) {
    case COMMON_SESSION_MACHINE_INIT: return APP_MONITOR_INIT;
    case COMMON_SESSION_MACHINE_RUNNING: return APP_MONITOR_RUNNING;
    case COMMON_SESSION_MACHINE_PAUSED: return APP_MONITOR_PAUSED;
    default: return APP_MONITOR_STOPPED;
    }
}

static common_session_request app_session_request(app_lifecycle_request request)
{
    switch (request) {
    case APP_LIFECYCLE_REQUEST_START: return COMMON_SESSION_REQUEST_START;
    case APP_LIFECYCLE_REQUEST_RESUME: return COMMON_SESSION_REQUEST_RESUME;
    case APP_LIFECYCLE_REQUEST_PAUSE: return COMMON_SESSION_REQUEST_PAUSE;
    case APP_LIFECYCLE_REQUEST_STOP: return COMMON_SESSION_REQUEST_STOP;
    case APP_LIFECYCLE_REQUEST_RESET: return COMMON_SESSION_REQUEST_RESET;
    default: return COMMON_SESSION_REQUEST_NONE;
    }
}

static app_lifecycle_request app_lifecycle_request_from_session(
    common_session_request request)
{
    switch (request) {
    case COMMON_SESSION_REQUEST_START: return APP_LIFECYCLE_REQUEST_START;
    case COMMON_SESSION_REQUEST_RESUME: return APP_LIFECYCLE_REQUEST_RESUME;
    case COMMON_SESSION_REQUEST_PAUSE: return APP_LIFECYCLE_REQUEST_PAUSE;
    case COMMON_SESSION_REQUEST_STOP: return APP_LIFECYCLE_REQUEST_STOP;
    case COMMON_SESSION_REQUEST_RESET: return APP_LIFECYCLE_REQUEST_RESET;
    default: return APP_LIFECYCLE_REQUEST_NONE;
    }
}

static void app_command_copy_effect(common_session_command_result *out,
    const app_command_effect *effect)
{
    if (out == NULL || effect == NULL) return;
    *out = (common_session_command_result) { 0 };
    (void)snprintf(out->text, sizeof(out->text), "%s", effect->text);
    out->exit_requested = effect->exit_requested != 0;
    out->arm_prompt = effect->arm_prompt != 0;
}

void app_command_provider_open(void *opaque,
    common_session_command_result *out)
{
    app_command_context *command = (app_command_context *)opaque;
    app_command_effect effect = { 0 };
    app_command_session_open(&command->session, &effect);
    app_command_copy_effect(out, &effect);
}

void app_command_provider_reject_line(void *opaque,
    common_session_command_result *out)
{
    app_command_context *command = (app_command_context *)opaque;
    app_command_effect effect = { 0 };
    app_command_session_reject_line(&command->session, &effect);
    app_command_copy_effect(out, &effect);
}

static void app_command_copy_debug(app_command_context *command,
    common_session_machine_state state, const common_debug_result *result,
    common_session_command_result *out)
{
    size_t source, target = strlen(out->text);
    if (target + 2u * strlen(result->text) >= sizeof(out->text)) {
        (void)snprintf(out->text, sizeof(out->text), "Debug output exceeds Console capacity.\r\n\r\n");
        common_machine_debug_cancel(command->machine);
        return;
    }
    for (source = 0u; result->text[source] != '\0'; ++source) {
        if (result->text[source] == '\n' &&
            (source == 0u || result->text[source - 1u] != '\r')) out->text[target++] = '\r';
        out->text[target++] = result->text[source];
    }
    out->text[target] = '\0';
    (void)snprintf(command->debug_prompt, sizeof(command->debug_prompt), "%s", result->prompt);
    if (!result->keep_active) {
        common_debug_close(command->debug);
        command->debug_active = LIB_FALSE;
        command->debug_completed_pending = LIB_FALSE;
    }
    if (result->lifecycle_request == COMMON_DEBUG_LIFECYCLE_RESUME &&
        app_command_session_begin_external(&command->session,
            app_command_state(state), APP_LIFECYCLE_REQUEST_RESUME))
        out->request = COMMON_SESSION_REQUEST_RESUME;
    else if (result->lifecycle_request != COMMON_DEBUG_LIFECYCLE_NONE)
        (void)snprintf(out->text, sizeof(out->text), "Debug lifecycle request is not applicable.\r\n\r\n");
}

void app_command_provider_submit_line(void *opaque,
    common_session_machine_state state, const char *line,
    common_session_command_result *out)
{
    app_command_context *command = (app_command_context *)opaque;
    app_command_effect effect = { 0 };
    if (command->debug_active) {
        common_debug_result result = { 0 };
        lib_status status = common_debug_submit_line(command->debug, line, &result);
        *out = (common_session_command_result) { 0 };
        if (status != LIB_STATUS_OK) {
            (void)snprintf(out->text, sizeof(out->text), "Debug command failed.\r\n\r\n");
        } else {
            app_command_copy_debug(command, state, &result, out);
        }
        command->session.prompt_due = out->request == COMMON_SESSION_REQUEST_NONE;
        return;
    }
    app_command_session_submit_line(&command->session, app_command_state(state), line,
        &effect);
    if (effect.action == APP_COMMAND_ACTION_DEBUG) {
        if (common_debug_open(command->debug, command->machine) == LIB_STATUS_OK) {
            command->debug_active = LIB_TRUE;
            (void)snprintf(command->debug_prompt, sizeof(command->debug_prompt), "-");
            (void)snprintf(effect.text, sizeof(effect.text),
                "Debugger: ? for help, q to return. Machine access requires pause.\r\n\r\n");
        } else (void)snprintf(effect.text, sizeof(effect.text), "Cannot open debugger.\r\n\r\n");
    } else if (effect.action != APP_COMMAND_ACTION_NONE) {
        int succeeded = effect.action == APP_COMMAND_ACTION_EJECT_FLOPPY ?
            common_machine_set_removable_media(command->machine, NULL) :
            common_machine_set_removable_media(command->machine, effect.path);
        app_command_session_complete_floppy(&command->session, effect.action,
            succeeded, &effect);
    }
    app_command_copy_effect(out, &effect);
    out->request = app_session_request(
        app_command_session_take_request(&command->session));
}

lib_bool app_command_provider_begin_external(void *opaque,
    common_session_machine_state state, common_session_request request)
{
    app_command_context *command = (app_command_context *)opaque;
    return app_command_session_begin_external(&command->session,
        app_command_state(state), app_lifecycle_request_from_session(request)) != 0;
}

void app_command_provider_note_runtime(void *opaque,
    common_session_machine_state prior, common_session_machine_state completed,
    common_session_command_result *out)
{
    app_command_context *command = (app_command_context *)opaque;
    app_command_effect effect = { 0 };
    app_command_session_note_runtime(&command->session, app_command_state(prior),
        app_machine_completed_state(completed), &effect);
    app_command_copy_effect(out, &effect);
    if (command->debug_active) {
        if (completed != COMMON_SESSION_MACHINE_PAUSED)
            command->debug_completed_pending = LIB_FALSE;
        common_debug_machine_state state = completed == COMMON_SESSION_MACHINE_PAUSED ?
            COMMON_DEBUG_MACHINE_PAUSED : completed == COMMON_SESSION_MACHINE_RUNNING ?
            COMMON_DEBUG_MACHINE_RUNNING : COMMON_DEBUG_MACHINE_STOPPED;
        common_debug_result result = { 0 };
        (void)common_debug_observe_machine(command->debug, state, LIB_STATUS_OK, &result);
        if (result.prompt_ready) {
            command->debug_completed = result;
            command->debug_completed_pending = LIB_TRUE;
        }
    }
}

void app_command_provider_note_broker(void *opaque,
    common_session_machine_state state, lib_bool vm_console_current,
    lib_bool monitor_running_surface)
{
    app_command_context *command = (app_command_context *)opaque;
    app_command_session_note_broker(&command->session, app_command_state(state),
        vm_console_current != 0, monitor_running_surface != 0);
}

void app_command_provider_note_monitor_current(void *opaque,
    lib_bool current, common_session_command_result *out)
{
    app_command_context *command = (app_command_context *)opaque;
    app_command_effect effect = { 0 };
    app_command_session_note_monitor_current(&command->session, current != 0,
        &effect);
    app_command_copy_effect(out, &effect);
    if (current && command->debug_completed_pending) {
        command->debug_completed_pending = LIB_FALSE;
        app_command_copy_debug(command, COMMON_SESSION_MACHINE_PAUSED,
            &command->debug_completed, out);
        out->arm_prompt = out->request == COMMON_SESSION_REQUEST_NONE;
    }
    (void)snprintf(out->prompt, sizeof(out->prompt), "%s",
        command->debug_active ? command->debug_prompt : "SoftPC> ");
}

lib_status app_command_initialize(app_command_context *command,
    common_machine *machine, common_session_display display)
{
    if (command == NULL || machine == NULL) return LIB_STATUS_INVALID_ARGUMENT;
    memset(command, 0, sizeof(*command));
    command->machine = machine;
    app_command_session_initialize(&command->session, display);
    return common_debug_create(&command->debug);
}

void app_command_dispose(app_command_context *command)
{
    if (command != NULL) common_debug_destroy(command->debug);
}
