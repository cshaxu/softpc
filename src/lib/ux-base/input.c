#include "lib/ux-base/input.h"

struct ux_hotkey_matcher {
    ux_hotkey_registration registrations[UX_HOTKEY_CAPACITY];
    lib_u32 registration_count;
    ux_input_event pending[3u];
    lib_u32 pending_count;
    lib_u32 suppressed_modifiers;
    lib_u32 suppressed_key;
};

static lib_bool ux_hotkey_is_modifier(lib_u32 key, lib_u32 *out_modifier)
{
    lib_u32 modifier = 0u;

    if (key == UX_KEY_CONTROL) modifier = UX_MODIFIER_CONTROL;
    else if (key == UX_KEY_ALT) modifier = UX_MODIFIER_ALT;
    else if (key == UX_KEY_SHIFT) modifier = UX_MODIFIER_SHIFT;
    if (out_modifier != LIB_NULL) *out_modifier = modifier;
    return modifier != 0u;
}

static lib_status ux_hotkey_emit(ux_input_sink sink, void *context,
    const ux_input_event *event)
{
    return sink == LIB_NULL || event == LIB_NULL ? LIB_STATUS_INVALID_ARGUMENT :
        sink(context, event);
}

static lib_status ux_hotkey_flush(ux_hotkey_matcher *matcher,
    ux_input_sink sink, void *context)
{
    lib_u32 index;
    lib_status status;

    for (index = 0u; index < matcher->pending_count; ++index) {
        status = ux_hotkey_emit(sink, context, &matcher->pending[index]);
        if (status != LIB_STATUS_OK) return status;
    }
    matcher->pending_count = 0u;
    return LIB_STATUS_OK;
}

static const ux_hotkey_registration *ux_hotkey_find(
    const ux_hotkey_matcher *matcher, lib_u32 key, lib_u32 modifiers)
{
    lib_u32 index;

    for (index = 0u; index < matcher->registration_count; ++index) {
        const ux_hotkey_registration *entry = &matcher->registrations[index];
        if (entry->virtual_key == key && entry->modifiers == modifiers)
            return entry;
    }
    return LIB_NULL;
}

static lib_bool ux_hotkey_modifier_is_prefix(const ux_hotkey_matcher *matcher,
    lib_u32 modifier)
{
    lib_u32 index;

    for (index = 0u; index < matcher->registration_count; ++index) {
        if ((matcher->registrations[index].modifiers & modifier) != 0u)
            return LIB_TRUE;
    }
    return LIB_FALSE;
}

static lib_status ux_input_set(ux_input_event *event, const void *source,
    ux_input_kind kind)
{
    if (event == LIB_NULL || source == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    memset(event, 0, sizeof(*event));
    event->source_handle = source;
    event->kind = kind;
    return LIB_STATUS_OK;
}

lib_status ux_input_make_key(ux_input_event *out_event,
    const void *source_handle, lib_u16 scan_code, lib_u32 virtual_key,
    lib_u32 modifiers, lib_bool pressed)
{
    lib_status status = ux_input_set(out_event, source_handle, UX_INPUT_KEY);

    if (status != LIB_STATUS_OK || virtual_key == 0u) return LIB_STATUS_INVALID_ARGUMENT;
    out_event->value.key.scan_code = scan_code;
    out_event->value.key.virtual_key = virtual_key;
    out_event->value.key.modifiers = modifiers;
    out_event->value.key.pressed = pressed != LIB_FALSE;
    return LIB_STATUS_OK;
}

lib_status ux_input_make_hotkey(ux_input_event *out_event,
    const void *source_handle, const char *identifier)
{
    const char *end;
    lib_status status = ux_input_set(out_event, source_handle, UX_INPUT_HOTKEY);

    if (status != LIB_STATUS_OK || identifier == LIB_NULL ||
        (end = memchr(identifier, '\0', UX_HOTKEY_IDENTIFIER_CAPACITY)) == LIB_NULL)
        return LIB_STATUS_INVALID_ARGUMENT;
    memcpy(out_event->value.hotkey.identifier, identifier,
        (lib_size)(end - identifier) + 1u);
    return LIB_STATUS_OK;
}

lib_status ux_input_make_window_close(ux_input_event *out_event,
    const void *source_handle)
{
    return ux_input_set(out_event, source_handle, UX_INPUT_WINDOW_CLOSE_REQUESTED);
}

lib_status ux_input_make_reset(ux_input_event *out_event,
    const void *source_handle)
{
    return ux_input_set(out_event, source_handle, UX_INPUT_RESET);
}

lib_status ux_hotkey_matcher_create(ux_hotkey_matcher **out_matcher,
    const ux_hotkey_registration *registrations, lib_u32 registration_count)
{
    ux_hotkey_matcher *matcher;
    lib_u32 index;

    if (out_matcher == LIB_NULL || (registration_count != 0u &&
        registrations == LIB_NULL) || registration_count > UX_HOTKEY_CAPACITY)
        return LIB_STATUS_INVALID_ARGUMENT;
    *out_matcher = LIB_NULL;
    matcher = calloc(1u, sizeof(*matcher));
    if (matcher == LIB_NULL) return LIB_STATUS_NO_MEMORY;
    for (index = 0u; index < registration_count; ++index) {
        const ux_hotkey_registration *entry = &registrations[index];
        if (entry->virtual_key == 0u || memchr(entry->identifier, '\0',
            sizeof(entry->identifier)) == LIB_NULL) {
            free(matcher);
            return LIB_STATUS_INVALID_ARGUMENT;
        }
        matcher->registrations[index] = *entry;
    }
    matcher->registration_count = registration_count;
    *out_matcher = matcher;
    return LIB_STATUS_OK;
}

void ux_hotkey_matcher_destroy(ux_hotkey_matcher *matcher)
{
    free(matcher);
}

lib_status ux_hotkey_matcher_submit(ux_hotkey_matcher *matcher,
    const ux_input_event *event, ux_input_sink sink, void *sink_context)
{
    const ux_hotkey_registration *match;
    lib_u32 modifier;
    lib_status status;

    if (matcher == LIB_NULL || event == LIB_NULL || sink == LIB_NULL ||
        event->source_handle == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    if (event->kind != UX_INPUT_KEY) {
        status = ux_hotkey_flush(matcher, sink, sink_context);
        return status != LIB_STATUS_OK ? status : ux_hotkey_emit(sink, sink_context, event);
    }
    if (event->value.key.pressed == LIB_FALSE) {
        if (event->value.key.virtual_key == matcher->suppressed_key) {
            matcher->suppressed_key = 0u;
            return LIB_STATUS_OK;
        }
        if (ux_hotkey_is_modifier(event->value.key.virtual_key, &modifier) &&
            (matcher->suppressed_modifiers & modifier) != 0u) {
            matcher->suppressed_modifiers &= ~modifier;
            return LIB_STATUS_OK;
        }
        status = ux_hotkey_flush(matcher, sink, sink_context);
        return status != LIB_STATUS_OK ? status : ux_hotkey_emit(sink, sink_context, event);
    }
    if (ux_hotkey_is_modifier(event->value.key.virtual_key, &modifier) &&
        ux_hotkey_modifier_is_prefix(matcher, modifier)) {
        if (matcher->pending_count == (lib_u32)(sizeof(matcher->pending) /
                sizeof(matcher->pending[0]))) return LIB_STATUS_LIMIT_EXCEEDED;
        matcher->pending[matcher->pending_count++] = *event;
        return LIB_STATUS_OK;
    }
    match = ux_hotkey_find(matcher, event->value.key.virtual_key,
        event->value.key.modifiers);
    if (match != LIB_NULL) {
        ux_input_event hotkey;
        matcher->pending_count = 0u;
        matcher->suppressed_modifiers = event->value.key.modifiers;
        matcher->suppressed_key = event->value.key.virtual_key;
        status = ux_input_make_hotkey(&hotkey, event->source_handle,
            match->identifier);
        return status != LIB_STATUS_OK ? status : ux_hotkey_emit(sink, sink_context, &hotkey);
    }
    status = ux_hotkey_flush(matcher, sink, sink_context);
    return status != LIB_STATUS_OK ? status : ux_hotkey_emit(sink, sink_context, event);
}

lib_status ux_hotkey_matcher_retire(ux_hotkey_matcher *matcher,
    const void *source_handle, ux_input_sink sink, void *sink_context)
{
    ux_input_event reset;
    lib_status status;

    if (matcher == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    status = ux_hotkey_flush(matcher, sink, sink_context);
    if (status != LIB_STATUS_OK) return status;
    matcher->suppressed_modifiers = 0u;
    matcher->suppressed_key = 0u;
    status = ux_input_make_reset(&reset, source_handle);
    return status != LIB_STATUS_OK ? status : ux_hotkey_emit(sink, sink_context, &reset);
}
