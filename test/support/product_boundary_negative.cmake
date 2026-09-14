set(fixture "${CMAKE_CURRENT_BINARY_DIR}/product-boundary-fixture")
file(REMOVE_RECURSE "${fixture}")
file(MAKE_DIRECTORY "${fixture}/src/app" "${fixture}/src/vm" "${fixture}/src/compat"
    "${fixture}/src/common" "${fixture}/src/lib" "${fixture}/src/mvdm")
file(WRITE "${fixture}/src/vm/vm_interface.h" "#include <common/machine/machine_interface.h>\n")
file(WRITE "${fixture}/src/app/composition.c" "#include <vm/vm_interface.h>\n")
file(WRITE "${fixture}/src/app/legal.c" "#include <lib/storage/file_interface.h>\n")
file(WRITE "${fixture}/src/common/legal.c" "#include <lib/host/sync_interface.h>\n")
file(WRITE "${fixture}/src/vm/legal.c" "#include <compat/machine.h>\n")
file(WRITE "${fixture}/src/compat/legal.c" "#include <lib/storage/medium_interface.h>\n")
file(WRITE "${fixture}/src/mvdm/legal.c" "#include <compat/ccpu/lifecycle.h>\n")
function(check expected)
    execute_process(COMMAND "${CMAKE_COMMAND}" "-DSOFTPC_SOURCE_DIR=${fixture}"
        -P "${SOFTPC_SOURCE_DIR}/test/support/product_boundary.cmake"
        RESULT_VARIABLE result OUTPUT_QUIET ERROR_VARIABLE error)
    if(expected STREQUAL "pass")
        if(NOT result EQUAL 0)
            message(FATAL_ERROR "Legal composition rejected: ${error}")
        endif()
    elseif(result EQUAL 0 OR NOT error MATCHES "Product boundary")
        message(FATAL_ERROR "Illegal dependency accepted or wrong failure: ${error}")
    endif()
endfunction()
check(pass)
foreach(pair IN ITEMS "app/config.c|../vm/vm_interface.h" "app/main.c|vm/vm_interface.h"
    "app/composition.h|vm/vm_interface.h" "app/main.c|compat/machine.h"
    "app/command.h|../compat/machine.h" "vm/vm_interface.h|compat/machine.h"
    "vm/driver.c|app/config.h" "compat/platform.c|common/machine/machine_interface.h"
    "app/composition.c|vm/driver.h" "compat/platform.c|../vm/driver.h"
    "common/control.c|../vm/vm_interface.h" "common/control.c|../compat/machine.h"
    "common/control.c|mvdm/softpc.new/base/inc/cpu4.h" "common/control.c|app/config.h"
    "lib/boundary.c|../common/machine/machine_interface.h" "lib/boundary.c|../vm/driver.h"
    "lib/boundary.c|compat/machine.h" "lib/boundary.c|mvdm/softpc.new/base/inc/cpu4.h"
    "mvdm/core.c|vm/driver.h" "mvdm/core.c|common/machine/machine_interface.h")
    string(REPLACE "|" ";" parts "${pair}")
    list(GET parts 0 path)
    list(GET parts 1 header)
    file(WRITE "${fixture}/src/${path}" "#include \"${header}\"\n")
    check(fail)
    file(REMOVE "${fixture}/src/${path}")
endforeach()
file(WRITE "${fixture}/src/app/composition.c" "void f(void) { vm_driver_create(0, 0); }\n")
check(fail)
file(REMOVE "${fixture}/src/app/composition.c")
file(WRITE "${fixture}/src/common/absolute.c" "#include \"${fixture}/src/vm/driver.h\"\n")
check(fail)
file(REMOVE_RECURSE "${fixture}")
