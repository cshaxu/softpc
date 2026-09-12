#include "lib/types/win32/console.h"
#include "lib/types/win32/sync.h"
#include "lib/console/binding_interface.h"
#include <assert.h>

static BOOL WINAPI failed_read(HANDLE input, LPVOID bytes, DWORD length,
    LPDWORD read, LPVOID reserved)
{ (void)input; (void)bytes; (void)length; (void)read; (void)reserved; return FALSE; }
static DWORD WINAPI failed_wait(DWORD count, const HANDLE *handles, BOOL all, DWORD timeout)
{ (void)count; (void)handles; (void)all; (void)timeout; return WAIT_FAILED; }
#undef lib_win32_read_console_a
#undef lib_win32_wait_for_multiple_objects
#define lib_win32_read_console_a failed_read
#define lib_win32_wait_for_multiple_objects failed_wait
#include "lib/host/win32/console.c"

static int failures;
static void receive(void *context, const lib_console_event *event)
{
    (void)context;
    assert(event->kind == LIB_CONSOLE_EVENT_IO_FAILURE);
    ++failures;
}
int main(void)
{
    host_console_backend backend = { 0 };
    assert(lib_console_create(&backend.console) == LIB_STATUS_OK);
    assert(lib_console_set_event_sink(backend.console, receive, NULL) == LIB_STATUS_OK);
    assert(lib_console_bind_generation(backend.console, 1u) == LIB_STATUS_OK);
    backend.generation = 1u;
    backend.stop_event = CreateEventA(NULL, TRUE, FALSE, NULL);
    assert(backend.stop_event);
    for (int mode = 0; mode != 2; ++mode) {
        backend.mode = mode == 0 ? HOST_CONSOLE_RAW_EVENTS : HOST_CONSOLE_COOKED_LINES;
        ResetEvent(backend.stop_event);
        host_console_reader(&backend);
        assert(failures == mode + 1);
        SetEvent(backend.stop_event);
        host_console_reader(&backend);
        assert(failures == mode + 1);
    }
    ResetEvent(backend.stop_event);
    backend.generation = 2u;
    host_console_reader(&backend);
    assert(failures == 2);
    CloseHandle(backend.stop_event);
    lib_console_release(backend.console);
    return 0;
}
