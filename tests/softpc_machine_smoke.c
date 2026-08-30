#include "softpc_machine.h"

#include <assert.h>

int main(void)
{
    softpc_machine_options options = { "CMakeLists.txt", NULL,
        SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_run(machine, 1u) == SOFTPC_MACHINE_BACKEND_UNAVAILABLE);
    softpc_machine_destroy(machine);
    return 0;
}
