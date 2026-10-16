if(NOT DEFINED KVM_NAMING_ROOT OR NOT DEFINED KVM_NAMING_VERIFIER)
    message(FATAL_ERROR "KVM naming fixture root and verifier are required")
endif()
execute_process(
    COMMAND "${CMAKE_COMMAND}" -DKVM_NAMING_ROOT:PATH=${KVM_NAMING_ROOT}
        -P "${KVM_NAMING_VERIFIER}"
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error)
if(result EQUAL 0)
    message(FATAL_ERROR "Obsolete KVM naming fixture passed")
endif()
if(NOT "${output}${error}" MATCHES "Obsolete Lib spelling")
    message(FATAL_ERROR "KVM naming fixture failed for the wrong reason: ${output}${error}")
endif()

# Each retired spelling must fail independently, not be masked by the first.
string(CONCAT retired_console "LIB_BASE_" "CONSOLE_INTERFACE_H")
string(CONCAT retired_linux "KVM_LINK" "VM_KEY_UP")
string(CONCAT retired_linux_type "kvm_link" "vm_key")
string(CONCAT retired_owner "policy.  Ho" "st is")
set(probe "${CMAKE_CURRENT_BINARY_DIR}/retired-lib-name.h")
foreach(spelling IN ITEMS "${retired_console}" "${retired_linux}"
        "${retired_linux_type}" "${retired_owner}")
    file(WRITE "${probe}" "${spelling}\n")
    execute_process(
        COMMAND "${CMAKE_COMMAND}" -DKVM_NAMING_ROOT:PATH=${probe}
            -P "${KVM_NAMING_VERIFIER}"
        RESULT_VARIABLE result OUTPUT_VARIABLE output ERROR_VARIABLE error)
    if(result EQUAL 0 OR NOT "${output}${error}" MATCHES "Obsolete Lib spelling")
        message(FATAL_ERROR "Retired Lib spelling was not rejected: ${spelling}")
    endif()
endforeach()
file(REMOVE "${probe}")
