cmake_minimum_required(VERSION 3.23)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(common_corpus LANGUAGES C)
    set(CMAKE_C_STANDARD 17)
    set(CMAKE_C_STANDARD_REQUIRED ON)
    set(CMAKE_C_EXTENSIONS OFF)
    set(COMMON_LIB_ROOT "" CACHE PATH "Explicit path to the shared Lib corpus")
    if(NOT EXISTS "${COMMON_LIB_ROOT}/CMakeLists.txt")
        message(FATAL_ERROR "COMMON_LIB_ROOT must name the shared Lib corpus")
    endif()
    add_subdirectory("${COMMON_LIB_ROOT}" "${CMAKE_CURRENT_BINARY_DIR}/lib")
endif()

add_subdirectory(ui)
add_subdirectory(machine)
add_subdirectory(session)

add_library(common-x86-xasm32 STATIC
    x86-xasm32/xasm32.c
    x86-xasm32/aasm32.c
    x86-xasm32/dasm32.c)
target_include_directories(common-x86-xasm32 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(common-x86-xasm32 PUBLIC types)

add_library(common-x86-debug STATIC
    x86-debug/command.c)
target_include_directories(common-x86-debug PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(common-x86-debug PUBLIC common-machine common-x86-xasm32 storage)

add_custom_target(common-verify
    COMMAND "${CMAKE_COMMAND}" -DCOMMON_ROOT=${CMAKE_CURRENT_SOURCE_DIR}
        -P "${CMAKE_CURRENT_SOURCE_DIR}/verify_manifest.cmake"
    COMMAND "${CMAKE_COMMAND}" -DCOMMON_ROOT=${CMAKE_CURRENT_SOURCE_DIR}
        -P "${CMAKE_CURRENT_SOURCE_DIR}/verify_corpus.cmake"
    VERBATIM)
