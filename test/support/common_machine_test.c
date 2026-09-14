#include "common_machine_test.h"

lib_bool common_machine_test_create(softpc_machine *machine,
    common_machine_test *out_test)
{
    common_machine_driver driver;
    if (out_test == NULL) return LIB_FALSE;
    *out_test = (common_machine_test) { 0 };
    if (vm_driver_create(&out_test->driver, machine) != LIB_STATUS_OK)
        return LIB_FALSE;
    vm_driver_describe(out_test->driver, &driver);
    if (common_machine_create(&out_test->machine, &driver) != LIB_STATUS_OK) {
        vm_driver_destroy(out_test->driver);
        *out_test = (common_machine_test) { 0 };
        return LIB_FALSE;
    }
    return LIB_TRUE;
}

void common_machine_test_destroy(common_machine_test *test)
{
    if (test == NULL) return;
    common_machine_destroy(test->machine);
    vm_driver_destroy(test->driver);
    *test = (common_machine_test) { 0 };
}
