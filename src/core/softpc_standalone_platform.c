#include "insignia.h"
#include "host_def.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "cpu4.h"
#include "cpu_vid.h"
#include "xt.h"
#include "error.h"
#include "fdisk.h"
#include "gfi.h"
#include "config.h"
#include "ios.h"
#include "ica.h"
#include "timestmp.h"
#include "timeval.h"
#include "timer.h"
#include "keyboard.h"
#include "gmi.h"
#include "gfx_upd.h"
#include "egaports.h"
#include "gvi.h"
#include "egagraph.h"
#include "video.h"
#include "host_com.h"
#include "virtual.h"
#include "softpc_standalone_dib.h"
#include "nt_graph.h"

/*
 * Minimal host ports for the detached CCPU.  These are deliberately machine
 * mechanics only: RAM allocation, a monotonic event clock, and interrupt
 * notification.  There is no product-session or service-dispatch layer here.
 */

extern void c_cpu_simulate();

static UTINY *softpc_ram;
static sys_addr softpc_ram_size;
IU32 softpc_ccpu_instruction_budget = 0;

/* These are controller buffers, not a second video implementation.  The
   original platform allocated them while bringing up its UI; the detached
   platform owns the same allocation at its presentation boundary instead. */
extern byte *EGA_planes;
extern byte *video_copy;
extern PC_palette *DAC;
extern IU8 Video_mode;
extern IU8 Currently_emulated_video_mode;

#define SOFTPC_VGA_MODE13_WIDTH 320u
#define SOFTPC_VGA_MODE13_HEIGHT 200u
#define SOFTPC_VGA_MODE13_PIXELS \
    (SOFTPC_VGA_MODE13_WIDTH * SOFTPC_VGA_MODE13_HEIGHT)
#define SOFTPC_VGA_MODE12_WIDTH 640u
#define SOFTPC_VGA_MODE12_HEIGHT 480u
#define SOFTPC_VGA_MODE12_PIXELS \
    (SOFTPC_VGA_MODE12_WIDTH * SOFTPC_VGA_MODE12_HEIGHT)
#define SOFTPC_VGA_MODE12_BYTES_PER_LINE 80u
#define SOFTPC_CGA_BYTES_PER_LINE 80u
#define SOFTPC_CGA_ODD_BANK_OFFSET 0x2000u

/* The original video core's optional stream-I/O path is a product console
   optimization.  The detached VM presents through its own console/window,
   so it remains disabled while retaining the original controller behavior. */
half_word *stream_io_buffer = NULL;
boolean stream_io_enabled = FALSE;
word stream_io_buffer_size = 0;
word *stream_io_dirty_count_ptr = NULL;

/* Original EGA/VGA controller host ports.  Presentation remains owned by the
   standalone console/window; these callbacks preserve the controller's
   lifecycle without importing the historical host product. */
static void softpc_video_void(void) {}
static void softpc_video_init_screen(void)
{
    if (video_copy == NULL) video_copy = (byte *)calloc(1u, 0x20000u);
    if (EGA_planes == NULL)
        EGA_planes = (byte *)calloc(4u, (size_t)EGA_PLANE_SIZE);
    if (DAC == NULL) DAC = (PC_palette *)calloc(VGA_DAC_SIZE, sizeof(*DAC));
    (void)softpc_standalone_dib_init();
}
static void softpc_video_init_adaptor(int adapter, int height)
{ UNUSED(adapter); UNUSED(height); }
static void softpc_video_int(int value) { UNUSED(value); }
static void softpc_video_palette(PC_palette *palette, int count)
{
    softpc_standalone_dib_set_palette(palette, count);
}
static boolean softpc_video_scroll(int start, int width, int height,
    int attribute, int lines, int ignored)
{ UNUSED(start); UNUSED(width); UNUSED(height); UNUSED(attribute); UNUSED(lines); UNUSED(ignored); return TRUE; }
static void softpc_video_cursor(int x, int y, half_word attribute)
{ UNUSED(x); UNUSED(y); UNUSED(attribute); }
static void softpc_video_two_ints(int first, int second)
{ UNUSED(first); UNUSED(second); }

/* This is the original nt_graph.c standard colour dispatch table, retained
   here as the standalone host's narrow routing layer.  The paint routines
   themselves remain the original nt_cga.c, nt_ega.c and nt_vga.c sources. */
static PAINTFUNCS softpc_standard_colour_paint_funcs = {
    nt_text,
    nt_cga_colour_med_graph_std, nt_cga_colour_hi_graph_std,
    nt_text,
    nt_ega_lo_graph_std, nt_ega_med_graph_std, nt_ega_hi_graph_std,
    nt_vga_graph_std, nt_vga_med_graph_std, nt_vga_hi_graph_std,
#ifdef V7VGA
    nt_v7vga_hi_graph_std
#endif
};

static INITFUNCS softpc_colour_init_funcs = {
    nt_init_text,
    nt_init_cga_colour_med_graph, nt_init_cga_colour_hi_graph,
    nt_init_text,
    nt_init_ega_lo_graph, nt_init_ega_med_graph, nt_init_ega_hi_graph,
    nt_init_vga_hi_graph
};

static void softpc_standalone_set_paint(DISPLAY_MODE mode, int height)
{
    UNUSED(height);
    FunnyPaintMode = FALSE;
    switch ((int)mode) {
    case TEXT_40_FUN: case CGA_TEXT_40_SP: case CGA_TEXT_40_SP_WR:
    case CGA_TEXT_40: case CGA_TEXT_40_WR: case TEXT_80_FUN:
    case CGA_TEXT_80_SP: case CGA_TEXT_80_SP_WR: case CGA_TEXT_80:
    case CGA_TEXT_80_WR:
        sc.ModeType = TEXT;
        paint_screen = softpc_standard_colour_paint_funcs.cga_text;
        softpc_colour_init_funcs.cga_text();
        break;
    case CGA_MED_FUN: case CGA_MED:
        sc.ModeType = GRAPHICS;
        paint_screen = softpc_standard_colour_paint_funcs.cga_med_graph;
        softpc_colour_init_funcs.cga_med_graph();
        break;
    case CGA_HI_FUN: case CGA_HI:
        sc.ModeType = GRAPHICS;
        paint_screen = softpc_standard_colour_paint_funcs.cga_hi_graph;
        softpc_colour_init_funcs.cga_hi_graph();
        break;
    case EGA_TEXT_40_SP: case EGA_TEXT_40_SP_WR: case EGA_TEXT_40:
    case EGA_TEXT_40_WR: case EGA_TEXT_80_SP: case EGA_TEXT_80_SP_WR:
    case EGA_TEXT_80: case EGA_TEXT_80_WR:
        sc.ModeType = TEXT;
        paint_screen = softpc_standard_colour_paint_funcs.ega_text;
        softpc_colour_init_funcs.ega_text();
        break;
    case EGA_HI_FUN: case EGA_HI: case EGA_HI_WR: case EGA_HI_SP:
    case EGA_HI_SP_WR:
        sc.ModeType = GRAPHICS;
        if (get_256_colour_mode()) {
#ifdef V7VGA
            if (get_seq_chain4_mode() && get_chain4_mode())
                paint_screen = softpc_standard_colour_paint_funcs.v7vga_hi_graph;
            else
#endif
            if (get_chain4_mode())
                paint_screen = softpc_standard_colour_paint_funcs.vga_graph;
            else if (get_char_height() == 2)
                paint_screen = softpc_standard_colour_paint_funcs.vga_med_graph;
            else
                paint_screen = softpc_standard_colour_paint_funcs.vga_hi_graph;
            softpc_colour_init_funcs.vga_hi_graph();
        } else {
            paint_screen = softpc_standard_colour_paint_funcs.ega_hi_graph;
            softpc_colour_init_funcs.ega_hi_graph();
        }
        break;
    case EGA_MED_FUN: case EGA_MED: case EGA_MED_WR: case EGA_MED_SP:
    case EGA_MED_SP_WR:
        sc.ModeType = GRAPHICS;
        paint_screen = softpc_standard_colour_paint_funcs.ega_med_graph;
        softpc_colour_init_funcs.ega_med_graph();
        break;
    case EGA_LO_FUN: case EGA_LO: case EGA_LO_WR: case EGA_LO_SP:
    case EGA_LO_SP_WR:
        sc.ModeType = GRAPHICS;
        paint_screen = softpc_standard_colour_paint_funcs.ega_lo_graph;
        softpc_colour_init_funcs.ega_lo_graph();
        break;
    default:
        paint_screen = softpc_video_void;
        break;
    }
}
static VIDEOFUNCS softpc_video_functions = {
    softpc_video_init_screen, softpc_video_init_adaptor, softpc_video_void,
    softpc_video_int, softpc_video_palette, softpc_video_int,
    softpc_video_void, softpc_video_void, softpc_video_void,
    softpc_video_void, softpc_video_void, softpc_video_void,
    softpc_video_scroll, softpc_video_scroll, (void (*)())softpc_video_cursor,
    (void (*)())softpc_standalone_set_paint, softpc_video_int, softpc_video_void,
    softpc_video_two_ints, softpc_video_int, softpc_video_int,
    softpc_video_int, softpc_video_two_ints, softpc_video_two_ints,
    softpc_video_void
};
VIDEOFUNCS *working_video_funcs = &softpc_video_functions;
void (*paint_screen)() = softpc_video_void;

int softpc_platform_presentation_is_graphics(void)
{
    return sc.ModeType == GRAPHICS;
}

int softpc_platform_video_buffers_init(void)
{
    host_init_screen();
    return video_copy != NULL && EGA_planes != NULL && DAC != NULL;
}

/* The fixed standalone machine has no product sound preference; retain the
   original default-enabled bell path for video BEL output. */
void host_ring_bell(long duration)
{
    if (host_runtime_inquire(C_SOUND_ON)) host_alarm(duration);
}
void stream_io_update(void) {}

/* V7's original controller owns the hardware-pointer registers.  The
   detached presentation layer has no separate hardware-pointer surface, so
   its two paint hooks are intentionally inert. */
static void softpc_v7_pointer_void()
{
}
void (*paint_v7ptr)() = softpc_v7_pointer_void;
void (*clear_v7ptr)() = softpc_v7_pointer_void;

CHAR *SPC_Product_Name = "SoftPC VM";

/* This VM intentionally has one machine instance.  The original mouse driver
   asks the Windows-era NIDDB service for a per-instance data handle; the
   standalone equivalent is a single owned allocation, not a VDM service. */
static IHP softpc_instance_data;

IHP *NIDDB_Allocate_Instance_Data(size, create_callback, terminate_callback)
int size;
NIDDB_CR_CALLBACK create_callback;
NIDDB_TM_CALLBACK terminate_callback;
{
    UNUSED(terminate_callback);
    if (size <= 0) return NULL;
    free(softpc_instance_data);
    softpc_instance_data = calloc(1u, (size_t)size);
    if (softpc_instance_data != NULL && create_callback != NULL)
        (*create_callback)(&softpc_instance_data);
    return &softpc_instance_data;
}

void NIDDB_Deallocate_Instance_Data(handle)
IHP *handle;
{
    if (handle == NULL) return;
    free(*handle);
    *handle = NULL;
}

void host_memset(address, value, size)
char *address;
char value;
unsigned int size;
{
    if (address != NULL) memset(address, (unsigned char)value, size);
}

void memfill(unsigned char data, unsigned char *first, unsigned char *last)
{
    if (first != NULL && last >= first) memset(first, data,
        (size_t)(last - first) + 1u);
}

void fwd_word_fill(unsigned short data, unsigned char *destination, int count)
{
    int index;
    for (index = 0; destination != NULL && index < count; ++index) {
        destination[index * 2] = (unsigned char)data;
        destination[index * 2 + 1] = (unsigned char)(data >> 8u);
    }
}

void memset4(unsigned int data, unsigned int *destination, unsigned int count)
{
    unsigned int index;
    for (index = 0u; destination != NULL && index < count; ++index)
        destination[index] = data;
}

#define SOFTPC_CONFIG_GFX_ADAPTER 54u
#define SOFTPC_VGA_ADAPTER 5u

/* The original PPI and 8253 own speaker gating and waveform generation.
   This is only their standalone host sink.  NTVDM used the private Beep
   device asynchronously; use a small Win32 worker so a guest tone never
   blocks the executor or its presentation event loop. */
#ifdef _WIN32
#define SOFTPC_SPEAKER_CLOCK_HZ 1193180ul
#define SOFTPC_SPEAKER_MIN_HZ 37ul
#define SOFTPC_SPEAKER_MAX_HZ 20000ul
#define SOFTPC_SPEAKER_SLICE_MS 40u
static HANDLE softpc_speaker_wake;
static HANDLE softpc_speaker_stop;
static HANDLE softpc_speaker_thread;
static volatile LONG softpc_speaker_enabled;
static volatile LONG softpc_speaker_frequency;

static DWORD WINAPI softpc_speaker_worker(void *unused)
{
    HANDLE waits[2];
    UNUSED(unused);
    waits[0] = softpc_speaker_stop;
    waits[1] = softpc_speaker_wake;
    for (;;) {
        DWORD result = WaitForMultipleObjects(2u, waits, FALSE, INFINITE);
        if (result == WAIT_OBJECT_0) break;
        if (result != WAIT_OBJECT_0 + 1u) continue;
        ResetEvent(softpc_speaker_wake);
        while (InterlockedCompareExchange(&softpc_speaker_enabled, 0, 0) != 0 &&
               InterlockedCompareExchange(&softpc_speaker_frequency, 0, 0) != 0) {
            DWORD frequency = (DWORD)InterlockedCompareExchange(
                &softpc_speaker_frequency, 0, 0);
            (void)Beep(frequency, SOFTPC_SPEAKER_SLICE_MS);
            if (WaitForSingleObject(softpc_speaker_stop, 0u) == WAIT_OBJECT_0)
                return 0u;
            if (WaitForSingleObject(softpc_speaker_wake, 0u) == WAIT_OBJECT_0)
                ResetEvent(softpc_speaker_wake);
        }
    }
    return 0u;
}

static void softpc_speaker_wake_worker(void)
{
    if (softpc_speaker_wake == NULL) {
        softpc_speaker_wake = CreateEventA(NULL, TRUE, FALSE, NULL);
        softpc_speaker_stop = CreateEventA(NULL, TRUE, FALSE, NULL);
        if (softpc_speaker_wake == NULL || softpc_speaker_stop == NULL) {
            if (softpc_speaker_wake != NULL) CloseHandle(softpc_speaker_wake);
            if (softpc_speaker_stop != NULL) CloseHandle(softpc_speaker_stop);
            softpc_speaker_wake = NULL;
            softpc_speaker_stop = NULL;
            return;
        }
    }
    if (softpc_speaker_thread == NULL) {
        softpc_speaker_thread = CreateThread(NULL, 0u, softpc_speaker_worker,
            NULL, 0u, NULL);
        if (softpc_speaker_thread == NULL) return;
    }
    SetEvent(softpc_speaker_wake);
}

static void softpc_speaker_shutdown(void)
{
    if (softpc_speaker_thread != NULL) {
        SetEvent(softpc_speaker_stop);
        SetEvent(softpc_speaker_wake);
        (void)WaitForSingleObject(softpc_speaker_thread, INFINITE);
        CloseHandle(softpc_speaker_thread);
        softpc_speaker_thread = NULL;
    }
    if (softpc_speaker_wake != NULL) CloseHandle(softpc_speaker_wake);
    if (softpc_speaker_stop != NULL) CloseHandle(softpc_speaker_stop);
    softpc_speaker_wake = NULL;
    softpc_speaker_stop = NULL;
    InterlockedExchange(&softpc_speaker_enabled, 0);
    InterlockedExchange(&softpc_speaker_frequency, 0);
}
#endif

void host_enable_timer2_sound(void)
{
#ifdef _WIN32
    InterlockedExchange(&softpc_speaker_enabled, 1);
    if (InterlockedCompareExchange(&softpc_speaker_frequency, 0, 0) != 0)
        softpc_speaker_wake_worker();
#endif
}

void host_disable_timer2_sound(void)
{
#ifdef _WIN32
    InterlockedExchange(&softpc_speaker_enabled, 0);
    if (softpc_speaker_wake != NULL) SetEvent(softpc_speaker_wake);
#endif
}

void host_alarm(long duration)
{
    UNUSED(duration);
#ifdef _WIN32
    (void)MessageBeep(MB_OK);
#endif
}

/* The console owns its event pump and invokes the machine in bounded slices.
   Returning here therefore yields to that slice boundary without borrowing a
   historical host scheduler. */
void host_release_timeslice(void)
{
}

/* The original CMOS controller expects the product configuration service.
 * A standalone fixed machine exposes no mutable product configuration: its
 * concrete media and memory are already supplied by softpc_machine_options. */
static CHAR softpc_empty_config_value[] = "";
static const CHAR *softpc_hdd_config_paths[2];
extern char *softpc_platform_floppy_config_value(void);
#define SOFTPC_CONFIG_HARD_DISK1_NAME 25u
#define SOFTPC_CONFIG_HARD_DISK2_NAME 26u
#define SOFTPC_CONFIG_FLOPPY_A_DEVICE 51u
void *config_inquire(host_id, values)
UTINY host_id;
void *values;
{
    UNUSED(values);
    if (host_id == SOFTPC_CONFIG_GFX_ADAPTER)
        return (void *)(ULONG_PTR)SOFTPC_VGA_ADAPTER;
    if (host_id == SOFTPC_CONFIG_HARD_DISK1_NAME)
        return (void *)(softpc_hdd_config_paths[0] != NULL ?
            softpc_hdd_config_paths[0] : softpc_empty_config_value);
    if (host_id == SOFTPC_CONFIG_HARD_DISK2_NAME)
        return (void *)(softpc_hdd_config_paths[1] != NULL ?
            softpc_hdd_config_paths[1] : softpc_empty_config_value);
    if (host_id == SOFTPC_CONFIG_FLOPPY_A_DEVICE)
        return softpc_platform_floppy_config_value();
    return softpc_empty_config_value;
}

/* Original ROM loading is a machine resource lookup.  The standalone VM has
   a fixed firmware directory rather than an NT resource provider. */
long host_read_resource(resource_id, name, destination, maximum, binary)
int resource_id;
char *name;
host_addr destination;
int maximum;
int binary;
{
    char path[260];
    FILE *file;
    size_t bytes;

    UNUSED(resource_id);
    UNUSED(binary);
    if (name == NULL || destination == 0 || maximum <= 0 ||
        (strcmp(name, "bios1.rom") != 0 &&
         strcmp(name, "bios4.rom") != 0 &&
         strcmp(name, "v7vga.rom") != 0))
        return 0L;
    sprintf(path, "firmware/roms/%s", name);
    file = fopen(path, "rb");
    if (file == NULL)
        return 0L;
    bytes = fread((void *)destination, 1u, (size_t)maximum, file);
    fclose(file);
    return (long)bytes;
}

void config_get(host_id, values)
UTINY host_id;
void **values;
{
    UNUSED(host_id);
    if (values != NULL) *values = NULL;
}

SHORT config_put(host_id, error_data)
UTINY host_id;
void *error_data;
{
    UNUSED(host_id);
    UNUSED(error_data);
    return 0;
}

SHORT host_runtime_inquire(what)
UTINY what;
{
    if (what == C_SOUND_ON) return TRUE;
    return 0;
}

/* GFI's old product configuration callbacks are retained as inert fixed-VM
   ports.  Actual image attachment installs the standalone GFI server directly
   and does not use a runtime configuration graph. */
void host_runtime_set(what, value)
UTINY what;
SHORT value;
{
    UNUSED(what);
    UNUSED(value);
}

void config_activate(host_id, req_state)
UTINY host_id;
BOOL req_state;
{
    UNUSED(host_id);
    UNUSED(req_state);
}

BOOL config_get_active(host_id)
UTINY host_id;
{
    if (host_id < SOFTPC_CONFIG_HARD_DISK1_NAME ||
        host_id > SOFTPC_CONFIG_HARD_DISK2_NAME)
        return FALSE;
    return softpc_hdd_config_paths[host_id -
        SOFTPC_CONFIG_HARD_DISK1_NAME] != NULL;
}

SHORT host_gfi_rdiskette_valid(host_id, values, error)
UTINY host_id;
ConfigValues *values;
CHAR *error;
{
    UNUSED(host_id);
    UNUSED(values);
    UNUSED(error);
    return C_CONFIG_OP_OK;
}

SHORT host_gfi_rdiskette_active(host_id, active, error)
UTINY host_id;
BOOL active;
CHAR *error;
{
    UNUSED(host_id);
    UNUSED(active);
    UNUSED(error);
    return C_CONFIG_OP_OK;
}

void host_gfi_rdiskette_change(host_id, apply)
UTINY host_id;
BOOL apply;
{
    UNUSED(host_id);
    UNUSED(apply);
}

long host_time(long *location)
{
    long value = (long)time(NULL);
    if (location != NULL) *location = value;
    return value;
}

struct host_tm *host_localtime(time_t *clock_value)
{
    static struct host_tm result;
    struct tm *native_time;
    if (clock_value == NULL) return NULL;
    native_time = localtime(clock_value);
    if (native_time == NULL) return NULL;
    result.tm_sec = native_time->tm_sec;
    result.tm_min = native_time->tm_min;
    result.tm_hour = native_time->tm_hour;
    result.tm_mday = native_time->tm_mday;
    result.tm_mon = native_time->tm_mon;
    result.tm_year = native_time->tm_year;
    result.tm_wday = native_time->tm_wday;
    result.tm_yday = native_time->tm_yday;
    result.tm_isdst = native_time->tm_isdst;
    return &result;
}

/* The original 8253 remains responsible for guest time.  These functions
   are only its standalone host ports: wall-clock sampling, bounded slice
   scheduling, host-speed calibration, and optional speaker presentation. */
void host_gettimeofday(struct host_timeval *value, struct host_timezone *zone)
{
    FILETIME file_time;
    ULARGE_INTEGER ticks;
    const ULONGLONG unix_epoch_in_filetime = 116444736000000000ULL;
    ULONGLONG microseconds;

    if (value == NULL) return;
    GetSystemTimeAsFileTime(&file_time);
    ticks.LowPart = file_time.dwLowDateTime;
    ticks.HighPart = file_time.dwHighDateTime;
    microseconds = (ticks.QuadPart - unix_epoch_in_filetime) / 10ULL;
    value->tv_sec = (IS32)(microseconds / 1000000ULL);
    value->tv_usec = (IS32)(microseconds % 1000000ULL);
    if (zone != NULL) {
        zone->tz_minuteswest = 0;
        zone->tz_dsttime = 0;
    }
}

IU32 host_speed(IU32 nominal_instructions)
{
    return nominal_instructions == 0u ? 1u : nominal_instructions;
}

void host_timer2_waveform(unsigned int delay, unsigned long low_clocks,
    unsigned long high_clocks, int starts_low, int repeats)
{
    UNUSED(delay);
    UNUSED(starts_low);
    UNUSED(repeats);
#ifdef _WIN32
    unsigned long period = low_clocks + high_clocks;
    unsigned long frequency = period == 0ul ? 0ul :
        SOFTPC_SPEAKER_CLOCK_HZ / period;
    if (frequency < SOFTPC_SPEAKER_MIN_HZ ||
        frequency > SOFTPC_SPEAKER_MAX_HZ)
        frequency = 0ul;
    InterlockedExchange(&softpc_speaker_frequency, (LONG)frequency);
    if (frequency != 0ul &&
        InterlockedCompareExchange(&softpc_speaker_enabled, 0, 0) != 0)
        softpc_speaker_wake_worker();
    else if (softpc_speaker_wake != NULL)
        SetEvent(softpc_speaker_wake);
#else
    UNUSED(low_clocks);
    UNUSED(high_clocks);
#endif
}

void host_timer_init(void)
{
    /* The VM loop calls the original time_strobe() once per bounded slice. */
}

/* Standalone LPT port: the original printer controller owns all guest
   registers, ACK transitions and IRQ behavior.  This host edge simply
   accepts output and reports an always-ready virtual endpoint. */
unsigned long host_lpt_status(int adapter)
{
    UNUSED(adapter);
    return 0u;
}

BOOL host_print_byte(int adapter, byte value)
{
    UNUSED(adapter);
    UNUSED(value);
    return TRUE;
}

BOOL host_print_doc(int adapter)
{
    UNUSED(adapter);
    return TRUE;
}

void host_reset_print(int adapter)
{
    UNUSED(adapter);
}

void host_print_auto_feed(int adapter, BOOL auto_feed)
{
    UNUSED(adapter);
    UNUSED(auto_feed);
}

/* Original UART controller host port.  Controller registers, IRQ selection,
   FIFO handling and BIOS semantics all remain in com.c/rs232_io.c.  The
   standalone VM supplies a connected, idle endpoint until a console serial
   backend is attached. */
void host_com_reset(adapter)
int adapter;
{
    UNUSED(adapter);
}

void host_com_close(adapter)
int adapter;
{
    UNUSED(adapter);
}

void host_com_read(adapter, value, error_mask)
int adapter;
UTINY *value;
int *error_mask;
{
    UNUSED(adapter);
    if (value != NULL) *value = 0;
    if (error_mask != NULL) *error_mask = HOST_COM_NO_DATA;
}

void host_com_write(adapter, value)
int adapter;
char value;
{
    UNUSED(adapter);
    UNUSED(value);
}

void host_com_ioctl(adapter, request, argument)
int adapter;
int request;
intptr_t argument;
{
    UNUSED(adapter);
    if (request == HOST_COM_INPUT_READY && argument != 0) {
        *(int *)argument = FALSE;
    } else if (request == HOST_COM_MODEM && argument != 0) {
        *(int *)argument = HOST_COM_MODEM_CTS | HOST_COM_MODEM_DSR |
            HOST_COM_MODEM_RLSD;
    }
}

void host_com_xon_change(host_id, apply)
IU8 host_id;
IBOOL apply;
{
    UNUSED(host_id);
    UNUSED(apply);
}

void host_com_send_delay_done(adapter, delay)
long adapter;
int delay;
{
    UNUSED(adapter);
    UNUSED(delay);
}

void host_com_msr_callback(adapter, status)
int adapter;
half_word status;
{
    UNUSED(adapter);
    UNUSED(status);
}

quick_event_delays host_delays = { 0, 0, 0, 0, 0, 0, 25000 };
extern void AT_kbd_init(void);
extern void AT_kbd_post(void);
extern void host_key_down(int key);
extern void host_key_up(int key);
extern int keyba_set1_scan_to_key(half_word scan);

static void softpc_keyboard_host_void(void)
{
}

static void softpc_keyboard_host_lights()
{
}

static KEYBDFUNCS softpc_keyboard_host_functions = {
    softpc_keyboard_host_void,
    softpc_keyboard_host_void,
    softpc_keyboard_host_void,
    softpc_keyboard_host_void,
    softpc_keyboard_host_lights,
    softpc_keyboard_host_lights
};
KEYBDFUNCS *working_keybd_funcs = &softpc_keyboard_host_functions;

SHORT host_error(error_number, options, extra_text)
int error_number;
int options;
char *extra_text;
{
    UNUSED(error_number);
    UNUSED(options);
    UNUSED(extra_text);
    return 0;
}

void softpc_platform_keyboard_reset(void)
{
    extern void keyboard_init(void);
    extern void keyboard_post(void);

    keyboard_init();
    keyboard_post();
    AT_kbd_init();
    AT_kbd_post();
}

int softpc_platform_keyboard_scancode(IU8 scan_code)
{
    int key = keyba_set1_scan_to_key((half_word)(scan_code & 0x7fu));
    if (key < 0) return 0;
    if ((scan_code & 0x80u) != 0u) host_key_up(key);
    else host_key_down(key);
    return 1;
}

typedef struct softpc_disk_media {
    FILE *file;
    int writable;
    IU32 total_sectors;
} softpc_disk_media;

static softpc_disk_media softpc_hdd_media[2];
#define SOFTPC_DISK_SECTOR_BYTES 512u

static int softpc_hdd_attach_media(softpc_disk_media *media, const char *path)
{
    media->file = NULL;
    media->writable = 0;
    media->total_sectors = 0u;
    if (path == NULL) return 1;
    media->file = fopen(path, "rb+");
    if (media->file != NULL) media->writable = 1;
    else media->file = fopen(path, "rb");
    if (media->file != NULL) {
        long bytes;
        if (fseek(media->file, 0L, SEEK_END) != 0) goto attach_failed;
        bytes = ftell(media->file);
        if (bytes < 0 || fseek(media->file, 0L, SEEK_SET) != 0)
            goto attach_failed;
        media->total_sectors = (IU32)((unsigned long)bytes /
            SOFTPC_DISK_SECTOR_BYTES);
    }
    return 1;
attach_failed:
    fclose(media->file);
    media->file = NULL;
    media->writable = 0;
    return 0;
}

int softpc_platform_hdd_attach(const char *hard_disk_path)
{
    unsigned int index;
    for (index = 0u; index < 2u; ++index) {
        if (softpc_hdd_media[index].file != NULL)
            fclose(softpc_hdd_media[index].file);
        softpc_hdd_media[index].file = NULL;
        softpc_hdd_media[index].writable = 0;
        softpc_hdd_media[index].total_sectors = 0u;
    }
    /* Fixed disks belong solely to the original fixed-disk controller.
       Removable media is attached separately through original FLA/GFI/FDC. */
    softpc_hdd_config_paths[0] = hard_disk_path;
    softpc_hdd_config_paths[1] = NULL;
    if (!softpc_hdd_attach_media(&softpc_hdd_media[0], softpc_hdd_config_paths[0])) return 0;
    if (!softpc_hdd_attach_media(&softpc_hdd_media[1], softpc_hdd_config_paths[1])) {
        if (softpc_hdd_media[0].file != NULL) fclose(softpc_hdd_media[0].file);
        softpc_hdd_media[0].file = NULL;
        softpc_hdd_config_paths[0] = NULL;
        softpc_hdd_config_paths[1] = NULL;
        return 0;
    }
    return 1;
}

void softpc_platform_hdd_detach(void)
{
    unsigned int index;
    fdisk_iodetach();
    for (index = 0u; index < 2u; ++index) {
        if (softpc_hdd_media[index].file != NULL)
            fclose(softpc_hdd_media[index].file);
        softpc_hdd_media[index].file = NULL;
        softpc_hdd_media[index].writable = 0;
        softpc_hdd_media[index].total_sectors = 0u;
    }
    softpc_hdd_config_paths[0] = NULL;
    softpc_hdd_config_paths[1] = NULL;
}

void host_fdisk_get_params(driveid, cylinders, heads, sectors)
int driveid;
int *cylinders;
int *heads;
int *sectors;
{
    IU32 total_sectors = 0u;
    if (driveid >= 0 && driveid < 2)
        total_sectors = softpc_hdd_media[driveid].total_sectors;
    *heads = 16;
    *sectors = 63;
    *cylinders = (int)(total_sectors / ((IU32)*heads * (IU32)*sectors));
    if (*cylinders < 1) *cylinders = 1;
    if (*cylinders > 16383) *cylinders = 16383;
}

int host_fdisk_rd(driveid, offset, sectors, buffer)
int driveid;
int offset;
int sectors;
char *buffer;
{
    softpc_disk_media *media;
    size_t bytes;
    if (driveid < 0 || driveid >= 2 || sectors < 0) return 0;
    media = &softpc_hdd_media[driveid];
    bytes = (size_t)sectors * SOFTPC_DISK_SECTOR_BYTES;
    if (media->file == NULL || fseek(media->file, (long)offset, SEEK_SET) != 0 ||
        fread(buffer, 1u, bytes, media->file) != bytes) {
        if (media->file != NULL) clearerr(media->file);
        return 0;
    }
    return 1;
}

int host_fdisk_wt(driveid, offset, sectors, buffer)
int driveid;
int offset;
int sectors;
char *buffer;
{
    softpc_disk_media *media;
    size_t bytes;
    if (driveid < 0 || driveid >= 2 || sectors < 0) return 0;
    media = &softpc_hdd_media[driveid];
    bytes = (size_t)sectors * SOFTPC_DISK_SECTOR_BYTES;
    if (media->file == NULL || !media->writable ||
        fseek(media->file, (long)offset, SEEK_SET) != 0 ||
        fwrite(buffer, 1u, bytes, media->file) != bytes || fflush(media->file) != 0) {
        if (media->file != NULL) clearerr(media->file);
        return 0;
    }
    return 1;
}

void host_fdisk_seek0(driveid)
int driveid;
{
    if (driveid >= 0 && driveid < 2 && softpc_hdd_media[driveid].file != NULL)
        (void)fseek(softpc_hdd_media[driveid].file, 0L, SEEK_SET);
}

UTINY *host_sas_init(sys_addr size)
{
    softpc_ram = (UTINY *)calloc((size_t)size + 0x2000u, 1u);
    softpc_ram_size = softpc_ram == NULL ? 0 : size;
    return softpc_ram;
}

UTINY *host_sas_term(void)
{
    free(softpc_ram);
    softpc_ram = NULL;
    softpc_ram_size = 0;
    return NULL;
}

int softpc_platform_write_physical(IU32 address, const IU8 *bytes, IU32 length)
{
    if (bytes == NULL || address > softpc_ram_size ||
        length > softpc_ram_size - address) return 0;
    memcpy(softpc_ram + address, bytes, length);
    return 1;
}

int softpc_platform_read_physical(IU32 address, IU8 *bytes, IU32 length)
{
    if (bytes == NULL || address > softpc_ram_size ||
        length > softpc_ram_size - address) return 0;
    /* A physical bus read must honour SAS_VIDEO and other original mapping
       types.  Reading host_sas memory directly bypasses C-VID's EGA planes,
       making B8000h appear empty even after a guest has written text. */
    c_sas_loads(address, bytes, length);
    return 1;
}

void host_set_hw_int(void)
{
    /* ica.h's historical CPU_HW_INT macro is the PIC bitmap bit (zero),
       whereas CCPU's CPU_INT_TYPE hardware-interrupt enum is value two.
       Keep the machine port on the executor ABI rather than the PIC macro. */
    c_cpu_interrupt((CPU_INT_TYPE)2, 0);
}

void host_clear_hw_int(void)
{
}

void PurgeLostIretHookLine(line_num, depth)
IU16 line_num;
IU32 depth;
{
    UNUSED(line_num);
    UNUSED(depth);
}

void host_timer_event(void)
{
}

void host_note_queue_added(IU32 value)
{
    UNUSED(value);
}

static IUH softpc_clock_ticks(void)
{
    return (IUH)((clock() * 1000000u) / CLOCKS_PER_SEC);
}

void host_q_write_timestamp(QTIMESTAMP_PTR stamp)
{
    IUH now = softpc_clock_ticks();
    stamp->data[0] = now;
    stamp->data[1] = 0;
}

IUH host_q_timestamp_diff(QTIMESTAMP_PTR begin, QTIMESTAMP_PTR end)
{
    return end->data[0] - begin->data[0];
}

IUH host_get_jump_restart(void)
{
    return 1;
}

IUH host_get_q_calib_val(void)
{
    return 1;
}

extern int soft_reset;

/* Ctrl-Alt-Del reaches this original keyboard BIOS hook.  The next machine
   reset remains owned by the public machine lifecycle. */
void reboot(void)
{
    soft_reset = 1;
}

void (*BIOS[256])() = { 0 };

/* Original reset.c asks the host only for presentation metadata and lifecycle
 * notifications.  A standalone machine has no product shell to notify. */
CHAR *host_get_version(void) { return ""; }
CHAR *host_get_unpublished_version(void) { return ""; }
CHAR *host_get_years(void) { return ""; }
CHAR *host_get_copyright(void) { return ""; }
void NIDDB_System_Reboot(void) { }
void host_timer_shutdown(void)
{
#ifdef _WIN32
    softpc_speaker_shutdown();
#endif
}
void host_reset(void) { }
/* The detached executor has no product logger.  Keep its optional diagnostic
 * stream valid so CCPU fault reports remain usable during standalone testing. */
FILE *trace_file = NULL;

static SHORT softpc_error_ignore()
{
    return 0;
}

static ERRORFUNCS softpc_error_functions = {
    softpc_error_ignore,
    softpc_error_ignore,
    softpc_error_ignore
};
ERRORFUNCS *working_error_funcs = &softpc_error_functions;

void LIM_b_write(sys_addr intel_addr)
{
    UNUSED(intel_addr);
}

void LIM_w_write(sys_addr intel_addr)
{
    UNUSED(intel_addr);
}

#ifdef host_simulate
#undef host_simulate
#endif
/* CPU_40's original host contract dispatches through this slot.  The
 * standalone machine has one executor, so bind it directly at load time. */
void (*host_simulate_func)() = c_cpu_simulate;
void host_simulate(void)
{
    c_cpu_simulate();
}
