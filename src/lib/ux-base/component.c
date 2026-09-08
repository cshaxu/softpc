#include "lib/ux-base/internal/component.h"

static atomic_uint_fast64_t ux_component_next_source_identity = 1u;

static void ux_component_report_failure(ux_component *component, lib_status status)
{
    if (component != LIB_NULL && component->failure_sink != LIB_NULL &&
        status != LIB_STATUS_OK)
        component->failure_sink(component->failure_context,
            component->source_identity, status);
}

lib_status ux_component_initialize(ux_component *component,
    const ux_component_options *options, ux_component_native_stop_fn native_stop,
    ux_component_dispose_fn dispose)
{
    lib_u64 identity;
    if (component == LIB_NULL || options == LIB_NULL || options->input_sink == LIB_NULL ||
        options->failure_sink == LIB_NULL ||
        native_stop == LIB_NULL || dispose == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    component->input_context = options->input_context;
    component->input_sink = options->input_sink;
    component->failure_context = options->failure_context;
    component->failure_sink = options->failure_sink;
    component->native_stop = native_stop;
    component->dispose = dispose;
    identity = atomic_fetch_add_explicit(
        &ux_component_next_source_identity, 1u, memory_order_relaxed);
    if (identity == 0u) return LIB_STATUS_LIMIT_EXCEEDED;
    component->source_identity = identity;
    ux_hotkey_matcher_initialize(&component->hotkey_matcher, &options->hotkeys);
    atomic_init(&component->stopping, 0);
    return ux_component_mailboxes_create(&component->mailboxes);
}

int ux_component_emit(ux_component *component, const ux_input_event *event)
{
    ux_input_event copied;
    if (component == LIB_NULL || event == LIB_NULL || component->input_sink == LIB_NULL ||
        atomic_load_explicit(&component->stopping, memory_order_acquire) != 0) return 0;
    copied = *event;
    ux_input_event_set_source(&copied, component, component->source_identity);
    if (!ux_hotkey_matcher_submit(&component->hotkey_matcher, &copied,
            component->input_sink, component->input_context)) {
        ux_component_report_failure(component, LIB_STATUS_IO_ERROR);
        return 0;
    }
    return 1;
}

void ux_component_emit_source_retired(ux_component *component)
{
    ux_input_event event = { 0 };
    if (component == LIB_NULL || component->input_sink == LIB_NULL) return;
    event.type = UX_EVENT_SOURCE_RETIRED;
    ux_input_event_set_source(&event, component, component->source_identity);
    atomic_store_explicit(&component->stopping, 1, memory_order_release);
    if (!component->input_sink(component->input_context, &event))
        ux_component_report_failure(component, LIB_STATUS_IO_ERROR);
}

lib_status ux_component_publish_frame(ux_component *component, const ux_frame *frame)
{
    return component == LIB_NULL ? LIB_STATUS_INVALID_ARGUMENT :
        ux_component_mailboxes_publish_frame(&component->mailboxes, frame);
}

lib_status ux_component_request_stop(ux_component *component)
{
    ux_component_control control = { UX_COMPONENT_CONTROL_STOP, { 0 } };
    return component == LIB_NULL ? LIB_STATUS_INVALID_ARGUMENT :
        ux_component_mailboxes_enqueue_control(&component->mailboxes, &control);
}

void ux_component_destroy(ux_component *component)
{
    if (component == LIB_NULL) return;
    if (ux_component_request_stop(component) != LIB_STATUS_OK) {
        ux_component_report_failure(component, LIB_STATUS_INVALID_STATE);
        return;
    }
    component->native_stop(component);
    component->dispose(component);
}
