#include "lib/types/types_interface.h"
#include "lib/types/file.h"
#include "ibmpc/product/entry_interface.h"

struct vm_app { lib_bool live; };

static struct vm_app fixture;
static lib_u32 failure;
static lib_u32 created;
static lib_u32 destroyed;

static lib_status fixture_bind(void *machine, common_machine *common)
{
    (void)machine;
    (void)common;
    return LIB_STATUS_OK;
}

static void fixture_destroy(void *machine)
{
    (void)machine;
    ++destroyed;
}

lib_status vm_app_create(const app_composed_machine *machine, vm_app **out_app)
{
    (void)machine;
    ++created;
    if (failure == 1u) return LIB_STATUS_NO_MEMORY;
    fixture.live = LIB_TRUE;
    *out_app = &fixture;
    return LIB_STATUS_OK;
}

lib_status vm_app_destroy(vm_app *app)
{
    if (app == LIB_NULL) return LIB_STATUS_OK;
    app->live = LIB_FALSE;
    return failure == 6u ? LIB_STATUS_INTERNAL_ERROR : LIB_STATUS_OK;
}

common_session *vm_app_session(const vm_app *app)
{ return app == LIB_NULL ? LIB_NULL : (common_session *)app; }

common_machine *vm_app_common_machine(const vm_app *app)
{ return app == LIB_NULL ? LIB_NULL : (common_machine *)app; }

common_ui *vm_app_ui(const vm_app *app)
{ return app == LIB_NULL ? LIB_NULL : (common_ui *)app; }

lib_bool common_session_enqueue_ui_event(void *context,
    const common_ui_event *event)
{
    (void)context;
    (void)event;
    return LIB_TRUE;
}

lib_status vm_app_compose_machine(vm_app *app)
{
    (void)app;
    return failure == 2u ? LIB_STATUS_INTERNAL_ERROR : LIB_STATUS_OK;
}

lib_status app_command_provider_initialize(app_command_context *command,
    common_machine *machine, common_session_display display,
    const app_command_extensions *extensions,
    common_session_command_provider *provider)
{
    (void)command;
    (void)machine;
    (void)display;
    (void)extensions;
    (void)provider;
    return failure == 3u ? LIB_STATUS_INTERNAL_ERROR : LIB_STATUS_OK;
}

lib_bool app_keyboard_hotkeys(kvm_hotkey_registry *registry)
{
    (void)registry;
    return failure == 4u ? LIB_FALSE : LIB_TRUE;
}

lib_status vm_app_compose_control(vm_app *app, const common_session_options *options)
{
    (void)app;
    (void)options;
    return failure == 5u ? LIB_STATUS_INTERNAL_ERROR : LIB_STATUS_OK;
}

lib_status vm_app_compose_ui(vm_app *app, const common_ui_options *options)
{
    (void)app;
    (void)options;
    return LIB_STATUS_OK;
}

lib_bool common_session_run(common_session *session)
{
    (void)session;
    return LIB_TRUE;
}

void app_command_dispose(app_command_context *command)
{ (void)command; }

const char *app_command_hotkey_help(void)
{ return "hotkeys"; }

lib_i32 main(void)
{
    const vm_app_definition definition = {
        .name = "PC",
        .machine = {.machine = &fixture, .bind = fixture_bind,
            .destroy = fixture_destroy},
        .ui = {.display = COMMON_SESSION_DISPLAY_CONSOLE}
    };
    vm_app_definition invalid_ui = definition;
    lib_u32 index;

    invalid_ui.ui.display = (common_session_display)99;
    if (vm_app_run(LIB_NULL) != 1 || vm_app_run(&invalid_ui) != 1) return 1;
    for (index = 0u; index <= 6u; ++index) {
        failure = index;
        fixture.live = LIB_FALSE;
        created = destroyed = 0u;
        if (vm_app_run(&definition) != (index == 0u ? 0 : 1)) return 2;
        if (index == 1u && (created != 1u || destroyed != 1u)) return 3;
        if (index != 1u && (created != 1u || destroyed != 0u)) return 4;
        if (fixture.live) return 5;
    }
    return 0;
}
