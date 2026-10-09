#include "lib/types/types_interface.h"
#include "common/ui/ui_interface.h"
#include "product/surface/entry_interface.h"
#include "product/config.h"
#include "product/composed_machine.h"

#include <assert.h>
#include <stdio.h>

struct common_ui { common_ui_options options; };

static common_ui surface;
static lib_u32 scenario;
static lib_u32 requests;
static lib_u32 destroyed;
static lib_u32 reported;

static void create_image(const char *path)
{
    const unsigned char sector[512] = {0};
    FILE *file = fopen(path, "wb");

    assert(file != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fclose(file) == 0);
}

lib_status common_ui_create(common_ui **out_ui, const common_ui_options *options)
{
    surface.options = *options;
    *out_ui = &surface;
    return LIB_STATUS_OK;
}

lib_status common_ui_destroy(common_ui *ui)
{
    assert(ui == &surface && requests == 1u);
    ++destroyed;
    return scenario == 3u ? LIB_STATUS_IO_ERROR : LIB_STATUS_OK;
}

lib_status common_ui_request_monitor_line(common_ui *ui)
{
    common_ui_event event = {0};

    assert(ui == &surface && ++requests == 1u);
    if (scenario == 0u || scenario == 3u) {
        event.kind = COMMON_UI_EVENT_MONITOR_LINE;
        lib_memory_copy(event.value.line.text, "exit", 5u);
        event.value.line.length = 4u;
    } else if (scenario == 1u) {
        event.kind = COMMON_UI_EVENT_CONSOLE_FAILED;
    } else {
        event.kind = COMMON_UI_EVENT_KVM_DELIVERY_FAILED;
        event.value.delivery_failure.source_identity = 1u;
        event.value.delivery_failure.status = LIB_STATUS_IO_ERROR;
    }
    assert(surface.options.event_sink(surface.options.event_context, &event));
    return LIB_STATUS_OK;
}

lib_status common_ui_write_monitor(common_ui *ui, const char *text)
{
    assert(ui == &surface);
    if (lib_text_find_substring(text, "input failed") ||
        lib_text_find_substring(text, "delivery failed")) ++reported;
    return LIB_STATUS_OK;
}

void common_ui_set_run_generation(common_ui *ui, lib_u32 generation)
{ (void)ui; (void)generation; assert(0); }

lib_status common_ui_apply_action(common_ui *ui, common_ui_action action,
    common_ui_state state)
{ (void)ui; (void)action; (void)state; assert(0); return LIB_STATUS_IO_ERROR; }

lib_status common_ui_set_state(common_ui *ui, common_ui_state state)
{ (void)ui; (void)state; assert(0); return LIB_STATUS_IO_ERROR; }

lib_status common_ui_publish_frame(common_ui *ui, const kvm_window_frame *frame,
    const kvm_console_character_map *characters, lib_u32 sequence,
    lib_bool window, lib_bool console, lib_bool status)
{
    (void)ui; (void)frame; (void)characters; (void)sequence;
    (void)window; (void)console; (void)status;
    assert(0);
    return LIB_STATUS_IO_ERROR;
}

lib_status common_ui_release_window_mouse(common_ui *ui)
{ (void)ui; assert(0); return LIB_STATUS_IO_ERROR; }

lib_status common_ui_cancel_monitor_line(common_ui *ui, lib_bool *completed)
{ (void)ui; (void)completed; assert(0); return LIB_STATUS_IO_ERROR; }

int main(void)
{
    const char *path = "presentation-shutdown.img";
    app_startup_config config = {0};
    product_surface_definition definition = {0};

    create_image(path);
    lib_text_copy(config.floppy_path, path);
    config.presentation = COMMON_SESSION_DISPLAY_WINDOW;
    config.floppy_mode = LIB_STORAGE_MEDIUM_READONLY;
    config.hard_disk_mode = LIB_STORAGE_MEDIUM_OVERLAY;
    assert(app_startup_compose_ui(&config, &definition.ui) == LIB_STATUS_OK);
    definition.name = "presentation-shutdown";
    for (scenario = 0u; scenario < 3u; ++scenario) {
        requests = destroyed = reported = 0u;
        assert(softpc_product_compose_machine(&config, &definition.machine) ==
            LIB_STATUS_OK);
        assert(product_surface_run(&definition) == (scenario == 0u ? 0 : 1));
        assert(destroyed == 1u && reported == (scenario == 1u || scenario == 2u));
    }
    assert(remove(path) == 0);

    scenario = 3u;
    requests = destroyed = reported = 0u;
    create_image(path);
    assert(softpc_product_compose_machine(&config, &definition.machine) ==
        LIB_STATUS_OK);
    assert(product_surface_run(&definition) == 1);
    assert(destroyed == 1u && reported == 0u);
    return 0;
}
