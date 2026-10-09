#ifndef IBMPC_NXVM_STARTUP_CONFIG_INTERFACE_H
#define IBMPC_NXVM_STARTUP_CONFIG_INTERFACE_H

#include "lib/types/types_interface.h"
#include "lib/storage/medium_interface.h"
#include "ibmpc/product/entry_interface.h"

/* NXVM App configuration.  It is intentionally outside Product: another PC
 * machine can expose different media slots, memory policy or no such knobs. */
#define NXVM_STARTUP_PATH_MAX 1024u
#define NXVM_STARTUP_MEDIA_SLOT_COUNT 2u

typedef struct nxvm_startup_config {
    app_composed_ui ui;
    lib_u8 floppy[NXVM_STARTUP_MEDIA_SLOT_COUNT][NXVM_STARTUP_PATH_MAX];
    lib_u8 fixed_disk[NXVM_STARTUP_MEDIA_SLOT_COUNT][NXVM_STARTUP_PATH_MAX];
    lib_storage_medium_mode floppy_mode[NXVM_STARTUP_MEDIA_SLOT_COUNT];
    lib_storage_medium_mode fixed_disk_mode[NXVM_STARTUP_MEDIA_SLOT_COUNT];
    lib_size floppy_count;
    lib_size fixed_disk_count;
    lib_size memory_bytes;
} nxvm_startup_config;

#endif
