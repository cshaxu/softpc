# Repository-local documentation topology gate.  It intentionally validates
# structure and authority links, not prose style or product behaviour.

if(NOT DEFINED SOFTPC_SOURCE_DIR)
    message(FATAL_ERROR "SOFTPC_SOURCE_DIR must name the repository root")
endif()

file(REAL_PATH "${SOFTPC_SOURCE_DIR}" SOFTPC_SOURCE_DIR)

function(softpc_require_file relative_path)
    set(full_path "${SOFTPC_SOURCE_DIR}/${relative_path}")
    if(NOT EXISTS "${full_path}")
        message(FATAL_ERROR "Documentation governance: missing ${relative_path}")
    endif()
endfunction()

function(softpc_require_text relative_path required_text)
    set(full_path "${SOFTPC_SOURCE_DIR}/${relative_path}")
    file(READ "${full_path}" contents)
    string(FIND "${contents}" "${required_text}" found_at)
    if(found_at EQUAL -1)
        message(FATAL_ERROR
            "Documentation governance: ${relative_path} must contain '${required_text}'")
    endif()
endfunction()

foreach(required_directory IN ITEMS
    docs/design docs/rules docs/states docs/proposals docs/history docs/etc)
    if(NOT IS_DIRECTORY "${SOFTPC_SOURCE_DIR}/${required_directory}")
        message(FATAL_ERROR "Documentation governance: missing ${required_directory}/")
    endif()
endforeach()

foreach(required_file IN ITEMS
    AGENTS.md CONTRIBUTING.md docs/README.md docs/etc/README.md
    docs/design/GOAL.md docs/design/ARCHITECTURE.md docs/design/CODING.md
    docs/design/UI.md docs/design/ROADMAP.md
    docs/rules/DOCUMENT.md docs/rules/EXECUTION.md
    docs/rules/ARCHITECTURE.md docs/rules/CODING.md
    docs/states/CURRENT.md docs/states/QUEUE.md docs/states/TODO.md)
    softpc_require_file("${required_file}")
endforeach()

softpc_require_text("docs/README.md" "states/CURRENT.md")
softpc_require_text("docs/README.md" "rules/EXECUTION.md")
softpc_require_text("docs/etc/README.md" "Supporting Material Index")
softpc_require_text("docs/rules/EXECUTION.md" "strictly increasing implementation-task number")
softpc_require_text("docs/rules/EXECUTION.md" "M8 T13")
softpc_require_text("AGENTS.md" "build version")
softpc_require_text("docs/states/CURRENT.md" "# Project Status")

file(GLOB top_level_documents LIST_DIRECTORIES false
    "${SOFTPC_SOURCE_DIR}/docs/*.md")
foreach(document_path IN LISTS top_level_documents)
    get_filename_component(document_name "${document_path}" NAME)
    if(NOT document_name STREQUAL "README.md")
        message(FATAL_ERROR
            "Documentation governance: docs/${document_name} must be placed in its authority directory")
    endif()
endforeach()

message(STATUS "SoftPC documentation governance topology is valid")
