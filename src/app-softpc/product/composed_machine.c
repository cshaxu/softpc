#include "lib/types/types_interface.h"

#include "config.h"
#include "composed_machine.h"
#include "machine/vm_interface.h"

static lib_status softpc_product_bind(void *machine, emulator_machine *emulator);
static lib_status softpc_product_destroy(void *machine);

lib_status softpc_product_compose_machine(const app_startup_config *config,
    app_composed_machine *out_machine)
{
    vm_options options = {0};
    vm_driver *driver = LIB_NULL;
    lib_status status;

    if (config == LIB_NULL || out_machine == LIB_NULL)
        return LIB_STATUS_INVALID_ARGUMENT;
    *out_machine = (app_composed_machine){0};
    options.floppy_path = config->floppy_path[0] == '\0' ? LIB_NULL :
        config->floppy_path;
    options.hard_disk_path = config->hard_disk_path[0] == '\0' ? LIB_NULL :
        config->hard_disk_path;
    options.floppy_mode = config->floppy_mode;
    options.hard_disk_mode = config->hard_disk_mode;
    options.memory_bytes = config->memory_bytes;
    options.serial_output_path = config->serial_output_path[0] == '\0' ? LIB_NULL :
        config->serial_output_path;
    options.printer_output_path = config->printer_output_path[0] == '\0' ? LIB_NULL :
        config->printer_output_path;
    status = vm_create(&options, &driver);
    if (status != LIB_STATUS_OK) return status;
    out_machine->composition.machine = driver;
    vm_driver_describe(driver, &out_machine->composition.driver);
    out_machine->composition.bind = softpc_product_bind;
    out_machine->composition.destroy = softpc_product_destroy;
    return LIB_STATUS_OK;
}

static lib_status softpc_product_bind(void *machine, emulator_machine *emulator)
{
    (void)machine;
    (void)emulator;
    return LIB_STATUS_OK;
}

static lib_status softpc_product_destroy(void *machine)
{
    return vm_destroy((vm_driver *)machine);
}
