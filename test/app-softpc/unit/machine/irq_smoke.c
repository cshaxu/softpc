#include "lib/types/types_interface.h"
#include "machine/machine.h"
#include "../../fixture_cleanup.h"

#include <assert.h>
#include <stdio.h>

#include "insignia.h"
#include "host_def.h"
#include "ios.h"
#include "ica.h"
#include "quick_ev.h"
#include "compat/ccpu/abi.h"
#include "compat/ccpu/lifecycle.h"
#include "compat/platform.h"
#include "compat/devices/snapshot.h"
#include "app-softpc/softpc.new/base/ccpu386/c_intr.h"

extern void reboot(void);
extern void host_set_hw_int(void);
extern void host_key_down(int key);
extern void host_key_up(int key);
extern void continue_output(void);

static void verify_rejected_interrupt(softpc_machine *machine)
{
    IU8 master_mask, slave_mask;
    IS32 rejected;
    unsigned short cs = c_getCS();
    unsigned long ip = c_getEIP(), sp = c_getESP(), flags = c_getEFLAGS();
    unsigned long stack = c_getSS_BASE() + ((sp - 6u) & 0xffffu);
    unsigned char before[6], after[6];

    inb(0x21u, &master_mask);
    inb(0xa1u, &slave_mask);
    outb(0x21u, 0xffu);
    outb(0xa1u, 0xffu);
    rejected = ica_intack();
    assert(rejected == -1);
    assert(softpc_machine_read_physical(machine, stack, before,
        sizeof(before)) == SOFTPC_MACHINE_OK);

    /* Exercise the real PIC result through the original CPU entry, not a
       duplicate predicate. Rejection must not push an interrupt frame. */
    do_intrupt((IU16)rejected, FALSE, FALSE, 0u);
    assert(c_getCS() == cs && c_getEIP() == ip);
    assert(c_getESP() == sp && c_getEFLAGS() == flags);
    assert(softpc_machine_read_physical(machine, stack, after,
        sizeof(after)) == SOFTPC_MACHINE_OK);
    assert(lib_memory_compare(before, after, sizeof(before)) == 0);

    /* A stale CPU request must also survive the actual instruction loop.
       The boot program is parked on JMP $, with interrupts enabled. */
    assert((flags & 0x200u) != 0u);
    host_set_hw_int();
    assert(softpc_machine_run(machine, 32u) == SOFTPC_MACHINE_OK);
    assert((*softpc_ccpu_interrupt_map_address() & 1u) == 0u);
    assert(c_getCS() == cs && c_getEIP() == ip);
    assert(c_getESP() == sp && c_getEFLAGS() == flags);
    outb(0xa1u, slave_mask);
    outb(0x21u, master_mask);
}

static void verify_keyboard_read_releases_irq(void)
{
    half_word irr = 0;
    half_word response = 0;

    /* A keyboard-device response raises IRQ1.  Reading its one-byte output
       buffer must lower the same line even if the guest polls the PIC while
       IRQ1 is masked instead of accepting the interrupt. */
    outb(0x20u, 0x0au);
    outb(0x60u, 0xf2u);
    inb(0x20u, &irr);
    assert((irr & 0x02u) != 0u);
    inb(0x60u, &response);
    assert(response == 0xfau);
    inb(0x20u, &irr);
    assert((irr & 0x02u) == 0u);
}

static void verify_scan_input(void)
{
    half_word status, value;
    unsigned released;
    /* Actual Compat ingress through the original keyboard controller. The
       ROM has selected translated Scan-1 output; make/break must survive. */
    for (released = 0; released < 2; ++released) {
        unsigned char scan = (unsigned char)(0x1eu | (released << 7));
        assert(softpc_platform_keyboard_scancode(scan));
        inb(0x64u, &status);
        assert((status & 1u) != 0u);
        inb(0x60u, &value);
        assert(value == scan);
    }
    assert(!softpc_platform_keyboard_scancode(0x54u));
    assert(!softpc_platform_keyboard_scancode(0xd4u));
    inb(0x64u, &status);
    assert((status & 1u) == 0u);
}

static void keyboard_command(half_word value)
{
    half_word status, reply;
    outb(0x60u, value);
    inb(0x64u, &status);
    assert((status & 1u) != 0u);
    inb(0x60u, &reply);
    assert(reply == 0xfau);
}

static half_word keyboard_read(void)
{
    half_word status, value;
    unsigned events;
    continue_output();
    inb(0x64u, &status);
    /* Consecutive bytes use the original delayed-refill/interrupt queue,
       unlike a single controller command's immediate acknowledgement. */
    for (events = 0; !(status & 1u) && events != 64; ++events) {
        dispatch_q_event();
        inb(0x64u, &status);
    }
    assert((status & 1u) != 0u);
    inb(0x60u, &value);
    return value;
}

static void keyboard_expect(half_word expected)
{
    assert(keyboard_read() == expected);
}

static void verify_keyboard_translation(void)
{
    softpc_device_keyboard_state saved, state;
    lib_u32 fingerprint = 2166136261u;
    unsigned code;
    assert(softpc_device_snapshot_capture_keyboard(&saved));
    for (code = 0; code < 256; ++code) {
        state = saved;
        state.translating = 1;
        state.interface_disabled = 0;
        state.interrupt_enabled = 0;
        state.output_full = state.pending_8042 = state.waiting_for_upcode = 0;
        state.status &= ~1u;
        state.fifo_count = 1;
        state.fifo[0] = (lib_u16)code;
        assert(softpc_device_snapshot_restore_keyboard(&state));
        if (code == 0xf0u) {
            /* A split break prefix produces no byte until its successor. */
            continue_output();
            assert(softpc_device_snapshot_capture_keyboard(&state));
            assert(state.waiting_for_upcode && !state.output_full);
            assert(state.fifo_count == 0 && !(state.status & 1u));
            state.fifo_count = 1;
            state.fifo[0] = 0x1cu; /* A: Set 2 -> Set 1 break 0x9e. */
            assert(softpc_device_snapshot_restore_keyboard(&state));
            keyboard_expect(0x9eu);
        } else {
            fingerprint = (fingerprint ^ keyboard_read()) * 16777619u;
        }
    }
    /* Fixed XP IBM-PC translation table, excluding the separately tested
       F0 control prefix. No production table is read to derive this value. */
    assert(fingerprint == 0x9bd36c6fu);
    assert(softpc_device_snapshot_restore_keyboard(&saved));
}

static void verify_keyboard_sets(void)
{
    static const struct { unsigned key; half_word make[3]; } added[] = {
        { 14, { 0x7d, 0x6a, 0x5d } }, { 56, { 0x73, 0x51, 0x51 } },
        { 94, { 0x7c, 0x68, 0x68 } }, { 107, { 0x7e, 0x6d, 0x7b } }
    };
    softpc_device_keyboard_state saved, state, restored;
    unsigned set, item, type, key;
    lib_memory_set(&saved, 0, sizeof(saved));
    assert(softpc_device_snapshot_capture_keyboard(&saved));
    /* Disable translation, leaving the physical controller and its normal
       command/response path in charge. Do not set scan_set through a fixture. */
    outb(0x64u, 0x60u);
    outb(0x60u, 0x01u);
    for (set = 1; set <= 3; ++set) {
        keyboard_command(0xf0u);
        keyboard_command((half_word)set);
        lib_memory_set(&state, 0, sizeof(state));
        assert(softpc_device_snapshot_capture_keyboard(&state));
        assert(state.scan_set == (int)set);
        assert(softpc_device_snapshot_restore_keyboard(&state));
        lib_memory_set(&restored, 0, sizeof(restored));
        assert(softpc_device_snapshot_capture_keyboard(&restored));
        assert(lib_memory_compare(&state, &restored, sizeof(state)) == 0);
        keyboard_command(0xfau); /* Enable make/break for every set-3 key. */
        for (item = 0; item < sizeof(added) / sizeof(added[0]); ++item) {
            half_word code = added[item].make[set - 1];
            host_key_down((int)added[item].key);
            /* XP retains key 14 as category 6 (not a physical key) even
               though its scan tables contain reserved values. */
            if (added[item].key == 14) {
                host_key_up((int)added[item].key);
                assert(softpc_device_snapshot_capture_keyboard(&state));
                assert(state.fifo_count == 0 && !state.output_full);
                assert(state.key_down_count[14] == 0);
                continue;
            }
            keyboard_expect(code);
            host_key_up((int)added[item].key);
            if (set != 1) keyboard_expect(0xf0u);
            keyboard_expect(set == 1 ? (half_word)(code | 0x80u) : code);
        }
    }
    for (item = 1; item < sizeof(added) / sizeof(added[0]); ++item)
    for (type = 1; type <= 3; ++type) {
        /* First put every key in type 4; the per-key command must alter
           exactly its own physical key. Reserved key 14 has no inverse. */
        keyboard_command(0xfau);
        keyboard_command((half_word)(0xfau + type));
        keyboard_command(added[item].make[2]);
        assert(softpc_device_snapshot_capture_keyboard(&state));
        for (key = 1; key < 127; ++key)
            assert(state.set_3_key_state[key] ==
                (key == added[item].key ? type : 4u));
    }
    keyboard_command(0xf6u);
    assert(softpc_device_snapshot_capture_keyboard(&state));
    assert(state.waiting_for_next_code == 0);
    assert(state.keyboard_disabled == 0);
    assert(softpc_device_snapshot_restore_keyboard(&saved));
}

int main(void)
{
    const char *path = "softpc-machine-irq-smoke.img";
    unsigned char sector[512] = { 0 };
    unsigned char marker = 0;
    unsigned char prior_user_timer[4];
    const unsigned char user_timer_vector[] = {0x00u, 0x09u, 0x00u, 0x00u};
    /* CS is zero even when the original BIOS invokes INT 1Ch with another DS. */
    const unsigned char user_timer[] = {0x2eu, 0xfeu, 0x06u, 0x01u, 0x05u, 0xcfu};
    half_word reset_command = 0xfeu;
    lib_u16 cs = 0u;
    lib_u32 eip = 0u;
    unsigned char program[] = {
        /* Re-enter the same boot sector through a nonzero CS. */
        0xeau, 0x10u, 0x00u, 0xc0u, 0x07u,
        0x90u, 0x90u, 0x90u, 0x90u, 0x90u, 0x90u, 0x90u, 0x90u,
        0x90u, 0x90u, 0x90u,
        /* Program the PIT with one instruction per tick while IF is clear. */
        0xfau, 0x31u, 0xc0u, 0x8eu, 0xd8u,
        /* Exercise the nonzero, odd stack shape used by the real loader. */
        0xb8u, 0x84u, 0x9fu, 0x8eu, 0xd0u, 0xbcu, 0xe3u, 0x00u,
        0xb0u, 0x34u, 0xe6u, 0x43u, 0xb0u, 0x01u, 0xe6u, 0x40u,
        0xb0u, 0x00u, 0xe6u, 0x40u,
        /* Enable interrupts and mark boot completion. The high byte at 0501h
           starts the following IRQ proof at zero, independently of host time. */
        0xfbu, 0xb8u, 0x5au, 0x00u, 0xa3u, 0x00u, 0x05u, 0xebu, 0xfeu
    };
    softpc_machine_options options = { path, NULL };
    softpc_machine *machine = NULL;
    FILE *file;

    lib_memory_copy(sector, program, sizeof(program));
    sector[510] = 0x55u; sector[511] = 0xaau;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fclose(file) == 0);

    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    /* The original ROM POST is part of the machine path.  A slice measured
       in the old replacement core's bootstrap instructions cannot reach a
       boot sector after that POST, so use a bounded machine slice instead. */
    assert(softpc_machine_run(machine, 6000u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_read_physical(machine, 0x500u, &marker, 1u) == SOFTPC_MACHINE_OK);
    assert(marker == 0x5au);
    /* Preserve the real BIOS IRQ0 handler. Its INT 1Ch callback proves actual
       delivery, whereas a nonzero BDA timestamp does not prove any interrupt. */
    assert(softpc_machine_read_physical(machine, 0x70u, prior_user_timer,
        sizeof(prior_user_timer)) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_write_physical(machine, 0x900u, user_timer,
        sizeof(user_timer)) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_write_physical(machine, 0x70u, user_timer_vector,
        sizeof(user_timer_vector)) == SOFTPC_MACHINE_OK);
    ica_hw_interrupt(ICA_MASTER, 0u, 1);
    assert(softpc_machine_run(machine, 256u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_read_physical(machine, 0x501u, &marker, 1u) == SOFTPC_MACHINE_OK);
    assert(marker != 0u);
    assert(softpc_machine_write_physical(machine, 0x70u, prior_user_timer,
        sizeof(prior_user_timer)) == SOFTPC_MACHINE_OK);
    verify_rejected_interrupt(machine);
    verify_scan_input();
    verify_keyboard_sets();
    verify_keyboard_translation();
    verify_keyboard_read_releases_irq();

    /* The original 8042 output-port pulse requests a CPU reset through the
       original keyboard controller; it is not a standalone reset shortcut. */
    outb(0x64u, reset_command);
    assert(softpc_machine_run(machine, 1u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_instruction_pointer(machine, &cs, &eip) ==
        SOFTPC_MACHINE_OK);
    assert(cs == 0xf000u);
    assert(eip == 0xfff0u);

    /* Ctrl+Alt+Del is decoded by the original BIOS keyboard service, which
       calls the historical reboot host callback. The standalone callback
       must assert that same CCPU reset line; it must not merely set the
       soft-reset classification flag and leave execution in DOS. */
    reboot();
    assert(softpc_machine_run(machine, 1u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_instruction_pointer(machine, &cs, &eip) ==
        SOFTPC_MACHINE_OK);
    assert(cs == 0xf000u);
    assert(eip == 0xfff0u);

    softpc_machine_destroy(machine);
    assert(softpc_test_remove_image(path));
    return 0;
}
