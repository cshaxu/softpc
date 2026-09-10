#include "command.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

static const char HELP_COMMANDS[] =
    "Insignia SoftPC\r\n===============\r\n"
    "  start                 cold-reset and run the machine\r\n"
    "  resume                continue a paused machine\r\n"
    "  pause                 request machine pause\r\n"
    "  stop                  stop execution\r\n"
    "  reset                 cold-reset and pause at firmware entry\r\n"
    "  floppy insert <image> insert drive A media while stopped/paused\r\n"
    "  floppy eject          eject drive A media while stopped/paused\r\n"
    "  help                  show this help\r\n  exit                  quit\r\n";
static const char HELP_HOTKEYS[] =
    "While the guest is running:\r\n"
    "  Ctrl+Alt+P            pause or resume\r\n"
    "  Ctrl+Alt+D            send Ctrl+Alt+Del to the guest\r\n"
    "  Ctrl+Alt+F            send Alt+Enter to the guest\r\n"
    "  Ctrl+Alt+M            release captured mouse\r\n";

static void clear(app_command_effect *e) { memset(e, 0, sizeof(*e)); }
static void text(app_command_effect *e, const char *s) { (void)snprintf(e->text, sizeof(e->text), "%s", s); }
static void help(app_command_effect *e)
{
    (void)snprintf(e->text, sizeof(e->text), "%s\r\n%s\r\n",
        HELP_COMMANDS, HELP_HOTKEYS);
}
/* Product prompt demand is separate from the host's one cooked reader.  The
 * broker alone owns that reader and safely makes its arm request idempotent. */
static void prompt(app_command_session *s) { s->prompt_due = 1; }
static void outcome(app_command_session *s, const char *message)
{
    (void)snprintf(s->pending_monitor_text, sizeof(s->pending_monitor_text),
        "%s", message);
    prompt(s);
}
static void reject(app_command_session *s, app_command_effect *e, const char *message) { (void)snprintf(e->text, sizeof(e->text), "%s\r\n", message); prompt(s); }
static char *trim(char *s) { char *e; while (*s && isspace((unsigned char)*s)) ++s; e=s+strlen(s); while(e!=s&&isspace((unsigned char)e[-1]))--e; *e=0; return s; }
static void lower(char *s) { while (*s) { *s=(char)tolower((unsigned char)*s); ++s; } }

static void accept(app_command_session *session,
    app_lifecycle_request request)
{
    session->pending_request = request;
    session->dispatch_pending = 1;
    session->transition_pending = 1;
}

static void lifecycle(app_command_session *s, app_monitor_state state,
    const char *c, app_command_effect *e)
{
    if (s->transition_pending || s->dispatch_pending) {
        reject(s, e, "Machine state transition is in progress.");
        return;
    }
    if (!strcmp(c,"start")) {
        if(state==APP_MONITOR_INIT||state==APP_MONITOR_STOPPED){accept(s,APP_LIFECYCLE_REQUEST_START);}
        else reject(s,e,state==APP_MONITOR_PAUSED?"Machine is paused; use resume, reset, or stop.":"Machine is already running; use pause, reset, or stop.");
    } else if (!strcmp(c,"pause")) {
        if(state==APP_MONITOR_RUNNING)accept(s,APP_LIFECYCLE_REQUEST_PAUSE);
        else reject(s,e,state==APP_MONITOR_PAUSED?"Machine is paused; use resume, reset, or stop.":state==APP_MONITOR_INIT?"Machine has not started; use start or reset.":"Machine is stopped; use start or reset.");
    } else if (!strcmp(c,"resume")) {
        if(state==APP_MONITOR_PAUSED)accept(s,APP_LIFECYCLE_REQUEST_RESUME);
        else reject(s,e,state==APP_MONITOR_RUNNING?"Machine is already running; use pause, reset, or stop.":state==APP_MONITOR_INIT?"Machine has not started; use start or reset.":"Machine is stopped; use start or reset.");
    } else if (!strcmp(c,"reset")) { accept(s,APP_LIFECYCLE_REQUEST_RESET); }
    else if (!strcmp(c,"stop")) {
        if(state==APP_MONITOR_RUNNING||state==APP_MONITOR_PAUSED){accept(s,APP_LIFECYCLE_REQUEST_STOP);}
        else reject(s,e,state==APP_MONITOR_INIT?"Machine has not started; use start or reset.":"Machine is stopped; use start or reset.");
    } else reject(s,e,"Unknown command.");
}

void app_command_session_initialize(app_command_session *s, softpc_presentation display) { memset(s,0,sizeof(*s));s->display=display; }
const char *app_command_hotkey_help(void) { return HELP_HOTKEYS; }
void app_command_session_open(app_command_session *s, app_command_effect *e) { clear(e); help(e); prompt(s); }
void app_command_session_submit_line(app_command_session *s, app_monitor_state state,
    const char *line,app_command_effect *e)
{
    char b[APP_COMMAND_TEXT_CAPACITY],*c,*a,*p; size_t n; clear(e);
    if(!line||(n=strlen(line))>=sizeof(b)){reject(s,e,line?"Command is too long.":"Unknown command.");return;}
    memcpy(b,line,n+1);c=trim(b);a=c;while(*a&&!isspace((unsigned char)*a))++a;if(*a)*a++=0;a=trim(a);lower(c);
    if(!*c){prompt(s);return;} if(!strcmp(c,"help")){help(e);prompt(s);return;} if(!strcmp(c,"exit")){e->exit_requested=1;return;}
    /* The parser does not own machine state.  It receives control's current
       stable fact for the one command validation below. */
    if(strcmp(c,"floppy")){lifecycle(s,state,c,e);return;}
    p=a;while(*p&&!isspace((unsigned char)*p))++p;if(*p)*p++=0;p=trim(p);lower(a);
    if(!strcmp(a,"eject")&&!*p)e->action=APP_COMMAND_ACTION_EJECT_FLOPPY;
    else if(!strcmp(a,"insert")&&*p&&strlen(p)<sizeof(e->path)){e->action=APP_COMMAND_ACTION_INSERT_FLOPPY;memcpy(e->path,p,strlen(p)+1);}
    else reject(s,e,"Usage: floppy insert <image> | eject");
}
app_lifecycle_request app_command_session_take_request(app_command_session *s)
{
    app_lifecycle_request request;
    if (s == NULL || !s->dispatch_pending) return APP_LIFECYCLE_REQUEST_NONE;
    request = s->pending_request;
    s->pending_request = APP_LIFECYCLE_REQUEST_NONE;
    s->dispatch_pending = 0;
    return request;
}
int app_command_session_begin_external(app_command_session *s,
    app_monitor_state state, app_lifecycle_request request)
{
    if (s == NULL || s->transition_pending ||
        request == APP_LIFECYCLE_REQUEST_NONE) return 0;
    if ((request == APP_LIFECYCLE_REQUEST_PAUSE &&
            state != APP_MONITOR_RUNNING) ||
        (request == APP_LIFECYCLE_REQUEST_RESUME &&
            state != APP_MONITOR_PAUSED) ||
        (request == APP_LIFECYCLE_REQUEST_STOP &&
            state != APP_MONITOR_RUNNING && state != APP_MONITOR_PAUSED))
        return 0;
    s->transition_pending = 1;
    return 1;
}
void app_command_session_complete_floppy(app_command_session *s,app_command_action a,int ok,app_command_effect *e)
{ clear(e); text(e,a==APP_COMMAND_ACTION_EJECT_FLOPPY?(ok?"Floppy ejected.\r\n":"Cannot eject floppy.\r\n"):(ok?"Floppy inserted.\r\n":"Cannot insert floppy.\r\n"));prompt(s); }
void app_command_session_note_runtime(app_command_session *s,
    app_monitor_state prior, app_runtime_state state,app_command_effect *e)
{
    if (s == NULL || e == NULL) return;
    clear(e);
    if (state == SOFTPC_RUNTIME_RESET_COMPLETED) {
        s->transition_pending = 0;
        outcome(s, "Machine reset and paused.\r\n");
        return;
    }
    if (state == SOFTPC_RUNTIME_PAUSED && prior != APP_MONITOR_PAUSED) {
        s->transition_pending = 0;
        outcome(s, "Machine paused.\r\n");
    } else if (state == SOFTPC_RUNTIME_RUNNING) {
        s->transition_pending = 0;
        if (prior == APP_MONITOR_INIT || prior == APP_MONITOR_STOPPED)
            outcome(s, "Machine started.\r\n");
        else if (prior == APP_MONITOR_PAUSED)
            outcome(s, "Machine resumed.\r\n");
        else if (s->display == SOFTPC_PRESENTATION_WINDOW)
            prompt(s);
    } else if (state == SOFTPC_RUNTIME_STOPPED &&
        prior != APP_MONITOR_STOPPED) {
        s->transition_pending = 0;
        outcome(s, "Machine stopped.\r\n");
    } else if (state == SOFTPC_RUNTIME_ERROR) {
        s->transition_pending = 0;
        outcome(s, "Machine error.\r\n");
    }
}

void app_command_session_note_broker(app_command_session *s,
    app_monitor_state state, int vm,
    int monitor_running_surface)
{
    if (s == NULL) return;
    if (vm && state == APP_MONITOR_RUNNING) {
        s->prompt_due = 0;
        s->pending_monitor_text[0] = '\0';
    } else if (!vm && state == APP_MONITOR_RUNNING &&
        monitor_running_surface) prompt(s);
}

void app_command_session_note_monitor_current(app_command_session *s,
    int current, app_command_effect *e)
{
    if (s == NULL || e == NULL) return;
    clear(e);
    if (!s->prompt_due || !current) return;
    text(e, s->pending_monitor_text);
    s->pending_monitor_text[0] = '\0';
    s->prompt_due = 0;
    e->arm_prompt = 1;
}
