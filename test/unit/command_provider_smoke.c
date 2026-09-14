#include "app/composition.h"
#include "vm/driver.h"

#include <windows.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

typedef struct completions {
    HANDLE paused;
    HANDLE running;
    HANDLE stopped;
} completions;

static softpc_machine *observed_product;
static HANDLE program_completed;
static lib_bool (*copy_product_frame)(void *, kvm_frame *);

/* Observe a disposable program's result on the executor, never race a host
 * RAM read against execution. This barrier proves changed CS:EIP was used. */
static lib_bool observe_program(void *context, kvm_frame *frame)
{
    lib_u32 marker = 0u;
    lib_bool copied = copy_product_frame(context, frame);
    if (softpc_machine_read_physical(observed_product, 0x600u, &marker, sizeof(marker)) ==
            SOFTPC_MACHINE_OK && marker == 0x56781234u)
        SetEvent(program_completed);
    return copied;
}

static void note_state(void *opaque, common_machine_state state, lib_u32 generation)
{
    completions *events = opaque;
    (void)generation;
    fprintf(stderr, "debug probe completion: %d\n", (int)state);
    if (state == COMMON_MACHINE_PAUSED || state == COMMON_MACHINE_RESET_COMPLETED)
        SetEvent(events->paused);
    if (state == COMMON_MACHINE_RUNNING) SetEvent(events->running);
    if (state == COMMON_MACHINE_STOPPED) SetEvent(events->stopped);
}

static void wait_for(HANDLE event)
{
    assert(WaitForSingleObject(event, 10000u) == WAIT_OBJECT_0);
    ResetEvent(event);
}

static void submit(common_session_command_provider *provider,
    common_session_machine_state state, const char *line,
    common_session_command_result *result)
{
    fprintf(stderr, "debug probe command: %s\n", line);
    provider->submit_line(provider->context, state, line, result);
    assert(!result->exit_requested);
}

static common_machine_debug_result access(common_machine *machine,
    const common_machine_debug_lease *lease, common_machine_debug_request request)
{
    common_machine_debug_result result;
    assert(common_machine_debug_execute_with_lease(machine, lease, &request, &result) == LIB_STATUS_OK);
    return result;
}

static lib_u32 reg(common_machine *machine, const common_machine_debug_lease *lease,
    lib_u32 id)
{
    return access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_READ_REGISTER, .register_id = id }).value;
}

static void setreg(common_machine *machine, const common_machine_debug_lease *lease,
    lib_u32 id, lib_u32 value)
{
    (void)access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_WRITE_REGISTER, .register_id = id, .address = value });
}

static void memory_word(common_machine *machine, const common_machine_debug_lease *lease,
    lib_u32 address, lib_u32 value)
{
    common_machine_debug_request request = {
        .operation = COMMON_MACHINE_DEBUG_WRITE_LINEAR, .address = address, .bytes = 4u };
    memcpy(request.data, &value, 4u);
    (void)access(machine, lease, request);
}

static void synchronous_access(common_machine *machine, const common_machine_debug_lease *lease)
{
    common_machine_debug_result before = access(machine, lease,
        (common_machine_debug_request){ .operation = COMMON_MACHINE_DEBUG_GET_CPU_SNAPSHOT });
    common_machine_debug_result after;
    const lib_u32 ids[] = { COMMON_DEBUG_EIP, COMMON_DEBUG_EFLAGS, COMMON_DEBUG_CS, COMMON_DEBUG_SS, COMMON_DEBUG_DS,
        COMMON_DEBUG_ES, COMMON_DEBUG_FS, COMMON_DEBUG_GS, COMMON_DEBUG_CR2, COMMON_DEBUG_CR3 };
    size_t index;
    lib_u32 old, cr0 = before.cpu.cr0, cr3 = before.cpu.cr3;
    for (index = 0; index < sizeof(ids) / sizeof(ids[0]); ++index) {
        old = reg(machine, lease, ids[index]);
        setreg(machine, lease, ids[index], old);
        assert(reg(machine, lease, ids[index]) == old);
    }
    after = access(machine, lease,
        (common_machine_debug_request){ .operation = COMMON_MACHINE_DEBUG_GET_CPU_SNAPSHOT });
    assert(memcmp(&before.cpu, &after.cpu, sizeof(before.cpu)) == 0);
    old = reg(machine, lease, COMMON_DEBUG_DS);
    setreg(machine, lease, COMMON_DEBUG_DS, 0x1234u);
    after = access(machine, lease,
        (common_machine_debug_request){ .operation = COMMON_MACHINE_DEBUG_GET_CPU_SNAPSHOT });
    assert(after.cpu.ds.selector == 0x1234u && after.cpu.ds.base == 0x12340u);
    setreg(machine, lease, COMMON_DEBUG_DS, old);
    assert(common_machine_debug_execute_with_lease(machine, lease,
        &(common_machine_debug_request){ .operation = COMMON_MACHINE_DEBUG_WRITE_REGISTER,
            .register_id = COMMON_DEBUG_CR0, .address = 0x80000000u }, &after) == LIB_STATUS_INVALID_ARGUMENT);
    assert(reg(machine, lease, COMMON_DEBUG_CR0) == cr0);
    old = reg(machine, lease, COMMON_DEBUG_DS);
    setreg(machine, lease, COMMON_DEBUG_CR0, cr0 | 1u);
    assert(common_machine_debug_execute_with_lease(machine, lease,
        &(common_machine_debug_request){ .operation = COMMON_MACHINE_DEBUG_WRITE_REGISTER,
            .register_id = COMMON_DEBUG_DS, .address = 0xfff8u }, &after) == LIB_STATUS_INVALID_ARGUMENT);
    assert(reg(machine, lease, COMMON_DEBUG_DS) == old);
    setreg(machine, lease, COMMON_DEBUG_CR0, cr0);
    old = access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_READ_PORT, .port = 0x21u, .bytes = 1u }).value;
    (void)access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_WRITE_PORT, .port = 0x21u, .bytes = 1u, .address = 0x5au });
    assert(access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_READ_PORT, .port = 0x21u, .bytes = 1u }).value == 0x5au);
    (void)access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_WRITE_PORT, .port = 0x21u, .bytes = 1u, .address = old });
    assert(common_machine_debug_execute_with_lease(machine, lease,
        &(common_machine_debug_request){ .operation = COMMON_MACHINE_DEBUG_READ_PORT,
            .port = 0x21u, .bytes = 2u }, &after) == LIB_STATUS_INVALID_ARGUMENT);
    /* Disposable page directory/table; all accesses remain on the paused executor. */
    memory_word(machine, lease, 0x10000u, 0x11003u);
    memory_word(machine, lease, 0x11000u + 2u * 4u, 0x30003u);
    memory_word(machine, lease, 0x11000u + 3u * 4u, 0x32003u);
    memory_word(machine, lease, 0x11000u + 4u * 4u, 0u);
    memory_word(machine, lease, 0x30ffcu, 0x11223344u);
    memory_word(machine, lease, 0x32000u, 0x55667788u);
    memory_word(machine, lease, 0x32ffcu, 0x12345678u);
    setreg(machine, lease, COMMON_DEBUG_CR3, 0x10000u);
    setreg(machine, lease, COMMON_DEBUG_CR0, cr0 | 0x80000001u);
    after = access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_READ_LINEAR, .address = 0x2ffcu, .bytes = 8u });
    assert(after.data[0] == 0x44u && after.data[4] == 0x88u);
    memory_word(machine, lease, 0x3000u, 0xaabbccddu);
    assert(common_machine_debug_execute_with_lease(machine, lease,
        &(common_machine_debug_request){ .operation = COMMON_MACHINE_DEBUG_READ_LINEAR,
            .address = 0x4000u, .bytes = 1u }, &after) == LIB_STATUS_INVALID_ARGUMENT);
    assert(reg(machine, lease, COMMON_DEBUG_CR2) == before.cpu.cr2);
    assert(common_machine_debug_execute_with_lease(machine, lease,
        &(common_machine_debug_request){ .operation = COMMON_MACHINE_DEBUG_WRITE_LINEAR,
            .address = 0x3ffcu, .bytes = 8u }, &after) == LIB_STATUS_INVALID_ARGUMENT);
    setreg(machine, lease, COMMON_DEBUG_CR0, cr0);
    setreg(machine, lease, COMMON_DEBUG_CR3, cr3);
    after = access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_READ_LINEAR, .address = 0x32000u, .bytes = 4u });
    assert(after.data[0] == 0xddu && after.data[3] == 0xaau);
    after = access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_READ_LINEAR, .address = 0x32ffcu, .bytes = 4u });
    assert(after.data[0] == 0x78u && after.data[3] == 0x12u); /* No partial write. */
    after = access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_READ_LINEAR, .address = 0x10000u, .bytes = 4u });
    assert(after.data[0] == 3u); /* No accessed/dirty mutation by inspection. */
    assert(common_machine_debug_execute_with_lease(machine, lease,
        &(common_machine_debug_request){ .operation = COMMON_MACHINE_DEBUG_WRITE_REAL,
            .segment = 0xf000u, .offset = 0xfff0u, .bytes = 1u }, &after) == LIB_STATUS_INVALID_ARGUMENT);
}

static void run_plan(common_machine *machine, common_machine_debug_lease *lease,
    completions *events, common_machine_debug_request request, lib_u32 expected_ip,
    lib_u32 expected_count)
{
    common_machine_debug_result result;
    (void)access(machine, lease, request);
    assert(common_machine_resume(machine));
    wait_for(events->running);
    wait_for(events->paused);
    assert(common_machine_debug_acquire(machine, lease) == LIB_STATUS_OK);
    result = access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_GET_EXECUTION_RESULT });
    fprintf(stderr, "plan completed IP=%lx count=%lu ready=%d\n",
        (unsigned long)reg(machine, lease, COMMON_DEBUG_EIP),
        (unsigned long)result.value, result.enabled);
    assert(result.enabled && result.value == expected_count);
    assert(reg(machine, lease, COMMON_DEBUG_EIP) == expected_ip);
}

static void execution_plans(common_machine *machine, common_machine_debug_lease *lease,
    completions *events)
{
    common_machine_debug_request trace = { .operation = COMMON_MACHINE_DEBUG_SET_EXECUTION_PLAN,
        .execution_kind = COMMON_MACHINE_DEBUG_EXECUTION_TRACE, .instruction_count = 1u };
    lib_u32 flags = reg(machine, lease, COMMON_DEBUG_EFLAGS);
    lib_u32 index;
    setreg(machine, lease, COMMON_DEBUG_EFLAGS, 2u);
    setreg(machine, lease, COMMON_DEBUG_CS, 0u);
    setreg(machine, lease, COMMON_DEBUG_SS, 0u);
    setreg(machine, lease, COMMON_DEBUG_ESP, 0x900u);
    setreg(machine, lease, COMMON_DEBUG_EIP, 0x500u);
    (void)access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_WRITE_LINEAR, .address = 0x500u, .bytes = 7u,
        .data = { 0xb8,1,0,0x40,0x40,0xeb,0xfe } });
    trace.instruction_count = 3u;
    run_plan(machine, lease, events, trace, 0x505u, 3u);
    assert(reg(machine, lease, COMMON_DEBUG_EAX) == 3u);
    /* Every interrupt-shadow bypass is one retirement, not zero or two. */
    setreg(machine, lease, COMMON_DEBUG_EAX, 0u);
    setreg(machine, lease, COMMON_DEBUG_EIP, 0x500u);
    (void)access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_WRITE_LINEAR, .address = 0x500u, .bytes = 7u,
        .data = { 0x8e,0xd0,0xfb,0xfa,0x16,0x17,0x90 } });
    trace.instruction_count = 1u;
    for (index = 0x502u; index <= 0x507u; ++index)
        run_plan(machine, lease, events, trace, index, 1u);
    /* IRET retires once; stack/control transfer remains original CPU work. */
    memory_word(machine, lease, 0x900u, 0x580u);
    memory_word(machine, lease, 0x904u, 2u);
    setreg(machine, lease, COMMON_DEBUG_EIP, 0x570u);
    (void)access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_WRITE_LINEAR, .address = 0x570u, .bytes = 1u,
        .data = { 0xcf } });
    run_plan(machine, lease, events, trace, 0x580u, 1u);
    /* REP is one completed instruction even when it writes four bytes. */
    setreg(machine, lease, COMMON_DEBUG_ECX, 4u);
    setreg(machine, lease, COMMON_DEBUG_EDI, 0xa00u);
    setreg(machine, lease, COMMON_DEBUG_ES, 0u);
    (void)access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_WRITE_LINEAR, .address = 0x580u, .bytes = 2u,
        .data = { 0xf3,0xaa } });
    run_plan(machine, lease, events, trace, 0x582u, 1u);
    assert(reg(machine, lease, COMMON_DEBUG_ECX) == 0u);
    /* A fault is not a retirement. Count the handler's first NOP instead. */
    memory_word(machine, lease, 6u * 4u, 0x700u);
    setreg(machine, lease, COMMON_DEBUG_ESP, 0x900u);
    setreg(machine, lease, COMMON_DEBUG_EIP, 0x580u);
    (void)access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_WRITE_LINEAR, .address = 0x580u, .bytes = 2u,
        .data = { 0x0f,0xff } });
    (void)access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_WRITE_LINEAR, .address = 0x700u, .bytes = 2u,
        .data = { 0x90,0x90 } });
    run_plan(machine, lease, events, trace, 0x701u, 1u);
    /* Guest TF still delivers its own INT 1 before the next instruction. */
    memory_word(machine, lease, 4u, 0x700u);
    setreg(machine, lease, COMMON_DEBUG_EIP, 0x701u);
    setreg(machine, lease, COMMON_DEBUG_EFLAGS, 0x102u);
    run_plan(machine, lease, events, trace, 0x700u, 1u);
    setreg(machine, lease, COMMON_DEBUG_EFLAGS, 2u);
    /* A breakpoint stops before its instruction; rearming progresses first. */
    setreg(machine, lease, COMMON_DEBUG_EIP, 0x500u);
    setreg(machine, lease, COMMON_DEBUG_EAX, 0u);
    (void)access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_WRITE_LINEAR, .address = 0x500u, .bytes = 3u,
        .data = { 0x40,0xeb,0xfd } });
    trace.execution_kind = COMMON_MACHINE_DEBUG_EXECUTION_BREAK_LINEAR;
    trace.address = 0x501u;
    run_plan(machine, lease, events, trace, 0x501u, 1u);
    trace.execution_kind = COMMON_MACHINE_DEBUG_EXECUTION_BREAK_REAL;
    trace.segment = 0u;
    trace.offset = 0x501u;
    run_plan(machine, lease, events, trace, 0x501u, 2u);
    assert(reg(machine, lease, COMMON_DEBUG_EAX) == 2u);
    /* User cancellation cannot later turn an ordinary pause into a debug hit. */
    trace.execution_kind = COMMON_MACHINE_DEBUG_EXECUTION_BREAK_LINEAR;
    trace.address = 0x800u;
    (void)access(machine, lease, trace);
    assert(common_machine_resume(machine));
    wait_for(events->running);
    common_machine_debug_cancel(machine);
    assert(common_machine_pause(machine));
    wait_for(events->paused);
    assert(common_machine_debug_acquire(machine, lease) == LIB_STATUS_OK);
    assert(!access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_GET_EXECUTION_RESULT }).enabled);
    (void)access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_CLEAR_EXECUTION_PLAN });
    setreg(machine, lease, COMMON_DEBUG_EFLAGS, flags);
}

static void command_matrix(common_machine *machine, common_machine_debug_lease *lease,
    common_session_command_provider *provider)
{
    common_session_command_result result;
    common_machine_debug_result bytes;
    const struct { const char *line, *contains; } commands[] = {
        {"e 0:b00 12 34", ""}, {"f 0:b02 b03 56", ""},
        {"m 0:b00 b03 0:b10", ""}, {"c 0:b00 b03 0:b10", ""},
        {"s 0:b10 b13 12 34", "0000:0B10"}, {"d 0:b10 b13", "12 34 56 56"},
        {"xe b20 90 90", ""}, {"xf b22 2 cc", ""}, {"xd b20 4", "90 90 CC CC"},
        {"xm b20 b30 4", ""}, {"xc b20 b30 4", ""},
        {"xs b30 4 90 90", "00000B30"}, {"xd b30 4", "90 90 CC CC"},
        {"xa b40", ""}, {"nop", ""}, {"", ""},
        {"xu b40 1", "NOP"}, {"xreg", "AX="}, {"xsreg", "CS"}, {"xcreg", "CR0"},
        {"h 1 2", "0003"}, {"v", ""}, {"AB", "41 42"},
        {"xw r b00", ""}, {"xw", "Watch-read"}, {"xw r", "removed"},
        {"xw w b00", ""}, {"xw w", "removed"}, {"xw e b40", ""}, {"xw e", "removed"},
        {"n debug-cli-transfer.bin", ""}, {"w 0:b00", "Writing"},
        {"l 0:b50", ""}, {"d 0:b50 b53", "12 34 56 56"}
    };
    unsigned i;
    setreg(machine, lease, COMMON_DEBUG_EBX, 0u);
    setreg(machine, lease, COMMON_DEBUG_ECX, 4u);
    for (i = 0; i < sizeof(commands)/sizeof(commands[0]); ++i) {
        submit(provider, COMMON_SESSION_MACHINE_PAUSED, commands[i].line, &result);
        assert(!strstr(result.text, "failed") && !strstr(result.text, "unsupported"));
        assert(strstr(result.text, commands[i].contains));
        assert(result.request == COMMON_SESSION_REQUEST_NONE);
    }
    bytes = access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_READ_LINEAR, .address = 0xb50u, .bytes = 4u });
    assert(memcmp(bytes.data, "\x12\x34\x56\x56", 4u) == 0);
    assert(remove("debug-cli-transfer.bin") == 0);
    /* Invalid watch/register/plan requests cannot dispatch execution. */
    submit(provider, COMMON_SESSION_MACHINE_PAUSED, "xw w nonsense", &result);
    assert(result.request == COMMON_SESSION_REQUEST_NONE);
    assert(!access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_GET_WATCH, .watch_kind = COMMON_MACHINE_DEBUG_WATCH_WRITE }).enabled);
    {
        lib_u32 ip = reg(machine, lease, COMMON_DEBUG_EIP);
        submit(provider, COMMON_SESSION_MACHINE_PAUSED, "g 0:800 nonsense", &result);
        assert(result.request == COMMON_SESSION_REQUEST_NONE);
        assert(reg(machine, lease, COMMON_DEBUG_EIP) == ip);
        submit(provider, COMMON_SESSION_MACHINE_PAUSED, "t 0:800 nonsense", &result);
        assert(result.request == COMMON_SESSION_REQUEST_NONE);
        assert(reg(machine, lease, COMMON_DEBUG_EIP) == ip);
        submit(provider, COMMON_SESSION_MACHINE_PAUSED, "t 0:800 0", &result);
        assert(result.request == COMMON_SESSION_REQUEST_NONE);
        assert(reg(machine, lease, COMMON_DEBUG_EIP) == ip);
    }
    assert(common_machine_debug_execute_with_lease(machine, lease,
        &(common_machine_debug_request){ .operation = COMMON_MACHINE_DEBUG_SET_WATCH,
            .watch_kind = (common_machine_debug_watch_kind)3 }, &bytes) == LIB_STATUS_INVALID_ARGUMENT);
}

static void watchpoints(common_machine *machine, common_machine_debug_lease *lease,
    completions *events, common_session_command_provider *provider)
{
    common_machine_debug_result value;
    common_session_command_result output;
    unsigned kind;
    setreg(machine, lease, COMMON_DEBUG_CS, 0u);
    setreg(machine, lease, COMMON_DEBUG_DS, 0u);
    setreg(machine, lease, COMMON_DEBUG_EFLAGS, 2u);
    /* word store, word load, loop. Watch the SECOND byte to prove overlap. */
    (void)access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_WRITE_LINEAR, .address = 0x800u, .bytes = 11u,
        .data = { 0xc7,0x06,0x00,0x0a,0x34,0x12,0xa1,0x00,0x0a,0xeb,0xf5 } });
    for (kind = 0u; kind < 3u; ++kind) {
        lib_u32 target = kind == COMMON_MACHINE_DEBUG_WATCH_EXECUTE ? 0x806u : 0xa01u;
        setreg(machine, lease, COMMON_DEBUG_EIP, 0x800u);
        (void)access(machine, lease, (common_machine_debug_request){
            .operation = COMMON_MACHINE_DEBUG_CLEAR_EXECUTION_PLAN });
        value = access(machine, lease, (common_machine_debug_request){
            .operation = COMMON_MACHINE_DEBUG_SET_WATCH, .watch_kind = kind, .address = target });
        assert(value.enabled && value.value == target);
        value = access(machine, lease, (common_machine_debug_request){
            .operation = COMMON_MACHINE_DEBUG_GET_WATCH, .watch_kind = kind });
        assert(value.enabled && value.value == target);
        /* Inspection and debugger writes cannot self-trigger the watch. */
        memory_word(machine, lease, 0xa00u, 0u);
        (void)access(machine, lease, (common_machine_debug_request){
            .operation = COMMON_MACHINE_DEBUG_READ_LINEAR, .address = 0xa00u, .bytes = 2u });
        assert(!access(machine, lease, (common_machine_debug_request){
            .operation = COMMON_MACHINE_DEBUG_GET_EXECUTION_RESULT }).enabled);
        submit(provider, COMMON_SESSION_MACHINE_PAUSED, "g", &output);
        assert(output.request == COMMON_SESSION_REQUEST_RESUME);
        assert(common_machine_resume(machine));
        wait_for(events->running);
        provider->note_runtime(provider->context, COMMON_SESSION_MACHINE_PAUSED,
            COMMON_SESSION_MACHINE_RUNNING, &output);
        wait_for(events->paused);
        assert(common_machine_debug_acquire(machine, lease) == LIB_STATUS_OK);
        value = access(machine, lease, (common_machine_debug_request){
            .operation = COMMON_MACHINE_DEBUG_GET_EXECUTION_RESULT });
        assert(value.enabled && value.observation.watch_hit);
        assert(value.observation.watch_kind == kind && value.observation.watch_address == target);
        assert(reg(machine, lease, COMMON_DEBUG_EIP) == (kind == 0u ? 0x809u : 0x806u));
        if (kind < 2u) {
            assert(value.observation.count == 1u);
            assert(value.observation.accesses[0].data == 0x1234u);
            assert(value.observation.accesses[0].bytes == 2u);
        }
        provider->note_runtime(provider->context, COMMON_SESSION_MACHINE_RUNNING,
            COMMON_SESSION_MACHINE_PAUSED, &output);
        provider->note_monitor_current(provider->context, LIB_TRUE, &output);
        assert(strstr(output.text, "Watch-") && strstr(output.text, " hit:"));
        assert(output.request == COMMON_SESSION_REQUEST_NONE);
        if (kind == COMMON_MACHINE_DEBUG_WATCH_EXECUTE) {
            /* T from the just-hit execute watch must execute, not re-hit. */
            run_plan(machine, lease, events, (common_machine_debug_request){
                .operation = COMMON_MACHINE_DEBUG_SET_EXECUTION_PLAN,
                .execution_kind = COMMON_MACHINE_DEBUG_EXECUTION_TRACE,
                .instruction_count = 1u }, 0x809u, 1u);
            assert(!access(machine, lease, (common_machine_debug_request){
                .operation = COMMON_MACHINE_DEBUG_GET_EXECUTION_RESULT }).observation.watch_hit);
        }
        (void)access(machine, lease, (common_machine_debug_request){
            .operation = COMMON_MACHINE_DEBUG_CLEAR_WATCH, .watch_kind = kind });
        assert(!access(machine, lease, (common_machine_debug_request){
            .operation = COMMON_MACHINE_DEBUG_GET_WATCH, .watch_kind = kind }).enabled);
    }
    /* A non-overlapping watch must not stop the first store; trace does. */
    setreg(machine, lease, COMMON_DEBUG_EIP, 0x800u);
    (void)access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_SET_WATCH, .watch_kind = COMMON_MACHINE_DEBUG_WATCH_WRITE,
        .address = 0xa02u });
    run_plan(machine, lease, events, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_SET_EXECUTION_PLAN,
        .execution_kind = COMMON_MACHINE_DEBUG_EXECUTION_TRACE, .instruction_count = 1u }, 0x806u, 1u);
    value = access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_GET_EXECUTION_RESULT });
    assert(!value.observation.watch_hit && value.observation.count == 1u);
    submit(provider, COMMON_SESSION_MACHINE_PAUSED, "xw u", &output);
    assert(strstr(output.text, "All watch points removed"));
    /* XT receives its observation through the same copied result, not a sink. */
    setreg(machine, lease, COMMON_DEBUG_EIP, 0x800u);
    submit(provider, COMMON_SESSION_MACHINE_PAUSED, "xt", &output);
    assert(output.request == COMMON_SESSION_REQUEST_RESUME);
    assert(common_machine_resume(machine)); wait_for(events->running); wait_for(events->paused);
    provider->note_runtime(provider->context, COMMON_SESSION_MACHINE_RUNNING,
        COMMON_SESSION_MACHINE_PAUSED, &output);
    provider->note_monitor_current(provider->context, LIB_TRUE, &output);
    assert(strstr(output.text, "Write: Lin=00000a00"));
    assert(common_machine_debug_acquire(machine, lease) == LIB_STATUS_OK);
}

static void access_boundaries(common_machine *machine, common_machine_debug_lease *lease,
    completions *events)
{
    common_machine_debug_request trace = {
        .operation = COMMON_MACHINE_DEBUG_SET_EXECUTION_PLAN,
        .execution_kind = COMMON_MACHINE_DEBUG_EXECUTION_TRACE, .instruction_count = 1u };
    common_machine_debug_result value;
    /* Byte and dword operands; implicit stack; reverse-buffer x87 qword. */
    const unsigned char program[] = {
        0xa0,0x00,0x0a, 0x66,0xa1,0x00,0x0a, 0xa2,0x04,0x0a,
        0x66,0xa3,0x04,0x0a, 0x50,0x58, 0xdb,0xe3, 0xdf,0x2e,0x00,0x0a,
        0xdf,0x3e,0x10,0x0a };
    const unsigned ends[] = {3,7,10,14,15,16,18,22,26};
    const unsigned widths[] = {1,4,1,4,2,2,0,8,8};
    unsigned i;
    common_machine_debug_request write = { .operation = COMMON_MACHINE_DEBUG_WRITE_LINEAR,
        .address = 0x800u, .bytes = sizeof(program) };
    memcpy(write.data, program, sizeof(program));
    (void)access(machine, lease, write);
    /* Exactly representable: check both operand observation and integer roundtrip. */
    memory_word(machine, lease, 0xa00u, 0x1234u);
    memory_word(machine, lease, 0xa04u, 0u);
    setreg(machine, lease, COMMON_DEBUG_EIP, 0x800u);
    setreg(machine, lease, COMMON_DEBUG_SS, 0u);
    setreg(machine, lease, COMMON_DEBUG_ESP, 0x900u);
    for (i = 0; i < sizeof(ends)/sizeof(ends[0]); ++i) {
        run_plan(machine, lease, events, trace, 0x800u + ends[i], 1u);
        value = access(machine, lease, (common_machine_debug_request){
            .operation = COMMON_MACHINE_DEBUG_GET_EXECUTION_RESULT });
        assert(value.observation.count == (widths[i] ? 1u : 0u));
        if (!widths[i]) continue;
        assert(value.observation.accesses[0].bytes == widths[i]);
        if (i == 7u)
            assert(value.observation.accesses[0].data == 0x0000123400001234ull);
        if (i == 8u) {
            lib_u64 stored;
            common_machine_debug_result bytes = access(machine, lease,
                (common_machine_debug_request){ .operation = COMMON_MACHINE_DEBUG_READ_LINEAR,
                    .address = 0xa10u, .bytes = 8u });
            memcpy(&stored, bytes.data, sizeof(stored));
            /* Both the observation and arithmetic must preserve this exact integer. */
            assert(value.observation.accesses[0].data == stored);
            assert(stored == 0x0000123400001234ull);
        }
    }
    /* Observation capacity never limits watch matching: hit byte 39. */
    (void)access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_WRITE_LINEAR, .address = 0x800u,
        .bytes = 2u, .data = {0xf3,0xaa} });
    setreg(machine, lease, COMMON_DEBUG_EIP, 0x800u);
    setreg(machine, lease, COMMON_DEBUG_ES, 0u);
    setreg(machine, lease, COMMON_DEBUG_EDI, 0xa00u);
    setreg(machine, lease, COMMON_DEBUG_ECX, 40u);
    (void)access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_SET_WATCH,
        .watch_kind = COMMON_MACHINE_DEBUG_WATCH_WRITE, .address = 0xa27u });
    run_plan(machine, lease, events, trace, 0x802u, 1u);
    value = access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_GET_EXECUTION_RESULT });
    assert(value.observation.truncated && value.observation.count == 32u);
    assert(value.observation.watch_hit && value.observation.watch_address == 0xa27u);
    (void)access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_CLEAR_WATCH, .watch_kind = COMMON_MACHINE_DEBUG_WATCH_WRITE });
    /* #UD's internal exception stack writes are not a completed instruction. */
    memory_word(machine, lease, 24u, 0x700u);
    setreg(machine, lease, COMMON_DEBUG_ESP, 0x900u);
    setreg(machine, lease, COMMON_DEBUG_EIP, 0x800u);
    (void)access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_WRITE_LINEAR, .address = 0x800u,
        .bytes = 2u, .data = {0x0f,0xff} });
    (void)access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_SET_WATCH,
        .watch_kind = COMMON_MACHINE_DEBUG_WATCH_WRITE, .address = 0x8ffu });
    run_plan(machine, lease, events, trace, 0x701u, 1u);
    value = access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_GET_EXECUTION_RESULT });
    assert(!value.observation.watch_hit && value.observation.count == 0u);
    common_machine_debug_cancel(machine);
    /* Synchronous query is ordered after cancel on the same executor. */
    assert(!access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_GET_WATCH,
        .watch_kind = COMMON_MACHINE_DEBUG_WATCH_WRITE }).enabled);
}

static void x87_values(common_machine *machine, common_machine_debug_lease *lease,
    completions *events)
{
    /* Real instructions on the existing executor; no host FP oracle or media writes. */
    static const struct {
        unsigned char opcode, load, store, bytes;
        unsigned char value[10];
    } formats[] = {
        {0xd9,0x06,0x1e,4, {0,0,0xc0,0x3f}}, /* FLD/FSTP single +1.5 */
        {0xd9,0x06,0x1e,4, {0,0,0xc0,0xbf}}, /* -1.5 */
        {0xdd,0x06,0x1e,8, {0,0,0,0,0,0,0xf8,0x3f}}, /* double +1.5 */
        {0xdd,0x06,0x1e,8, {0,0,0,0,0,0,0xf8,0xbf}},
        {0xdb,0x2e,0x3e,10,{0,0,0,0,0,0,0,0xc0,0xff,0x3f}}, /* extended */
        {0xdb,0x2e,0x3e,10,{0,0,0,0,0,0,0,0xc0,0xff,0xbf}},
        {0xdf,0x2e,0x3e,8, {0x34,0x12,0,0,0x34,0x12,0,0}}, /* integer */
        {0xdf,0x2e,0x3e,8, {0xcc,0xed,0xff,0xff,0xcb,0xed,0xff,0xff}},
        {0xdf,0x26,0x36,10,{0x56,0x34,0x12,0,0,0,0,0,0,0}}, /* packed BCD */
        {0xdf,0x26,0x36,10,{0x56,0x34,0x12,0,0,0,0,0,0,0x80}}
    };
    static const struct { unsigned char modrm; double expected; } arithmetic[] = {
        {0x06,8.0}, {0x0e,12.0}, {0x26,4.0}, {0x36,3.0}
    }; /* FADD/FMUL/FSUB/FDIV m64: 6 op 2 */
    common_machine_debug_request trace = {
        .operation = COMMON_MACHINE_DEBUG_SET_EXECUTION_PLAN,
        .execution_kind = COMMON_MACHINE_DEBUG_EXECUTION_TRACE, .instruction_count = 3u };
    common_machine_debug_request write = { .operation = COMMON_MACHINE_DEBUG_WRITE_LINEAR };
    unsigned i;
    setreg(machine, lease, COMMON_DEBUG_CS, 0u);
    setreg(machine, lease, COMMON_DEBUG_DS, 0u);
    setreg(machine, lease, COMMON_DEBUG_EFLAGS, 2u);
    for (i = 0; i < sizeof(formats) / sizeof(formats[0]); ++i) {
        unsigned char program[] = {0xdb,0xe3, formats[i].opcode,formats[i].load,0,0x0a,
            formats[i].opcode,formats[i].store,0x20,0x0a};
        common_machine_debug_result result;
        write.address = 0xa00u; write.bytes = formats[i].bytes;
        memcpy(write.data, formats[i].value, write.bytes);
        (void)access(machine, lease, write);
        write.address = 0x800u; write.bytes = sizeof(program);
        memcpy(write.data, program, sizeof(program));
        (void)access(machine, lease, write);
        setreg(machine, lease, COMMON_DEBUG_EIP, 0x800u);
        run_plan(machine, lease, events, trace, 0x800u + sizeof(program), 3u);
        result = access(machine, lease, (common_machine_debug_request){
            .operation = COMMON_MACHINE_DEBUG_READ_LINEAR, .address = 0xa20u,
            .bytes = formats[i].bytes });
        assert(memcmp(result.data, formats[i].value, formats[i].bytes) == 0);
    }
    trace.instruction_count = 4u;
    for (i = 0; i < sizeof(arithmetic) / sizeof(arithmetic[0]); ++i) {
        unsigned char program[] = {0xdb,0xe3,0xdd,0x06,0,0x0a,
            0xdc,arithmetic[i].modrm,0x10,0x0a,0xdd,0x1e,0x20,0x0a};
        const double operands[] = {6.0,2.0};
        common_machine_debug_result result;
        double actual;
        write.bytes = sizeof(double); write.address = 0xa00u;
        memcpy(write.data, &operands[0], write.bytes);
        (void)access(machine, lease, write);
        write.address = 0xa10u;
        memcpy(write.data, &operands[1], write.bytes);
        (void)access(machine, lease, write);
        write.address = 0x800u; write.bytes = sizeof(program);
        memcpy(write.data, program, write.bytes);
        (void)access(machine, lease, write);
        setreg(machine, lease, COMMON_DEBUG_EIP, 0x800u);
        run_plan(machine, lease, events, trace, 0x800u + sizeof(program), 4u);
        result = access(machine, lease, (common_machine_debug_request){
            .operation = COMMON_MACHINE_DEBUG_READ_LINEAR, .address = 0xa20u, .bytes = 8u });
        memcpy(&actual, result.data, sizeof(actual));
        assert(actual == arithmetic[i].expected);
    }
}

static void trace_cli(common_machine *machine, common_machine_debug_lease *lease,
    completions *events, common_session_command_provider *provider)
{
    common_session_command_result result;
    unsigned index;
    setreg(machine, lease, COMMON_DEBUG_EFLAGS, 2u);
    setreg(machine, lease, COMMON_DEBUG_EIP, 0x700u);
    submit(provider, COMMON_SESSION_MACHINE_PAUSED, "t 2", &result);
    assert(result.request == COMMON_SESSION_REQUEST_RESUME);
    for (index = 0; index < 2u; ++index) {
        assert(common_machine_resume(machine));
        wait_for(events->running);
        provider->note_runtime(provider->context, COMMON_SESSION_MACHINE_PAUSED,
            COMMON_SESSION_MACHINE_RUNNING, &result);
        wait_for(events->paused);
        provider->note_runtime(provider->context, COMMON_SESSION_MACHINE_RUNNING,
            COMMON_SESSION_MACHINE_PAUSED, &result);
        provider->note_monitor_current(provider->context, LIB_TRUE, &result);
        assert(strstr(result.text, "AX=") != NULL);
        assert(strstr(result.text, "Machine paused.") != NULL);
        assert(result.request == (index == 0u ? COMMON_SESSION_REQUEST_RESUME :
            COMMON_SESSION_REQUEST_NONE));
    }
    assert(common_machine_debug_acquire(machine, lease) == LIB_STATUS_OK);
    assert(reg(machine, lease, COMMON_DEBUG_EIP) == 0x702u);
    (void)access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_WRITE_LINEAR, .address = 0x700u,
        .bytes = 3u, .data = {0x40,0xeb,0xfd} });
    setreg(machine, lease, COMMON_DEBUG_EIP, 0x700u);
    submit(provider, COMMON_SESSION_MACHINE_PAUSED, "xg 701 2", &result);
    assert(result.request == COMMON_SESSION_REQUEST_RESUME);
    for (index = 0u; index < 2u; ++index) {
        assert(common_machine_resume(machine)); wait_for(events->running);
        provider->note_runtime(provider->context, COMMON_SESSION_MACHINE_PAUSED,
            COMMON_SESSION_MACHINE_RUNNING, &result);
        wait_for(events->paused);
        provider->note_runtime(provider->context, COMMON_SESSION_MACHINE_RUNNING,
            COMMON_SESSION_MACHINE_PAUSED, &result);
        provider->note_monitor_current(provider->context, LIB_TRUE, &result);
        assert(strstr(result.text, "instructions executed before the break point."));
        assert(result.request == (index == 0u ? COMMON_SESSION_REQUEST_RESUME :
            COMMON_SESSION_REQUEST_NONE));
    }
    assert(common_machine_debug_acquire(machine, lease) == LIB_STATUS_OK);
    /* Restore the fault-handler fixture used by the operand tests. */
    (void)access(machine, lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_WRITE_LINEAR, .address = 0x700u,
        .bytes = 2u, .data = {0x90,0x90} });
}

int main(void)
{
    const char *path = "debug-commands-smoke.img";
    unsigned char sector[512] = { 0xeb, 0xfe };
    FILE *file;
    softpc_machine_options options = { .floppy_path = path,
        .presentation = SOFTPC_PRESENTATION_WINDOW,
        .media_mode = SOFTPC_MEDIA_OVERLAY };
    softpc_machine *product = NULL;
    vm_driver *adapter = NULL;
    common_machine_driver driver = { 0 };
    common_machine *machine = NULL;
    app_command_context commands = { 0 };
    common_session_command_provider provider = { 0 };
    common_session_command_result result = { 0 };
    common_machine_debug_lease lease;
    common_machine_debug_result value;
    completions events = { CreateEventA(NULL, FALSE, FALSE, NULL),
        CreateEventA(NULL, FALSE, FALSE, NULL), CreateEventA(NULL, FALSE, FALSE, NULL) };
    lib_u32 saved_eax;
    const common_session_machine_state inactive[] = {
        COMMON_SESSION_MACHINE_INIT, COMMON_SESSION_MACHINE_STOPPED };
    size_t index;
    assert(events.paused && events.running && events.stopped);
    sector[510] = 0x55; sector[511] = 0xaa;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fclose(file) == 0);
    assert(softpc_machine_create(&options, &product) == SOFTPC_MACHINE_OK);
    assert(vm_driver_create(&adapter, product) == LIB_STATUS_OK);
    vm_driver_describe(adapter, &driver);
    observed_product = product;
    program_completed = CreateEventA(NULL, FALSE, FALSE, NULL);
    assert(program_completed != NULL);
    copy_product_frame = driver.copy_frame;
    driver.copy_frame = observe_program;
    assert(common_machine_create(&machine, &driver) == LIB_STATUS_OK);
    common_machine_set_state_sink(machine, note_state, &events);
    assert(app_composition_initialize(&commands, machine,
        options.presentation, &provider) == LIB_STATUS_OK);
    /* Exercise the actual composed provider, not a second hotkey dispatcher. */
    assert(provider.context == &commands && provider.open == app_command_provider_open);
    assert(provider.submit_line == app_command_provider_submit_line);
    assert(provider.begin_external == app_command_provider_begin_external);
    assert(provider.handle_hotkey(provider.context, COMMON_SESSION_MACHINE_PAUSED,
        "send-ctrl-alt-del", &result) && result.request == COMMON_SESSION_REQUEST_NONE);
    assert(provider.handle_hotkey(provider.context, COMMON_SESSION_MACHINE_PAUSED,
        "send-alt-enter", &result) && result.request == COMMON_SESSION_REQUEST_NONE);
    assert(provider.handle_hotkey(provider.context, COMMON_SESSION_MACHINE_PAUSED,
        "release-window-mouse", &result) && result.release_window_mouse);
    assert(!provider.handle_hotkey(provider.context, COMMON_SESSION_MACHINE_PAUSED,
        NULL, &result));
    for (index = 0u; index < sizeof(inactive) / sizeof(inactive[0]); ++index) {
        submit(&provider, inactive[index], "debug", &result);
        assert(commands.debug_active && result.request == COMMON_SESSION_REQUEST_NONE);
        provider.note_monitor_current(&commands, LIB_TRUE, &result);
        assert(result.arm_prompt && strcmp(result.prompt, "-") == 0);
        submit(&provider, inactive[index], "?", &result);
        assert(strstr(result.text, "assemble") != NULL);
        submit(&provider, inactive[index], "r", &result);
        assert(strstr(result.text, "must be paused") != NULL && commands.debug_active);
        submit(&provider, inactive[index], "q", &result);
        assert(!commands.debug_active && common_machine_state_get(machine) == COMMON_MACHINE_STOPPED);
    }
    assert(common_machine_reset(machine));
    wait_for(events.paused);
    provider.note_runtime(&commands, COMMON_SESSION_MACHINE_INIT,
        COMMON_SESSION_MACHINE_RESET_COMPLETED, &result);
    submit(&provider, COMMON_SESSION_MACHINE_PAUSED, "debug", &result);
    submit(&provider, COMMON_SESSION_MACHINE_PAUSED, "r", &result);
    assert(strstr(result.text, "AX=") != NULL && strstr(result.text, "failed") == NULL);
    assert(common_machine_debug_acquire(machine, &lease) == LIB_STATUS_OK);
    synchronous_access(machine, &lease);
    assert(common_machine_debug_execute_with_lease(machine, &lease,
        &(common_machine_debug_request){ .operation = COMMON_MACHINE_DEBUG_READ_REGISTER,
            .register_id = COMMON_DEBUG_EAX }, &value) == LIB_STATUS_OK);
    saved_eax = value.value;
    submit(&provider, COMMON_SESSION_MACHINE_PAUSED, "r ax", &result);
    provider.note_monitor_current(&commands, LIB_TRUE, &result);
    assert(result.arm_prompt && strcmp(result.prompt, ":") == 0);
    submit(&provider, COMMON_SESSION_MACHINE_PAUSED, "1234", &result);
    assert(common_machine_debug_execute_with_lease(machine, &lease,
        &(common_machine_debug_request){ .operation = COMMON_MACHINE_DEBUG_READ_REGISTER,
            .register_id = COMMON_DEBUG_EAX }, &value) == LIB_STATUS_OK);
    assert((value.value & 0xffffu) == 0x1234u);
    assert(common_machine_debug_execute_with_lease(machine, &lease,
        &(common_machine_debug_request){ .operation = COMMON_MACHINE_DEBUG_WRITE_REGISTER,
            .register_id = COMMON_DEBUG_EAX, .address = saved_eax }, &value) == LIB_STATUS_OK);
    submit(&provider, COMMON_SESSION_MACHINE_PAUSED, "e 0:500 12 34", &result);
    submit(&provider, COMMON_SESSION_MACHINE_PAUSED, "d 0:500", &result);
    assert(strstr(result.text, "12 34") != NULL);
    submit(&provider, COMMON_SESSION_MACHINE_PAUSED, "u f000:fff0", &result);
    assert(strstr(result.text, "F000:FFF0") != NULL);
    submit(&provider, COMMON_SESSION_MACHINE_PAUSED, "a 0:510", &result);
    provider.note_monitor_current(&commands, LIB_TRUE, &result);
    assert(result.arm_prompt && strcmp(result.prompt, "assemble> ") == 0);
    submit(&provider, COMMON_SESSION_MACHINE_PAUSED, "nop", &result);
    submit(&provider, COMMON_SESSION_MACHINE_PAUSED, "", &result);
    submit(&provider, COMMON_SESSION_MACHINE_PAUSED, "d 0:510", &result);
    assert(strstr(result.text, "90") != NULL);
    submit(&provider, COMMON_SESSION_MACHINE_PAUSED, "i 60", &result);
    assert(strstr(result.text, "unsupported") == NULL && strstr(result.text, "failed") == NULL);
    submit(&provider, COMMON_SESSION_MACHINE_PAUSED, "xd ffffffff 1", &result);
    /* With A20 wrapping enabled this is the last ROM byte, not an invalid
     * host pointer. The original SAS bus, not host RAM bounds, decides. */
    assert(strstr(result.text, "failed") == NULL);
    assert(provider.handle_hotkey(&commands, COMMON_SESSION_MACHINE_PAUSED,
        "pause-toggle", &result));
    assert(commands.debug_active && result.request == COMMON_SESSION_REQUEST_RESUME);
    /* A second CAP cannot bypass the command transition reservation. */
    assert(provider.handle_hotkey(&commands, COMMON_SESSION_MACHINE_PAUSED,
        "pause-toggle", &result));
    assert(result.request == COMMON_SESSION_REQUEST_NONE);
    assert(common_machine_resume(machine));
    wait_for(events.running);
    provider.note_runtime(&commands, COMMON_SESSION_MACHINE_PAUSED,
        COMMON_SESSION_MACHINE_RUNNING, &result);
    submit(&provider, COMMON_SESSION_MACHINE_RUNNING, "r", &result);
    assert(strstr(result.text, "must be paused") != NULL && commands.debug_active);
    submit(&provider, COMMON_SESSION_MACHINE_RUNNING, "q", &result);
    submit(&provider, COMMON_SESSION_MACHINE_RUNNING, "debug", &result);
    assert(commands.debug_active && common_machine_state_get(machine) == COMMON_MACHINE_RUNNING);
    assert(provider.handle_hotkey(&commands, COMMON_SESSION_MACHINE_RUNNING,
        "pause-toggle", &result));
    assert(result.request == COMMON_SESSION_REQUEST_PAUSE);
    assert(common_machine_pause(machine));
    wait_for(events.paused);
    provider.note_runtime(&commands, COMMON_SESSION_MACHINE_RUNNING,
        COMMON_SESSION_MACHINE_PAUSED, &result);
    assert(common_machine_debug_execute_with_lease(machine, &lease,
        &(common_machine_debug_request){0}, &value) == LIB_STATUS_INVALID_STATE);
    submit(&provider, COMMON_SESSION_MACHINE_PAUSED, "r", &result);
    assert(strstr(result.text, "AX=") != NULL);
    assert(common_machine_debug_acquire(machine, &lease) == LIB_STATUS_OK);
    /* mov word [0600],1234; mov word [0602],5678; jmp $ */
    execution_plans(machine, &lease, &events);
    command_matrix(machine, &lease, &provider);
    trace_cli(machine, &lease, &events, &provider);
    watchpoints(machine, &lease, &events, &provider);
    access_boundaries(machine, &lease, &events);
    x87_values(machine, &lease, &events);
    (void)access(machine, &lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_SET_WATCH, .watch_kind = COMMON_MACHINE_DEBUG_WATCH_READ,
        .address = 0xa00u });
    (void)access(machine, &lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_SET_EXECUTION_PLAN,
        .execution_kind = COMMON_MACHINE_DEBUG_EXECUTION_BREAK_LINEAR, .address = 0x12345678u });
    assert(common_machine_reset(machine));
    wait_for(events.paused);
    provider.note_runtime(&commands, COMMON_SESSION_MACHINE_PAUSED,
        COMMON_SESSION_MACHINE_RESET_COMPLETED, &result);
    assert(common_machine_debug_acquire(machine, &lease) == LIB_STATUS_OK);
    assert(!access(machine, &lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_GET_EXECUTION_RESULT }).enabled);
    assert(!access(machine, &lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_GET_WATCH, .watch_kind = COMMON_MACHINE_DEBUG_WATCH_READ }).enabled);
    (void)access(machine, &lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_WRITE_REAL, .offset = 0x520u, .bytes = 14u,
        .data = { 0xc7,0x06,0x00,0x06,0x34,0x12,0xc7,0x06,0x02,0x06,0x78,0x56,0xeb,0xfe } });
    memory_word(machine, &lease, 0x600u, 0u);
    setreg(machine, &lease, COMMON_DEBUG_DS, 0u);
    setreg(machine, &lease, COMMON_DEBUG_CS, 0u);
    setreg(machine, &lease, COMMON_DEBUG_EIP, 0x520u);
    submit(&provider, COMMON_SESSION_MACHINE_PAUSED, "g", &result);
    assert(result.request == COMMON_SESSION_REQUEST_RESUME && commands.debug_active);
    assert(common_machine_resume(machine));
    wait_for(events.running);
    wait_for(program_completed);
    provider.note_runtime(&commands, COMMON_SESSION_MACHINE_PAUSED,
        COMMON_SESSION_MACHINE_RUNNING, &result);
    assert(common_machine_stop(machine));
    wait_for(events.stopped);
    submit(&provider, COMMON_SESSION_MACHINE_STOPPED, "q", &result);
    provider.note_monitor_current(&commands, LIB_TRUE, &result);
    assert(result.arm_prompt && strcmp(result.prompt, "SoftPC> ") == 0);
    app_command_dispose(&commands);
    common_machine_destroy(machine);
    vm_driver_destroy(adapter);
    softpc_machine_destroy(product);
    assert(remove(path) == 0);
    CloseHandle(events.paused); CloseHandle(events.running); CloseHandle(events.stopped);
    CloseHandle(program_completed);
    puts("debug commands: four-state CLI, real registers/memory, CAP and errors passed");
    return 0;
}
