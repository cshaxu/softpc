#include "runtime.h"
#include "test_cleanup.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>

static int runtime_input_wait_for_byte(softpc_machine *machine,
    uint32_t address, uint8_t expected, DWORD deadline, uint8_t *observed)
{
    uint8_t value = 0u;

    do {
        if (softpc_machine_read_physical(machine, address, &value,
                sizeof(value)) == SOFTPC_MACHINE_OK && value >= expected)
            return 1;
        Sleep(1u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
    if (observed != NULL) *observed = value;
    return 0;
}

static int runtime_input_wait_for_state(app_runtime *runtime,
    app_runtime_state expected, DWORD deadline)
{
    do {
        if (app_runtime_get_state(runtime) == expected) return 1;
        Sleep(1u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
    return 0;
}

static int runtime_input_enqueue_key(app_runtime *runtime, uint16_t scan,
    lib_u32 key, uint8_t pressed)
{
    ux_event event = { 0 };

    event.type = UX_EVENT_KEY;
    event.data.key.scan_code = scan;
    event.data.key.key = key;
    event.data.key.pressed = pressed;
    return app_runtime_enqueue_input_event(runtime, &event);
}

int main(void)
{
    static const char image_path[] = "softpc-runtime-input-smoke.img";
    /* The boot code halts after installing an IRQ1 handler.  The handler
       drains port 60h, increments 0500h, acknowledges the original PIC, and
       returns.  It makes the test observe the complete standalone queue ->
       original 8042 -> PIC -> CCPU delivery path without guest DOS files. */
    static const uint8_t boot_code[] = {
        0xfau, 0x31u, 0xc0u, 0x8eu, 0xd8u,
        0xc6u, 0x06u, 0x01u, 0x05u, 0x55u,
        0xb8u, 0x1au, 0x7cu, 0xa3u, 0x24u, 0x00u,
        0x0eu, 0x58u, 0xa3u, 0x26u, 0x00u,
        0xfbu, 0x90u, 0xf4u, 0xebu, 0xfdu,
        0xe4u, 0x60u, 0xfeu, 0x06u, 0x00u, 0x05u,
        0xb0u, 0x20u, 0xe6u, 0x20u, 0xcfu
    };
    uint8_t sector[512] = { 0 };
    FILE *image = NULL;
    softpc_machine_options options = { image_path, NULL,
        SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    app_runtime *runtime = NULL;
    DWORD started_at;
    DWORD deadline;
    uint8_t delivered = 0u;
    uint8_t reset_irq_count = 0u;

    memcpy(sector, boot_code, sizeof(boot_code));
    sector[510] = 0x55u;
    sector[511] = 0xaau;
    image = fopen(image_path, "wb");
    assert(image != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), image) == sizeof(sector));
    assert(fclose(image) == 0);
    image = NULL;

    options.media_mode = SOFTPC_MEDIA_OVERLAY;
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(app_runtime_create(machine, &runtime));
    assert(app_runtime_start(runtime));
    assert(runtime_input_wait_for_byte(machine, 0x501u, 0x55u,
        GetTickCount() + 5000u, NULL));
    Sleep(250u);
    assert(softpc_machine_read_physical(machine, 0x500u, &delivered,
        sizeof(delivered)) == SOFTPC_MACHINE_OK);
    reset_irq_count = delivered;

    started_at = GetTickCount();
    assert(runtime_input_enqueue_key(runtime, 0x1fu, 'S', 1u));
    assert(runtime_input_enqueue_key(runtime, 0x1fu, 'S', 0u));
    assert(runtime_input_enqueue_key(runtime, 0x20u, 'D', 1u));
    assert(runtime_input_enqueue_key(runtime, 0x20u, 'D', 0u));
    deadline = started_at + 1000u;
    assert(runtime_input_wait_for_byte(machine, 0x500u, 4u, deadline,
        &delivered));
    /* The first queued key is woken by the frontend; three continuation
       wakes must deliver the rest without three 50 ms device-timer waits. */
    assert((DWORD)(GetTickCount() - started_at) < 125u);
    assert(app_runtime_get_state(runtime) == SOFTPC_RUNTIME_RUNNING);

    /* Paused is a monitor/control state, not a second guest-input mode.  A
       release is as much a guest record as a make: it must be rejected here,
       then a subsequent cold run must reach its boot code with no IRQ1 from
       that rejected old-run input. */
    assert(app_runtime_pause(runtime));
    assert(runtime_input_wait_for_state(runtime, SOFTPC_RUNTIME_PAUSED,
        GetTickCount() + 5000u));
    assert(!runtime_input_enqueue_key(runtime, 0x1fu, 'S', 0u));
    assert(app_runtime_stop(runtime));
    assert(runtime_input_wait_for_state(runtime, SOFTPC_RUNTIME_STOPPED,
        GetTickCount() + 5000u));
    assert(app_runtime_start(runtime));
    assert(runtime_input_wait_for_byte(machine, 0x501u, 0x55u,
        GetTickCount() + 5000u, NULL));
    Sleep(250u);
    assert(softpc_machine_read_physical(machine, 0x500u, &delivered,
        sizeof(delivered)) == SOFTPC_MACHINE_OK);
    assert(delivered == reset_irq_count);

    assert(app_runtime_stop(runtime));
    app_runtime_destroy(runtime);
    softpc_machine_destroy(machine);
    assert(softpc_test_remove_image(image_path));
    return 0;
}
#else
int main(void)
{
    return 0;
}
#endif
