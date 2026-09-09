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
    /* Each private mailbox owns a complete ux_frame.  Keep both test
       components out of the small default 32-bit thread stack. */
    static ux_component first;
    static ux_component second;
    static ux_component third;
    ux_component_options options = { 0 };
    ux_input_event event = { 0 };
    ux_component_control control = { UX_COMPONENT_CONTROL_SET_WINDOW_MOUSE_ENABLED,
        { 0 } };
    ux_component_control taken;
    ux_component_control disable_controls[2] = {
        { UX_COMPONENT_CONTROL_SET_WINDOW_MOUSE_ENABLED, { 0 } },
        { UX_COMPONENT_CONTROL_RELEASE_WINDOW_MOUSE, { 0 } }
    };
    atomic_uint_fast64_t identity_next;
    lib_u64 identity;
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
    assert(ux_component_initialize(&third, &options, component_probe_stop,
        component_probe_dispose) == LIB_STATUS_OK);
    assert(first.source_identity != 0u);
    assert(second.source_identity != 0u);
    assert(first.source_identity != second.source_identity);

    /* Source identity is a single non-repeating epoch: issuing the final
       representable value permanently exhausts it instead of wrapping. */
    atomic_init(&identity_next, UINT64_MAX - 1u);
    assert(ux_component_allocate_source_identity(&identity_next, &identity) ==
        LIB_STATUS_OK && identity == UINT64_MAX - 1u);
    assert(ux_component_allocate_source_identity(&identity_next, &identity) ==
        LIB_STATUS_OK && identity == UINT64_MAX);
    assert(ux_component_allocate_source_identity(&identity_next, &identity) ==
        LIB_STATUS_LIMIT_EXCEEDED);
    assert(atomic_load_explicit(&identity_next, memory_order_relaxed) == 0u);

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
        assert(ux_component_enqueue_controls(&second, &control, 1u) ==
            LIB_STATUS_OK);
    /* A full ordinary FIFO rejects the next request and retains every
       original record. The failure is reported as well as returned. */
    assert(ux_component_enqueue_controls(&second, &control, 1u) ==
        LIB_STATUS_LIMIT_EXCEEDED);
    assert(probe.failure_count == 2u);
    assert(probe.last_failure == LIB_STATUS_LIMIT_EXCEEDED);
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

    /* Multi-record control requests are all-or-nothing. This is the exact
       shape used by Window disable-mouse: no disabled flag may be left queued
       without its following release when only one ordinary slot remains. */
    for (index = 0u; index + 1u < UX_COMPONENT_CONTROL_CAPACITY; ++index)
        assert(ux_component_enqueue_controls(&third, &control, 1u) ==
            LIB_STATUS_OK);
    disable_controls[0].value.window_mouse_enabled = LIB_FALSE;
    assert(ux_component_enqueue_controls(&third, disable_controls, 2u) ==
        LIB_STATUS_LIMIT_EXCEEDED);
    assert(probe.failure_count == 3u);
    assert(probe.last_failure == LIB_STATUS_LIMIT_EXCEEDED);
    for (index = 0u; index + 1u < UX_COMPONENT_CONTROL_CAPACITY; ++index) {
        assert(ux_component_mailboxes_take_control(&third.mailboxes, &taken));
        assert(taken.kind == UX_COMPONENT_CONTROL_SET_WINDOW_MOUSE_ENABLED);
        assert(taken.value.window_mouse_enabled == LIB_FALSE);
    }
    assert(!ux_component_mailboxes_take_control(&third.mailboxes, &taken));

    ux_component_destroy(&first);
    ux_component_destroy(&second);
    ux_component_destroy(&third);
    return 0;
}
