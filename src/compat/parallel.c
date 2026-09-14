/*
 * Standalone carrier for the original nt_lpt.c host state machine.
 *
 * The parallel controller remains base/comms/printer.c.  This file retains
 * the original host-side buffering, status and lifecycle rules while the
 * NT file/device-control calls are replaced by the standalone VM's fixed
 * virtual printer sink.  No product monitor, DOS-open state or system handle
 * leaks across this boundary.
 */
#include "insignia.h"
#include "host_def.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "xt.h"
#include "host_lpt.h"

#define KBUFFER_SIZE 1024
#define HIGH_WATER 1020
#define DIRECT_ACCESS_HIGH_WATER 1020
#define SOFTPC_LPT_OUTPUT_PATH_MAX 1024

typedef struct {
    unsigned long port_status;
    int inactive_counter;
    int inactive_trigger;
    int bytes_in_buffer;
    int flush_threshold;
    boolean active;
    boolean direct_access;
    boolean no_device_attached;
    byte *buffer;
    FILE *output;
    char output_path[SOFTPC_LPT_OUTPUT_PATH_MAX];
} HOST_LPT;

static HOST_LPT host_lpt[NUM_PARALLEL_PORTS];

int softpc_host_lpt_set_output_path(int adapter, const char *path)
{
    HOST_LPT *lpt;
    size_t length;
    if (adapter < 0 || adapter >= NUM_PARALLEL_PORTS) return FALSE;
    lpt = &host_lpt[adapter];
    if (path == 0) {
        lpt->output_path[0] = '\0';
        return TRUE;
    }
    length = strlen(path);
    if (length >= sizeof(lpt->output_path)) return FALSE;
    memcpy(lpt->output_path, path, length + 1u);
    return TRUE;
}

/* The fixed standalone profile deliberately has no physical printer
 * selection.  This is the sole replacement for nt_lpt's CreateFile,
 * WriteFile and DeviceIoControl exits: output is accepted by a virtual
 * sink, while all original buffering/state transitions stay intact. */
static boolean flush_buffer(int adapter)
{
    HOST_LPT *lpt = &host_lpt[adapter];
    if (!lpt->active) return FALSE;
    if (lpt->output != 0 && lpt->bytes_in_buffer != 0 &&
        (fwrite(lpt->buffer, 1u, (size_t)lpt->bytes_in_buffer,
            lpt->output) != (size_t)lpt->bytes_in_buffer ||
        fflush(lpt->output) != 0)) return FALSE;
    lpt->bytes_in_buffer = 0;
    return TRUE;
}

static SHORT host_lpt_open(int adapter, boolean direct_access)
{
    HOST_LPT *lpt;
    if (adapter < 0 || adapter >= NUM_PARALLEL_PORTS) return FALSE;
    lpt = &host_lpt[adapter];
    if (direct_access && lpt->no_device_attached) return FALSE;
    if (lpt->active) return TRUE;
    lpt->buffer = (byte *)malloc(KBUFFER_SIZE);
    if (lpt->buffer == NULL) return FALSE;
    if (lpt->output_path[0] != '\0') {
        lpt->output = fopen(lpt->output_path, "ab");
        if (lpt->output == NULL) {
            free(lpt->buffer);
            lpt->buffer = NULL;
            return FALSE;
        }
    }
    lpt->bytes_in_buffer = 0;
    lpt->flush_threshold = direct_access ? DIRECT_ACCESS_HIGH_WATER :
        HIGH_WATER;
    lpt->port_status = 0;
    lpt->inactive_counter = 0;
    lpt->inactive_trigger = 0;
    lpt->direct_access = direct_access;
    lpt->active = TRUE;
    return TRUE;
}

void host_lpt_close(int adapter)
{
    HOST_LPT *lpt;
    if (adapter < 0 || adapter >= NUM_PARALLEL_PORTS) return;
    lpt = &host_lpt[adapter];
    if (!lpt->active) return;
    (void)flush_buffer(adapter);
    if (lpt->output != NULL) {
        fclose(lpt->output);
        lpt->output = NULL;
    }
    free(lpt->buffer);
    lpt->buffer = NULL;
    lpt->bytes_in_buffer = 0;
    lpt->active = FALSE;
    lpt->direct_access = FALSE;
    lpt->port_status = 0;
}

void host_lpt_close_all(void)
{
    int adapter;
    for (adapter = 0; adapter < NUM_PARALLEL_PORTS; ++adapter)
        host_lpt_close(adapter);
}

unsigned long host_lpt_status(int adapter)
{
    if (adapter < 0 || adapter >= NUM_PARALLEL_PORTS) return HOST_LPT_BUSY;
    return host_lpt[adapter].port_status;
}

boolean host_set_lpt_direct_access(int adapter, boolean direct_access)
{
    host_lpt_close(adapter);
    if (!host_lpt_open(adapter, direct_access)) {
        if (adapter >= 0 && adapter < NUM_PARALLEL_PORTS)
            host_lpt[adapter].port_status = HOST_LPT_BUSY;
        return FALSE;
    }
    return TRUE;
}

UCHAR host_read_printer_status_port(int adapter)
{
    if (!host_set_lpt_direct_access(adapter, TRUE)) return 0;
    return 0;
}

BOOL host_print_byte(adapter, value)
int adapter;
byte value;
{
    HOST_LPT *lpt;
    if (!host_lpt_open(adapter, FALSE)) {
        if (adapter >= 0 && adapter < NUM_PARALLEL_PORTS)
            host_lpt[adapter].port_status = HOST_LPT_BUSY;
        return FALSE;
    }
    lpt = &host_lpt[adapter];
    lpt->buffer[lpt->bytes_in_buffer++] = value;
    if (lpt->bytes_in_buffer >= lpt->flush_threshold &&
        !flush_buffer(adapter)) {
        lpt->port_status = HOST_LPT_BUSY;
        return FALSE;
    }
    lpt->inactive_counter = 0;
    return TRUE;
}

BOOL host_print_doc(int adapter)
{
    return flush_buffer(adapter);
}

void host_reset_print(int adapter)
{
    host_lpt_close(adapter);
}

void host_print_auto_feed(adapter, value)
int adapter;
BOOL value;
{
    UNUSED(adapter);
    UNUSED(value);
}

void host_lpt_heart_beat(void)
{
    int adapter;
    for (adapter = 0; adapter < NUM_PARALLEL_PORTS; ++adapter) {
        HOST_LPT *lpt = &host_lpt[adapter];
        if (lpt->active && lpt->inactive_trigger != 0 &&
            ++lpt->inactive_counter == lpt->inactive_trigger)
            host_lpt_close(adapter);
    }
}

void host_lpt_dos_open(int adapter)
{
    UNUSED(adapter);
}

void host_lpt_dos_close(int adapter)
{
    host_lpt_close(adapter);
}

void host_lpt_flush_initialize(void)
{
}
