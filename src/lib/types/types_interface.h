#ifndef LIB_TYPES_INTERFACE_H
#define LIB_TYPES_INTERFACE_H

#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>

typedef uint8_t lib_u8;
typedef uint16_t lib_u16;
typedef uint32_t lib_u32;
typedef uint64_t lib_u64;
typedef int32_t lib_i32;
typedef int64_t lib_i64;
typedef size_t lib_size;
typedef int lib_bool;
typedef va_list lib_format_arguments;

typedef struct lib_file lib_file;
typedef struct lib_sync_event lib_sync_event;
typedef struct lib_sync_task lib_sync_task;

#define LIB_NATIVE_CONSOLE_LINE_MAX 1024u

typedef enum lib_native_console_event_kind {
    LIB_NATIVE_CONSOLE_EVENT_RAW_KEY,
    LIB_NATIVE_CONSOLE_EVENT_RAW_MOUSE,
    LIB_NATIVE_CONSOLE_EVENT_COOKED_LINE,
    LIB_NATIVE_CONSOLE_EVENT_REJECTED_LINE
} lib_native_console_event_kind;

enum {
    LIB_NATIVE_CONSOLE_MODIFIER_CONTROL = 0x01u,
    LIB_NATIVE_CONSOLE_MODIFIER_ALT = 0x02u,
    LIB_NATIVE_CONSOLE_MODIFIER_SHIFT = 0x04u
};

typedef struct lib_native_console_raw_key {
    lib_u32 key;
    lib_u32 unicode;
    lib_u16 scan_code;
    lib_u8 modifiers;
    lib_bool extended;
    lib_bool pressed;
} lib_native_console_raw_key;

typedef struct lib_native_console_raw_mouse {
    lib_i32 delta_x;
    lib_i32 delta_y;
    lib_u32 buttons;
} lib_native_console_raw_mouse;

typedef struct lib_native_console_line {
    lib_u32 length;
    char text[LIB_NATIVE_CONSOLE_LINE_MAX];
} lib_native_console_line;

typedef struct lib_native_console_event {
    lib_native_console_event_kind kind;
    lib_u32 binding_generation;
    union {
        lib_native_console_raw_key raw_key;
        lib_native_console_raw_mouse raw_mouse;
        lib_native_console_line line;
    } value;
} lib_native_console_event;

typedef enum lib_file_access {
    LIB_FILE_ACCESS_READONLY,
    LIB_FILE_ACCESS_READWRITE
} lib_file_access;

typedef enum lib_file_write_mode {
    LIB_FILE_WRITE_TRUNCATE,
    LIB_FILE_WRITE_APPEND
} lib_file_write_mode;

typedef int lib_status;

typedef enum lib_sync_wait_result {
    LIB_SYNC_WAIT_SIGNALED,
    LIB_SYNC_WAIT_CANCELLED,
    LIB_SYNC_WAIT_TIMED_OUT,
    LIB_SYNC_WAIT_INVALID_ARGUMENT,
    LIB_SYNC_WAIT_FAULT
} lib_sync_wait_result;

typedef void (*lib_sync_task_entry)(void *context,
    const lib_sync_task *task);

enum {
    LIB_STATUS_OK = 0,
    LIB_STATUS_INVALID_ARGUMENT = 1,
    LIB_STATUS_INVALID_STATE = 2,
    LIB_STATUS_UNSUPPORTED = 3,
    LIB_STATUS_NO_MEMORY = 4,
    LIB_STATUS_IO_ERROR = 5,
    LIB_STATUS_NOT_CURRENT = 6,
    LIB_STATUS_LIMIT_EXCEEDED = 7
};

#define LIB_FALSE 0
#define LIB_TRUE 1
#define LIB_NULL NULL

/* Cross-platform C runtime vocabulary.  These functions deliberately expose
 * no platform handle, product state, or I/O policy. */
void *lib_memory_set(void *destination, int value, lib_size byte_count);
void *lib_memory_copy(void *destination, const void *source, lib_size byte_count);
void *lib_memory_move(void *destination, const void *source, lib_size byte_count);
int lib_memory_compare(const void *left, const void *right, lib_size byte_count);
const void *lib_memory_find(const void *bytes, int value, lib_size byte_count);

lib_size lib_text_length(const char *text);
int lib_text_compare(const char *left, const char *right);
void lib_text_ascii_lower(char *text);
int lib_text_format(char *destination, lib_size destination_capacity,
    const char *format, ...);
int lib_text_format_v(char *destination, lib_size destination_capacity,
    const char *format, lib_format_arguments arguments);
int lib_text_format_append(char **cursor, lib_size *remaining,
    const char *format, ...);
int lib_text_format_append_v(char **cursor, lib_size *remaining,
    const char *format, lib_format_arguments arguments);

void *lib_allocate(lib_size byte_count);
void *lib_allocate_zero(lib_size count, lib_size byte_count);
void lib_release(void *memory);

/* Host monotonic time as a copied counter and its copied frequency. */
lib_status lib_monotonic_counter(lib_u64 *out_units,
    lib_u64 *out_units_per_second);

/* Neutral synchronization primitives. Platform handles and thread ABI remain
 * private to the selected types platform adapter. */
void lib_sync_sleep_milliseconds(lib_u32 milliseconds);
void lib_sync_yield(void);
lib_status lib_sync_event_create(lib_sync_event **out_event);
void lib_sync_event_destroy(lib_sync_event *event);
void lib_sync_event_signal(lib_sync_event *event);
void lib_sync_event_reset(lib_sync_event *event);
lib_sync_wait_result lib_sync_event_wait(lib_sync_event *event,
    lib_u32 timeout_milliseconds);
lib_sync_wait_result lib_sync_wait_any(lib_sync_event *const *events,
    lib_u32 event_count, const lib_sync_task *cancel_task,
    lib_u32 timeout_milliseconds, lib_u32 *out_event_index);
lib_status lib_sync_task_create(lib_sync_task_entry entry, void *context,
    lib_sync_task **out_task);
void lib_sync_task_request_cancel(lib_sync_task *task);
int lib_sync_task_cancelled(const lib_sync_task *task);
lib_sync_wait_result lib_sync_task_wait_cancel(const lib_sync_task *task,
    lib_u32 timeout_milliseconds);
void lib_sync_task_join(lib_sync_task *task);
void lib_sync_task_destroy(lib_sync_task *task);

/* Current generic modifier state for a native input dispatch. */
enum {
    LIB_NATIVE_INPUT_MODIFIER_CONTROL = 0x01u,
    LIB_NATIVE_INPUT_MODIFIER_ALT = 0x02u,
    LIB_NATIVE_INPUT_MODIFIER_SHIFT = 0x04u
};
lib_u8 lib_native_input_current_modifiers(void);

/* Neutral byte-stream primitive.  Native descriptors and C FILE objects stay
 * entirely within types. */
lib_status lib_file_open(const char *path, lib_file_access access,
    lib_file **out_file);
lib_status lib_file_open_writer(const char *path, lib_file_write_mode mode,
    lib_file **out_file);
lib_status lib_file_read_exact(lib_file *file, void *bytes, lib_size byte_count);
lib_status lib_file_write_exact(lib_file *file, const void *bytes,
    lib_size byte_count);
lib_status lib_file_flush(lib_file *file);
lib_status lib_file_seek_absolute(lib_file *file, lib_i64 offset);
lib_status lib_file_byte_count(lib_file *file, lib_i64 *out_byte_count);
lib_status lib_file_close(lib_file **file);

#include "lib/types/atomic.h"

#endif
