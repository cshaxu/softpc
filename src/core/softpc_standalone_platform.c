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
 * notification.  There is no VDM session, BOP dispatcher, DOS service or
 * Windows NT process integration here.
 */

extern void c_cpu_simulate();

static UTINY *softpc_ram;
static sys_addr softpc_ram_size;
IU32 softpc_ccpu_instruction_budget = 0;

#define SOFTPC_KEYBOARD_QUEUE_SIZE 32u
static IU8 softpc_keyboard_queue[SOFTPC_KEYBOARD_QUEUE_SIZE];
static IU32 softpc_keyboard_head;
static IU32 softpc_keyboard_tail;

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
    c_cpu_interrupt(CPU_HW_INT, 0);
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
