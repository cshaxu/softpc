set(standalone_sources
    "${SOFTPC_SOURCE_DIR}/src/core/softpc/base/ccpu386/softpc_ccpu_facade.c"
    "${SOFTPC_SOURCE_DIR}/src/core/softpc/base/cvidc/softpc_gdp_state.c"
    "${SOFTPC_SOURCE_DIR}/src/core/softpc/base/cvidc/softpc_gdp_state.h"
    "${SOFTPC_SOURCE_DIR}/src/core/softpc/base/cvidc/softpc_gdp_slots.h"
    "${SOFTPC_SOURCE_DIR}/src/core/softpc/base/cvidc/sascdef.c"
    "${SOFTPC_SOURCE_DIR}/src/core/softpc/base/support/ios.c"
    "${SOFTPC_SOURCE_DIR}/src/core/softpc/base/disks/fdisk.c"
    "${SOFTPC_SOURCE_DIR}/src/core/softpc_device_bop.c"
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

# Original controller sources retain inactive historic host branches so their
# provenance and machine implementation remain intact.  The standalone target
# must never activate those branches.
file(READ "${SOFTPC_SOURCE_DIR}/CMakeLists.txt" build_definition)
string(TOLOWER "${build_definition}" normalized_build_definition)
if(normalized_build_definition MATCHES "target_compile_definitions\\([^\\)]*ntvdm")
    message(FATAL_ERROR "Standalone SoftPC enables an NTVDM compile definition")
endif()

# Firmware is part of the standalone machine's executable guest contract.
# Discover every checked-in ROM source so future BIOS additions cannot evade
# the same product-semantic guard through a stale manual list.
file(GLOB firmware_sources
    "${SOFTPC_SOURCE_DIR}/src/core/firmware/*.asm")
list(APPEND standalone_sources ${firmware_sources})

foreach(source IN LISTS standalone_sources)
    file(READ "${source}" contents)
    string(TOLOWER "${contents}" normalized_contents)
    if(normalized_contents MATCHES "(^|[^[:alnum:]_])(mvdm|ntvdm|vdm|wow|vdd|basesrv|csr|dos)([^[:alnum:]_]|$)")
        message(FATAL_ERROR "Standalone CCPU contains product-shell semantics: ${source}")
    endif()
endforeach()

file(READ "${SOFTPC_SOURCE_DIR}/src/core/softpc_standalone_platform.c"
    standalone_platform)
string(TOLOWER "${standalone_platform}" normalized_platform)
if(normalized_platform MATCHES "softpc_ata")
    message(FATAL_ERROR "Standalone platform retains a handwritten ATA controller")
endif()
