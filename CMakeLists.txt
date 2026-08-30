cmake_minimum_required(VERSION 3.23)
project(softpcvm LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(softpc-machine
    src/core/softpc_machine.c)
target_include_directories(softpc-machine PUBLIC include)
target_compile_definitions(softpc-machine PRIVATE _CRT_SECURE_NO_WARNINGS)

# The original CCPU manifest is the executable x86 core.  It deliberately
# links no NTVDM host, DOS/WOW component or Win32 presentation provider.
# A narrow original-ROM BIOS Operation table is machine-resident and only
# registers restored hardware services.
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/src/core/softpc/base/ccpu386/c-files"
    SOFTPC_CCPU_SOURCE_NAMES)
list(FILTER SOFTPC_CCPU_SOURCE_NAMES EXCLUDE REGEX "^p\\.")
set(SOFTPC_CCPU_SOURCES)
foreach(SOFTPC_CCPU_SOURCE_NAME IN LISTS SOFTPC_CCPU_SOURCE_NAMES)
    list(APPEND SOFTPC_CCPU_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/core/softpc/base/ccpu386/${SOFTPC_CCPU_SOURCE_NAME}")
endforeach()
file(GLOB SOFTPC_CVIDC_GENERATED_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core/softpc/base/cvidc/sinit*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core/softpc/base/cvidc/sevid*.c")
add_library(softpc-ccpu-core OBJECT
    ${SOFTPC_CCPU_SOURCES}
    src/core/softpc/base/ccpu386/softpc_ccpu_facade.c
    src/core/softpc/base/ccpu386/vglob.c
    ${SOFTPC_CVIDC_GENERATED_SOURCES}
    src/core/softpc/base/cvidc/ev_glue.c
    src/core/softpc/base/cvidc/evidfunc.c
    src/core/softpc/base/cvidc/j_c_lang.c
    src/core/softpc/base/cvidc/accessfn.c
    src/core/softpc/base/cvidc/vglfunc.c
    src/core/softpc/base/cvidc/softpc_gdp_state.c
    src/core/softpc/base/cvidc/sascdef.c
    src/core/softpc/base/support/ios.c
    src/core/softpc/base/system/ica.c
    src/core/softpc/base/system/quick_ev.c
    src/core/softpc/base/system/at_dma.c
    src/core/softpc/base/system/cmos.c
    src/core/softpc/base/system/idetect.c
    src/core/softpc/base/system/rom.c
    src/core/softpc/base/system/timer.c
    src/core/softpc/base/system/timestrb.c
    src/core/softpc/base/support/time_day.c
    src/core/softpc/base/bios/cmos_bis.c
    src/core/softpc/base/comms/printer.c
    src/core/softpc/base/comms/printer_.c
    src/core/softpc/base/keymouse/ppi.c
    src/core/softpc/base/keymouse/keyba.c
    src/core/softpc/base/keymouse/keybd_io.c
    src/core/softpc/base/disks/fla.c
    src/core/softpc/base/disks/gfi.c
    src/core/softpc/base/disks/gfi_mpty.c
    src/core/softpc/base/disks/floppy.c
    src/core/softpc/base/disks/floppy_i.c
    src/core/softpc/base/disks/fdisk.c
    src/core/softpc/base/video/ega_mode.c
    src/core/softpc/base/video/ega_prts.c
    src/core/softpc/base/video/ega_vide.c
    src/core/softpc/base/video/gfx_updt.c
    src/core/softpc/base/video/video.c
    src/core/softpc/base/video/gvi.c
    src/core/softpc/base/video/video_io.c
    src/core/softpc/base/video/v7_ports.c
    src/core/softpc/base/video/v7_video.c
    src/core/softpc/base/video/vga_mode.c
    src/core/softpc/base/video/vga_prts.c
    src/core/softpc/base/video/vga_vide.c
    src/core/softpc/base/video/ega_read.c
    src/core/softpc/base/video/ega_writ.c
    src/core/softpc/base/video/ega_dmmy.c
    src/core/softpc/base/video/ega_trcr.c
    src/core/softpc/base/video/egawrtm0.c
    src/core/softpc/base/video/egwrtm12.c
    src/core/softpc/base/video/cga.c
    src/core/softpc_device_bop.c
    src/core/softpc_gfi_image.c
    src/core/softpc_standalone_platform.c
    src/core/softpc_physical_mapping.c)
set_target_properties(softpc-ccpu-core PROPERTIES
    C_STANDARD 90
    C_EXTENSIONS ON)
target_compile_definitions(softpc-ccpu-core PRIVATE
    CPU_40_STYLE CCPU C_VID CPU_486 PROD SOFTPC_STANDALONE=1)
target_include_directories(softpc-ccpu-core PRIVATE
    src/core/softpc/host/inc
    src/core/softpc/host/inc/x86/prod
    src/core/softpc/base/inc
    src/core/softpc/base/ccpu386
    src/core/softpc/base/cvidc
    src/core)

# Keep the executor in the machine library so that every unresolved host edge
# is visible to the standalone build, rather than being hidden in an unused
# object target.
target_sources(softpc-machine PRIVATE $<TARGET_OBJECTS:softpc-ccpu-core>)
target_include_directories(softpc-machine PRIVATE
    src/core/softpc/host/inc
    src/core/softpc/host/inc/x86/prod
    src/core/softpc/base/inc
    src/core/softpc/base/ccpu386
    src/core/softpc/base/cvidc
    src/core)
target_compile_definitions(softpc-machine PRIVATE SOFTPC_STANDALONE=1)

add_executable(softpcvm
    src/vm/main.c
    src/vm/console.c
    src/vm/win32_window.c)
target_link_libraries(softpcvm PRIVATE softpc-machine)
if(WIN32)
    target_link_libraries(softpcvm PRIVATE user32 gdi32)
endif()

enable_testing()
add_executable(softpc-machine-smoke tests/softpc_machine_smoke.c)
target_link_libraries(softpc-machine-smoke PRIVATE softpc-machine)
add_test(NAME softpc-machine-smoke COMMAND softpc-machine-smoke)
set_tests_properties(softpc-machine-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-irq-smoke tests/softpc_irq_smoke.c)
target_link_libraries(softpc-irq-smoke PRIVATE softpc-machine)
add_test(NAME softpc-irq-smoke COMMAND softpc-irq-smoke)
set_tests_properties(softpc-irq-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-fdc-smoke tests/softpc_fdc_smoke.c)
target_link_libraries(softpc-fdc-smoke PRIVATE softpc-machine)
target_include_directories(softpc-fdc-smoke PRIVATE
    src/core/softpc/host/inc
    src/core/softpc/host/inc/x86/prod
    src/core/softpc/base/inc)
add_test(NAME softpc-fdc-smoke COMMAND softpc-fdc-smoke)
set_tests_properties(softpc-fdc-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-printer-smoke tests/softpc_printer_smoke.c)
target_link_libraries(softpc-printer-smoke PRIVATE softpc-machine)
target_include_directories(softpc-printer-smoke PRIVATE
    src/core/softpc/host/inc
    src/core/softpc/host/inc/x86/prod
    src/core/softpc/base/inc)
add_test(NAME softpc-printer-smoke COMMAND softpc-printer-smoke)
set_tests_properties(softpc-printer-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-real-boot-smoke tests/softpc_real_boot_smoke.c)
target_link_libraries(softpc-real-boot-smoke PRIVATE softpc-machine)
add_test(NAME softpc-standalone-source-boundary
    COMMAND "${CMAKE_COMMAND}" "-DSOFTPC_SOURCE_DIR=${CMAKE_SOURCE_DIR}" -P
        "${CMAKE_SOURCE_DIR}/tests/standalone_source_boundary.cmake")
add_test(NAME softpcvm-help COMMAND softpcvm --help)
