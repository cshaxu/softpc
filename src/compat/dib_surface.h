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

int softpc_standalone_dib_init(void);
int softpc_standalone_dib_surface(const void **bits_out, const void **info_out,
    unsigned long *width_out, unsigned long *height_out);
/* Bind the DIB allocated by original nt_graph::CreateSpcDIB to standalone
 * pixel storage.  The original header remains the painter contract; a
 * separate RGB header is published to the frontend. */
int softpc_standalone_dib_bind(PBITMAPINFO painter_info);
/* Every host display update uses this one transaction boundary. A bind or
 * damage update remains private until the matching outer end. */
void softpc_standalone_dib_begin_update(void);
void softpc_standalone_dib_end_update(void);
BOOL softpc_standalone_dib_damage(const SMALL_RECT *rect);
unsigned long softpc_standalone_dib_generation(void);
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
    ((void)(handle), softpc_standalone_dib_damage((rect)))

#endif
