#include "lib/ux/actions.h"
#include "lib/ux/frame.h"
#include "lib/ux/mailbox.h"
#include "lib/ux/router.h"

#include <assert.h>
#include <stdlib.h>

int main(void)
{
    ux_frame *frame = calloc(1u, sizeof(*frame));
    ux_frame *copied = calloc(1u, sizeof(*copied));
    ux_mailbox *mailbox = NULL;
    ux_action_registry actions;
    ux_router router;

    assert(frame != NULL && copied != NULL);
    frame->valid = 1u;
    frame->graphics = 0u;
    frame->text_columns = 80u;
    frame->text_rows = 25u;
    frame->text[0] = 'X';
    assert(ux_frame_is_valid(frame));

    ux_actions_initialize(&actions);
    assert(ux_actions_register(&actions, 'P', UX_MODIFIER_CONTROL |
        UX_MODIFIER_ALT, 7u) == LIB_STATUS_OK);
    assert(ux_actions_match(&actions, 'P', UX_MODIFIER_CONTROL |
        UX_MODIFIER_ALT) == 7u);
    assert(ux_actions_match(&actions, 'P', 0u) == UX_ACTION_NONE);

    ux_router_initialize(&router, UX_TARGET_CONSOLE);
    assert(ux_router_target(&router) == UX_TARGET_CONSOLE);
    ux_router_request(&router, UX_TARGET_WINDOW);
    assert(ux_router_target(&router) == UX_TARGET_WINDOW);

    assert(ux_mailbox_create(&mailbox) == LIB_STATUS_OK);
    assert(ux_mailbox_publish(mailbox, frame) == LIB_STATUS_OK);
    assert(ux_mailbox_generation(mailbox) == 1u);
    assert(ux_mailbox_capture(mailbox, copied) == LIB_STATUS_OK);
    assert(copied->sequence == 1u && copied->text[0] == 'X');
    ux_mailbox_destroy(mailbox);
    free(copied);
    free(frame);
    return 0;
}
