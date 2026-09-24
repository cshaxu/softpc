#include "lib/types/types_interface.h"
#include "machine/debug.h"
#include "compat/ccpu/abi.h"
#include "compat/platform.h"
#include "x86/debug/protocol_interface.h"


extern void inb(unsigned short port, unsigned char *value);
extern void outb(unsigned short port, unsigned char value);

static lib_status softpc_debug_register(const x86_debug_request *request,
    x86_debug_response *result)
{
    lib_bool write = request->operation == X86_DEBUG_WRITE_REGISTER;
    switch (request->register_id) {
#define GENERAL_REGISTER(name) case X86_DEBUG_##name: \
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
    case X86_DEBUG_CR0:
        /* MOV_CR raises #GP for PG without PE. Reject before calling the
         * setter so a debugger typo cannot escape into the guest exception. */
        if (write && (request->address & 0x80000001u) == 0x80000000u)
            return LIB_STATUS_INVALID_ARGUMENT;
        if (write) c_setCR0(request->address);
        else result->value = c_getCR0();
        return LIB_STATUS_OK;
#undef GENERAL_REGISTER
#define SEGMENT_REGISTER(name) case X86_DEBUG_##name: \
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

static x86_debug_segment_snapshot softpc_debug_segment(
    lib_u16 selector, lib_u32 base, lib_u32 limit, lib_u16 ar)
{
    x86_debug_segment_snapshot value = { 0 };
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

#if defined(_MSC_VER)
static __declspec(thread) softpc_debug_state *active_debug;
#else
static __thread softpc_debug_state *active_debug;
#endif


void softpc_debug_bind(softpc_debug_state *state)
{
    active_debug = state;
}

/* Stop only before an instruction, after original inter-instruction work.
 * The existing executor rendezvous parks this same thread. CCPU must refetch
 * on return because paused debug access may have changed CS:EIP or memory. */
int softpc_host_debug_begin(void)
{
    softpc_debug_state *state = active_debug;
    lib_u32 address;
    lib_bool skip;
    if (state == NULL) return 0;
    if (state->kind == X86_DEBUG_EXECUTION_NONE && !state->stop_pending &&
        !state->watches[0].enabled && !state->watches[1].enabled && !state->watches[2].enabled) {
        state->in_instruction = LIB_FALSE;
        return 0;
    }
    address = c_getCS_BASE() + c_getEIP();
    skip = state->skip_first && state->stopped_address == address;
    state->skip_first = LIB_FALSE;
    if (!state->stop_pending) {
        state->observation = (x86_debug_observation) { 0 };
        state->result_ready = LIB_FALSE;
        if (!skip && state->watches[X86_DEBUG_WATCH_EXECUTE].enabled &&
            state->watches[X86_DEBUG_WATCH_EXECUTE].address == address) {
            state->observation.watch_hit = LIB_TRUE;
            state->observation.watch_kind = X86_DEBUG_WATCH_EXECUTE;
            state->observation.watch_address = address;
        } else if (skip || state->kind < X86_DEBUG_EXECUTION_BREAK_REAL ||
            address != state->address) {
            state->in_instruction = LIB_TRUE;
            return 0;
        }
    }
    state->in_instruction = LIB_FALSE;
    state->stopped_address = address;
    state->skip_first = LIB_TRUE;
    state->result_ready = LIB_TRUE;
    state->stop_pending = LIB_TRUE;
    state->kind = X86_DEBUG_EXECUTION_NONE;
    softpc_platform_executor_event();
    return 1;
}

void softpc_host_debug_retired(void)
{
    softpc_debug_state *state = active_debug;
    if (state == NULL) return;
    state->in_instruction = LIB_FALSE;
    if (state->kind == X86_DEBUG_EXECUTION_NONE &&
        !state->watches[0].enabled && !state->watches[1].enabled && !state->watches[2].enabled) return;
    if (state->executed != UINT32_MAX) ++state->executed;
    if (state->kind == X86_DEBUG_EXECUTION_TRACE &&
        state->executed >= state->target_count) state->stop_pending = LIB_TRUE;
    if (state->observation.watch_hit) state->stop_pending = LIB_TRUE;
}

/* CCPU copied operand notification. Never read memory again for tracing. */
void softpc_host_debug_access(unsigned long address, unsigned long bytes,
    int write, const unsigned char *data, int reversed)
{
    softpc_debug_state *state = active_debug;
    x86_debug_observation *record;
    x86_debug_watch_kind kind = write ? X86_DEBUG_WATCH_WRITE :
        X86_DEBUG_WATCH_READ;
    lib_u32 index;
    lib_u64 value = 0;
    if (state == NULL || !state->in_instruction || bytes == 0u) return;
    record = &state->observation;
    if (state->watches[kind].enabled &&
        (lib_u32)(state->watches[kind].address - address) < bytes && !record->watch_hit) {
        record->watch_hit = LIB_TRUE;
        record->watch_kind = kind;
        record->watch_address = state->watches[kind].address;
    }
    if (state->kind != X86_DEBUG_EXECUTION_TRACE && !record->watch_hit) return;
    if (record->count == X86_DEBUG_ACCESS_CAPACITY) {
        record->truncated = LIB_TRUE;
        return;
    }
    for (index = 0u; index < bytes && index < 8u; ++index)
        value |= (lib_u64)data[reversed ? bytes - 1u - index : index] << (index * 8u);
    record->accesses[record->count++] = (x86_debug_memory_access) {
        .write = write != 0, .linear = address, .bytes = bytes, .data = value };
}

unsigned long softpc_host_debug_read(unsigned long address, unsigned long bytes,
    unsigned long data)
{
    softpc_host_debug_access(address, bytes, 0, (const unsigned char *)&data, 0);
    return data;
}

lib_status softpc_machine_debug(softpc_machine *machine, softpc_debug_state *state,
    const x86_debug_request *request,
    x86_debug_response *result)
{
    lib_u32 address;
    lib_bool write;
    if (machine == NULL || state == NULL || request == NULL || result == NULL ||
        request->bytes > X86_DEBUG_BYTES)
        return LIB_STATUS_INVALID_ARGUMENT;
    lib_memory_set(result, 0, sizeof(*result));
    switch (request->operation) {
    case X86_DEBUG_READ_REGISTER:
    case X86_DEBUG_WRITE_REGISTER:
        return softpc_debug_register(request, result);
    case X86_DEBUG_GET_CODE_BASE:
        result->value = c_getCS_BASE();
        return LIB_STATUS_OK;
    case X86_DEBUG_GET_CODE_DEFAULT_SIZE:
        result->value = (c_getCS_AR() & 0x4000u) != 0u;
        return LIB_STATUS_OK;
    case X86_DEBUG_GET_CPU_SNAPSHOT:
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
    case X86_DEBUG_READ_PORT:
        if (request->bytes != 1u) return LIB_STATUS_INVALID_ARGUMENT;
        inb(request->port, result->data);
        result->value = result->data[0]; result->bytes = 1u;
        return LIB_STATUS_OK;
    case X86_DEBUG_WRITE_PORT:
        if (request->bytes != 1u || request->address > 0xffu)
            return LIB_STATUS_INVALID_ARGUMENT;
        outb(request->port, (unsigned char)request->address);
        return LIB_STATUS_OK;
    case X86_DEBUG_READ_REAL:
    case X86_DEBUG_WRITE_REAL:
    case X86_DEBUG_READ_LINEAR:
    case X86_DEBUG_WRITE_LINEAR:
        address = request->operation == X86_DEBUG_READ_REAL ||
            request->operation == X86_DEBUG_WRITE_REAL ?
            ((lib_u32)request->segment << 4u) + request->offset : request->address;
        write = request->operation == X86_DEBUG_WRITE_REAL ||
            request->operation == X86_DEBUG_WRITE_LINEAR;
        if (write) lib_memory_copy(result->data, request->data, request->bytes);
        if (!softpc_machine_debug_memory(address, result->data, request->bytes, write))
            return LIB_STATUS_INVALID_ARGUMENT;
        result->bytes = request->bytes;
        return LIB_STATUS_OK;
    case X86_DEBUG_CLEAR_EXECUTION_PLAN:
        state->kind = X86_DEBUG_EXECUTION_NONE;
        state->executed = 0u;
        state->stop_pending = state->result_ready = LIB_FALSE;
        state->observation = (x86_debug_observation) { 0 };
        return LIB_STATUS_OK;
    case X86_DEBUG_SET_EXECUTION_PLAN:
        if (request->execution_kind < X86_DEBUG_EXECUTION_TRACE ||
            request->execution_kind > X86_DEBUG_EXECUTION_BREAK_LINEAR ||
            (request->execution_kind == X86_DEBUG_EXECUTION_TRACE &&
                (request->instruction_count == 0u || request->instruction_count > UINT32_MAX)))
            return LIB_STATUS_INVALID_ARGUMENT;
        address = request->execution_kind == X86_DEBUG_EXECUTION_BREAK_REAL ?
            ((lib_u32)request->segment << 4u) + request->offset : request->address;
        write = (state->result_ready || state->skip_first) &&
            state->stopped_address == c_getCS_BASE() + c_getEIP();
        state->kind = request->execution_kind;
        state->target_count = request->instruction_count;
        state->address = address;
        state->executed = 0u;
        state->stop_pending = state->result_ready = LIB_FALSE;
        state->skip_first = write;
        state->observation = (x86_debug_observation) { 0 };
        return LIB_STATUS_OK;
    case X86_DEBUG_GET_EXECUTION_RESULT:
        result->enabled = state->result_ready;
        result->value = state->executed;
        result->observation = state->observation;
        return LIB_STATUS_OK;
    case X86_DEBUG_SET_WATCH:
    case X86_DEBUG_CLEAR_WATCH:
    case X86_DEBUG_GET_WATCH:
        if ((unsigned)request->watch_kind > X86_DEBUG_WATCH_EXECUTE)
            return LIB_STATUS_INVALID_ARGUMENT;
        if (request->operation == X86_DEBUG_SET_WATCH) {
            state->watches[request->watch_kind].address = request->address;
            state->watches[request->watch_kind].enabled = LIB_TRUE;
        } else if (request->operation == X86_DEBUG_CLEAR_WATCH)
            state->watches[request->watch_kind].enabled = LIB_FALSE;
        result->enabled = state->watches[request->watch_kind].enabled;
        result->value = state->watches[request->watch_kind].address;
        return LIB_STATUS_OK;
    default:
        return LIB_STATUS_UNSUPPORTED;
    }
}
