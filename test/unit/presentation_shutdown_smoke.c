/* Run real App composition, Session, command provider and machine worker.
 * Fake only UI: no native Console, boot or second failure boundary. */
#include "app/composition.h"
#include "common/ui/ui_interface.h"
#include "../lib/cleanup.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

struct common_ui { common_ui_options options; };
static common_ui surface;
static unsigned scenario, requests, destroyed, reported;

lib_status common_ui_create(common_ui **out, const common_ui_options *options)
{
    surface.options = *options;
    *out = &surface;
    return LIB_STATUS_OK;
}

lib_status common_ui_destroy(common_ui *ui)
{
    assert(ui == &surface && requests == 1u);
    ++destroyed;
    return LIB_STATUS_OK;
}

lib_status common_ui_request_monitor_line(common_ui *ui)
{
    common_ui_event event = { 0 };
    assert(ui == &surface && ++requests == 1u);
    if (scenario == 0u) {
        event.kind = COMMON_UI_EVENT_MONITOR_LINE;
        memcpy(event.value.line.text, "exit", 5u);
        event.value.line.length = 4u;
    } else if (scenario == 1u) {
        event.kind = COMMON_UI_EVENT_CONSOLE_FAILED;
    } else {
        event.kind = COMMON_UI_EVENT_KVM_DELIVERY_FAILED;
        event.value.delivery_failure.source_identity = 1u;
        event.value.delivery_failure.status = LIB_STATUS_IO_ERROR;
    }
    assert(ui->options.event_sink(ui->options.event_context, &event));
    return LIB_STATUS_OK;
}

lib_status common_ui_write_monitor(common_ui *ui, const char *text)
{
    assert(ui == &surface);
    if (strstr(text, "input failed") || strstr(text, "delivery failed")) ++reported;
    return LIB_STATUS_OK;
}

/* No run request is issued, so Session must not touch KVM state. */
void common_ui_set_run_generation(common_ui *ui, lib_u32 generation)
{ (void)ui; (void)generation; assert(0); }
lib_status common_ui_apply_action(common_ui *ui, common_ui_action a, common_ui_state s)
{ (void)ui; (void)a; (void)s; assert(0); return LIB_STATUS_IO_ERROR; }
lib_status common_ui_set_state(common_ui *ui, common_ui_state s)
{ (void)ui; (void)s; assert(0); return LIB_STATUS_IO_ERROR; }
lib_status common_ui_publish_frame(common_ui *ui, const kvm_frame *frame,
    lib_bool window, lib_bool console, lib_bool status)
{ (void)ui; (void)frame; (void)window; (void)console; (void)status;
  assert(0); return LIB_STATUS_IO_ERROR; }
lib_status common_ui_release_window_mouse(common_ui *ui)
{ (void)ui; assert(0); return LIB_STATUS_IO_ERROR; }
lib_status common_ui_cancel_monitor_line(common_ui *ui, lib_bool *completed)
{ (void)ui; (void)completed; assert(0); return LIB_STATUS_IO_ERROR; }

int main(void)
{
    const char *path = "presentation-shutdown.img";
    const unsigned char sector[512] = { 0 };
    FILE *file = fopen(path, "wb");
    app_startup_config config = { 0 };
    assert(file != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fclose(file) == 0);
    strcpy(config.floppy_path, path);
    config.presentation = COMMON_SESSION_DISPLAY_WINDOW;
    for (scenario = 0u; scenario < 3u; ++scenario) {
        requests = destroyed = reported = 0u;
        assert(app_composition_run(&config) ==
            (scenario == 0u ? LIB_STATUS_OK : LIB_STATUS_IO_ERROR));
        assert(destroyed == 1u && reported == (scenario != 0u));
    }
    assert(softpc_test_remove_image(path));
    return 0;
}
