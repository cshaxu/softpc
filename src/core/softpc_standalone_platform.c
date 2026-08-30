#include "insignia.h"
#include "host_def.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "cpu4.h"
#include "cpu_vid.h"
#include "error.h"
#include "fdisk.h"
#include "ios.h"
#include "ica.h"
#include "timestmp.h"
#include "timeval.h"
#include "timer.h"
#include "keyboard.h"

/*
 * Minimal host ports for the detached CCPU.  These are deliberately machine
 * mechanics only: RAM allocation, a monotonic event clock, and interrupt
 * notification.  There is no product-session or service-dispatch layer here.
 */

extern void c_cpu_simulate();

static UTINY *softpc_ram;
static sys_addr softpc_ram_size;
IU32 softpc_ccpu_instruction_budget = 0;

#define SOFTPC_CONFIG_GFX_ADAPTER 54u
#define SOFTPC_VGA_ADAPTER 5u

static void softpc_timer2_gate(port, value)
io_addr port;
half_word value;
{
    UNUSED(port);
    UNUSED(value);
}

void (*timer_gate_func) IPT2(io_addr, port, half_word, value) =
    softpc_timer2_gate;

void host_enable_timer2_sound(void)
{
}

void host_disable_timer2_sound(void)
{
}

/* The original keyboard BIOS uses this as a bounded audible indication.
   The standalone console has no mandatory sound backend yet, so the machine
   preserves the timing contract without inventing a host product service. */
void host_alarm(long duration)
{
    UNUSED(duration);
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
#define SOFTPC_CONFIG_HARD_DISK1_NAME 25u
#define SOFTPC_CONFIG_HARD_DISK2_NAME 26u
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
    UNUSED(what);
    return 0;
}

SHORT gfi_drive_type(drive)
UTINY drive;
{
    UNUSED(drive);
    return 0;
}

void set_tod(void)
{
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

quick_event_delays host_delays = { 0, 0, 0, 0, 0, 0, 25000 };
extern void AT_kbd_init(void);
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

/* Fixed-machine 8253 channel 0.  The run-loop advances it in guest
 * instruction time and routes expiry through the machine PIC. */
static IU32 softpc_pit_reload = 65536u;
static IU32 softpc_pit_elapsed;
static IU32 softpc_pit_ticks;
static IU8 softpc_pit_write_low;
static IU8 softpc_pit_read_low;

static void softpc_pit_inb(port, value)
io_addr port;
IU8 *value;
{
    if (port != 0x40u) {
        *value = 0u;
        return;
    }
    if (!softpc_pit_read_low) {
        *value = (IU8)(softpc_pit_reload & 0xffu);
        softpc_pit_read_low = 1u;
    } else {
        *value = (IU8)((softpc_pit_reload >> 8u) & 0xffu);
        softpc_pit_read_low = 0u;
    }
}

static void softpc_pit_outb(port, value)
io_addr port;
IU8 value;
{
    if (port == 0x43u) {
        if ((value & 0xc0u) == 0u) softpc_pit_write_low = 0u;
        return;
    }
    if (port != 0x40u) return;
    if (!softpc_pit_write_low) {
        softpc_pit_reload = (softpc_pit_reload & 0xff00u) | value;
        softpc_pit_write_low = 1u;
    } else {
        softpc_pit_reload = (softpc_pit_reload & 0x00ffu) | ((IU32)value << 8u);
        if (softpc_pit_reload == 0u) softpc_pit_reload = 65536u;
        softpc_pit_elapsed = 0u;
        softpc_pit_write_low = 0u;
    }
}

void softpc_platform_timer_init(void)
{
    softpc_pit_reload = 65536u;
    softpc_pit_elapsed = 0u;
    softpc_pit_ticks = 0u;
    softpc_pit_write_low = 0u;
    softpc_pit_read_low = 0u;
    io_define_inb(TIMER_ADAPTOR, softpc_pit_inb);
    io_define_outb(TIMER_ADAPTOR, softpc_pit_outb);
    io_connect_port(0x40u, TIMER_ADAPTOR, IO_READ | IO_WRITE);
    io_connect_port(0x43u, TIMER_ADAPTOR, IO_WRITE);
}

void softpc_platform_timer_advance(IU32 instructions)
{
    softpc_pit_elapsed += instructions;
    while (softpc_pit_elapsed >= softpc_pit_reload) {
        softpc_pit_elapsed -= softpc_pit_reload;
        ++softpc_pit_ticks;
        ica_hw_interrupt(ICA_MASTER, CPU_TIMER_INT, 1);
    }
}

IU32 softpc_platform_timer_ticks(void)
{
    return softpc_pit_ticks;
}

void softpc_platform_keyboard_init(void)
{
    extern void keyboard_init(void);

    AT_kbd_init();
    keyboard_init();
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

int softpc_platform_hdd_attach(const char *floppy_path, const char *hard_disk_path)
{
    unsigned int index;
    for (index = 0u; index < 2u; ++index) {
        if (softpc_hdd_media[index].file != NULL)
            fclose(softpc_hdd_media[index].file);
        softpc_hdd_media[index].file = NULL;
        softpc_hdd_media[index].writable = 0;
        softpc_hdd_media[index].total_sectors = 0u;
    }
    /* The temporary bootstrap still supplies a floppy image as its first
       block device.  Preserve that ordering until the original FDC/BIOS
       path replaces the bootstrap, while the port controller itself is the
       original SoftPC fdisk implementation. */
    softpc_hdd_config_paths[0] = floppy_path != NULL ? floppy_path : hard_disk_path;
    softpc_hdd_config_paths[1] = floppy_path != NULL ? hard_disk_path : NULL;
    if (!softpc_hdd_attach_media(&softpc_hdd_media[0], softpc_hdd_config_paths[0])) return 0;
    if (!softpc_hdd_attach_media(&softpc_hdd_media[1], softpc_hdd_config_paths[1])) {
        if (softpc_hdd_media[0].file != NULL) fclose(softpc_hdd_media[0].file);
        softpc_hdd_media[0].file = NULL;
        softpc_hdd_config_paths[0] = NULL;
        softpc_hdd_config_paths[1] = NULL;
        return 0;
    }
    hda_init();
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

void softpc_platform_hdd_init(void)
{
    /* hda_init runs after the concrete image paths are attached. */
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

void fast_disk_bios_attach(driveid)
int driveid;
{
    UNUSED(driveid);
}

void fast_disk_bios_detach(driveid)
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
    memcpy(bytes, softpc_ram + address, length);
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

int soft_reset = 1;

/* Ctrl-Alt-Del reaches this original keyboard BIOS hook.  The next machine
   reset remains owned by the public machine lifecycle. */
void reboot(void)
{
    soft_reset = 1;
}

void (*BIOS[256])() = { 0 };
/* The detached executor has no product logger.  Keep its optional diagnostic
 * stream valid so CCPU fault reports remain usable during standalone testing. */
FILE *trace_file = NULL;
READ_POINTERS read_pointers = { 0 };

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

void host_simulate(void)
{
    c_cpu_simulate();
}
