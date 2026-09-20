# Package presets must repair stale Release flags in reused build trees.
file(READ "${SOFTPC_SOURCE_DIR}/CMakePresets.json" package_presets)
foreach(preset_index RANGE 0 1)
    string(JSON package_type GET "${package_presets}"
        configurePresets ${preset_index} cacheVariables CMAKE_BUILD_TYPE)
    string(JSON package_flags GET "${package_presets}"
        configurePresets ${preset_index} cacheVariables CMAKE_C_FLAGS_RELEASE)
    if(NOT package_type STREQUAL "Release" OR
        NOT package_flags STREQUAL "-O3 -DNDEBUG")
        message(FATAL_ERROR "Both GNU package presets must select optimized Release flags")
    endif()
endforeach()

# New host code has one concrete ownership taxonomy.  No compatibility or
# convenience aggregate may appear beside these seven owners.
set(allowed_host_taxonomies bios ccpu cmos cvidc devices keymouse system)
file(GLOB host_entries RELATIVE "${SOFTPC_SOURCE_DIR}/src/compat"
    "${SOFTPC_SOURCE_DIR}/src/compat/*")
foreach(host_entry IN LISTS host_entries)
    if(IS_DIRECTORY "${SOFTPC_SOURCE_DIR}/src/compat/${host_entry}")
        list(FIND allowed_host_taxonomies "${host_entry}" host_taxonomy_index)
        if(host_taxonomy_index EQUAL -1)
            message(FATAL_ERROR "Standalone host has no taxonomy owner: ${host_entry}")
        endif()
    endif()
endforeach()

# Implementation headers are private by default. Their filenames carry their
# local role; only public contracts use the *_interface.h suffix.
file(GLOB_RECURSE shared_library_paths RELATIVE "${SOFTPC_SOURCE_DIR}/src/lib"
    "${SOFTPC_SOURCE_DIR}/src/lib/*")
foreach(shared_library_path IN LISTS shared_library_paths)
    if(shared_library_path MATCHES "(^|/|_)(private|internal|native)(_|\\.|/)")
        message(FATAL_ERROR
            "Shared library implementation has a redundant private/internal suffix: ${shared_library_path}")
    endif()
endforeach()

file(GLOB_RECURSE library_named_sources "${SOFTPC_SOURCE_DIR}/src/lib/*.[ch]")
foreach(source IN LISTS library_named_sources)
    file(READ "${source}" naming_source)
    string(REGEX REPLACE "/\\*([^*]|\\*+[^*/])*\\*+/" " " naming_source "${naming_source}")
    string(REGEX REPLACE "//[^\n]*" " " naming_source "${naming_source}")
    if(naming_source MATCHES "[A-Za-z_][A-Za-z0-9_]*_(native|internal|private)(_|[^A-Za-z0-9_])" OR
       naming_source MATCHES "(^|[^A-Za-z0-9_])(native|internal|private)_[A-Za-z0-9_]+")
        message(FATAL_ERROR "Library identifier must name its operation: ${source}")
    endif()
endforeach()

# types is vocabulary only: platform behavior is implemented by the owning
# component's selected win32/linux source, never by a hidden types target.
set(LIBRARY_ROOT "${SOFTPC_SOURCE_DIR}/src/lib")
include("${LIBRARY_ROOT}/verify_types_layout.cmake")
file(GLOB_RECURSE types_implementation_sources
    "${SOFTPC_SOURCE_DIR}/src/lib/types/*.c")
if(types_implementation_sources)
    message(FATAL_ERROR
        "types must be header-only: ${types_implementation_sources}")
endif()
file(READ "${SOFTPC_SOURCE_DIR}/src/lib/CMakeLists.txt" lib_cmake_types_source)
if(NOT lib_cmake_types_source MATCHES "add_library\\(types INTERFACE\\)" OR
   lib_cmake_types_source MATCHES "LIBRARY_TYPES_SOURCES")
    message(FATAL_ERROR "types must remain an INTERFACE-only vocabulary target")
endif()
foreach(platform_neutral_base IN ITEMS
    "src/lib/base/clock.c"
    "src/lib/base/sync.c"
    "src/lib/storage/file.c"
    "src/lib/kvm-base/mailbox.c")
    file(READ "${SOFTPC_SOURCE_DIR}/${platform_neutral_base}" base_source)
    if(base_source MATCHES "#[ \\t]*(if|ifdef|ifndef)[^\\n]*(WIN32|__linux__|__APPLE__)")
        message(FATAL_ERROR
            "Neutral component base contains platform selection: ${platform_neutral_base}")
    endif()
endforeach()
foreach(component_platform_source IN ITEMS
    "src/lib/base/win32/clock.c"
    "src/lib/base/linux/clock.c"
    "src/lib/base/win32/sync.c"
    "src/lib/base/linux/sync.c"
    "src/lib/storage/win32/file.c"
    "src/lib/storage/linux/file.c")
    if(NOT EXISTS "${SOFTPC_SOURCE_DIR}/${component_platform_source}")
        message(FATAL_ERROR "Missing selected-platform peer: ${component_platform_source}")
    endif()
endforeach()

set(standalone_sources
    "${SOFTPC_SOURCE_DIR}/src/compat/ccpu/facade.c"
    "${SOFTPC_SOURCE_DIR}/src/compat/cvidc/gdp_state.c"
    "${SOFTPC_SOURCE_DIR}/src/compat/cvidc/gdp_state.h"
    "${SOFTPC_SOURCE_DIR}/src/compat/cvidc/gdp_slots.h"
    "${SOFTPC_SOURCE_DIR}/src/mvdm/softpc.new/base/cvidc/sascdef.c"
    "${SOFTPC_SOURCE_DIR}/src/mvdm/softpc.new/base/support/ios.c"
    "${SOFTPC_SOURCE_DIR}/src/mvdm/softpc.new/base/disks/fdisk.c"
    "${SOFTPC_SOURCE_DIR}/src/compat/gfi_image.c"
    "${SOFTPC_SOURCE_DIR}/src/compat/hdd_media.c"
    "${SOFTPC_SOURCE_DIR}/src/compat/video.c"
    "${SOFTPC_SOURCE_DIR}/src/compat/v7_pointer.c"
    "${SOFTPC_SOURCE_DIR}/src/compat/memory.c"
    "${SOFTPC_SOURCE_DIR}/src/mvdm/softpc.new/base/keymouse/keybd_io.c"
    "${SOFTPC_SOURCE_DIR}/src/mvdm/softpc.new/base/system/idetect.c"
    "${SOFTPC_SOURCE_DIR}/src/compat/device_bop.c"
    "${SOFTPC_SOURCE_DIR}/src/compat/platform.c"
    "${SOFTPC_SOURCE_DIR}/src/vm/machine.c")

if(EXISTS "${SOFTPC_SOURCE_DIR}/src/compat/softpc_compat")
    message(FATAL_ERROR "Standalone host retains the obsolete softpc_compat taxonomy")
endif()

# The transitional aggregates must not return after the source-layout move.
if(EXISTS "${SOFTPC_SOURCE_DIR}/src/host")
    message(FATAL_ERROR "Standalone retains the retired src/host layout")
endif()
if(EXISTS "${SOFTPC_SOURCE_DIR}/src/core")
    message(FATAL_ERROR "Standalone source retains the obsolete src/core layout")
endif()
foreach(app_source IN ITEMS
    "src/app/main.c"
    "src/vm/driver.c"
    "src/app/keyboard.c")
    if(NOT EXISTS "${SOFTPC_SOURCE_DIR}/${app_source}")
        message(FATAL_ERROR "Standalone application source is missing: ${app_source}")
    endif()
endforeach()
foreach(retired_machine_source IN ITEMS
    "src/compat/machine.c"
    "src/compat/machine.h"
    "src/app/command_binding.c"
    "src/app/command_binding.h"
    "src/app/machine_driver.c"
    "src/app/machine_driver.h"
    "src/app/prompt_trace.c"
    "src/app/prompt_trace.h"
    "src/app/runtime.c"
    "src/app/runtime.h"
    "src/app/input_queue.c"
    "src/app/input_queue.h")
    if(EXISTS "${SOFTPC_SOURCE_DIR}/${retired_machine_source}")
        message(FATAL_ERROR "Application retains a second machine implementation: ${retired_machine_source}")
    endif()
endforeach()
if(NOT EXISTS "${SOFTPC_SOURCE_DIR}/src/common/machine/machine.c")
    message(FATAL_ERROR "Common machine source is missing")
endif()
if(NOT EXISTS "${SOFTPC_SOURCE_DIR}/src/common/ui/ui.c")
    message(FATAL_ERROR "Common UI source is missing")
endif()
if(NOT EXISTS "${SOFTPC_SOURCE_DIR}/src/common/session/session.c")
    message(FATAL_ERROR "Common session source is missing")
endif()
foreach(retired_session_source IN ITEMS
    "src/app/control.c"
    "src/app/control.h"
    "src/app/control_state.c"
    "src/app/control_state.h"
    "src/app/reconciler.c"
    "src/app/reconciler.h"
    "src/app/presentation_plan.c"
    "src/app/presentation_plan.h")
    if(EXISTS "${SOFTPC_SOURCE_DIR}/${retired_session_source}")
        message(FATAL_ERROR "Application retains a second session implementation: ${retired_session_source}")
    endif()
endforeach()
file(READ "${SOFTPC_SOURCE_DIR}/src/app/composition.c" app_session_composition)
if(NOT app_session_composition MATCHES "common_session_create" OR
   app_session_composition MATCHES "common_session_(queue|state|reconciler)_")
    message(FATAL_ERROR "Application must compose, not implement, common session control")
endif()

# Product code may compose Common/x86 through root contracts, but never reach
# their implementation, private helper, or source file. Keeping this
# allow-list small makes the post-extraction ownership boundary executable.
set(allowed_common_product_contracts
    "common/machine/machine_interface.h"
    "common/session/session_interface.h"
    "common/ui/ui_interface.h"
    "x86/debug/debug_interface.h"
    "x86/debug/protocol_interface.h"
    "x86/xasm32/xasm32_interface.h")
file(GLOB_RECURSE product_common_consumers
    "${SOFTPC_SOURCE_DIR}/src/app/*.[ch]"
    "${SOFTPC_SOURCE_DIR}/src/vm/*.[ch]"
    "${SOFTPC_SOURCE_DIR}/src/compat/*.[ch]")
foreach(source IN LISTS product_common_consumers)
    file(STRINGS "${source}" common_include_lines REGEX
        "#[ \t]*include[ \t]+[<\"](common|x86)/[^>\"]+[>\"]")
    foreach(include_line IN LISTS common_include_lines)
        string(REGEX REPLACE
            ".*[<\"]((common|x86)/[^>\"]+)[>\"].*" "\\1" common_contract
            "${include_line}")
        list(FIND allowed_common_product_contracts "${common_contract}"
            common_contract_index)
        if(common_contract_index EQUAL -1)
            message(FATAL_ERROR
                "Product reaches a non-public Common/x86 boundary: ${source}: ${include_line}")
        endif()
    endforeach()
    file(READ "${source}" common_product_source)
    if(common_product_source MATCHES "(common|x86)/[A-Za-z0-9_/-]+\\.c")
        message(FATAL_ERROR "Product includes a Common/x86 implementation: ${source}")
    endif()
endforeach()

include("${SOFTPC_SOURCE_DIR}/test/support/product_boundary.cmake")

# The imported KVM component consumes copied values only.
# It cannot acquire SoftPC's runtime, machine, renderer, or original key-map
# ownership; those remain in the project binding under src/app.
file(GLOB_RECURSE shared_win32_sources
    "${SOFTPC_SOURCE_DIR}/src/lib/kvm-base/*.[ch]"
    "${SOFTPC_SOURCE_DIR}/src/lib/kvm-window/*.[ch]"
    "${SOFTPC_SOURCE_DIR}/src/lib/kvm-console/*.[ch]")
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
# component implementation header. Shared contracts may include a private
# same-component inline helper where the canonical corpus needs its declarations
# to compile; that transitive implementation detail is not a product include.
file(GLOB_RECURSE product_lib_consumers
    "${SOFTPC_SOURCE_DIR}/src/app/*.[ch]"
    "${SOFTPC_SOURCE_DIR}/src/compat/*.[ch]")
foreach(source IN LISTS product_lib_consumers)
    file(STRINGS "${source}" include_lines REGEX
        "#[ \t]*include[ \t]+\"lib/[^\"]+\.h\"")
    foreach(include_line IN LISTS include_lines)
        string(FIND "${include_line}" "_interface.h\"" interface_suffix)
        if(interface_suffix EQUAL -1)
            message(FATAL_ERROR "Non-interface library header crosses a public boundary: ${source}")
        endif()
        if(include_line MATCHES "lib/kvm-base/(worker|mailbox|mailbox_wake|input|actions)_interface.h" OR
           include_line MATCHES "lib/[^/]+/(win32|linux)/" OR
           include_line MATCHES "lib/console/binding_interface.h")
            message(FATAL_ERROR "Leaf-support contract crosses an application boundary: ${source}")
        endif()
    endforeach()
endforeach()

# kvm-window owns no product default.  The application supplies one creation
# title, kvm-window copies it before native startup, and the native Window uses
# that copy rather than a hidden literal.
file(READ "${SOFTPC_SOURCE_DIR}/src/lib/kvm-window/window_interface.h" window_header)
file(READ "${SOFTPC_SOURCE_DIR}/src/lib/kvm-window/window.c" window_source)
file(READ "${SOFTPC_SOURCE_DIR}/src/lib/kvm-window/win32/component.c"
    native_window_source)
string(FIND "${window_header}" "const char *initial_title" title_option_index)
string(FIND "${window_source}" "lib_memory_copy(window->initial_title" title_copy_index)
string(FIND "${native_window_source}" "component->initial_title" title_native_index)
if(title_option_index EQUAL -1 OR title_copy_index EQUAL -1 OR
    title_native_index EQUAL -1)
    message(FATAL_ERROR "kvm-window must use the application's copied initial title")
endif()

# The public KVM input ABI uses only lib-defined key identities and flags. Win32
# values are permitted inside the private win32 adapter and SoftPC binding, not
# in the shared public event contract.
file(READ "${SOFTPC_SOURCE_DIR}/src/lib/kvm-base/event_interface.h" kvm_event_header)
if(kvm_event_header MATCHES "VK_[A-Za-z0-9_]+" OR
    kvm_event_header MATCHES "ENHANCED_KEY" OR
    kvm_event_header MATCHES "KEY_EVENT_RECORD")
    message(FATAL_ERROR "Public KVM input ABI leaks a Win32 key/injection value")
endif()

# Component selection is application policy.  Shared KVM must not retain the
# removed unified runner or a target router.
file(READ "${SOFTPC_SOURCE_DIR}/src/app/main.c" app_main_source)
file(READ "${SOFTPC_SOURCE_DIR}/src/common/ui/ui.c" common_ui_source)
if(EXISTS "${SOFTPC_SOURCE_DIR}/src/lib/ux" OR
   app_main_source MATCHES "kvm_presenter|kvm_run" OR
   app_session_composition MATCHES "kvm_presenter|kvm_run" OR
   common_ui_source MATCHES "kvm_presenter|kvm_run")
    message(FATAL_ERROR "Standalone retains the removed unified KVM route")
endif()

# Console Broker owns only native Console I/O. Common UI owns its logical
# Console and line sink, and binds it through the public broker API.
file(READ "${SOFTPC_SOURCE_DIR}/src/lib/console-broker/console_interface.h" console_broker_public)
file(READ "${SOFTPC_SOURCE_DIR}/src/lib/console-broker/console.c" console_broker_source)
file(READ "${SOFTPC_SOURCE_DIR}/src/common/ui/ui.c" common_ui_console_source)
if(console_broker_public MATCHES "console_broker_cooked" OR
   console_broker_source MATCHES "console_broker_cooked" OR
   common_ui_console_source MATCHES "console_broker_cooked" OR
   NOT common_ui_console_source MATCHES "console_broker_replace" OR
   NOT common_ui_console_source MATCHES "console_broker_request_cooked_line")
    message(FATAL_ERROR "Console broker retains monitor-specific ownership")
endif()

# The library's verify_types_layout above owns the complete source/build DAG.
# Do not keep a second hardcoded subset here.

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

file(READ "${SOFTPC_SOURCE_DIR}/src/compat/platform.c"
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
file(READ "${SOFTPC_SOURCE_DIR}/src/lib/kvm-window/win32/component.c" window_frontend)
string(TOLOWER "${window_frontend}" normalized_window_frontend)
if(normalized_window_frontend MATCHES "ega_planes|\\bdac\\b")
    message(FATAL_ERROR "Standalone window bypasses the original SoftPC renderer")
endif()

file(READ "${SOFTPC_SOURCE_DIR}/src/lib/kvm-console/win32/component.c" console_frontend)
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
    if(owner_source MATCHES "(^|[^[:alnum:]_])softpc_[A-Za-z0-9_]+")
        message(FATAL_ERROR "Standalone application retains an unowned softpc_ symbol: ${source}")
    endif()
endforeach()

# Product provider registration has one policy-free composition boundary.
file(READ "${SOFTPC_SOURCE_DIR}/src/app/composition.c" composition_source)
if(NOT composition_source MATCHES "if \\(common_machine_shutdown\\(machine_runtime\\) != LIB_STATUS_OK\\) \\{[^}]*exit\\(EXIT_FAILURE\\);[^}]*\\}[ \t\r\n]+if \\(common_ui_destroy\\(ui\\) != LIB_STATUS_OK\\) \\{[^}]*exit\\(EXIT_FAILURE\\);[^}]*\\}[ \t\r\n]+\\(void\\)common_session_destroy\\(session\\);[ \t\r\n]+app_command_dispose\\(&commands\\);[ \t\r\n]+common_machine_destroy\\(machine_runtime\\);[ \t\r\n]+vm_destroy\\(machine_driver\\);")
    message(FATAL_ERROR "Composition must quiesce callbacks before ordered consumer teardown")
endif()
if(composition_source MATCHES "strcmp|x86_debug_|pause-toggle|send-ctrl-alt-del|send-alt-enter")
    message(FATAL_ERROR "Composition must not interpret commands, hotkeys or debugger policy")
endif()
file(GLOB app_provider_sources "${SOFTPC_SOURCE_DIR}/src/app/*.c")
foreach(source IN LISTS app_provider_sources)
    if(NOT source MATCHES "/composition\\.c$")
        file(READ "${source}" contents)
        if(contents MATCHES "\\.handle_hotkey[ \t]*=")
            message(FATAL_ERROR "Only composition may register the product hotkey provider")
        endif()
    endif()
endforeach()

# Common UI owns KVM/broker teardown; App/Compat must not add a second owner.
file(GLOB app_shutdown_sources "${SOFTPC_SOURCE_DIR}/src/app/*.c"
    "${SOFTPC_SOURCE_DIR}/src/compat/*.c")
set(checked_shutdown "kvm_(window|console)_destroy|console_broker_destroy")
foreach(source IN LISTS app_shutdown_sources)
    file(STRINGS "${source}" shutdown_lines REGEX "(${checked_shutdown})[ \t]*\\(")
    foreach(line IN LISTS shutdown_lines)
        message(FATAL_ERROR "KVM/broker teardown bypasses Common UI: ${source}: ${line}")
    endforeach()
endforeach()

# GDP/SAS vocabulary comes from the retained original headers, never a copy.
file(GLOB_RECURSE duplicate_original_headers
    "${SOFTPC_SOURCE_DIR}/src/compat/gdpvar.h"
    "${SOFTPC_SOURCE_DIR}/src/compat/sas4gen.h")
if(duplicate_original_headers OR build_definition MATCHES "compat/ccpu/legacy")
    message(FATAL_ERROR "Compat duplicates original GDP/SAS declarations")
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
