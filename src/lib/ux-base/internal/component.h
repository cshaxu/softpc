#ifndef UX_BASE_INTERNAL_COMPONENT_H
#define UX_BASE_INTERNAL_COMPONENT_H

#include "lib/ux-base/component.h"
#include "lib/ux-base/internal/mailbox.h"

typedef void (*ux_component_native_stop_fn)(ux_component *component);
typedef void (*ux_component_dispose_fn)(ux_component *component);

struct ux_component {
    ux_component_mailboxes mailboxes;
    void *input_context;
    ux_input_sink input_sink;
    ux_hotkey_matcher hotkey_matcher;
    atomic_int stopping;
    ux_component_native_stop_fn native_stop;
    ux_component_dispose_fn dispose;
};

lib_status ux_component_initialize(ux_component *component,
    const ux_component_options *options, ux_component_native_stop_fn native_stop,
    ux_component_dispose_fn dispose);
int ux_component_emit(ux_component *component, const ux_input_event *event);
void ux_component_emit_source_retired(ux_component *component);

#endif
