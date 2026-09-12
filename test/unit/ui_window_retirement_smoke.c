#include "lib/ui-window/window.h"
#include "lib/types/win32/window.h"
#include "lib/types/win32/sync.h"
#include <assert.h>

static HANDLE waiting, proceed;
static HWND created;
static int scenario, retired, failures;
static HWND WINAPI create_window(DWORD ex, LPCSTR klass, LPCSTR title,
    DWORD style, int x, int y, int width, int height, HWND parent, HMENU menu,
    HINSTANCE instance, LPVOID param)
{
    created = CreateWindowExA(ex, klass, title, style, x, y, width, height,
        parent, menu, instance, param);
    return created;
}
static BOOL WINAPI hide_window(HWND window, int command)
{ (void)command; return ShowWindow(window, SW_HIDE); }
static BOOL WINAPI no_foreground(HWND window) { (void)window; return TRUE; }
static HWND WINAPI no_focus(HWND window) { return window; }
static HDC WINAPI create_dc(HDC dc)
{ return scenario == 2 ? NULL : CreateCompatibleDC(dc); }
static ui_mailbox_wake_wait_result controlled_wait(const ui_mailbox_wake *wake,
    lib_u32 timeout)
{
    (void)wake; (void)timeout;
    SetEvent(waiting);
    assert(WaitForSingleObject(proceed, INFINITE) == WAIT_OBJECT_0);
    if (scenario == 1 || scenario == 6) return UI_MAILBOX_WAKE_WAIT_FAULT;
    if (scenario == 3) PostQuitMessage(0);
    if (scenario == 4) assert(DestroyWindow(created));
    return UI_MAILBOX_WAKE_WAIT_WAKE;
}
#undef lib_win32_create_window_ex_a
#undef lib_win32_show_window
#undef lib_win32_set_foreground_window
#undef lib_win32_set_focus
#undef lib_win32_create_compatible_dc
#define lib_win32_create_window_ex_a create_window
#define lib_win32_show_window hide_window
#define lib_win32_set_foreground_window no_foreground
#define lib_win32_set_focus no_focus
#define lib_win32_create_compatible_dc create_dc
#define ui_mailbox_wake_wait_messages controlled_wait
#include "lib/ui-window/win32/component.c"

static int input(void *context, const ui_input_event *event)
{
    (void)context;
    assert(event->type == UI_EVENT_SOURCE_RETIRED);
    ++retired;
    return scenario < 5;
}
static void failure(void *context, lib_u64 identity, lib_status status)
{
    (void)context;
    assert(identity != 0u && status == LIB_STATUS_IO_ERROR);
    ++failures;
}
int main(void)
{
    static ui_frame frame;
    for (scenario = 0; scenario != 7; ++scenario) {
        ui_window_options options = { 0 };
        ui_window *window = NULL;
        waiting = CreateEventA(NULL, TRUE, FALSE, NULL);
        proceed = CreateEventA(NULL, TRUE, FALSE, NULL);
        assert(waiting && proceed);
        retired = failures = 0;
        options.component.input_sink = input;
        options.component.failure_sink = failure;
        options.initial_title = "retirement proof";
        assert(ui_window_create(&window, &options) == LIB_STATUS_OK);
        assert(WaitForSingleObject(waiting, INFINITE) == WAIT_OBJECT_0);
        frame.valid = 1u; frame.text_columns = 80u; frame.text_rows = 25u;
        assert(ui_window_publish_frame(window, &frame) == LIB_STATUS_OK);
        if (scenario == 0 || scenario == 5)
            assert(ui_component_request_stop(&window->base) == LIB_STATUS_OK);
        SetEvent(proceed);
        /* Wait for the actual worker before destroy queues an additional STOP. */
        ui_window_win32_state *state = window->worker_state;
        assert(WaitForSingleObject(state->worker, 5000u) == WAIT_OBJECT_0);
        assert(retired == 1 && failures == (scenario != 0));
        assert(!IsWindow(created));
        ui_window_destroy(window);
        assert(retired == 1);
        CloseHandle(waiting); CloseHandle(proceed);
    }
    return 0;
}
