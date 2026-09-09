#include "lib/ux-window/internal.h"
#include "lib/ux-console/internal.h"

#include <assert.h>

typedef struct leaf_probe {
    unsigned int failures;
    lib_status last_failure;
} leaf_probe;

static int leaf_input(void *opaque, const ux_input_event *event)
{ (void)opaque; (void)event; return 1; }

static void leaf_failure(void *opaque, lib_u64 identity, lib_status status)
{
    leaf_probe *probe = (leaf_probe *)opaque;
    (void)identity;
    assert(probe != LIB_NULL);
    ++probe->failures;
    probe->last_failure = status;
}

static void leaf_stop(ux_component *component)
{ (void)component; }

static void leaf_dispose(ux_component *component)
{ ux_component_mailboxes_destroy(&component->mailboxes); }

static void leaf_drain(ux_component *component)
{
    ux_component_control control;
    unsigned int count = 0u;

    while (ux_component_mailboxes_take_control(&component->mailboxes, &control)) {
        assert(control.kind == UX_COMPONENT_CONTROL_SET_WINDOW_TITLE);
        assert(control.value.title[0] == 'x');
        ++count;
    }
    assert(count == UX_COMPONENT_CONTROL_CAPACITY);
}

int main(void)
{
    /* Each leaf embeds a complete copied-frame mailbox; keep these out of the
       small default Windows test-thread stack. */
    static ux_window window;
    static ux_console console;
    ux_component_options options = { 0 };
    ux_component_control title = { UX_COMPONENT_CONTROL_SET_WINDOW_TITLE, { 0 } };
    leaf_probe window_probe = { 0 };
    leaf_probe console_probe = { 0 };
    unsigned int index;

    options.input_sink = leaf_input;
    options.failure_sink = leaf_failure;
    options.failure_context = &window_probe;
    assert(ux_component_initialize(&window.base, &options, leaf_stop,
        leaf_dispose) == LIB_STATUS_OK);
    for (index = 0u; index < UX_COMPONENT_CONTROL_CAPACITY; ++index)
        assert(ux_window_set_title(&window, "x") == LIB_STATUS_OK);
    assert(ux_window_set_title(&window, "x") == LIB_STATUS_LIMIT_EXCEEDED);
    assert(window_probe.failures == 1u);
    assert(window_probe.last_failure == LIB_STATUS_LIMIT_EXCEEDED);
    leaf_drain(&window.base);

    options.failure_context = &console_probe;
    assert(ux_component_initialize(&console.base, &options, leaf_stop,
        leaf_dispose) == LIB_STATUS_OK);
    title.value.title[0] = 'x';
    for (index = 0u; index < UX_COMPONENT_CONTROL_CAPACITY; ++index)
        assert(ux_component_enqueue_controls(&console.base, &title, 1u) ==
            LIB_STATUS_OK);
    assert(ux_component_enqueue_controls(&console.base, &title, 1u) ==
        LIB_STATUS_LIMIT_EXCEEDED);
    assert(console_probe.failures == 1u);
    assert(console_probe.last_failure == LIB_STATUS_LIMIT_EXCEEDED);
    leaf_drain(&console.base);

    /* These are synthetic leaf storage objects: no native worker was started,
       so capacity cleanup destroys only the manually initialized mailboxes.
       Real STOP/destroy ordering is covered by the retirement barrier test. */
    ux_component_mailboxes_destroy(&window.base.mailboxes);
    ux_component_mailboxes_destroy(&console.base.mailboxes);
    return 0;
}
