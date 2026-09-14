#include "host/debug.h"
#include "common/debug/debug_interface.h"

#include <string.h>

/* Match the recovered CCPU ABI (IU32 is unsigned long on Windows). No CPU
 * state is duplicated here; these are the original register accessors. */
#define DECLARE_REGISTER(name) \
    extern unsigned long c_get##name(void); \
    extern void c_set##name(unsigned long value)
DECLARE_REGISTER(EAX);
DECLARE_REGISTER(ECX);
DECLARE_REGISTER(EDX);
DECLARE_REGISTER(EBX);
DECLARE_REGISTER(ESP);
DECLARE_REGISTER(EBP);
DECLARE_REGISTER(ESI);
DECLARE_REGISTER(EDI);
extern unsigned long c_getEIP(void);
extern unsigned long c_getEFLAGS(void);
extern unsigned long c_getCR0(void);
extern unsigned long c_getCR2(void);
extern unsigned long c_getCR3(void);
extern unsigned long c_getCS_BASE(void);
extern unsigned short c_getCS_AR(void);
extern unsigned short c_getES(void);
extern unsigned short c_getCS(void);
extern unsigned short c_getSS(void);
extern unsigned short c_getDS(void);
extern unsigned short c_getFS(void);
extern unsigned short c_getGS(void);

static lib_status softpc_debug_register(const common_machine_debug_request *request,
    common_machine_debug_result *result)
{
    lib_bool write = request->operation == COMMON_MACHINE_DEBUG_WRITE_REGISTER;
    switch (request->register_id) {
#define GENERAL_REGISTER(name) case COMMON_DEBUG_##name: \
        if (write) c_set##name(request->address); \
        else result->value = c_get##name(); \
        return LIB_STATUS_OK
    GENERAL_REGISTER(EAX);
    GENERAL_REGISTER(ECX);
    GENERAL_REGISTER(EDX);
    GENERAL_REGISTER(EBX);
    GENERAL_REGISTER(ESP);
    GENERAL_REGISTER(EBP);
    GENERAL_REGISTER(ESI);
    GENERAL_REGISTER(EDI);
#undef GENERAL_REGISTER
#define READ_REGISTER(name) case COMMON_DEBUG_##name: \
        if (write) return LIB_STATUS_UNSUPPORTED; \
        result->value = c_get##name(); return LIB_STATUS_OK
    READ_REGISTER(EIP);
    READ_REGISTER(EFLAGS);
    READ_REGISTER(ES);
    READ_REGISTER(CS);
    READ_REGISTER(SS);
    READ_REGISTER(DS);
    READ_REGISTER(FS);
    READ_REGISTER(GS);
    READ_REGISTER(CR0);
    READ_REGISTER(CR2);
    READ_REGISTER(CR3);
#undef READ_REGISTER
    default: return LIB_STATUS_UNSUPPORTED;
    }
}

lib_status softpc_machine_debug(softpc_machine *machine,
    const common_machine_debug_request *request,
    common_machine_debug_result *result)
{
    lib_u32 address;
    softpc_machine_result status;
    if (machine == NULL || request == NULL || result == NULL ||
        request->bytes > COMMON_MACHINE_DEBUG_BYTES)
        return LIB_STATUS_INVALID_ARGUMENT;
    memset(result, 0, sizeof(*result));
    switch (request->operation) {
    case COMMON_MACHINE_DEBUG_READ_REGISTER:
    case COMMON_MACHINE_DEBUG_WRITE_REGISTER:
        return softpc_debug_register(request, result);
    case COMMON_MACHINE_DEBUG_GET_CODE_BASE:
        result->value = c_getCS_BASE();
        return LIB_STATUS_OK;
    case COMMON_MACHINE_DEBUG_GET_CODE_DEFAULT_SIZE:
        result->value = (c_getCS_AR() & 0x4000u) != 0u;
        return LIB_STATUS_OK;
    case COMMON_MACHINE_DEBUG_READ_REAL:
    case COMMON_MACHINE_DEBUG_WRITE_REAL:
    case COMMON_MACHINE_DEBUG_READ_LINEAR:
    case COMMON_MACHINE_DEBUG_WRITE_LINEAR:
        /* A physical RAM copy is a linear copy only with paging disabled.
         * Do not invent page translation or bypass page/device semantics. */
        if ((c_getCR0() & 0x80000000ul) != 0u) return LIB_STATUS_UNSUPPORTED;
        address = request->operation == COMMON_MACHINE_DEBUG_READ_REAL ||
            request->operation == COMMON_MACHINE_DEBUG_WRITE_REAL ?
            ((lib_u32)request->segment << 4u) + request->offset : request->address;
        if (request->operation == COMMON_MACHINE_DEBUG_READ_REAL ||
            request->operation == COMMON_MACHINE_DEBUG_READ_LINEAR)
            status = softpc_machine_read_physical(machine, address,
                result->data, request->bytes);
        else
            status = softpc_machine_write_physical(machine, address,
                request->data, request->bytes);
        result->bytes = status == SOFTPC_MACHINE_OK ? request->bytes : 0u;
        return status == SOFTPC_MACHINE_OK ? LIB_STATUS_OK : LIB_STATUS_INVALID_ARGUMENT;
    case COMMON_MACHINE_DEBUG_CLEAR_EXECUTION_PLAN:
        /* No plan can be installed by this adapter. Plain G still resumes. */
        return LIB_STATUS_OK;
    default:
        return LIB_STATUS_UNSUPPORTED;
    }
}
