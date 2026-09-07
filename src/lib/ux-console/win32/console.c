#include "lib/ux-console/internal.h"

#ifdef _WIN32
#include <windows.h>

struct ux_console_native {
    HANDLE wake;
    HANDLE ready;
    HANDLE thread;
};

static lib_bool ux_console_process_controls(ux_console *console)
{
    ux_control_message message;
    lib_bool has_message;

    for (;;) {
        if (ux_console_take_control(console, &message, &has_message) !=
                LIB_STATUS_OK || has_message == LIB_FALSE)
            return LIB_TRUE;
        if (message.kind == UX_CONTROL_STOP)
            return LIB_FALSE;
        /* This component has no public producer for Window-only commands.
         * A shared control FIFO nevertheless discards them without effect. */
    }
}

static DWORD WINAPI ux_console_worker(void *context)
{
    ux_console *console = context;
    HANDLE waits[1];
    lib_u32 displayed = 0u;

    waits[0] = console->native->wake;
    (void)SetEvent(console->native->ready);
    while (WaitForMultipleObjects(1u, waits, FALSE, INFINITE) == WAIT_OBJECT_0) {
        ux_frame frame;
        lib_u32 generation;
        if (ux_console_process_controls(console) == LIB_FALSE) break;
        if (ux_frame_mailbox_take(&console->frames, &frame, &generation) == LIB_STATUS_OK &&
            generation != displayed) {
            displayed = generation;
            (void)ux_console_present_frame(console, &frame);
        }
    }
    return 0u;
}

lib_status ux_console_native_start(ux_console *console)
{
    ux_console_native *native;
    if (console == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    native = calloc(1u, sizeof(*native));
    if (native == LIB_NULL) return LIB_STATUS_NO_MEMORY;
    native->wake = CreateEventA(NULL, FALSE, FALSE, NULL);
    native->ready = CreateEventA(NULL, TRUE, FALSE, NULL);
    if (native->wake == NULL || native->ready == NULL) {
        if (native->wake != NULL) CloseHandle(native->wake);
        if (native->ready != NULL) CloseHandle(native->ready);
        free(native);
        return LIB_STATUS_NO_MEMORY;
    }
    console->native = native;
    native->thread = CreateThread(NULL, 0u, ux_console_worker, console, 0u, NULL);
    if (native->thread == NULL || WaitForSingleObject(native->ready, INFINITE) != WAIT_OBJECT_0) {
        ux_console_native_stop(console);
        return LIB_STATUS_IO_ERROR;
    }
    return LIB_STATUS_OK;
}

void ux_console_native_stop(ux_console *console)
{
    ux_console_native *native;
    const ux_control_message stop = { UX_CONTROL_STOP, { { 0 } } };
    if (console == LIB_NULL || (native = console->native) == LIB_NULL) return;
    (void)ux_console_push_control(console, &stop);
    if (native->thread != NULL) { (void)WaitForSingleObject(native->thread, INFINITE); CloseHandle(native->thread); }
    CloseHandle(native->ready); CloseHandle(native->wake);
    console->native = LIB_NULL;
    free(native);
}

void ux_console_native_signal(ux_console *console)
{
    if (console != LIB_NULL && console->native != LIB_NULL)
        (void)SetEvent(console->native->wake);
}
#endif
