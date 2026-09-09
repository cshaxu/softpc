#include "presentation.h"

#ifdef _WIN32
#include "keyboard.h"
#include "presentation_plan.h"
#include "reconciler.h"
#include "lib/ux-console/console.h"
#include "lib/ux-window/window.h"

#include <stdlib.h>
#include <string.h>
#include <windows.h>

struct app_presentation {
    app_runtime *runtime;
    softpc_presentation display;
    int console_control;
    ux_window *window;
    ux_console *console;
    int vm_console_current;
    app_monitor_console *monitor;
    app_control_queue *control_queue;
    ux_hotkey_registry hotkeys;
    app_runtime_state displayed_state;
    uint32_t observed_frame_sequence;
    /* Delivery is a property of an individual output object.  A recreated
     * Window/Console must receive the last completed frame even when the VM
     * has not published a newer sequence. */
    uint32_t window_delivered_frame_sequence;
    uint32_t console_delivered_frame_sequence;
    app_runtime_frame observed_frame;
    app_reconciler reducer;
    app_reconciler_action pending_runtime_action;
};

typedef struct app_presentation app_presentation_context;

static void app_presentation_publish_title(app_presentation_context *context)
{
    if (context == NULL || context->window == NULL) return;
    (void)ux_window_set_title(context->window,
        context->displayed_state == SOFTPC_RUNTIME_PAUSED ?
            "Insignia SoftPC (Paused)" : "Insignia SoftPC (Running)");
}

static int app_presentation_guest_input(void *opaque, const ux_event *event)
{
    app_presentation_context *context = (app_presentation_context *)opaque;
    return context != NULL && app_control_queue_push_ux_for_run(
        context->control_queue, event,
        app_runtime_run_generation(context->runtime));
}

/* One app-owned queue sink for Window and VM-Console events.  The source
 * handle is retained by ux-base for tracing only; action meaning is the
 * registered identifier, never a leaf-specific callback. */
static int app_presentation_input(void *opaque, const ux_input_event *event)
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
        (void)app_control_queue_push_ux_delivery_failed(context->control_queue,
            source_identity, status, app_runtime_run_generation(context->runtime));
}

static int app_presentation_create_window(app_presentation_context *context)
{
    ux_window_options options = { 0 };

    if (context == NULL || context->window != NULL) return context != NULL;
    options.component.input_context = context;
    options.component.input_sink = app_presentation_input;
    options.component.failure_context = context;
    options.component.failure_sink = app_presentation_delivery_failed;
    options.component.hotkeys = context->hotkeys;
    options.initial_title = context->displayed_state == SOFTPC_RUNTIME_PAUSED ?
        "Insignia SoftPC (Paused)" : "Insignia SoftPC (Running)";
    options.initial_frozen = context->displayed_state != SOFTPC_RUNTIME_RUNNING;
    if (ux_window_create(&context->window, &options) != LIB_STATUS_OK)
        return 0;
    context->window_delivered_frame_sequence = 0u;
    app_presentation_publish_title(context);
    if (context->displayed_state == SOFTPC_RUNTIME_RUNNING)
        (void)ux_window_unfreeze(context->window);
    return 1;
}

static int app_presentation_create_console(app_presentation_context *context)
{
    ux_console_options options = { 0 };

    if (context == NULL || context->console != NULL) return context != NULL;
    options.input_context = context;
    options.input_sink = app_presentation_input;
    options.failure_context = context;
    options.failure_sink = app_presentation_delivery_failed;
    options.hotkeys = context->hotkeys;
    if (ux_console_create(&context->console, &options) != LIB_STATUS_OK)
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
    context->vm_console_current = 0;
    if (context->window != NULL)
        ux_window_destroy(context->window);
    if (context->console != NULL)
        ux_console_destroy(context->console);
    context->window = NULL;
    context->console = NULL;
    context->window_delivered_frame_sequence = 0u;
    context->console_delivered_frame_sequence = 0u;
}

static int app_presentation_apply_next_action(app_presentation_context *context)
{
    app_reconciler_action action;
    if (context == NULL) return 0;
    action = app_reconciler_take_action(&context->reducer);
    switch (action) {
    case APP_RECONCILER_ACTION_NONE:
        return 1;
    case APP_RECONCILER_ACTION_RUNTIME_START:
    case APP_RECONCILER_ACTION_RUNTIME_PAUSE:
    case APP_RECONCILER_ACTION_RUNTIME_RESUME:
    case APP_RECONCILER_ACTION_RUNTIME_STOP:
        context->pending_runtime_action = action;
        return 1;
    case APP_RECONCILER_ACTION_CREATE_WINDOW:
        if (!app_presentation_create_window(context)) return 0;
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
        context->vm_console_current = 1;
        return app_control_queue_push_broker_completed(context->control_queue, 1,
            app_runtime_run_generation(context->runtime));
    case APP_RECONCILER_ACTION_BIND_MONITOR:
        if (context->console == NULL) return 0;
        if (!app_monitor_console_activate_self(context->monitor,
                context->console)) {
            return 0;
        }
        context->vm_console_current = 0;
        return app_control_queue_push_broker_completed(context->control_queue, 0,
            app_runtime_run_generation(context->runtime));
    case APP_RECONCILER_ACTION_DESTROY_VM_CONSOLE:
        ux_console_destroy(context->console);
        context->console = NULL;
        context->console_delivered_frame_sequence = 0u;
        context->vm_console_current = 0;
        return app_control_queue_push_component_completed(context->control_queue,
            APP_CONTROL_COMPONENT_VM_CONSOLE, 0,
            app_runtime_run_generation(context->runtime));
    case APP_RECONCILER_ACTION_DESTROY_WINDOW:
        ux_window_destroy(context->window);
        context->window = NULL;
        context->window_delivered_frame_sequence = 0u;
        return app_control_queue_push_component_completed(context->control_queue,
            APP_CONTROL_COMPONENT_WINDOW, 0,
            app_runtime_run_generation(context->runtime));
    }
    return 0;
}

static int app_presentation_publish(app_presentation_context *context,
    const ux_frame *frame)
{
    ux_frame console_status;
    const ux_frame *console_frame = frame;
    if (context == NULL || frame == NULL) return 0;
    /* A graphics frame has no console rendering contract.  In the one mode
     * where the VM raw Console remains active beside Window, SoftPC provides
     * the product status surface rather than leaving stale guest text. */
    if (context->console != NULL && frame->graphics != 0u &&
        context->display == SOFTPC_PRESENTATION_CONSOLE &&
        !context->console_control) {
        static const char message[] =
            "Insignia SoftPC is running in the Window.\r\n"
            "Raw VM Console hotkeys: Ctrl+Alt+P/D/F/M\r\n";
        size_t index, row = 0u, column = 0u;
        memset(&console_status, 0, sizeof(console_status));
        console_status.valid = 1u;
        console_status.sequence = frame->sequence;
        console_status.text_columns = UX_TEXT_COLUMNS;
        console_status.text_rows = UX_TEXT_ROWS;
        console_status.cursor_column = -1;
        console_status.cursor_row = -1;
        for (index = 0u; index < sizeof(console_status.text); ++index) {
            console_status.text[index] = ' ';
            console_status.attributes[index] = 0x07u;
        }
        for (index = 0u; index < sizeof(message) - 1u; ++index) {
            if (message[index] == '\r') continue;
            if (message[index] == '\n') { ++row; column = 0u; continue; }
            if (row < UX_TEXT_ROWS && column < UX_TEXT_COLUMNS)
                console_status.text[row * UX_TEXT_COLUMNS + column] =
                    (lib_u8)message[index];
            ++column;
        }
        console_frame = &console_status;
    }
    if (context->reducer.vm_console_actual &&
        context->reducer.current_console_actual == APP_RECONCILER_CONSOLE_VM &&
        context->console != NULL &&
        context->console_delivered_frame_sequence != frame->sequence) {
        if (ux_console_publish_frame(context->console, console_frame) !=
            LIB_STATUS_OK) return 0;
        context->console_delivered_frame_sequence = frame->sequence;
    }
    if (context->reducer.window_actual && context->window != NULL &&
        context->window_delivered_frame_sequence != frame->sequence) {
        if (ux_window_publish_frame(context->window, frame) != LIB_STATUS_OK)
            return 0;
        context->window_delivered_frame_sequence = frame->sequence;
    }
    return 1;
}

static int app_presentation_frame_targets_ready(
    const app_presentation_context *context)
{
    app_presentation_plan desired;

    if (context == NULL) return 0;
    desired = app_reconciler_desired(&context->reducer);
    if (!desired.window_enabled && !desired.vm_console_enabled) return 0;
    if (desired.window_enabled && !context->reducer.window_actual) return 0;
    if (desired.vm_console_enabled &&
        (!context->reducer.vm_console_actual ||
         context->reducer.current_console_actual != APP_RECONCILER_CONSOLE_VM))
        return 0;
    return 1;
}

int app_presentation_create(app_presentation **out_presentation,
    app_runtime *runtime, softpc_presentation presentation, int console_control,
    app_monitor_console *monitor, app_control_queue *control_queue)
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
    context->display = presentation;
    context->console_control = console_control;
    context->monitor = monitor;
    context->control_queue = control_queue;
    context->displayed_state = SOFTPC_RUNTIME_STOPPED;
    app_reconciler_initialize(&context->reducer, presentation, console_control);
    *out_presentation = context;
    return 1;
}

void app_presentation_destroy(app_presentation *presentation)
{
    if (presentation == NULL) return;
    app_presentation_destroy_components(presentation);
    free(presentation);
}

void app_presentation_note_runtime_completed(app_presentation *presentation,
    app_runtime_state state)
{
    if (presentation == NULL) return;
    presentation->displayed_state = state;
    app_presentation_publish_title(presentation);
    if (presentation->window != NULL) {
        if (state == SOFTPC_RUNTIME_RUNNING)
            (void)ux_window_unfreeze(presentation->window);
        else if (state == SOFTPC_RUNTIME_PAUSED)
            (void)ux_window_freeze(presentation->window);
    }
    app_reconciler_note_runtime(&presentation->reducer, state);
}

void app_presentation_note_frame_completed(app_presentation *presentation,
    uint32_t sequence, int graphics)
{
    uint32_t run_generation;
    if (presentation == NULL || sequence == 0u ||
        sequence <= presentation->observed_frame_sequence) return;
    if (!app_runtime_copy_published_frame(presentation->runtime,
            &presentation->observed_frame, &run_generation) ||
        presentation->observed_frame.sequence != sequence) return;
    (void)run_generation;
    presentation->observed_frame_sequence = sequence;
    app_reconciler_note_frame(&presentation->reducer, graphics);
}

void app_presentation_note_component_completed(app_presentation *presentation,
    app_control_component_kind component, int exists)
{
    if (presentation == NULL) return;
    if (component == APP_CONTROL_COMPONENT_WINDOW)
        app_reconciler_note_window(&presentation->reducer, exists);
    else
        app_reconciler_note_vm_console(&presentation->reducer, exists);
}

void app_presentation_note_broker_completed(app_presentation *presentation,
    int vm_console_current)
{
    if (presentation != NULL) app_reconciler_note_current_console(
        &presentation->reducer, vm_console_current ? APP_RECONCILER_CONSOLE_VM :
        APP_RECONCILER_CONSOLE_MONITOR);
}

void app_presentation_request_intent(app_presentation *presentation,
    app_reconciler_intent intent)
{
    if (presentation != NULL)
        app_reconciler_note_intent(&presentation->reducer, intent);
}

app_reconciler_action app_presentation_take_runtime_action(
    app_presentation *presentation)
{
    app_reconciler_action action;
    if (presentation == NULL) return APP_RECONCILER_ACTION_NONE;
    action = presentation->pending_runtime_action;
    presentation->pending_runtime_action = APP_RECONCILER_ACTION_NONE;
    return action;
}

void app_presentation_release_window_mouse(app_presentation *presentation)
{
    if (presentation != NULL && presentation->window != NULL)
        (void)ux_window_release_mouse(presentation->window);
}

int app_presentation_reconcile(app_presentation *context)
{
    if (context == NULL) return 0;
    /* Component and broker actual state only arrives through their completion
     * records.  A non-NULL local handle means merely that a request was
     * issued; treating it as actual here reintroduces a second control path. */
    if (!app_presentation_apply_next_action(context)) return 0;
    if (context->observed_frame_sequence != 0u &&
        app_presentation_frame_targets_ready(context) &&
        !app_presentation_publish(context, &context->observed_frame)) return 0;
    return 1;
}

#endif
