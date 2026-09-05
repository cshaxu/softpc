#ifndef WIN32_PRESENTATION_PRESENTER_H
#define WIN32_PRESENTATION_PRESENTER_H

#include "actions.h"
#include "event.h"
#include "mailbox.h"
#include "router.h"

/*
 * The presentation binding is the complete boundary between a reusable
 * Win32 presenter and a product.  Every value crossing it is copied or
 * opaque: presenters do not know an executor, a machine, or a guest input
 * protocol.  The product owns the consequences of an action; the component
 * only recognizes registered chords and forwards normalized host events.
 */
typedef enum win32_presentation_run_state {
    WIN32_PRESENTATION_RUN_STOPPED,
    WIN32_PRESENTATION_RUN_RUNNING,
    WIN32_PRESENTATION_RUN_PAUSED,
    WIN32_PRESENTATION_RUN_ERROR
} win32_presentation_run_state;

typedef enum win32_presentation_run_result {
    WIN32_PRESENTATION_RUN_CONTINUE,
    WIN32_PRESENTATION_RUN_STOPPED_RESULT,
    WIN32_PRESENTATION_RUN_PAUSED_RESULT,
    WIN32_PRESENTATION_RUN_SWITCH_WINDOW,
    WIN32_PRESENTATION_RUN_SWITCH_CONSOLE,
    WIN32_PRESENTATION_RUN_ERROR_RESULT
} win32_presentation_run_result;

typedef struct win32_presentation_binding {
    void *context;
    win32_presentation_mailbox *mailbox;
    win32_presentation_router *router;
    const win32_presentation_action_registry *actions;
    win32_presentation_event_sink input_sink;
    win32_presentation_run_state (*get_state)(void *context);
    win32_presentation_run_result (*handle_action)(void *context,
        win32_presentation_action action,
        win32_presentation_event_sink input_sink);
    win32_presentation_run_result (*handle_close)(void *context,
        win32_presentation_event_sink input_sink);
    void (*get_window_title)(void *context, char *buffer,
        unsigned int buffer_size);
} win32_presentation_binding;

int win32_presentation_binding_valid(
    const win32_presentation_binding *binding);

#ifdef _WIN32
win32_presentation_run_result win32_presentation_run_console(
    const win32_presentation_binding *binding);
win32_presentation_run_result win32_presentation_run_window(
    const win32_presentation_binding *binding);
#endif

#endif
