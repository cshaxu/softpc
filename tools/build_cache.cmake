# Root assembly policy only; independently reusable corpora do not include it.
foreach(softpc_cache_entry IN ITEMS build build/.ninja build/.ccache)
    set(softpc_cache_path "${CMAKE_SOURCE_DIR}/${softpc_cache_entry}")
    if(IS_SYMLINK "${softpc_cache_path}" OR
            (EXISTS "${softpc_cache_path}" AND NOT IS_DIRECTORY "${softpc_cache_path}"))
        message(FATAL_ERROR "Build cache root must be a real directory: ${softpc_cache_entry}")
    endif()
endforeach()
if(CMAKE_GENERATOR MATCHES "^Ninja")
    file(REAL_PATH "${CMAKE_SOURCE_DIR}/build/.ninja" softpc_ninja_root)
    file(REAL_PATH "${CMAKE_BINARY_DIR}" softpc_binary_root)
    cmake_path(IS_PREFIX softpc_ninja_root "${softpc_binary_root}" softpc_inside_ninja)
    if(NOT softpc_inside_ninja OR softpc_binary_root STREQUAL softpc_ninja_root)
        message(FATAL_ERROR "Ninja build trees must be below build/.ninja")
    endif()
endif()

# Embed the environment in generated commands: cmake --build <dir> must obey
# the same cache location as a preset, independently of its caller's shell.
if(CMAKE_C_COMPILER_LAUNCHER)
    list(GET CMAKE_C_COMPILER_LAUNCHER 0 softpc_launcher)
    get_filename_component(softpc_launcher "${softpc_launcher}" NAME)
    if(softpc_launcher MATCHES "^ccache([.]exe)?$")
        set(CMAKE_C_COMPILER_LAUNCHER "${CMAKE_COMMAND}" -E env
            "CCACHE_DIR=${CMAKE_SOURCE_DIR}/build/.ccache"
            "CCACHE_TEMPDIR=${CMAKE_SOURCE_DIR}/build/.ccache/tmp"
            "CCACHE_MAXSIZE=5G" ${CMAKE_C_COMPILER_LAUNCHER})
    endif()
endif()
