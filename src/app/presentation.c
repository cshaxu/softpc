#include "presentation.h"

#ifdef _WIN32
#include "keyboard.h"
#include "presentation_plan.h"
#include "lib/host/console.h"
#include "lib/ux-console/console.h"
#include "lib/ux-window/window.h"

#include <string.h>
#include <windows.h>

typedef struct app_presentation_context {
    app_runtime *runtime;
    ux_window *window;
    ux_console *console;
    host_console_broker *console_broker;
    ux_hotkey_registry hotkeys;
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
    return context != NULL && app_keyboard_deliver_input(context->runtime, event);
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
    if (host_console_broker_create(&context->console_broker,
            ux_console_get_console(context->console),
            HOST_CONSOLE_RAW_EVENTS) != LIB_STATUS_OK) {
        ux_component_destroy(ux_console_component(context->console));
        context->console = NULL;
        return 0;
    }
    return 1;
}

static void app_presentation_destroy_components(app_presentation_context *context)
{
    if (context == NULL) return;
    /* The broker owns the process Console activation.  Retire it before its
     * borrowed VM Console object is destroyed. */
    host_console_broker_destroy(context->console_broker);
    context->console_broker = NULL;
    if (context->window != NULL)
        ux_component_destroy(ux_window_component(context->window));
    if (context->console != NULL)
        ux_component_destroy(ux_console_component(context->console));
    context->window = NULL;
    context->console = NULL;
}

static void app_presentation_destroy_window(app_presentation_context *context)
{
    if (context != NULL && context->window != NULL) {
        ux_component_destroy(ux_window_component(context->window));
        context->window = NULL;
    }
}

static void app_presentation_destroy_console(app_presentation_context *context)
{
    if (context == NULL) return;
    host_console_broker_destroy(context->console_broker);
    context->console_broker = NULL;
    if (context->console != NULL) {
        ux_component_destroy(ux_console_component(context->console));
        context->console = NULL;
    }
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
    softpc_presentation presentation, int console_control)
{
    app_presentation_context context = { 0 };
    app_runtime_frame frame = { 0 };
    uint32_t prior_sequence = 0u;
    int result = SOFTPC_VM_FRONTEND_ERROR;

    if (runtime == NULL || !app_keyboard_register_hotkeys(&context.hotkeys))
        return SOFTPC_VM_FRONTEND_ERROR;
    context.runtime = runtime;
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
        if (state == SOFTPC_RUNTIME_ERROR || state == SOFTPC_RUNTIME_STOPPED ||
            (state == SOFTPC_RUNTIME_PAUSED &&
             presentation == SOFTPC_PRESENTATION_CONSOLE))
            break;
        if (state == SOFTPC_RUNTIME_PAUSED) {
            Sleep(5u);
            continue;
        }
        if (sequence != prior_sequence && app_runtime_copy_frame(runtime, &frame)) {
            prior_sequence = sequence;
        }
        plan = app_presentation_derive(presentation, console_control, state,
            frame.valid != 0u && frame.graphics != 0u);
        if (plan.window_enabled && !app_presentation_create_window(&context))
            goto done;
        if (!plan.window_enabled) app_presentation_destroy_window(&context);
        if (plan.vm_console_enabled && !app_presentation_create_console(&context))
            goto done;
        if (!plan.vm_console_enabled) app_presentation_destroy_console(&context);
        if (frame.valid != 0u && !app_presentation_publish(&context, &frame))
            goto done;
        Sleep(5u);
    }
    result = app_runtime_get_state(runtime) == SOFTPC_RUNTIME_PAUSED ?
        SOFTPC_VM_FRONTEND_PAUSED : SOFTPC_VM_FRONTEND_STOPPED;
done:
    app_presentation_destroy_components(&context);
    return result;
}
#endif
