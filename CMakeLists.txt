cmake_minimum_required(VERSION 3.23)
project(softpcvm LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# SoftPC's selected CPU_40_STYLE machine is supported as either an x86 or
# x64 host build.  Keep the host word size explicit: historical code must use
# ULONG_PTR/IUH at host-pointer boundaries, never assume that ULONG is a
# pointer.  A 32-bit CMake toolchain selects x86; a native 64-bit toolchain
# selects x64 without changing the guest machine profile.
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 4 AND NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "SoftPC requires a 32-bit or 64-bit host pointer model")
endif()
math(EXPR SOFTPC_HOST_POINTER_BITS "${CMAKE_SIZEOF_VOID_P} * 8")
message(STATUS "SoftPC host pointer width: ${SOFTPC_HOST_POINTER_BITS}-bit")

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
file(GLOB SOFTPC_CVIDC_GENERATED_INPUTS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core/softpc/base/cvidc/sinit*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core/softpc/base/cvidc/sevid*.c")
find_package(Python3 REQUIRED COMPONENTS Interpreter)
set(SOFTPC_CVIDC_TRANSFORMED_SOURCES)
foreach(SOFTPC_CVIDC_GENERATED_INPUT IN LISTS SOFTPC_CVIDC_GENERATED_INPUTS)
    get_filename_component(SOFTPC_CVIDC_GENERATED_NAME
        "${SOFTPC_CVIDC_GENERATED_INPUT}" NAME)
    set(SOFTPC_CVIDC_TRANSFORMED_SOURCE
        "${CMAKE_CURRENT_BINARY_DIR}/generated-cvid/${SOFTPC_CVIDC_GENERATED_NAME}")
    add_custom_command(
        OUTPUT "${SOFTPC_CVIDC_TRANSFORMED_SOURCE}"
        COMMAND "${Python3_EXECUTABLE}"
            "${CMAKE_CURRENT_SOURCE_DIR}/scripts/transform_cvid_gdp.py"
            "${SOFTPC_CVIDC_GENERATED_INPUT}"
            "${SOFTPC_CVIDC_TRANSFORMED_SOURCE}"
        DEPENDS "${SOFTPC_CVIDC_GENERATED_INPUT}"
            "${CMAKE_CURRENT_SOURCE_DIR}/scripts/transform_cvid_gdp.py"
        COMMENT "Transforming C-VID GDP layout ${SOFTPC_CVIDC_GENERATED_NAME}"
        VERBATIM)
    list(APPEND SOFTPC_CVIDC_TRANSFORMED_SOURCES
        "${SOFTPC_CVIDC_TRANSFORMED_SOURCE}")
endforeach()
# ``j_c_lang`` is the generated-rule register file.  It carries private host
# pointers (not guest registers), so x64 needs a pointer-width IUH only at
# this generated-rule ABI boundary.  On an x86 host, the original generated
# signatures already have the correct native widths; forcing the x64 rule
# word there corrupts their pointer and void function-table declarations.
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set_source_files_properties(
        ${SOFTPC_CVIDC_TRANSFORMED_SOURCES}
        src/core/softpc/base/cvidc/j_c_lang.c
        src/core/softpc/base/cvidc/ev_glue.c
        src/core/softpc/base/cvidc/vglfunc.c
        PROPERTIES COMPILE_DEFINITIONS CVIDC_RULE_WORD=1)
endif()
add_library(softpc-ccpu-core OBJECT
    ${SOFTPC_CCPU_SOURCES}
    src/core/softpc/base/ccpu386/ntstubs.c
    src/core/softpc/base/ccpu386/softpc_ccpu_facade.c
    src/core/softpc/base/ccpu386/vglob.c
    ${SOFTPC_CVIDC_TRANSFORMED_SOURCES}
    src/core/softpc/base/cvidc/ev_glue.c
    src/core/softpc/base/cvidc/evidfunc.c
    src/core/softpc/base/cvidc/j_c_lang.c
    src/core/softpc/base/cvidc/accessfn.c
    src/core/softpc/base/cvidc/vglfunc.c
    src/core/softpc/base/cvidc/softpc_gdp_state.c
    src/core/softpc/base/cvidc/sascdef.c)

# Controllers, firmware and the standalone host boundary evolve independently
# of the CCPU/C-VIDC executor.  Keep them in a separate object target so a
# recovered original device does not force recompilation of the full executor.
add_library(softpc-machine-devices OBJECT
    src/core/softpc/base/support/ios.c
    src/core/softpc/base/system/ica.c
    src/core/softpc/base/system/quick_ev.c
    src/core/softpc/base/system/at_dma.c
    src/core/softpc/base/system/cmos.c
    src/core/softpc/base/system/dummy_nt.c
    src/core/softpc/base/system/idetect.c
    src/core/softpc/base/system/illegalp.c
    src/core/softpc/base/system/rom.c
    src/core/softpc/base/system/timer.c
    src/core/softpc/base/system/timestrb.c
    src/core/softpc/base/system/unexp_nt.c
    src/core/softpc/base/support/time_day.c
    src/core/softpc/base/bios/cmos_bis.c
    src/core/softpc/base/bios/build_id.c
    src/core/softpc/base/bios/reset.c
    src/core/softpc/base/bios/bootstra.c
    src/core/softpc/base/bios/slave_bs.c
    src/core/softpc/base/bios/tape_io.c
    src/core/softpc/base/bios/equipmnt.c
    src/core/softpc/base/bios/mem_size.c
    src/core/softpc/base/bios/rom_basc.c
    src/core/softpc/base/comms/printer.c
    src/core/softpc/base/comms/printer_.c
    src/core/softpc/base/comms/com.c
    src/core/softpc/base/comms/rs232_io.c
    src/core/softpc/base/keymouse/ppi.c
    src/core/softpc/base/keymouse/keyba.c
    src/core/softpc/base/keymouse/keybd_io.c
    src/core/softpc/base/keymouse/mouse.c
    src/core/softpc/base/keymouse/mouse_io.c
    src/core/softpc/base/disks/fla.c
    src/core/softpc/base/disks/gfi.c
    src/core/softpc/base/disks/gfi_mpty.c
    src/core/softpc/base/disks/floppy.c
    src/core/softpc/base/disks/floppy_i.c
    src/core/softpc/base/disks/fdisk.c
    src/core/softpc/base/disks/diskbios.c
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
    src/core/softpc/host/src/nt_ega.c
    src/core/softpc/host/src/nt_graph.c
    src/core/softpc/host/src/nt_keycd.c
    src/core/softpc/host/src/nt_lpt.c
    src/core/softpc/host/src/nt_sound.c
    src/core/softpc/host/src/nt_munge.c
    src/core/softpc/host/src/nt_cga.c
    src/core/softpc/host/src/nt_com.c
    src/core/softpc/host/src/nt_vga.c
    src/core/softpc_standalone_dib.c
    src/core/softpc_device_bop.c
    src/core/softpc_gfi_image.c
    src/core/softpc_standalone_platform.c)
set_target_properties(softpc-ccpu-core PROPERTIES
    C_STANDARD 90
    C_EXTENSIONS ON)
set_target_properties(softpc-machine-devices PROPERTIES
    C_STANDARD 90
    C_EXTENSIONS ON)
target_compile_definitions(softpc-ccpu-core PRIVATE
    CPU_40_STYLE CCPU C_VID CPU_486 V7VGA PROD SOFTPC_STANDALONE=1
    SOFTPC_HOST_POINTER_BITS=${SOFTPC_HOST_POINTER_BITS})
target_compile_definitions(softpc-machine-devices PRIVATE
    CPU_40_STYLE CCPU C_VID CPU_486 V7VGA PROD SOFTPC_STANDALONE=1
    SOFTPC_HOST_POINTER_BITS=${SOFTPC_HOST_POINTER_BITS})
target_include_directories(softpc-ccpu-core PRIVATE
    src/core/softpc/host/inc
    src/core/softpc/host/inc/x86/prod
    src/core/softpc/base/inc
    src/core/softpc/base/ccpu386
    src/core/softpc/base/cvidc
    src/core)
target_include_directories(softpc-machine-devices PRIVATE
    include
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
target_sources(softpc-machine PRIVATE $<TARGET_OBJECTS:softpc-machine-devices>)
target_include_directories(softpc-machine PRIVATE
    src/core/softpc/host/inc
    src/core/softpc/host/inc/x86/prod
    src/core/softpc/base/inc
    src/core/softpc/base/ccpu386
    src/core/softpc/base/cvidc
    src/core)
target_compile_definitions(softpc-machine PRIVATE
    SOFTPC_STANDALONE=1 SOFTPC_HOST_POINTER_BITS=${SOFTPC_HOST_POINTER_BITS})

add_executable(softpcvm
    src/vm/main.c
    src/vm/console.c
    src/vm/win32_window.c)
target_link_libraries(softpcvm PRIVATE softpc-machine)
if(WIN32)
    target_link_libraries(softpcvm PRIVATE user32 gdi32)
endif()

# The standalone distribution has one fixed configuration alongside the two
# host-width launchers.  A 32-bit and a 64-bit configure can therefore share
# one stable delivery directory without overwriting each other's executable.
set(SOFTPC_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/output")
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(SOFTPC_LAUNCHER_NAME softpc32)
else()
    set(SOFTPC_LAUNCHER_NAME softpc64)
endif()
file(MAKE_DIRECTORY "${SOFTPC_OUTPUT_DIRECTORY}")
set_target_properties(softpcvm PROPERTIES
    OUTPUT_NAME "${SOFTPC_LAUNCHER_NAME}"
    RUNTIME_OUTPUT_DIRECTORY "${SOFTPC_OUTPUT_DIRECTORY}")
if(NOT EXISTS "${SOFTPC_OUTPUT_DIRECTORY}/softpc.ini")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/softpc.ini"
        "${SOFTPC_OUTPUT_DIRECTORY}/softpc.ini" COPYONLY)
endif()

enable_testing()
add_executable(softpc-machine-smoke tests/softpc_machine_smoke.c)
target_link_libraries(softpc-machine-smoke PRIVATE softpc-machine)
add_test(NAME softpc-machine-smoke COMMAND softpc-machine-smoke)
set_tests_properties(softpc-machine-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-dual-media-smoke tests/softpc_dual_media_smoke.c)
target_link_libraries(softpc-dual-media-smoke PRIVATE softpc-machine)
target_include_directories(softpc-dual-media-smoke PRIVATE
    src/core/softpc/host/inc
    src/core/softpc/host/inc/x86/prod
    src/core/softpc/base/inc)
add_test(NAME softpc-dual-media-smoke COMMAND softpc-dual-media-smoke)
set_tests_properties(softpc-dual-media-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-irq-smoke tests/softpc_irq_smoke.c)
target_link_libraries(softpc-irq-smoke PRIVATE softpc-machine)
target_include_directories(softpc-irq-smoke PRIVATE
    src/core/softpc/host/inc
    src/core/softpc/host/inc/x86/prod
    src/core/softpc/base/inc)
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
add_executable(softpc-sound-smoke tests/softpc_sound_smoke.c)
target_link_libraries(softpc-sound-smoke PRIVATE softpc-machine)
target_include_directories(softpc-sound-smoke PRIVATE
    src/core/softpc/host/inc
    src/core/softpc/host/inc/x86/prod
    src/core/softpc/base/inc)
add_test(NAME softpc-sound-smoke COMMAND softpc-sound-smoke)
set_tests_properties(softpc-sound-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-serial-smoke tests/softpc_serial_smoke.c)
target_link_libraries(softpc-serial-smoke PRIVATE softpc-machine)
target_include_directories(softpc-serial-smoke PRIVATE
    src/core/softpc/host/inc
    src/core/softpc/host/inc/x86/prod
    src/core/softpc/base/inc)
add_test(NAME softpc-serial-smoke COMMAND softpc-serial-smoke)
set_tests_properties(softpc-serial-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-mouse-smoke tests/softpc_mouse_smoke.c)
target_link_libraries(softpc-mouse-smoke PRIVATE softpc-machine)
target_include_directories(softpc-mouse-smoke PRIVATE
    src/core/softpc/host/inc
    src/core/softpc/host/inc/x86/prod
    src/core/softpc/base/inc)
add_test(NAME softpc-mouse-smoke COMMAND softpc-mouse-smoke)
set_tests_properties(softpc-mouse-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-keycode-smoke tests/softpc_keycode_smoke.c)
target_link_libraries(softpc-keycode-smoke PRIVATE softpc-machine)
target_include_directories(softpc-keycode-smoke PRIVATE
    src/core/softpc/host/inc
    src/core/softpc/host/inc/x86/prod
    src/core/softpc/base/inc)
add_test(NAME softpc-keycode-smoke COMMAND softpc-keycode-smoke)
set_tests_properties(softpc-keycode-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-vga-frame-smoke tests/softpc_vga_frame_smoke.c)
target_link_libraries(softpc-vga-frame-smoke PRIVATE softpc-machine)
target_include_directories(softpc-vga-frame-smoke PRIVATE
    src/core/softpc/host/inc
    src/core/softpc/host/inc/x86/prod
    src/core/softpc/base/inc)
target_compile_definitions(softpc-vga-frame-smoke PRIVATE
    CPU_40_STYLE CCPU C_VID CPU_486 V7VGA PROD SOFTPC_STANDALONE=1
    SOFTPC_HOST_POINTER_BITS=${SOFTPC_HOST_POINTER_BITS})
add_test(NAME softpc-vga-frame-smoke COMMAND softpc-vga-frame-smoke)
set_tests_properties(softpc-vga-frame-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-bop-smoke tests/softpc_bop_smoke.c)
target_link_libraries(softpc-bop-smoke PRIVATE softpc-machine)
target_include_directories(softpc-bop-smoke PRIVATE
    src/core/softpc/host/inc
    src/core/softpc/host/inc/x86/prod
    src/core/softpc/base/inc)
add_test(NAME softpc-bop-smoke COMMAND softpc-bop-smoke)
set_tests_properties(softpc-bop-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-quick-time-smoke tests/softpc_quick_time_smoke.c)
target_link_libraries(softpc-quick-time-smoke PRIVATE softpc-machine)
target_include_directories(softpc-quick-time-smoke PRIVATE
    src/core/softpc/host/inc
    src/core/softpc/host/inc/x86/prod
    src/core/softpc/base/inc)
add_test(NAME softpc-quick-time-smoke COMMAND softpc-quick-time-smoke)
set_tests_properties(softpc-quick-time-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-real-boot-smoke tests/softpc_real_boot_smoke.c)
target_link_libraries(softpc-real-boot-smoke PRIVATE softpc-machine)
add_test(NAME softpc-standalone-source-boundary
    COMMAND "${CMAKE_COMMAND}" "-DSOFTPC_SOURCE_DIR=${CMAKE_SOURCE_DIR}" -P
        "${CMAKE_SOURCE_DIR}/tests/standalone_source_boundary.cmake")
add_test(NAME softpcvm-reject-arguments COMMAND softpcvm --help)
set_tests_properties(softpcvm-reject-arguments PROPERTIES WILL_FAIL TRUE)

# Smoke tests intentionally use assert() for both the operation and its
# postcondition.  Keep those checks live even when validating an optimized
# machine library; production targets retain their normal Release NDEBUG.
set(SOFTPC_TEST_TARGETS
    softpc-machine-smoke
    softpc-dual-media-smoke
    softpc-irq-smoke
    softpc-fdc-smoke
    softpc-printer-smoke
    softpc-sound-smoke
    softpc-serial-smoke
    softpc-mouse-smoke
    softpc-keycode-smoke
    softpc-vga-frame-smoke
    softpc-bop-smoke
    softpc-quick-time-smoke
    softpc-real-boot-smoke)
foreach(softpc_test_target IN LISTS SOFTPC_TEST_TARGETS)
    if(MSVC)
        target_compile_options(${softpc_test_target} PRIVATE /UNDEBUG)
    else()
        target_compile_options(${softpc_test_target} PRIVATE -UNDEBUG)
    endif()
endforeach()
