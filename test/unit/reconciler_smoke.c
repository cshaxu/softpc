#include "reconciler.h"

#include <assert.h>

/* Presentation reducer tests deliberately contain no lifecycle command.  The
 * control reducer owns those requests; this unit proves that completed facts
 * alone produce one-way component/Current-Console work. */
int main(void)
{
    app_reconciler state;

    app_reconciler_initialize(&state, SOFTPC_PRESENTATION_CONSOLE, 1);
    app_reconciler_note_runtime(&state, SOFTPC_RUNTIME_RUNNING);
    app_reconciler_note_frame(&state, 0);
    assert(app_reconciler_take_action(&state) ==
        APP_RECONCILER_ACTION_CREATE_VM_CONSOLE);
    app_reconciler_note_vm_console(&state, 1);
    assert(app_reconciler_take_action(&state) ==
        APP_RECONCILER_ACTION_BIND_VM_CONSOLE);
    app_reconciler_note_current_console(&state, APP_RECONCILER_CONSOLE_VM);
    assert(app_reconciler_next_action(&state) == APP_RECONCILER_ACTION_NONE);

    /* A graphics frame on console_control=1 creates Window, returns Current
       Console to monitor, then retires the VM Console. */
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

    /* Pause is a completed runtime fact.  It retains this graphics Window
       but keeps monitor current; Window close only changes presentation. */
    app_reconciler_note_runtime(&state, SOFTPC_RUNTIME_PAUSED);
    assert(app_reconciler_next_action(&state) == APP_RECONCILER_ACTION_NONE);
    app_reconciler_note_window_close(&state);
    assert(app_reconciler_take_action(&state) ==
        APP_RECONCILER_ACTION_DESTROY_WINDOW);
    app_reconciler_note_window(&state, 0);
    assert(app_reconciler_next_action(&state) == APP_RECONCILER_ACTION_NONE);

    /* Stopped clears the previous route; a later running fact cannot inherit
       an old Window until runtime has supplied a new completed frame. */
    app_reconciler_note_runtime(&state, SOFTPC_RUNTIME_STOPPED);
    app_reconciler_note_runtime(&state, SOFTPC_RUNTIME_RUNNING);
    assert(app_reconciler_next_action(&state) == APP_RECONCILER_ACTION_NONE);
    app_reconciler_note_frame(&state, 0);
    assert(app_reconciler_take_action(&state) ==
        APP_RECONCILER_ACTION_CREATE_VM_CONSOLE);
    return 0;
}
