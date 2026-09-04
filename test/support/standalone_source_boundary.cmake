set(standalone_sources
    "${SOFTPC_SOURCE_DIR}/src/core/softpc-port-abi/ccpu/softpc_ccpu_facade.c"
    "${SOFTPC_SOURCE_DIR}/src/core/softpc-port-abi/cvidc/softpc_gdp_state.c"
    "${SOFTPC_SOURCE_DIR}/src/core/softpc-port-abi/cvidc/softpc_gdp_state.h"
    "${SOFTPC_SOURCE_DIR}/src/core/softpc-port-abi/cvidc/softpc_gdp_slots.h"
    "${SOFTPC_SOURCE_DIR}/src/mvdm/softpc.new/base/cvidc/sascdef.c"
    "${SOFTPC_SOURCE_DIR}/src/mvdm/softpc.new/base/support/ios.c"
    "${SOFTPC_SOURCE_DIR}/src/mvdm/softpc.new/base/disks/fdisk.c"
    "${SOFTPC_SOURCE_DIR}/src/host/media/softpc_gfi_image.c"
    "${SOFTPC_SOURCE_DIR}/src/host/media/softpc_hdd_media.c"
    "${SOFTPC_SOURCE_DIR}/src/host/video/softpc_platform_video.c"
    "${SOFTPC_SOURCE_DIR}/src/host/video/softpc_v7_pointer.c"
    "${SOFTPC_SOURCE_DIR}/src/mvdm/softpc.new/base/keymouse/keybd_io.c"
    "${SOFTPC_SOURCE_DIR}/src/mvdm/softpc.new/base/system/idetect.c"
    "${SOFTPC_SOURCE_DIR}/src/host/machine/softpc_device_bop.c"
    "${SOFTPC_SOURCE_DIR}/src/host/platform/softpc_standalone_platform.c"
    "${SOFTPC_SOURCE_DIR}/src/host/machine/softpc_machine.c")

if(EXISTS "${SOFTPC_SOURCE_DIR}/src/host/softpc_compat")
    message(FATAL_ERROR "Standalone host retains the obsolete softpc_compat taxonomy")
endif()

if(EXISTS "${SOFTPC_SOURCE_DIR}/src/core/softpc_physical_mapping.c" OR
   EXISTS "${SOFTPC_SOURCE_DIR}/src/core/softpc_physical_mapping.h")
    message(FATAL_ERROR "Standalone SoftPC retains the obsolete physical-mapping shim")
endif()

file(STRINGS "${SOFTPC_SOURCE_DIR}/src/mvdm/softpc.new/base/ccpu386/c-files"
    ccpu_source_names)
foreach(name IN LISTS ccpu_source_names)
    if(NOT name MATCHES "^p\\.")
        list(APPEND standalone_sources
            "${SOFTPC_SOURCE_DIR}/src/mvdm/softpc.new/base/ccpu386/${name}")
    endif()
endforeach()

# Original controller sources retain inactive historic host branches so their
# provenance and machine implementation remain intact.  The standalone target
# must never activate those branches.
file(READ "${SOFTPC_SOURCE_DIR}/CMakeLists.txt" build_definition)
string(TOLOWER "${build_definition}" normalized_build_definition)
if(normalized_build_definition MATCHES "target_compile_definitions\\([^\\)]*ntvdm")
    message(FATAL_ERROR "Standalone SoftPC enables an NTVDM compile definition")
endif()

# cmosnt.c is the Windows NT RTC host variant.  The standalone machine uses
# the original cross-host controller in cmos.c and must not link that variant.
if(normalized_build_definition MATCHES "cmosnt\\.c")
    message(FATAL_ERROR "Standalone SoftPC activates the NT-specific CMOS controller")
endif()

# The fixed VM must retain the original machine controllers.  Standalone
# sources are permitted only at their host/media/presentation boundaries;
# dropping one of these source files is a regression toward a replacement
# controller even if the new implementation happens to build.
set(required_original_controller_sources
    "src/mvdm/softpc.new/base/support/ios.c"
    "src/mvdm/softpc.new/base/system/ica.c"
    "src/mvdm/softpc.new/base/system/quick_ev.c"
    "src/mvdm/softpc.new/base/system/at_dma.c"
    "src/mvdm/softpc.new/base/system/cmos.c"
    "src/mvdm/softpc.new/base/system/dummy_nt.c"
    "src/mvdm/softpc.new/base/system/idetect.c"
    "src/mvdm/softpc.new/base/system/illegalp.c"
    "src/mvdm/softpc.new/base/system/rom.c"
    "src/mvdm/softpc.new/base/system/timer.c"
    "src/mvdm/softpc.new/base/system/timestrb.c"
    "src/mvdm/softpc.new/base/system/unexp_nt.c"
    "src/mvdm/softpc.new/base/support/time_day.c"
    "src/mvdm/softpc.new/base/bios/cmos_bis.c"
    "src/mvdm/softpc.new/base/bios/build_id.c"
    "src/mvdm/softpc.new/base/bios/reset.c"
    "src/mvdm/softpc.new/base/bios/bootstra.c"
    "src/mvdm/softpc.new/base/bios/slave_bs.c"
    "src/mvdm/softpc.new/base/bios/tape_io.c"
    "src/mvdm/softpc.new/base/bios/equipmnt.c"
    "src/mvdm/softpc.new/base/bios/mem_size.c"
    "src/mvdm/softpc.new/base/bios/rom_basc.c"
    "src/mvdm/softpc.new/base/keymouse/ppi.c"
    "src/mvdm/softpc.new/base/keymouse/keyba.c"
    "src/mvdm/softpc.new/base/keymouse/keybd_io.c"
    "src/mvdm/softpc.new/base/keymouse/mouse.c"
    "src/mvdm/softpc.new/base/keymouse/mouse_io.c"
    "src/mvdm/softpc.new/base/disks/fla.c"
    "src/mvdm/softpc.new/base/disks/gfi.c"
    "src/mvdm/softpc.new/base/disks/gfi_mpty.c"
    "src/mvdm/softpc.new/base/disks/floppy.c"
    "src/mvdm/softpc.new/base/disks/floppy_i.c"
    "src/mvdm/softpc.new/base/disks/fdisk.c"
    "src/mvdm/softpc.new/base/disks/diskbios.c"
    "src/mvdm/softpc.new/base/comms/com.c"
    "src/mvdm/softpc.new/base/comms/printer.c"
    "src/mvdm/softpc.new/base/comms/printer_.c"
    "src/mvdm/softpc.new/base/comms/rs232_io.c"
    "src/mvdm/softpc.new/base/video/ega_mode.c"
    "src/mvdm/softpc.new/base/video/ega_prts.c"
    "src/mvdm/softpc.new/base/video/ega_vide.c"
    "src/mvdm/softpc.new/base/video/gfx_updt.c"
    "src/mvdm/softpc.new/base/video/video.c"
    "src/mvdm/softpc.new/base/video/gvi.c"
    "src/mvdm/softpc.new/base/video/video_io.c"
    "src/mvdm/softpc.new/base/video/v7_ports.c"
    "src/mvdm/softpc.new/base/video/v7_video.c"
    "src/mvdm/softpc.new/base/video/vga_mode.c"
    "src/mvdm/softpc.new/base/video/vga_prts.c"
    "src/mvdm/softpc.new/base/video/vga_vide.c"
    "src/mvdm/softpc.new/base/video/ega_read.c"
    "src/mvdm/softpc.new/base/video/ega_writ.c"
    "src/mvdm/softpc.new/base/video/ega_dmmy.c"
    "src/mvdm/softpc.new/base/video/ega_trcr.c"
    "src/mvdm/softpc.new/base/video/egawrtm0.c"
    "src/mvdm/softpc.new/base/video/egwrtm12.c"
    "src/mvdm/softpc.new/base/video/cga.c"
    "src/mvdm/softpc.new/host/src/nt_cga.c"
    "src/mvdm/softpc.new/host/src/nt_ega.c"
    "src/mvdm/softpc.new/host/src/nt_vga.c"
    "src/mvdm/softpc.new/host/src/nt_munge.c"
    "src/mvdm/softpc.new/host/src/nt_graph.c"
    "src/mvdm/softpc.new/host/src/nt_keycd.c"
    "src/mvdm/softpc.new/host/src/nt_com.c"
    "src/mvdm/softpc.new/host/src/nt_lpt.c"
    "src/mvdm/softpc.new/host/src/nt_sound.c")
foreach(source IN LISTS required_original_controller_sources)
    string(FIND "${build_definition}" "${source}" source_location)
    if(source_location EQUAL -1)
        message(FATAL_ERROR "Standalone SoftPC omits original controller: ${source}")
    endif()
endforeach()

# Firmware is part of the standalone machine's executable guest contract.
# Discover every checked-in ROM source so future BIOS additions cannot evade
# the same product-semantic guard through a stale manual list.
file(GLOB firmware_sources
    "${SOFTPC_SOURCE_DIR}/src/core/firmware/*.asm")
list(APPEND standalone_sources ${firmware_sources})

foreach(source IN LISTS standalone_sources)
    file(READ "${source}" contents)
    string(TOLOWER "${contents}" normalized_contents)
    if(normalized_contents MATCHES "(^|[^[:alnum:]_])(mvdm|ntvdm|vdm|wow|vdd|basesrv|csr|dos)([^[:alnum:]_]|$)")
        message(FATAL_ERROR "Standalone CCPU contains product-shell semantics: ${source}")
    endif()
endforeach()

file(READ "${SOFTPC_SOURCE_DIR}/src/host/platform/softpc_standalone_platform.c"
    standalone_platform)
string(TOLOWER "${standalone_platform}" normalized_platform)
if(normalized_platform MATCHES "softpc_ata")
    message(FATAL_ERROR "Standalone platform retains a handwritten ATA controller")
endif()
if(normalized_platform MATCHES "paint_v7ptr|clear_v7ptr")
    message(FATAL_ERROR "Standalone platform retains V7 presentation callbacks")
endif()

# Presentation is deliberately a DIB consumer.  Controller planes and DAC
# interpretation stay in the original nt_cga/nt_ega/nt_vga renderer path.
file(READ "${SOFTPC_SOURCE_DIR}/src/vm/win32_window.c" window_frontend)
string(TOLOWER "${window_frontend}" normalized_window_frontend)
if(normalized_window_frontend MATCHES "ega_planes|\\bdac\\b")
    message(FATAL_ERROR "Standalone window bypasses the original SoftPC renderer")
endif()

file(READ "${SOFTPC_SOURCE_DIR}/src/vm/console.c" console_frontend)
string(TOLOWER "${window_frontend}${console_frontend}" normalized_frontends)
if(normalized_frontends MATCHES "host_key_(down|up)|mouse_send")
    message(FATAL_ERROR "Standalone frontend bypasses original SoftPC input controllers")
endif()

# Runtime owns every machine pointer.  Both display frontends are mailbox
# clients: they may enqueue host records and copy published frames only.
if(normalized_frontends MATCHES "softpc_machine_")
    message(FATAL_ERROR "Standalone frontend directly accesses the machine")
endif()

# M8 T1: test tiers are an input boundary, not merely a CTest convention.
# Unit fixtures may write their tiny disk bytes under build/, but neither their
# source nor their resource scripts may name product artifacts. Integration is
# deliberately separate because it exercises that package.
if(EXISTS "${SOFTPC_SOURCE_DIR}/tests")
    message(FATAL_ERROR "Legacy tests/ directory remains; use test/{unit,integration,support}")
endif()
file(GLOB_RECURSE unit_test_sources
    "${SOFTPC_SOURCE_DIR}/test/unit/*"
    "${SOFTPC_SOURCE_DIR}/test/support/*.cmake")
foreach(source IN LISTS unit_test_sources)
    if(IS_DIRECTORY "${source}")
        continue()
    endif()
    file(READ "${source}" contents)
    string(TOLOWER "${contents}" normalized_contents)
    if(normalized_contents MATCHES "artifacts[/\\\\]")
        message(FATAL_ERROR "Unit test names product artifacts: ${source}")
    endif()
endforeach()

if(NOT EXISTS "${SOFTPC_SOURCE_DIR}/test/integration/softpc_package_smoke.c")
    message(FATAL_ERROR "Missing fixed-package integration runner")
endif()
if(build_definition MATCHES "tests/")
    message(FATAL_ERROR "CMake retains a legacy tests/ source reference")
endif()
if(NOT build_definition MATCHES "LABELS \\\"unit\\\"")
    message(FATAL_ERROR "CMake does not label the unit test tier")
endif()
if(NOT build_definition MATCHES "LABELS \\\"integration;")
    message(FATAL_ERROR "CMake does not label the integration test tier")
endif()
