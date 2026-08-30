cmake_minimum_required(VERSION 3.23)
project(softpcvm LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(softpc-machine
    src/core/softpc_machine.c)
target_include_directories(softpc-machine PUBLIC include)
target_compile_definitions(softpc-machine PRIVATE _CRT_SECURE_NO_WARNINGS)

# First detached SoftPC core slice.  It deliberately links no NTVDM host,
# BOP service dispatcher, DOS/WOW component or Win32 presentation provider.
add_library(softpc-ccpu-core OBJECT
    src/core/softpc/base/ccpu386/c_main.c
    src/core/softpc/base/ccpu386/ccpusas4.c
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

add_executable(softpcvm src/vm/main.c)
target_link_libraries(softpcvm PRIVATE softpc-machine)

enable_testing()
add_executable(softpc-machine-smoke tests/softpc_machine_smoke.c)
target_link_libraries(softpc-machine-smoke PRIVATE softpc-machine)
add_test(NAME softpc-machine-smoke COMMAND softpc-machine-smoke)
set_tests_properties(softpc-machine-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
