set(x86_boundary_script "${CMAKE_CURRENT_LIST_DIR}/x86_boundary.cmake")
set(fixture "${CMAKE_CURRENT_BINARY_DIR}/x86-boundary-fixture")
file(REMOVE_RECURSE "${fixture}")
file(MAKE_DIRECTORY "${fixture}/src/app-softpc/product" "${fixture}/src/app-softpc/machine" "${fixture}/src/app-softpc/compat"
    "${fixture}/src/x86" "${fixture}/src/emulator" "${fixture}/src/lib" "${fixture}/src/app-softpc/softpc.new")
file(WRITE "${fixture}/src/app-softpc/machine/vm_interface.h" "#include <emulator/machine/machine_interface.h>\n")
file(WRITE "${fixture}/src/app-softpc/product/composed_machine.c" "#include <app-softpc/machine/vm_interface.h>\n")
file(WRITE "${fixture}/src/app-softpc/product/legal.c" "#include <lib/storage/file_interface.h>\n")
file(WRITE "${fixture}/src/emulator/legal.c" "#include <lib/base/sync_interface.h>\n")
file(WRITE "${fixture}/src/app-softpc/machine/legal.c" "#include <app-softpc/compat/platform.h>\n")
file(WRITE "${fixture}/src/app-softpc/compat/legal.c" "#include <lib/storage/medium_interface.h>\n")
file(WRITE "${fixture}/src/app-softpc/softpc.new/legal.c" "#include <app-softpc/compat/ccpu/lifecycle.h>\n")
file(WRITE "${fixture}/src/app-softpc/softpc.new/original.c" "#include <compat/devices/snapshot.h>\n")
function(check expected)
    execute_process(COMMAND "${CMAKE_COMMAND}" "-DSOFTPC_SOURCE_DIR=${fixture}"
        -P "${x86_boundary_script}"
        RESULT_VARIABLE result OUTPUT_QUIET ERROR_VARIABLE error)
    if(expected STREQUAL "pass")
        if(NOT result EQUAL 0)
            message(FATAL_ERROR "Legal composition rejected: ${error}")
        endif()
    elseif(result EQUAL 0 OR NOT error MATCHES "x86 boundary")
        message(FATAL_ERROR "Illegal dependency accepted or wrong failure for ${path}|${header}: ${error}")
    endif()
endfunction()
check(pass)
foreach(pair IN ITEMS "app-softpc/product/config.c|../machine/vm_interface.h" "app-softpc/product/main.c|app-softpc/machine/vm_interface.h"
    "app-softpc/product/composed_machine.h|app-softpc/machine/vm_interface.h" "app-softpc/product/main.c|app-softpc/compat/platform.h"
    "app-softpc/product/extensions.h|../compat/platform.h" "app-softpc/machine/vm_interface.h|app-softpc/compat/platform.h"
    "app-softpc/machine/driver.c|app-softpc/product/config.h" "app-softpc/compat/platform.c|emulator/machine/machine_interface.h"
    "app-softpc/product/composed_machine.c|app-softpc/machine/driver.h" "app-softpc/compat/platform.c|../machine/driver.h"
    "emulator/control.c|../app-softpc/machine/vm_interface.h" "emulator/control.c|../app-softpc/compat/platform.h"
    "emulator/control.c|app-softpc/softpc.new/base/inc/cpu4.h" "emulator/control.c|app-softpc/x86/config.h"
    "lib/boundary.c|../emulator/machine/machine_interface.h" "lib/boundary.c|../app-softpc/machine/driver.h"
    "lib/boundary.c|app-softpc/compat/platform.h" "lib/boundary.c|app-softpc/softpc.new/base/inc/cpu4.h"
    "app-softpc/softpc.new/core.c|app-softpc/machine/driver.h" "app-softpc/softpc.new/core.c|emulator/machine/machine_interface.h"
    "emulator/control.c|../x86/debug/protocol_interface.h"
    "lib/boundary.c|x86/debug/protocol_interface.h"
    "app-softpc/compat/platform.c|x86/debug/protocol_interface.h"
    "app-softpc/softpc.new/core.c|x86/debug/protocol_interface.h")
    string(REPLACE "|" ";" parts "${pair}")
    list(GET parts 0 path)
    list(GET parts 1 header)
    file(WRITE "${fixture}/src/${path}" "#include \"${header}\"\n")
    check(fail)
    file(REMOVE "${fixture}/src/${path}")
endforeach()
file(WRITE "${fixture}/src/app-softpc/product/composed_machine.c" "void f(void) { vm_driver_create(0, 0); }\n")
check(fail)
file(REMOVE "${fixture}/src/app-softpc/product/composed_machine.c")
file(WRITE "${fixture}/src/emulator/absolute.c" "#include \"${fixture}/src/app-softpc/machine/driver.h\"\n")
check(fail)
file(REMOVE_RECURSE "${fixture}")
