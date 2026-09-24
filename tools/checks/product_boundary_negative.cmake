set(fixture "${CMAKE_CURRENT_BINARY_DIR}/product-boundary-fixture")
file(REMOVE_RECURSE "${fixture}")
file(MAKE_DIRECTORY "${fixture}/src/app-softpc/product" "${fixture}/src/app-softpc/machine" "${fixture}/src/app-softpc/compat"
    "${fixture}/src/common" "${fixture}/src/lib" "${fixture}/src/x86" "${fixture}/src/app-softpc/softpc.new")
file(WRITE "${fixture}/src/app-softpc/machine/vm_interface.h" "#include <common/machine/machine_interface.h>\n")
file(WRITE "${fixture}/src/app-softpc/product/composition.c" "#include <app-softpc/machine/vm_interface.h>\n")
file(WRITE "${fixture}/src/app-softpc/product/legal.c" "#include <lib/storage/file_interface.h>\n")
file(WRITE "${fixture}/src/common/legal.c" "#include <lib/base/sync_interface.h>\n")
file(WRITE "${fixture}/src/x86/legal.c" "#include <common/machine/machine_interface.h>\n")
file(WRITE "${fixture}/src/app-softpc/machine/legal.c" "#include <app-softpc/compat/platform.h>\n")
file(WRITE "${fixture}/src/app-softpc/compat/legal.c" "#include <lib/storage/medium_interface.h>\n")
file(WRITE "${fixture}/src/app-softpc/softpc.new/legal.c" "#include <app-softpc/compat/ccpu/lifecycle.h>\n")
file(WRITE "${fixture}/src/app-softpc/softpc.new/original.c" "#include <compat/devices/snapshot.h>\n")
function(check expected)
    execute_process(COMMAND "${CMAKE_COMMAND}" "-DSOFTPC_SOURCE_DIR=${fixture}"
        -P "${SOFTPC_SOURCE_DIR}/tools/checks/product_boundary.cmake"
        RESULT_VARIABLE result OUTPUT_QUIET ERROR_VARIABLE error)
    if(expected STREQUAL "pass")
        if(NOT result EQUAL 0)
            message(FATAL_ERROR "Legal composition rejected: ${error}")
        endif()
    elseif(result EQUAL 0 OR NOT error MATCHES "Product boundary")
        message(FATAL_ERROR "Illegal dependency accepted or wrong failure for ${path}|${header}: ${error}")
    endif()
endfunction()
check(pass)
foreach(pair IN ITEMS "app-softpc/product/config.c|../machine/vm_interface.h" "app-softpc/product/main.c|app-softpc/machine/vm_interface.h"
    "app-softpc/product/composition.h|app-softpc/machine/vm_interface.h" "app-softpc/product/main.c|app-softpc/compat/platform.h"
    "app-softpc/product/command.h|../compat/platform.h" "app-softpc/machine/vm_interface.h|app-softpc/compat/platform.h"
    "app-softpc/machine/driver.c|app-softpc/product/config.h" "app-softpc/compat/platform.c|common/machine/machine_interface.h"
    "app-softpc/product/composition.c|app-softpc/machine/driver.h" "app-softpc/compat/platform.c|../machine/driver.h"
    "common/control.c|../app-softpc/machine/vm_interface.h" "common/control.c|../app-softpc/compat/platform.h"
    "common/control.c|app-softpc/softpc.new/base/inc/cpu4.h" "common/control.c|app-softpc/product/config.h"
    "lib/boundary.c|../common/machine/machine_interface.h" "lib/boundary.c|../app-softpc/machine/driver.h"
    "lib/boundary.c|app-softpc/compat/platform.h" "lib/boundary.c|app-softpc/softpc.new/base/inc/cpu4.h"
    "app-softpc/softpc.new/core.c|app-softpc/machine/driver.h" "app-softpc/softpc.new/core.c|common/machine/machine_interface.h"
    "common/control.c|../x86/debug/protocol_interface.h"
    "lib/boundary.c|x86/debug/protocol_interface.h"
    "app-softpc/compat/platform.c|x86/debug/protocol_interface.h"
    "app-softpc/softpc.new/core.c|x86/debug/protocol_interface.h" "x86/probe.c|../app-softpc/machine/driver.h")
    string(REPLACE "|" ";" parts "${pair}")
    list(GET parts 0 path)
    list(GET parts 1 header)
    file(WRITE "${fixture}/src/${path}" "#include \"${header}\"\n")
    check(fail)
    file(REMOVE "${fixture}/src/${path}")
endforeach()
file(WRITE "${fixture}/src/app-softpc/product/composition.c" "void f(void) { vm_driver_create(0, 0); }\n")
check(fail)
file(REMOVE "${fixture}/src/app-softpc/product/composition.c")
file(WRITE "${fixture}/src/common/absolute.c" "#include \"${fixture}/src/app-softpc/machine/driver.h\"\n")
check(fail)
file(REMOVE_RECURSE "${fixture}")
