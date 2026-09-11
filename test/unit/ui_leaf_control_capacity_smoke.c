#include "lib/ui-window/window.h"
#include "lib/ui-console/console.h"

#include <assert.h>

typedef struct leaf_probe {
    unsigned int failures;
    lib_status last_failure;
} leaf_probe;

static int leaf_input(void *opaque, const ui_input_event *event)
{ (void)opaque; (void)event; return 1; }

static void leaf_failure(void *opaque, lib_u64 identity, lib_status status)
{
    leaf_probe *probe = (leaf_probe *)opaque;
    (void)identity;
    assert(probe != LIB_NULL);
    ++probe->failures;
    probe->last_failure = status;
}

static void leaf_stop(ui_component *component)
{ (void)component; }

static void leaf_dispose(ui_component *component)
{ ui_component_mailboxes_destroy(&component->mailboxes); }

static void leaf_drain(ui_component *component)
{
    ui_component_control control;
    unsigned int count = 0u;

    while (ui_component_mailboxes_take_control(&component->mailboxes, &control)) {
        assert(control.kind == UI_COMPONENT_CONTROL_SET_WINDOW_TITLE);
        assert(control.value.title[0] == 'x');
        ++count;
    }
    assert(count == UI_COMPONENT_CONTROL_CAPACITY);
}

int main(void)
{
    /* Each leaf embeds a complete copied-frame mailbox; keep these out of the
       small default Windows test-thread stack. */
    static ui_window window;
    static ui_console console;
    ui_component_options options = { 0 };
    ui_component_control title = { UI_COMPONENT_CONTROL_SET_WINDOW_TITLE, { 0 } };
    leaf_probe window_probe = { 0 };
    leaf_probe console_probe = { 0 };
    unsigned int index;

    options.input_sink = leaf_input;
    options.failure_sink = leaf_failure;
    options.failure_context = &window_probe;
    assert(ui_component_initialize(&window.base, &options, leaf_stop,
        leaf_dispose) == LIB_STATUS_OK);
    for (index = 0u; index < UI_COMPONENT_CONTROL_CAPACITY; ++index)
        assert(ui_window_set_title(&window, "x") == LIB_STATUS_OK);
    assert(ui_window_set_title(&window, "x") == LIB_STATUS_LIMIT_EXCEEDED);
    assert(window_probe.failures == 1u);
    assert(window_probe.last_failure == LIB_STATUS_LIMIT_EXCEEDED);
    leaf_drain(&window.base);

    /* Window freeze is one atomic ordered control batch: first it makes future
       capture impossible, then it releases current capture. Unfreeze is one
       independent permission change and never implies a capture request. */
    {
        ui_component_control taken;
        assert(ui_window_freeze(&window) == LIB_STATUS_OK);
        assert(ui_component_mailboxes_take_control(&window.base.mailboxes,
            &taken));
        assert(taken.kind == UI_COMPONENT_CONTROL_SET_WINDOW_FROZEN);
        assert(taken.value.window_frozen == LIB_TRUE);
        assert(ui_component_mailboxes_take_control(&window.base.mailboxes,
            &taken));
        assert(taken.kind == UI_COMPONENT_CONTROL_RELEASE_WINDOW_MOUSE);
        assert(!ui_component_mailboxes_take_control(&window.base.mailboxes,
            &taken));
        assert(ui_window_unfreeze(&window) == LIB_STATUS_OK);
        assert(ui_component_mailboxes_take_control(&window.base.mailboxes,
            &taken));
        assert(taken.kind == UI_COMPONENT_CONTROL_SET_WINDOW_FROZEN);
        assert(taken.value.window_frozen == LIB_FALSE);
        assert(!ui_component_mailboxes_take_control(&window.base.mailboxes,
            &taken));
    }

    options.failure_context = &console_probe;
    assert(ui_component_initialize(&console.base, &options, leaf_stop,
        leaf_dispose) == LIB_STATUS_OK);
    title.value.title[0] = 'x';
    for (index = 0u; index < UI_COMPONENT_CONTROL_CAPACITY; ++index)
        assert(ui_component_enqueue_controls(&console.base, &title, 1u) ==
            LIB_STATUS_OK);
    assert(ui_component_enqueue_controls(&console.base, &title, 1u) ==
        LIB_STATUS_LIMIT_EXCEEDED);
    assert(console_probe.failures == 1u);
    assert(console_probe.last_failure == LIB_STATUS_LIMIT_EXCEEDED);
    leaf_drain(&console.base);

    /* These are synthetic leaf storage objects: no native worker was started,
       so capacity cleanup destroys only the manually initialized mailboxes.
       Real STOP/destroy ordering is covered by the retirement barrier test. */
    ui_component_mailboxes_destroy(&window.base.mailboxes);
    ui_component_mailboxes_destroy(&console.base.mailboxes);
    return 0;
}
