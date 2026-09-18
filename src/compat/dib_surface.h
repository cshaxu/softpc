#ifndef SOFTPC_STANDALONE_DIB_H
#define SOFTPC_STANDALONE_DIB_H
#include <windows.h>

/* The original nt_ega.c/nt_vga.c render into this small Console-DIB
 * contract. Standalone owns the surface and receives only dirty rectangles. */
typedef struct _CONSOLE_GRAPHICS_BUFFER_INFO {
    DWORD dwBitMapInfoLength;
    PBITMAPINFO lpBitMapInfo;
    DWORD dwUsage;
    HANDLE hMutex;
    void *lpBitMap;
} CONSOLE_GRAPHICS_BUFFER_INFO, *PCONSOLE_GRAPHICS_BUFFER_INFO;

BOOL softpc_standalone_invalidate_dibits(HANDLE ignored,
    const SMALL_RECT *rect);

int softpc_standalone_dib_init(void);
int softpc_standalone_dib_surface(const void **bits_out, const void **info_out,
    unsigned long *width_out, unsigned long *height_out);
/* Bind the DIB allocated by original nt_graph::CreateSpcDIB to standalone
 * pixel storage.  The original header remains the painter contract; a
 * separate RGB header is published to the frontend. */
int softpc_standalone_dib_bind(PBITMAPINFO painter_info);
/* A newly bound painter target becomes observable after the original painter
 * submits its first dirty region. Compat-only overlays query this before
 * touching pixels, and tag cached pixels with the binding generation. */
int softpc_standalone_dib_ready(void);
unsigned long softpc_standalone_dib_generation(void);
/* Palette and hardware-pointer updates are overlays, rather than original
 * painter output. They may update an observable DIB only. */
BOOL softpc_standalone_dib_invalidate_overlay(const SMALL_RECT *rect);
int softpc_standalone_text_surface(const void **cells_out,
    unsigned long *columns_out, unsigned long *rows_out,
    unsigned long *stride_out, unsigned long *cell_bytes_out);
int softpc_standalone_text_surface_geometry(unsigned long *columns_out,
    unsigned long *rows_out);
int softpc_standalone_text_surface_fill_character(unsigned long start,
    unsigned long count, unsigned char value, unsigned long *written_out);
int softpc_standalone_text_surface_fill_attribute(unsigned long start,
    unsigned long count, unsigned char value, unsigned long *written_out);
int softpc_standalone_text_surface_write_cell(unsigned long column,
    unsigned long row, unsigned long byte_offset, unsigned char value);
void softpc_standalone_dib_set_palette_entries(const PALETTEENTRY *entries,
    int count);
unsigned long softpc_standalone_dib_palette_history(const RGBQUAD **entries);
int softpc_standalone_dib_take_dirty(long *left, long *top, long *right,
    long *bottom);
void softpc_standalone_dib_invalidate_all(void);

#define InvalidateConsoleDIBits(handle, rect) \
    softpc_standalone_invalidate_dibits((handle), (rect))

#endif
