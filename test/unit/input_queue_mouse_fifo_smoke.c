#include "input_queue.h"

#include <assert.h>
#include <string.h>

int main(void)
{
    app_input_queue *queue = NULL;
    kvm_input_event first = { 0 };
    kvm_input_event second = { 0 };
    kvm_input_event actual = { 0 };

    first.type = KVM_EVENT_MOUSE;
    first.data.mouse.relative = 1u;
    first.data.mouse.delta_y = 16;
    first.data.mouse.buttons = KVM_MOUSE_BUTTON_LEFT;
    second = first;
    second.data.mouse.delta_y = 16;
    assert(app_input_queue_create(&queue));
    assert(app_input_queue_push(queue, &first));
    assert(app_input_queue_push(queue, &second));
    assert(app_input_queue_pop(queue, &actual));
    assert(memcmp(&actual, &first, sizeof(actual)) == 0);
    assert(app_input_queue_pop(queue, &actual));
    assert(memcmp(&actual, &second, sizeof(actual)) == 0);
    assert(!app_input_queue_pending(queue));
    app_input_queue_destroy(queue);
    return 0;
}
