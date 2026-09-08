#include "presentation.h"

#ifdef _WIN32
#include "keyboard.h"
#include "lib/host/console.h"
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
    unsigned int text_frames_since_graphics;
    app_runtime_state displayed_state;
    int close_requested;
    uint32_t displayed_run_generation;
    uint32_t observed_frame_sequence;
    app_runtime_frame observed_frame;
    app_reconciler reducer;
};

typedef struct app_presentation app_presentation_context;

static void app_presentation_publish_title(app_presentation_context *context)
{
    if (context == NULL || context->window == NULL) return;
    (void)ux_window_set_title(context->window,
        app_runtime_get_state(context->runtime) == SOFTPC_RUNTIME_PAUSED ?
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

static int app_presentation_create_window(app_presentation_context *context)
{
    ux_window_options options = { 0 };

    if (context == NULL || context->window != NULL) return context != NULL;
    options.input_context = context;
    options.input_sink = app_presentation_input;
    options.hotkeys = context->hotkeys;
    if (ux_window_create(&context->window, &options) != LIB_STATUS_OK)
        return 0;
    app_presentation_publish_title(context);
    if (app_runtime_get_state(context->runtime) == SOFTPC_RUNTIME_RUNNING)
        (void)ux_window_enable_mouse(context->window);
    return 1;
}

static int app_presentation_create_console(app_presentation_context *context)
{
    ux_console_options options = { 0 };

    if (context == NULL || context->console != NULL) return context != NULL;
    options.input_context = context;
    options.input_sink = app_presentation_input;
    options.hotkeys = context->hotkeys;
    return ux_console_create(&context->console, &options) == LIB_STATUS_OK;
}

static void app_presentation_destroy_components(app_presentation_context *context)
{
    if (context == NULL) return;
    /* Replace the VM object before destroying it. Host retains one Current
     * Console for its entire life; presentation merely changes the object. */
    if (context->console != NULL)
        (void)host_console_replace_active(app_monitor_console_broker(context->monitor),
            ux_console_get_console(context->console),
            app_monitor_console_object(context->monitor), HOST_CONSOLE_COOKED_LINES);
    context->vm_console_current = 0;
    if (context->window != NULL)
        ux_component_destroy(ux_window_component(context->window));
    if (context->console != NULL)
        ux_component_destroy(ux_console_component(context->console));
    context->window = NULL;
    context->console = NULL;
}

static int app_presentation_apply_next_action(app_presentation_context *context)
{
    app_reconciler_action action;
    if (context == NULL) return 0;
    action = app_reconciler_take_action(&context->reducer);
    switch (action) {
    case APP_RECONCILER_ACTION_NONE:
    case APP_RECONCILER_ACTION_RUNTIME_START:
    case APP_RECONCILER_ACTION_RUNTIME_PAUSE:
    case APP_RECONCILER_ACTION_RUNTIME_RESUME:
    case APP_RECONCILER_ACTION_RUNTIME_STOP:
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
        if (host_console_replace_active(
            app_monitor_console_broker(context->monitor),
            app_monitor_console_object(context->monitor),
            ux_console_get_console(context->console), HOST_CONSOLE_RAW_EVENTS) !=
            LIB_STATUS_OK) {
            return 0;
        }
        context->vm_console_current = 1;
        return app_control_queue_push_broker_completed(context->control_queue, 1,
            app_runtime_run_generation(context->runtime));
    case APP_RECONCILER_ACTION_BIND_MONITOR:
        if (context->console == NULL) return 0;
        if (host_console_replace_active(
            app_monitor_console_broker(context->monitor),
            ux_console_get_console(context->console),
            app_monitor_console_object(context->monitor), HOST_CONSOLE_COOKED_LINES) !=
            LIB_STATUS_OK) {
            return 0;
        }
        context->vm_console_current = 0;
        return app_control_queue_push_broker_completed(context->control_queue, 0,
            app_runtime_run_generation(context->runtime));
    case APP_RECONCILER_ACTION_DESTROY_VM_CONSOLE:
        ux_component_destroy(ux_console_component(context->console));
        context->console = NULL;
        context->vm_console_current = 0;
        return app_control_queue_push_component_completed(context->control_queue,
            APP_CONTROL_COMPONENT_VM_CONSOLE, 0,
            app_runtime_run_generation(context->runtime));
    case APP_RECONCILER_ACTION_DESTROY_WINDOW:
        ux_component_destroy(ux_window_component(context->window));
        context->window = NULL;
        return app_control_queue_push_component_completed(context->control_queue,
            APP_CONTROL_COMPONENT_WINDOW, 0,
            app_runtime_run_generation(context->runtime));
    }
    return 0;
}

static int app_presentation_publish(app_presentation_context *context,
    const ux_frame *frame)
{
    if (context == NULL || frame == NULL) return 0;
    if (context->console != NULL &&
        ux_component_publish_frame(ux_console_component(context->console), frame) !=
            LIB_STATUS_OK) return 0;
    if (context->window != NULL &&
        ux_component_publish_frame(ux_window_component(context->window), frame) !=
            LIB_STATUS_OK) return 0;
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
    if (!app_keyboard_register_hotkeys(&context->hotkeys)) {
        free(context);
        return 0;
    }
    context->runtime = runtime;
    context->display = presentation;
    context->console_control = console_control;
    context->monitor = monitor;
    context->control_queue = control_queue;
    context->displayed_state = app_runtime_get_state(runtime);
    context->displayed_run_generation = app_runtime_run_generation(runtime);
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
{ if (presentation != NULL) app_reconciler_note_runtime(&presentation->reducer, state); }

void app_presentation_note_frame_completed(app_presentation *presentation,
    int graphics)
{ if (presentation != NULL) app_reconciler_note_frame(&presentation->reducer, graphics); }

void app_presentation_note_component_completed(app_presentation *presentation,
    app_control_component_kind component, int exists)
{
    if (presentation == NULL) return;
    if (component == APP_CONTROL_COMPONENT_WINDOW)
        app_reconciler_note_window(&presentation->reducer, exists);
    else app_reconciler_note_vm_console(&presentation->reducer, exists);
}

void app_presentation_note_broker_completed(app_presentation *presentation,
    int vm_console_current)
{
    if (presentation != NULL) app_reconciler_note_current_console(
        &presentation->reducer, vm_console_current ? APP_RECONCILER_CONSOLE_VM :
        APP_RECONCILER_CONSOLE_MONITOR);
}

int app_presentation_prepare_resume(app_presentation *presentation)
{
    app_runtime_frame frame = { 0 };
    int graphics = 0;

    if (presentation == NULL) return 0;
    if (app_runtime_copy_frame(presentation->runtime, &frame))
        graphics = frame.graphics != 0u;
    app_reconciler_note_frame(&presentation->reducer, graphics);
    app_reconciler_note_runtime(&presentation->reducer, SOFTPC_RUNTIME_RUNNING);
    app_reconciler_note_intent(&presentation->reducer,
        APP_RECONCILER_INTENT_RESUME);
    /* Resume is the one product transition that must establish its required
     * presenter set and Current Console before the VM is allowed to run. */
    for (;;) {
        app_reconciler_action action;
        app_reconciler_note_window(&presentation->reducer,
            presentation->window != NULL);
        app_reconciler_note_vm_console(&presentation->reducer,
            presentation->console != NULL);
        app_reconciler_note_current_console(&presentation->reducer,
            presentation->vm_console_current ? APP_RECONCILER_CONSOLE_VM :
            APP_RECONCILER_CONSOLE_MONITOR);
        action = app_reconciler_next_action(&presentation->reducer);
        if (action == APP_RECONCILER_ACTION_RUNTIME_RESUME ||
            action == APP_RECONCILER_ACTION_NONE) break;
        if (!app_presentation_apply_next_action(presentation)) return 0;
    }
    presentation->close_requested = 0;
    return 1;
}

int app_presentation_reconcile(app_presentation *context)
{
    app_runtime_state state;
    uint32_t sequence;
    int frame_changed = 0;

    if (context == NULL) return 0;
    state = app_runtime_get_state(context->runtime);
    if (context->displayed_run_generation !=
        app_runtime_run_generation(context->runtime)) {
        context->displayed_run_generation = app_runtime_run_generation(context->runtime);
        context->close_requested = 0;
        context->observed_frame_sequence = 0u;
        memset(&context->observed_frame, 0, sizeof(context->observed_frame));
    }
    if (state != context->displayed_state) {
        context->displayed_state = state;
        app_presentation_publish_title(context);
        if (context->window != NULL) {
                if (state == SOFTPC_RUNTIME_RUNNING)
                    (void)ux_window_enable_mouse(context->window);
                else if (state == SOFTPC_RUNTIME_PAUSED)
                    (void)ux_window_disable_mouse(context->window);
        }
    }
    if (app_runtime_take_window_mouse_release(context->runtime) &&
        context->window != NULL)
        (void)ux_window_release_mouse(context->window);
    if (app_runtime_take_window_close(context->runtime)) {
        context->close_requested = 1;
        app_reconciler_note_intent(&context->reducer,
            APP_RECONCILER_INTENT_WINDOW_CLOSE);
    }
    sequence = app_runtime_published_frame_sequence(context->runtime);
    if (sequence != 0u && sequence != context->observed_frame_sequence &&
        app_runtime_copy_published_frame(context->runtime,
            &context->observed_frame, &sequence) &&
        sequence == context->displayed_run_generation) {
        context->observed_frame_sequence = context->observed_frame.sequence;
        frame_changed = 1;
    }
    /* Component and broker actual state only arrives through their completion
     * records.  A non-NULL local handle means merely that a request was
     * issued; treating it as actual here reintroduces a second control path. */
    if (!app_presentation_apply_next_action(context)) return 0;
    if (frame_changed && !app_presentation_publish(context,
            &context->observed_frame)) return 0;
    return 1;
}

#ifdef SOFTPC_WINDOW_TESTING
int app_presentation_run(app_runtime *runtime,
    softpc_presentation presentation, int console_control,
    app_monitor_console *monitor, app_control_queue *control_queue)
{
    app_presentation *component = NULL;
    int result = SOFTPC_VM_FRONTEND_ERROR;
    if (!app_presentation_create(&component, runtime, presentation,
            console_control, monitor, control_queue)) return result;
    for (;;) {
        if (!app_presentation_reconcile(component)) break;
        if (app_runtime_get_state(runtime) == SOFTPC_RUNTIME_ERROR ||
            app_runtime_get_state(runtime) == SOFTPC_RUNTIME_STOPPED ||
            (component->close_requested &&
             app_runtime_get_state(runtime) == SOFTPC_RUNTIME_PAUSED)) {
            result = app_runtime_get_state(runtime) == SOFTPC_RUNTIME_PAUSED ?
                SOFTPC_VM_FRONTEND_PAUSED : SOFTPC_VM_FRONTEND_STOPPED;
            break;
        }
        Sleep(5u);
    }
    app_presentation_destroy(component);
    return result;
}
#endif
#endif
