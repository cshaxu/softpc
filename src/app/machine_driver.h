#ifndef APP_MACHINE_DRIVER_H
#define APP_MACHINE_DRIVER_H

#include "common/machine/machine_interface.h"
#include "machine.h"

typedef struct app_machine_driver app_machine_driver;

lib_status app_machine_driver_create(app_machine_driver **out_driver,
    softpc_machine *machine);
void app_machine_driver_destroy(app_machine_driver *driver);
void app_machine_driver_describe(app_machine_driver *driver,
    common_machine_driver *out_driver);
void app_machine_driver_cursor_shape(kvm_frame *frame, lib_u32 percent);

#endif
