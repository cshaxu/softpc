/*
 * Standalone carrier for the original nt_com.c host contract.
 *
 * The original UART, IRQ and pacing state remain in base/comms/com.c.  This
 * file keeps the host-side port state that nt_com.c maintained, but replaces
 * its NT serial driver, OVERLAPPED requests and receive thread with a
 * bounded non-blocking virtual endpoint.  The fixed machine intentionally
 * has no implicit host serial device: receive queues start empty and output
 * goes to a virtual sink.
 */
#include "insignia.h"
#include "host_def.h"

#include "xt.h"
#include "host_com.h"

#include <stdio.h>
#include <string.h>

#define SOFTPC_COM_PORTS 4
#define SOFTPC_COM_QUEUE_SIZE 256
#define SOFTPC_COM_OUTPUT_PATH_MAX 1024

typedef struct {
    UTINY rx[SOFTPC_COM_QUEUE_SIZE];
    unsigned int rx_head;
    unsigned int rx_tail;
    unsigned int rx_count;
    unsigned int tx_count;
    int baud;
    int data_bits;
    int stop_bits;
    int parity;
    int break_enabled;
    int dtr;
    int rts;
    int opened;
    int xon_enabled;
    int modem;
    half_word last_msr;
    FILE *output;
    char output_path[SOFTPC_COM_OUTPUT_PATH_MAX];
} HOST_COM;

static HOST_COM host_com[SOFTPC_COM_PORTS];

static HOST_COM *host_com_port(int adapter)
{
    if (adapter < 0 || adapter >= SOFTPC_COM_PORTS) return 0;
    return &host_com[adapter];
}

int softpc_host_com_set_output_path(int adapter, const char *path)
{
    HOST_COM *port = host_com_port(adapter);
    size_t length;
    if (port == 0) return FALSE;
    if (path == 0) {
        port->output_path[0] = '\0';
        return TRUE;
    }
    length = strlen(path);
    if (length >= sizeof(port->output_path)) return FALSE;
    memcpy(port->output_path, path, length + 1u);
    return TRUE;
}

void host_com_init(adapter)
int adapter;
{
    HOST_COM *port = host_com_port(adapter);
    if (port != 0) {
        port->opened = FALSE;
        port->rx_head = 0;
        port->rx_tail = 0;
        port->rx_count = 0;
        port->tx_count = 0;
    }
}

int host_com_open(adapter)
int adapter;
{
    HOST_COM *port = host_com_port(adapter);
    if (port == 0) return FALSE;
    if (port->output_path[0] != '\0' && port->output == 0) {
        port->output = fopen(port->output_path, "ab");
        if (port->output == 0) return FALSE;
    }
    port->opened = TRUE;
    return TRUE;
}

void host_com_reset(adapter)
int adapter;
{
    HOST_COM *port = host_com_port(adapter);
    if (port == 0) return;
    port->rx_head = 0;
    port->rx_tail = 0;
    port->rx_count = 0;
    port->tx_count = 0;
    port->baud = HOST_COM_B9600;
    port->data_bits = 8;
    port->stop_bits = 1;
    port->parity = HOST_COM_PARITY_NONE;
    port->break_enabled = FALSE;
    port->dtr = TRUE;
    port->rts = TRUE;
    port->opened = TRUE;
    port->xon_enabled = FALSE;
    port->modem = HOST_COM_MODEM_CTS | HOST_COM_MODEM_DSR |
        HOST_COM_MODEM_RLSD;
}

void host_com_close(adapter)
int adapter;
{
    HOST_COM *port = host_com_port(adapter);
    if (port == 0) return;
    port->opened = FALSE;
    if (port->output != 0) {
        fclose(port->output);
        port->output = 0;
    }
    port->rx_head = 0;
    port->rx_tail = 0;
    port->rx_count = 0;
    port->tx_count = 0;
}

void host_com_close_all(void)
{
    int adapter;
    for (adapter = 0; adapter < SOFTPC_COM_PORTS; ++adapter)
        host_com_close(adapter);
}

/* The original NT adapter used the heartbeat to harvest asynchronous serial
 * driver I/O.  This standalone backend has no host serial handle or worker;
 * its RX/TX state is synchronous, but retains the original event port so the
 * machine heartbeat has the same controller-facing shape. */
void host_com_heart_beat(void)
{
}

void host_com_read(adapter, value, error_mask)
int adapter;
UTINY *value;
int *error_mask;
{
    HOST_COM *port = host_com_port(adapter);
    if (value != 0) *value = 0;
    if (error_mask != 0) *error_mask = HOST_COM_NO_DATA;
    if (port == 0 || port->rx_count == 0) return;
    if (value != 0) *value = port->rx[port->rx_tail];
    port->rx_tail = (port->rx_tail + 1u) % SOFTPC_COM_QUEUE_SIZE;
    --port->rx_count;
    if (error_mask != 0) *error_mask = 0;
}

void host_com_write(adapter, value)
int adapter;
char value;
{
    HOST_COM *port = host_com_port(adapter);
    /* The historical controller opened a product-host endpoint only when an
       application touched UART line control.  The detached build instead
       opens a configured synchronous output endpoint at its first original
       transmit callback. */
    if (port == 0) return;
    if ((!port->opened || (port->output_path[0] != '\0' &&
        port->output == 0)) && !host_com_open(adapter)) return;
    if (port->output != 0) {
        if (fputc((unsigned char)value, port->output) == EOF ||
            fflush(port->output) != 0) return;
    }
    /* nt_com.c's WriteFile exit is a successful virtual sink here.  Keep a
       bounded accounting value so FLUSH and close retain original meaning. */
    if (port->tx_count != SOFTPC_COM_QUEUE_SIZE) ++port->tx_count;
}

void host_com_ioctl(adapter, request, argument)
int adapter;
int request;
intptr_t argument;
{
    HOST_COM *port = host_com_port(adapter);
    if (port == 0) return;
    switch (request) {
    case HOST_COM_SBRK: port->break_enabled = TRUE; break;
    case HOST_COM_CBRK: port->break_enabled = FALSE; break;
    case HOST_COM_SDTR: port->dtr = TRUE; break;
    case HOST_COM_CDTR: port->dtr = FALSE; break;
    case HOST_COM_SRTS: port->rts = TRUE; break;
    case HOST_COM_CRTS: port->rts = FALSE; break;
    case HOST_COM_BAUD: port->baud = (int)argument; break;
    case HOST_COM_DATABITS: port->data_bits = (int)argument; break;
    case HOST_COM_STOPBITS: port->stop_bits = (int)argument; break;
    case HOST_COM_PARITY: port->parity = (int)argument; break;
    case HOST_COM_FLUSH: port->tx_count = 0; break;
    case HOST_COM_INPUT_READY:
        if (argument != 0) *(int *)argument = port->rx_count != 0;
        break;
    case HOST_COM_MODEM:
        if (argument != 0) *(int *)argument = port->modem;
        break;
    }
}

void host_com_xon_change(host_id, apply)
IU8 host_id;
IBOOL apply;
{
    HOST_COM *port = host_com_port((int)host_id);
    if (port != 0) port->xon_enabled = apply != 0;
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
    HOST_COM *port = host_com_port(adapter);
    if (port != 0) port->last_msr = status;
}
