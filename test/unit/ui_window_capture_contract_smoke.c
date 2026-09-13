#include "lib/ui-window/window.h"
#include "lib/types/win32/window.h"
#include "lib/types/win32/sync.h"
#include <assert.h>

static HWND owner, focused;
static RECT client = {0,0,640,480}, clipped;
static POINT origin = {100,200};
static unsigned releases, clips, events;
static int clip_ok = 1, resize_ok = 1, title_ok = 1;
static void *context;
static void notify_loss(void);
static HWND WINAPI get_capture(void) { return owner; }
static HWND WINAPI set_capture(HWND w) { HWND old=owner; owner=w; return old; }
static BOOL WINAPI release_capture(void)
{ ++releases; owner=NULL; notify_loss(); return TRUE; }
static HWND WINAPI set_focus(HWND w) { focused=w; return w; }
static HWND WINAPI get_focus(void) { return focused; }
static BOOL WINAPI clip(const RECT *r)
{ ++clips; if (r) clipped=*r; return r ? clip_ok : TRUE; }
static BOOL WINAPI get_client(HWND w, RECT *r) { (void)w; *r=client; return TRUE; }
static BOOL WINAPI to_screen(HWND w, POINT *p)
{ (void)w; p->x+=origin.x; p->y+=origin.y; return TRUE; }
static HCURSOR WINAPI cursor(HCURSOR c) { return c; }
static LONG_PTR WINAPI get_context(HWND w, int index)
{ (void)w; return index==GWLP_USERDATA ? (LONG_PTR)context : 0; }
static BOOL WINAPI resize(HWND w, HWND after, int x,int y,int cx,int cy,UINT f)
{ (void)w;(void)after;(void)x;(void)y;(void)cx;(void)cy;(void)f;return resize_ok; }
static BOOL WINAPI title(HWND w,LPCSTR text) { (void)w;(void)text;return title_ok; }
#undef lib_win32_get_capture
#undef lib_win32_set_capture
#undef lib_win32_release_capture
#undef lib_win32_set_focus
#undef lib_win32_get_focus
#undef lib_win32_clip_cursor
#undef lib_win32_get_client_rect
#undef lib_win32_client_to_screen
#undef lib_win32_set_cursor
#undef lib_win32_get_window_long_ptr_a
#undef lib_win32_set_window_pos
#undef lib_win32_set_window_text_a
#define lib_win32_get_capture get_capture
#define lib_win32_set_capture set_capture
#define lib_win32_release_capture release_capture
#define lib_win32_set_focus set_focus
#define lib_win32_get_focus get_focus
#define lib_win32_clip_cursor clip
#define lib_win32_get_client_rect get_client
#define lib_win32_client_to_screen to_screen
#define lib_win32_set_cursor cursor
#define lib_win32_get_window_long_ptr_a get_context
#define lib_win32_set_window_pos resize
#define lib_win32_set_window_text_a title
#include "lib/ui-window/win32/mouse.c"
#include "lib/ui-window/win32/geometry.c"
#include "lib/ui-window/win32/component.c"

static void notify_loss(void) { win32_window_proc((HWND)1,WM_CAPTURECHANGED,0,0); }
static int input(void *p,const ui_input_event *e)
{ (void)p; assert(e->type==UI_EVENT_MOUSE); ++events; return 1; }
static void failure(void *p,lib_u64 id,lib_status status)
{ (void)p;(void)id;(void)status; }
static void join(ui_component *p) { (void)p; }
static void dispose(ui_component *p) { ui_component_mailboxes_destroy(&p->mailboxes); }
int main(void)
{
    static ui_window window;
    static ui_win32_window_context c;
    ui_component_options options={.input_sink=input,.failure_sink=failure};
    assert(ui_component_initialize(&window.base,&options,join,dispose)==LIB_STATUS_OK);
    c.component=&window; context=&c;
    assert(ui_win32_mouse_refresh_bounds(&c.mouse) && clips==0);
    assert(ui_win32_mouse_capture(&c.mouse,(HWND)1,0));
    assert(clipped.left==100 && clipped.top==200 && clipped.right==740 && clipped.bottom==680);
    origin.x=-300; origin.y=50;
    win32_window_proc((HWND)1,WM_MOVE,0,0);
    assert(clipped.left==-300 && clipped.top==50);
    client.right=320; client.bottom=240;
    win32_window_proc((HWND)1,WM_SIZE,0,0);
    assert(clipped.right==20 && clipped.bottom==290);
    c.left_button=1; c.mouse.motion.remainder_x=7;
    owner=(HWND)2; notify_loss();
    assert(!c.mouse.captured && !c.left_button && !c.mouse.motion.valid);
    assert(!c.mouse.motion.remainder_x && releases==0 && events==1 && owner==(HWND)2);
    notify_loss(); assert(events==1 && releases==0);
    assert(ui_win32_mouse_capture(&c.mouse,(HWND)1,0));
    c.right_button=1; win32_window_release_mouse(&c);
    assert(releases==1 && events==2 && !c.mouse.captured && !c.right_button);
    assert(ui_win32_mouse_capture(&c.mouse,(HWND)1,0));
    clip_ok=0; win32_window_proc((HWND)1,WM_MOVE,0,0);
    assert(!c.mouse.captured && releases==2);
    unsigned previous=clips;
    win32_window_proc((HWND)1,WM_MOVE,0,0); assert(clips==previous);
    resize_ok=0;
    win32_window_resize_client((HWND)1,&c,640,480);
    assert(c.client_surface_width==0);
    resize_ok=1;
    win32_window_resize_client((HWND)1,&c,640,480);
    assert(c.client_surface_width==640 && c.client_surface_height==480);
    title_ok=0;
    ui_component_control command={.kind=UI_COMPONENT_CONTROL_SET_WINDOW_TITLE};
    assert(ui_component_mailboxes_enqueue_controls(&window.base.mailboxes,&command,1)==0);
    assert(!win32_window_consume_mailboxes((HWND)1,&c) && window.base.stopping);
    ui_component_destroy(&window.base);
    for (unsigned edge=UI_WINDOW_EDGE_LEFT;edge<=UI_WINDOW_EDGE_BOTTOMRIGHT;++edge) {
        ui_window_rect r={10,20,826,749};
        ui_window_constrain_sizing(&r,(ui_window_edge)edge,16,29,640,480);
        int w=r.right-r.left-16,h=r.bottom-r.top-29;
        assert((edge==UI_WINDOW_EDGE_LEFT || edge==UI_WINDOW_EDGE_RIGHT) ?
            (w==800 && h==600) : (w==933 && h==700));
        assert((edge==UI_WINDOW_EDGE_LEFT || edge==UI_WINDOW_EDGE_TOPLEFT || edge==UI_WINDOW_EDGE_BOTTOMLEFT) ? r.right==826 : r.left==10);
        assert((edge==UI_WINDOW_EDGE_TOP || edge==UI_WINDOW_EDGE_TOPLEFT || edge==UI_WINDOW_EDGE_TOPRIGHT) ? r.bottom==749 : r.top==20);
    }
    return 0;
}
