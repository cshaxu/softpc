cmake_minimum_required(VERSION 3.23)

if(NOT DEFINED SOFTPC_SOURCE_DIR OR NOT DEFINED SELF_TEST_ROOT)
    message(FATAL_ERROR "SOFTPC_SOURCE_DIR and SELF_TEST_ROOT are required")
endif()

file(REMOVE_RECURSE "${SELF_TEST_ROOT}")
foreach(component IN ITEMS lib emulator x86 app-softpc)
    file(MAKE_DIRECTORY "${SELF_TEST_ROOT}/src/${component}")
    file(MAKE_DIRECTORY "${SELF_TEST_ROOT}/test/${component}")
endforeach()
file(WRITE "${SELF_TEST_ROOT}/src/lib/probe.h"
    "#include \"x86/product/entry_interface.h\"\n")
execute_process(COMMAND "${CMAKE_COMMAND}"
    "-DSOFTPC_SOURCE_DIR=${SELF_TEST_ROOT}"
    -P "${SOFTPC_SOURCE_DIR}/tools/checks/inward_component_boundaries.cmake"
    RESULT_VARIABLE result OUTPUT_VARIABLE output ERROR_VARIABLE error)
if(result EQUAL 0 OR NOT "${output}${error}" MATCHES "lib -> x86")
    message(FATAL_ERROR "Expected Lib outward-include rejection: ${output}${error}")
endif()

file(REMOVE "${SELF_TEST_ROOT}/src/lib/probe.h")
file(WRITE "${SELF_TEST_ROOT}/test/emulator/README.md"
    "See src/x86 for details.\n")
execute_process(COMMAND "${CMAKE_COMMAND}"
    "-DSOFTPC_SOURCE_DIR=${SELF_TEST_ROOT}"
    -P "${SOFTPC_SOURCE_DIR}/tools/checks/inward_component_boundaries.cmake"
    RESULT_VARIABLE result OUTPUT_VARIABLE output ERROR_VARIABLE error)
if(result EQUAL 0 OR NOT "${output}${error}" MATCHES "emulator -> x86")
    message(FATAL_ERROR "Expected Emulator outward-document rejection: ${output}${error}")
endif()

file(REMOVE "${SELF_TEST_ROOT}/test/emulator/README.md")
file(WRITE "${SELF_TEST_ROOT}/src/x86/CMakeLists.txt"
    "target_link_libraries(x86-probe PRIVATE softpc-vm)\n")
execute_process(COMMAND "${CMAKE_COMMAND}"
    "-DSOFTPC_SOURCE_DIR=${SELF_TEST_ROOT}"
    -P "${SOFTPC_SOURCE_DIR}/tools/checks/inward_component_boundaries.cmake"
    RESULT_VARIABLE result OUTPUT_VARIABLE output ERROR_VARIABLE error)
if(result EQUAL 0 OR NOT "${output}${error}" MATCHES "x86 -> app-softpc")
    message(FATAL_ERROR "Expected X86 outward-target rejection: ${output}${error}")
endif()

file(REMOVE_RECURSE "${SELF_TEST_ROOT}")
message(STATUS "Component inward-boundary negative coverage: OK")
