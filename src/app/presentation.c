#include "presentation.h"

#ifdef _WIN32
#include "keyboard.h"
#include "lib/host/console.h"
#include "presentation_plan.h"
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
    app_monitor_console *monitor;
    app_control_queue *control_queue;
    ux_hotkey_registry hotkeys;
    unsigned int text_frames_since_graphics;
    app_runtime_state displayed_state;
    int close_requested;
    uint32_t displayed_run_generation;
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
    if (ux_console_create(&context->console, &options) != LIB_STATUS_OK)
        return 0;
    if (host_console_replace_active(app_monitor_console_broker(context->monitor),
            app_monitor_console_object(context->monitor),
            ux_console_get_console(context->console), HOST_CONSOLE_RAW_EVENTS) !=
            LIB_STATUS_OK) {
        ux_component_destroy(ux_console_component(context->console));
        context->console = NULL;
        return 0;
    }
    return 1;
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
    if (context->window != NULL)
        ux_component_destroy(ux_window_component(context->window));
    if (context->console != NULL)
        ux_component_destroy(ux_console_component(context->console));
    context->window = NULL;
    context->console = NULL;
}

static int app_presentation_apply_plan(app_presentation_context *context,
    const app_presentation_plan *plan)
{
    if (context == NULL || plan == NULL) return 0;
    if (plan->window_enabled && !app_presentation_create_window(context)) return 0;
    if (plan->vm_console_enabled && context->console == NULL &&
        !app_presentation_create_console(context)) return 0;
    if (!plan->vm_console_enabled && context->console != NULL) {
        if (host_console_replace_active(app_monitor_console_broker(context->monitor),
                ux_console_get_console(context->console),
                app_monitor_console_object(context->monitor),
                HOST_CONSOLE_COOKED_LINES) != LIB_STATUS_OK) return 0;
        ux_component_destroy(ux_console_component(context->console));
        context->console = NULL;
    }
    if (!plan->window_enabled && context->window != NULL) {
        ux_component_destroy(ux_window_component(context->window));
        context->window = NULL;
    }
    return 1;
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
    *out_presentation = context;
    return 1;
}

void app_presentation_destroy(app_presentation *presentation)
{
    if (presentation == NULL) return;
    app_presentation_destroy_components(presentation);
    free(presentation);
}

int app_presentation_prepare_resume(app_presentation *presentation)
{
    app_runtime_frame frame = { 0 };
    app_presentation_plan plan;
    int graphics = 0;

    if (presentation == NULL) return 0;
    if (app_runtime_copy_frame(presentation->runtime, &frame))
        graphics = frame.graphics != 0u;
    plan = app_presentation_derive(presentation->display,
        presentation->console_control, SOFTPC_RUNTIME_RUNNING, graphics);
    if (!app_presentation_apply_plan(presentation, &plan)) return 0;
    presentation->close_requested = 0;
    return 1;
}

int app_presentation_reconcile(app_presentation *context)
{
    app_runtime_frame frame = { 0 };
    app_runtime_state state;
    app_presentation_plan plan;
    uint32_t sequence;

    if (context == NULL) return 0;
    state = app_runtime_get_state(context->runtime);
    if (context->displayed_run_generation !=
        app_runtime_run_generation(context->runtime)) {
        context->displayed_run_generation = app_runtime_run_generation(context->runtime);
        context->close_requested = 0;
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
    if (app_runtime_take_window_close(context->runtime)) context->close_requested = 1;
    sequence = app_runtime_published_frame_sequence(context->runtime);
    if (sequence != 0u && app_runtime_published_frame_run_generation(
            context->runtime) == context->displayed_run_generation)
        (void)app_runtime_copy_frame(context->runtime, &frame);
    plan = app_presentation_derive(context->display, context->console_control,
        state, frame.valid != 0u && frame.graphics != 0u);
    if (context->close_requested && state == SOFTPC_RUNTIME_PAUSED)
        plan.window_enabled = 0;
    if (!app_presentation_apply_plan(context, &plan)) return 0;
    if (frame.valid != 0u && !app_presentation_publish(context, &frame)) return 0;
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
