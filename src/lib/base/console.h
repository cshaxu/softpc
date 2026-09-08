#ifndef LIB_CONSOLE_H
#define LIB_CONSOLE_H

#include "lib/base/base.h"

/* A logical Console is a copied-value endpoint.  It deliberately has no
 * native handle, process ownership, reader, or presentation policy.  Host is
 * the only component that binds one logical object to native Console I/O. */

#define LIB_CONSOLE_LINE_MAX 1024u
#define LIB_CONSOLE_TEXT_COLUMNS 80u
#define LIB_CONSOLE_TEXT_ROWS 25u

typedef struct lib_console lib_console;

typedef enum lib_console_event_kind {
    LIB_CONSOLE_EVENT_RAW_KEY,
    LIB_CONSOLE_EVENT_RAW_MOUSE,
    LIB_CONSOLE_EVENT_COOKED_LINE,
    LIB_CONSOLE_EVENT_REJECTED_LINE
} lib_console_event_kind;

typedef struct lib_console_raw_key {
    lib_u32 key;
    lib_u32 unicode;
    lib_u8 modifiers;
    lib_bool pressed;
} lib_console_raw_key;

typedef struct lib_console_raw_mouse {
    lib_i32 delta_x;
    lib_i32 delta_y;
    lib_u32 buttons;
} lib_console_raw_mouse;

typedef struct lib_console_line {
    lib_u32 length;
    char text[LIB_CONSOLE_LINE_MAX];
} lib_console_line;

/* Generic text-mode Console output.  It has no native handle, font, window,
 * or guest dependency; host maps the copied values at its native boundary. */
typedef struct lib_console_text_frame {
    lib_u16 columns;
    lib_u16 rows;
    lib_i32 cursor_column;
    lib_i32 cursor_row;
    lib_bool cursor_visible;
    lib_u8 text[LIB_CONSOLE_TEXT_COLUMNS * LIB_CONSOLE_TEXT_ROWS];
    lib_u16 attributes[LIB_CONSOLE_TEXT_COLUMNS * LIB_CONSOLE_TEXT_ROWS];
    lib_u32 palette[16u]; /* 0x00RRGGBB */
} lib_console_text_frame;

typedef struct lib_console_event {
    lib_console_event_kind kind;
    lib_u32 binding_generation;
    union {
        lib_console_raw_key raw_key;
        lib_console_raw_mouse raw_mouse;
        lib_console_line line;
    } value;
} lib_console_event;

typedef void (*lib_console_event_sink)(void *context,
    const lib_console_event *event);

lib_status lib_console_create(lib_console **out_console);
lib_console *lib_console_retain(lib_console *console);
void lib_console_release(lib_console *console);
void lib_console_destroy(lib_console *console);
lib_status lib_console_set_event_sink(lib_console *console,
    lib_console_event_sink sink, void *context);
lib_status lib_console_write_text(lib_console *console,
    const char *text, lib_size length);
lib_status lib_console_present_text_frame(lib_console *console,
    const lib_console_text_frame *frame);

#endif
