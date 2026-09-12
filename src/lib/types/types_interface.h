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
typedef struct lib_native_event lib_native_event;
typedef struct lib_native_task lib_native_task;

typedef enum lib_file_access {
    LIB_FILE_ACCESS_READONLY,
    LIB_FILE_ACCESS_READWRITE
} lib_file_access;

typedef enum lib_file_write_mode {
    LIB_FILE_WRITE_TRUNCATE,
    LIB_FILE_WRITE_APPEND
} lib_file_write_mode;

typedef int lib_status;

typedef enum lib_native_wait_result {
    LIB_NATIVE_WAIT_SIGNALED,
    LIB_NATIVE_WAIT_TIMED_OUT,
    LIB_NATIVE_WAIT_FAULT
} lib_native_wait_result;

typedef void (*lib_native_task_entry)(void *context);


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

/* Raw platform synchronization ABI. These functions do not know cancellation,
 * task policy, ownership, or component-level wait precedence. */
void lib_native_sleep_milliseconds(lib_u32 milliseconds);
void lib_native_yield(void);
lib_status lib_native_event_create(lib_bool manual_reset,
    lib_native_event **out_event);
void lib_native_event_destroy(lib_native_event *event);
void lib_native_event_signal(lib_native_event *event);
void lib_native_event_reset(lib_native_event *event);
lib_native_wait_result lib_native_event_wait(const lib_native_event *event,
    lib_u32 timeout_milliseconds);
lib_native_wait_result lib_native_event_wait_many(
    const lib_native_event *const *events, lib_u32 event_count,
    lib_u32 timeout_milliseconds, lib_u32 *out_event_index);
lib_status lib_native_task_create(lib_native_task_entry entry, void *context,
    lib_native_task **out_task);
void lib_native_task_join(lib_native_task *task);
void lib_native_task_destroy(lib_native_task *task);

/* Current generic modifier state for a native input dispatch. */
enum {
    LIB_NATIVE_INPUT_MODIFIER_CONTROL = 0x01u,
    LIB_NATIVE_INPUT_MODIFIER_ALT = 0x02u,
    LIB_NATIVE_INPUT_MODIFIER_SHIFT = 0x04u
};
enum { LIB_NATIVE_INPUT_FLAG_EXTENDED = 0x01u };
/* Copied native key values: this is an ABI vocabulary, not UI policy. */
enum {
    LIB_NATIVE_KEY_BACK = 0x08u, LIB_NATIVE_KEY_TAB = 0x09u,
    LIB_NATIVE_KEY_RETURN = 0x0du, LIB_NATIVE_KEY_SHIFT = 0x10u,
    LIB_NATIVE_KEY_CONTROL = 0x11u, LIB_NATIVE_KEY_ALT = 0x12u,
    LIB_NATIVE_KEY_PAUSE = 0x13u, LIB_NATIVE_KEY_CAPS_LOCK = 0x14u,
    LIB_NATIVE_KEY_ESCAPE = 0x1bu, LIB_NATIVE_KEY_SPACE = 0x20u,
    LIB_NATIVE_KEY_PAGE_UP = 0x21u, LIB_NATIVE_KEY_PAGE_DOWN = 0x22u,
    LIB_NATIVE_KEY_END = 0x23u, LIB_NATIVE_KEY_HOME = 0x24u,
    LIB_NATIVE_KEY_LEFT = 0x25u, LIB_NATIVE_KEY_UP = 0x26u,
    LIB_NATIVE_KEY_RIGHT = 0x27u, LIB_NATIVE_KEY_DOWN = 0x28u,
    LIB_NATIVE_KEY_SNAPSHOT = 0x2cu, LIB_NATIVE_KEY_INSERT = 0x2du,
    LIB_NATIVE_KEY_DELETE = 0x2eu, LIB_NATIVE_KEY_0 = 0x30u,
    LIB_NATIVE_KEY_9 = 0x39u, LIB_NATIVE_KEY_A = 0x41u,
    LIB_NATIVE_KEY_Z = 0x5au, LIB_NATIVE_KEY_LEFT_WINDOWS = 0x5bu,
    LIB_NATIVE_KEY_RIGHT_WINDOWS = 0x5cu, LIB_NATIVE_KEY_APPS = 0x5du,
    LIB_NATIVE_KEY_NUMPAD_0 = 0x60u, LIB_NATIVE_KEY_NUMPAD_9 = 0x69u,
    LIB_NATIVE_KEY_MULTIPLY = 0x6au, LIB_NATIVE_KEY_ADD = 0x6bu,
    LIB_NATIVE_KEY_SUBTRACT = 0x6du, LIB_NATIVE_KEY_DECIMAL = 0x6eu,
    LIB_NATIVE_KEY_DIVIDE = 0x6fu, LIB_NATIVE_KEY_F1 = 0x70u,
    LIB_NATIVE_KEY_F12 = 0x7bu, LIB_NATIVE_KEY_F13 = 0x7cu,
    LIB_NATIVE_KEY_F24 = 0x87u, LIB_NATIVE_KEY_NUM_LOCK = 0x90u,
    LIB_NATIVE_KEY_SCROLL_LOCK = 0x91u, LIB_NATIVE_KEY_LEFT_SHIFT = 0xa0u,
    LIB_NATIVE_KEY_RIGHT_SHIFT = 0xa1u, LIB_NATIVE_KEY_LEFT_CONTROL = 0xa2u,
    LIB_NATIVE_KEY_RIGHT_CONTROL = 0xa3u, LIB_NATIVE_KEY_LEFT_ALT = 0xa4u,
    LIB_NATIVE_KEY_RIGHT_ALT = 0xa5u, LIB_NATIVE_KEY_OEM_1 = 0xbau,
    LIB_NATIVE_KEY_OEM_PLUS = 0xbbu, LIB_NATIVE_KEY_OEM_COMMA = 0xbcu,
    LIB_NATIVE_KEY_OEM_MINUS = 0xbdu, LIB_NATIVE_KEY_OEM_PERIOD = 0xbeu,
    LIB_NATIVE_KEY_OEM_2 = 0xbfu, LIB_NATIVE_KEY_OEM_3 = 0xc0u,
    LIB_NATIVE_KEY_OEM_4 = 0xdbu, LIB_NATIVE_KEY_OEM_5 = 0xdcu,
    LIB_NATIVE_KEY_OEM_6 = 0xddu, LIB_NATIVE_KEY_OEM_7 = 0xdeu
};
lib_u8 lib_native_input_current_modifiers(void);
lib_u8 lib_native_input_flags(lib_u64 native_control_state);
lib_u16 lib_native_input_scan_code(lib_u16 native_key);
lib_bool lib_native_input_map_scalar(lib_u32 scalar, lib_u16 *out_native_key,
    lib_u8 *out_modifiers);

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
