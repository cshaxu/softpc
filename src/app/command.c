#include "command.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

static const char HELP[] =
    "Insignia SoftPC\r\n===============\r\n"
    "  start                 cold-reset and run the machine\r\n"
    "  resume                continue a paused machine\r\n"
    "  pause                 request machine pause\r\n"
    "  stop                  stop execution\r\n"
    "  reset                 cold-reset and pause at firmware entry\r\n"
    "  floppy insert <image> insert drive A media while stopped/paused\r\n"
    "  floppy eject          eject drive A media while stopped/paused\r\n"
    "  help                  show this help\r\n  exit                  quit\r\n\r\n"
    "While the guest is running in a raw VM Console:\r\n"
    "  Ctrl+Alt+P            pause or resume\r\n"
    "  Ctrl+Alt+D            send Ctrl+Alt+Del to the guest\r\n"
    "  Ctrl+Alt+F            send Alt+Enter to the guest\r\n"
    "  Ctrl+Alt+M            release captured mouse\r\n";

static void clear(app_command_effect *e) { memset(e, 0, sizeof(*e)); }
static void text(app_command_effect *e, const char *s) { (void)snprintf(e->text, sizeof(e->text), "%s", s); }
static void prompt(app_command_session *s) { s->prompt_due = 1; }
static void reject(app_command_session *s, app_command_effect *e, const char *message) { (void)snprintf(e->text, sizeof(e->text), "%s\r\n", message); prompt(s); }
static char *trim(char *s) { char *e; while (*s && isspace((unsigned char)*s)) ++s; e=s+strlen(s); while(e!=s&&isspace((unsigned char)e[-1]))--e; *e=0; return s; }
static void lower(char *s) { while (*s) { *s=(char)tolower((unsigned char)*s); ++s; } }

static void accept(app_command_session *session,
    app_reconciler_intent intent)
{
    session->pending_intent = intent;
    session->dispatch_pending = 1;
}

static void lifecycle(app_command_session *s, const char *c, app_command_effect *e)
{
    if (s->turn_pending || s->dispatch_pending) {
        reject(s, e, "Machine state transition is in progress.");
        return;
    }
    if (!strcmp(c,"start")) {
        if(s->state==APP_MONITOR_INIT||s->state==APP_MONITOR_STOPPED){s->start_requested=1;accept(s,APP_RECONCILER_INTENT_START);}
        else reject(s,e,s->state==APP_MONITOR_PAUSED?"Machine is paused; use resume, reset, or stop.":"Machine is already running; use pause, reset, or stop.");
    } else if (!strcmp(c,"pause")) {
        if(s->state==APP_MONITOR_RUNNING)accept(s,APP_RECONCILER_INTENT_PAUSE);
        else reject(s,e,s->state==APP_MONITOR_PAUSED?"Machine is paused; use resume, reset, or stop.":s->state==APP_MONITOR_INIT?"Machine has not started; use start or reset.":"Machine is stopped; use start or reset.");
    } else if (!strcmp(c,"resume")) {
        if(s->state==APP_MONITOR_PAUSED)accept(s,APP_RECONCILER_INTENT_RESUME);
        else reject(s,e,s->state==APP_MONITOR_RUNNING?"Machine is already running; use pause, reset, or stop.":s->state==APP_MONITOR_INIT?"Machine has not started; use start or reset.":"Machine is stopped; use start or reset.");
    } else if (!strcmp(c,"reset")) { s->reset_requested=1; accept(s,APP_RECONCILER_INTENT_RESET); }
    else if (!strcmp(c,"stop")) {
        if(s->state==APP_MONITOR_RUNNING||s->state==APP_MONITOR_PAUSED){s->stop_requested=1;accept(s,APP_RECONCILER_INTENT_STOP);}
        else reject(s,e,s->state==APP_MONITOR_INIT?"Machine has not started; use start or reset.":"Machine is stopped; use start or reset.");
    } else reject(s,e,"Unknown command.");
}

void app_command_session_initialize(app_command_session *s, softpc_presentation display) { memset(s,0,sizeof(*s));s->display=display;s->state=APP_MONITOR_INIT; }
void app_command_session_open(app_command_session *s, app_command_effect *e) { clear(e); (void)snprintf(e->text,sizeof(e->text),"%s\r\n",HELP);prompt(s); }
void app_command_session_submit_line(app_command_session *s,const char *line,app_command_effect *e)
{
    char b[APP_COMMAND_TEXT_CAPACITY],*c,*a,*p; size_t n; clear(e);s->line_active=0;
    if(!line||(n=strlen(line))>=sizeof(b)){reject(s,e,line?"Command is too long.":"Unknown command.");return;}
    memcpy(b,line,n+1);c=trim(b);a=c;while(*a&&!isspace((unsigned char)*a))++a;if(*a)*a++=0;a=trim(a);lower(c);
    if(!*c){prompt(s);return;} if(!strcmp(c,"help")){(void)snprintf(e->text,sizeof(e->text),"%s\r\n",HELP);prompt(s);return;} if(!strcmp(c,"exit")){e->exit_requested=1;return;}
    if(strcmp(c,"floppy")){lifecycle(s,c,e);return;}
    p=a;while(*p&&!isspace((unsigned char)*p))++p;if(*p)*p++=0;p=trim(p);lower(a);
    if(!strcmp(a,"eject")&&!*p)e->action=APP_COMMAND_ACTION_EJECT_FLOPPY;
    else if(!strcmp(a,"insert")&&*p&&strlen(p)<sizeof(e->path)){e->action=APP_COMMAND_ACTION_INSERT_FLOPPY;memcpy(e->path,p,strlen(p)+1);}
    else reject(s,e,"Usage: floppy insert <image> | eject");
}
app_reconciler_intent app_command_session_take_intent(app_command_session *s)
{
    app_reconciler_intent intent;
    if (s == NULL || !s->dispatch_pending) return APP_RECONCILER_INTENT_NONE;
    intent = s->pending_intent;
    s->pending_intent = APP_RECONCILER_INTENT_NONE;
    s->dispatch_pending = 0;
    s->turn_intent = intent;
    s->turn_pending = intent != APP_RECONCILER_INTENT_NONE;
    return intent;
}
void app_command_session_complete_floppy(app_command_session *s,app_command_action a,int ok,app_command_effect *e)
{ clear(e); text(e,a==APP_COMMAND_ACTION_EJECT_FLOPPY?(ok?"Floppy ejected.\r\n":"Cannot eject floppy.\r\n"):(ok?"Floppy inserted.\r\n":"Cannot insert floppy.\r\n"));prompt(s); }
void app_command_session_note_runtime(app_command_session *s,app_runtime_state state,app_command_effect *e)
{ clear(e);if(state==SOFTPC_RUNTIME_PAUSED&&s->state!=APP_MONITOR_PAUSED){s->state=APP_MONITOR_PAUSED;text(e,s->reset_requested?"Machine reset and paused.\r\n":"Machine paused.\r\n");s->reset_requested=0;prompt(s);if(s->turn_intent==APP_RECONCILER_INTENT_RESET||s->turn_intent==APP_RECONCILER_INTENT_PAUSE)s->turn_pending=0;}else if(state==SOFTPC_RUNTIME_RUNNING){if(s->reset_requested)return;s->state=APP_MONITOR_RUNNING;s->start_requested=0;if(s->display==SOFTPC_PRESENTATION_WINDOW)prompt(s);if(s->turn_intent==APP_RECONCILER_INTENT_START||s->turn_intent==APP_RECONCILER_INTENT_RESUME)s->turn_pending=0;}else if(state==SOFTPC_RUNTIME_STOPPED&&(s->state!=APP_MONITOR_STOPPED||s->stop_requested)){s->state=APP_MONITOR_STOPPED;if(!s->reset_requested&&!s->start_requested){text(e,"Machine stopped.\r\n");prompt(s);}s->stop_requested=0;if(s->turn_intent==APP_RECONCILER_INTENT_STOP)s->turn_pending=0;} }
void app_command_session_note_broker(app_command_session *s,int vm,int monitor_running_surface){if(!vm&&s->state==APP_MONITOR_RUNNING&&monitor_running_surface)prompt(s);}
void app_command_session_note_monitor_current(app_command_session *s,int current,app_command_effect *e){if(s->prompt_due&&current&&!s->line_active){e->arm_prompt=1;s->prompt_due=0;s->line_active=1;}}
app_monitor_state app_command_session_state(const app_command_session *s){return s->state;}
