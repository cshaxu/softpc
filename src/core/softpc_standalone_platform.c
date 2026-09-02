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
#include "egamode.h"
#include "gvi.h"
#include "egagraph.h"
#include "egavideo.h"
#include "video.h"
#include "host_com.h"
#include "virtual.h"
#include "softpc_standalone_dib.h"
#include "nt_graph.h"
#include "nt_ega.h"
#include "softpc_machine.h"
#include "softpc_ccpu_lifecycle.h"
#include "softpc_host_input.h"

/*
 * Minimal host ports for the detached CCPU.  These are deliberately machine
 * mechanics only: RAM allocation, a monotonic event clock, and interrupt
 * notification.  There is no product-session or service-dispatch layer here.
 */

extern void c_cpu_simulate();
static UTINY *softpc_ram;
static sys_addr softpc_ram_size;
IU32 softpc_ccpu_instruction_budget = 0;
IBOOL softpc_ccpu_instruction_budget_active = FALSE;

extern void softpc_standalone_sound_timer2_gate(half_word value);

static void softpc_standalone_timer_gate(io_addr port, half_word value)
{
    SWTMR_gate(port, value);
    if (port == TIMER2_REG)
        softpc_standalone_sound_timer2_gate(value);
}

void softpc_platform_install_timer2_sound_gate(void)
{
    timer_gate_func = softpc_standalone_timer_gate;
    softpc_standalone_sound_timer2_gate(GATE_SIGNAL_RISE);
}

#ifdef _WIN32
/* Original timestrb.c documents a host alarm of roughly 20 Hz.  The timer
   queue callback only marks the CCPU event pending; host_timer_event() stays
   on the machine/executor thread where the original device state lives. */
static HANDLE softpc_clock_timer;
static HANDLE softpc_executor_event;
static volatile LONG softpc_clock_pending_ticks;
static volatile LONG softpc_executor_wake_pending;
static void (*softpc_executor_callback)(void *);
static void *softpc_executor_callback_context;
void softpc_platform_executor_event(void);

static IBOOL softpc_platform_take_pending(volatile LONG *pending)
{
    LONG observed;

    if (*pending <= 0) return FALSE;
    do {
        observed = InterlockedCompareExchange(pending, 0, 0);
        if (observed <= 0) return FALSE;
    } while (InterlockedCompareExchange(pending, observed - 1, observed) !=
        observed);
    return TRUE;
}

static VOID CALLBACK softpc_clock_tick(PVOID context, BOOLEAN fired)
{
    UNUSED(context);
    UNUSED(fired);
    /* The timer thread only records the original host heartbeat.  CCPU's
       generated inter-instruction/HLT mailbox consumes it on the executor
       thread and then calls host_timer_event, avoiding cross-thread CPU state
       mutation. */
    (void)InterlockedIncrement(&softpc_clock_pending_ticks);
    if (softpc_executor_event != NULL) SetEvent(softpc_executor_event);
}
#endif

IBOOL softpc_platform_consume_clock_tick(void)
{
#ifdef _WIN32
    return softpc_platform_take_pending(&softpc_clock_pending_ticks);
#else
    return FALSE;
#endif
}

/* A frontend may request that an HLT loop return to its owning executor so
 * queued keyboard/mouse input can be applied.  This is deliberately separate
 * from the 20Hz device clock: waking must not advance the PIT, video, or
 * controller state. */
void softpc_platform_request_executor_wake(void)
{
#ifdef _WIN32
    (void)InterlockedExchange(&softpc_executor_wake_pending, 1);
    if (softpc_executor_event != NULL) SetEvent(softpc_executor_event);
#endif
}

IBOOL softpc_platform_consume_executor_wake(void)
{
#ifdef _WIN32
    return softpc_platform_take_pending(&softpc_executor_wake_pending);
#else
    return FALSE;
#endif
}

/* A bounded public machine run is measured only at the generated CCPU
 * inter-instruction safe point.  The continuous VM executor supplies the
 * maximum native budget and is stopped through its separate lifecycle port. */
IBOOL softpc_platform_consume_instruction_budget(void)
{
    if (!softpc_ccpu_instruction_budget_active ||
        softpc_ccpu_instruction_budget == 0u)
        return FALSE;
    --softpc_ccpu_instruction_budget;
    return softpc_ccpu_instruction_budget == 0u;
}


/* Called only from the standalone CCPU HLT path after both pending sources
 * have been checked.  The auto-reset event avoids a polling spin: either the
 * original host timer or frontend input wakes this executor immediately. */
void softpc_platform_wait_for_executor_event(void)
{
#ifdef _WIN32
    if (softpc_executor_event != NULL)
        (void)WaitForSingleObject(softpc_executor_event, INFINITE);
    else
        Sleep(1u);
#endif
}

void softpc_platform_set_boot_clock(int active)
{
#ifdef _WIN32
    /* Timer availability is owned by original timer.c.  Reset may call this
       legacy lifecycle hook, but it must not manufacture guest heartbeats. */
    if (!active) (void)InterlockedExchange(&softpc_clock_pending_ticks, 0);
#else
    UNUSED(active);
#endif
}

void softpc_platform_set_runtime_heartbeat(int enabled)
{
    UNUSED(enabled);
}

void softpc_platform_set_executor_callback(void (*callback)(void *),
    void *context)
{
    softpc_executor_callback_context = context;
    softpc_executor_callback = callback;
}

/* These are controller buffers, not a second video implementation.  The
   original platform allocated them while bringing up its UI; the detached
   platform owns the same allocation at its presentation boundary instead. */
extern byte *EGA_planes;
extern byte *video_copy;
extern PC_palette *DAC;
extern IU8 Video_mode;
extern IU8 Currently_emulated_video_mode;
extern void nt_init_screen(void);
extern void nt_graphics_tick(void);
extern void nt_change_plane_mask(int plane_mask);
extern void nt_mark_screen_refresh(void);

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

/* Transitional duplicate video vtable retained for audit only.  Original
   nt_graph owns the sole compiled VIDEOFUNCS state below. */
#if 0
static void softpc_video_void(void) {}
static void softpc_video_init_screen(void)
{
    if (video_copy == NULL) video_copy = (byte *)calloc(1u, 0x20000u);
    if (EGA_planes == NULL)
        EGA_planes = (byte *)calloc(4u, (size_t)EGA_PLANE_SIZE);
    if (DAC == NULL) DAC = (PC_palette *)calloc(VGA_DAC_SIZE, sizeof(*DAC));
    (void)softpc_standalone_dib_init();
    nt_init_screen();
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
static int softpc_video_cursor_valid;
static long softpc_video_cursor_x;
static long softpc_video_cursor_y;
static void softpc_video_cursor(int x, int y, half_word attribute)
{
    softpc_video_cursor_x = x;
    softpc_video_cursor_y = y;
    softpc_video_cursor_valid = 1;
    UNUSED(attribute);
}
static void softpc_video_two_ints(int first, int second)
{ UNUSED(first); UNUSED(second); }

/* This is the machine-only half of original nt_graph.c's nt_graphics_tick:
   VGA register writes deliberately settle for EGA_TICK_DELAY heartbeats
   before choose_display_mode selects the original paint routine.  The
   standalone frontend has no console-server flush, mouse-delay, or cursor
   resize work, so none of those NTVDM product branches belong here. */
static int softpc_video_mode_change_ticks;
static int softpc_video_flush_ticks;
static void softpc_video_graphics_tick(void)
{
    if (video_adapter != EGA && video_adapter != VGA) return;
    if (softpc_video_mode_change_ticks != 0) {
        if (--softpc_video_mode_change_ticks == 0) {
            (void)(*choose_display_mode)();
            set_mode_change_required(FALSE);
        }
    } else if (get_mode_change_required()) {
        softpc_video_mode_change_ticks = EGA_TICK_DELAY - 1;
    } else if (++softpc_video_flush_ticks == 2) {
        /* The original nt_graphics_tick coalesces C-VID dirty marks for two
           machine ticks before its host painter consumes them. */
        nt_graphics_tick();
        (void)(*update_alg.calc_update)();
        softpc_video_flush_ticks = 0;
    }
}

/* Original nt_graph.c owns the paint vectors and the complete controller-mode
 * selection state machine.  This remains only the standalone host vtable. */
extern void nt_set_paint_routine(DISPLAY_MODE mode, int height);

static VIDEOFUNCS softpc_video_functions = {
    softpc_video_init_screen, softpc_video_init_adaptor, softpc_video_void,
    softpc_video_int, softpc_video_palette, softpc_video_int,
    softpc_video_void, softpc_video_void, nt_mark_screen_refresh,
    softpc_video_graphics_tick, softpc_video_void, softpc_video_void,
    softpc_video_scroll, softpc_video_scroll, (void (*)())softpc_video_cursor,
    (void (*)())nt_set_paint_routine, nt_change_plane_mask, softpc_video_void,
    softpc_video_two_ints, softpc_video_int, softpc_video_int,
    softpc_video_int, softpc_video_two_ints, softpc_video_two_ints,
    softpc_video_void
};
#endif

extern VIDEOFUNCS nt_video_funcs;

int softpc_platform_presentation_is_graphics(void)
{
    return sc.ModeType == GRAPHICS;
}

int softpc_platform_presentation_cursor(long *column_out, long *row_out)
{
    UNUSED(column_out);
    UNUSED(row_out);
    return 0;
}

int softpc_platform_presentation_fonts(uint8_t *primary, uint8_t *secondary,
    unsigned long *height_out, unsigned long *attribute_select_out)
{
    static const unsigned long font_offsets[8] = {
        0u, 0x4000u, 0x8000u, 0xc000u, 0x2000u, 0x6000u, 0xa000u, 0xe000u
    };
    unsigned long height;
    unsigned long font;
    unsigned long character;

    unsigned long secondary_font;

    if (primary == NULL || secondary == NULL || height_out == NULL ||
        attribute_select_out == NULL || EGA_planes == NULL) return 0;
    height = sas_hw_at_no_check(ega_char_height);
    if (height == 0u || height > 16u) height = 16u;
    font = (unsigned long)get_prim_font_index() & 7u;
    secondary_font = (unsigned long)get_sec_font_index() & 7u;
    memset(primary, 0, 256u * 16u);
    memset(secondary, 0, 256u * 16u);
    for (character = 0u; character < 256u; ++character) {
        unsigned long row;
        byte *source = EGA_planes + FONT_BASE_ADDR + (font_offsets[font] << 2) +
            ((unsigned long)FONT_MAX_HEIGHT * character << 2);
        byte *secondary_source = EGA_planes + FONT_BASE_ADDR +
            (font_offsets[secondary_font] << 2) +
            ((unsigned long)FONT_MAX_HEIGHT * character << 2);
        for (row = 0u; row < height; ++row)
        {
            primary[character * 16u + row] = source[row << 2];
            secondary[character * 16u + row] = secondary_source[row << 2];
        }
    }
    *height_out = height;
    *attribute_select_out = get_attrib_font_select() ? 1u : 0u;
    return 1;
}

int softpc_platform_presentation_font(uint8_t *glyphs,
    unsigned long *height_out)
{
    unsigned char secondary[256u * 16u];
    unsigned long attribute_select;

    return softpc_platform_presentation_fonts(glyphs, secondary, height_out,
        &attribute_select);
}

int softpc_platform_video_buffers_init(void)
{
    (void)softpc_standalone_dib_init();
    host_init_screen();
    return video_copy != NULL && EGA_planes != NULL && DAC != NULL;
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

/* Standalone audio is only a presentation sink.  nt_sound.c owns the
 * original PPI/Timer2 state transitions and requests a frequency here. */
#ifdef _WIN32
#define SOFTPC_SPEAKER_MIN_HZ 10ul
#define SOFTPC_SPEAKER_MAX_HZ 20000ul
#define SOFTPC_SPEAKER_SLICE_MS 40u
static HANDLE softpc_speaker_wake;
static HANDLE softpc_speaker_stop;
static HANDLE softpc_speaker_thread;
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
        while (InterlockedCompareExchange(&softpc_speaker_frequency, 0, 0) != 0) {
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

void softpc_standalone_audio_set_tone(ULONG frequency, ULONG duration)
{
    if (duration < 10u || frequency < SOFTPC_SPEAKER_MIN_HZ ||
        frequency > SOFTPC_SPEAKER_MAX_HZ)
        frequency = 0u;
    InterlockedExchange(&softpc_speaker_frequency, (LONG)frequency);
    if (frequency != 0u) softpc_speaker_wake_worker();
    else if (softpc_speaker_wake != NULL) SetEvent(softpc_speaker_wake);
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
    InterlockedExchange(&softpc_speaker_frequency, 0);
}
#endif

/* Original idetect.c calls this only after it has classified repeated failed
   keyboard polls as guest idle.  Yield the host quantum without sleeping or
   advancing machine time; this is the same outer scheduling primitive NXVM
   uses for a core that has no productive work. */
void host_release_timeslice(void)
{
#ifdef _WIN32
    Sleep(0u);
#endif
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
#define SOFTPC_CONFIG_WIN_SIZE 55u

/* The original CMOS controller owns its register values.  This host port
   reports only which concrete fixed-media backends have been attached. */
half_word softpc_host_cmos_fixed_disk_type(void)
{
    half_word disk = 0u;
    if (softpc_hdd_config_paths[0] != NULL &&
        softpc_hdd_config_paths[0][0] != '\0')
        disk = 0x30u;
    if (softpc_hdd_config_paths[1] != NULL &&
        softpc_hdd_config_paths[1][0] != '\0')
        disk |= 0x04u;
    return disk;
}
void *config_inquire(host_id, values)
UTINY host_id;
void *values;
{
    UNUSED(values);
    if (host_id == SOFTPC_CONFIG_GFX_ADAPTER)
        return (void *)(ULONG_PTR)SOFTPC_VGA_ADAPTER;
    /* nt_graph obtains this historical scalar through config_inquire(),
       encoded as a pointer.  The original NT host uses scale 2 by default. */
    if (host_id == SOFTPC_CONFIG_WIN_SIZE)
        return (void *)(ULONG_PTR)2u;
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

/* Original ROM loading is a machine resource lookup.  The standalone VM
   embeds its fixed firmware as RCDATA, so launching it never depends on the
   process working directory or an adjacent ROM directory. */
long host_read_resource(resource_id, name, destination, maximum, binary)
int resource_id;
char *name;
host_addr destination;
int maximum;
int binary;
{
    UNUSED(resource_id);
    UNUSED(binary);
#ifdef _WIN32
    HRSRC resource;
    HGLOBAL loaded;
    DWORD bytes;
    int identifier;

    if (name == NULL || destination == 0 || maximum <= 0) return 0L;
    if (strcmp(name, "bios1.rom") == 0)
        identifier = 101;
    else if (strcmp(name, "bios4.rom") == 0)
        identifier = 102;
    else if (strcmp(name, "v7vga.rom") == 0)
        identifier = 103;
    else
        return 0L;
    resource = FindResourceA(NULL, MAKEINTRESOURCEA(identifier), RT_RCDATA);
    if (resource == NULL || (bytes = SizeofResource(NULL, resource)) == 0u)
        return 0L;
    loaded = LoadResource(NULL, resource);
    if (loaded == NULL) return 0L;
    if (bytes > (DWORD)maximum) bytes = (DWORD)maximum;
    memcpy((void *)destination, LockResource(loaded), (size_t)bytes);
    return (long)bytes;
#else
    UNUSED(name);
    UNUSED(destination);
    UNUSED(maximum);
    return 0L;
#endif
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

void host_timer_init(void)
{
#ifdef _WIN32
    if (softpc_executor_event == NULL)
        softpc_executor_event = CreateEventA(NULL, FALSE, FALSE, NULL);
    if (softpc_clock_timer == NULL) {
        (void)CreateTimerQueueTimer(&softpc_clock_timer, NULL,
            softpc_clock_tick, NULL, 50u, 50u, WT_EXECUTEDEFAULT);
    }
#endif
}

quick_event_delays host_delays = { 0, 0, 0, 0, 0, 0, 25000 };
extern void AT_kbd_init(void);
extern void AT_kbd_post(void);
extern void host_key_down(int key);
extern void host_key_up(int key);

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

int softpc_platform_keyboard_key(int key, int released)
{
    if (key <= 0) return 0;
    if (released) host_key_up(key); else host_key_down(key);
    return 1;
}

int softpc_platform_keyboard_scancode(IU8 scan_code)
{
    int key = softpc_host_scan1_to_key((unsigned int)(scan_code & 0x7fu));
    if (key == 0) return 0;
    if ((scan_code & 0x80u) != 0u) host_key_up(key);
    else host_key_down(key);
    return 1;
}

typedef struct softpc_disk_media {
    FILE *file;
    unsigned char *data;
    size_t data_bytes;
    softpc_media_mode mode;
    IU32 total_sectors;
} softpc_disk_media;

static softpc_disk_media softpc_hdd_media[2];
#define SOFTPC_DISK_SECTOR_BYTES 512u

static int softpc_hdd_attach_media(softpc_disk_media *media, const char *path,
    softpc_media_mode mode)
{
    FILE *file;
    long bytes;

    media->file = NULL;
    media->data = NULL;
    media->data_bytes = 0u;
    media->total_sectors = 0u;
    if (path == NULL) return 1;
    file = fopen(path, mode == SOFTPC_MEDIA_DIRECT ? "rb+" : "rb");
    if (file == NULL) return 0;
    if (fseek(file, 0L, SEEK_END) != 0) goto attach_failed;
    bytes = ftell(file);
    if (bytes < (long)SOFTPC_DISK_SECTOR_BYTES ||
        fseek(file, 0L, SEEK_SET) != 0)
        goto attach_failed;
    if (mode == SOFTPC_MEDIA_OVERLAY) {
        media->data = malloc((size_t)bytes);
        if (media->data == NULL || fread(media->data, 1u, (size_t)bytes, file) !=
            (size_t)bytes)
            goto attach_failed;
        fclose(file);
        media->data_bytes = (size_t)bytes;
    } else {
        media->file = file;
    }
    media->mode = mode;
    media->total_sectors = (IU32)((unsigned long)bytes /
        SOFTPC_DISK_SECTOR_BYTES);
    return 1;
attach_failed:
    fclose(file);
    free(media->data);
    media->data = NULL;
    media->data_bytes = 0u;
    return 0;
}

int softpc_platform_hdd_attach(const char *hard_disk_path, softpc_media_mode mode)
{
    unsigned int index;
    for (index = 0u; index < 2u; ++index) {
        if (softpc_hdd_media[index].file != NULL)
            fclose(softpc_hdd_media[index].file);
        free(softpc_hdd_media[index].data);
        softpc_hdd_media[index].file = NULL;
        softpc_hdd_media[index].data = NULL;
        softpc_hdd_media[index].data_bytes = 0u;
        softpc_hdd_media[index].total_sectors = 0u;
    }
    /* Fixed disks belong solely to the original fixed-disk controller.
       Removable media is attached separately through original FLA/GFI/FDC. */
    softpc_hdd_config_paths[0] = hard_disk_path;
    softpc_hdd_config_paths[1] = NULL;
    if (!softpc_hdd_attach_media(&softpc_hdd_media[0], softpc_hdd_config_paths[0], mode)) return 0;
    if (!softpc_hdd_attach_media(&softpc_hdd_media[1], softpc_hdd_config_paths[1], mode)) {
        if (softpc_hdd_media[0].file != NULL)
            fclose(softpc_hdd_media[0].file);
        free(softpc_hdd_media[0].data);
        softpc_hdd_media[0].file = NULL;
        softpc_hdd_media[0].data = NULL;
        softpc_hdd_media[0].data_bytes = 0u;
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
        free(softpc_hdd_media[index].data);
        softpc_hdd_media[index].file = NULL;
        softpc_hdd_media[index].data = NULL;
        softpc_hdd_media[index].data_bytes = 0u;
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
    if (driveid < 0 || driveid >= 2 || offset < 0 || sectors < 0) return 0;
    media = &softpc_hdd_media[driveid];
    bytes = (size_t)sectors * SOFTPC_DISK_SECTOR_BYTES;
    if ((media->data == NULL && media->file == NULL) || (IU32)offset > media->total_sectors *
        SOFTPC_DISK_SECTOR_BYTES || bytes > (size_t)(media->total_sectors *
        SOFTPC_DISK_SECTOR_BYTES - (IU32)offset)) {
        return 0;
    }
    if (media->mode == SOFTPC_MEDIA_OVERLAY)
        memcpy(buffer, media->data + offset, bytes);
    else if (fseek(media->file, (long)offset, SEEK_SET) != 0 ||
        fread(buffer, 1u, bytes, media->file) != bytes)
        return 0;
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
    if (driveid < 0 || driveid >= 2 || offset < 0 || sectors < 0) return 0;
    media = &softpc_hdd_media[driveid];
    bytes = (size_t)sectors * SOFTPC_DISK_SECTOR_BYTES;
    if ((media->data == NULL && media->file == NULL) ||
        media->mode == SOFTPC_MEDIA_READONLY || (IU32)offset >
        media->total_sectors * SOFTPC_DISK_SECTOR_BYTES || bytes >
        (size_t)(media->total_sectors * SOFTPC_DISK_SECTOR_BYTES - (IU32)offset)) {
        return 0;
    }
    if (media->mode == SOFTPC_MEDIA_OVERLAY)
        memcpy(media->data + offset, buffer, bytes);
    else if (fseek(media->file, (long)offset, SEEK_SET) != 0 ||
        fwrite(buffer, 1u, bytes, media->file) != bytes ||
        fflush(media->file) != 0)
        return 0;
    return 1;
}

void host_fdisk_seek0(driveid)
int driveid;
{
    UNUSED(driveid);
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

PVOID softpc_xms_physical_pointer(ULONG address, ULONG length)
{
    if (softpc_ram == NULL || address > softpc_ram_size ||
        length > softpc_ram_size - address) return NULL;
    return softpc_ram + address;
}

PVOID softpc_xms_guest_pointer(USHORT segment, USHORT offset)
{
    return softpc_xms_physical_pointer(((ULONG)segment << 4) + offset, 1u);
}

BOOL softpc_xms_copy_physical(ULONG destination, ULONG source, ULONG length)
{
    PVOID destination_pointer = softpc_xms_physical_pointer(destination, length);
    PVOID source_pointer = softpc_xms_physical_pointer(source, length);
    if (destination_pointer == NULL || source_pointer == NULL) return FALSE;
    memmove(destination_pointer, source_pointer, length);
    sas_overwrite_memory(destination, length);
    return TRUE;
}

int softpc_platform_write_physical(IU32 address, const IU8 *bytes, IU32 length)
{
    if (bytes == NULL || address > softpc_ram_size ||
        length > softpc_ram_size - address) return 0;
    /* A physical bus write must take the same original SAS route as reads.
       Direct host-RAM copying bypasses SAS_VIDEO, so it fails to update
       C-VID's EGA planes and its original dirty-marking algorithm. */
    c_sas_stores(address, (IU8 *)bytes, length);
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

/* CPU_40's original keyboard controller sets the CCPU reset bit and then
 * calls this historical host notification. CCPU's original NT endpoint is
 * empty; the sole executor is already executing the 8042 command, so no
 * second scheduler or CPU-state owner is needed here. */
void host_cpu_interrupt(void)
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
    /* Preserve the machine-facing portion of the original nt_timer heartbeat.
       Product scheduling (WOW, DEM, console switching and NT worker threads)
       is deliberately absent; these calls advance only restored devices and
       the standalone DIB renderer. */
    host_graphics_tick();
    /* quick_ev.c calibrates the original CCPU time-to-jump conversion from
       this host heartbeat.  It is machine timing support, not a frontend
       cadence or an invented guest tick. */
    quick_tick_recalibrate();
    host_com_heart_beat();
    host_lpt_heart_beat();
    time_strobe();
    PlayContinuousTone();
    softpc_platform_executor_event();
}

void softpc_platform_executor_event(void)
{
    if (softpc_executor_callback != NULL)
        (*softpc_executor_callback)(softpc_executor_callback_context);
    /* The original keyboard polling service may have entered nested
       host_simulate frames.  A standalone stop returns to the outer executor
       frame; it does not alter normal device/BOP unwinds. */
    if (softpc_ccpu_lifecycle_exit_requested())
        softpc_ccpu_lifecycle_return_outer();
}

void host_note_queue_added(IU32 value)
{
    UNUSED(value);
}

/* nt_timer.c used the NT performance counter for CCPU's quick-event
 * recalibration.  Do the same at the standalone host boundary: clock() is
 * process CPU time, so it stops advancing while the window/event loop waits
 * and makes a real machine's elapsed-time accounting depend on host load. */
static IUH softpc_clock_ticks(void)
{
    static LARGE_INTEGER frequency;
    LARGE_INTEGER counter;
    ULONGLONG seconds;
    ULONGLONG remainder;
    ULONGLONG microseconds;

    if (frequency.QuadPart == 0 && !QueryPerformanceFrequency(&frequency))
        return 0;
    if (!QueryPerformanceCounter(&counter)) return 0;
    seconds = (ULONGLONG)counter.QuadPart / (ULONGLONG)frequency.QuadPart;
    remainder = (ULONGLONG)counter.QuadPart % (ULONGLONG)frequency.QuadPart;
    microseconds = seconds * 1000000ULL +
        (remainder * 1000000ULL) / (ULONGLONG)frequency.QuadPart;
    return (IUH)microseconds;
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
    if (softpc_clock_timer != NULL) {
        (void)DeleteTimerQueueTimer(NULL, softpc_clock_timer,
            INVALID_HANDLE_VALUE);
        softpc_clock_timer = NULL;
    }
    if (softpc_executor_event != NULL) CloseHandle(softpc_executor_event);
    softpc_executor_event = NULL;
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
extern ERRORFUNCS *working_error_funcs;

/* `reset.c` remains the original owner of its GWI table storage.  Bind the
   standalone endpoints once before the original firmware reset runs. */
void softpc_platform_bind_reset_host_functions(void)
{
    working_video_funcs = &nt_video_funcs;
    working_keybd_funcs = &softpc_keyboard_host_functions;
    working_error_funcs = &softpc_error_functions;
}

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
