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
# MSYS2's windres can otherwise infer the host architecture rather than the
# selected C toolchain.  Resources are linked into every x86 executable, so
# make the COFF target explicit.
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} --target=pe-i386")
endif()

add_library(softpc-machine
    src/host/machine.c)
target_include_directories(softpc-machine PUBLIC src/host)
target_compile_definitions(softpc-machine PRIVATE _CRT_SECURE_NO_WARNINGS)

# This is a source-synchronised local component, not a cross-repository
# dependency.  Project bindings retain all machine, lifecycle and hotkey
# decisions; the library handles copied-value Win32 presentation mechanics.
add_library(win32-presentation STATIC
    src/lib/platform/win32/input.c
    src/lib/platform/win32/geometry.c
    src/lib/platform/win32/mouse.c)
target_include_directories(win32-presentation PUBLIC
    src/lib/platform/win32)
if(WIN32)
    target_link_libraries(win32-presentation PUBLIC user32)
endif()

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
file(GLOB SOFTPC_CVIDC_GENERATED_INPUTS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/mvdm/softpc.new/base/cvidc/sinit*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/mvdm/softpc.new/base/cvidc/sevid*.c")
find_package(Python3 REQUIRED COMPONENTS Interpreter)
set(SOFTPC_CMOS_INPUT
    "${CMAKE_CURRENT_SOURCE_DIR}/src/mvdm/softpc.new/base/system/cmos.c")
set(SOFTPC_CMOS_SOURCE
    "${CMAKE_CURRENT_BINARY_DIR}/generated-device/cmos.c")
add_custom_command(
    OUTPUT "${SOFTPC_CMOS_SOURCE}"
    COMMAND "${Python3_EXECUTABLE}"
        "${CMAKE_CURRENT_SOURCE_DIR}/scripts/transform_cmos.py"
        "${SOFTPC_CMOS_INPUT}"
        "${SOFTPC_CMOS_SOURCE}"
    DEPENDS "${SOFTPC_CMOS_INPUT}"
        "${CMAKE_CURRENT_SOURCE_DIR}/scripts/transform_cmos.py"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/host/compat/cmos/port.h"
    COMMENT "Transforming CMOS media port ABI"
    VERBATIM)
set_source_files_properties("${SOFTPC_CMOS_SOURCE}" PROPERTIES GENERATED TRUE)
set(SOFTPC_TIMER_INPUT
    "${CMAKE_CURRENT_SOURCE_DIR}/src/mvdm/softpc.new/base/system/timer.c")
set(SOFTPC_TIMER_SOURCE
    "${CMAKE_CURRENT_BINARY_DIR}/generated-device/timer.c")
add_custom_command(
    OUTPUT "${SOFTPC_TIMER_SOURCE}"
    COMMAND "${Python3_EXECUTABLE}"
        "${CMAKE_CURRENT_SOURCE_DIR}/scripts/transform_timer.py"
        "${SOFTPC_TIMER_INPUT}"
        "${SOFTPC_TIMER_SOURCE}"
    DEPENDS "${SOFTPC_TIMER_INPUT}"
        "${CMAKE_CURRENT_SOURCE_DIR}/scripts/transform_timer.py"
    COMMENT "Transforming timer quick-event port ABI"
    VERBATIM)
set_source_files_properties("${SOFTPC_TIMER_SOURCE}" PROPERTIES GENERATED TRUE)
set(SOFTPC_RESET_INPUT
    "${CMAKE_CURRENT_SOURCE_DIR}/src/mvdm/softpc.new/base/bios/reset.c")
set(SOFTPC_RESET_SOURCE
    "${CMAKE_CURRENT_BINARY_DIR}/generated-device/reset.c")
add_custom_command(
    OUTPUT "${SOFTPC_RESET_SOURCE}"
    COMMAND "${Python3_EXECUTABLE}"
        "${CMAKE_CURRENT_SOURCE_DIR}/scripts/transform_reset.py"
        "${SOFTPC_RESET_INPUT}"
        "${SOFTPC_RESET_SOURCE}"
    DEPENDS "${SOFTPC_RESET_INPUT}"
        "${CMAKE_CURRENT_SOURCE_DIR}/scripts/transform_reset.py"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/host/compat/bios/host_def.h"
    COMMENT "Transforming BIOS reset host ABI"
    VERBATIM)
set_source_files_properties("${SOFTPC_RESET_SOURCE}" PROPERTIES
    GENERATED TRUE
    COMPILE_OPTIONS "-iquote${CMAKE_CURRENT_SOURCE_DIR}/src/host/compat/bios")
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
set(SOFTPC_CVIDC_EVENT_GLUE_INPUT
    "${CMAKE_CURRENT_SOURCE_DIR}/src/mvdm/softpc.new/base/cvidc/ev_glue.c")
set(SOFTPC_CVIDC_EVENT_GLUE_SOURCE
    "${CMAKE_CURRENT_BINARY_DIR}/generated-cvid/ev_glue.c")
add_custom_command(
    OUTPUT "${SOFTPC_CVIDC_EVENT_GLUE_SOURCE}"
    COMMAND "${Python3_EXECUTABLE}"
        "${CMAKE_CURRENT_SOURCE_DIR}/scripts/transform_cvid_gdp.py"
        --event-glue "${SOFTPC_CVIDC_EVENT_GLUE_INPUT}"
        "${SOFTPC_CVIDC_EVENT_GLUE_SOURCE}"
    DEPENDS "${SOFTPC_CVIDC_EVENT_GLUE_INPUT}"
        "${CMAKE_CURRENT_SOURCE_DIR}/scripts/transform_cvid_gdp.py"
    COMMENT "Transforming C-VID event glue port ABI"
    VERBATIM)
set(SOFTPC_CCPU_CPU4GEN_INPUT
    "${CMAKE_CURRENT_SOURCE_DIR}/src/mvdm/softpc.new/base/ccpu386/cpu4gen.h")
set(SOFTPC_CCPU_CPU4GEN_SOURCE
    "${CMAKE_CURRENT_BINARY_DIR}/generated-ccpu/cpu4gen.h")
add_custom_command(
    OUTPUT "${SOFTPC_CCPU_CPU4GEN_SOURCE}"
    COMMAND "${Python3_EXECUTABLE}"
        "${CMAKE_CURRENT_SOURCE_DIR}/scripts/transform_cvid_gdp.py"
        --ccpu-cpu4gen "${SOFTPC_CCPU_CPU4GEN_INPUT}"
        "${SOFTPC_CCPU_CPU4GEN_SOURCE}"
    DEPENDS "${SOFTPC_CCPU_CPU4GEN_INPUT}"
        "${CMAKE_CURRENT_SOURCE_DIR}/scripts/transform_cvid_gdp.py"
    COMMENT "Transforming CCPU GDP header port ABI"
    VERBATIM)
set(SOFTPC_CCPU_VGLOB_INPUT
    "${CMAKE_CURRENT_SOURCE_DIR}/src/mvdm/softpc.new/base/ccpu386/vglob.c")
set(SOFTPC_CCPU_VGLOB_SOURCE
    "${CMAKE_CURRENT_BINARY_DIR}/generated-ccpu/vglob.c")
add_custom_command(
    OUTPUT "${SOFTPC_CCPU_VGLOB_SOURCE}"
    COMMAND "${Python3_EXECUTABLE}"
        "${CMAKE_CURRENT_SOURCE_DIR}/scripts/transform_cvid_gdp.py"
        --ccpu-vglob "${SOFTPC_CCPU_VGLOB_INPUT}"
        "${SOFTPC_CCPU_VGLOB_SOURCE}"
    DEPENDS "${SOFTPC_CCPU_VGLOB_INPUT}"
        "${CMAKE_CURRENT_SOURCE_DIR}/scripts/transform_cvid_gdp.py"
    COMMENT "Transforming CCPU VGA global port ABI"
    VERBATIM)
set(SOFTPC_CCPU_SAS_HEADER_INPUT
    "${CMAKE_CURRENT_SOURCE_DIR}/src/mvdm/softpc.new/base/inc/sas.h")
set(SOFTPC_CCPU_SAS_HEADER_SOURCE
    "${CMAKE_CURRENT_BINARY_DIR}/generated-ccpu/sas.h")
set(SOFTPC_CCPU_BSIC_HEADER_INPUT
    "${CMAKE_CURRENT_SOURCE_DIR}/src/mvdm/softpc.new/base/ccpu386/c_bsic.h")
set(SOFTPC_CCPU_BSIC_HEADER_SOURCE
    "${CMAKE_CURRENT_BINARY_DIR}/generated-ccpu/c_bsic.h")
set(SOFTPC_CCPU_SAS_INPUT
    "${CMAKE_CURRENT_SOURCE_DIR}/src/mvdm/softpc.new/base/ccpu386/ccpusas4.c")
set(SOFTPC_CCPU_SAS_SOURCE
    "${CMAKE_CURRENT_BINARY_DIR}/generated-ccpu/ccpusas4.c")
set(SOFTPC_CCPU_MAIN_INPUT
    "${CMAKE_CURRENT_SOURCE_DIR}/src/mvdm/softpc.new/base/ccpu386/c_main.c")
set(SOFTPC_CCPU_MAIN_SOURCE
    "${CMAKE_CURRENT_BINARY_DIR}/generated-ccpu/c_main.c")
set(SOFTPC_CCPU_NTSTUBS_INPUT
    "${CMAKE_CURRENT_SOURCE_DIR}/src/mvdm/softpc.new/base/ccpu386/ntstubs.c")
set(SOFTPC_CCPU_NTSTUBS_SOURCE
    "${CMAKE_CURRENT_BINARY_DIR}/generated-ccpu/ntstubs.c")
set(SOFTPC_CCPU_NTTHREAD_INPUT
    "${CMAKE_CURRENT_SOURCE_DIR}/src/mvdm/softpc.new/base/ccpu386/ntthread.c")
set(SOFTPC_CCPU_NTTHREAD_SOURCE
    "${CMAKE_CURRENT_BINARY_DIR}/generated-ccpu/ntthread.c")
set(SOFTPC_CCPU_FPU_INPUT
    "${CMAKE_CURRENT_SOURCE_DIR}/src/mvdm/softpc.new/base/ccpu386/fpu.c")
set(SOFTPC_CCPU_FPU_SOURCE
    "${CMAKE_CURRENT_BINARY_DIR}/generated-ccpu/fpu.c")
set(SOFTPC_CCPU_ZFRSRVD_INPUT
    "${CMAKE_CURRENT_SOURCE_DIR}/src/mvdm/softpc.new/base/ccpu386/zfrsrvd.c")
set(SOFTPC_CCPU_ZFRSRVD_SOURCE
    "${CMAKE_CURRENT_BINARY_DIR}/generated-ccpu/zfrsrvd.c")
set(SOFTPC_CCPU_PAGE_INPUT "${CMAKE_CURRENT_SOURCE_DIR}/src/mvdm/softpc.new/base/ccpu386/c_page.c")
set(SOFTPC_CCPU_PAGE_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/generated-ccpu/c_page.c")
set(SOFTPC_CCPU_REG_INPUT "${CMAKE_CURRENT_SOURCE_DIR}/src/mvdm/softpc.new/base/ccpu386/c_reg.c")
set(SOFTPC_CCPU_REG_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/generated-ccpu/c_reg.c")
set(SOFTPC_CCPU_XCPTN_INPUT "${CMAKE_CURRENT_SOURCE_DIR}/src/mvdm/softpc.new/base/ccpu386/c_xcptn.c")
set(SOFTPC_CCPU_XCPTN_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/generated-ccpu/c_xcptn.c")
set(SOFTPC_CCPU_POPF_INPUT "${CMAKE_CURRENT_SOURCE_DIR}/src/mvdm/softpc.new/base/ccpu386/popf.c")
set(SOFTPC_CCPU_POPF_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/generated-ccpu/popf.c")
foreach(SOFTPC_CCPU_ABI_NAME IN ITEMS sas-header bsic-header sas-source main ntstubs ntthread fpu zfrsrvd page reg xcptn popf)
    if(SOFTPC_CCPU_ABI_NAME STREQUAL "sas-header")
        set(SOFTPC_CCPU_ABI_INPUT "${SOFTPC_CCPU_SAS_HEADER_INPUT}")
        set(SOFTPC_CCPU_ABI_OUTPUT "${SOFTPC_CCPU_SAS_HEADER_SOURCE}")
    elseif(SOFTPC_CCPU_ABI_NAME STREQUAL "bsic-header")
        set(SOFTPC_CCPU_ABI_INPUT "${SOFTPC_CCPU_BSIC_HEADER_INPUT}")
        set(SOFTPC_CCPU_ABI_OUTPUT "${SOFTPC_CCPU_BSIC_HEADER_SOURCE}")
    elseif(SOFTPC_CCPU_ABI_NAME STREQUAL "main")
        set(SOFTPC_CCPU_ABI_INPUT "${SOFTPC_CCPU_MAIN_INPUT}")
        set(SOFTPC_CCPU_ABI_OUTPUT "${SOFTPC_CCPU_MAIN_SOURCE}")
    elseif(SOFTPC_CCPU_ABI_NAME STREQUAL "ntstubs")
        set(SOFTPC_CCPU_ABI_INPUT "${SOFTPC_CCPU_NTSTUBS_INPUT}")
        set(SOFTPC_CCPU_ABI_OUTPUT "${SOFTPC_CCPU_NTSTUBS_SOURCE}")
    elseif(SOFTPC_CCPU_ABI_NAME STREQUAL "ntthread")
        set(SOFTPC_CCPU_ABI_INPUT "${SOFTPC_CCPU_NTTHREAD_INPUT}")
        set(SOFTPC_CCPU_ABI_OUTPUT "${SOFTPC_CCPU_NTTHREAD_SOURCE}")
    elseif(SOFTPC_CCPU_ABI_NAME STREQUAL "fpu")
        set(SOFTPC_CCPU_ABI_INPUT "${SOFTPC_CCPU_FPU_INPUT}")
        set(SOFTPC_CCPU_ABI_OUTPUT "${SOFTPC_CCPU_FPU_SOURCE}")
    elseif(SOFTPC_CCPU_ABI_NAME STREQUAL "zfrsrvd")
        set(SOFTPC_CCPU_ABI_INPUT "${SOFTPC_CCPU_ZFRSRVD_INPUT}")
        set(SOFTPC_CCPU_ABI_OUTPUT "${SOFTPC_CCPU_ZFRSRVD_SOURCE}")
    elseif(SOFTPC_CCPU_ABI_NAME STREQUAL "page")
        set(SOFTPC_CCPU_ABI_INPUT "${SOFTPC_CCPU_PAGE_INPUT}")
        set(SOFTPC_CCPU_ABI_OUTPUT "${SOFTPC_CCPU_PAGE_SOURCE}")
    elseif(SOFTPC_CCPU_ABI_NAME STREQUAL "reg")
        set(SOFTPC_CCPU_ABI_INPUT "${SOFTPC_CCPU_REG_INPUT}")
        set(SOFTPC_CCPU_ABI_OUTPUT "${SOFTPC_CCPU_REG_SOURCE}")
    elseif(SOFTPC_CCPU_ABI_NAME STREQUAL "xcptn")
        set(SOFTPC_CCPU_ABI_INPUT "${SOFTPC_CCPU_XCPTN_INPUT}")
        set(SOFTPC_CCPU_ABI_OUTPUT "${SOFTPC_CCPU_XCPTN_SOURCE}")
    elseif(SOFTPC_CCPU_ABI_NAME STREQUAL "popf")
        set(SOFTPC_CCPU_ABI_INPUT "${SOFTPC_CCPU_POPF_INPUT}")
        set(SOFTPC_CCPU_ABI_OUTPUT "${SOFTPC_CCPU_POPF_SOURCE}")
    else()
        set(SOFTPC_CCPU_ABI_INPUT "${SOFTPC_CCPU_SAS_INPUT}")
        set(SOFTPC_CCPU_ABI_OUTPUT "${SOFTPC_CCPU_SAS_SOURCE}")
    endif()
    add_custom_command(
        OUTPUT "${SOFTPC_CCPU_ABI_OUTPUT}"
        COMMAND "${Python3_EXECUTABLE}"
            "${CMAKE_CURRENT_SOURCE_DIR}/scripts/transform_cvid_gdp.py"
            "--ccpu-${SOFTPC_CCPU_ABI_NAME}" "${SOFTPC_CCPU_ABI_INPUT}"
            "${SOFTPC_CCPU_ABI_OUTPUT}"
        DEPENDS "${SOFTPC_CCPU_ABI_INPUT}"
            "${CMAKE_CURRENT_SOURCE_DIR}/scripts/transform_cvid_gdp.py"
        COMMENT "Transforming CCPU ${SOFTPC_CCPU_ABI_NAME} port ABI"
        VERBATIM)
endforeach()
add_custom_target(softpc-ccpu-port-abi-inputs
    DEPENDS "${SOFTPC_CCPU_CPU4GEN_SOURCE}"
            "${SOFTPC_CCPU_VGLOB_SOURCE}"
            "${SOFTPC_CCPU_SAS_HEADER_SOURCE}"
            "${SOFTPC_CCPU_BSIC_HEADER_SOURCE}"
            "${SOFTPC_CCPU_SAS_SOURCE}"
            "${SOFTPC_CCPU_MAIN_SOURCE}"
            "${SOFTPC_CCPU_NTSTUBS_SOURCE}"
            "${SOFTPC_CCPU_NTTHREAD_SOURCE}"
            "${SOFTPC_CCPU_FPU_SOURCE}"
            "${SOFTPC_CCPU_ZFRSRVD_SOURCE}"
            "${SOFTPC_CCPU_PAGE_SOURCE}"
            "${SOFTPC_CCPU_REG_SOURCE}"
            "${SOFTPC_CCPU_XCPTN_SOURCE}"
            "${SOFTPC_CCPU_POPF_SOURCE}")
set_source_files_properties("${SOFTPC_CCPU_SAS_SOURCE}" "${SOFTPC_CCPU_MAIN_SOURCE}" "${SOFTPC_CCPU_NTSTUBS_SOURCE}" "${SOFTPC_CCPU_NTTHREAD_SOURCE}" "${SOFTPC_CCPU_FPU_SOURCE}" "${SOFTPC_CCPU_ZFRSRVD_SOURCE}" "${SOFTPC_CCPU_PAGE_SOURCE}" "${SOFTPC_CCPU_REG_SOURCE}" "${SOFTPC_CCPU_XCPTN_SOURCE}" "${SOFTPC_CCPU_POPF_SOURCE}" PROPERTIES GENERATED TRUE)
list(REMOVE_ITEM SOFTPC_CCPU_SOURCES "${SOFTPC_CCPU_SAS_INPUT}")
list(APPEND SOFTPC_CCPU_SOURCES "${SOFTPC_CCPU_SAS_SOURCE}")
list(REMOVE_ITEM SOFTPC_CCPU_SOURCES "${SOFTPC_CCPU_MAIN_INPUT}")
list(APPEND SOFTPC_CCPU_SOURCES "${SOFTPC_CCPU_MAIN_SOURCE}")
list(REMOVE_ITEM SOFTPC_CCPU_SOURCES "${SOFTPC_CCPU_NTSTUBS_INPUT}")
list(APPEND SOFTPC_CCPU_SOURCES "${SOFTPC_CCPU_NTSTUBS_SOURCE}")
list(REMOVE_ITEM SOFTPC_CCPU_SOURCES "${SOFTPC_CCPU_NTTHREAD_INPUT}")
list(APPEND SOFTPC_CCPU_SOURCES "${SOFTPC_CCPU_NTTHREAD_SOURCE}")
list(REMOVE_ITEM SOFTPC_CCPU_SOURCES "${SOFTPC_CCPU_FPU_INPUT}")
list(APPEND SOFTPC_CCPU_SOURCES "${SOFTPC_CCPU_FPU_SOURCE}")
list(REMOVE_ITEM SOFTPC_CCPU_SOURCES "${SOFTPC_CCPU_ZFRSRVD_INPUT}")
list(APPEND SOFTPC_CCPU_SOURCES "${SOFTPC_CCPU_ZFRSRVD_SOURCE}")
list(REMOVE_ITEM SOFTPC_CCPU_SOURCES "${SOFTPC_CCPU_PAGE_INPUT}" "${SOFTPC_CCPU_REG_INPUT}")
list(APPEND SOFTPC_CCPU_SOURCES "${SOFTPC_CCPU_PAGE_SOURCE}" "${SOFTPC_CCPU_REG_SOURCE}")
list(REMOVE_ITEM SOFTPC_CCPU_SOURCES "${SOFTPC_CCPU_XCPTN_INPUT}")
list(APPEND SOFTPC_CCPU_SOURCES "${SOFTPC_CCPU_XCPTN_SOURCE}")
list(REMOVE_ITEM SOFTPC_CCPU_SOURCES "${SOFTPC_CCPU_POPF_INPUT}")
list(APPEND SOFTPC_CCPU_SOURCES "${SOFTPC_CCPU_POPF_SOURCE}")
# ``j_c_lang`` is the generated-rule register file.  It carries private host
# pointers (not guest registers), so x64 needs a pointer-width IUH only at
# this generated-rule ABI boundary.  On an x86 host, the original generated
# signatures already have the correct native widths; forcing the x64 rule
# word there corrupts their pointer and void function-table declarations.
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set_source_files_properties(
        ${SOFTPC_CVIDC_TRANSFORMED_SOURCES}
        src/mvdm/softpc.new/base/cvidc/j_c_lang.c
        "${SOFTPC_CVIDC_EVENT_GLUE_SOURCE}"
        src/mvdm/softpc.new/base/cvidc/vglfunc.c
        PROPERTIES COMPILE_DEFINITIONS CVIDC_RULE_WORD=1)
endif()
add_library(softpc-ccpu-core OBJECT
    ${SOFTPC_CCPU_SOURCES}
    "${SOFTPC_CCPU_NTSTUBS_SOURCE}"
    src/host/compat/ccpu/facade.c
    "${SOFTPC_CCPU_VGLOB_SOURCE}"
    ${SOFTPC_CVIDC_TRANSFORMED_SOURCES}
    "${SOFTPC_CVIDC_EVENT_GLUE_SOURCE}"
    src/mvdm/softpc.new/base/cvidc/evidfunc.c
    src/mvdm/softpc.new/base/cvidc/j_c_lang.c
    src/mvdm/softpc.new/base/cvidc/accessfn.c
    src/mvdm/softpc.new/base/cvidc/vglfunc.c
    src/mvdm/softpc.new/base/cvidc/sascdef.c)
add_library(softpc-machine-compat OBJECT
    src/host/compat/cvidc/gdp_state.c
    src/host/compat/ccpu/lifecycle.c)
add_dependencies(softpc-ccpu-core softpc-ccpu-port-abi-inputs)

# Controllers, firmware and the standalone host boundary evolve independently
# of the CCPU/C-VIDC executor.  Keep them in a separate object target so a
# recovered original device does not force recompilation of the full executor.
set_source_files_properties(src/mvdm/softpc.new/base/system/illegalp.c PROPERTIES
    COMPILE_OPTIONS "-iquote${CMAKE_CURRENT_SOURCE_DIR}/src/host/compat/system")
set_source_files_properties(src/mvdm/softpc.new/base/keymouse/keyba.c PROPERTIES
    COMPILE_OPTIONS "-iquote${CMAKE_CURRENT_SOURCE_DIR}/src/host/compat/keymouse")
set_source_files_properties(src/mvdm/softpc.new/base/support/time_day.c PROPERTIES
    COMPILE_OPTIONS "-iquote${CMAKE_CURRENT_SOURCE_DIR}/src/mvdm/softpc.new/base/inc")
add_library(softpc-machine-devices OBJECT
    src/mvdm/softpc.new/base/support/ios.c
    src/mvdm/softpc.new/base/system/ica.c
    src/mvdm/softpc.new/base/system/quick_ev.c
    src/mvdm/softpc.new/base/system/at_dma.c
    "${SOFTPC_CMOS_SOURCE}"
    src/mvdm/softpc.new/base/system/dummy_nt.c
    src/mvdm/softpc.new/base/system/idetect.c
    src/mvdm/softpc.new/base/system/illegalp.c
    src/mvdm/softpc.new/base/system/rom.c
    "${SOFTPC_TIMER_SOURCE}"
    src/mvdm/softpc.new/base/system/timestrb.c
    src/mvdm/softpc.new/base/system/unexp_nt.c
    src/mvdm/softpc.new/base/support/time_day.c
    src/mvdm/softpc.new/base/bios/cmos_bis.c
    src/mvdm/softpc.new/base/bios/build_id.c
    "${SOFTPC_RESET_SOURCE}"
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
    src/host/compat/edl_fast_bop.c
    src/host/compat/graphics_console_compat.c
    src/host/dib_surface.c
    src/host/video.c
    src/host/v7_pointer.c
    src/host/device_bop.c
    src/host/memory.c
    src/host/gfi_image.c
    src/host/hdd_media.c
    src/host/serial.c
    src/host/parallel.c
    src/host/keyboard.c
    src/host/mouse_instance.c
    src/host/audio.c
    src/host/platform.c)
add_dependencies(softpc-machine-devices softpc-ccpu-port-abi-inputs)
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
    "${CMAKE_CURRENT_BINARY_DIR}/generated-ccpu"
    src/host/compat/ccpu
    src/host/compat/cvidc
    src/host/compat/ccpu/legacy
    src/host/compat
    src/host
    src/mvdm/softpc.new/host/inc
    src/mvdm/softpc.new/base/inc
    src/mvdm/softpc.new/base/ccpu386
    src/mvdm/softpc.new/base/cvidc)
target_include_directories(softpc-machine-devices PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}/generated-ccpu"
    src/host/compat
    src/host/compat/ccpu
    src/host/compat/cvidc
    src/host/compat/ccpu/legacy
    src/host/compat
    src/host
    src/mvdm/softpc.new/host/inc
    src/mvdm/softpc.new/base/inc
    src/mvdm/softpc.new/base/ccpu386
    src/mvdm/softpc.new/base/cvidc)
target_include_directories(softpc-machine-compat PRIVATE
    src/host/compat/ccpu
    src/host/compat/cvidc
    src/host/compat/ccpu/legacy
    src/host
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
    src/host/compat/ccpu
    src/host/compat/cvidc
    src/host/compat/ccpu/legacy
    src/host
    src/mvdm/softpc.new/host/inc
    src/mvdm/softpc.new/base/inc
    src/mvdm/softpc.new/base/ccpu386
    src/mvdm/softpc.new/base/cvidc)
target_compile_definitions(softpc-machine PRIVATE
    SOFTPC_STANDALONE=1 SOFTPC_HOST_POINTER_BITS=${SOFTPC_HOST_POINTER_BITS})

add_executable(softpcvm
    src/app/main.c
    src/app/console.c
    src/app/runtime.c
    src/app/keyboard.c
    src/app/prompt_trace.c
    src/app/window.c
    src/app/firmware.rc)
target_link_libraries(softpcvm PRIVATE softpc-machine)
target_link_libraries(softpcvm PRIVATE win32-presentation)
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
    src/app/runtime.c
    src/app/keyboard.c
    src/app/prompt_trace.c)
target_link_libraries(softpc-runtime-smoke PRIVATE softpc-machine)
target_include_directories(softpc-runtime-smoke PRIVATE
    src/app src/lib/platform/win32)
add_test(NAME softpc-runtime-smoke COMMAND softpc-runtime-smoke)
set_tests_properties(softpc-runtime-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-runtime-input-continuation-smoke
    test/unit/runtime_input_continuation_smoke.c
    src/app/runtime.c
    src/app/keyboard.c
    src/app/prompt_trace.c)
target_link_libraries(softpc-runtime-input-continuation-smoke PRIVATE
    softpc-machine)
target_include_directories(softpc-runtime-input-continuation-smoke PRIVATE
    src/app src/lib/platform/win32 test/support)
add_test(NAME softpc-runtime-input-continuation-smoke
    COMMAND softpc-runtime-input-continuation-smoke)
set_tests_properties(softpc-runtime-input-continuation-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-win32-window-smoke
    test/unit/win32_window_smoke.c
    src/app/runtime.c
    src/app/keyboard.c
    src/app/prompt_trace.c
    src/app/window.c)
target_link_libraries(softpc-win32-window-smoke PRIVATE softpc-machine)
target_link_libraries(softpc-win32-window-smoke PRIVATE win32-presentation)
target_compile_definitions(softpc-win32-window-smoke PRIVATE
    SOFTPC_WINDOW_TESTING=1)
if(WIN32)
    target_link_libraries(softpc-win32-window-smoke PRIVATE user32 gdi32)
endif()
target_include_directories(softpc-win32-window-smoke PRIVATE src/app)
add_test(NAME softpc-win32-window-smoke COMMAND softpc-win32-window-smoke)
set_tests_properties(softpc-win32-window-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-win32-keyboard-smoke
    test/unit/win32_keyboard_smoke.c)
target_link_libraries(softpc-win32-keyboard-smoke PRIVATE
    softpc-machine win32-presentation)
add_test(NAME softpc-win32-keyboard-smoke COMMAND softpc-win32-keyboard-smoke)
set_tests_properties(softpc-win32-keyboard-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(softpc-win32-presentation-smoke
    test/unit/win32_presentation_smoke.c)
target_link_libraries(softpc-win32-presentation-smoke PRIVATE
    win32-presentation)
add_test(NAME softpc-win32-presentation-smoke
    COMMAND softpc-win32-presentation-smoke)
set_tests_properties(softpc-win32-presentation-smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_test(NAME softpc-standalone-source-boundary
    COMMAND "${CMAKE_COMMAND}" "-DSOFTPC_SOURCE_DIR=${CMAKE_SOURCE_DIR}" -P
        "${CMAKE_SOURCE_DIR}/test/support/standalone_source_boundary.cmake")
add_test(NAME softpc-documentation-governance
    COMMAND "${CMAKE_COMMAND}" "-DSOFTPC_SOURCE_DIR=${CMAKE_SOURCE_DIR}" -P
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
    softpc-runtime-input-continuation-smoke
    softpc-win32-window-smoke
    softpc-win32-keyboard-smoke
    softpc-win32-presentation-smoke)
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
    softpc-win32-window-smoke
    softpc-win32-keyboard-smoke
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
set_tests_properties(softpc-package-smoke PROPERTIES
    LABELS "integration;${SOFTPC_TEST_WIDTH_LABEL}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
