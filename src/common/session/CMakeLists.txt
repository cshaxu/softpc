add_library(common-session
    session.c
    control.c
    control_state.c
    reconciler.c
    presentation_plan.c)
target_include_directories(common-session PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..)
target_include_directories(common-session PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../app
    ${CMAKE_CURRENT_SOURCE_DIR}/../../host)
target_link_libraries(common-session PUBLIC kvm-base PRIVATE common-ui)
