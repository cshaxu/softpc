#ifndef WIN32_PRESENTATION_FRAME_H
#define WIN32_PRESENTATION_FRAME_H

#include <stdint.h>

/* Value-only ABI shared between a project runtime and the Win32 presenter.
 * It intentionally carries no renderer pointer, machine pointer, or lock. */
#define WIN32_PRESENTATION_TEXT_COLUMNS 80u
#define WIN32_PRESENTATION_TEXT_ROWS 25u
#define WIN32_PRESENTATION_DIB_MAX_WIDTH 1280u
#define WIN32_PRESENTATION_DIB_MAX_HEIGHT 768u
#define WIN32_PRESENTATION_DIB_MAX_BYTES \
    (WIN32_PRESENTATION_DIB_MAX_WIDTH * WIN32_PRESENTATION_DIB_MAX_HEIGHT)
#define WIN32_PRESENTATION_DIB_INFO_BYTES 1064u

typedef struct win32_presentation_frame {
    uint32_t sequence;
    uint32_t graphics;
    uint32_t valid;
    int32_t cursor_column;
    int32_t cursor_row;
    uint32_t cursor_size;
    uint8_t text[WIN32_PRESENTATION_TEXT_COLUMNS * WIN32_PRESENTATION_TEXT_ROWS];
    uint16_t attributes[WIN32_PRESENTATION_TEXT_COLUMNS * WIN32_PRESENTATION_TEXT_ROWS];
    uint32_t text_palette[16u];
    uint8_t font[256u * 16u];
    uint8_t secondary_font[256u * 16u];
    uint32_t font_height;
    uint32_t attribute_font_select;
    uint32_t dib_width;
    uint32_t dib_height;
    int32_t dirty_left;
    int32_t dirty_top;
    int32_t dirty_right;
    int32_t dirty_bottom;
    uint8_t dib_info[WIN32_PRESENTATION_DIB_INFO_BYTES];
    uint8_t dib_bits[WIN32_PRESENTATION_DIB_MAX_BYTES];
} win32_presentation_frame;

#endif
