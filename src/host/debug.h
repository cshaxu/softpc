#ifndef SOFTPC_HOST_DEBUG_H
#define SOFTPC_HOST_DEBUG_H

#include "host/machine.h"
#include "common/machine/machine_interface.h"

/* Product adapter, called only at the paused executor rendezvous. */
lib_status softpc_machine_debug(softpc_machine *machine,
    const common_machine_debug_request *request,
    common_machine_debug_result *result);

#endif
