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
