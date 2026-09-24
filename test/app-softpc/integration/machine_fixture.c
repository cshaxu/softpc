#include "machine_fixture.h"

lib_bool softpc_machine_fixture_create(softpc_machine *machine,
    softpc_machine_fixture *out_test)
{
    common_machine_driver driver;
    if (out_test == NULL) return LIB_FALSE;
    *out_test = (softpc_machine_fixture) { 0 };
    if (vm_driver_create(&out_test->driver, machine) != LIB_STATUS_OK)
        return LIB_FALSE;
    vm_driver_describe(out_test->driver, &driver);
    if (common_machine_create(&out_test->machine, &driver) != LIB_STATUS_OK) {
        vm_driver_destroy(out_test->driver);
        *out_test = (softpc_machine_fixture) { 0 };
        return LIB_FALSE;
    }
    return LIB_TRUE;
}

void softpc_machine_fixture_destroy(softpc_machine_fixture *test)
{
    if (test == NULL) return;
    common_machine_destroy(test->machine);
    vm_driver_destroy(test->driver);
    *test = (softpc_machine_fixture) { 0 };
}
