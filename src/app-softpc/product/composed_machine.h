#ifndef SOFTPC_PRODUCT_COMPOSED_MACHINE_H
#define SOFTPC_PRODUCT_COMPOSED_MACHINE_H

#include "product/surface/machine_interface.h"

typedef struct app_startup_config app_startup_config;

/* The App keeps its immutable configuration and composes this concrete
 * machine value before Product takes ownership of it. */
lib_status softpc_product_compose_machine(const app_startup_config *config,
    app_composed_machine *out_machine);

#endif
