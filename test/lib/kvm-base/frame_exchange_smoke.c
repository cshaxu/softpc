#include "lib/types/test.h"
#include "lib/kvm-base/frame_exchange_interface.h"
#include "lib/base/sync_interface.h"

static lib_u32 allocation, fail_at, live_allocations;
static void *allocate_frame(lib_size count, lib_size size)
{
    if (++allocation == fail_at) return LIB_NULL;
    void *memory = lib_allocate_zero(count, size);
    if (memory != LIB_NULL) ++live_allocations;
    return memory;
}
static void release_frame(void *memory)
{
    if (memory != LIB_NULL) { lib_test_assert(live_allocations > 0u); --live_allocations; }
    lib_release(memory);
}
#define lib_allocate_zero allocate_frame
#define lib_release release_frame
#include "lib/kvm-base/frame_exchange.c"
#undef lib_allocate_zero
#undef lib_release

typedef struct test_frame { lib_u32 serial, complement; } test_frame;

static void publish(kvm_frame_exchange *exchange, lib_u32 serial)
{
    test_frame *frame = kvm_frame_exchange_writer(exchange);
    lib_test_assert(frame != LIB_NULL);
    frame->serial = serial;
    frame->complement = ~serial;
    kvm_frame_exchange_publish(exchange);
}

static void check(const test_frame *frame, lib_u32 serial)
{
    lib_test_assert(frame != LIB_NULL && frame->serial == serial &&
        frame->complement == ~serial);
}

static void exercise(lib_bool retained)
{
    kvm_frame_exchange *exchange = LIB_NULL;
    const test_frame *processing, *displayed;
    void *slots[4] = { 0 };
    lib_u32 count = 0u;
    lib_test_assert(kvm_frame_exchange_create(&exchange,
        sizeof(test_frame), retained) == LIB_STATUS_OK);
    lib_test_assert(kvm_frame_exchange_take(exchange) == LIB_NULL);
    lib_test_assert(kvm_frame_exchange_lock_latest(exchange) == LIB_NULL);
    kvm_frame_exchange_unlock(exchange);
    publish(exchange, 1u);
    displayed = kvm_frame_exchange_take(exchange);
    check(displayed, 1u);
    kvm_frame_exchange_finish(exchange, LIB_TRUE);
    publish(exchange, 2u);
    processing = kvm_frame_exchange_take(exchange);
    check(processing, 2u);
    lib_test_assert(kvm_frame_exchange_take(exchange) == LIB_NULL);
    /* A stalled consumer cannot prevent publication or mutate held inputs.
     * Record distinct writer addresses rather than merely checking values. */
    for (lib_u32 serial = 3u; serial < 10003u; ++serial) {
        void *writer = kvm_frame_exchange_writer(exchange);
        lib_u32 i;
        lib_test_assert(writer != processing && (!retained || writer != displayed));
        for (i = 0u; i < count && slots[i] != writer; ++i) {}
        if (i == count) { lib_test_assert(count < 2u); slots[count++] = writer; }
        publish(exchange, serial);
        check(processing, 2u);
        if (retained) check(displayed, 1u);
        check(kvm_frame_exchange_lock_latest(exchange), serial);
        kvm_frame_exchange_unlock(exchange);
    }
    lib_test_assert(count == 2u);
    /* Failed old output must never replace the newer pending publication. */
    kvm_frame_exchange_finish(exchange, LIB_FALSE);
    processing = kvm_frame_exchange_take(exchange);
    check(processing, 10002u);
    kvm_frame_exchange_finish(exchange, LIB_FALSE);
    lib_test_assert(kvm_frame_exchange_take(exchange) == processing);
    kvm_frame_exchange_finish(exchange, LIB_TRUE);
    lib_test_assert(kvm_frame_exchange_take(exchange) == LIB_NULL);
    kvm_frame_exchange_detach(exchange);
    check(kvm_frame_exchange_take(exchange), 10002u);
    /* Invalidating a run cannot write into an in-flight immutable input or
     * requeue it when that consumer finishes. */
    kvm_frame_exchange_invalidate(exchange);
    check(processing, 10002u);
    lib_test_assert(kvm_frame_exchange_lock_latest(exchange) == LIB_NULL);
    kvm_frame_exchange_unlock(exchange);
    kvm_frame_exchange_finish(exchange, LIB_FALSE);
    lib_test_assert(kvm_frame_exchange_take(exchange) == LIB_NULL);
    publish(exchange, 10003u);
    check(kvm_frame_exchange_take(exchange), 10003u);
    kvm_frame_exchange_finish(exchange, LIB_TRUE);
    kvm_frame_exchange_detach(exchange);
    kvm_frame_exchange_destroy(exchange);
}

typedef struct concurrent_test {
    kvm_frame_exchange *exchange;
    base_sync_event *held, *produced;
} concurrent_test;

static void produce(void *context, const base_sync_task *task)
{
    concurrent_test *test = context;
    (void)task;
    lib_test_assert(base_sync_event_wait(test->held, 5000u) == BASE_SYNC_WAIT_SIGNALED);
    for (lib_u32 serial = 3u; serial <= 100000u; ++serial)
        publish(test->exchange, serial);
    lib_test_assert(base_sync_event_signal(test->produced) == LIB_STATUS_OK);
}

static void concurrent_exchange(lib_bool retained)
{
    concurrent_test test = { 0 };
    base_sync_task *producer;
    const test_frame *displayed, *processing;
    lib_test_assert(kvm_frame_exchange_create(&test.exchange,
        sizeof(test_frame), retained) == LIB_STATUS_OK);
    lib_test_assert(base_sync_event_create(BASE_SYNC_EVENT_AUTO_RESET, &test.held) == LIB_STATUS_OK);
    lib_test_assert(base_sync_event_create(BASE_SYNC_EVENT_AUTO_RESET, &test.produced) == LIB_STATUS_OK);
    publish(test.exchange, 1u);
    displayed = kvm_frame_exchange_take(test.exchange);
    kvm_frame_exchange_finish(test.exchange, LIB_TRUE);
    publish(test.exchange, 2u);
    processing = kvm_frame_exchange_take(test.exchange);
    lib_test_assert(base_sync_task_create(produce, &test, &producer) == LIB_STATUS_OK);
    lib_test_assert(base_sync_event_signal(test.held) == LIB_STATUS_OK);
    /* Actual concurrent reads, while the producer repeatedly overwrites the
     * pending slot. No consumer progress is required to complete production. */
    base_sync_wait_result result;
    do {
        check(processing, 2u);
        if (retained) check(displayed, 1u);
        result = base_sync_event_wait(test.produced, 0u);
    } while (result == BASE_SYNC_WAIT_TIMED_OUT);
    lib_test_assert(result == BASE_SYNC_WAIT_SIGNALED);
    lib_test_assert(base_sync_task_destroy(producer) == LIB_STATUS_OK);
    kvm_frame_exchange_finish(test.exchange, LIB_TRUE);
    check(kvm_frame_exchange_take(test.exchange), 100000u);
    kvm_frame_exchange_finish(test.exchange, LIB_TRUE);
    base_sync_event_destroy(test.held);
    base_sync_event_destroy(test.produced);
    kvm_frame_exchange_destroy(test.exchange);
}

int main(void)
{
    kvm_frame_exchange *exchange = (kvm_frame_exchange *)1;
    lib_test_assert(kvm_frame_exchange_create(LIB_NULL, 1u, LIB_FALSE) ==
        LIB_STATUS_INVALID_ARGUMENT);
    lib_test_assert(kvm_frame_exchange_create(&exchange, 0u, LIB_FALSE) ==
        LIB_STATUS_INVALID_ARGUMENT && exchange == LIB_NULL);
    kvm_frame_exchange_destroy(LIB_NULL);
    for (fail_at = 1u; fail_at <= 5u; ++fail_at) {
        allocation = 0u;
        lib_test_assert(kvm_frame_exchange_create(&exchange, sizeof(test_frame), LIB_TRUE) ==
            LIB_STATUS_NO_MEMORY);
        lib_test_assert(exchange == LIB_NULL && live_allocations == 0u);
    }
    fail_at = 0u;
    exercise(LIB_FALSE);
    exercise(LIB_TRUE);
    concurrent_exchange(LIB_FALSE);
    concurrent_exchange(LIB_TRUE);
    lib_test_assert(live_allocations == 0u);
    return 0;
}
