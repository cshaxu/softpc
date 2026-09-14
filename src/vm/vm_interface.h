#ifndef VM_INTERFACE_H
#define VM_INTERFACE_H

#include "common/machine/machine_interface.h"
#include "lib/storage/medium_interface.h"

typedef struct vm_driver vm_driver;
typedef struct vm_options {
    const char *floppy_path;
    const char *hard_disk_path;
    const char *serial_output_path;
    const char *printer_output_path;
    lib_u32 memory_bytes;
    lib_storage_medium_mode media_mode;
} vm_options;

/* Composition creates the process's one concrete backend and injects its driver.
   A concurrent/live second create returns INVALID_STATE with a null output;
   failed create releases admission. Destroy releases it only after resource
   disposal, and must be called after Common has joined the executor. */
lib_status vm_create(const vm_options *options, vm_driver **out_driver);
void vm_destroy(vm_driver *driver);
void vm_driver_describe(vm_driver *driver, common_machine_driver *out_driver);
/* Reset the existing opt-in diagnostic output before startup. */
void vm_trace_reset(void);

#endif
