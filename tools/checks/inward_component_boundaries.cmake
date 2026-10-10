cmake_minimum_required(VERSION 3.23)

if(NOT DEFINED SOFTPC_SOURCE_DIR)
    message(FATAL_ERROR "SOFTPC_SOURCE_DIR is required")
endif()

set(component_names lib emulator x86 app-softpc)

function(component_level component out_level)
    list(FIND component_names "${component}" level)
    if(level LESS 0)
        message(FATAL_ERROR "Unknown component: ${component}")
    endif()
    set(${out_level} "${level}" PARENT_SCOPE)
endfunction()

function(require_inward owner dependency path kind)
    component_level("${owner}" owner_level)
    component_level("${dependency}" dependency_level)
    if(dependency_level GREATER owner_level)
        message(FATAL_ERROR
            "Outward component ${kind}: ${path}: ${owner} -> ${dependency}")
    endif()
endfunction()

function(check_component_root relative_root owner)
    set(root "${SOFTPC_SOURCE_DIR}/${relative_root}")
    if(NOT IS_DIRECTORY "${root}")
        message(FATAL_ERROR "Missing component root: ${relative_root}")
    endif()
    file(GLOB_RECURSE paths LIST_DIRECTORIES FALSE RELATIVE "${root}"
        "${root}/*")
    foreach(path IN LISTS paths)
        get_filename_component(name "${path}" NAME)
        get_filename_component(extension "${path}" EXT)
        if(NOT name STREQUAL "CMakeLists.txt" AND
           NOT extension IN_LIST boundary_extensions)
            continue()
        endif()
        file(READ "${root}/${path}" text)
        foreach(component IN LISTS component_names)
            string(FIND "${text}" "${component}/" component_path_index)
            string(FIND "${text}" "src/${component}" source_root_index)
            string(FIND "${text}" "test/${component}" test_root_index)
            if(NOT "${component_path_index}" EQUAL -1 OR
               NOT "${source_root_index}" EQUAL -1 OR
               NOT "${test_root_index}" EQUAL -1)
                require_inward("${owner}" "${component}"
                    "${relative_root}/${path}" "path reference")
            endif()
        endforeach()
        if(name STREQUAL "CMakeLists.txt" OR extension STREQUAL ".cmake")
            string(FIND "${text}" "emulator-" emulator_target_index)
            if(NOT "${emulator_target_index}" EQUAL -1)
                require_inward("${owner}" "emulator"
                    "${relative_root}/${path}" "CMake target")
            endif()
            string(FIND "${text}" "x86-" x86_target_index)
            if(NOT "${x86_target_index}" EQUAL -1)
                require_inward("${owner}" "x86"
                    "${relative_root}/${path}" "CMake target")
            endif()
            string(FIND "${text}" "softpc-" softpc_target_index)
            if(NOT "${softpc_target_index}" EQUAL -1)
                require_inward("${owner}" "app-softpc"
                    "${relative_root}/${path}" "CMake target")
            endif()
        endif()
    endforeach()
endfunction()

set(boundary_extensions .c .h .cmake .md .sha256)
foreach(component IN LISTS component_names)
    check_component_root("src/${component}" "${component}")
    check_component_root("test/${component}" "${component}")
endforeach()

message(STATUS "Component inward source/test/configuration/documentation boundaries verified")
