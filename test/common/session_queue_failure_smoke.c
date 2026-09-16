#include "common/session/control.h"
#include <assert.h>

static int fail_allocation;
static void *allocate(lib_size count, lib_size size)
{ return fail_allocation ? NULL : lib_allocate_zero(count, size); }
#define lib_allocate_zero allocate
#include "common/session/control.c"

int main(void)
{
    common_session_queue *queue;
    common_session_event event;
    kvm_input_event key = { 0 };
    unsigned int i, round;
    assert(common_session_queue_create(&queue));
    for (round = 0u; round < 2u; ++round) {
        for (i = 0u; i < 64u; ++i)
            assert(common_session_queue_push_frame_completed(queue, i, 0, 1u));
        fail_allocation = 1;
        /* Reverse arrival order still retains the original class priority. */
        assert(!common_session_queue_push_runtime_completed(queue,
            COMMON_SESSION_MACHINE_PAUSED, 20u + round));
        assert(!common_session_queue_push_console_failed(queue));
        key.source_identity = 100u + round;
        assert(!common_session_queue_push_kvm_for_run(queue, &key, 30u + round));
        key.source_identity = 999u;
        assert(!common_session_queue_push_kvm_for_run(queue, &key, 999u));
        for (i = 0u; i < 64u; ++i) {
            assert(common_session_queue_take(queue, &event, 0u));
            assert(event.kind == COMMON_SESSION_EVENT_FRAME_COMPLETED);
            assert(event.value.frame.sequence == i);
        }
        assert(common_session_queue_take(queue, &event, 0u));
        assert(event.kind == COMMON_SESSION_EVENT_KVM_DELIVERY_FAILED);
        assert(event.run_generation == 30u + round);
        assert(event.value.delivery_failure.source_identity == 100u + round);
        assert(event.value.delivery_failure.status == LIB_STATUS_NO_MEMORY);
        assert(common_session_queue_take(queue, &event, 0u));
        assert(event.kind == COMMON_SESSION_EVENT_QUEUE_DELIVERY_FAILED);
        assert(event.run_generation == 20u + round);
        assert(event.value.queue_delivery_status == LIB_STATUS_NO_MEMORY);
        assert(!common_session_queue_take(queue, &event, 0u));
        fail_allocation = 0;
    }
    assert(common_session_queue_push_console_failed(queue));
    assert(common_session_queue_take(queue, &event, 0u));
    assert(event.kind == COMMON_SESSION_EVENT_CONSOLE_FAILED);
    common_session_queue_destroy(queue);
    return 0;
}
