#include "machine.h"
#include <windows.h>
#include "dib_surface.h"
#include "test_cleanup.h"

#include <stdio.h>
#include <string.h>

extern void host_timer_event(void);

static int fail(const char *message)
{
    fprintf(stderr, "vga CCPU update smoke: %s\n", message);
    return 1;
}

static int write_boot_image(const char *path)
{
    uint8_t sector[512] = { 0 };
    static const uint8_t program[] = {
        0xb8, 0x05, 0x6f,       /* mov ax, 6f05h (V7 extended set mode) */
        0xbb, 0x67, 0x00,       /* mov bx, 0067h (640x480x256) */
        0xcd, 0x10,             /* int 10h */
        0xb8, 0x00, 0xa0,       /* mov ax, a000h */
        0x8e, 0xc0,             /* mov es, ax */
        0xc6, 0x06, 0x01, 0x05, 0x55, /* mov byte ptr [0501h], 55h */
        0xeb, 0xfe,             /* wait; test patches this to EB 00 */
        0xbf, 0x00, 0x00,       /* mov di, 0000h */
        0xb0, 0xff,             /* mov al, ffh */
        0xaa,                   /* stosb */
        0xb9, 0x40, 0x00,       /* mov cx, 0040h */
        0xf3, 0xaa,             /* rep stosb */
        0xbf, 0x00, 0x01,       /* mov di, 0100h */
        0x66, 0xb8, 0xff, 0xff, 0xff, 0xff, /* mov eax, ffffffffh */
        0x66, 0xb9, 0x10, 0x00, 0x00, 0x00, /* mov ecx, 10h */
        0x66, 0xf3, 0xab,       /* rep stosd */
        0xc6, 0x06, 0x00, 0x05, 0x5a, /* mov byte ptr [0500h], 5ah */
        0xeb, 0xfe              /* halt here */
    };
    FILE *file;

    memcpy(sector, program, sizeof(program));
    sector[510] = 0x55;
    sector[511] = 0xaa;
    file = fopen(path, "wb");
    if (file == NULL) {
        return 0;
    }
    if (fwrite(sector, 1, sizeof(sector), file) != sizeof(sector)) {
        fclose(file);
        return 0;
    }
    return fclose(file) == 0;
}

static int run_until(softpc_machine *machine, uint32_t address, uint8_t value)
{
    unsigned int slice;
    uint8_t observed;

    for (slice = 0; slice != 64; ++slice) {
        if (softpc_machine_run(machine, 6000u) != SOFTPC_MACHINE_OK ||
            softpc_machine_read_physical(machine, address, &observed,
                sizeof(observed)) != SOFTPC_MACHINE_OK) {
            return 0;
        }
        if (observed == value) {
            return 1;
        }
    }
    return 0;
}

int main(void)
{
    const char *image_path = "vga-ccpu-update.img";
    const uint8_t release_wait[] = { 0xeb, 0x00 };
    uint8_t before[256];
    const void *bits;
    const void *info;
    unsigned long width;
    unsigned long height;
    long dirty_left;
    long dirty_top;
    long dirty_right;
    long dirty_bottom;
    softpc_machine_options options = { 0 };
    softpc_machine *machine = NULL;
    int result = 1;

    if (!write_boot_image(image_path)) {
        return fail("could not write the local test boot image");
    }
    options.floppy_path = image_path;
    options.memory_bytes = 4u * 1024u * 1024u;
    options.presentation = SOFTPC_PRESENTATION_WINDOW;
    options.media_mode = SOFTPC_MEDIA_READONLY;
    if (softpc_machine_create(&options, &machine) != SOFTPC_MACHINE_OK ||
        softpc_machine_reset(machine) != SOFTPC_MACHINE_OK) {
        result = fail("could not create or reset machine");
        goto done;
    }
    if (!run_until(machine, 0x501u, 0x55u)) {
        result = fail("guest did not reach graphics-ready marker");
        goto done;
    }

    /* Settle the mode switch and discard its initial full-surface update. */
    host_timer_event();
    host_timer_event();
    (void)softpc_standalone_dib_take_dirty(&dirty_left, &dirty_top,
        &dirty_right, &dirty_bottom);
    if (!softpc_standalone_dib_surface(&bits, &info, &width, &height) ||
        bits == NULL || width != 640 || height != 480) {
        result = fail("V7 640x480 mode did not create its original DIB surface");
        goto done;
    }
    memcpy(before, bits, sizeof(before));

    if (softpc_machine_write_physical(machine, 0x7c12u, release_wait,
            sizeof(release_wait)) != SOFTPC_MACHINE_OK ||
        !run_until(machine, 0x500u, 0x5au)) {
        result = fail("guest did not execute VGA write sequence");
        goto done;
    }
    host_timer_event();
    host_timer_event();
    if (!softpc_standalone_dib_surface(&bits, &info, &width, &height) ||
        bits == NULL || memcmp(before, bits, sizeof(before)) == 0) {
        result = fail("CCPU VGA writes did not reach the original DIB painter");
        goto done;
    }
    if (!softpc_standalone_dib_take_dirty(&dirty_left, &dirty_top,
            &dirty_right, &dirty_bottom)) {
        result = fail("CCPU VGA writes did not publish a dirty rectangle");
        goto done;
    }
    /* Windows 3.1's V7 virtual-display driver can change its presentable
       surface while handling a key make, without a directly mapped VGA
       store.  The standalone host must ask the original renderer to refresh
       after that hardware event, just as it already does for InPort mouse
       input. */
    if (softpc_machine_key_number(machine, 30u, 0u) != SOFTPC_MACHINE_OK) {
        result = fail("could not inject keyboard make event");
        goto done;
    }
    host_timer_event();
    host_timer_event();
    if (!softpc_standalone_dib_take_dirty(&dirty_left, &dirty_top,
            &dirty_right, &dirty_bottom)) {
        result = fail("keyboard make did not request original graphics refresh");
        goto done;
    }
    result = 0;

done:
    softpc_machine_destroy(machine);
    (void)softpc_test_remove_image(image_path);
    return result;
}
