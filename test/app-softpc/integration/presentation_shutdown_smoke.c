#include "lib/types/types_interface.h"
#include "emulator/ui/ui_interface.h"
#include "product/surface/entry_interface.h"
#include "product/config.h"
#include "product/composed_machine.h"

#include <assert.h>
#include <stdio.h>

struct emulator_ui { emulator_ui_options options; };

static emulator_ui surface;
static lib_u32 scenario;
static lib_u32 requests;
static lib_u32 destroyed;
static lib_u32 reported;
static lib_u32 generations;
static lib_u32 states;

static void create_image(const char *path)
{
    const unsigned char sector[512] = {0};
    FILE *file = fopen(path, "wb");

    assert(file != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fclose(file) == 0);
}

lib_status emulator_ui_create(emulator_ui **out_ui, const emulator_ui_options *options)
{
    surface.options = *options;
    *out_ui = &surface;
    return LIB_STATUS_OK;
}

lib_status emulator_ui_destroy(emulator_ui *ui)
{
    assert(ui == &surface && requests == 1u);
    ++destroyed;
    return scenario == 3u ? LIB_STATUS_IO_ERROR : LIB_STATUS_OK;
}

lib_status emulator_ui_request_monitor_line(emulator_ui *ui)
{
    emulator_ui_event event = {0};

    assert(ui == &surface && ++requests == 1u);
    if (scenario == 0u || scenario == 3u) {
        event.kind = EMULATOR_UI_EVENT_MONITOR_LINE;
        lib_memory_copy(event.value.line.text, "exit", 5u);
        event.value.line.length = 4u;
    } else if (scenario == 1u) {
        event.kind = EMULATOR_UI_EVENT_CONSOLE_FAILED;
    } else {
        event.kind = EMULATOR_UI_EVENT_KVM_DELIVERY_FAILED;
        event.value.delivery_failure.source_identity = 1u;
        event.value.delivery_failure.status = LIB_STATUS_IO_ERROR;
    }
    assert(surface.options.event_sink(surface.options.event_context, &event));
    return LIB_STATUS_OK;
}

lib_status emulator_ui_write_monitor(emulator_ui *ui, const char *text)
{
    assert(ui == &surface);
    if (lib_text_find_substring(text, "input failed") ||
        lib_text_find_substring(text, "delivery failed")) ++reported;
    return LIB_STATUS_OK;
}

void emulator_ui_set_run_generation(emulator_ui *ui, lib_u32 generation)
{
    assert(ui == &surface);
    (void)generation;
    ++generations;
}

lib_status emulator_ui_apply_action(emulator_ui *ui, emulator_ui_action action,
    emulator_ui_state state)
{
    assert(ui == &surface);
    (void)action;
    (void)state;
    return LIB_STATUS_OK;
}

lib_status emulator_ui_set_state(emulator_ui *ui, emulator_ui_state state)
{
    assert(ui == &surface);
    (void)state;
    ++states;
    return LIB_STATUS_OK;
}

lib_status emulator_ui_publish_frame(emulator_ui *ui, const kvm_window_frame *frame,
    const kvm_console_character_map *characters, lib_u32 sequence,
    lib_bool window, lib_bool console, lib_bool status)
{
    (void)ui; (void)frame; (void)characters; (void)sequence;
    (void)window; (void)console; (void)status;
    assert(0);
    return LIB_STATUS_IO_ERROR;
}

lib_status emulator_ui_release_window_mouse(emulator_ui *ui)
{ (void)ui; assert(0); return LIB_STATUS_IO_ERROR; }

lib_status emulator_ui_cancel_monitor_line(emulator_ui *ui, lib_bool *completed)
{ (void)ui; (void)completed; assert(0); return LIB_STATUS_IO_ERROR; }

int main(void)
{
    const char *path = "presentation-shutdown.img";
    app_startup_config config = {0};
    product_surface_definition definition = {0};

    create_image(path);
    lib_text_copy(config.floppy_path, path);
    config.presentation = EMULATOR_SESSION_DISPLAY_WINDOW;
    config.floppy_mode = LIB_STORAGE_MEDIUM_READONLY;
    config.hard_disk_mode = LIB_STORAGE_MEDIUM_OVERLAY;
    assert(app_startup_compose_ui(&config, &definition.ui) == LIB_STATUS_OK);
    definition.name = "presentation-shutdown";
    definition.banner = "presentation-shutdown\n\nBuilt on test";
    for (scenario = 0u; scenario < 3u; ++scenario) {
        requests = destroyed = reported = generations = states = 0u;
        assert(softpc_product_compose_machine(&config, &definition.machine) ==
            LIB_STATUS_OK);
        assert(product_surface_run(&definition) == (scenario == 0u ? 0 : 1));
        assert(destroyed == 1u && generations != 0u && states != 0u &&
            reported == (scenario == 1u || scenario == 2u));
    }
    assert(remove(path) == 0);

    scenario = 3u;
    requests = destroyed = reported = generations = states = 0u;
    create_image(path);
    assert(softpc_product_compose_machine(&config, &definition.machine) ==
        LIB_STATUS_OK);
    assert(product_surface_run(&definition) == 1);
    assert(destroyed == 1u && generations != 0u && states != 0u && reported == 0u);
    return 0;
}
