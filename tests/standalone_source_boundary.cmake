set(standalone_sources
    "${CMAKE_SOURCE_DIR}/src/core/softpc/base/ccpu386/c_main.c"
    "${CMAKE_SOURCE_DIR}/src/core/softpc/base/ccpu386/softpc_ccpu_facade.c"
    "${CMAKE_SOURCE_DIR}/src/core/softpc_standalone_platform.c"
    "${CMAKE_SOURCE_DIR}/src/core/softpc_machine.c")

foreach(source IN LISTS standalone_sources)
    file(READ "${source}" contents)
    if(contents MATCHES "(^|[^[:alnum:]_])bop[[:space:]]*\\(")
        message(FATAL_ERROR "Standalone core contains a BOP dispatcher: ${source}")
    endif()
endforeach()
