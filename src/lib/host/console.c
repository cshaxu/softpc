#include "lib/base/internal/console.h"
#include "lib/host/internal/console_native.h"

struct host_console_broker {
    atomic_flag lock;
    host_console_native *native_console;
    lib_console *current;
    host_console_mode current_mode;
    lib_u32 generation;
};

static void host_console_lock(host_console_broker *broker)
{
    while (atomic_flag_test_and_set_explicit(&broker->lock,
        memory_order_acquire)) { }
}

static void host_console_unlock(host_console_broker *broker)
{
    atomic_flag_clear_explicit(&broker->lock, memory_order_release);
}

static lib_status host_console_start(host_console_broker *broker,
    lib_console *console, host_console_mode mode, lib_u32 generation)
{
    lib_status status = host_console_native_activate(broker->native_console,
        console, mode, generation);
    if (status != LIB_STATUS_OK) return status;
    status = lib_console_set_output_sink(console, host_console_native_write,
        broker->native_console);
    if (status != LIB_STATUS_OK) host_console_native_deactivate(broker->native_console);
    return status;
}

lib_status host_console_broker_create(host_console_broker **out_broker,
    lib_console *initial_console, host_console_mode initial_mode)
{
    host_console_broker *broker;
    lib_status status;
    if (out_broker == LIB_NULL || initial_console == LIB_NULL ||
        (initial_mode != HOST_CONSOLE_RAW_EVENTS &&
         initial_mode != HOST_CONSOLE_COOKED_LINES)) return LIB_STATUS_INVALID_ARGUMENT;
    *out_broker = LIB_NULL;
    broker = calloc(1u, sizeof(*broker));
    if (broker == LIB_NULL) return LIB_STATUS_NO_MEMORY;
    atomic_flag_clear(&broker->lock);
    status = host_console_native_create(&broker->native_console);
    if (status == LIB_STATUS_OK) {
        broker->current = lib_console_retain(initial_console);
        broker->current_mode = initial_mode;
        broker->generation = 1u;
        status = host_console_start(broker, broker->current, initial_mode,
            broker->generation);
    }
    if (status != LIB_STATUS_OK) {
        if (broker->current != LIB_NULL) lib_console_release(broker->current);
        host_console_native_destroy(broker->native_console);
        free(broker);
        return status;
    }
    *out_broker = broker;
    return LIB_STATUS_OK;
}

lib_status host_console_replace_active(host_console_broker *broker,
    lib_console *old_console, lib_console *next_console,
    host_console_mode next_mode)
{
    lib_status status;
    lib_console *old;
    lib_console *next;
    lib_u32 next_generation;
    if (broker == LIB_NULL || old_console == LIB_NULL || next_console == LIB_NULL ||
        (next_mode != HOST_CONSOLE_RAW_EVENTS &&
         next_mode != HOST_CONSOLE_COOKED_LINES)) return LIB_STATUS_INVALID_ARGUMENT;
    host_console_lock(broker);
    if (broker->current != old_console) {
        host_console_unlock(broker);
        return LIB_STATUS_INVALID_STATE;
    }
    old = broker->current;
    next_generation = broker->generation + 1u;
    next = lib_console_retain(next_console);
    host_console_native_deactivate(broker->native_console);
    (void)lib_console_set_output_sink(old, LIB_NULL, LIB_NULL);
    status = host_console_start(broker, next, next_mode, next_generation);
    if (status != LIB_STATUS_OK) {
        /* A failed next object never becomes visible.  Restore the old reader
         * before returning, preserving the one-current-object invariant. */
        status = host_console_start(broker, old, broker->current_mode,
            broker->generation);
        lib_console_release(next);
        host_console_unlock(broker);
        return status == LIB_STATUS_OK ? LIB_STATUS_IO_ERROR : status;
    }
    broker->current = next;
    broker->current_mode = next_mode;
    broker->generation = next_generation;
    host_console_unlock(broker);
    lib_console_release(old);
    return LIB_STATUS_OK;
}

void host_console_broker_destroy(host_console_broker *broker)
{
    lib_console *current;
    if (broker == LIB_NULL) return;
    host_console_lock(broker);
    current = broker->current;
    broker->current = LIB_NULL;
    host_console_native_deactivate(broker->native_console);
    if (current != LIB_NULL)
        (void)lib_console_set_output_sink(current, LIB_NULL, LIB_NULL);
    host_console_unlock(broker);
    if (current != LIB_NULL) lib_console_release(current);
    host_console_native_destroy(broker->native_console);
    free(broker);
}
