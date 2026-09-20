#ifndef VM_DRIVER_H
#define VM_DRIVER_H

#include "vm_interface.h"
#include "core/machine/machine.h"

/* Internal non-owning wrapper for serialized low-level tests; production
 * acquires machine/audio ownership only through vm_create. */
lib_status vm_driver_create(vm_driver **out_driver,
    softpc_machine *machine);
void vm_driver_destroy(vm_driver *driver);
void vm_driver_cursor_shape(kvm_text_frame *frame, lib_u32 percent);

#endif
