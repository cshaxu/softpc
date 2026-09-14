cmake_minimum_required(VERSION 3.23)
project(softpcvm LANGUAGES C RC)

# The CCPU is a full instruction interpreter. A single-config generator
# otherwise leaves CMAKE_BUILD_TYPE empty and silently emits an unoptimised
# VM, turning ordinary guest setup scans into multi-minute stalls. Keep an
# explicit user-selected configuration intact; only default a fresh
# single-config tree to Release.
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

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
set(SOFTPC_ARTIFACT_ARCHITECTURE "${SOFTPC_ARTIFACT_ARCHITECTURE}" CACHE STRING
    "Declared package architecture: x86 or x64")
if(NOT SOFTPC_ARTIFACT_ARCHITECTURE STREQUAL "")
    if(NOT SOFTPC_ARTIFACT_ARCHITECTURE MATCHES "^(x86|x64)$")
        message(FATAL_ERROR "SOFTPC_ARTIFACT_ARCHITECTURE must be x86 or x64")
    endif()
    if((CMAKE_SIZEOF_VOID_P EQUAL 4 AND NOT SOFTPC_ARTIFACT_ARCHITECTURE STREQUAL "x86")
        OR (CMAKE_SIZEOF_VOID_P EQUAL 8 AND NOT SOFTPC_ARTIFACT_ARCHITECTURE STREQUAL "x64"))
        message(FATAL_ERROR "SOFTPC_ARTIFACT_ARCHITECTURE does not match the selected compiler")
    endif()
endif()
# MSYS2's windres can otherwise infer the host architecture rather than the
# selected C toolchain.  Resources are linked into every x86 executable, so
# make the COFF target explicit.
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} --target=pe-i386")
endif()

add_library(softpc-machine
    src/vm/debug.c
    src/vm/input.c
    src/compat/machine.c)
target_include_directories(softpc-machine PUBLIC src PRIVATE src/compat)
target_compile_definitions(softpc-machine PRIVATE _CRT_SECURE_NO_WARNINGS)

# This is a source-synchronised local component, not a cross-repository
# dependency. Project bindings retain all machine and lifecycle decisions;
# the imported library owns generic host, storage, and KVM mechanics.
add_subdirectory(src/lib)
enable_testing()

add_subdirectory(src/common)
add_subdirectory(test/lib)
add_subdirectory(test/common)
target_link_libraries(softpc-machine PRIVATE storage host)

# The original CCPU manifest is the executable x86 core.  It deliberately
# links no NTVDM host, DOS/WOW component or Win32 presentation provider.
# A narrow original-ROM BIOS Operation table is machine-resident and only
# registers restored hardware services.
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/src/mvdm/softpc.new/base/ccpu386/c-files"
    SOFTPC_CCPU_SOURCE_NAMES)
list(FILTER SOFTPC_CCPU_SOURCE_NAMES EXCLUDE REGEX "^p\\.")
set(SOFTPC_CCPU_SOURCES)
foreach(SOFTPC_CCPU_SOURCE_NAME IN LISTS SOFTPC_CCPU_SOURCE_NAMES)
    list(APPEND SOFTPC_CCPU_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/mvdm/softpc.new/base/ccpu386/${SOFTPC_CCPU_SOURCE_NAME}")
endforeach()

# Keep the historical object/archive ordering while compiling the checked-in
# sources directly.  The old generated-source path happened to append these
# translation units after c-files; static archive resolution depends on that
# order.  This is source selection only, not a source transformation.
set(SOFTPC_CCPU_DIRECT_PORT_HEAD
    src/mvdm/softpc.new/base/ccpu386/ccpusas4.c
    src/mvdm/softpc.new/base/ccpu386/c_main.c)
set(SOFTPC_CCPU_DIRECT_PORT_TAIL
    src/mvdm/softpc.new/base/ccpu386/ntthread.c
    src/mvdm/softpc.new/base/ccpu386/fpu.c
    src/mvdm/softpc.new/base/ccpu386/zfrsrvd.c
    src/mvdm/softpc.new/base/ccpu386/c_page.c
    src/mvdm/softpc.new/base/ccpu386/c_reg.c
    src/mvdm/softpc.new/base/ccpu386/c_xcptn.c
    src/mvdm/softpc.new/base/ccpu386/popf.c)
list(REMOVE_ITEM SOFTPC_CCPU_SOURCES
    ${SOFTPC_CCPU_DIRECT_PORT_HEAD}
    ${SOFTPC_CCPU_DIRECT_PORT_TAIL})
file(GLOB SOFTPC_CVIDC_RULE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/mvdm/softpc.new/base/cvidc/sinit*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/mvdm/softpc.new/base/cvidc/sevid*.c")

# C-VID generated rules keep their original 32-bit rule-register layout, but
# their checked-in source now names the native-width GDP access boundary.
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set_source_files_properties(
        ${SOFTPC_CVIDC_RULE_SOURCES}
        src/mvdm/softpc.new/base/cvidc/j_c_lang.c
        src/mvdm/softpc.new/base/cvidc/ev_glue.c
        src/mvdm/softpc.new/base/cvidc/vglfunc.c
        PROPERTIES COMPILE_DEFINITIONS CVIDC_RULE_WORD=1)
endif()
set_source_files_properties(src/mvdm/softpc.new/base/bios/reset.c PROPERTIES
    COMPILE_OPTIONS "-iquote${CMAKE_CURRENT_SOURCE_DIR}/src/compat/bios")
add_library(softpc-ccpu-core OBJECT
    ${SOFTPC_CCPU_SOURCES}
    ${SOFTPC_CCPU_DIRECT_PORT_HEAD}
    src/mvdm/softpc.new/base/ccpu386/ntstubs.c
    ${SOFTPC_CCPU_DIRECT_PORT_TAIL}
    src/compat/ccpu/facade.c
    src/mvdm/softpc.new/base/ccpu386/vglob.c
    ${SOFTPC_CVIDC_RULE_SOURCES}
    src/mvdm/softpc.new/base/cvidc/ev_glue.c
    src/mvdm/softpc.new/base/cvidc/evidfunc.c
    src/mvdm/softpc.new/base/cvidc/j_c_lang.c
    src/mvdm/softpc.new/base/cvidc/accessfn.c
    src/mvdm/softpc.new/base/cvidc/vglfunc.c
    src/mvdm/softpc.new/base/cvidc/sascdef.c)
add_library(softpc-machine-compat OBJECT
    src/compat/cvidc/gdp_state.c
    src/compat/ccpu/lifecycle.c)

# Controllers, firmware and the standalone host boundary evolve independently
# of the CCPU/C-VIDC executor.  Keep them in a separate object target so a
# recovered original device does not force recompilation of the full executor.
set_source_files_properties(src/mvdm/softpc.new/base/system/illegalp.c PROPERTIES
    COMPILE_OPTIONS "-iquote${CMAKE_CURRENT_SOURCE_DIR}/src/compat/system")
set_source_files_properties(src/mvdm/softpc.new/base/keymouse/keyba.c PROPERTIES
    COMPILE_OPTIONS "-iquote${CMAKE_CURRENT_SOURCE_DIR}/src/compat/keymouse")
set_source_files_properties(src/mvdm/softpc.new/base/support/time_day.c PROPERTIES
    COMPILE_OPTIONS "-iquote${CMAKE_CURRENT_SOURCE_DIR}/src/mvdm/softpc.new/base/inc")
add_library(softpc-machine-devices OBJECT
    src/mvdm/softpc.new/base/support/ios.c
    src/mvdm/softpc.new/base/system/ica.c
    src/mvdm/softpc.new/base/system/quick_ev.c
    src/mvdm/softpc.new/base/system/at_dma.c
    src/mvdm/softpc.new/base/system/cmos.c
    src/mvdm/softpc.new/base/system/dummy_nt.c
    src/mvdm/softpc.new/base/system/idetect.c
    src/mvdm/softpc.new/base/system/illegalp.c
    src/mvdm/softpc.new/base/system/rom.c
    src/mvdm/softpc.new/base/system/timer.c
    src/mvdm/softpc.new/base/system/timestrb.c
    src/mvdm/softpc.new/base/system/unexp_nt.c
    src/mvdm/softpc.new/base/support/time_day.c
    src/mvdm/softpc.new/base/bios/cmos_bis.c
    src/mvdm/softpc.new/base/bios/build_id.c
    src/mvdm/softpc.new/base/bios/reset.c
    src/mvdm/softpc.new/base/bios/bootstra.c
    src/mvdm/softpc.new/base/bios/slave_bs.c
    src/mvdm/softpc.new/base/bios/tape_io.c
    src/mvdm/softpc.new/base/bios/equipmnt.c
    src/mvdm/softpc.new/base/bios/mem_size.c
    src/mvdm/softpc.new/base/bios/rom_basc.c
    src/mvdm/softpc.new/base/comms/printer.c
    src/mvdm/softpc.new/base/comms/printer_.c
    src/mvdm/softpc.new/base/comms/com.c
    src/mvdm/softpc.new/base/comms/rs232_io.c
    src/mvdm/softpc.new/base/keymouse/ppi.c
    src/mvdm/softpc.new/base/keymouse/keyba.c
    src/mvdm/softpc.new/base/keymouse/keybd_io.c
    src/mvdm/softpc.new/base/keymouse/mouse.c
    src/mvdm/softpc.new/base/keymouse/mouse_io.c
    src/mvdm/softpc.new/base/disks/fla.c
    src/mvdm/softpc.new/base/disks/gfi.c
    src/mvdm/softpc.new/base/disks/gfi_mpty.c
    src/mvdm/softpc.new/base/disks/floppy.c
    src/mvdm/softpc.new/base/disks/floppy_i.c
    src/mvdm/softpc.new/base/disks/fdisk.c
    src/mvdm/softpc.new/base/disks/diskbios.c
    src/mvdm/softpc.new/base/video/ega_mode.c
    src/mvdm/softpc.new/base/video/ega_prts.c
    src/mvdm/softpc.new/base/video/ega_vide.c
    src/mvdm/softpc.new/base/video/gfx_updt.c
    src/mvdm/softpc.new/base/video/video.c
    src/mvdm/softpc.new/base/video/gvi.c
    src/mvdm/softpc.new/base/video/video_io.c
    src/mvdm/softpc.new/base/video/v7_ports.c
    src/mvdm/softpc.new/base/video/v7_video.c
    src/mvdm/softpc.new/base/video/vga_mode.c
    src/mvdm/softpc.new/base/video/vga_prts.c
    src/mvdm/softpc.new/base/video/vga_vide.c
    src/mvdm/softpc.new/base/video/ega_read.c
    src/mvdm/softpc.new/base/video/ega_writ.c
    src/mvdm/softpc.new/base/video/ega_dmmy.c
    src/mvdm/softpc.new/base/video/ega_trcr.c
    src/mvdm/softpc.new/base/video/egawrtm0.c
    src/mvdm/softpc.new/base/video/egwrtm12.c
    src/mvdm/softpc.new/base/video/cga.c
    src/mvdm/softpc.new/host/src/nt_ega.c
    src/mvdm/softpc.new/host/src/nt_graph.c
    src/mvdm/softpc.new/host/src/nt_keycd.c
    src/mvdm/softpc.new/host/src/nt_sound.c
    src/mvdm/softpc.new/host/src/nt_munge.c
    src/mvdm/softpc.new/host/src/nt_cga.c
    src/mvdm/softpc.new/host/src/nt_vga.c
    src/compat/edl_fast_bop.c
    src/compat/graphics_console_compat.c
    src/compat/dib_surface.c
    src/compat/video.c
    src/compat/v7_pointer.c
    src/compat/device_bop.c
    src/compat/memory.c
    src/compat/gfi_image.c
    src/compat/hdd_media.c
    src/compat/serial.c
    src/compat/parallel.c
    src/compat/keyboard.c
    src/compat/mouse_instance.c
    src/compat/audio.c
    src/compat/platform.c)
set_target_properties(softpc-ccpu-core PROPERTIES
    C_STANDARD 90
    C_EXTENSIONS ON)
set_target_properties(softpc-machine-devices PROPERTIES
    C_STANDARD 90
    C_EXTENSIONS ON)
set_target_properties(softpc-machine-compat PROPERTIES
    C_STANDARD 90
    C_EXTENSIONS ON)
target_compile_definitions(softpc-ccpu-core PRIVATE
    CPU_40_STYLE CCPU C_VID CPU_486 SPC386 V7VGA WDCTRL_BOP PROD SOFTPC_STANDALONE=1
    SOFTPC_CCPU_TLS_SIMSTACK=1
    SOFTPC_HOST_POINTER_BITS=${SOFTPC_HOST_POINTER_BITS})
target_compile_definitions(softpc-machine-devices PRIVATE
    CPU_40_STYLE CCPU C_VID CPU_486 SPC386 V7VGA WDCTRL_BOP PROD SOFTPC_STANDALONE=1
    SOFTPC_HOST_POINTER_BITS=${SOFTPC_HOST_POINTER_BITS})
target_compile_definitions(softpc-machine-compat PRIVATE
    CPU_40_STYLE CCPU C_VID CPU_486 SPC386 V7VGA WDCTRL_BOP PROD SOFTPC_STANDALONE=1
    SOFTPC_HOST_POINTER_BITS=${SOFTPC_HOST_POINTER_BITS})
target_include_directories(softpc-ccpu-core PRIVATE
    src/compat/ccpu
    src/compat/cvidc
    src/compat/ccpu/legacy
    src/compat
    src/mvdm/softpc.new/host/inc
    src/mvdm/softpc.new/base/inc
    src/mvdm/softpc.new/base/ccpu386
    src/mvdm/softpc.new/base/cvidc)
target_include_directories(softpc-machine-devices PRIVATE
    src
    src/compat
    src/compat/ccpu
    src/compat/cvidc
    src/compat/ccpu/legacy
    src/mvdm/softpc.new/host/inc
    src/mvdm/softpc.new/base/inc
    src/mvdm/softpc.new/base/ccpu386
    src/mvdm/softpc.new/base/cvidc)
target_include_directories(softpc-machine-compat PRIVATE
    src/compat/ccpu
    src/compat/cvidc
    src/compat/ccpu/legacy
    src/compat
    src/mvdm/softpc.new/host/inc
    src/mvdm/softpc.new/base/inc
    src/mvdm/softpc.new/base/ccpu386
    src/mvdm/softpc.new/base/cvidc)

# Keep the executor in the machine library so that every unresolved host edge
# is visible to the standalone build, rather than being hidden in an unused
# object target.
target_sources(softpc-machine PRIVATE $<TARGET_OBJECTS:softpc-ccpu-core>)
target_sources(softpc-machine PRIVATE $<TARGET_OBJECTS:softpc-machine-devices>)
target_sources(softpc-machine PRIVATE $<TARGET_OBJECTS:softpc-machine-compat>)
target_include_directories(softpc-machine PRIVATE
    src/compat/ccpu
    src/compat/cvidc
    src/compat/ccpu/legacy
    src/compat
    src/mvdm/softpc.new/host/inc
    src/mvdm/softpc.new/base/inc
    src/mvdm/softpc.new/base/ccpu386
    src/mvdm/softpc.new/base/cvidc)
target_compile_definitions(softpc-machine PRIVATE
    SOFTPC_STANDALONE=1 SOFTPC_HOST_POINTER_BITS=${SOFTPC_HOST_POINTER_BITS})

add_executable(softpcvm
    src/app/main.c
    src/app/config.c
    src/app/composition.c
    src/vm/driver.c
    src/app/keyboard.c
    src/app/command.c
    src/vm/trace.c
    src/app/firmware.rc)
target_link_libraries(softpcvm PRIVATE softpc-machine)
target_link_libraries(softpcvm PRIVATE types storage kvm-base common-ui common-session common-machine common-debug)
if(WIN32)
    target_link_libraries(softpcvm PRIVATE user32 gdi32)
endif()

# The standalone distribution keeps its two host-width launchers together.
# The adjacent softpc.ini is deliberately user-owned configuration, never a
# CMake template or package-generation output.
set(SOFTPC_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/assets/binary")
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(SOFTPC_LAUNCHER_NAME softpc32)
else()
    set(SOFTPC_LAUNCHER_NAME softpc64)
endif()
file(MAKE_DIRECTORY "${SOFTPC_OUTPUT_DIRECTORY}")
set_target_properties(softpcvm PROPERTIES
    OUTPUT_NAME "${SOFTPC_LAUNCHER_NAME}"
    RUNTIME_OUTPUT_DIRECTORY "${SOFTPC_OUTPUT_DIRECTORY}")

enable_testing()
add_executable(softpc-machine-smoke test/unit/machine_smoke.c)
add_executable(softpc-config-smoke test/unit/config_smoke.c src/app/config.c)
target_include_directories(softpc-config-smoke PRIVATE src)
target_link_libraries(softpc-config-smoke PRIVATE types)
add_test(NAME softpc-config-smoke COMMAND softpc-config-smoke)
set_tests_properties(softpc-config-smoke PROPERTIES LABELS "unit")
add_test(NAME softpc-product-boundary-negative COMMAND ${CMAKE_COMMAND}
    -DSOFTPC_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
    -P ${CMAKE_CURRENT_SOURCE_DIR}/test/support/product_boundary_negative.cmake)
set_tests_properties(softpc-product-boundary-negative PROPERTIES LABELS "unit")
target_link_libraries(softpc-machine-smoke PRIVATE softpc-machine)
add_test(NAME softpc-machine-smoke COMMAND softpc-machine-smoke)
set_tests_properties(softpc-machine-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-dual-media-smoke test/unit/dual_media_smoke.c)
target_link_libraries(softpc-dual-media-smoke PRIVATE softpc-machine)
target_include_directories(softpc-dual-media-smoke PRIVATE
    src/mvdm/softpc.new/host/inc
    src/mvdm/softpc.new/base/inc)
add_test(NAME softpc-dual-media-smoke COMMAND softpc-dual-media-smoke)
set_tests_properties(softpc-dual-media-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-partition-image-smoke test/unit/partition_image_smoke.c)
target_link_libraries(softpc-partition-image-smoke PRIVATE softpc-machine)
add_test(NAME softpc-partition-image-smoke COMMAND softpc-partition-image-smoke)
set_tests_properties(softpc-partition-image-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-irq-smoke test/unit/irq_smoke.c)
target_link_libraries(softpc-irq-smoke PRIVATE softpc-machine)
target_include_directories(softpc-irq-smoke PRIVATE
    src/mvdm/softpc.new/host/inc
    src/mvdm/softpc.new/base/inc)
add_test(NAME softpc-irq-smoke COMMAND softpc-irq-smoke)
set_tests_properties(softpc-irq-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-fdc-smoke test/unit/fdc_smoke.c)
target_link_libraries(softpc-fdc-smoke PRIVATE softpc-machine)
target_include_directories(softpc-fdc-smoke PRIVATE
    src/mvdm/softpc.new/host/inc
    src/mvdm/softpc.new/base/inc)
add_test(NAME softpc-fdc-smoke COMMAND softpc-fdc-smoke)
set_tests_properties(softpc-fdc-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-printer-smoke test/unit/printer_smoke.c)
target_link_libraries(softpc-printer-smoke PRIVATE softpc-machine)
target_include_directories(softpc-printer-smoke PRIVATE
    src/mvdm/softpc.new/host/inc
    src/mvdm/softpc.new/base/inc)
add_test(NAME softpc-printer-smoke COMMAND softpc-printer-smoke)
set_tests_properties(softpc-printer-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-sound-smoke test/unit/sound_smoke.c)
target_link_libraries(softpc-sound-smoke PRIVATE softpc-machine)
target_include_directories(softpc-sound-smoke PRIVATE
    src/mvdm/softpc.new/host/inc
    src/mvdm/softpc.new/base/inc)
add_test(NAME softpc-sound-smoke COMMAND softpc-sound-smoke)
set_tests_properties(softpc-sound-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-audio-lifecycle-smoke test/unit/audio_lifecycle_smoke.c)
target_link_libraries(softpc-audio-lifecycle-smoke PRIVATE softpc-machine)
target_include_directories(softpc-audio-lifecycle-smoke PRIVATE src src/compat)
add_test(NAME softpc-audio-lifecycle-smoke COMMAND softpc-audio-lifecycle-smoke)
set_tests_properties(softpc-audio-lifecycle-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-serial-smoke test/unit/serial_smoke.c)
target_link_libraries(softpc-serial-smoke PRIVATE softpc-machine)
target_include_directories(softpc-serial-smoke PRIVATE
    src/mvdm/softpc.new/host/inc
    src/mvdm/softpc.new/base/inc)
add_test(NAME softpc-serial-smoke COMMAND softpc-serial-smoke)
set_tests_properties(softpc-serial-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-mouse-smoke test/unit/mouse_smoke.c)
target_link_libraries(softpc-mouse-smoke PRIVATE softpc-machine)
target_include_directories(softpc-mouse-smoke PRIVATE
    src/mvdm/softpc.new/host/inc
    src/mvdm/softpc.new/base/inc)
add_test(NAME softpc-mouse-smoke COMMAND softpc-mouse-smoke)
set_tests_properties(softpc-mouse-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-keycode-smoke test/unit/keycode_smoke.c)
target_link_libraries(softpc-keycode-smoke PRIVATE softpc-machine)
target_include_directories(softpc-keycode-smoke PRIVATE
    src/mvdm/softpc.new/host/inc
    src/mvdm/softpc.new/base/inc)
add_test(NAME softpc-keycode-smoke COMMAND softpc-keycode-smoke)
set_tests_properties(softpc-keycode-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-vga-frame-smoke test/unit/vga_frame_smoke.c)
target_link_libraries(softpc-vga-frame-smoke PRIVATE softpc-machine)
target_include_directories(softpc-vga-frame-smoke PRIVATE
    src/mvdm/softpc.new/host/inc
    src/mvdm/softpc.new/base/inc)
target_compile_definitions(softpc-vga-frame-smoke PRIVATE
    CPU_40_STYLE CCPU C_VID CPU_486 V7VGA PROD SOFTPC_STANDALONE=1
    SOFTPC_HOST_POINTER_BITS=${SOFTPC_HOST_POINTER_BITS})
add_test(NAME softpc-vga-frame-smoke COMMAND softpc-vga-frame-smoke)
set_tests_properties(softpc-vga-frame-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-bop-smoke test/unit/bop_smoke.c)
target_link_libraries(softpc-bop-smoke PRIVATE softpc-machine)
target_include_directories(softpc-bop-smoke PRIVATE
    src/mvdm/softpc.new/host/inc
    src/mvdm/softpc.new/base/inc)
add_test(NAME softpc-bop-smoke COMMAND softpc-bop-smoke)
set_tests_properties(softpc-bop-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-quick-time-smoke test/unit/quick_time_smoke.c)
target_link_libraries(softpc-quick-time-smoke PRIVATE softpc-machine)
target_include_directories(softpc-quick-time-smoke PRIVATE
    src/mvdm/softpc.new/host/inc
    src/mvdm/softpc.new/base/inc)
add_test(NAME softpc-quick-time-smoke COMMAND softpc-quick-time-smoke)
set_tests_properties(softpc-quick-time-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-lifecycle-smoke test/unit/lifecycle_smoke.c)
target_link_libraries(softpc-lifecycle-smoke PRIVATE softpc-machine)
add_test(NAME softpc-lifecycle-smoke COMMAND softpc-lifecycle-smoke)
set_tests_properties(softpc-lifecycle-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-runtime-smoke
    test/unit/runtime_smoke.c
    test/support/common_machine_test.c
    src/common/session/control.c
    src/vm/driver.c
    src/app/keyboard.c
    src/common/session/presentation_plan.c
    src/common/session/reconciler.c
    src/vm/trace.c)
target_link_libraries(softpc-runtime-smoke PRIVATE softpc-machine common-machine types console host storage kvm-base kvm-window kvm-console)
target_include_directories(softpc-runtime-smoke PRIVATE test/support src/app)
add_test(NAME softpc-runtime-smoke COMMAND softpc-runtime-smoke)
set_tests_properties(softpc-runtime-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-runtime-restart-boot-smoke
    test/integration/runtime_restart_boot_smoke.c
    test/support/common_machine_test.c
    src/common/session/control.c
    src/vm/driver.c
    src/app/keyboard.c
    src/vm/trace.c)
target_link_libraries(softpc-runtime-restart-boot-smoke PRIVATE
    softpc-machine common-machine types console host storage kvm-base kvm-window kvm-console)
target_include_directories(softpc-runtime-restart-boot-smoke PRIVATE test/support src/app)
add_test(NAME softpc-runtime-restart-boot-smoke
    COMMAND softpc-runtime-restart-boot-smoke)
set_tests_properties(softpc-runtime-restart-boot-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    LABELS "integration")
add_executable(softpc-runtime-input-continuation-smoke
    test/unit/runtime_input_continuation_smoke.c
    test/support/common_machine_test.c
    src/common/session/control.c
    src/vm/driver.c
    src/app/keyboard.c
    src/vm/trace.c)
target_link_libraries(softpc-runtime-input-continuation-smoke PRIVATE
    softpc-machine common-machine types console host storage kvm-base kvm-window kvm-console)
target_include_directories(softpc-runtime-input-continuation-smoke PRIVATE
    test/support src/app)
add_test(NAME softpc-runtime-input-continuation-smoke
    COMMAND softpc-runtime-input-continuation-smoke)
set_tests_properties(softpc-runtime-input-continuation-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-command-provider-smoke test/unit/command_provider_smoke.c
    src/app/composition.c src/app/command.c src/vm/driver.c
    src/app/keyboard.c src/vm/trace.c)
target_link_libraries(softpc-command-provider-smoke PRIVATE softpc-machine common-debug common-session common-machine)
add_test(NAME softpc-command-provider-smoke COMMAND softpc-command-provider-smoke)
set_tests_properties(softpc-command-provider-smoke PROPERTIES LABELS "unit" TIMEOUT 45
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
add_executable(softpc-command-smoke test/unit/command_smoke.c
    src/app/command.c
    src/common/session/reconciler.c
    src/common/session/presentation_plan.c)
target_link_libraries(softpc-command-smoke PRIVATE common-debug common-machine)
target_include_directories(softpc-command-smoke PRIVATE src/app src/compat src)
add_test(NAME softpc-command-smoke COMMAND softpc-command-smoke)
set_tests_properties(softpc-command-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_test(NAME softpc-standalone-source-boundary
    COMMAND "${CMAKE_COMMAND}" "-DSOFTPC_SOURCE_DIR=${CMAKE_SOURCE_DIR}" -P
        "${CMAKE_SOURCE_DIR}/test/support/standalone_source_boundary.cmake")
add_test(NAME softpc-kvm-naming
    COMMAND "${CMAKE_COMMAND}" "-DKVM_NAMING_ROOT=${CMAKE_SOURCE_DIR}" -P
        "${CMAKE_SOURCE_DIR}/src/lib/verify_kvm_naming.cmake")
add_test(NAME softpc-documentation-governance
    COMMAND "${CMAKE_COMMAND}" "-DSOFTPC_SOURCE_DIR=${CMAKE_SOURCE_DIR}" -P
        "${CMAKE_SOURCE_DIR}/tools/Verify-DocumentationGovernance.cmake")
add_test(NAME softpc-documentation-governance-selftest
    COMMAND "${CMAKE_COMMAND}" -DSELF_TEST=ON
        "-DSELF_TEST_ROOT=${CMAKE_BINARY_DIR}/documentation-governance-selftest" -P
        "${CMAKE_SOURCE_DIR}/tools/Verify-DocumentationGovernance.cmake")
add_test(NAME softpc-win32-presentation-manifest
    COMMAND "${CMAKE_COMMAND}" "-DSOFTPC_SOURCE_DIR=${CMAKE_SOURCE_DIR}" -P
        "${CMAKE_SOURCE_DIR}/test/support/win32_presentation_manifest.cmake")

# Smoke tests intentionally use assert() for both the operation and its
# postcondition.  Keep those checks live even when validating an optimized
# machine library; production targets retain their normal Release NDEBUG.
set(SOFTPC_TEST_TARGETS
    softpc-machine-smoke
    softpc-dual-media-smoke
    softpc-partition-image-smoke
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
    softpc-lifecycle-smoke
    softpc-runtime-smoke
    softpc-runtime-restart-boot-smoke
    softpc-runtime-input-continuation-smoke
)
add_executable(softpc-presentation-shutdown-smoke test/unit/presentation_shutdown_smoke.c)
target_include_directories(softpc-presentation-shutdown-smoke PRIVATE src src/compat src/app)
get_target_property(shutdown_test_sources softpcvm SOURCES)
list(REMOVE_ITEM shutdown_test_sources src/app/main.c src/app/firmware.rc)
target_sources(softpc-presentation-shutdown-smoke PRIVATE ${shutdown_test_sources})
get_target_property(shutdown_test_links softpcvm LINK_LIBRARIES)
target_link_libraries(softpc-presentation-shutdown-smoke PRIVATE ${shutdown_test_links})
add_test(NAME softpc-presentation-shutdown-smoke COMMAND softpc-presentation-shutdown-smoke)
set_tests_properties(softpc-presentation-shutdown-smoke PROPERTIES LABELS "unit" TIMEOUT 10)
list(APPEND SOFTPC_TEST_TARGETS softpc-presentation-shutdown-smoke)
add_executable(softpc-runtime-cursor-smoke test/unit/runtime_cursor_smoke.c)
target_include_directories(softpc-runtime-cursor-smoke PRIVATE src src/compat src/app)
get_target_property(cursor_test_sources softpcvm SOURCES)
list(REMOVE_ITEM cursor_test_sources src/app/main.c src/vm/driver.c src/app/firmware.rc)
target_sources(softpc-runtime-cursor-smoke PRIVATE ${cursor_test_sources}
    src/vm/driver.c)
target_link_libraries(softpc-runtime-cursor-smoke PRIVATE ${shutdown_test_links})
add_test(NAME softpc-runtime-cursor-smoke COMMAND softpc-runtime-cursor-smoke)
set_tests_properties(softpc-runtime-cursor-smoke PROPERTIES LABELS "unit")
list(APPEND SOFTPC_TEST_TARGETS softpc-runtime-cursor-smoke)
list(APPEND SOFTPC_TEST_TARGETS softpc-command-smoke)
list(APPEND SOFTPC_TEST_TARGETS softpc-command-provider-smoke)
foreach(softpc_test_target IN LISTS SOFTPC_TEST_TARGETS)
    # The machine resource port reads fixed firmware from the hosting module.
    # Keep every executable test self-contained, just like the launcher.
    target_sources(${softpc_test_target} PRIVATE src/app/firmware.rc)
    if(MSVC)
        target_compile_options(${softpc_test_target} PRIVATE /UNDEBUG)
    else()
        target_compile_options(${softpc_test_target} PRIVATE -UNDEBUG)
    endif()
endforeach()

set(SOFTPC_UNIT_TESTS
    softpc-machine-smoke
    softpc-dual-media-smoke
    softpc-partition-image-smoke
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
    softpc-lifecycle-smoke
    softpc-runtime-smoke
    softpc-runtime-input-continuation-smoke
    softpc-command-smoke
    softpc-standalone-source-boundary
    softpc-documentation-governance)

# These probes write their deliberately tiny disk fixtures by relative name.
# A build-tree working directory confines them to ignored disposable output.
foreach(softpc_unit_test IN LISTS SOFTPC_UNIT_TESTS)
    set_tests_properties(${softpc_unit_test} PROPERTIES
        LABELS "unit"
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
endforeach()
set_tests_properties(softpc-documentation-governance PROPERTIES LABELS "governance")

add_executable(softpc-package-smoke test/integration/package_smoke.c)
add_dependencies(softpc-package-smoke softpcvm)
target_compile_definitions(softpc-package-smoke PRIVATE
    SOFTPC_PACKAGE_DIRECTORY="${SOFTPC_OUTPUT_DIRECTORY}"
    SOFTPC_PACKAGE_EXECUTABLE="${SOFTPC_OUTPUT_DIRECTORY}/${SOFTPC_LAUNCHER_NAME}.exe"
    SOFTPC_PACKAGE_MEDIA_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}/assets/media")
if(WIN32)
    target_link_libraries(softpc-package-smoke PRIVATE user32)
endif()
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(SOFTPC_TEST_WIDTH_LABEL x86)
else()
    set(SOFTPC_TEST_WIDTH_LABEL x64)
endif()
add_test(NAME softpc-package-smoke COMMAND softpc-package-smoke)
add_test(NAME softpc-package-compact-console COMMAND softpc-package-smoke --compact-console)
set_tests_properties(softpc-package-smoke softpc-package-compact-console PROPERTIES
    LABELS "integration;${SOFTPC_TEST_WIDTH_LABEL}"
    TIMEOUT 90
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
