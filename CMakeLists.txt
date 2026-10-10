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

add_library(softpc-machine STATIC)
target_include_directories(softpc-machine PUBLIC src src/app-softpc PRIVATE src/app-softpc/compat)
target_compile_definitions(softpc-machine PRIVATE _CRT_SECURE_NO_WARNINGS)

# This is a source-synchronised local component, not a cross-repository
# dependency. Project bindings retain all machine and lifecycle decisions;
# the imported library owns generic host, storage, and KVM mechanics.
add_subdirectory(src/lib)
enable_testing()

add_subdirectory(src/emulator)
add_subdirectory(src/product)
add_subdirectory(test/lib)
add_subdirectory(test/emulator)
add_subdirectory(test/product)
target_link_libraries(softpc-machine PRIVATE storage base audio)

# CCPU calls the driver's debug observers, so include the VM objects directly
# instead of depending on cyclic static-archive extraction order.
add_library(softpc-vm OBJECT
    src/app-softpc/machine/machine.c
    src/app-softpc/machine/driver.c
    src/app-softpc/machine/input.c
    src/app-softpc/machine/debug.c
    src/app-softpc/machine/debug_memory.c
    src/app-softpc/machine/snapshot.c
    src/app-softpc/machine/snapshot_image.c)
target_include_directories(softpc-vm PUBLIC src)
target_compile_definitions(softpc-vm PRIVATE _CRT_SECURE_NO_WARNINGS
    SOFTPC_STANDALONE=1 SOFTPC_HOST_POINTER_BITS=${SOFTPC_HOST_POINTER_BITS})
target_link_libraries(softpc-vm PUBLIC softpc-machine emulator-machine storage kvm-base)
if(WIN32)
    target_link_libraries(softpc-vm PRIVATE user32 gdi32)
endif()

# The original CCPU manifest is the executable x86 core.  It deliberately
# links no NTVDM host, DOS/WOW component or Win32 presentation provider.
# A narrow original-ROM BIOS Operation table is machine-resident and only
# registers restored hardware services.
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/src/app-softpc/softpc.new/base/ccpu386/c-files"
    SOFTPC_CCPU_SOURCE_NAMES)
list(FILTER SOFTPC_CCPU_SOURCE_NAMES EXCLUDE REGEX "^p\\.")
set(SOFTPC_CCPU_SOURCES)
foreach(SOFTPC_CCPU_SOURCE_NAME IN LISTS SOFTPC_CCPU_SOURCE_NAMES)
    list(APPEND SOFTPC_CCPU_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/app-softpc/softpc.new/base/ccpu386/${SOFTPC_CCPU_SOURCE_NAME}")
endforeach()

# Keep the historical object/archive ordering while compiling the checked-in
# sources directly.  The old generated-source path happened to append these
# translation units after c-files; static archive resolution depends on that
# order.  This is source selection only, not a source transformation.
set(SOFTPC_CCPU_DIRECT_PORT_HEAD
    src/app-softpc/softpc.new/base/ccpu386/ccpusas4.c
    src/app-softpc/softpc.new/base/ccpu386/c_main.c)
set(SOFTPC_CCPU_DIRECT_PORT_TAIL
    src/app-softpc/softpc.new/base/ccpu386/ntthread.c
    src/app-softpc/softpc.new/base/ccpu386/fpu.c
    src/app-softpc/softpc.new/base/ccpu386/zfrsrvd.c
    src/app-softpc/softpc.new/base/ccpu386/c_page.c
    src/app-softpc/softpc.new/base/ccpu386/c_reg.c
    src/app-softpc/softpc.new/base/ccpu386/c_xcptn.c
    src/app-softpc/softpc.new/base/ccpu386/popf.c)
list(REMOVE_ITEM SOFTPC_CCPU_SOURCES
    ${SOFTPC_CCPU_DIRECT_PORT_HEAD}
    ${SOFTPC_CCPU_DIRECT_PORT_TAIL})
file(GLOB SOFTPC_CVIDC_RULE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/app-softpc/softpc.new/base/cvidc/sinit*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/app-softpc/softpc.new/base/cvidc/sevid*.c")

# C-VID generated rules keep their original 32-bit rule-register layout, but
# their checked-in source now names the native-width GDP access boundary.
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set_source_files_properties(
        ${SOFTPC_CVIDC_RULE_SOURCES}
        src/app-softpc/softpc.new/base/cvidc/j_c_lang.c
        src/app-softpc/softpc.new/base/cvidc/ev_glue.c
        src/app-softpc/softpc.new/base/cvidc/vglfunc.c
        PROPERTIES COMPILE_DEFINITIONS CVIDC_RULE_WORD=1)
endif()
set_source_files_properties(src/app-softpc/softpc.new/base/bios/reset.c PROPERTIES
    COMPILE_OPTIONS "-iquote${CMAKE_CURRENT_SOURCE_DIR}/src/app-softpc/compat/bios")
add_library(softpc-ccpu-core OBJECT
    ${SOFTPC_CCPU_SOURCES}
    ${SOFTPC_CCPU_DIRECT_PORT_HEAD}
    src/app-softpc/softpc.new/base/ccpu386/ntstubs.c
    ${SOFTPC_CCPU_DIRECT_PORT_TAIL}
    src/app-softpc/compat/ccpu/facade.c
    src/app-softpc/softpc.new/base/ccpu386/vglob.c
    ${SOFTPC_CVIDC_RULE_SOURCES}
    src/app-softpc/softpc.new/base/cvidc/ev_glue.c
    src/app-softpc/softpc.new/base/cvidc/evidfunc.c
    src/app-softpc/softpc.new/base/cvidc/j_c_lang.c
    src/app-softpc/softpc.new/base/cvidc/accessfn.c
    src/app-softpc/softpc.new/base/cvidc/vglfunc.c
    src/app-softpc/softpc.new/base/cvidc/sascdef.c)
add_library(softpc-machine-compat OBJECT
    src/app-softpc/compat/snapshot_stream.c
    src/app-softpc/compat/media_snapshot.c
    src/app-softpc/compat/cvidc/gdp_state.c
    src/app-softpc/compat/ccpu/lifecycle.c
    src/app-softpc/compat/ccpu/archive.c
    src/app-softpc/compat/devices/archive.c)

# Controllers, firmware and the standalone host boundary evolve independently
# of the CCPU/C-VIDC executor.  Keep them in a separate object target so a
# recovered original device does not force recompilation of the full executor.
set_source_files_properties(src/app-softpc/softpc.new/base/system/illegalp.c PROPERTIES
    COMPILE_OPTIONS "-iquote${CMAKE_CURRENT_SOURCE_DIR}/src/app-softpc/compat/system")
set_source_files_properties(src/app-softpc/softpc.new/base/keymouse/keyba.c PROPERTIES
    COMPILE_OPTIONS "-iquote${CMAKE_CURRENT_SOURCE_DIR}/src/app-softpc/compat/keymouse")
set_source_files_properties(src/app-softpc/softpc.new/base/support/time_day.c PROPERTIES
    COMPILE_OPTIONS "-iquote${CMAKE_CURRENT_SOURCE_DIR}/src/app-softpc/softpc.new/base/inc")
add_library(softpc-machine-devices OBJECT
    src/app-softpc/softpc.new/base/support/ios.c
    src/app-softpc/softpc.new/base/system/ica.c
    src/app-softpc/softpc.new/base/system/quick_ev.c
    src/app-softpc/softpc.new/base/system/at_dma.c
    src/app-softpc/softpc.new/base/system/cmos.c
    src/app-softpc/softpc.new/base/system/dummy_nt.c
    src/app-softpc/softpc.new/base/system/idetect.c
    src/app-softpc/softpc.new/base/system/illegalp.c
    src/app-softpc/softpc.new/base/system/rom.c
    src/app-softpc/softpc.new/base/system/timer.c
    src/app-softpc/softpc.new/base/system/timestrb.c
    src/app-softpc/softpc.new/base/system/unexp_nt.c
    src/app-softpc/softpc.new/base/support/time_day.c
    src/app-softpc/softpc.new/base/bios/cmos_bis.c
    src/app-softpc/softpc.new/base/bios/build_id.c
    src/app-softpc/softpc.new/base/bios/reset.c
    src/app-softpc/softpc.new/base/bios/bootstra.c
    src/app-softpc/softpc.new/base/bios/slave_bs.c
    src/app-softpc/softpc.new/base/bios/tape_io.c
    src/app-softpc/softpc.new/base/bios/equipmnt.c
    src/app-softpc/softpc.new/base/bios/mem_size.c
    src/app-softpc/softpc.new/base/bios/rom_basc.c
    src/app-softpc/softpc.new/base/comms/printer.c
    src/app-softpc/softpc.new/base/comms/printer_.c
    src/app-softpc/softpc.new/base/comms/com.c
    src/app-softpc/softpc.new/base/comms/rs232_io.c
    src/app-softpc/softpc.new/base/keymouse/ppi.c
    src/app-softpc/softpc.new/base/keymouse/keyba.c
    src/app-softpc/softpc.new/base/keymouse/keybd_io.c
    src/app-softpc/softpc.new/base/keymouse/mouse.c
    src/app-softpc/softpc.new/base/keymouse/mouse_io.c
    src/app-softpc/softpc.new/base/disks/fla.c
    src/app-softpc/softpc.new/base/disks/gfi.c
    src/app-softpc/softpc.new/base/disks/gfi_mpty.c
    src/app-softpc/softpc.new/base/disks/floppy.c
    src/app-softpc/softpc.new/base/disks/floppy_i.c
    src/app-softpc/softpc.new/base/disks/fdisk.c
    src/app-softpc/softpc.new/base/disks/diskbios.c
    src/app-softpc/softpc.new/base/video/ega_mode.c
    src/app-softpc/softpc.new/base/video/ega_prts.c
    src/app-softpc/softpc.new/base/video/ega_vide.c
    src/app-softpc/softpc.new/base/video/gfx_updt.c
    src/app-softpc/softpc.new/base/video/video.c
    src/app-softpc/softpc.new/base/video/gvi.c
    src/app-softpc/softpc.new/base/video/video_io.c
    src/app-softpc/softpc.new/base/video/v7_ports.c
    src/app-softpc/softpc.new/base/video/v7_video.c
    src/app-softpc/softpc.new/base/video/vga_mode.c
    src/app-softpc/softpc.new/base/video/vga_prts.c
    src/app-softpc/softpc.new/base/video/vga_vide.c
    src/app-softpc/softpc.new/base/video/ega_read.c
    src/app-softpc/softpc.new/base/video/ega_writ.c
    src/app-softpc/softpc.new/base/video/ega_dmmy.c
    src/app-softpc/softpc.new/base/video/ega_trcr.c
    src/app-softpc/softpc.new/base/video/egawrtm0.c
    src/app-softpc/softpc.new/base/video/egwrtm12.c
    src/app-softpc/softpc.new/base/video/cga.c
    src/app-softpc/softpc.new/host/src/nt_ega.c
    src/app-softpc/softpc.new/host/src/nt_graph.c
    src/app-softpc/softpc.new/host/src/nt_keycd.c
    src/app-softpc/softpc.new/host/src/nt_sound.c
    src/app-softpc/softpc.new/host/src/nt_munge.c
    src/app-softpc/softpc.new/host/src/nt_cga.c
    src/app-softpc/softpc.new/host/src/nt_vga.c
    src/app-softpc/compat/edl_fast_bop.c
    src/app-softpc/compat/graphics_console_compat.c
    src/app-softpc/compat/dib_surface.c
    src/app-softpc/compat/video.c
    src/app-softpc/compat/v7_pointer.c
    src/app-softpc/compat/device_bop.c
    src/app-softpc/compat/memory.c
    src/app-softpc/compat/gfi_image.c
    src/app-softpc/compat/hdd_media.c
    src/app-softpc/compat/serial.c
    src/app-softpc/compat/parallel.c
    src/app-softpc/compat/keyboard.c
    src/app-softpc/compat/mouse_instance.c
    src/app-softpc/compat/audio.c
    src/app-softpc/compat/platform.c)
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
    src/app-softpc/compat/ccpu
    src/app-softpc/compat/cvidc
    src/app-softpc/compat
    src/app-softpc/softpc.new/host/inc
    src/app-softpc/softpc.new/base/inc
    src/app-softpc/softpc.new/base/ccpu386
    src/app-softpc/softpc.new/base/cvidc)
target_include_directories(softpc-machine-devices PRIVATE
    src
    src/app-softpc
    src/app-softpc/compat
    src/app-softpc/compat/ccpu
    src/app-softpc/compat/cvidc
    src/app-softpc/softpc.new/host/inc
    src/app-softpc/softpc.new/base/inc
    src/app-softpc/softpc.new/base/ccpu386
    src/app-softpc/softpc.new/base/cvidc)
target_include_directories(softpc-machine-compat PRIVATE
    src
    src/app-softpc/compat/ccpu
    src/app-softpc/compat/cvidc
    src/app-softpc/compat
    src/app-softpc/softpc.new/host/inc
    src/app-softpc/softpc.new/base/inc
    src/app-softpc/softpc.new/base/ccpu386
    src/app-softpc/softpc.new/base/cvidc)

# Original SAS/CCPU macros are confined to the debugger memory translation unit.
set_source_files_properties(src/app-softpc/machine/debug_memory.c PROPERTIES
    INCLUDE_DIRECTORIES "$<TARGET_PROPERTY:softpc-machine-compat,INCLUDE_DIRECTORIES>"
    COMPILE_DEFINITIONS "$<TARGET_PROPERTY:softpc-machine-compat,COMPILE_DEFINITIONS>")

# Keep the executor in the machine library so that every unresolved host edge
# is visible to the standalone build, rather than being hidden in an unused
# object target.
target_sources(softpc-machine PRIVATE $<TARGET_OBJECTS:softpc-ccpu-core>)
target_sources(softpc-machine PRIVATE $<TARGET_OBJECTS:softpc-machine-devices>)
target_sources(softpc-machine PRIVATE $<TARGET_OBJECTS:softpc-machine-compat>)
target_include_directories(softpc-machine PRIVATE
    src/app-softpc/compat/ccpu
    src/app-softpc/compat/cvidc
    src/app-softpc/compat
    src/app-softpc/softpc.new/host/inc
    src/app-softpc/softpc.new/base/inc
    src/app-softpc/softpc.new/base/ccpu386
    src/app-softpc/softpc.new/base/cvidc)
target_compile_definitions(softpc-machine PRIVATE
    SOFTPC_STANDALONE=1 SOFTPC_HOST_POINTER_BITS=${SOFTPC_HOST_POINTER_BITS})

add_executable(softpcvm
    src/app-softpc/product/main.c
    src/app-softpc/product/config.c
    src/app-softpc/product/composed_machine.c
    src/app-softpc/product/extensions.c
    src/app-softpc/product/firmware.rc)
target_link_libraries(softpcvm PRIVATE softpc-vm)
target_link_libraries(softpcvm PRIVATE product-surface types storage)

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
add_executable(softpc-machine-smoke test/app-softpc/unit/machine/machine_smoke.c)
add_executable(softpc-config-smoke test/app-softpc/unit/product/config_smoke.c src/app-softpc/product/config.c)
target_include_directories(softpc-config-smoke PRIVATE src src/app-softpc)
target_link_libraries(softpc-config-smoke PRIVATE types base)
add_test(NAME "app-softpc/unit/product/config_smoke" COMMAND softpc-config-smoke)
set_tests_properties("app-softpc/unit/product/config_smoke" PROPERTIES LABELS "unit")
add_test(NAME "checks/product_boundary_negative" COMMAND ${CMAKE_COMMAND}
    -DSOFTPC_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
    -P ${CMAKE_CURRENT_SOURCE_DIR}/tools/checks/product_boundary_negative.cmake)
set_tests_properties("checks/product_boundary_negative" PROPERTIES LABELS "checks")
target_link_libraries(softpc-machine-smoke PRIVATE softpc-vm)
add_test(NAME "app-softpc/unit/machine/machine_smoke" COMMAND softpc-machine-smoke)
add_executable(softpc-dual-media-smoke test/app-softpc/unit/machine/dual_media_smoke.c)
target_link_libraries(softpc-dual-media-smoke PRIVATE softpc-vm)
target_include_directories(softpc-dual-media-smoke PRIVATE
    src/app-softpc/softpc.new/host/inc
    src/app-softpc/softpc.new/base/inc)
add_test(NAME "app-softpc/unit/machine/dual_media_smoke" COMMAND softpc-dual-media-smoke)
add_executable(softpc-partition-image-smoke test/app-softpc/unit/machine/partition_image_smoke.c)
target_link_libraries(softpc-partition-image-smoke PRIVATE softpc-vm)
add_test(NAME "app-softpc/unit/machine/partition_image_smoke" COMMAND softpc-partition-image-smoke)
add_executable(softpc-irq-smoke test/app-softpc/unit/machine/irq_smoke.c)
add_executable(softpc-pit-smoke test/app-softpc/unit/machine/pit_smoke.c)
target_link_libraries(softpc-pit-smoke PRIVATE softpc-vm)
target_include_directories(softpc-pit-smoke PRIVATE
    src/app-softpc/softpc.new/host/inc
    src/app-softpc/softpc.new/base/inc)
add_test(NAME "app-softpc/unit/machine/pit_smoke" COMMAND softpc-pit-smoke)
target_link_libraries(softpc-irq-smoke PRIVATE softpc-vm)
target_include_directories(softpc-irq-smoke PRIVATE
    src/app-softpc/softpc.new/host/inc
    src/app-softpc/softpc.new/base/inc)
add_test(NAME "app-softpc/unit/machine/irq_smoke" COMMAND softpc-irq-smoke)
add_executable(softpc-fdc-smoke test/app-softpc/unit/machine/fdc_smoke.c)
target_link_libraries(softpc-fdc-smoke PRIVATE softpc-vm)
target_include_directories(softpc-fdc-smoke PRIVATE
    src/app-softpc/softpc.new/host/inc
    src/app-softpc/softpc.new/base/inc)
add_test(NAME "app-softpc/unit/machine/fdc_smoke" COMMAND softpc-fdc-smoke)
add_executable(softpc-printer-smoke test/app-softpc/unit/machine/printer_smoke.c)
target_link_libraries(softpc-printer-smoke PRIVATE softpc-vm)
target_include_directories(softpc-printer-smoke PRIVATE
    src/app-softpc/softpc.new/host/inc
    src/app-softpc/softpc.new/base/inc)
add_test(NAME "app-softpc/unit/machine/printer_smoke" COMMAND softpc-printer-smoke)
add_executable(softpc-parallel-failure-smoke
    test/app-softpc/unit/machine/parallel_failure_smoke.c)
target_link_libraries(softpc-parallel-failure-smoke PRIVATE softpc-vm)
target_include_directories(softpc-parallel-failure-smoke PRIVATE src src/app-softpc
    $<TARGET_PROPERTY:softpc-machine-compat,INCLUDE_DIRECTORIES>)
target_compile_definitions(softpc-parallel-failure-smoke PRIVATE
    $<TARGET_PROPERTY:softpc-machine-compat,COMPILE_DEFINITIONS>)
set_target_properties(softpc-parallel-failure-smoke PROPERTIES
    C_STANDARD 90 C_EXTENSIONS ON)
add_test(NAME "app-softpc/unit/machine/parallel_failure_smoke" COMMAND softpc-parallel-failure-smoke)
set_tests_properties("app-softpc/unit/machine/parallel_failure_smoke" PROPERTIES LABELS "unit" TIMEOUT 15)
add_executable(softpc-sound-smoke test/app-softpc/unit/machine/sound_smoke.c)
target_link_libraries(softpc-sound-smoke PRIVATE softpc-vm)
target_include_directories(softpc-sound-smoke PRIVATE
    src/app-softpc/softpc.new/host/inc
    src/app-softpc/softpc.new/base/inc)
add_test(NAME "app-softpc/unit/machine/sound_smoke" COMMAND softpc-sound-smoke)
add_executable(softpc-audio-lifecycle-smoke test/app-softpc/unit/machine/audio_lifecycle_smoke.c)
target_link_libraries(softpc-audio-lifecycle-smoke PRIVATE softpc-vm)
target_include_directories(softpc-audio-lifecycle-smoke PRIVATE src src/app-softpc/compat)
add_test(NAME "app-softpc/unit/machine/audio_lifecycle_smoke" COMMAND softpc-audio-lifecycle-smoke)
add_executable(softpc-serial-smoke test/app-softpc/unit/machine/serial_smoke.c)
target_link_libraries(softpc-serial-smoke PRIVATE softpc-vm)
target_include_directories(softpc-serial-smoke PRIVATE
    src/app-softpc/softpc.new/host/inc
    src/app-softpc/softpc.new/base/inc)
add_test(NAME "app-softpc/unit/machine/serial_smoke" COMMAND softpc-serial-smoke)
add_executable(softpc-mouse-smoke test/app-softpc/unit/machine/mouse_smoke.c)
target_link_libraries(softpc-mouse-smoke PRIVATE softpc-vm)
target_include_directories(softpc-mouse-smoke PRIVATE
    src/app-softpc/softpc.new/host/inc
    src/app-softpc/softpc.new/base/inc)
add_test(NAME "app-softpc/unit/machine/mouse_smoke" COMMAND softpc-mouse-smoke)
add_executable(softpc-keycode-smoke test/app-softpc/unit/machine/keycode_smoke.c)
target_link_libraries(softpc-keycode-smoke PRIVATE softpc-vm)
target_include_directories(softpc-keycode-smoke PRIVATE
    src/app-softpc/softpc.new/host/inc
    src/app-softpc/softpc.new/base/inc)
add_test(NAME "app-softpc/unit/machine/keycode_smoke" COMMAND softpc-keycode-smoke)
add_executable(softpc-vga-frame-smoke test/app-softpc/unit/machine/vga_frame_smoke.c)
target_link_libraries(softpc-vga-frame-smoke PRIVATE softpc-vm)
target_include_directories(softpc-vga-frame-smoke PRIVATE
    src/app-softpc/softpc.new/host/inc
    src/app-softpc/softpc.new/base/inc)
target_compile_definitions(softpc-vga-frame-smoke PRIVATE
    CPU_40_STYLE CCPU C_VID CPU_486 V7VGA PROD SOFTPC_STANDALONE=1
    SOFTPC_HOST_POINTER_BITS=${SOFTPC_HOST_POINTER_BITS})
add_test(NAME "app-softpc/unit/machine/vga_frame_smoke" COMMAND softpc-vga-frame-smoke)
add_executable(softpc-text-console-compat-smoke
    test/app-softpc/unit/machine/text_console_compat_smoke.c)
target_link_libraries(softpc-text-console-compat-smoke PRIVATE softpc-vm)
target_include_directories(softpc-text-console-compat-smoke PRIVATE src src/app-softpc)
add_test(NAME "app-softpc/unit/machine/text_console_compat_smoke"
    COMMAND softpc-text-console-compat-smoke)
add_executable(softpc-bop-smoke test/app-softpc/unit/machine/bop_smoke.c)
target_link_libraries(softpc-bop-smoke PRIVATE softpc-vm)
target_include_directories(softpc-bop-smoke PRIVATE
    src/app-softpc/softpc.new/host/inc
    src/app-softpc/softpc.new/base/inc)
add_test(NAME "app-softpc/unit/machine/bop_smoke" COMMAND softpc-bop-smoke)
add_executable(softpc-quick-time-smoke test/app-softpc/unit/machine/quick_time_smoke.c)
target_link_libraries(softpc-quick-time-smoke PRIVATE softpc-vm)
target_include_directories(softpc-quick-time-smoke PRIVATE
    src/app-softpc/softpc.new/host/inc
    src/app-softpc/softpc.new/base/inc)
add_test(NAME "app-softpc/unit/machine/quick_time_smoke" COMMAND softpc-quick-time-smoke)
add_executable(softpc-lifecycle-smoke test/app-softpc/unit/machine/lifecycle_smoke.c)
target_link_libraries(softpc-lifecycle-smoke PRIVATE softpc-vm)
add_test(NAME "app-softpc/unit/machine/lifecycle_smoke" COMMAND softpc-lifecycle-smoke)
add_executable(softpc-checkpoint-smoke test/app-softpc/unit/machine/checkpoint_smoke.c)
target_link_libraries(softpc-checkpoint-smoke PRIVATE softpc-vm)
target_include_directories(softpc-checkpoint-smoke PRIVATE
    src/app-softpc/softpc.new/host/inc
    src/app-softpc/softpc.new/base/ccpu386
    src/app-softpc/softpc.new/base/inc)
add_test(NAME "app-softpc/unit/machine/checkpoint_smoke" COMMAND softpc-checkpoint-smoke)
set_tests_properties("app-softpc/unit/machine/checkpoint_smoke" PROPERTIES TIMEOUT 15)
add_executable(softpc-snapshot-transaction-smoke
    test/app-softpc/integration/snapshot_transaction_smoke.c)
add_executable(softpc-media-snapshot-smoke test/app-softpc/unit/machine/media_snapshot_smoke.c
    src/app-softpc/compat/snapshot_stream.c)
target_include_directories(softpc-media-snapshot-smoke PRIVATE src src/app-softpc)
target_link_libraries(softpc-media-snapshot-smoke PRIVATE storage)
add_test(NAME "app-softpc/unit/machine/media_snapshot_smoke" COMMAND softpc-media-snapshot-smoke)
set_tests_properties("app-softpc/unit/machine/media_snapshot_smoke" PROPERTIES TIMEOUT 20 LABELS "unit")
target_link_libraries(softpc-snapshot-transaction-smoke PRIVATE softpc-vm
    emulator-machine)
add_test(NAME "app-softpc/integration/snapshot_transaction_smoke"
    COMMAND softpc-snapshot-transaction-smoke)
set_tests_properties("app-softpc/integration/snapshot_transaction_smoke" PROPERTIES TIMEOUT 20 LABELS "integration")
set(SOFTPC_SNAPSHOT_CROSS_PROCESS_WORK_DIRECTORY
    "${CMAKE_CURRENT_BINARY_DIR}/test-work/app-softpc/integration/snapshot_cross_process")
add_test(NAME "app-softpc/integration/snapshot_cross_process"
    COMMAND "${CMAKE_COMMAND}"
        "-DSNAPSHOT_EXECUTABLE=$<TARGET_FILE:softpc-snapshot-transaction-smoke>"
        "-DSNAPSHOT_WORKING_DIRECTORY=${SOFTPC_SNAPSHOT_CROSS_PROCESS_WORK_DIRECTORY}"
        -P "${CMAKE_SOURCE_DIR}/test/app-softpc/integration/snapshot_cross_process.cmake")
set_tests_properties("app-softpc/integration/snapshot_cross_process" PROPERTIES TIMEOUT 30 LABELS "integration")
add_executable(softpc-snapshot-boundary-smoke test/app-softpc/unit/machine/snapshot_boundary_smoke.c)
target_include_directories(softpc-snapshot-boundary-smoke PRIVATE src src/app-softpc)
add_test(NAME "app-softpc/unit/machine/snapshot_boundary_smoke" COMMAND softpc-snapshot-boundary-smoke)
set_tests_properties("app-softpc/unit/machine/snapshot_boundary_smoke" PROPERTIES TIMEOUT 15)
add_executable(softpc-runtime-smoke
    test/app-softpc/integration/runtime_smoke.c
    test/app-softpc/integration/machine_fixture.c)
target_link_libraries(softpc-runtime-smoke PRIVATE softpc-vm emulator-machine types console console-broker storage kvm-base kvm-window kvm-console product-surface-keyboard)
target_include_directories(softpc-runtime-smoke PRIVATE src/app-softpc/product)
add_test(NAME "app-softpc/integration/runtime_smoke" COMMAND softpc-runtime-smoke)
add_executable(softpc-runtime-restart-boot-smoke
    test/app-softpc/integration/runtime_restart_boot_smoke.c
    test/app-softpc/integration/machine_fixture.c)
target_link_libraries(softpc-runtime-restart-boot-smoke PRIVATE
    softpc-vm emulator-machine types console console-broker storage kvm-base kvm-window kvm-console product-surface-keyboard)
target_include_directories(softpc-runtime-restart-boot-smoke PRIVATE src/app-softpc/product)
target_compile_definitions(softpc-runtime-restart-boot-smoke PRIVATE
    SOFTPC_TEST_RESTART_BOOT_IMAGE="${CMAKE_CURRENT_SOURCE_DIR}/assets/media/win31_en_installed.img")
add_test(NAME "app-softpc/integration/runtime_restart_boot_smoke"
    COMMAND softpc-runtime-restart-boot-smoke)
set_tests_properties("app-softpc/integration/runtime_restart_boot_smoke" PROPERTIES
    LABELS "integration")
# This test drives a live guest through Windows UI transitions with bounded
# real-time waits. Reserve the requested parallel capacity instead of making
# those waits depend on unrelated CPU-heavy test processes.
set_tests_properties("app-softpc/integration/runtime_restart_boot_smoke" PROPERTIES PROCESSORS 8)
add_executable(softpc-runtime-input-continuation-smoke
    test/app-softpc/integration/runtime_input_continuation_smoke.c
    test/app-softpc/integration/machine_fixture.c)
target_link_libraries(softpc-runtime-input-continuation-smoke PRIVATE
    softpc-vm emulator-machine types console console-broker storage kvm-base kvm-window kvm-console product-surface-keyboard)
target_include_directories(softpc-runtime-input-continuation-smoke PRIVATE src/app-softpc/product)
add_test(NAME "app-softpc/integration/runtime_input_continuation_smoke"
    COMMAND softpc-runtime-input-continuation-smoke)
add_executable(softpc-command-provider-smoke test/app-softpc/integration/command_provider_smoke.c
    src/app-softpc/product/extensions.c)
target_link_libraries(softpc-command-provider-smoke PRIVATE softpc-vm product-debug emulator-session emulator-machine product-surface-command product-surface-keyboard)
add_test(NAME "app-softpc/integration/command_provider_smoke" COMMAND softpc-command-provider-smoke)
set_tests_properties("app-softpc/integration/command_provider_smoke" PROPERTIES LABELS "integration" TIMEOUT 45)
add_executable(softpc-x87-layout-smoke test/app-softpc/unit/machine/x87_layout_smoke.c)
target_include_directories(softpc-x87-layout-smoke PRIVATE
    src src/app-softpc/softpc.new/host/inc src/app-softpc/softpc.new/base/inc)
target_compile_definitions(softpc-x87-layout-smoke PRIVATE LITTLEND SOFTPC_STANDALONE)
add_test(NAME "app-softpc/unit/machine/x87_layout_smoke" COMMAND softpc-x87-layout-smoke)
set_tests_properties("app-softpc/unit/machine/x87_layout_smoke" PROPERTIES LABELS "unit")
add_test(NAME "checks/standalone_source_boundary"
    COMMAND "${CMAKE_COMMAND}" "-DSOFTPC_SOURCE_DIR=${CMAKE_SOURCE_DIR}" -P
        "${CMAKE_SOURCE_DIR}/tools/checks/standalone_source_boundary.cmake")
add_test(NAME "checks/lib_types_base_convergence"
    COMMAND "${CMAKE_COMMAND}" "-DSOFTPC_SOURCE_DIR=${CMAKE_SOURCE_DIR}" -P
        "${CMAKE_SOURCE_DIR}/tools/checks/lib_types_base_convergence.cmake")
add_test(NAME "checks/lib_types_base_convergence_negative"
    COMMAND "${CMAKE_COMMAND}" "-DSOFTPC_SOURCE_DIR=${CMAKE_SOURCE_DIR}" -P
        "${CMAKE_SOURCE_DIR}/tools/checks/lib_types_base_convergence_negative.cmake")
add_test(NAME "checks/kvm_naming"
    COMMAND "${CMAKE_COMMAND}" "-DKVM_NAMING_ROOT=${CMAKE_SOURCE_DIR}" -P
        "${CMAKE_SOURCE_DIR}/src/lib/verify_kvm_naming.cmake")
add_test(NAME "checks/documentation_governance"
    COMMAND "${CMAKE_COMMAND}" "-DSOFTPC_SOURCE_DIR=${CMAKE_SOURCE_DIR}" -P
        "${CMAKE_SOURCE_DIR}/tools/Verify-DocumentationGovernance.cmake")
add_test(NAME "checks/documentation_governance/selftest"
    COMMAND "${CMAKE_COMMAND}" -DSELF_TEST=ON
        "-DSELF_TEST_ROOT=${CMAKE_BINARY_DIR}/documentation-governance-selftest" -P
        "${CMAKE_SOURCE_DIR}/tools/Verify-DocumentationGovernance.cmake")
add_test(NAME "checks/win32_presentation_manifest"
    COMMAND "${CMAKE_COMMAND}" "-DSOFTPC_SOURCE_DIR=${CMAKE_SOURCE_DIR}" -P
        "${CMAKE_SOURCE_DIR}/tools/checks/win32_presentation_manifest.cmake")

# Smoke tests intentionally use assert() for both the operation and its
# postcondition.  Keep those checks live even when validating an optimized
# machine library; production targets retain their normal Release NDEBUG.
set(SOFTPC_TEST_TARGETS
    softpc-pit-smoke
    softpc-media-snapshot-smoke
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
    softpc-text-console-compat-smoke
    softpc-bop-smoke
    softpc-quick-time-smoke
    softpc-lifecycle-smoke
    softpc-checkpoint-smoke
    softpc-snapshot-transaction-smoke
    softpc-snapshot-boundary-smoke
    softpc-runtime-smoke
    softpc-runtime-restart-boot-smoke
    softpc-runtime-input-continuation-smoke
)
add_executable(softpc-sound-state-smoke test/app-softpc/unit/machine/sound_state_smoke.c)
target_include_directories(softpc-sound-state-smoke PRIVATE src src/app-softpc
    $<TARGET_PROPERTY:softpc-machine-devices,INCLUDE_DIRECTORIES>)
target_compile_definitions(softpc-sound-state-smoke PRIVATE
    $<TARGET_PROPERTY:softpc-machine-devices,COMPILE_DEFINITIONS>)
set_target_properties(softpc-sound-state-smoke PROPERTIES C_STANDARD 90 C_EXTENSIONS ON)
add_test(NAME "app-softpc/unit/machine/sound_state_smoke" COMMAND softpc-sound-state-smoke)
set_tests_properties("app-softpc/unit/machine/sound_state_smoke" PROPERTIES LABELS "unit" TIMEOUT 15)
list(APPEND SOFTPC_TEST_TARGETS softpc-sound-state-smoke)
add_executable(softpc-audio-first-tone-smoke test/app-softpc/unit/machine/audio_first_tone_smoke.c)
target_link_libraries(softpc-audio-first-tone-smoke PRIVATE base types)
target_include_directories(softpc-audio-first-tone-smoke PRIVATE src src/app-softpc
    $<TARGET_PROPERTY:softpc-machine-compat,INCLUDE_DIRECTORIES>)
target_compile_definitions(softpc-audio-first-tone-smoke PRIVATE
    $<TARGET_PROPERTY:softpc-machine-compat,COMPILE_DEFINITIONS>)
set_target_properties(softpc-audio-first-tone-smoke PROPERTIES C_STANDARD 90 C_EXTENSIONS ON)
add_test(NAME "app-softpc/unit/machine/audio_first_tone_smoke" COMMAND softpc-audio-first-tone-smoke)
set_tests_properties("app-softpc/unit/machine/audio_first_tone_smoke" PROPERTIES LABELS "unit" TIMEOUT 15)
list(APPEND SOFTPC_TEST_TARGETS softpc-audio-first-tone-smoke)
add_executable(softpc-audio-first-delivery-smoke test/app-softpc/unit/machine/audio_first_delivery_smoke.c)
target_link_libraries(softpc-audio-first-delivery-smoke PRIVATE base types)
target_include_directories(softpc-audio-first-delivery-smoke PRIVATE src src/app-softpc
    $<TARGET_PROPERTY:softpc-machine-compat,INCLUDE_DIRECTORIES>)
target_compile_definitions(softpc-audio-first-delivery-smoke PRIVATE
    $<TARGET_PROPERTY:softpc-machine-compat,COMPILE_DEFINITIONS>)
set_target_properties(softpc-audio-first-delivery-smoke PROPERTIES C_STANDARD 90 C_EXTENSIONS ON)
add_test(NAME "app-softpc/unit/machine/audio_first_delivery_smoke" COMMAND softpc-audio-first-delivery-smoke)
set_tests_properties("app-softpc/unit/machine/audio_first_delivery_smoke" PROPERTIES LABELS "unit" TIMEOUT 15)
list(APPEND SOFTPC_TEST_TARGETS softpc-audio-first-delivery-smoke)
foreach(probe IN ITEMS platform audio)
    set(probe_target softpc-${probe}-failure-smoke)
    add_executable(${probe_target} test/app-softpc/unit/machine/${probe}_failure_smoke.c)
    target_link_libraries(${probe_target} PRIVATE softpc-vm)
    target_include_directories(${probe_target} PRIVATE src src/app-softpc
        $<TARGET_PROPERTY:softpc-machine-compat,INCLUDE_DIRECTORIES>)
    target_compile_definitions(${probe_target} PRIVATE
        $<TARGET_PROPERTY:softpc-machine-compat,COMPILE_DEFINITIONS>)
    # The probes include the actual legacy-facing implementation.
    set_target_properties(${probe_target} PROPERTIES C_STANDARD 90 C_EXTENSIONS ON)
    add_test(NAME "app-softpc/unit/machine/${probe}_failure_smoke" COMMAND ${probe_target})
    set_tests_properties("app-softpc/unit/machine/${probe}_failure_smoke" PROPERTIES LABELS "unit" TIMEOUT 15)
    list(APPEND SOFTPC_TEST_TARGETS ${probe_target})
endforeach()
get_target_property(shutdown_test_links softpcvm LINK_LIBRARIES)
add_executable(softpc-presentation-shutdown-smoke
    test/app-softpc/integration/presentation_shutdown_smoke.c)
target_include_directories(softpc-presentation-shutdown-smoke PRIVATE src src/app-softpc
    src/app-softpc/compat src/app-softpc/product)
get_target_property(shutdown_test_sources softpcvm SOURCES)
list(REMOVE_ITEM shutdown_test_sources src/app-softpc/product/main.c
    src/app-softpc/product/firmware.rc)
target_sources(softpc-presentation-shutdown-smoke PRIVATE ${shutdown_test_sources})
target_link_libraries(softpc-presentation-shutdown-smoke PRIVATE softpc-machine
    ${shutdown_test_links})
add_test(NAME "app-softpc/integration/presentation_shutdown_smoke"
    COMMAND softpc-presentation-shutdown-smoke)
set_tests_properties("app-softpc/integration/presentation_shutdown_smoke" PROPERTIES LABELS "integration" TIMEOUT 10)
list(APPEND SOFTPC_TEST_TARGETS softpc-presentation-shutdown-smoke)
add_executable(softpc-runtime-cursor-smoke test/app-softpc/integration/runtime_cursor_smoke.c)
target_include_directories(softpc-runtime-cursor-smoke PRIVATE src src/app-softpc/compat src/app-softpc/product)
get_target_property(cursor_test_sources softpcvm SOURCES)
list(REMOVE_ITEM cursor_test_sources src/app-softpc/product/main.c src/app-softpc/product/firmware.rc)
target_sources(softpc-runtime-cursor-smoke PRIVATE ${cursor_test_sources})
target_link_libraries(softpc-runtime-cursor-smoke PRIVATE ${shutdown_test_links})
add_test(NAME "app-softpc/integration/runtime_cursor_smoke" COMMAND softpc-runtime-cursor-smoke)
set_tests_properties("app-softpc/integration/runtime_cursor_smoke" PROPERTIES LABELS "integration")
list(APPEND SOFTPC_TEST_TARGETS softpc-runtime-cursor-smoke)
list(APPEND SOFTPC_TEST_TARGETS softpc-command-provider-smoke)
list(APPEND SOFTPC_TEST_TARGETS softpc-x87-layout-smoke)
foreach(softpc_test_target IN LISTS SOFTPC_TEST_TARGETS)
    # The machine resource port reads fixed firmware from the hosting module.
    # Keep every executable test self-contained, just like the launcher.
    target_sources(${softpc_test_target} PRIVATE src/app-softpc/product/firmware.rc)
    if(MSVC)
        target_compile_options(${softpc_test_target} PRIVATE /UNDEBUG)
    else()
        target_compile_options(${softpc_test_target} PRIVATE -UNDEBUG)
    endif()
endforeach()

set(SOFTPC_UNIT_CTESTS
    "app-softpc/unit/product/config_smoke"
    "app-softpc/unit/machine/pit_smoke"
    "app-softpc/unit/machine/media_snapshot_smoke"
    "app-softpc/unit/machine/machine_smoke"
    "app-softpc/unit/machine/dual_media_smoke"
    "app-softpc/unit/machine/partition_image_smoke"
    "app-softpc/unit/machine/irq_smoke"
    "app-softpc/unit/machine/fdc_smoke"
    "app-softpc/unit/machine/printer_smoke"
    "app-softpc/unit/machine/sound_smoke"
    "app-softpc/unit/machine/sound_state_smoke"
    "app-softpc/unit/machine/audio_lifecycle_smoke"
    "app-softpc/unit/machine/audio_first_tone_smoke"
    "app-softpc/unit/machine/audio_first_delivery_smoke"
    "app-softpc/unit/machine/serial_smoke"
    "app-softpc/unit/machine/mouse_smoke"
    "app-softpc/unit/machine/keycode_smoke"
    "app-softpc/unit/machine/vga_frame_smoke"
    "app-softpc/unit/machine/text_console_compat_smoke"
    "app-softpc/unit/machine/bop_smoke"
    "app-softpc/unit/machine/quick_time_smoke"
    "app-softpc/unit/machine/lifecycle_smoke"
    "app-softpc/unit/machine/checkpoint_smoke"
    "app-softpc/unit/machine/snapshot_boundary_smoke"
    "app-softpc/unit/machine/platform_failure_smoke"
    "app-softpc/unit/machine/audio_failure_smoke"
    "app-softpc/unit/machine/parallel_failure_smoke"
    "app-softpc/unit/machine/x87_layout_smoke")
set(SOFTPC_INTEGRATION_CTESTS
    "app-softpc/integration/snapshot_transaction_smoke"
    "app-softpc/integration/snapshot_cross_process"
    "app-softpc/integration/runtime_smoke"
    "app-softpc/integration/runtime_restart_boot_smoke"
    "app-softpc/integration/runtime_input_continuation_smoke"
    "app-softpc/integration/command_provider_smoke"
    "app-softpc/integration/presentation_shutdown_smoke"
    "app-softpc/integration/runtime_cursor_smoke")
set(SOFTPC_CHECK_CTESTS
    "checks/product_boundary_negative"
    "checks/standalone_source_boundary"
    "checks/lib_types_base_convergence"
    "checks/lib_types_base_convergence_negative"
    "checks/kvm_naming"
    "checks/documentation_governance"
    "checks/documentation_governance/selftest"
    "checks/win32_presentation_manifest")
set_tests_properties(${SOFTPC_UNIT_CTESTS} PROPERTIES LABELS "unit")
set_tests_properties(${SOFTPC_INTEGRATION_CTESTS} PROPERTIES LABELS "integration")
set_tests_properties(${SOFTPC_CHECK_CTESTS} PROPERTIES LABELS "checks")

add_executable(softpc-package-smoke test/app-softpc/integration/package_smoke.c)
target_include_directories(softpc-package-smoke PRIVATE src)
target_link_libraries(softpc-package-smoke PRIVATE base)
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
add_test(NAME "app-softpc/integration/package_smoke" COMMAND softpc-package-smoke)
add_test(NAME "app-softpc/integration/package_smoke/compact_console" COMMAND softpc-package-smoke --compact-console)
set_tests_properties("app-softpc/integration/package_smoke" "app-softpc/integration/package_smoke/compact_console" PROPERTIES
    LABELS "integration;desktop;${SOFTPC_TEST_WIDTH_LABEL}"
    TIMEOUT 90
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

set(SOFTPC_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
include(tools/checks/build_ownership.cmake)
add_test(NAME "checks/build_ownership_negative" COMMAND "${CMAKE_COMMAND}"
    "-DSOFTPC_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}"
    -P "${CMAKE_CURRENT_SOURCE_DIR}/tools/checks/build_ownership.cmake")
set_tests_properties("checks/build_ownership_negative" PROPERTIES LABELS "checks")
list(APPEND SOFTPC_CHECK_CTESTS "checks/build_ownership_negative")

# Parallel background tests must not share generated media, snapshots or probe
# files. Checked-in media is supplied through an explicit read-only path, so
# every non-desktop test may use a private ignored build directory.
get_property(softpc_registered_tests DIRECTORY PROPERTY TESTS)
foreach(softpc_registered_test IN LISTS softpc_registered_tests)
    get_test_property(${softpc_registered_test} LABELS softpc_registered_labels)
    list(FIND softpc_registered_labels "desktop" softpc_desktop_label)
    if(softpc_desktop_label EQUAL -1)
        set(softpc_test_work_directory
            "${CMAKE_CURRENT_BINARY_DIR}/test-work/${softpc_registered_test}")
        file(MAKE_DIRECTORY "${softpc_test_work_directory}")
        set_tests_properties(${softpc_registered_test} PROPERTIES
            WORKING_DIRECTORY "${softpc_test_work_directory}")
    endif()
endforeach()
