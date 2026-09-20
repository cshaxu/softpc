# One compiled VM source owner, independent of static archive extraction order.
function(softpc_check_build_source target source)
    if(source MATCHES "^\\$<")
        return() # The preserved machine OBJECT composition is not a C source.
    endif()
    get_filename_component(path "${source}" ABSOLUTE BASE_DIR "${SOFTPC_SOURCE_DIR}")
    file(RELATIVE_PATH path "${SOFTPC_SOURCE_DIR}" "${path}")
    if((path MATCHES "^src/core/machine/" AND NOT target STREQUAL "softpc-vm") OR
       (target STREQUAL "softpc-vm" AND NOT path MATCHES "^src/core/machine/") OR
       (target STREQUAL "softpcvm" AND NOT path MATCHES "^src/app/"))
        message(FATAL_ERROR "Build ownership: ${target} compiles ${path}")
    endif()
endfunction()

if(DEFINED CASE_TARGET)
    softpc_check_build_source("${CASE_TARGET}" "${CASE_SOURCE}")
elseif(CMAKE_SCRIPT_MODE_FILE)
    foreach(sample IN ITEMS "softpc-vm|src/core/machine/driver.c|0"
        "softpcvm|src/app/composition.c|0" "probe|test/unit/machine_smoke.c|0"
        "softpcvm|src/core/machine/driver.c|1" "probe|src/core/machine/debug.c|1"
        "softpc-machine|src/core/machine/input.c|1" "softpc-vm|src/core/compat/platform.c|1"
        "softpcvm|src/app/../core/compat/platform.c|1")
        string(REPLACE "|" ";" parts "${sample}")
        list(GET parts 0 target)
        list(GET parts 1 source)
        list(GET parts 2 failure)
        execute_process(COMMAND "${CMAKE_COMMAND}" "-DSOFTPC_SOURCE_DIR=${SOFTPC_SOURCE_DIR}"
            "-DCASE_TARGET=${target}" "-DCASE_SOURCE=${source}" -P "${CMAKE_CURRENT_LIST_FILE}"
            RESULT_VARIABLE result OUTPUT_QUIET ERROR_VARIABLE error)
        if((failure AND (result EQUAL 0 OR NOT error MATCHES "Build ownership:")) OR
           (NOT failure AND NOT result EQUAL 0))
            message(FATAL_ERROR "Build ownership fixture failed: ${sample}: ${error}")
        endif()
    endforeach()
else()
    get_property(targets DIRECTORY PROPERTY BUILDSYSTEM_TARGETS)
    foreach(target IN LISTS targets)
        get_target_property(sources "${target}" SOURCES)
        foreach(source IN LISTS sources)
            softpc_check_build_source("${target}" "${source}")
        endforeach()
    endforeach()
    file(GLOB expected RELATIVE "${SOFTPC_SOURCE_DIR}" "${SOFTPC_SOURCE_DIR}/src/core/machine/*.c")
    get_target_property(actual softpc-vm SOURCES)
    list(SORT expected)
    list(SORT actual)
    if(NOT actual STREQUAL expected)
        message(FATAL_ERROR "Build ownership: VM sources are missing or duplicated")
    endif()
endif()
