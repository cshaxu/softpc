set(standalone_sources
    "${SOFTPC_SOURCE_DIR}/src/core/softpc/base/ccpu386/softpc_ccpu_facade.c"
    "${SOFTPC_SOURCE_DIR}/src/core/softpc/base/cvidc/softpc_gdp_state.c"
    "${SOFTPC_SOURCE_DIR}/src/core/softpc/base/cvidc/softpc_gdp_state.h"
    "${SOFTPC_SOURCE_DIR}/src/core/softpc/base/cvidc/softpc_gdp_slots.h"
    "${SOFTPC_SOURCE_DIR}/src/core/softpc_io.c"
    "${SOFTPC_SOURCE_DIR}/src/core/softpc_pic8259.c"
    "${SOFTPC_SOURCE_DIR}/src/core/softpc_quick_events.c"
    "${SOFTPC_SOURCE_DIR}/src/core/softpc_physical_mapping.c"
    "${SOFTPC_SOURCE_DIR}/src/core/softpc_standalone_platform.c"
    "${SOFTPC_SOURCE_DIR}/src/core/softpc_machine.c")

file(STRINGS "${SOFTPC_SOURCE_DIR}/src/core/softpc/base/ccpu386/c-files"
    ccpu_source_names)
foreach(name IN LISTS ccpu_source_names)
    if(NOT name MATCHES "^p\\.")
        list(APPEND standalone_sources
            "${SOFTPC_SOURCE_DIR}/src/core/softpc/base/ccpu386/${name}")
    endif()
endforeach()

foreach(source IN LISTS standalone_sources)
    file(READ "${source}" contents)
    string(TOLOWER "${contents}" normalized_contents)
    if(normalized_contents MATCHES "(^|[^[:alnum:]_])bop[[:space:]]*\\(")
        message(FATAL_ERROR "Standalone CCPU contains a BOP dispatcher: ${source}")
    endif()
    if(normalized_contents MATCHES "(^|[^[:alnum:]_])(mvdm|ntvdm|vdm|wow|vdd|basesrv|csr|dos)([^[:alnum:]_]|$)")
        message(FATAL_ERROR "Standalone CCPU contains product-shell semantics: ${source}")
    endif()
endforeach()
