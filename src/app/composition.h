#ifndef APP_COMPOSITION_H
#define APP_COMPOSITION_H

#include "command.h"
#include "config.h"

/* Own entity assembly and teardown; config is borrowed for this blocking run. */
lib_status app_composition_run(const app_startup_config *config);

/* Wire product command and keyboard handling into the Common provider. */
lib_status app_composition_initialize(app_command_context *, common_machine *,
    common_session_display, lib_size snapshot_maximum,
    common_session_command_provider *);

#endif
