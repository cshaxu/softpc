cmake_minimum_required(VERSION 3.23)
project(softpcvm LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(softpc-machine
    src/core/softpc_machine.c)
target_include_directories(softpc-machine PUBLIC include)
target_compile_definitions(softpc-machine PRIVATE _CRT_SECURE_NO_WARNINGS)

# The original CCPU manifest is the executable x86 core.  It deliberately
# links no NTVDM host, BOP service dispatcher, DOS/WOW component or Win32
# presentation provider.
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/src/core/softpc/base/ccpu386/c-files"
    SOFTPC_CCPU_SOURCE_NAMES)
list(FILTER SOFTPC_CCPU_SOURCE_NAMES EXCLUDE REGEX "^p\\.")
set(SOFTPC_CCPU_SOURCES)
foreach(SOFTPC_CCPU_SOURCE_NAME IN LISTS SOFTPC_CCPU_SOURCE_NAMES)
    list(APPEND SOFTPC_CCPU_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/core/softpc/base/ccpu386/${SOFTPC_CCPU_SOURCE_NAME}")
endforeach()
add_library(softpc-ccpu-core OBJECT
    ${SOFTPC_CCPU_SOURCES}
    src/core/softpc/base/ccpu386/softpc_ccpu_facade.c
    src/core/softpc/base/cvidc/softpc_gdp_state.c
    src/core/softpc/base/cvidc/sascdef.c
    src/core/softpc_io.c
    src/core/softpc_pic8259.c
    src/core/softpc_quick_events.c
    src/core/softpc_standalone_platform.c
    src/core/softpc_physical_mapping.c)
set_target_properties(softpc-ccpu-core PROPERTIES
    C_STANDARD 90
    C_EXTENSIONS ON)
target_compile_definitions(softpc-ccpu-core PRIVATE
    CPU_40_STYLE CCPU CPU_486 PROD SOFTPC_STANDALONE=1)
target_include_directories(softpc-ccpu-core PRIVATE
    src/core/softpc/host/inc
    src/core/softpc/host/inc/x86/prod
    src/core/softpc/base/inc
    src/core/softpc/base/ccpu386
    src/core/softpc/base/cvidc
    src/core)

# Keep the executor in the machine library so that every unresolved host edge
# is visible to the standalone build, rather than being hidden in an unused
# object target.
target_sources(softpc-machine PRIVATE $<TARGET_OBJECTS:softpc-ccpu-core>)
target_include_directories(softpc-machine PRIVATE
    src/core/softpc/host/inc
    src/core/softpc/host/inc/x86/prod
    src/core/softpc/base/inc
    src/core/softpc/base/ccpu386
    src/core/softpc/base/cvidc
    src/core)
target_compile_definitions(softpc-machine PRIVATE SOFTPC_STANDALONE=1)

add_executable(softpcvm
    src/vm/main.c
    src/vm/console.c
    src/vm/win32_window.c)
target_link_libraries(softpcvm PRIVATE softpc-machine)
if(WIN32)
    target_link_libraries(softpcvm PRIVATE user32 gdi32)
endif()

enable_testing()
add_executable(softpc-machine-smoke tests/softpc_machine_smoke.c)
target_link_libraries(softpc-machine-smoke PRIVATE softpc-machine)
add_test(NAME softpc-machine-smoke COMMAND softpc-machine-smoke)
set_tests_properties(softpc-machine-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-irq-smoke tests/softpc_irq_smoke.c)
target_link_libraries(softpc-irq-smoke PRIVATE softpc-machine)
add_test(NAME softpc-irq-smoke COMMAND softpc-irq-smoke)
set_tests_properties(softpc-irq-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_test(NAME softpc-standalone-source-boundary
    COMMAND "${CMAKE_COMMAND}" "-DSOFTPC_SOURCE_DIR=${CMAKE_SOURCE_DIR}" -P
        "${CMAKE_SOURCE_DIR}/tests/standalone_source_boundary.cmake")
add_test(NAME softpcvm-help COMMAND softpcvm --help)
