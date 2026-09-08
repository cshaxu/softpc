#include "lib/ux-base/internal/component.h"

#include <assert.h>

typedef struct component_probe {
    unsigned int input_count;
    unsigned int failure_count;
    lib_u64 last_identity;
    lib_status last_failure;
    int accept_input;
} component_probe;

static int component_probe_input(void *opaque, const ux_input_event *event)
{
    component_probe *probe = (component_probe *)opaque;
    if (probe == LIB_NULL || event == LIB_NULL || !probe->accept_input) return 0;
    ++probe->input_count;
    probe->last_identity = event->source_identity;
    return 1;
}

static void component_probe_failure(void *opaque, lib_u64 source_identity,
    lib_status status)
{
    component_probe *probe = (component_probe *)opaque;
    assert(probe != LIB_NULL);
    ++probe->failure_count;
    probe->last_identity = source_identity;
    probe->last_failure = status;
}

static void component_probe_stop(ux_component *component)
{ (void)component; }

static void component_probe_dispose(ux_component *component)
{ ux_component_mailboxes_destroy(&component->mailboxes); }

int main(void)
{
    component_probe probe = { 0 };
    ux_component first = { 0 };
    ux_component second = { 0 };
    ux_component_options options = { 0 };
    ux_input_event event = { 0 };
    ux_component_control control = { UX_COMPONENT_CONTROL_SET_WINDOW_MOUSE_ENABLED,
        { 0 } };
    ux_component_control taken;
    unsigned int index;

    probe.accept_input = 1;
    options.input_context = &probe;
    options.input_sink = component_probe_input;
    options.failure_context = &probe;
    options.failure_sink = component_probe_failure;
    assert(ux_component_initialize(&first, &options, component_probe_stop,
        component_probe_dispose) == LIB_STATUS_OK);
    assert(ux_component_initialize(&second, &options, component_probe_stop,
        component_probe_dispose) == LIB_STATUS_OK);
    assert(first.source_identity != 0u);
    assert(second.source_identity != 0u);
    assert(first.source_identity != second.source_identity);

    event.type = UX_EVENT_KEY;
    event.data.key.virtual_key = 'A';
    event.data.key.pressed = 1u;
    assert(ux_component_emit(&first, &event));
    assert(probe.input_count == 1u);
    assert(probe.last_identity == first.source_identity);
    probe.accept_input = 0;
    assert(!ux_component_emit(&first, &event));
    assert(probe.failure_count == 1u);
    assert(probe.last_failure == LIB_STATUS_IO_ERROR);

    for (index = 0u; index < UX_COMPONENT_CONTROL_CAPACITY; ++index)
        assert(ux_component_mailboxes_enqueue_control(&second.mailboxes,
            &control) == LIB_STATUS_OK);
    /* STOP has one reserved FIFO slot.  A full normal queue cannot make
       destroy wait forever for a stop record it could not enqueue. */
    assert(ux_component_request_stop(&second) == LIB_STATUS_OK);
    for (index = 0u; index < UX_COMPONENT_CONTROL_CAPACITY; ++index) {
        assert(ux_component_mailboxes_take_control(&second.mailboxes, &taken));
        assert(taken.kind == UX_COMPONENT_CONTROL_SET_WINDOW_MOUSE_ENABLED);
    }
    assert(ux_component_mailboxes_take_control(&second.mailboxes, &taken));
    assert(taken.kind == UX_COMPONENT_CONTROL_STOP);
    assert(!ux_component_mailboxes_take_control(&second.mailboxes, &taken));

    ux_component_destroy(&first);
    ux_component_destroy(&second);
    return 0;
}
