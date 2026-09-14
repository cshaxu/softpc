#ifndef COMMON_MACHINE_TEST_H
#define COMMON_MACHINE_TEST_H

#include "app/machine_driver.h"

typedef struct common_machine_test {
    common_machine *machine;
    app_machine_driver *driver;
} common_machine_test;

lib_bool common_machine_test_create(softpc_machine *machine,
    common_machine_test *out_test);
void common_machine_test_destroy(common_machine_test *test);

#endif
