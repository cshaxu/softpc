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
            COMMON_MACHINE_DEBUG_READ_PORT, COMMON_MACHINE_DEBUG_WRITE_PORT,
            COMMON_MACHINE_DEBUG_GET_CPU_SNAPSHOT, COMMON_MACHINE_DEBUG_SET_WATCH,
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
    assert(strstr(result.text, "unsupported") != NULL);
    submit(&provider, COMMON_SESSION_MACHINE_PAUSED, "xd ffffffff 1", &result);
    assert(strstr(result.text, "failed") != NULL);
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
    submit(&provider, COMMON_SESSION_MACHINE_PAUSED, "g", &result);
    assert(result.request == COMMON_SESSION_REQUEST_RESUME && binding.debug_active);
    assert(common_machine_resume(machine));
    wait_for(events.running);
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
    puts("debug binding: four-state CLI, real registers/memory, CAP and errors passed");
    return 0;
}
