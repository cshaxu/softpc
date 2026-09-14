cmake_minimum_required(VERSION 3.23)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(common_corpus LANGUAGES C)
    set(CMAKE_C_STANDARD 17)
    set(CMAKE_C_STANDARD_REQUIRED ON)
    set(CMAKE_C_EXTENSIONS OFF)
    include(CTest)
    set(COMMON_LIB_ROOT "" CACHE PATH "Explicit path to the shared Lib corpus")
    if(NOT EXISTS "${COMMON_LIB_ROOT}/CMakeLists.txt")
        message(FATAL_ERROR "COMMON_LIB_ROOT must name the shared Lib corpus")
    endif()
    # Common verification never registers the importing product's Lib tests.
    set(common_testing ${BUILD_TESTING})
    set(BUILD_TESTING OFF)
    add_subdirectory("${COMMON_LIB_ROOT}" "${CMAKE_CURRENT_BINARY_DIR}/lib")
    set(BUILD_TESTING ${common_testing})
endif()

add_subdirectory(ui)
add_subdirectory(machine)
add_subdirectory(session)

add_library(common-xasm32 STATIC
    xasm32/xasm32.c
    xasm32/aasm32.c
    xasm32/dasm32.c)
target_include_directories(common-xasm32 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(common-xasm32 PUBLIC types)

add_library(common-debug STATIC
    debug/debug.c
    debug/command.c)
target_include_directories(common-debug PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(common-debug PUBLIC common-machine common-xasm32 storage)

add_custom_target(common-verify
    COMMAND "${CMAKE_COMMAND}" -DCOMMON_ROOT=${CMAKE_CURRENT_SOURCE_DIR}
        -P "${CMAKE_CURRENT_SOURCE_DIR}/verify_manifest.cmake"
    COMMAND "${CMAKE_COMMAND}" -DCOMMON_ROOT=${CMAKE_CURRENT_SOURCE_DIR}
        -P "${CMAKE_CURRENT_SOURCE_DIR}/verify_corpus.cmake"
    VERBATIM)
option(COMMON_BUILD_TESTING "Build Common self-contained proofs" ${BUILD_TESTING})
if(COMMON_BUILD_TESTING)
    foreach(check IN ITEMS manifest corpus)
        add_test(NAME common.${check}
            COMMAND "${CMAKE_COMMAND}" -DCOMMON_ROOT=${CMAKE_CURRENT_SOURCE_DIR}
                -P "${CMAKE_CURRENT_SOURCE_DIR}/verify_${check}.cmake")
    endforeach()
    add_test(NAME common.verifier-negative
        COMMAND "${CMAKE_COMMAND}" -DCOMMON_ROOT=${CMAKE_CURRENT_SOURCE_DIR}
            -DPROBE_ROOT=${CMAKE_CURRENT_BINARY_DIR}/verifier-probe
            -P "${CMAKE_CURRENT_SOURCE_DIR}/test/verify_negative.cmake")
    add_executable(common-sync-smoke test/sync_smoke.c)
    target_link_libraries(common-sync-smoke PRIVATE common-session host)
    if(MSVC)
        target_compile_options(common-sync-smoke PRIVATE /UNDEBUG)
    else()
        target_compile_options(common-sync-smoke PRIVATE -UNDEBUG)
    endif()
    add_test(NAME common.sync COMMAND common-sync-smoke)
    set_tests_properties(common.sync PROPERTIES TIMEOUT 30)
endif()
