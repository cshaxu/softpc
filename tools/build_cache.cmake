# Root assembly policy only; independently reusable corpora do not include it.
foreach(softpc_cache_entry IN ITEMS build build/.ninja build/.ccache)
    set(softpc_cache_path "${CMAKE_SOURCE_DIR}/${softpc_cache_entry}")
    if(IS_SYMLINK "${softpc_cache_path}" OR
            (EXISTS "${softpc_cache_path}" AND NOT IS_DIRECTORY "${softpc_cache_path}"))
        message(FATAL_ERROR "Build cache root must be a real directory: ${softpc_cache_entry}")
    endif()
endforeach()
function(softpc_verify_ninja_tree)
    if(NOT CMAKE_GENERATOR MATCHES "^Ninja")
        return()
    endif()
    file(REAL_PATH "${CMAKE_SOURCE_DIR}/build/.ninja" softpc_ninja_root)
    file(REAL_PATH "${CMAKE_BINARY_DIR}" softpc_binary_root)
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(softpc_cache_arch x86)
    else()
        set(softpc_cache_arch x64)
    endif()
    if(NOT softpc_binary_root STREQUAL "${softpc_ninja_root}/gcc-${softpc_cache_arch}-release")
        message(FATAL_ERROR "Use the canonical build/.ninja/gcc-${softpc_cache_arch}-release tree")
    endif()
endfunction()

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
