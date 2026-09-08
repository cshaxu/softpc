#include "lib/base/console.h"
#include "lib/host/internal/console_native.h"

#include <assert.h>

struct host_console_native {
    lib_console *active;
    host_console_mode mode;
    lib_u32 generation;
};

static int host_console_fail_next_activation;
static int host_console_fail_next_prepare;

lib_status host_console_native_create(host_console_native **out_native)
{
    static host_console_native native_console;
    native_console.active = LIB_NULL;
    *out_native = &native_console;
    return LIB_STATUS_OK;
}

void host_console_native_destroy(host_console_native *native_console)
{ (void)native_console; }

lib_status host_console_native_prepare(host_console_native *native_console,
    lib_console *console, host_console_mode mode)
{
    if (host_console_fail_next_prepare) {
        host_console_fail_next_prepare = 0;
        return LIB_STATUS_IO_ERROR;
    }
    return native_console == NULL || console == NULL ||
        (mode != HOST_CONSOLE_RAW_EVENTS && mode != HOST_CONSOLE_COOKED_LINES) ?
        LIB_STATUS_INVALID_ARGUMENT : LIB_STATUS_OK;
}

lib_status host_console_native_activate(host_console_native *native_console,
    lib_console *console, host_console_mode mode, lib_u32 generation)
{
    if (host_console_fail_next_activation) {
        host_console_fail_next_activation = 0;
        return LIB_STATUS_IO_ERROR;
    }
    native_console->active = console;
    native_console->mode = mode;
    native_console->generation = generation;
    return LIB_STATUS_OK;
}

void host_console_native_deactivate(host_console_native *native_console)
{ native_console->active = LIB_NULL; }

lib_status host_console_native_write(void *context, const char *text,
    lib_size length)
{
    host_console_native *native_console = (host_console_native *)context;
    (void)text;
    return native_console->active == LIB_NULL || length == 0u ?
        LIB_STATUS_IO_ERROR : LIB_STATUS_OK;
}

lib_status host_console_native_write_text_frame(void *context,
    const lib_console_text_frame *frame)
{
    host_console_native *native_console = (host_console_native *)context;
    return native_console->active == LIB_NULL || frame == LIB_NULL ?
        LIB_STATUS_IO_ERROR : LIB_STATUS_OK;
}

int main(void)
{
    lib_console *first = LIB_NULL;
    lib_console *second = LIB_NULL;
    host_console_broker *broker = LIB_NULL;
    host_console_broker *second_broker = LIB_NULL;

    assert(lib_console_create(&first) == LIB_STATUS_OK);
    assert(lib_console_create(&second) == LIB_STATUS_OK);
    assert(host_console_broker_create(&broker, first,
        HOST_CONSOLE_COOKED_LINES) == LIB_STATUS_OK);
    assert(host_console_broker_create(&second_broker, second,
        HOST_CONSOLE_RAW_EVENTS) == LIB_STATUS_INVALID_STATE);
    assert(second_broker == LIB_NULL);
    assert(lib_console_write_text(first, "a", 1u) == LIB_STATUS_OK);
    assert(host_console_replace_active(broker, first, second,
        HOST_CONSOLE_RAW_EVENTS) == LIB_STATUS_OK);
    assert(lib_console_write_text(first, "a", 1u) == LIB_STATUS_NOT_CURRENT);
    assert(lib_console_write_text(second, "b", 1u) == LIB_STATUS_OK);
    host_console_fail_next_prepare = 1;
    assert(host_console_replace_active(broker, second, first,
        HOST_CONSOLE_COOKED_LINES) == LIB_STATUS_IO_ERROR);
    /* Preflight failure did not stop or detach the old current object. */
    assert(lib_console_write_text(second, "b", 1u) == LIB_STATUS_OK);
    host_console_fail_next_activation = 1;
    assert(host_console_replace_active(broker, second, first,
        HOST_CONSOLE_COOKED_LINES) == LIB_STATUS_IO_ERROR);
    assert(lib_console_write_text(second, "b", 1u) == LIB_STATUS_OK);
    host_console_broker_destroy(broker);
    assert(lib_console_write_text(second, "b", 1u) == LIB_STATUS_NOT_CURRENT);
    assert(host_console_broker_create(&second_broker, first,
        HOST_CONSOLE_COOKED_LINES) == LIB_STATUS_OK);
    host_console_broker_destroy(second_broker);
    lib_console_destroy(first);
    lib_console_destroy(second);
    return 0;
}
