#include "softpc_machine.h"

#include <assert.h>
#include <stdio.h>

static void write_boot_image(const char *path, unsigned char marker)
{
    unsigned char sector[512] = { 0 };
    FILE *file;
    /* mov byte ptr [0500],42; jmp $ */
    sector[0] = 0xc6u; sector[1] = 0x06u; sector[2] = 0x00u;
    sector[3] = 0x05u; sector[4] = marker; sector[5] = 0xebu; sector[6] = 0xfeu;
    sector[510] = 0x55u; sector[511] = 0xaau;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fclose(file) == 0);
}

static void write_keyboard_boot_image(const char *path)
{
    unsigned char sector[512] = { 0 };
    FILE *file;
    /* in al,60h; mov [0500],al; jmp $ */
    sector[0] = 0xe4u; sector[1] = 0x60u; sector[2] = 0xa2u;
    sector[3] = 0x00u; sector[4] = 0x05u; sector[5] = 0xebu; sector[6] = 0xfeu;
    sector[510] = 0x55u; sector[511] = 0xaau;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fclose(file) == 0);
}

static void run_boot_image(const char *path, int floppy, unsigned char expected)
{
    unsigned char marker = 0;
    softpc_machine_options options = { NULL, NULL,
        SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    if (floppy) options.floppy_path = path;
    else options.hard_disk_path = path;
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_run(machine, 4u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_read_physical(machine, 0x500u, &marker, 1u) == SOFTPC_MACHINE_OK);
    assert(marker == expected);
    softpc_machine_destroy(machine);
}

static void run_keyboard_boot_image(const char *path)
{
    unsigned char marker = 0;
    softpc_machine_options options = { path, NULL, SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_key_scancode(machine, 0x1eu) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_run(machine, 4u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_read_physical(machine, 0x500u, &marker, 1u) == SOFTPC_MACHINE_OK);
    assert(marker == 0x1eu);
    softpc_machine_destroy(machine);
}

int main(void)
{
    const char *floppy = "softpc-machine-floppy-smoke.img";
    const char *hdd = "softpc-machine-hdd-smoke.img";
    const char *keyboard = "softpc-machine-keyboard-smoke.img";
    write_boot_image(floppy, 0x42u);
    write_boot_image(hdd, 0x77u);
    write_keyboard_boot_image(keyboard);
    run_boot_image(floppy, 1, 0x42u);
    run_boot_image(hdd, 0, 0x77u);
    run_keyboard_boot_image(keyboard);
    assert(remove(floppy) == 0);
    assert(remove(hdd) == 0);
    assert(remove(keyboard) == 0);
    return 0;
}
