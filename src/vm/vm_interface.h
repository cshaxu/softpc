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

/* Main creates the concrete backend, injects its existing Common driver,
   and destroys it only after Common has joined the executor. */
lib_status vm_create(const vm_options *options, vm_driver **out_driver);
void vm_destroy(vm_driver *driver);
void vm_driver_describe(vm_driver *driver, common_machine_driver *out_driver);
/* Reset the existing opt-in diagnostic output before startup. */
void vm_trace_reset(void);

#endif
