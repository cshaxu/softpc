#include "control_state.h"

#include <assert.h>

static void complete_window(app_control_state *state)
{
    assert(app_control_state_take_action(state) ==
        APP_RECONCILER_ACTION_CREATE_WINDOW);
    /* A completion barrier prevents duplicate create work. */
    assert(app_control_state_take_action(state) == APP_RECONCILER_ACTION_NONE);
    app_control_state_note_window(state, 1);
}

static void complete_vm_console(app_control_state *state)
{
    assert(app_control_state_take_action(state) ==
        APP_RECONCILER_ACTION_CREATE_VM_CONSOLE);
    app_control_state_note_vm_console(state, 1);
    assert(app_control_state_take_action(state) ==
        APP_RECONCILER_ACTION_BIND_VM_CONSOLE);
    app_control_state_note_current_console(state, 1);
}

static void test_console_text_to_graphics_monitor(void)
{
    app_control_state state;
    app_control_state_initialize(&state, SOFTPC_PRESENTATION_CONSOLE, 1);
    assert(state.monitor_actual == APP_MONITOR_INIT);
    app_control_state_note_runtime(&state, SOFTPC_RUNTIME_RUNNING);
    assert(state.monitor_actual == APP_MONITOR_RUNNING);
    assert(app_control_state_take_action(&state) == APP_RECONCILER_ACTION_NONE);

    assert(app_control_state_note_frame(&state, 1u, 0));
    assert(!app_control_state_note_frame(&state, 1u, 1));
    complete_vm_console(&state);
    assert(!app_control_state_monitor_is_current(&state));
    assert(app_control_state_frame_targets_ready(&state));

    assert(app_control_state_note_frame(&state, 2u, 1));
    assert(app_control_state_take_action(&state) ==
        APP_RECONCILER_ACTION_BIND_MONITOR);
    app_control_state_note_current_console(&state, 0);
    assert(app_control_state_take_action(&state) ==
        APP_RECONCILER_ACTION_DESTROY_VM_CONSOLE);
    app_control_state_note_vm_console(&state, 0);
    complete_window(&state);
    assert(app_control_state_monitor_is_current(&state));
    assert(app_control_state_monitor_is_running_graphics_surface(&state));
}

static void test_console_graphics_vm_console(void)
{
    app_control_state state;
    app_control_state_initialize(&state, SOFTPC_PRESENTATION_CONSOLE, 0);
    app_control_state_note_runtime(&state, SOFTPC_RUNTIME_RUNNING);
    assert(app_control_state_note_frame(&state, 1u, 1));
    complete_vm_console(&state);
    complete_window(&state);
    assert(!app_control_state_monitor_is_current(&state));
    assert(app_control_state_frame_targets_ready(&state));
}

static void test_static_window_pause_stop_and_close(void)
{
    app_control_state state;
    app_control_state_initialize(&state, SOFTPC_PRESENTATION_WINDOW, 0);
    app_control_state_note_runtime(&state, SOFTPC_RUNTIME_RUNNING);
    complete_window(&state);
    assert(app_control_state_monitor_is_current(&state));
    app_control_state_note_runtime(&state, SOFTPC_RUNTIME_PAUSED);
    assert(state.monitor_actual == APP_MONITOR_PAUSED);
    assert(app_control_state_take_action(&state) == APP_RECONCILER_ACTION_NONE);
    app_control_state_note_window_close(&state);
    assert(app_control_state_take_action(&state) ==
        APP_RECONCILER_ACTION_DESTROY_WINDOW);
    app_control_state_note_window(&state, 0);
    app_control_state_note_runtime(&state, SOFTPC_RUNTIME_STOPPED);
    assert(state.monitor_actual == APP_MONITOR_STOPPED);
    assert(app_control_state_take_action(&state) == APP_RECONCILER_ACTION_NONE);
}

static void test_reset_completion_restores_paused_view(void)
{
    app_control_state state;
    app_control_state_initialize(&state, SOFTPC_PRESENTATION_CONSOLE, 0);
    app_control_state_note_runtime(&state, SOFTPC_RUNTIME_RUNNING);
    assert(app_control_state_note_frame(&state, 1u, 1));
    complete_vm_console(&state);
    complete_window(&state);

    app_control_state_note_runtime(&state, SOFTPC_RUNTIME_RESET_COMPLETED);
    assert(state.monitor_actual == APP_MONITOR_PAUSED);
    assert(app_control_state_take_action(&state) ==
        APP_RECONCILER_ACTION_BIND_MONITOR);
    app_control_state_note_current_console(&state, 0);
    assert(app_control_state_take_action(&state) ==
        APP_RECONCILER_ACTION_DESTROY_VM_CONSOLE);
    app_control_state_note_vm_console(&state, 0);
    assert(app_control_state_take_action(&state) == APP_RECONCILER_ACTION_NONE);

    /* Reset completes paused, retaining the last visible Window.  Resume
       restores the selected running ownership before the VM publishes its
       next frame; it does not synthesize a new route. */
    app_control_state_note_runtime(&state, SOFTPC_RUNTIME_RUNNING);
    assert(!app_control_state_frame_targets_ready(&state));
    assert(app_control_state_take_action(&state) == APP_RECONCILER_ACTION_CREATE_VM_CONSOLE);
    app_control_state_note_vm_console(&state, 1);
    assert(!app_control_state_frame_targets_ready(&state));
    assert(app_control_state_take_action(&state) == APP_RECONCILER_ACTION_BIND_VM_CONSOLE);
    assert(!app_control_state_frame_targets_ready(&state));
    app_control_state_note_current_console(&state, 1);
    assert(app_control_state_frame_targets_ready(&state));
    assert(app_control_state_note_frame(&state, 2u, 0));
    assert(app_control_state_frame_targets_ready(&state));
}

int main(void)
{
    test_console_text_to_graphics_monitor();
    test_console_graphics_vm_console();
    test_static_window_pause_stop_and_close();
    test_reset_completion_restores_paused_view();
    return 0;
}
