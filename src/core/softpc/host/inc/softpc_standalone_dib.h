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

#define InvalidateConsoleDIBits(handle, rect) \
    softpc_standalone_invalidate_dibits((handle), (rect))

#endif
