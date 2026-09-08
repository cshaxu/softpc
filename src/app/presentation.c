#include "presentation.h"

#ifdef _WIN32
#include "keyboard.h"
#include "lib/host/console.h"
#include "presentation_plan.h"
#include "lib/ux-console/console.h"
#include "lib/ux-window/window.h"

#include <string.h>
#include <windows.h>

typedef struct app_presentation_context {
    app_runtime *runtime;
    ux_window *window;
    ux_console *console;
    app_monitor_console *monitor;
    app_control_queue *control_queue;
    ux_hotkey_registry hotkeys;
    unsigned int text_frames_since_graphics;
    app_runtime_state displayed_state;
} app_presentation_context;

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
    return context != NULL && app_control_queue_push_ux(context->control_queue,
        event);
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

int app_presentation_run(app_runtime *runtime,
    softpc_presentation presentation, int console_control,
    app_monitor_console *monitor, app_control_queue *control_queue)
{
    app_presentation_context context = { 0 };
    app_runtime_frame frame = { 0 };
    uint32_t prior_sequence = 0u;
    int result = SOFTPC_VM_FRONTEND_ERROR;

    if (runtime == NULL || monitor == NULL || control_queue == NULL ||
        !app_keyboard_register_hotkeys(&context.hotkeys))
        return SOFTPC_VM_FRONTEND_ERROR;
    context.runtime = runtime;
    context.monitor = monitor;
    context.control_queue = control_queue;
    context.displayed_state = app_runtime_get_state(runtime);
    for (;;) {
        app_runtime_state state = app_runtime_get_state(runtime);
        app_presentation_plan plan;
        uint32_t sequence = app_runtime_published_frame_sequence(runtime);
        if (state != context.displayed_state) {
            context.displayed_state = state;
            app_presentation_publish_title(&context);
            if (context.window != NULL) {
                if (state == SOFTPC_RUNTIME_RUNNING)
                    (void)ux_window_enable_mouse(context.window);
                else if (state == SOFTPC_RUNTIME_PAUSED)
                    (void)ux_window_disable_mouse(context.window);
            }
        }
        if (app_runtime_take_window_mouse_release(runtime) && context.window != NULL)
            (void)ux_window_release_mouse(context.window);
        if (app_runtime_take_window_close(runtime) && state == SOFTPC_RUNTIME_PAUSED)
            break;
        if (state == SOFTPC_RUNTIME_ERROR || state == SOFTPC_RUNTIME_STOPPED)
            break;
        if (sequence != prior_sequence && app_runtime_copy_frame(runtime, &frame)) {
            prior_sequence = sequence;
        }
        plan = app_presentation_derive(presentation, console_control, state,
            frame.valid != 0u && frame.graphics != 0u);
        if (!app_presentation_apply_plan(&context, &plan)) goto done;
        if (frame.valid != 0u && !app_presentation_publish(&context, &frame)) goto done;
        if (state == SOFTPC_RUNTIME_PAUSED) {
            Sleep(5u);
            continue;
        }
        Sleep(5u);
    }
    result = app_runtime_get_state(runtime) == SOFTPC_RUNTIME_PAUSED ?
        SOFTPC_VM_FRONTEND_PAUSED : SOFTPC_VM_FRONTEND_STOPPED;
done:
    app_presentation_destroy_components(&context);
    return result;
}
#endif
