/* Run real App composition, Session, command provider and machine worker.
 * Fake only UI: no native Console, boot or second failure boundary. */
#include "product/composition.h"
#include "common/ui/ui_interface.h"
#include "../unit/machine/cleanup.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>

struct common_ui { common_ui_options options; };
static common_ui surface;
static unsigned scenario, requests, destroyed, reported;
static jmp_buf terminal_exit;
static void checked_exit(int status)
{
    common_ui_event event = { 0 };
    assert(scenario == 3u && status == EXIT_FAILURE && destroyed == 1u);
    event.kind = COMMON_UI_EVENT_CONSOLE_FAILED;
    assert(surface.options.event_sink(surface.options.event_context, &event));
    longjmp(terminal_exit, 1);
}
static lib_status checked_session_destroy(common_session *session)
{
    assert(scenario != 3u); /* Failed UI destroy must not release its receiver. */
    return common_session_destroy(session);
}
#define exit checked_exit
#define common_session_destroy checked_session_destroy
#include "product/composition.c"
#undef common_session_destroy
#undef exit

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
    return scenario == 3u ? LIB_STATUS_IO_ERROR : LIB_STATUS_OK;
}

lib_status common_ui_request_monitor_line(common_ui *ui)
{
    common_ui_event event = { 0 };
    assert(ui == &surface && ++requests == 1u);
    if (scenario == 0u || scenario == 3u) {
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
lib_status common_ui_publish_frame(common_ui *ui, const kvm_window_frame *frame,
    const kvm_console_character_map *characters, lib_u32 sequence,
    lib_bool window, lib_bool console, lib_bool status)
{ (void)characters; (void)sequence; (void)ui; (void)frame; (void)window; (void)console; (void)status;
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
    /* Last case is terminal: process exit owns the retained dependency graph. */
    scenario = 3u;
    requests = destroyed = reported = 0u;
    if (setjmp(terminal_exit) == 0) {
        (void)app_composition_run(&config);
        assert(0);
    }
    assert(softpc_test_remove_image(path));
    return 0;
}
