cmake_minimum_required(VERSION 3.23)
project(softpcvm LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(softpc-machine
    src/core/softpc_machine.c)
target_include_directories(softpc-machine PUBLIC include)
target_compile_definitions(softpc-machine PRIVATE _CRT_SECURE_NO_WARNINGS)

add_executable(softpcvm src/vm/main.c)
target_link_libraries(softpcvm PRIVATE softpc-machine)

enable_testing()
add_executable(softpc-machine-smoke tests/softpc_machine_smoke.c)
target_link_libraries(softpc-machine-smoke PRIVATE softpc-machine)
add_test(NAME softpc-machine-smoke COMMAND softpc-machine-smoke)
set_tests_properties(softpc-machine-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
