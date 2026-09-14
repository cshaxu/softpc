#ifndef APP_COMMAND_BINDING_H
#define APP_COMMAND_BINDING_H

#include "app/command.h"
#include "common/debug/debug_interface.h"
#include "common/session/session_interface.h"

/* The app chooses its CLI. Session continues owning dispatch and Console I/O. */
typedef struct app_command_binding {
    app_command_session session;
    common_machine *machine;
    common_debug *debug;
    lib_bool debug_active;
    common_debug_result debug_completed;
    lib_bool debug_completed_pending;
    char debug_prompt[COMMON_DEBUG_PROMPT_CAPACITY];
} app_command_binding;

lib_status app_command_binding_initialize(app_command_binding *binding,
    common_machine *machine, common_session_display display,
    common_session_command_provider *provider);
void app_command_binding_dispose(app_command_binding *binding);

#endif
