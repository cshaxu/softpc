set(standalone_sources
    "${SOFTPC_SOURCE_DIR}/src/core/softpc/base/ccpu386/softpc_ccpu_facade.c"
    "${SOFTPC_SOURCE_DIR}/src/core/softpc/base/cvidc/softpc_gdp_state.c"
    "${SOFTPC_SOURCE_DIR}/src/core/softpc/base/cvidc/softpc_gdp_state.h"
    "${SOFTPC_SOURCE_DIR}/src/core/softpc/base/cvidc/softpc_gdp_slots.h"
    "${SOFTPC_SOURCE_DIR}/src/core/softpc/base/cvidc/sascdef.c"
    "${SOFTPC_SOURCE_DIR}/src/core/softpc/base/support/ios.c"
    "${SOFTPC_SOURCE_DIR}/src/core/softpc/base/disks/fdisk.c"
    "${SOFTPC_SOURCE_DIR}/src/core/softpc_gfi_image.c"
    "${SOFTPC_SOURCE_DIR}/src/core/softpc/base/keymouse/keybd_io.c"
    "${SOFTPC_SOURCE_DIR}/src/core/softpc/base/system/idetect.c"
    "${SOFTPC_SOURCE_DIR}/src/core/softpc_device_bop.c"
    "${SOFTPC_SOURCE_DIR}/src/core/softpc_standalone_platform.c"
    "${SOFTPC_SOURCE_DIR}/src/core/softpc_machine.c")

if(EXISTS "${SOFTPC_SOURCE_DIR}/src/core/softpc_physical_mapping.c" OR
   EXISTS "${SOFTPC_SOURCE_DIR}/src/core/softpc_physical_mapping.h")
    message(FATAL_ERROR "Standalone SoftPC retains the obsolete physical-mapping shim")
endif()

file(STRINGS "${SOFTPC_SOURCE_DIR}/src/core/softpc/base/ccpu386/c-files"
    ccpu_source_names)
foreach(name IN LISTS ccpu_source_names)
    if(NOT name MATCHES "^p\\.")
        list(APPEND standalone_sources
            "${SOFTPC_SOURCE_DIR}/src/core/softpc/base/ccpu386/${name}")
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
    "src/core/softpc/base/support/ios.c"
    "src/core/softpc/base/system/ica.c"
    "src/core/softpc/base/system/quick_ev.c"
    "src/core/softpc/base/system/at_dma.c"
    "src/core/softpc/base/system/cmos.c"
    "src/core/softpc/base/system/dummy_nt.c"
    "src/core/softpc/base/system/idetect.c"
    "src/core/softpc/base/system/illegalp.c"
    "src/core/softpc/base/system/rom.c"
    "src/core/softpc/base/system/timer.c"
    "src/core/softpc/base/system/timestrb.c"
    "src/core/softpc/base/system/unexp_nt.c"
    "src/core/softpc/base/support/time_day.c"
    "src/core/softpc/base/bios/cmos_bis.c"
    "src/core/softpc/base/bios/build_id.c"
    "src/core/softpc/base/bios/reset.c"
    "src/core/softpc/base/bios/bootstra.c"
    "src/core/softpc/base/bios/slave_bs.c"
    "src/core/softpc/base/bios/tape_io.c"
    "src/core/softpc/base/bios/equipmnt.c"
    "src/core/softpc/base/bios/mem_size.c"
    "src/core/softpc/base/bios/rom_basc.c"
    "src/core/softpc/base/keymouse/ppi.c"
    "src/core/softpc/base/keymouse/keyba.c"
    "src/core/softpc/base/keymouse/keybd_io.c"
    "src/core/softpc/base/keymouse/mouse.c"
    "src/core/softpc/base/keymouse/mouse_io.c"
    "src/core/softpc/base/disks/fla.c"
    "src/core/softpc/base/disks/gfi.c"
    "src/core/softpc/base/disks/gfi_mpty.c"
    "src/core/softpc/base/disks/floppy.c"
    "src/core/softpc/base/disks/floppy_i.c"
    "src/core/softpc/base/disks/fdisk.c"
    "src/core/softpc/base/disks/diskbios.c"
    "src/core/softpc/base/comms/com.c"
    "src/core/softpc/base/comms/printer.c"
    "src/core/softpc/base/comms/printer_.c"
    "src/core/softpc/base/comms/rs232_io.c"
    "src/core/softpc/base/video/ega_mode.c"
    "src/core/softpc/base/video/ega_prts.c"
    "src/core/softpc/base/video/ega_vide.c"
    "src/core/softpc/base/video/gfx_updt.c"
    "src/core/softpc/base/video/video.c"
    "src/core/softpc/base/video/gvi.c"
    "src/core/softpc/base/video/video_io.c"
    "src/core/softpc/base/video/v7_ports.c"
    "src/core/softpc/base/video/v7_video.c"
    "src/core/softpc/base/video/vga_mode.c"
    "src/core/softpc/base/video/vga_prts.c"
    "src/core/softpc/base/video/vga_vide.c"
    "src/core/softpc/base/video/ega_read.c"
    "src/core/softpc/base/video/ega_writ.c"
    "src/core/softpc/base/video/ega_dmmy.c"
    "src/core/softpc/base/video/ega_trcr.c"
    "src/core/softpc/base/video/egawrtm0.c"
    "src/core/softpc/base/video/egwrtm12.c"
    "src/core/softpc/base/video/cga.c"
    "src/core/softpc/host/src/nt_cga.c"
    "src/core/softpc/host/src/nt_ega.c"
    "src/core/softpc/host/src/nt_vga.c"
    "src/core/softpc/host/src/nt_munge.c"
    "src/core/softpc/host/src/nt_graph.c"
    "src/core/softpc/host/src/nt_keycd.c"
    "src/core/softpc/host/src/nt_com.c"
    "src/core/softpc/host/src/nt_lpt.c"
    "src/core/softpc/host/src/nt_sound.c")
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

file(READ "${SOFTPC_SOURCE_DIR}/src/core/softpc_standalone_platform.c"
    standalone_platform)
string(TOLOWER "${standalone_platform}" normalized_platform)
if(normalized_platform MATCHES "softpc_ata")
    message(FATAL_ERROR "Standalone platform retains a handwritten ATA controller")
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
