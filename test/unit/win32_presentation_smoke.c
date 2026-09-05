#include "actions.h"
#include "event_queue.h"
#include "frame.h"
#include "geometry.h"
#include "mailbox.h"
#include "mouse.h"
#include "router.h"

#include <assert.h>

#ifdef _WIN32
int main(void)
{
    RECT display;
    RECT source = { 10, 20, 30, 40 };
    RECT target = { 0, 0, 0, 0 };
    win32_presentation_frame frame = { 0 };
    win32_presentation_frame copied = { 0 };
    win32_presentation_frame *pending;
    const win32_presentation_frame *current;
    win32_presentation_mailbox *mailbox;
    win32_presentation_mouse mouse;
    win32_presentation_action_registry actions;
    win32_presentation_router router;
    win32_presentation_event_queue *events;
    win32_presentation_event event = { 0 };

    frame.sequence = 7u;
    frame.dib_width = 640u;
    frame.dib_height = 480u;
    assert(frame.sequence == 7u);
    assert(win32_presentation_display_rect(1280, 960, frame.dib_width,
        frame.dib_height, &display));
    assert(display.left == 0 && display.top == 0 &&
        display.right == 1280 && display.bottom == 960);
    win32_presentation_map_dirty_rect(&source, &display, frame.dib_width,
        frame.dib_height, &target);
    assert(target.left == 20 && target.top == 40 &&
        target.right == 60 && target.bottom == 80);
    assert(win32_presentation_dib_pixel(RGB(0, 0, 168)) == 0x000000a8u);
    mouse.x = 3;
    mouse.y = 4;
    mouse.valid = mouse.captured = mouse.host_cursor_hidden = 1;
    win32_presentation_mouse_reset(&mouse);
    assert(mouse.valid == 0 && mouse.captured == 0 &&
        mouse.host_cursor_hidden == 0);

    win32_presentation_actions_init(&actions);
    assert(win32_presentation_actions_register(&actions, 'P',
        WIN32_PRESENTATION_MODIFIER_CONTROL |
        WIN32_PRESENTATION_MODIFIER_ALT,
        WIN32_PRESENTATION_ACTION_PAUSE_TOGGLE));
    assert(win32_presentation_actions_match(&actions, 'P',
        WIN32_PRESENTATION_MODIFIER_CONTROL |
        WIN32_PRESENTATION_MODIFIER_ALT) ==
        WIN32_PRESENTATION_ACTION_PAUSE_TOGGLE);
    assert(win32_presentation_actions_match(&actions, 'P', 0u) ==
        WIN32_PRESENTATION_ACTION_NONE);

    win32_presentation_router_init(&router,
        WIN32_PRESENTATION_DISPLAY_CONSOLE);
    assert(win32_presentation_router_target(&router) ==
        WIN32_PRESENTATION_TARGET_CONSOLE);
    frame.valid = 1u;
    frame.graphics = 1u;
    assert(win32_presentation_router_observe(&router, &frame) ==
        WIN32_PRESENTATION_TARGET_WINDOW);

    assert(win32_presentation_event_queue_create(&events));
    event.type = WIN32_PRESENTATION_EVENT_MOUSE;
    event.data.mouse.delta_x = 3;
    event.data.mouse.delta_y = -2;
    event.data.mouse.left_down = 1u;
    assert(win32_presentation_event_queue_push(events, &event));
    event.data.mouse.delta_x = 4;
    event.data.mouse.delta_y = 5;
    assert(win32_presentation_event_queue_push(events, &event));
    assert(win32_presentation_event_queue_pop(events, &event));
    assert(event.type == WIN32_PRESENTATION_EVENT_MOUSE &&
        event.data.mouse.delta_x == 7 && event.data.mouse.delta_y == 3 &&
        event.data.mouse.left_down == 1u);
    assert(!win32_presentation_event_queue_pending(events));
    win32_presentation_event_queue_destroy(events);
    frame.graphics = 0u;
    assert(win32_presentation_router_observe(&router, &frame) ==
        WIN32_PRESENTATION_TARGET_WINDOW);
    assert(win32_presentation_router_observe(&router, &frame) ==
        WIN32_PRESENTATION_TARGET_WINDOW);
    assert(win32_presentation_router_observe(&router, &frame) ==
        WIN32_PRESENTATION_TARGET_CONSOLE);
    win32_presentation_router_init(&router,
        WIN32_PRESENTATION_DISPLAY_WINDOW);
    assert(win32_presentation_router_observe(&router, &frame) ==
        WIN32_PRESENTATION_TARGET_WINDOW);

    assert(win32_presentation_mailbox_create(&mailbox));
    assert(win32_presentation_mailbox_begin_update(mailbox, &pending,
        &current));
    assert(current->valid == 0u);
    pending->valid = 1u;
    pending->graphics = 0u;
    pending->text[0] = 'X';
    assert(win32_presentation_mailbox_finish_update(mailbox, 1) == 1u);
    assert(WaitForSingleObject((HANDLE)win32_presentation_mailbox_event(
        mailbox), 0u) == WAIT_OBJECT_0);
    assert(win32_presentation_mailbox_sequence(mailbox) == 1u);
    assert(win32_presentation_mailbox_copy(mailbox, &copied));
    assert(copied.sequence == 1u && copied.text[0] == 'X');
    win32_presentation_mailbox_destroy(mailbox);
    return 0;
}
#else
int main(void) { return 0; }
#endif
