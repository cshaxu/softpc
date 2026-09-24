set(fixture "${CMAKE_CURRENT_BINARY_DIR}/lib-types-base-convergence-fixture")
file(REMOVE_RECURSE "${fixture}")
file(MAKE_DIRECTORY "${fixture}/src/app-softpc/product"
    "${fixture}/src/app-softpc/softpc.new")
file(WRITE "${fixture}/src/app-softpc/product/legal.c"
    "#include \"lib/types/types_interface.h\"\nvoid f(void) { lib_u32 n = 0u; (void)n; }\n")
execute_process(COMMAND "${CMAKE_COMMAND}" "-DSOFTPC_SOURCE_DIR=${fixture}"
    -P "${SOFTPC_SOURCE_DIR}/tools/checks/lib_types_base_convergence.cmake"
    RESULT_VARIABLE legal_result OUTPUT_QUIET ERROR_VARIABLE legal_error)
if(NOT legal_result EQUAL 0)
    message(FATAL_ERROR "Lib Types/Base legal fixture rejected: ${legal_error}")
endif()
file(WRITE "${fixture}/src/app-softpc/product/illegal.c"
    "#include <string.h>\nvoid f(void) { (void)strlen(\"x\"); }\n")
execute_process(COMMAND "${CMAKE_COMMAND}" "-DSOFTPC_SOURCE_DIR=${fixture}"
    -P "${SOFTPC_SOURCE_DIR}/tools/checks/lib_types_base_convergence.cmake"
    RESULT_VARIABLE illegal_result OUTPUT_QUIET ERROR_VARIABLE illegal_error)
if(illegal_result EQUAL 0 OR NOT illegal_error MATCHES "Lib Types convergence")
    message(FATAL_ERROR "Lib Types/Base bypass accepted: ${illegal_error}")
endif()
file(WRITE "${fixture}/src/app-softpc/softpc.new/legacy.c"
    "#include <string.h>\nvoid f(void) { (void)strlen(\"x\"); }\n")
file(REMOVE "${fixture}/src/app-softpc/product/illegal.c")
execute_process(COMMAND "${CMAKE_COMMAND}" "-DSOFTPC_SOURCE_DIR=${fixture}"
    -P "${SOFTPC_SOURCE_DIR}/tools/checks/lib_types_base_convergence.cmake"
    RESULT_VARIABLE mirror_result OUTPUT_QUIET ERROR_VARIABLE mirror_error)
if(NOT mirror_result EQUAL 0)
    message(FATAL_ERROR "Lib Types/Base mirror exemption rejected: ${mirror_error}")
endif()
file(REMOVE_RECURSE "${fixture}")
