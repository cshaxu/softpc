#ifndef SOFTPC_MACHINE_FIXTURE_H
#define SOFTPC_MACHINE_FIXTURE_H

#include "machine/driver.h"

typedef struct softpc_machine_fixture {
    common_machine *machine;
    vm_driver *driver;
} softpc_machine_fixture;

lib_bool softpc_machine_fixture_create(softpc_machine *machine,
    softpc_machine_fixture *out_test);
void softpc_machine_fixture_destroy(softpc_machine_fixture *test);

#endif
