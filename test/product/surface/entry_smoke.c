#include "lib/types/types_interface.h"
#include "lib/types/file.h"
#include "lib/kvm-base/hotkey_interface.h"
#include "emulator/product/composition_interface.h"
#include "product/surface/entry_interface.h"

struct emulator_product { lib_bool live; };

static struct emulator_product fixture;
static lib_u32 failure;
static lib_u32 created;
static lib_u32 destroyed;
static char opening[128];

int product_surface_entry_smoke_printf(const char *format, ...)
{
    lib_c_va_list arguments;
    int result;

    lib_c_va_start(arguments, format);
    result = lib_c_vsnprintf(opening, sizeof(opening), format, arguments);
    lib_c_va_end(arguments);
    return result;
}

static lib_status fixture_bind(void *machine, emulator_machine *emulator)
{
    (void)machine;
    (void)emulator;
    return LIB_STATUS_OK;
}

static lib_status fixture_destroy(void *machine)
{
    (void)machine;
    ++destroyed;
    return LIB_STATUS_OK;
}

lib_status emulator_product_create(const emulator_product_machine *machine,
    emulator_product **out_app)
{
    (void)machine;
    ++created;
    if (failure == 1u) return LIB_STATUS_NO_MEMORY;
    fixture.live = LIB_TRUE;
    *out_app = &fixture;
    return LIB_STATUS_OK;
}

lib_status emulator_product_destroy(emulator_product *app)
{
    if (app == LIB_NULL) return LIB_STATUS_OK;
    app->live = LIB_FALSE;
    return failure == 6u ? LIB_STATUS_INTERNAL_ERROR : LIB_STATUS_OK;
}

lib_bool emulator_session_enqueue_ui_event(void *context,
    const emulator_ui_event *event)
{
    (void)context;
    (void)event;
    return LIB_TRUE;
}

lib_status emulator_product_compose_machine(emulator_product *app)
{
    (void)app;
    return failure == 2u ? LIB_STATUS_INTERNAL_ERROR : LIB_STATUS_OK;
}

lib_status product_surface_command_provider_initialize(product_surface_command_context *command,
    emulator_machine *machine, emulator_session_display display,
    const product_surface_command_extensions *extensions,
    emulator_session_command_provider *provider)
{
    (void)command;
    (void)machine;
    (void)display;
    (void)extensions;
    (void)provider;
    return failure == 3u ? LIB_STATUS_INTERNAL_ERROR : LIB_STATUS_OK;
}

lib_bool product_surface_keyboard_hotkeys(kvm_hotkey_registry *registry)
{
    (void)registry;
    return failure == 4u ? LIB_FALSE : LIB_TRUE;
}

lib_status emulator_product_compose_control(emulator_product *app,
    const emulator_session_options *options)
{
    (void)app;
    (void)options;
    return failure == 5u ? LIB_STATUS_INTERNAL_ERROR : LIB_STATUS_OK;
}

lib_status emulator_product_compose_ui(emulator_product *app,
    const emulator_ui_options *options)
{
    (void)app;
    (void)options;
    return LIB_STATUS_OK;
}

lib_bool emulator_session_run(emulator_session *session)
{
    (void)session;
    return LIB_TRUE;
}

lib_i32 emulator_product_run(const emulator_product_definition *definition)
{
    emulator_product product = {0};
    emulator_session_options session_options = {0};
    emulator_ui_options ui_options = {0};

    if (definition == LIB_NULL || definition->configure_control == LIB_NULL ||
        definition->configure_ui == LIB_NULL) return 1;
    ++created;
    if (failure == 1u) {
        ++destroyed;
        return 1;
    }
    if (failure == 2u || failure == 5u) return 1;
    fixture.live = LIB_TRUE;
    if (definition->configure_control(definition->context, (emulator_machine *)&product,
            &session_options) != LIB_STATUS_OK ||
        definition->configure_ui(definition->context, &ui_options) != LIB_STATUS_OK) {
        fixture.live = LIB_FALSE;
        return 1;
    }
    fixture.live = LIB_FALSE;
    return failure == 6u ? 1 : 0;
}

void product_surface_command_dispose(product_surface_command_context *command)
{ (void)command; }

emulator_product_help_map product_surface_keyboard_hotkey_help(void)
{ return (emulator_product_help_map){LIB_NULL, 0u}; }

lib_i32 main(void)
{
    const product_surface_definition definition = {
        .name = "PC",
        .machine = {.composition = {.machine = &fixture, .bind = fixture_bind,
            .destroy = fixture_destroy}},
        .ui = {.display = EMULATOR_SESSION_DISPLAY_CONSOLE}
    };
    product_surface_definition invalid_ui = definition;
    lib_u32 index;
    lib_size opening_length;

    invalid_ui.ui.display = (emulator_session_display)99;
    if (product_surface_run(LIB_NULL) != 1 || product_surface_run(&invalid_ui) != 1) return 1;
    for (index = 0u; index <= 6u; ++index) {
        failure = index;
        fixture.live = LIB_FALSE;
        created = destroyed = 0u;
        opening[0] = '\0';
        if (product_surface_run(&definition) != (index == 0u ? 0 : 1)) return 2;
        if (index == 1u && (created != 1u || destroyed != 1u)) return 3;
        if (index != 1u && (created != 1u || destroyed != 0u)) return 4;
        if (fixture.live) return 5;
        opening_length = lib_text_length(opening);
        if (lib_text_find_substring(opening, "PC\n\nBuilt on ") != opening ||
            opening_length < 2u || opening[opening_length - 2u] != '\n' ||
            opening[opening_length - 1u] != '\n') return 6;
    }
    return 0;
}
lib_status emulator_product_monitor_format_window_titles(const char *name,
    char *out_running, lib_size running_capacity, char *out_paused,
    lib_size paused_capacity)
{
    return lib_c_snprintf(out_running, running_capacity, "%s", name) < 0 ||
        lib_c_snprintf(out_paused, paused_capacity, "%s", name) < 0 ?
        LIB_STATUS_LIMIT_EXCEEDED : LIB_STATUS_OK;
}

lib_status emulator_product_monitor_format_window_status(const char *name,
    emulator_product_help_map hotkeys, char *out_text, lib_size capacity)
{
    (void)hotkeys;
    return lib_c_snprintf(out_text, capacity, "%s", name) < 0 ?
        LIB_STATUS_LIMIT_EXCEEDED : LIB_STATUS_OK;
}
