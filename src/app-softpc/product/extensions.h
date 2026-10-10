#ifndef SOFTPC_PRODUCT_EXTENSIONS_H
#define SOFTPC_PRODUCT_EXTENSIONS_H

#include "x86/product/entry_interface.h"

lib_status softpc_product_configure_extensions(app_composed_machine *machine,
    x86_product_command_extensions *out_extensions);

#endif
