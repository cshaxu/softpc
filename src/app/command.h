#ifndef APP_COMMAND_H
#define APP_COMMAND_H

#include "runtime.h"

#define APP_COMMAND_TEXT_CAPACITY 2048u
#define APP_COMMAND_PATH_CAPACITY 1024u

typedef enum app_monitor_state { APP_MONITOR_INIT, APP_MONITOR_STOPPED, APP_MONITOR_PAUSED, APP_MONITOR_RUNNING } app_monitor_state;
typedef enum app_command_action { APP_COMMAND_ACTION_NONE, APP_COMMAND_ACTION_EJECT_FLOPPY, APP_COMMAND_ACTION_INSERT_FLOPPY } app_command_action;
/* A parsed lifecycle request is control input, not a presentation intent.
 * Only the control loop may consume it and submit it to runtime. */
typedef enum app_lifecycle_request {
    APP_LIFECYCLE_REQUEST_NONE,
    APP_LIFECYCLE_REQUEST_START,
    APP_LIFECYCLE_REQUEST_RESUME,
    APP_LIFECYCLE_REQUEST_PAUSE,
    APP_LIFECYCLE_REQUEST_STOP,
    APP_LIFECYCLE_REQUEST_RESET
} app_lifecycle_request;

/* Product command policy only: no runtime, presenter, broker, or native I/O. */
typedef struct app_command_session {
    softpc_presentation display;
    app_lifecycle_request pending_request;
    int dispatch_pending;
    int transition_pending;
    int prompt_due;
    char pending_monitor_text[APP_COMMAND_TEXT_CAPACITY];
} app_command_session;

typedef struct app_command_effect {
    app_command_action action;
    int exit_requested, arm_prompt;
    char text[APP_COMMAND_TEXT_CAPACITY];
    char path[APP_COMMAND_PATH_CAPACITY];
} app_command_effect;

/* The registered raw-Console hotkey section is product help text shared by
 * monitor `help` and the graphical raw-Console status surface. */
const char *app_command_hotkey_help(void);
void app_command_session_initialize(app_command_session *, softpc_presentation);
void app_command_session_open(app_command_session *, app_command_effect *);
void app_command_session_reject_line(app_command_session *, app_command_effect *);
void app_command_session_submit_line(app_command_session *, app_monitor_state,
    const char *, app_command_effect *);
/* The only monitor lifecycle-request path.  An accepted command is taken
 * exactly once; rejected and local commands have no request. */
app_lifecycle_request app_command_session_take_request(app_command_session *);
/* Registered UI hotkeys are product control input too.  They do not create a
 * monitor-line request, but they reserve the same transition boundary before
 * control dispatches their already-derived runtime command. */
int app_command_session_begin_external(app_command_session *,
    app_monitor_state, app_lifecycle_request);
void app_command_session_complete_floppy(app_command_session *, app_command_action, int, app_command_effect *);
void app_command_session_note_runtime(app_command_session *, app_monitor_state,
    app_runtime_state, app_command_effect *);
void app_command_session_note_broker(app_command_session *, app_monitor_state,
    int vm, int monitor_running_surface);
void app_command_session_note_monitor_current(app_command_session *, int, app_command_effect *);

#endif
