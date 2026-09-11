#include "presentation.h"

#ifdef _WIN32
#include "command.h"
#include "keyboard.h"
#include "lib/ui-console/console_interface.h"
#include "lib/ui-window/window_interface.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

struct app_presentation {
    app_runtime *runtime;
    ui_window *window;
    ui_console *console;
    app_monitor_console *monitor;
    app_control_queue *control_queue;
    ui_hotkey_registry hotkeys;
    /* Delivery is a property of an individual output object.  A recreated
     * Window/Console must receive the last completed frame even when the VM
     * has not published a newer sequence. */
    uint32_t window_delivered_frame_sequence;
    uint32_t console_delivered_frame_sequence;
};

typedef struct app_presentation app_presentation_context;

static void app_presentation_publish_title(app_presentation_context *context,
    app_runtime_state state)
{
    if (context == NULL || context->window == NULL) return;
    (void)ui_window_set_title(context->window,
        state == SOFTPC_RUNTIME_PAUSED ?
            "Insignia SoftPC (Paused)" : "Insignia SoftPC (Running)");
}

static int app_presentation_guest_input(void *opaque, const ui_event *event)
{
    app_presentation_context *context = (app_presentation_context *)opaque;
    return context != NULL && app_control_queue_push_ui_for_run(
        context->control_queue, event,
        app_runtime_run_generation(context->runtime));
}

/* One app-owned queue sink for Window and VM-Console events.  The source
 * handle is retained by ui-base for tracing only; action meaning is the
 * registered identifier, never a leaf-specific callback. */
static int app_presentation_input(void *opaque, const ui_input_event *event)
{
    app_presentation_context *context = (app_presentation_context *)opaque;
    if (context == NULL || event == NULL) return 0;
    return app_presentation_guest_input(context, event);
}

static void app_presentation_delivery_failed(void *opaque,
    lib_u64 source_identity, lib_status status)
{
    app_presentation_context *context = (app_presentation_context *)opaque;
    if (context != NULL)
        (void)app_control_queue_push_ui_delivery_failed(context->control_queue,
            source_identity, status, app_runtime_run_generation(context->runtime));
}

static int app_presentation_create_window(app_presentation_context *context,
    app_runtime_state state)
{
    ui_window_options options = { 0 };

    if (context == NULL || context->window != NULL) return context != NULL;
    options.component.input_context = context;
    options.component.input_sink = app_presentation_input;
    options.component.failure_context = context;
    options.component.failure_sink = app_presentation_delivery_failed;
    options.component.hotkeys = context->hotkeys;
    options.initial_title = state == SOFTPC_RUNTIME_PAUSED ?
        "Insignia SoftPC (Paused)" : "Insignia SoftPC (Running)";
    options.initial_frozen = state != SOFTPC_RUNTIME_RUNNING;
    if (ui_window_create(&context->window, &options) != LIB_STATUS_OK)
        return 0;
    context->window_delivered_frame_sequence = 0u;
    app_presentation_publish_title(context, state);
    if (state == SOFTPC_RUNTIME_RUNNING)
        (void)ui_window_unfreeze(context->window);
    return 1;
}

static int app_presentation_create_console(app_presentation_context *context)
{
    ui_console_options options = { 0 };

    if (context == NULL || context->console != NULL) return context != NULL;
    options.input_context = context;
    options.input_sink = app_presentation_input;
    options.failure_context = context;
    options.failure_sink = app_presentation_delivery_failed;
    options.hotkeys = context->hotkeys;
    if (ui_console_create(&context->console, &options) != LIB_STATUS_OK)
        return 0;
    context->console_delivered_frame_sequence = 0u;
    return 1;
}

static void app_presentation_destroy_components(app_presentation_context *context)
{
    if (context == NULL) return;
    /* Replace the VM object before destroying it. Host retains one Current
     * Console for its entire life; presentation merely changes the object. */
    if (context->console != NULL)
        (void)app_monitor_console_activate_self(context->monitor,
            context->console);
    if (context->window != NULL)
        ui_window_destroy(context->window);
    if (context->console != NULL)
        ui_console_destroy(context->console);
    context->window = NULL;
    context->console = NULL;
    context->window_delivered_frame_sequence = 0u;
    context->console_delivered_frame_sequence = 0u;
}

int app_presentation_apply_action(app_presentation *presentation,
    app_reconciler_action action, app_runtime_state runtime_actual)
{
    app_presentation_context *context = presentation;
    if (context == NULL) return 0;
    switch (action) {
    case APP_RECONCILER_ACTION_NONE:
        return 1;
    case APP_RECONCILER_ACTION_CREATE_WINDOW:
        if (!app_presentation_create_window(context, runtime_actual)) return 0;
        return app_control_queue_push_component_completed(context->control_queue,
            APP_CONTROL_COMPONENT_WINDOW, 1,
            app_runtime_run_generation(context->runtime));
    case APP_RECONCILER_ACTION_CREATE_VM_CONSOLE:
        if (!app_presentation_create_console(context)) return 0;
        return app_control_queue_push_component_completed(context->control_queue,
            APP_CONTROL_COMPONENT_VM_CONSOLE, 1,
            app_runtime_run_generation(context->runtime));
    case APP_RECONCILER_ACTION_BIND_VM_CONSOLE:
        if (context->console == NULL) return 0;
        if (!app_monitor_console_activate_vm(context->monitor,
                context->console)) {
            return 0;
        }
        return app_control_queue_push_broker_completed(context->control_queue, 1,
            app_runtime_run_generation(context->runtime));
    case APP_RECONCILER_ACTION_BIND_MONITOR:
        if (context->console == NULL) return 0;
        if (!app_monitor_console_activate_self(context->monitor,
                context->console)) {
            return 0;
        }
        return app_control_queue_push_broker_completed(context->control_queue, 0,
            app_runtime_run_generation(context->runtime));
    case APP_RECONCILER_ACTION_DESTROY_VM_CONSOLE:
        ui_console_destroy(context->console);
        context->console = NULL;
        context->console_delivered_frame_sequence = 0u;
        return app_control_queue_push_component_completed(context->control_queue,
            APP_CONTROL_COMPONENT_VM_CONSOLE, 0,
            app_runtime_run_generation(context->runtime));
    case APP_RECONCILER_ACTION_DESTROY_WINDOW:
        ui_window_destroy(context->window);
        context->window = NULL;
        context->window_delivered_frame_sequence = 0u;
        return app_control_queue_push_component_completed(context->control_queue,
            APP_CONTROL_COMPONENT_WINDOW, 0,
            app_runtime_run_generation(context->runtime));
    }
    return 0;
}

int app_presentation_publish_frame(app_presentation *presentation,
    const app_runtime_frame *frame, int window_actual,
    int vm_console_current, int console_status_surface)
{
    app_presentation_context *context = presentation;
    ui_frame console_status;
    const ui_frame *console_frame = frame;
    if (context == NULL || frame == NULL) return 0;
    /* A graphics frame has no console rendering contract.  In the one mode
     * where the VM raw Console remains active beside Window, SoftPC provides
     * the product status surface rather than leaving stale guest text. */
    if (context->console != NULL && frame->graphics != 0u &&
        console_status_surface) {
        static const char status_line[] =
            "Insignia SoftPC is running in the Window.";
        char message[APP_COMMAND_TEXT_CAPACITY];
        size_t index, row = 0u, column = 0u;
        (void)snprintf(message, sizeof(message), "%s\r\n\r\n%s\r\n",
            status_line, app_command_hotkey_help());
        memset(&console_status, 0, sizeof(console_status));
        console_status.valid = 1u;
        console_status.sequence = frame->sequence;
        console_status.text_columns = UI_TEXT_COLUMNS;
        console_status.text_rows = UI_TEXT_ROWS;
        console_status.cursor_column = -1;
        console_status.cursor_row = -1;
        for (index = 0u; index < sizeof(console_status.text); ++index) {
            console_status.text[index] = ' ';
            console_status.attributes[index] = 0x07u;
        }
        for (index = 0u; message[index] != '\0'; ++index) {
            if (message[index] == '\r') continue;
            if (message[index] == '\n') { ++row; column = 0u; continue; }
            if (row < UI_TEXT_ROWS && column < UI_TEXT_COLUMNS)
                console_status.text[row * UI_TEXT_COLUMNS + column] =
                    (lib_u8)message[index];
            ++column;
        }
        console_frame = &console_status;
    }
    if (vm_console_current && context->console != NULL &&
        context->console_delivered_frame_sequence != frame->sequence) {
        if (ui_console_publish_frame(context->console, console_frame) !=
            LIB_STATUS_OK) return 0;
        context->console_delivered_frame_sequence = frame->sequence;
    }
    if (window_actual && context->window != NULL &&
        context->window_delivered_frame_sequence != frame->sequence) {
        if (ui_window_publish_frame(context->window, frame) != LIB_STATUS_OK)
            return 0;
        context->window_delivered_frame_sequence = frame->sequence;
    }
    return 1;
}

int app_presentation_create(app_presentation **out_presentation,
    app_runtime *runtime, app_monitor_console *monitor,
    app_control_queue *control_queue)
{
    app_presentation_context *context;

    if (out_presentation == NULL || runtime == NULL || monitor == NULL ||
        control_queue == NULL) return 0;
    *out_presentation = NULL;
    context = calloc(1u, sizeof(*context));
    if (context == NULL) return 0;
    if (!app_keyboard_hotkeys(&context->hotkeys)) {
        free(context);
        return 0;
    }
    context->runtime = runtime;
    context->monitor = monitor;
    context->control_queue = control_queue;
    *out_presentation = context;
    return 1;
}

void app_presentation_destroy(app_presentation *presentation)
{
    if (presentation == NULL) return;
    app_presentation_destroy_components(presentation);
    free(presentation);
}

void app_presentation_release_window_mouse(app_presentation *presentation)
{
    if (presentation != NULL && presentation->window != NULL)
        (void)ui_window_release_mouse(presentation->window);
}

void app_presentation_set_runtime_state(app_presentation *presentation,
    app_runtime_state state)
{
    if (presentation == NULL) return;
    app_presentation_publish_title(presentation, state);
    if (presentation->window != NULL) {
        if (state == SOFTPC_RUNTIME_RUNNING)
            (void)ui_window_unfreeze(presentation->window);
        else if (state == SOFTPC_RUNTIME_PAUSED)
            (void)ui_window_freeze(presentation->window);
    }
}

#endif
