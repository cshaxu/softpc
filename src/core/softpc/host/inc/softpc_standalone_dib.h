#ifndef SOFTPC_STANDALONE_DIB_H
#define SOFTPC_STANDALONE_DIB_H

/* The original nt_ega.c/nt_vga.c render into this small Console-DIB
 * contract. Standalone owns the surface and receives only dirty rectangles. */
typedef struct _CONSOLE_GRAPHICS_BUFFER_INFO {
    void *lpBitMap;
    PBITMAPINFO lpBitMapInfo;
    HANDLE hMutex;
} CONSOLE_GRAPHICS_BUFFER_INFO;

BOOL softpc_standalone_invalidate_dibits(HANDLE ignored,
    const SMALL_RECT *rect);

int softpc_standalone_dib_surface(const void **bits_out, const void **info_out,
    unsigned long *width_out, unsigned long *height_out);
/* Rebind the original nt_graph output contract to standalone-owned storage
 * after a controller mode change.  Unlike graphicsResize's historical
 * console-buffer branch, this never transfers ownership to a Windows
 * console server. */
int softpc_standalone_dib_resize(int width, int height, int bits_per_pixel);
int softpc_standalone_text_surface(const void **cells_out,
    unsigned long *columns_out, unsigned long *rows_out,
    unsigned long *stride_out, unsigned long *cell_bytes_out);
void softpc_standalone_dib_set_palette(PC_palette *palette, int count);
int softpc_standalone_dib_take_dirty(long *left, long *top, long *right,
    long *bottom);

#define InvalidateConsoleDIBits(handle, rect) \
    softpc_standalone_invalidate_dibits((handle), (rect))

#endif
