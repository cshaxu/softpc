set(fixture "${CMAKE_CURRENT_BINARY_DIR}/product-boundary-fixture")
file(REMOVE_RECURSE "${fixture}")
file(MAKE_DIRECTORY "${fixture}/src/app" "${fixture}/src/core/machine" "${fixture}/src/core/compat"
    "${fixture}/src/common" "${fixture}/src/lib" "${fixture}/src/x86" "${fixture}/src/core/softpc.new")
file(WRITE "${fixture}/src/core/machine/vm_interface.h" "#include <common/machine/machine_interface.h>\n")
file(WRITE "${fixture}/src/app/composition.c" "#include <core/machine/vm_interface.h>\n")
file(WRITE "${fixture}/src/app/legal.c" "#include <lib/storage/file_interface.h>\n")
file(WRITE "${fixture}/src/common/legal.c" "#include <lib/base/sync_interface.h>\n")
file(WRITE "${fixture}/src/x86/legal.c" "#include <common/machine/machine_interface.h>\n")
file(WRITE "${fixture}/src/core/machine/legal.c" "#include <core/compat/platform.h>\n")
file(WRITE "${fixture}/src/core/compat/legal.c" "#include <lib/storage/medium_interface.h>\n")
file(WRITE "${fixture}/src/core/softpc.new/legal.c" "#include <core/compat/ccpu/lifecycle.h>\n")
file(WRITE "${fixture}/src/core/softpc.new/original.c" "#include <compat/devices/snapshot.h>\n")
function(check expected)
    execute_process(COMMAND "${CMAKE_COMMAND}" "-DSOFTPC_SOURCE_DIR=${fixture}"
        -P "${SOFTPC_SOURCE_DIR}/test/checks/product_boundary.cmake"
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
foreach(pair IN ITEMS "app/config.c|../core/machine/vm_interface.h" "app/main.c|core/machine/vm_interface.h"
    "app/composition.h|core/machine/vm_interface.h" "app/main.c|core/compat/platform.h"
    "app/command.h|../core/compat/platform.h" "core/machine/vm_interface.h|core/compat/platform.h"
    "core/machine/driver.c|app/config.h" "core/compat/platform.c|common/machine/machine_interface.h"
    "app/composition.c|core/machine/driver.h" "core/compat/platform.c|../machine/driver.h"
    "common/control.c|../core/machine/vm_interface.h" "common/control.c|../core/compat/platform.h"
    "common/control.c|core/softpc.new/base/inc/cpu4.h" "common/control.c|app/config.h"
    "lib/boundary.c|../common/machine/machine_interface.h" "lib/boundary.c|../core/machine/driver.h"
    "lib/boundary.c|core/compat/platform.h" "lib/boundary.c|core/softpc.new/base/inc/cpu4.h"
    "core/softpc.new/core.c|core/machine/driver.h" "core/softpc.new/core.c|common/machine/machine_interface.h"
    "common/control.c|../x86/debug/protocol_interface.h"
    "lib/boundary.c|x86/debug/protocol_interface.h"
    "core/compat/platform.c|x86/debug/protocol_interface.h"
    "core/softpc.new/core.c|x86/debug/protocol_interface.h" "x86/probe.c|../core/machine/driver.h")
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
file(WRITE "${fixture}/src/common/absolute.c" "#include \"${fixture}/src/core/machine/driver.h\"\n")
check(fail)
file(REMOVE_RECURSE "${fixture}")
