#include "lib/types/win32/console.h"
#include "lib/types/win32/sync.h"
#include "lib/console/binding_interface.h"
#include <assert.h>
#include <string.h>

static const char *input;
static unsigned chunk = 7, reads, cancel_at;
static HANDLE stop;
static BOOL WINAPI read_chunk(HANDLE h, LPVOID bytes, DWORD capacity, LPDWORD count, LPVOID p)
{
    unsigned n = 0;
    (void)h; (void)p;
    while (input[n] && n < capacity && n < chunk) {
        ((char *)bytes)[n] = input[n];
        if (input[n++] == '\n') break;
    }
    input += n; *count = n;
    if (++reads == cancel_at) SetEvent(stop);
    return TRUE;
}
static unsigned palette_attempts, palette_sets, writes;
static int palette_query_ok, palette_set_ok, cursor_ok = 1;
static WCHAR first_cell;
static BOOL WINAPI screen_info(HANDLE h, PCONSOLE_SCREEN_BUFFER_INFO p)
{ (void)h; memset(p, 0, sizeof(*p)); p->dwSize.X=80; p->dwSize.Y=25; return TRUE; }
static BOOL WINAPI palette_get(HANDLE h, PCONSOLE_SCREEN_BUFFER_INFOEX p)
{ (void)h; (void)p; ++palette_attempts; return palette_query_ok; }
static BOOL WINAPI palette_set(HANDLE h, PCONSOLE_SCREEN_BUFFER_INFOEX p)
{ (void)h; (void)p; ++palette_sets; return palette_set_ok; }
static BOOL WINAPI write_cells(HANDLE h, const CHAR_INFO *p, COORD a, COORD b, PSMALL_RECT r)
{ (void)h; (void)a; (void)b; (void)r; ++writes; first_cell=p[0].Char.UnicodeChar; return TRUE; }
static BOOL WINAPI cursor_info(HANDLE h, const CONSOLE_CURSOR_INFO *p)
{ (void)h; (void)p; return cursor_ok; }
static BOOL WINAPI cursor_position(HANDLE h, COORD p)
{ (void)h; (void)p; return cursor_ok; }
#undef lib_win32_read_console_a
#define lib_win32_read_console_a read_chunk
#undef lib_win32_get_console_screen_buffer_info
#define lib_win32_get_console_screen_buffer_info screen_info
#undef lib_win32_get_console_screen_buffer_info_ex
#define lib_win32_get_console_screen_buffer_info_ex palette_get
#undef lib_win32_set_console_screen_buffer_info_ex
#define lib_win32_set_console_screen_buffer_info_ex palette_set
#undef lib_win32_write_console_output_w
#define lib_win32_write_console_output_w write_cells
#undef lib_win32_set_console_cursor_info
#define lib_win32_set_console_cursor_info cursor_info
#undef lib_win32_set_console_cursor_position
#define lib_win32_set_console_cursor_position cursor_position
#include "lib/host/win32/console.c"

static unsigned delivered;
static lib_console_event received;
static void receive(void *p, const lib_console_event *event)
{ (void)p; received=*event; ++delivered; }
int main(void)
{
    host_console_backend b={0};
    char line[2200];
    lib_console_text_frame f={0};
    assert(lib_console_create(&b.console)==LIB_STATUS_OK);
    assert(lib_console_set_event_sink(b.console,receive,NULL)==LIB_STATUS_OK);
    assert(lib_console_bind_generation(b.console,1)==LIB_STATUS_OK);
    b.generation=1; b.mode=HOST_CONSOLE_COOKED_LINES;
    stop=b.stop_event=CreateEventA(NULL,TRUE,FALSE,NULL); assert(stop);
    for (unsigned n=1022;n<=1024;++n) {
        memset(line,'x',n); strcpy(line+n,"\r\nhelp\r\n"); input=line;
        for (chunk=1;chunk<=1023;chunk+=1022) {
            input=line; delivered=0;
            host_console_reader(&b);
            assert(delivered==1);
            assert(received.kind==(n>1023 ? LIB_CONSOLE_EVENT_REJECTED_LINE : LIB_CONSOLE_EVENT_COOKED_LINE));
            assert(received.value.line.length==(n>1023 ? 0 : n));
            host_console_reader(&b);
            assert(delivered==2 && received.kind==LIB_CONSOLE_EVENT_COOKED_LINE);
            assert(strcmp(received.value.line.text,"help")==0);
        }
    }
    memset(line,'x',2048);strcpy(line+2048,"\r\n");input=line;
    reads=0;cancel_at=2;chunk=7;delivered=0;
    host_console_reader(&b);assert(delivered==0);ResetEvent(stop);cancel_at=0;
    assert(lib_console_pc_glyph(0)==' ' && lib_console_pc_glyph('A')=='A');
    assert(lib_console_pc_glyph(1)==0x263a && lib_console_pc_glyph(0x7f)==0x2302);
    assert(lib_console_pc_glyph(0xb3)==0x2502 && lib_console_pc_glyph(0xc4)==0x2500);
    assert(lib_console_pc_glyph(0xda)==0x250c && lib_console_pc_glyph(0xdb)==0x2588);
    assert(lib_console_pc_glyph(0x82)==0xe9 && lib_console_pc_glyph(0xff)==0xa0);
    InitializeCriticalSection(&b.output_lock);InitializeCriticalSection(&b.transaction_lock);b.output=(HANDLE)1;
    f.columns=80;f.rows=25;f.text[0]=0xdb;f.palette[0]=1;
    for(int i=0;i<2;++i) assert(host_console_backend_write_text_frame_bound(&b,b.console,1,&f)==LIB_STATUS_OK);
    assert(first_cell==0x2588 && writes==1 && palette_attempts==2);
    palette_query_ok=1;
    assert(host_console_backend_write_text_frame_bound(&b,b.console,1,&f)==0);
    assert(b.previous_palette[0]==0 && palette_sets==1);
    palette_set_ok=1;
    assert(host_console_backend_write_text_frame_bound(&b,b.console,1,&f)==0);
    assert(b.previous_palette[0]==1 && palette_sets==2);
    assert(host_console_backend_write_text_frame_bound(&b,b.console,1,&f)==0);
    assert(palette_sets==2);
    cursor_ok=0;
    assert(host_console_backend_write_text_frame_bound(&b,b.console,1,&f)==LIB_STATUS_IO_ERROR);
    f.cursor_visible=f.cursor_phase=1;
    assert(host_console_backend_write_text_frame_bound(&b,b.console,1,&f)==LIB_STATUS_IO_ERROR);
    cursor_ok=1;
    assert(host_console_backend_write_text_frame_bound(&b,b.console,1,&f)==LIB_STATUS_OK);
    DeleteCriticalSection(&b.transaction_lock);DeleteCriticalSection(&b.output_lock);CloseHandle(stop);lib_console_release(b.console);
    return 0;
}
