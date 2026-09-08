#ifndef LIB_HOST_INTERNAL_CONSOLE_NATIVE_H
#define LIB_HOST_INTERNAL_CONSOLE_NATIVE_H

#include "lib/host/console.h"

typedef struct host_console_native host_console_native;

lib_status host_console_native_create(host_console_native **out_native);
void host_console_native_destroy(host_console_native *native_console);
/* Validate that the next binding can be activated without changing the
 * current Console's reader, mode, or output ownership. */
lib_status host_console_native_prepare(host_console_native *native_console,
    lib_console *console, host_console_mode mode);
void host_console_native_discard_prepare(host_console_native *native_console);
lib_status host_console_native_activate(host_console_native *native_console,
    lib_console *console, host_console_mode mode, lib_u32 generation);
void host_console_native_deactivate(host_console_native *native_console);
lib_status host_console_native_write(void *context, const char *text,
    lib_size length);
lib_status host_console_native_write_text_frame(void *context,
    const lib_console_text_frame *frame);

#endif
