#include "insignia.h"
#include "host_def.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "cpu4.h"
#include "cpu_vid.h"
#include "error.h"
#include "ios.h"
#include "ica.h"
#include "timestmp.h"

/*
 * Minimal host ports for the detached CCPU.  These are deliberately machine
 * mechanics only: RAM allocation, a monotonic event clock, and interrupt
 * notification.  There is no product-session or service-dispatch layer here.
 */

extern void c_cpu_simulate();

static UTINY *softpc_ram;
static sys_addr softpc_ram_size;
IU32 softpc_ccpu_instruction_budget = 0;

#define SOFTPC_KEYBOARD_QUEUE_SIZE 32u
static IU8 softpc_keyboard_queue[SOFTPC_KEYBOARD_QUEUE_SIZE];
static IU32 softpc_keyboard_head;
static IU32 softpc_keyboard_tail;

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

static void softpc_keyboard_inb(port, value)
io_addr port;
IU8 *value;
{
    if (port == 0x60u) {
        if (softpc_keyboard_head == softpc_keyboard_tail) *value = 0;
        else {
            *value = softpc_keyboard_queue[softpc_keyboard_tail];
            softpc_keyboard_tail = (softpc_keyboard_tail + 1u) % SOFTPC_KEYBOARD_QUEUE_SIZE;
        }
    } else {
        *value = softpc_keyboard_head == softpc_keyboard_tail ? 0u : 1u;
    }
}

static void softpc_keyboard_outb(port, value)
io_addr port;
IU8 value;
{
    UNUSED(port);
    UNUSED(value);
}

void softpc_platform_keyboard_init(void)
{
    softpc_keyboard_head = 0;
    softpc_keyboard_tail = 0;
    io_define_inb(AT_KEYB_ADAPTOR, softpc_keyboard_inb);
    io_define_outb(AT_KEYB_ADAPTOR, softpc_keyboard_outb);
    io_connect_port(0x60u, AT_KEYB_ADAPTOR, IO_READ | IO_WRITE);
    io_connect_port(0x64u, AT_KEYB_ADAPTOR, IO_READ | IO_WRITE);
}

int softpc_platform_keyboard_scancode(IU8 scan_code)
{
    IU32 next = (softpc_keyboard_head + 1u) % SOFTPC_KEYBOARD_QUEUE_SIZE;
    if (next == softpc_keyboard_tail) return 0;
    softpc_keyboard_queue[softpc_keyboard_head] = scan_code;
    softpc_keyboard_head = next;
    ica_hw_interrupt(ICA_MASTER, CPU_KB_INT, 1);
    return 1;
}

static FILE *softpc_hdd_file;
static IU8 softpc_hdd_sector_count;
static IU8 softpc_hdd_lba_low;
static IU8 softpc_hdd_lba_mid;
static IU8 softpc_hdd_lba_high;
static IU8 softpc_hdd_drive_head;
static IU8 softpc_hdd_status;
#define SOFTPC_ATA_SECTOR_BYTES 512u
#define SOFTPC_ATA_MAX_SECTORS 128u
static IU8 softpc_hdd_buffer[SOFTPC_ATA_SECTOR_BYTES * SOFTPC_ATA_MAX_SECTORS];
static IU32 softpc_hdd_buffer_offset;
static IU32 softpc_hdd_transfer_bytes;
static int softpc_hdd_write_active;
static int softpc_hdd_writable;
static IU32 softpc_hdd_total_sectors;

static void softpc_hdd_inb(port, value)
io_addr port;
IU8 *value;
{
    if (port == 0x1f0u && softpc_hdd_status == 0x48u &&
        softpc_hdd_buffer_offset < softpc_hdd_transfer_bytes) {
        *value = softpc_hdd_buffer[softpc_hdd_buffer_offset++];
        if (softpc_hdd_buffer_offset == softpc_hdd_transfer_bytes)
            softpc_hdd_status = 0x40u;
    } else if (port == 0x1f7u || port == 0x3f6u) {
        *value = softpc_hdd_status;
    } else {
        *value = 0u;
    }
}

static void softpc_hdd_inw(port, value)
io_addr port;
word *value;
{
    IU8 low;
    IU8 high;
    softpc_hdd_inb(port, &low);
    softpc_hdd_inb(port, &high);
    *value = (word)((IU16)low | ((IU16)high << 8u));
}

static void softpc_hdd_read_sectors(void)
{
    unsigned long lba;
    IU32 sectors = softpc_hdd_sector_count;
    IU32 transfer_bytes;
    if (sectors == 0u) sectors = 256u;
    if (softpc_hdd_file == NULL || sectors > SOFTPC_ATA_MAX_SECTORS) {
        softpc_hdd_status = 0x41u;
        return;
    }
    transfer_bytes = sectors * SOFTPC_ATA_SECTOR_BYTES;
    lba = (unsigned long)softpc_hdd_lba_low |
        ((unsigned long)softpc_hdd_lba_mid << 8u) |
        ((unsigned long)softpc_hdd_lba_high << 16u) |
        ((unsigned long)(softpc_hdd_drive_head & 0x0fu) << 24u);
    if (fseek(softpc_hdd_file, (long)(lba * SOFTPC_ATA_SECTOR_BYTES), SEEK_SET) != 0 ||
        fread(softpc_hdd_buffer, 1u, transfer_bytes, softpc_hdd_file) != transfer_bytes) {
        clearerr(softpc_hdd_file);
        softpc_hdd_status = 0x41u;
        return;
    }
    softpc_hdd_buffer_offset = 0;
    softpc_hdd_transfer_bytes = transfer_bytes;
    softpc_hdd_status = 0x48u;
}

static unsigned long softpc_hdd_lba(void)
{
    return (unsigned long)softpc_hdd_lba_low |
        ((unsigned long)softpc_hdd_lba_mid << 8u) |
        ((unsigned long)softpc_hdd_lba_high << 16u) |
        ((unsigned long)(softpc_hdd_drive_head & 0x0fu) << 24u);
}

static void softpc_hdd_begin_write(void)
{
    IU32 sectors = softpc_hdd_sector_count;
    if (sectors == 0u) sectors = 256u;
    if (softpc_hdd_file == NULL || !softpc_hdd_writable ||
        sectors > SOFTPC_ATA_MAX_SECTORS) {
        softpc_hdd_status = 0x41u;
        return;
    }
    softpc_hdd_transfer_bytes = sectors * SOFTPC_ATA_SECTOR_BYTES;
    softpc_hdd_buffer_offset = 0u;
    softpc_hdd_write_active = 1;
    softpc_hdd_status = 0x48u;
}

static void softpc_hdd_put_identify_word(IU32 word_index, word value)
{
    softpc_hdd_buffer[word_index * 2u] = (IU8)value;
    softpc_hdd_buffer[word_index * 2u + 1u] = (IU8)(value >> 8u);
}

static void softpc_hdd_identify(void)
{
    IU32 cylinders;
    if (softpc_hdd_file == NULL) {
        softpc_hdd_status = 0x41u;
        return;
    }
    memset(softpc_hdd_buffer, 0, SOFTPC_ATA_SECTOR_BYTES);
    cylinders = softpc_hdd_total_sectors / (16u * 63u);
    if (cylinders == 0u) cylinders = 1u;
    if (cylinders > 16383u) cylinders = 16383u;
    softpc_hdd_put_identify_word(0u, 0x0040u);
    softpc_hdd_put_identify_word(1u, (word)cylinders);
    softpc_hdd_put_identify_word(3u, 16u);
    softpc_hdd_put_identify_word(6u, 63u);
    softpc_hdd_put_identify_word(49u, 0x0200u);
    softpc_hdd_put_identify_word(53u, 0x0001u);
    softpc_hdd_put_identify_word(60u, (word)softpc_hdd_total_sectors);
    softpc_hdd_put_identify_word(61u, (word)(softpc_hdd_total_sectors >> 16u));
    softpc_hdd_buffer_offset = 0u;
    softpc_hdd_transfer_bytes = SOFTPC_ATA_SECTOR_BYTES;
    softpc_hdd_write_active = 0;
    softpc_hdd_status = 0x48u;
}

static void softpc_hdd_data_write(IU8 value)
{
    unsigned long lba;
    if (!softpc_hdd_write_active || softpc_hdd_status != 0x48u ||
        softpc_hdd_buffer_offset >= softpc_hdd_transfer_bytes) return;
    softpc_hdd_buffer[softpc_hdd_buffer_offset++] = value;
    if (softpc_hdd_buffer_offset != softpc_hdd_transfer_bytes) return;
    lba = softpc_hdd_lba();
    if (fseek(softpc_hdd_file, (long)(lba * SOFTPC_ATA_SECTOR_BYTES), SEEK_SET) != 0 ||
        fwrite(softpc_hdd_buffer, 1u, softpc_hdd_transfer_bytes,
            softpc_hdd_file) != softpc_hdd_transfer_bytes ||
        fflush(softpc_hdd_file) != 0) {
        clearerr(softpc_hdd_file);
        softpc_hdd_status = 0x41u;
    } else softpc_hdd_status = 0x40u;
    softpc_hdd_write_active = 0;
}

static void softpc_hdd_outb(port, value)
io_addr port;
IU8 value;
{
    switch (port) {
    case 0x1f0u: softpc_hdd_data_write(value); break;
    case 0x1f2u: softpc_hdd_sector_count = value; break;
    case 0x1f3u: softpc_hdd_lba_low = value; break;
    case 0x1f4u: softpc_hdd_lba_mid = value; break;
    case 0x1f5u: softpc_hdd_lba_high = value; break;
    case 0x1f6u: softpc_hdd_drive_head = value; break;
    case 0x1f7u:
        if (value == 0x20u) softpc_hdd_read_sectors();
        else if (value == 0x30u) softpc_hdd_begin_write();
        else if (value == 0xecu) softpc_hdd_identify();
        break;
    case 0x3f6u:
        if (value & 0x04u) softpc_hdd_status = 0x40u;
        break;
    }
}

static void softpc_hdd_outw(port, value)
io_addr port;
word value;
{
    if (port == 0x1f0u) {
        softpc_hdd_data_write((IU8)value);
        softpc_hdd_data_write((IU8)(value >> 8u));
    } else softpc_hdd_outb(port, (IU8)value);
}

int softpc_platform_hdd_attach(const char *path)
{
    if (softpc_hdd_file != NULL) fclose(softpc_hdd_file);
    softpc_hdd_file = NULL;
    softpc_hdd_status = 0x40u;
    softpc_hdd_buffer_offset = 0;
    softpc_hdd_transfer_bytes = 0;
    softpc_hdd_write_active = 0;
    softpc_hdd_writable = 0;
    softpc_hdd_total_sectors = 0u;
    if (path == NULL) return 1;
    softpc_hdd_file = fopen(path, "rb+");
    if (softpc_hdd_file != NULL) softpc_hdd_writable = 1;
    else softpc_hdd_file = fopen(path, "rb");
    if (softpc_hdd_file != NULL) {
        long bytes;
        if (fseek(softpc_hdd_file, 0L, SEEK_END) != 0) goto attach_failed;
        bytes = ftell(softpc_hdd_file);
        if (bytes < 0 || fseek(softpc_hdd_file, 0L, SEEK_SET) != 0)
            goto attach_failed;
        softpc_hdd_total_sectors = (IU32)((unsigned long)bytes /
            SOFTPC_ATA_SECTOR_BYTES);
    }
    return softpc_hdd_file != NULL;
attach_failed:
    fclose(softpc_hdd_file);
    softpc_hdd_file = NULL;
    softpc_hdd_writable = 0;
    return 0;
}

void softpc_platform_hdd_detach(void)
{
    if (softpc_hdd_file != NULL) fclose(softpc_hdd_file);
    softpc_hdd_file = NULL;
    softpc_hdd_status = 0x40u;
    softpc_hdd_buffer_offset = 0;
    softpc_hdd_transfer_bytes = 0;
    softpc_hdd_write_active = 0;
    softpc_hdd_writable = 0;
    softpc_hdd_total_sectors = 0u;
}

void softpc_platform_hdd_init(void)
{
    io_addr port;
    io_define_in_routines(HDA_ADAPTOR, softpc_hdd_inb, softpc_hdd_inw, 0, 0);
    io_define_out_routines(HDA_ADAPTOR, softpc_hdd_outb, softpc_hdd_outw,
        0, 0);
    for (port = 0x1f0u; port <= 0x1f7u; ++port)
        io_connect_port(port, HDA_ADAPTOR, IO_READ | IO_WRITE);
    io_connect_port(0x3f6u, HDA_ADAPTOR, IO_READ | IO_WRITE);
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

void rom_init(void)
{
}
