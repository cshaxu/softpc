# New host code has one concrete ownership taxonomy.  No compatibility or
# convenience aggregate may appear beside these six owners.
set(allowed_host_taxonomies comms compat input machine media platform video)
file(GLOB host_entries RELATIVE "${SOFTPC_SOURCE_DIR}/src/host"
    "${SOFTPC_SOURCE_DIR}/src/host/*")
foreach(host_entry IN LISTS host_entries)
    if(IS_DIRECTORY "${SOFTPC_SOURCE_DIR}/src/host/${host_entry}")
        list(FIND allowed_host_taxonomies "${host_entry}" host_taxonomy_index)
        if(host_taxonomy_index EQUAL -1)
            message(FATAL_ERROR "Standalone host has no taxonomy owner: ${host_entry}")
        endif()
    endif()
endforeach()

set(standalone_sources
    "${SOFTPC_SOURCE_DIR}/src/host/compat/ccpu/facade.c"
    "${SOFTPC_SOURCE_DIR}/src/host/compat/cvidc/gdp_state.c"
    "${SOFTPC_SOURCE_DIR}/src/host/compat/cvidc/gdp_state.h"
    "${SOFTPC_SOURCE_DIR}/src/host/compat/cvidc/gdp_slots.h"
    "${SOFTPC_SOURCE_DIR}/src/mvdm/softpc.new/base/cvidc/sascdef.c"
    "${SOFTPC_SOURCE_DIR}/src/mvdm/softpc.new/base/support/ios.c"
    "${SOFTPC_SOURCE_DIR}/src/mvdm/softpc.new/base/disks/fdisk.c"
    "${SOFTPC_SOURCE_DIR}/src/host/gfi_image.c"
    "${SOFTPC_SOURCE_DIR}/src/host/hdd_media.c"
    "${SOFTPC_SOURCE_DIR}/src/host/video.c"
    "${SOFTPC_SOURCE_DIR}/src/host/v7_pointer.c"
    "${SOFTPC_SOURCE_DIR}/src/host/memory.c"
    "${SOFTPC_SOURCE_DIR}/src/mvdm/softpc.new/base/keymouse/keybd_io.c"
    "${SOFTPC_SOURCE_DIR}/src/mvdm/softpc.new/base/system/idetect.c"
    "${SOFTPC_SOURCE_DIR}/src/host/device_bop.c"
    "${SOFTPC_SOURCE_DIR}/src/host/platform.c"
    "${SOFTPC_SOURCE_DIR}/src/host/machine.c")

if(EXISTS "${SOFTPC_SOURCE_DIR}/src/host/softpc_compat")
    message(FATAL_ERROR "Standalone host retains the obsolete softpc_compat taxonomy")
endif()

# The transitional aggregates must not return after the source-layout move.
if(EXISTS "${SOFTPC_SOURCE_DIR}/src/vm")
    message(FATAL_ERROR "Standalone application retains the obsolete src/vm layout")
endif()
if(EXISTS "${SOFTPC_SOURCE_DIR}/src/core")
    message(FATAL_ERROR "Standalone source retains the obsolete src/core layout")
endif()
foreach(app_source IN ITEMS
    "src/app/main.c"
    "src/app/runtime.c"
    "src/app/presentation.c"
    "src/app/keyboard.c")
    if(NOT EXISTS "${SOFTPC_SOURCE_DIR}/${app_source}")
        message(FATAL_ERROR "Standalone application source is missing: ${app_source}")
    endif()
endforeach()

# The imported UX component consumes copied values only.
# It cannot acquire SoftPC's runtime, machine, renderer, or original key-map
# ownership; those remain in the project binding under src/app.
file(GLOB_RECURSE shared_win32_sources
    "${SOFTPC_SOURCE_DIR}/src/lib/ux-base/*.[ch]"
    "${SOFTPC_SOURCE_DIR}/src/lib/ux-window/*.[ch]"
    "${SOFTPC_SOURCE_DIR}/src/lib/ux-console/*.[ch]")
foreach(source IN LISTS shared_win32_sources)
    file(READ "${source}" shared_win32_contents)
    if(shared_win32_contents MATCHES
        "(softpc_machine|app_runtime|KeyMsgToKeyCode|EGA_planes|host_machine_)")
        message(FATAL_ERROR "Shared Win32 presentation leaks project ownership: ${source}")
    endif()
endforeach()

# The checked-in shared corpus must be reusable without exposing either
# importing product's identity through native class registration or source.
# The standalone CMake project name is an explicit NXVM compatibility
# exemption and is deliberately not included here.
file(GLOB_RECURSE shared_identity_sources
    "${SOFTPC_SOURCE_DIR}/src/lib/*.[ch]"
    "${SOFTPC_SOURCE_DIR}/src/lib/README.md")
foreach(source IN LISTS shared_identity_sources)
    file(READ "${source}" shared_identity_contents)
    string(TOLOWER "${shared_identity_contents}" normalized_shared_identity)
    if(normalized_shared_identity MATCHES "(softpc|insignia|nxvm)")
        message(FATAL_ERROR "Shared library leaks importing product identity: ${source}")
    endif()
endforeach()

# Shared implementation and its component documentation describe only generic
# application mechanics. Product execution vocabulary belongs in the importer.
file(GLOB_RECURSE shared_neutral_corpus
    "${SOFTPC_SOURCE_DIR}/src/lib/*.[ch]"
    "${SOFTPC_SOURCE_DIR}/src/lib/*.md")
foreach(source IN LISTS shared_neutral_corpus)
    file(READ "${source}" shared_neutral_contents)
    string(TOLOWER "${shared_neutral_contents}" normalized_shared_neutral)
    if(normalized_shared_neutral MATCHES
        "(^|[^[:alnum:]_])(guest|vm|machine)([^[:alnum:]_]|$)")
        message(FATAL_ERROR
            "Shared library leaks product execution terminology: ${source}")
    endif()
endforeach()

# A shared public contract is visibly named. Product code must not reach a
# component implementation header, and one public contract may compose only
# other public contracts.
file(GLOB_RECURSE product_lib_consumers
    "${SOFTPC_SOURCE_DIR}/src/app/*.[ch]"
    "${SOFTPC_SOURCE_DIR}/src/host/*.[ch]")
file(GLOB_RECURSE shared_interface_headers
    "${SOFTPC_SOURCE_DIR}/src/lib/*_interface.h")
foreach(source IN LISTS product_lib_consumers shared_interface_headers)
    file(STRINGS "${source}" include_lines REGEX
        "#[ \t]*include[ \t]+\"lib/[^\"]+\.h\"")
    foreach(include_line IN LISTS include_lines)
        string(FIND "${include_line}" "_interface.h\"" interface_suffix)
        if(interface_suffix EQUAL -1)
            message(FATAL_ERROR "Non-interface library header crosses a public boundary: ${source}")
        endif()
    endforeach()
endforeach()

# ux-window owns no product default.  The application supplies one creation
# title, ux-window copies it before native startup, and the native Window uses
# that copy rather than a hidden literal.
file(READ "${SOFTPC_SOURCE_DIR}/src/lib/ux-window/window_interface.h" window_header)
file(READ "${SOFTPC_SOURCE_DIR}/src/lib/ux-window/window.c" window_source)
file(READ "${SOFTPC_SOURCE_DIR}/src/lib/ux-window/win32/component.c"
    native_window_source)
string(FIND "${window_header}" "const char *initial_title" title_option_index)
string(FIND "${window_source}" "memcpy(window->initial_title" title_copy_index)
string(FIND "${native_window_source}" "component->initial_title" title_native_index)
if(title_option_index EQUAL -1 OR title_copy_index EQUAL -1 OR
    title_native_index EQUAL -1)
    message(FATAL_ERROR "ux-window must use the application's copied initial title")
endif()

# The public UX input ABI uses only lib-defined key identities and flags. Win32
# values are permitted inside the private win32 adapter and SoftPC binding, not
# in the shared public event contract.
file(READ "${SOFTPC_SOURCE_DIR}/src/lib/ux-base/event_interface.h" ux_event_header)
if(ux_event_header MATCHES "VK_[A-Za-z0-9_]+" OR
    ux_event_header MATCHES "ENHANCED_KEY" OR
    ux_event_header MATCHES "KEY_EVENT_RECORD")
    message(FATAL_ERROR "Public UX input ABI leaks a Win32 key/injection value")
endif()

# Component selection is application policy.  Shared UX must not retain the
# removed unified runner or a target router.
file(READ "${SOFTPC_SOURCE_DIR}/src/app/main.c" app_main_source)
file(READ "${SOFTPC_SOURCE_DIR}/src/app/presentation.c" app_presentation_source)
if(EXISTS "${SOFTPC_SOURCE_DIR}/src/lib/ux" OR
   app_main_source MATCHES "ux_presenter|ux_run" OR
   app_presentation_source MATCHES "ux_presenter|ux_run")
    message(FATAL_ERROR "Standalone retains the removed unified UX route")
endif()

# Host owns only generic native Console I/O. The product monitor owns its
# logical Console and line sink, and binds it through the public broker API.
file(READ "${SOFTPC_SOURCE_DIR}/src/lib/host/console_interface.h" host_console_public)
file(READ "${SOFTPC_SOURCE_DIR}/src/lib/host/console.c" host_console_source)
file(READ "${SOFTPC_SOURCE_DIR}/src/app/monitor.c" app_monitor_source)
if(host_console_public MATCHES "host_console_cooked" OR
   host_console_source MATCHES "host_console_cooked" OR
   app_monitor_source MATCHES "host_console_cooked" OR
   NOT app_monitor_source MATCHES "host_console_broker_replace" OR
   NOT app_monitor_source MATCHES "host_console_broker_request_cooked_line")
    message(FATAL_ERROR "Console broker retains monitor-specific ownership")
endif()

# The split shared UX graph is deliberately narrow.  These target links make
# its allowed component edges executable rather than README-only claims.
file(READ "${SOFTPC_SOURCE_DIR}/src/lib/CMakeLists.txt" lib_cmake_source)
if(NOT lib_cmake_source MATCHES
   "target_link_libraries\\(ux-base PUBLIC base-console\\)" OR
   NOT lib_cmake_source MATCHES
   "target_link_libraries\\(ux-window PUBLIC base-console ux-base\\)" OR
   NOT lib_cmake_source MATCHES
   "target_link_libraries\\(ux-console PUBLIC base-console ux-base\\)" OR
   lib_cmake_source MATCHES
   "target_link_libraries\\(ux-(window|console) [^\\)]*(host-sync|storage-medium)")
    message(FATAL_ERROR "Shared UX component dependency graph is not split")
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
file(GLOB retired_source_transform_scripts
    "${SOFTPC_SOURCE_DIR}/scripts/transform_*.py")
if(retired_source_transform_scripts)
    message(FATAL_ERROR "Standalone build retains source-transform scripts: ${retired_source_transform_scripts}")
endif()
if(normalized_build_definition MATCHES
    "(add_custom_command|python3|generated-(ccpu|cvid|device))")
    message(FATAL_ERROR "Standalone build retains generated-source machinery")
endif()
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
    "src/mvdm/softpc.new/host/src/nt_sound.c")
foreach(source IN LISTS required_original_controller_sources)
    string(FIND "${build_definition}" "${source}" source_location)
    if(source_location EQUAL -1)
        message(FATAL_ERROR "Standalone SoftPC omits original controller: ${source}")
    endif()
endforeach()

foreach(source IN LISTS standalone_sources)
    file(READ "${source}" contents)
    string(TOLOWER "${contents}" normalized_contents)
    if(normalized_contents MATCHES "(^|[^[:alnum:]_])(mvdm|ntvdm|vdm|wow|vdd|basesrv|csr|dos)([^[:alnum:]_]|$)")
        message(FATAL_ERROR "Standalone CCPU contains product-shell semantics: ${source}")
    endif()
endforeach()

file(READ "${SOFTPC_SOURCE_DIR}/src/host/platform.c"
    standalone_platform)
string(TOLOWER "${standalone_platform}" normalized_platform)
if(normalized_platform MATCHES "host_ata")
    message(FATAL_ERROR "Standalone platform retains a handwritten ATA controller")
endif()
if(normalized_platform MATCHES "paint_v7ptr|clear_v7ptr")
    message(FATAL_ERROR "Standalone platform retains V7 presentation callbacks")
endif()
if(normalized_platform MATCHES "host_ram")
    message(FATAL_ERROR "Standalone platform retains SAS machine-memory backing")
endif()

# Presentation is deliberately a DIB consumer.  Controller planes and DAC
# interpretation stay in the original nt_cga/nt_ega/nt_vga renderer path.
file(READ "${SOFTPC_SOURCE_DIR}/src/lib/ux-window/win32/component.c" window_frontend)
string(TOLOWER "${window_frontend}" normalized_window_frontend)
if(normalized_window_frontend MATCHES "ega_planes|\\bdac\\b")
    message(FATAL_ERROR "Standalone window bypasses the original SoftPC renderer")
endif()

file(READ "${SOFTPC_SOURCE_DIR}/src/lib/ux-console/win32/component.c" console_frontend)
string(TOLOWER "${window_frontend}${console_frontend}" normalized_frontends)
if(normalized_frontends MATCHES "host_key_(down|up)|mouse_send")
    message(FATAL_ERROR "Standalone frontend bypasses original SoftPC input controllers")
endif()

# Runtime owns every machine pointer.  Both display frontends are mailbox
# clients: they may enqueue host records and copy published frames only.
if(normalized_frontends MATCHES "host_machine_")
    message(FATAL_ERROR "Standalone frontend directly accesses the machine")
endif()

# Application globals make their owner visible. The compatibility host remains
# part of the original machine's generated and direct host ABI, and therefore
# retains the historical spellings imported by that machine.
file(GLOB_RECURSE standalone_owner_sources
    "${SOFTPC_SOURCE_DIR}/src/app/*.[ch]")
foreach(source IN LISTS standalone_owner_sources)
    file(READ "${source}" owner_source)
    string(REPLACE "softpc_machine" "" owner_source "${owner_source}")
    string(REPLACE "softpc_presentation" "" owner_source "${owner_source}")
    string(REPLACE "softpc_media_mode" "" owner_source "${owner_source}")
    if(owner_source MATCHES "(^|[^[:alnum:]_])softpc_[A-Za-z0-9_]+")
        message(FATAL_ERROR "Standalone application retains an unowned softpc_ symbol: ${source}")
    endif()
endforeach()

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
    if(normalized_contents MATCHES "(assets|artifacts)[/\\\\]")
        message(FATAL_ERROR "Unit test names product assets: ${source}")
    endif()
endforeach()

if(NOT EXISTS "${SOFTPC_SOURCE_DIR}/test/integration/package_smoke.c")
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
