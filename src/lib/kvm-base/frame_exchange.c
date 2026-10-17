#include "lib/kvm-base/frame_exchange_interface.h"
#include "lib/base/sync_interface.h"

struct kvm_frame_exchange {
    base_sync_mutex *lock;
    void *slots[4];
    void *writer;
    const void *pending;
    const void *processing;
    const void *displayed;
    const void *latest;
    lib_u32 count;
};

lib_status kvm_frame_exchange_create(kvm_frame_exchange **out_exchange,
    lib_size frame_size, lib_bool retain_displayed)
{
    kvm_frame_exchange *exchange;
    lib_status status;
    if (out_exchange == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    *out_exchange = LIB_NULL;
    if (frame_size == 0u) return LIB_STATUS_INVALID_ARGUMENT;
    exchange = lib_allocate_zero(1u, sizeof(*exchange));
    if (exchange == LIB_NULL) return LIB_STATUS_NO_MEMORY;
    exchange->count = retain_displayed ? 4u : 3u;
    status = base_sync_mutex_create(&exchange->lock);
    for (lib_u32 i = 0u; status == LIB_STATUS_OK && i < exchange->count; ++i) {
        exchange->slots[i] = lib_allocate_zero(1u, frame_size);
        if (exchange->slots[i] == LIB_NULL) status = LIB_STATUS_NO_MEMORY;
    }
    if (status != LIB_STATUS_OK) {
        kvm_frame_exchange_destroy(exchange);
        return status;
    }
    exchange->writer = exchange->slots[0];
    *out_exchange = exchange;
    return LIB_STATUS_OK;
}

void kvm_frame_exchange_destroy(kvm_frame_exchange *exchange)
{
    if (exchange == LIB_NULL) return;
    for (lib_u32 i = 0u; i < exchange->count; ++i) lib_release(exchange->slots[i]);
    base_sync_mutex_destroy(exchange->lock);
    lib_release(exchange);
}

void *kvm_frame_exchange_writer(kvm_frame_exchange *exchange)
{ return exchange->writer; }

void kvm_frame_exchange_publish(kvm_frame_exchange *exchange)
{
    base_sync_mutex_lock(exchange->lock);
    exchange->latest = exchange->pending = exchange->writer;
    /* At most pending, processing and displayed are unavailable. The remaining
     * slot is always writable, including when the consumer never advances. */
    for (lib_u32 i = 0u; i < exchange->count; ++i) {
        void *slot = exchange->slots[i];
        if (slot != exchange->pending && slot != exchange->processing &&
            slot != exchange->displayed) {
            exchange->writer = slot;
            break;
        }
    }
    base_sync_mutex_unlock(exchange->lock);
}

const void *kvm_frame_exchange_take(kvm_frame_exchange *exchange)
{
    const void *frame = LIB_NULL;
    base_sync_mutex_lock(exchange->lock);
    if (exchange->processing == LIB_NULL) {
        frame = exchange->processing = exchange->pending;
        exchange->pending = LIB_NULL;
    }
    base_sync_mutex_unlock(exchange->lock);
    return frame;
}

void kvm_frame_exchange_finish(kvm_frame_exchange *exchange, lib_bool success)
{
    base_sync_mutex_lock(exchange->lock);
    if (exchange->processing != LIB_NULL) {
        if (success && exchange->count == 4u)
            exchange->displayed = exchange->processing;
        else if (!success && exchange->latest == exchange->processing)
            exchange->pending = exchange->processing;
        exchange->processing = LIB_NULL;
    }
    base_sync_mutex_unlock(exchange->lock);
}

void kvm_frame_exchange_detach(kvm_frame_exchange *exchange)
{
    base_sync_mutex_lock(exchange->lock);
    exchange->processing = exchange->displayed = LIB_NULL;
    exchange->pending = exchange->latest;
    base_sync_mutex_unlock(exchange->lock);
}

const void *kvm_frame_exchange_lock_latest(kvm_frame_exchange *exchange)
{
    base_sync_mutex_lock(exchange->lock);
    return exchange->latest;
}

void kvm_frame_exchange_unlock(kvm_frame_exchange *exchange)
{ base_sync_mutex_unlock(exchange->lock); }

void kvm_frame_exchange_invalidate(kvm_frame_exchange *exchange)
{
    base_sync_mutex_lock(exchange->lock);
    exchange->pending = exchange->latest = LIB_NULL;
    base_sync_mutex_unlock(exchange->lock);
}
