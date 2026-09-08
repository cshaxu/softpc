#include "lib/ux-base/internal/component.h"

lib_status ux_component_initialize(ux_component *component,
    const ux_component_options *options, ux_component_native_stop_fn native_stop,
    ux_component_dispose_fn dispose)
{
    if (component == LIB_NULL || options == LIB_NULL || options->input_sink == LIB_NULL ||
        native_stop == LIB_NULL || dispose == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    component->input_context = options->input_context;
    component->input_sink = options->input_sink;
    component->native_stop = native_stop;
    component->dispose = dispose;
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
    ux_input_event_set_source(&copied, component);
    return ux_hotkey_matcher_submit(&component->hotkey_matcher, &copied,
        component->input_sink, component->input_context);
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
    (void)ux_component_request_stop(component);
    component->native_stop(component);
    component->dispose(component);
}
