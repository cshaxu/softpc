#include "machine.h"
#include "test_cleanup.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>

#include "insignia.h"
#include "host_def.h"
#include "host_com.h"
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
    const char *output_path = "softpc-serial-output.bin";
    softpc_machine_options options = { path, NULL,
        SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    half_word status = 0u;
    UTINY received = 0xffu;
    int error_mask = 0;
    int input_ready = TRUE;
    int modem = 0;

    make_boot_disk(path);
    assert(remove(output_path) == 0 || errno == ENOENT);
    options.serial_output_path = output_path;
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);

    /* The original UART remains the controller.  Exercise the recovered
       host edge independently: fixed-profile COM1 is connected, has no
       synthetic input and accepts/flushes output without an NT driver. */
    host_com_ioctl(0, HOST_COM_MODEM, (intptr_t)&modem);
    assert((modem & (HOST_COM_MODEM_CTS | HOST_COM_MODEM_DSR |
        HOST_COM_MODEM_RLSD)) == (HOST_COM_MODEM_CTS | HOST_COM_MODEM_DSR |
        HOST_COM_MODEM_RLSD));
    host_com_ioctl(0, HOST_COM_INPUT_READY, (intptr_t)&input_ready);
    assert(!input_ready);
    host_com_read(0, &received, &error_mask);
    assert(error_mask == HOST_COM_NO_DATA);
    host_com_write(0, 'S');
    host_com_ioctl(0, HOST_COM_FLUSH, 0);

    inb(RS232_COM1_PORT_START + SOFTPC_RS232_LSR, &status);
    assert((status & 0x60u) == 0x60u);
    inb(RS232_COM1_PORT_START + SOFTPC_RS232_MSR, &status);
    assert((status & 0x10u) != 0u);
    outb(RS232_COM1_PORT_START + SOFTPC_RS232_TX_RX, 0x41u);
    inb(RS232_COM1_PORT_START + SOFTPC_RS232_LSR, &status);
    assert((status & 0x60u) == 0x60u);

    /* Reset must retain the configured host endpoint without retaining UART
       state or leaking an extra endpoint handle. */
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    host_com_write(0, 'R');

    softpc_machine_destroy(machine);
    {
        unsigned char output[3];
        FILE *file = fopen(output_path, "rb");
        assert(file != NULL);
        assert(fread(output, 1u, sizeof(output), file) == sizeof(output));
        assert(output[0] == 'S');
        assert(output[1] == 'A');
        assert(output[2] == 'R');
        assert(fgetc(file) == EOF);
        assert(fclose(file) == 0);
    }
    assert(remove(output_path) == 0);
    assert(softpc_test_remove_image(path));
    return 0;
}
