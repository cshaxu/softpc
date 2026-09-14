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
DECLARE_REGISTER(EIP);
DECLARE_REGISTER(EFLAGS);
DECLARE_REGISTER(CR0);
DECLARE_REGISTER(CR2);
DECLARE_REGISTER(CR3);
#undef DECLARE_REGISTER
/* The original segment setters catch CPU exceptions and return their number. */
#define DECLARE_SEGMENT(name) \
    extern unsigned short c_get##name(void); \
    extern long c_set##name(unsigned short value); \
    extern unsigned long c_get##name##_BASE(void); \
    extern unsigned long c_get##name##_LIMIT(void); \
    extern unsigned short c_get##name##_AR(void)
DECLARE_SEGMENT(ES);
DECLARE_SEGMENT(CS);
DECLARE_SEGMENT(SS);
DECLARE_SEGMENT(DS);
DECLARE_SEGMENT(FS);
DECLARE_SEGMENT(GS);
#undef DECLARE_SEGMENT
extern unsigned long c_getGDT_BASE(void), c_getIDT_BASE(void);
extern unsigned short c_getGDT_LIMIT(void), c_getIDT_LIMIT(void);
extern unsigned long c_getLDT_BASE(void), c_getLDT_LIMIT(void);
extern unsigned short c_getLDT_SELECTOR(void);
extern unsigned long c_getTR_BASE(void), c_getTR_LIMIT(void);
extern unsigned short c_getTR_SELECTOR(void), c_getTR_AR(void);
extern void inb(unsigned short port, unsigned char *value);
extern void outb(unsigned short port, unsigned char value);
extern int softpc_platform_debug_memory(unsigned long address,
    unsigned char *data, unsigned long bytes, int write);

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
    GENERAL_REGISTER(EIP);
    GENERAL_REGISTER(EFLAGS);
    GENERAL_REGISTER(CR2);
    GENERAL_REGISTER(CR3);
    case COMMON_DEBUG_CR0:
        /* MOV_CR raises #GP for PG without PE. Reject before calling the
         * setter so a debugger typo cannot escape into the guest exception. */
        if (write && (request->address & 0x80000001u) == 0x80000000u)
            return LIB_STATUS_INVALID_ARGUMENT;
        if (write) c_setCR0(request->address);
        else result->value = c_getCR0();
        return LIB_STATUS_OK;
#undef GENERAL_REGISTER
#define SEGMENT_REGISTER(name) case COMMON_DEBUG_##name: \
        if (write) return request->address <= 0xffffu && \
            c_set##name((unsigned short)request->address) == 0 ? \
                LIB_STATUS_OK : LIB_STATUS_INVALID_ARGUMENT; \
        result->value = c_get##name(); return LIB_STATUS_OK
    SEGMENT_REGISTER(ES);
    SEGMENT_REGISTER(CS);
    SEGMENT_REGISTER(SS);
    SEGMENT_REGISTER(DS);
    SEGMENT_REGISTER(FS);
    SEGMENT_REGISTER(GS);
#undef SEGMENT_REGISTER
    default: return LIB_STATUS_UNSUPPORTED;
    }
}

static common_machine_debug_segment_snapshot softpc_debug_segment(
    lib_u16 selector, lib_u32 base, lib_u32 limit, lib_u16 ar)
{
    common_machine_debug_segment_snapshot value = { 0 };
    value.selector = selector; value.base = base; value.limit = limit;
    value.dpl = (ar >> 5u) & 3u;
    value.type = ar & 15u;
    value.accessed = (ar & 1u) != 0u;
    value.executable = (ar & 8u) != 0u;
    value.conform = value.executable && (ar & 4u) != 0u;
    value.readable = value.executable && (ar & 2u) != 0u;
    value.defsize = value.executable && (ar & 0x4000u) != 0u;
    value.big = !value.executable && (ar & 0x4000u) != 0u;
    value.expdown = !value.executable && (ar & 4u) != 0u;
    value.writable = !value.executable && (ar & 2u) != 0u;
    return value;
}

lib_status softpc_machine_debug(softpc_machine *machine,
    const common_machine_debug_request *request,
    common_machine_debug_result *result)
{
    lib_u32 address;
    lib_bool write;
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
    case COMMON_MACHINE_DEBUG_GET_CPU_SNAPSHOT:
#define SNAPSHOT(lower, upper) result->cpu.lower = softpc_debug_segment( \
        c_get##upper(), c_get##upper##_BASE(), c_get##upper##_LIMIT(), c_get##upper##_AR())
        SNAPSHOT(es, ES); SNAPSHOT(cs, CS); SNAPSHOT(ss, SS);
        SNAPSHOT(ds, DS); SNAPSHOT(fs, FS); SNAPSHOT(gs, GS);
#undef SNAPSHOT
        result->cpu.tr = softpc_debug_segment(c_getTR_SELECTOR(),
            c_getTR_BASE(), c_getTR_LIMIT(), c_getTR_AR());
        /* LDT cache exposes only selector/base/limit, not descriptor AR. */
        result->cpu.ldtr.selector = c_getLDT_SELECTOR();
        result->cpu.ldtr.base = c_getLDT_BASE();
        result->cpu.ldtr.limit = c_getLDT_LIMIT();
        result->cpu.gdtr.base = c_getGDT_BASE();
        result->cpu.gdtr.limit = c_getGDT_LIMIT();
        result->cpu.idtr.base = c_getIDT_BASE();
        result->cpu.idtr.limit = c_getIDT_LIMIT();
        result->cpu.cr0 = c_getCR0(); result->cpu.cr2 = c_getCR2(); result->cpu.cr3 = c_getCR3();
        return LIB_STATUS_OK;
    case COMMON_MACHINE_DEBUG_READ_PORT:
        if (request->bytes != 1u) return LIB_STATUS_INVALID_ARGUMENT;
        inb(request->port, result->data);
        result->value = result->data[0]; result->bytes = 1u;
        return LIB_STATUS_OK;
    case COMMON_MACHINE_DEBUG_WRITE_PORT:
        if (request->bytes != 1u || request->address > 0xffu)
            return LIB_STATUS_INVALID_ARGUMENT;
        outb(request->port, (unsigned char)request->address);
        return LIB_STATUS_OK;
    case COMMON_MACHINE_DEBUG_READ_REAL:
    case COMMON_MACHINE_DEBUG_WRITE_REAL:
    case COMMON_MACHINE_DEBUG_READ_LINEAR:
    case COMMON_MACHINE_DEBUG_WRITE_LINEAR:
        address = request->operation == COMMON_MACHINE_DEBUG_READ_REAL ||
            request->operation == COMMON_MACHINE_DEBUG_WRITE_REAL ?
            ((lib_u32)request->segment << 4u) + request->offset : request->address;
        write = request->operation == COMMON_MACHINE_DEBUG_WRITE_REAL ||
            request->operation == COMMON_MACHINE_DEBUG_WRITE_LINEAR;
        if (write) memcpy(result->data, request->data, request->bytes);
        if (!softpc_platform_debug_memory(address, result->data, request->bytes, write))
            return LIB_STATUS_INVALID_ARGUMENT;
        result->bytes = request->bytes;
        return LIB_STATUS_OK;
    case COMMON_MACHINE_DEBUG_CLEAR_EXECUTION_PLAN:
        /* No plan can be installed by this adapter. Plain G still resumes. */
        return LIB_STATUS_OK;
    default:
        return LIB_STATUS_UNSUPPORTED;
    }
}
