#ifndef APP_COMPOSITION_H
#define APP_COMPOSITION_H

#include "command.h"

/* Wire product command and keyboard handling into the Common provider. */
lib_status app_composition_initialize(app_command_context *, common_machine *,
    common_session_display, common_session_command_provider *);

#endif
