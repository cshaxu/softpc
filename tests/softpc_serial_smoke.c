#include "softpc_machine.h"

#include <assert.h>
#include <stdio.h>

#include "insignia.h"
#include "host_def.h"
#include "ios.h"

enum {
    SOFTPC_RS232_TX_RX = 0,
    SOFTPC_RS232_LSR = 5,
    SOFTPC_RS232_MSR = 6
};

static void make_boot_disk(const char *path)
{
    unsigned char sector[512] = { 0 };
    FILE *file = fopen(path, "wb");
    assert(file != NULL);
    sector[510] = 0x55u;
    sector[511] = 0xaau;
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fseek(file, 1474560L - 1L, SEEK_SET) == 0);
    assert(fputc(0, file) == 0);
    assert(fclose(file) == 0);
}

int main(void)
{
    const char *path = "softpc-serial-smoke.img";
    softpc_machine_options options = { path, NULL,
        SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    half_word status = 0u;

    make_boot_disk(path);
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);

    inb(RS232_COM1_PORT_START + SOFTPC_RS232_LSR, &status);
    assert((status & 0x60u) == 0x60u);
    inb(RS232_COM1_PORT_START + SOFTPC_RS232_MSR, &status);
    assert((status & 0x10u) != 0u);
    outb(RS232_COM1_PORT_START + SOFTPC_RS232_TX_RX, 0x41u);
    inb(RS232_COM1_PORT_START + SOFTPC_RS232_LSR, &status);
    assert((status & 0x60u) == 0x60u);

    softpc_machine_destroy(machine);
    assert(remove(path) == 0);
    return 0;
}
