#include "app/command_binding.h"
#include "app/machine_driver.h"

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

int main(void)
{
    const char *path = "debug-binding-smoke.img";
    unsigned char sector[512] = { 0xeb, 0xfe };
    FILE *file;
    softpc_machine_options options = { .floppy_path = path,
        .presentation = SOFTPC_PRESENTATION_WINDOW,
        .media_mode = SOFTPC_MEDIA_OVERLAY };
    softpc_machine *product = NULL;
    app_machine_driver *adapter = NULL;
    common_machine_driver driver = { 0 };
    common_machine *machine = NULL;
    app_command_binding binding = { 0 };
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
    assert(app_machine_driver_create(&adapter, product) == LIB_STATUS_OK);
    app_machine_driver_describe(adapter, &driver);
    observed_product = product;
    program_completed = CreateEventA(NULL, FALSE, FALSE, NULL);
    assert(program_completed != NULL);
    copy_product_frame = driver.copy_frame;
    driver.copy_frame = observe_program;
    assert(common_machine_create(&machine, &driver) == LIB_STATUS_OK);
    common_machine_set_state_sink(machine, note_state, &events);
    assert(app_command_binding_initialize(&binding, machine,
        options.presentation, &provider) == LIB_STATUS_OK);
    for (index = 0u; index < sizeof(inactive) / sizeof(inactive[0]); ++index) {
        submit(&provider, inactive[index], "debug", &result);
        assert(binding.debug_active && result.request == COMMON_SESSION_REQUEST_NONE);
        provider.note_monitor_current(&binding, LIB_TRUE, &result);
        assert(result.arm_prompt && strcmp(result.prompt, "-") == 0);
        submit(&provider, inactive[index], "?", &result);
        assert(strstr(result.text, "assemble") != NULL);
        submit(&provider, inactive[index], "r", &result);
        assert(strstr(result.text, "must be paused") != NULL && binding.debug_active);
        submit(&provider, inactive[index], "q", &result);
        assert(!binding.debug_active && common_machine_state_get(machine) == COMMON_MACHINE_STOPPED);
    }
    assert(common_machine_reset(machine));
    wait_for(events.paused);
    provider.note_runtime(&binding, COMMON_SESSION_MACHINE_INIT,
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
    provider.note_monitor_current(&binding, LIB_TRUE, &result);
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
    provider.note_monitor_current(&binding, LIB_TRUE, &result);
    assert(result.arm_prompt && strcmp(result.prompt, "assemble> ") == 0);
    submit(&provider, COMMON_SESSION_MACHINE_PAUSED, "nop", &result);
    submit(&provider, COMMON_SESSION_MACHINE_PAUSED, "", &result);
    submit(&provider, COMMON_SESSION_MACHINE_PAUSED, "d 0:510", &result);
    assert(strstr(result.text, "90") != NULL);
    {
        const common_machine_debug_operation unsupported[] = {
            COMMON_MACHINE_DEBUG_SET_WATCH,
            COMMON_MACHINE_DEBUG_CLEAR_WATCH, COMMON_MACHINE_DEBUG_GET_WATCH,
            COMMON_MACHINE_DEBUG_SET_EXECUTION_PLAN, COMMON_MACHINE_DEBUG_GET_EXECUTION_RESULT
        };
        for (index = 0u; index < sizeof(unsupported) / sizeof(unsupported[0]); ++index)
            assert(common_machine_debug_execute_with_lease(machine, &lease,
                &(common_machine_debug_request){ .operation = unsupported[index] },
                &value) == LIB_STATUS_UNSUPPORTED);
    }
    submit(&provider, COMMON_SESSION_MACHINE_PAUSED, "t", &result);
    assert(strstr(result.text, "unsupported") != NULL && result.request == COMMON_SESSION_REQUEST_NONE);
    submit(&provider, COMMON_SESSION_MACHINE_PAUSED, "i 60", &result);
    assert(strstr(result.text, "unsupported") == NULL && strstr(result.text, "failed") == NULL);
    submit(&provider, COMMON_SESSION_MACHINE_PAUSED, "xd ffffffff 1", &result);
    /* With A20 wrapping enabled this is the last ROM byte, not an invalid
     * host pointer. The original SAS bus, not host RAM bounds, decides. */
    assert(strstr(result.text, "failed") == NULL);
    assert(provider.handle_hotkey(&binding, COMMON_SESSION_MACHINE_PAUSED,
        "pause-toggle", &result));
    assert(binding.debug_active && result.request == COMMON_SESSION_REQUEST_RESUME);
    assert(common_machine_resume(machine));
    wait_for(events.running);
    provider.note_runtime(&binding, COMMON_SESSION_MACHINE_PAUSED,
        COMMON_SESSION_MACHINE_RUNNING, &result);
    submit(&provider, COMMON_SESSION_MACHINE_RUNNING, "r", &result);
    assert(strstr(result.text, "must be paused") != NULL && binding.debug_active);
    submit(&provider, COMMON_SESSION_MACHINE_RUNNING, "q", &result);
    submit(&provider, COMMON_SESSION_MACHINE_RUNNING, "debug", &result);
    assert(binding.debug_active && common_machine_state_get(machine) == COMMON_MACHINE_RUNNING);
    assert(provider.handle_hotkey(&binding, COMMON_SESSION_MACHINE_RUNNING,
        "pause-toggle", &result));
    assert(result.request == COMMON_SESSION_REQUEST_PAUSE);
    assert(common_machine_pause(machine));
    wait_for(events.paused);
    provider.note_runtime(&binding, COMMON_SESSION_MACHINE_RUNNING,
        COMMON_SESSION_MACHINE_PAUSED, &result);
    assert(common_machine_debug_execute_with_lease(machine, &lease,
        &(common_machine_debug_request){0}, &value) == LIB_STATUS_INVALID_STATE);
    submit(&provider, COMMON_SESSION_MACHINE_PAUSED, "r", &result);
    assert(strstr(result.text, "AX=") != NULL);
    assert(common_machine_debug_acquire(machine, &lease) == LIB_STATUS_OK);
    /* mov word [0600],1234; mov word [0602],5678; jmp $ */
    (void)access(machine, &lease, (common_machine_debug_request){
        .operation = COMMON_MACHINE_DEBUG_WRITE_REAL, .offset = 0x520u, .bytes = 14u,
        .data = { 0xc7,0x06,0x00,0x06,0x34,0x12,0xc7,0x06,0x02,0x06,0x78,0x56,0xeb,0xfe } });
    memory_word(machine, &lease, 0x600u, 0u);
    setreg(machine, &lease, COMMON_DEBUG_DS, 0u);
    setreg(machine, &lease, COMMON_DEBUG_CS, 0u);
    setreg(machine, &lease, COMMON_DEBUG_EIP, 0x520u);
    submit(&provider, COMMON_SESSION_MACHINE_PAUSED, "g", &result);
    assert(result.request == COMMON_SESSION_REQUEST_RESUME && binding.debug_active);
    assert(common_machine_resume(machine));
    wait_for(events.running);
    wait_for(program_completed);
    provider.note_runtime(&binding, COMMON_SESSION_MACHINE_PAUSED,
        COMMON_SESSION_MACHINE_RUNNING, &result);
    assert(common_machine_stop(machine));
    wait_for(events.stopped);
    submit(&provider, COMMON_SESSION_MACHINE_STOPPED, "q", &result);
    provider.note_monitor_current(&binding, LIB_TRUE, &result);
    assert(result.arm_prompt && strcmp(result.prompt, "SoftPC> ") == 0);
    app_command_binding_dispose(&binding);
    common_machine_destroy(machine);
    app_machine_driver_destroy(adapter);
    softpc_machine_destroy(product);
    assert(remove(path) == 0);
    CloseHandle(events.paused); CloseHandle(events.running); CloseHandle(events.stopped);
    CloseHandle(program_completed);
    puts("debug binding: four-state CLI, real registers/memory, CAP and errors passed");
    return 0;
}
