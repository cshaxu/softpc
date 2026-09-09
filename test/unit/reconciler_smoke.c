#include "reconciler.h"

#include <assert.h>

int main(void)
{
    app_reconciler state;

    app_reconciler_initialize(&state, SOFTPC_PRESENTATION_CONSOLE, 1);
    assert(app_reconciler_next_action(&state) == APP_RECONCILER_ACTION_NONE);

    app_reconciler_note_intent(&state, APP_RECONCILER_INTENT_START);
    assert(app_reconciler_next_action(&state) == APP_RECONCILER_ACTION_RUNTIME_START);
    assert(app_reconciler_take_action(&state) == APP_RECONCILER_ACTION_RUNTIME_START);
    /* A dispatched effect remains only desired until its matching completion
       makes it actual.  It must not be emitted twice. */
    assert(app_reconciler_next_action(&state) == APP_RECONCILER_ACTION_NONE);
    app_reconciler_note_runtime(&state, SOFTPC_RUNTIME_RUNNING);
    /* Runtime RUNNING alone must not steal the cooked monitor: raw Console
       ownership begins only when a completed VM frame is observed. */
    assert(app_reconciler_next_action(&state) == APP_RECONCILER_ACTION_NONE);
    app_reconciler_note_frame(&state, 1);
    assert(app_reconciler_next_action(&state) == APP_RECONCILER_ACTION_CREATE_WINDOW);
    app_reconciler_note_window(&state, 1);
    assert(app_reconciler_next_action(&state) == APP_RECONCILER_ACTION_NONE);

    app_reconciler_note_intent(&state, APP_RECONCILER_INTENT_PAUSE);
    assert(app_reconciler_next_action(&state) == APP_RECONCILER_ACTION_RUNTIME_PAUSE);
    app_reconciler_note_runtime(&state, SOFTPC_RUNTIME_PAUSED);
    assert(app_reconciler_next_action(&state) == APP_RECONCILER_ACTION_NONE);

    app_reconciler_note_intent(&state, APP_RECONCILER_INTENT_RESUME);
    assert(app_reconciler_next_action(&state) == APP_RECONCILER_ACTION_RUNTIME_RESUME);
    app_reconciler_note_runtime(&state, SOFTPC_RUNTIME_RUNNING);

    app_reconciler_note_intent(&state, APP_RECONCILER_INTENT_WINDOW_CLOSE);
    assert(app_reconciler_next_action(&state) == APP_RECONCILER_ACTION_RUNTIME_PAUSE);
    app_reconciler_note_runtime(&state, SOFTPC_RUNTIME_PAUSED);
    assert(app_reconciler_next_action(&state) == APP_RECONCILER_ACTION_DESTROY_WINDOW);
    app_reconciler_note_window(&state, 0);
    app_reconciler_note_intent(&state, APP_RECONCILER_INTENT_STOP);
    assert(app_reconciler_next_action(&state) == APP_RECONCILER_ACTION_RUNTIME_STOP);
    app_reconciler_note_runtime(&state, SOFTPC_RUNTIME_STOPPED);
    assert(app_reconciler_next_action(&state) == APP_RECONCILER_ACTION_NONE);

    /* Console graphics with console_control=0 needs three completed actions:
       Window, VM Console object, then Current Console binding. */
    app_reconciler_initialize(&state, SOFTPC_PRESENTATION_CONSOLE, 0);
    app_reconciler_note_intent(&state, APP_RECONCILER_INTENT_START);
    app_reconciler_note_runtime(&state, SOFTPC_RUNTIME_RUNNING);
    app_reconciler_note_frame(&state, 1);
    assert(app_reconciler_next_action(&state) == APP_RECONCILER_ACTION_CREATE_WINDOW);
    app_reconciler_note_window(&state, 1);
    assert(app_reconciler_next_action(&state) == APP_RECONCILER_ACTION_CREATE_VM_CONSOLE);
    app_reconciler_note_vm_console(&state, 1);
    assert(app_reconciler_next_action(&state) == APP_RECONCILER_ACTION_BIND_VM_CONSOLE);
    app_reconciler_note_current_console(&state, APP_RECONCILER_CONSOLE_VM);
    assert(app_reconciler_next_action(&state) == APP_RECONCILER_ACTION_NONE);

    /* This is the real Win3.1 transition, not a fresh graphics startup:
       first text has created and bound raw VM Console; then graphics with
       console_control=1 must return native Console to the monitor before
       retiring the VM Console object. */
    app_reconciler_initialize(&state, SOFTPC_PRESENTATION_CONSOLE, 1);
    app_reconciler_note_intent(&state, APP_RECONCILER_INTENT_START);
    app_reconciler_note_runtime(&state, SOFTPC_RUNTIME_RUNNING);
    app_reconciler_note_frame(&state, 0);
    assert(app_reconciler_take_action(&state) ==
        APP_RECONCILER_ACTION_CREATE_VM_CONSOLE);
    app_reconciler_note_vm_console(&state, 1);
    assert(app_reconciler_take_action(&state) ==
        APP_RECONCILER_ACTION_BIND_VM_CONSOLE);
    app_reconciler_note_current_console(&state, APP_RECONCILER_CONSOLE_VM);
    assert(app_reconciler_next_action(&state) == APP_RECONCILER_ACTION_NONE);
    app_reconciler_note_frame(&state, 1);
    assert(app_reconciler_take_action(&state) ==
        APP_RECONCILER_ACTION_CREATE_WINDOW);
    app_reconciler_note_window(&state, 1);
    assert(app_reconciler_take_action(&state) ==
        APP_RECONCILER_ACTION_BIND_MONITOR);
    app_reconciler_note_current_console(&state, APP_RECONCILER_CONSOLE_MONITOR);
    assert(app_reconciler_take_action(&state) ==
        APP_RECONCILER_ACTION_DESTROY_VM_CONSOLE);
    app_reconciler_note_vm_console(&state, 0);
    assert(app_reconciler_next_action(&state) == APP_RECONCILER_ACTION_NONE);

    /* Component facts also clear only their own pending action. */
    app_reconciler_initialize(&state, SOFTPC_PRESENTATION_WINDOW, 1);
    app_reconciler_note_intent(&state, APP_RECONCILER_INTENT_START);
    app_reconciler_note_runtime(&state, SOFTPC_RUNTIME_RUNNING);
    assert(app_reconciler_take_action(&state) == APP_RECONCILER_ACTION_CREATE_WINDOW);
    assert(app_reconciler_next_action(&state) == APP_RECONCILER_ACTION_NONE);
    app_reconciler_note_window(&state, 1);
    assert(app_reconciler_next_action(&state) == APP_RECONCILER_ACTION_NONE);

    /* Reset is a single intent: stop an existing run, cold start, then
       pause only after the new run reports running. */
    app_reconciler_initialize(&state, SOFTPC_PRESENTATION_WINDOW, 1);
    app_reconciler_note_runtime(&state, SOFTPC_RUNTIME_RUNNING);
    app_reconciler_note_intent(&state, APP_RECONCILER_INTENT_RESET);
    assert(app_reconciler_take_action(&state) == APP_RECONCILER_ACTION_RUNTIME_STOP);
    app_reconciler_note_runtime(&state, SOFTPC_RUNTIME_STOPPED);
    assert(app_reconciler_take_action(&state) == APP_RECONCILER_ACTION_RUNTIME_START);
    app_reconciler_note_runtime(&state, SOFTPC_RUNTIME_RUNNING);
    assert(app_reconciler_take_action(&state) == APP_RECONCILER_ACTION_RUNTIME_PAUSE);
    app_reconciler_note_runtime(&state, SOFTPC_RUNTIME_PAUSED);
    assert(app_reconciler_next_action(&state) == APP_RECONCILER_ACTION_CREATE_WINDOW);
    app_reconciler_note_window(&state, 1);
    assert(app_reconciler_next_action(&state) == APP_RECONCILER_ACTION_NONE);
    return 0;
}
