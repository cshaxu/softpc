#include "lib/ui-window/window.h"
#include "lib/types/win32/window.h"
#include "lib/types/win32/sync.h"
#include <assert.h>

static HANDLE waiting, proceed;
static HWND created;
static int scenario, retired, failures, ordinary, closes;
static void input_scenario(void);
static void paint_scenario(void);
static unsigned paint_ends;
static int inject_output_failure;
static BOOL WINAPI invalidate(HWND w,const RECT *r,BOOL erase)
{ return inject_output_failure && scenario==9 ? FALSE : InvalidateRect(w,r,erase); }
static BOOL WINAPI blit(HDC d,int x,int y,int w,int h,HDC s,int sx,int sy,int sw,int sh,DWORD op)
{ return scenario==10 ? FALSE : StretchBlt(d,x,y,w,h,s,sx,sy,sw,sh,op); }
static BOOL WINAPI invert(HDC d,const RECT *r)
{ return scenario==11 ? FALSE : InvertRect(d,r); }
static HDC WINAPI begin(HWND w,PAINTSTRUCT *p)
{ return inject_output_failure && scenario==12 ? NULL : BeginPaint(w,p); }
static BOOL WINAPI end(HWND w,const PAINTSTRUCT *p)
{ ++paint_ends; return EndPaint(w,p); }
static HCURSOR WINAPI make_cursor(HINSTANCE i,int x,int y,int w,int h,const void *a,const void *b)
{ return scenario==13 ? NULL : CreateCursor(i,x,y,w,h,a,b); }
static HWND WINAPI create_window(DWORD ex, LPCWSTR klass, LPCWSTR title,
    DWORD style, int x, int y, int width, int height, HWND parent, HMENU menu,
    HINSTANCE instance, LPVOID param)
{
    created = CreateWindowExW(ex, klass, title, style, x, y, width, height,
        parent, menu, instance, param);
    assert(created == NULL || IsWindowUnicode(created));
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
    inject_output_failure=1;
    if (scenario == 1 || scenario == 6) return UI_MAILBOX_WAKE_WAIT_FAULT;
    if (scenario == 3) PostQuitMessage(0);
    if (scenario == 4) assert(DestroyWindow(created));
    if (scenario == 7 || scenario == 8) input_scenario();
    if (scenario >= 10 && scenario <= 12) paint_scenario();
    return UI_MAILBOX_WAKE_WAIT_WAKE;
}
#undef lib_win32_create_window_ex_w
#undef lib_win32_show_window
#undef lib_win32_set_foreground_window
#undef lib_win32_set_focus
#undef lib_win32_create_compatible_dc
#define lib_win32_create_window_ex_w create_window
#define lib_win32_show_window hide_window
#define lib_win32_set_foreground_window no_foreground
#define lib_win32_set_focus no_focus
#define lib_win32_create_compatible_dc create_dc
#undef lib_win32_invalidate_rect
#undef lib_win32_stretch_blt
#undef lib_win32_invert_rect
#undef lib_win32_begin_paint
#undef lib_win32_end_paint
#undef lib_win32_create_cursor
#define lib_win32_invalidate_rect invalidate
#define lib_win32_stretch_blt blit
#define lib_win32_invert_rect invert
#define lib_win32_begin_paint begin
#define lib_win32_end_paint end
#define lib_win32_create_cursor make_cursor
#define ui_mailbox_wake_wait_messages controlled_wait
static void checked_fail(ui_component *component, lib_status status)
{
    static ui_frame rejected = { .valid = 1, .text_columns = 80, .text_rows = 25 };
    ui_component_fail(component, status);
    assert(ui_component_mailboxes_publish_frame(&component->mailboxes, &rejected) ==
        LIB_STATUS_INVALID_STATE);
    ui_component_control title = { .kind = UI_COMPONENT_CONTROL_SET_WINDOW_TITLE };
    assert(ui_component_mailboxes_enqueue_controls(&component->mailboxes, &title, 1u) ==
        LIB_STATUS_INVALID_STATE);
}
#define ui_component_fail checked_fail
#include "lib/ui-window/win32/component.c"
#undef ui_component_fail

static int input(void *context, const ui_input_event *event)
{
    (void)context;
    if (scenario == 7 && event->type == UI_EVENT_KEY) { ++ordinary; return 0; }
    if (scenario == 8 && event->type == UI_EVENT_WINDOW_CLOSE) { ++closes; return 1; }
    assert(event->type == UI_EVENT_SOURCE_RETIRED);
    ++retired;
    return scenario < 5 || scenario >= 7;
}
static void failure(void *context, lib_u64 identity, lib_status status)
{
    (void)context;
    assert(identity != 0u && status == LIB_STATUS_IO_ERROR);
    ++failures;
}
static void input_scenario(void)
{
    ui_win32_window_context *ctx = win32_window_context(created);
    ui_input_event key = { .type = UI_EVENT_KEY };
    key.data.key.pressed = 1; key.data.key.scan_code = 0x1d;
    key.data.key.key = scenario == 7 ? 'A' : UI_KEY_CONTROL;
    key.data.key.modifiers = UI_HOTKEY_MODIFIER_CONTROL;
    if (scenario == 7) {
        assert(!win32_window_emit_normalized(ctx, &key));
        assert(!win32_window_emit_normalized(ctx, &key));
    } else {
        assert(win32_window_emit_normalized(ctx, &key));
        SendMessageA(created, WM_CLOSE, 0, 0);
        key.data.key.pressed = 0;
        assert(win32_window_emit_normalized(ctx, &key));
        assert(ui_component_request_stop(&ctx->component->base) == LIB_STATUS_OK);
    }
}
static void paint_scenario(void)
{
    ui_win32_window_context *ctx=win32_window_context(created);
    if (!ctx->surface_dc) return; /* First wake consumes the copied frame. */
    SendMessageW(created,WM_PAINT,0,0);
}
int main(void)
{
    static ui_frame frame;
    for (scenario = 0; scenario != 14; ++scenario) {
        ui_window_options options = { 0 };
        ui_window *window = NULL;
        waiting = CreateEventA(NULL, TRUE, FALSE, NULL);
        proceed = CreateEventA(NULL, TRUE, FALSE, NULL);
        assert(waiting && proceed);
        retired = failures = ordinary = closes = 0;
        paint_ends=0;
        inject_output_failure=0;
        options.component.input_sink = input;
        options.component.failure_sink = failure;
        options.initial_title = "retirement proof";
        options.initial_frozen = scenario == 8;
        assert(ui_hotkey_registry_register(&options.component.hotkeys, 'P',
            UI_HOTKEY_MODIFIER_CONTROL | UI_HOTKEY_MODIFIER_ALT, "toggle") == LIB_STATUS_OK);
        if (scenario==13) {
            assert(ui_window_create(&window,&options)==LIB_STATUS_IO_ERROR && !window);
            assert(!IsWindow(created) && retired==0);
            CloseHandle(waiting); CloseHandle(proceed);
            continue;
        }
        assert(ui_window_create(&window, &options) == LIB_STATUS_OK);
        assert(WaitForSingleObject(waiting, INFINITE) == WAIT_OBJECT_0);
        frame.valid = 1u; frame.text_columns = 80u; frame.text_rows = 25u;
        frame.cursor_visible=1; frame.font_height=16; frame.cursor_top=14; frame.cursor_bottom=15;
        assert(ui_window_publish_frame(window, &frame) == LIB_STATUS_OK);
        if (scenario == 0 || scenario == 5)
            assert(ui_component_request_stop(&window->base) == LIB_STATUS_OK);
        SetEvent(proceed);
        /* Wait for the actual worker before destroy queues an additional STOP. */
        ui_window_win32_state *state = window->worker_state;
        assert(WaitForSingleObject(state->worker, 5000u) == WAIT_OBJECT_0);
        assert(retired == 1 && failures == (scenario != 0 && scenario != 8));
        assert(!IsWindow(created));
        if (scenario==10 || scenario==11) assert(paint_ends>0);
        assert(ordinary == (scenario == 7) && closes == (scenario == 8));
        assert(ui_window_publish_frame(window, &frame) == LIB_STATUS_INVALID_STATE);
        ui_window_destroy(window);
        assert(retired == 1);
        CloseHandle(waiting); CloseHandle(proceed);
    }
    return 0;
}
