#include "machine.h"
#include <windows.h>
#include "dib_surface.h"
#include "test_cleanup.h"

#include <stdio.h>
#include <string.h>

extern void host_timer_event(void);

static uint32_t vga_ccpu_final_jump;

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
        /* Indexed V7 extension registers must accept both byte I/O and the
           PC's 16-bit index/value form (AL = index, AH = value). */
        0xba, 0xc4, 0x03,       /* mov dx, 03c4h */
        0xb8, 0x06, 0xea,       /* mov ax, ea06h (unlock SR6) */
        0xef,                   /* out dx, ax */
        0xb0, 0x06,             /* mov al, 06h */
        0xee,                   /* out dx, al */
        0x42,                   /* inc dx */
        0xec,                   /* in al, dx */
        0x88, 0x06, 0x04, 0x05, /* mov [0504h], al */
        0x4a,                   /* dec dx */
        0xb8, 0xf8, 0x5a,       /* mov ax, 5af8h (F8 = 5Ah) */
        0xef,                   /* out dx, ax */
        0x89, 0x16, 0x07, 0x05, /* mov [0507h], dx */
        0xec,                   /* in al, dx (current sequencer index) */
        0x88, 0x06, 0x05, 0x05, /* mov [0505h], al */
        0x42,                   /* inc dx */
        0xec,                   /* in al, dx (F8 after word write) */
        0x88, 0x06, 0x02, 0x05, /* mov [0502h], al */
        0x4a,                   /* dec dx */
        0xb0, 0xf8,             /* mov al, f8h */
        0xee,                   /* out dx, al */
        0x42,                   /* inc dx */
        0xec,                   /* in al, dx */
        0x88, 0x06, 0x06, 0x05, /* mov [0506h], al */
        0x4a,                   /* dec dx */
        0xb0, 0x06,             /* mov al, 06h */
        0xee,                   /* out dx, al */
        0x42,                   /* inc dx */
        0xb0, 0xea,             /* mov al, eah */
        0xee,                   /* out dx, al */
        0x4a,                   /* dec dx */
        0xb0, 0xf8,             /* mov al, f8h */
        0xee,                   /* out dx, al */
        0x42,                   /* inc dx */
        0xb0, 0x01,             /* mov al, 01h */
        0xee,                   /* out dx, al */
        0x4a,                   /* dec dx */
        0xb0, 0xf8,             /* mov al, f8h */
        0xee,                   /* out dx, al */
        0x42,                   /* inc dx */
        0xec,                   /* in al, dx */
        0x88, 0x06, 0x03, 0x05, /* mov [0503h], al */
        0x4a,                   /* dec dx */
        0xb8, 0xf8, 0x00,       /* mov ax, 00f8h (restore F8) */
        0xef,                   /* out dx, ax */
        /* Revision-3 extension registers discard reserved write bits. */
        0xb8, 0xf3, 0xff,       /* mov ax, fff3h */
        0xef,                   /* out dx, ax */
        0xb0, 0xf3,             /* mov al, f3h */
        0xee,                   /* out dx, al */
        0x42,                   /* inc dx */
        0xec,                   /* in al, dx */
        0x88, 0x06, 0x08, 0x05, /* mov [0508h], al */
        0x4a,                   /* dec dx */
        0xb8, 0xf3, 0x00,       /* mov ax, 00f3h (disable masked writes) */
        0xef,                   /* out dx, ax */
        0xb8, 0xf9, 0xff,       /* mov ax, fff9h */
        0xef,                   /* out dx, ax */
        0xb0, 0xf9,             /* mov al, f9h */
        0xee,                   /* out dx, al */
        0x42,                   /* inc dx */
        0xec,                   /* in al, dx */
        0x88, 0x06, 0x09, 0x05, /* mov [0509h], al */
        0x4a,                   /* dec dx */
        0xb8, 0xfa, 0xff,       /* mov ax, fffah */
        0xef,                   /* out dx, ax */
        0xb0, 0xfa,             /* mov al, fah */
        0xee,                   /* out dx, al */
        0x42,                   /* inc dx */
        0xec,                   /* in al, dx */
        0x88, 0x06, 0x0a, 0x05, /* mov [050ah], al */
        0x4a,                   /* dec dx */
        0xb8, 0xfb, 0xff,       /* mov ax, fffbh */
        0xef,                   /* out dx, ax */
        0xb0, 0xfb,             /* mov al, fbh */
        0xee,                   /* out dx, al */
        0x42,                   /* inc dx */
        0xec,                   /* in al, dx */
        0x88, 0x06, 0x0b, 0x05, /* mov [050bh], al */
        0x4a,                   /* dec dx */
        0xb8, 0xfe, 0xff,       /* mov ax, fffeh */
        0xef,                   /* out dx, ax */
        0xb0, 0xfe,             /* mov al, feh */
        0xee,                   /* out dx, al */
        0x42,                   /* inc dx */
        0xec,                   /* in al, dx */
        0x88, 0x06, 0x0c, 0x05, /* mov [050ch], al */
        0x4a,                   /* dec dx */
        0xb8, 0xa5, 0x7f,       /* mov ax, 7fa5h */
        0xef,                   /* out dx, ax */
        0xb0, 0xa5,             /* mov al, a5h */
        0xee,                   /* out dx, al */
        0x42,                   /* inc dx */
        0xec,                   /* in al, dx */
        0x88, 0x06, 0x0d, 0x05, /* mov [050dh], al */
        0x4a,                   /* dec dx */
        0xb8, 0xc1, 0xff,       /* mov ax, ffc1h */
        0xef,                   /* out dx, ax */
        0xb0, 0xc1,             /* mov al, c1h */
        0xee,                   /* out dx, al */
        0x42,                   /* inc dx */
        0xec,                   /* in al, dx */
        0x88, 0x06, 0x0e, 0x05, /* mov [050eh], al */
        0x4a,                   /* dec dx */
        0xb8, 0xf1, 0xff,       /* mov ax, fff1h */
        0xef,                   /* out dx, ax */
        0xb0, 0xf1,             /* mov al, f1h */
        0xee,                   /* out dx, al */
        0x42,                   /* inc dx */
        0xec,                   /* in al, dx */
        0x88, 0x06, 0x0f, 0x05, /* mov [050fh], al */
        0x4a,                   /* dec dx */
        0xb8, 0xff, 0x00,       /* mov ax, 00ffh (ERFF controls clear) */
        0xef,                   /* out dx, ax */
        0xb0, 0xff,             /* mov al, ffh */
        0xee,                   /* out dx, al */
        0x42,                   /* inc dx */
        0xec,                   /* in al, dx */
        0x88, 0x06, 0x10, 0x05, /* mov [0510h], al */
        0x4a,                   /* dec dx */
        0xb8, 0xf9, 0x00,       /* mov ax, 00f9h */
        0xef,                   /* out dx, ax */
        0xb8, 0xfe, 0x00,       /* mov ax, 00feh */
        0xef,                   /* out dx, ax */
        0xbf, 0x00, 0x00,       /* mov di, 0000h */
        0xb0, 0xff,             /* mov al, ffh */
        0xaa,                   /* stosb */
        0xb9, 0x40, 0x00,       /* mov cx, 0040h */
        0xf3, 0xaa,             /* rep stosb */
        0xbf, 0x00, 0x01,       /* mov di, 0100h */
        0x66, 0xb8, 0xff, 0xff, 0xff, 0xff, /* mov eax, ffffffffh */
        0x66, 0xb9, 0x10, 0x00, 0x00, 0x00, /* mov ecx, 10h */
        0x66, 0xf3, 0xab,       /* rep stosd */
        0x31, 0xc0,             /* xor ax, ax */
        0x8e, 0xd8,             /* mov ds, ax */
        0xbe, 0x00, 0x06,       /* mov si, 0600h */
        0xbf, 0x00, 0x02,       /* mov di, 0200h */
        0x66, 0xb9, 0x10, 0x00, 0x00, 0x00, /* mov ecx, 10h */
        0x66, 0xf3, 0xa5,       /* rep movsd */
        /* F6 independently selects the CPU-write and CRTC-read banks. On the
           selected 512 KiB V7, the two fields remain distinct. */
        0xba, 0xc4, 0x03,       /* mov dx, 03c4h */
        0xb8, 0xf6, 0x40,       /* mov ax, 40f6h (CPU write bank = 1) */
        0xef,                   /* out dx, ax */
        0xbf, 0x00, 0x00,       /* mov di, 0000h */
        0x66, 0xb8, 0x7e, 0x7e, 0x7e, 0x7e, /* mov eax, 7e7e7e7eh */
        0x66, 0xb9, 0x01, 0x00, 0x00, 0x00, /* mov ecx, 1 */
        0x66, 0xf3, 0xab,       /* rep stosd into the virtual page */
        0xb8, 0xf6, 0x10,       /* mov ax, 10f6h (CRTC read bank = 1) */
        0xef,                   /* out dx, ax */
        0xc6, 0x06, 0x00, 0x05, 0x5a, /* mov byte ptr [0500h], 5ah */
        0xeb, 0xfe              /* halt here */
    };
    FILE *file;

    if (sizeof(program) > 0x1c0u)
        return 0;
    vga_ccpu_final_jump = 0x7c00u + (uint32_t)sizeof(program) - 2u;
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
    static const uint8_t colour_expansion[] = {
        /* Remain in the V7 BIOS' 67h 640x480x256 mode selected by the
           boot sector.  The installed V7VGA.DRV uses this exact mode and
           programs FE=06h while expanding graphical Prompt characters. */
        0x31, 0xc0,             /* xor ax, ax */
        0x8e, 0xd8,             /* mov ds, ax */
        0xb8, 0x00, 0xa0,       /* mov ax, a000h */
        0x8e, 0xc0,             /* mov es, ax */
        0xba, 0xc4, 0x03,       /* mov dx, 03c4h */
        0xb8, 0x06, 0xea,       /* mov ax, ea06h */
        0xef,                   /* out dx, ax */
        0xb0, 0xfc,             /* mov al, fch */
        0xee,                   /* out dx, al */
        0x42,                   /* inc dx */
        0xec,                   /* in al, dx */
        0x88, 0x06, 0x12, 0x05, /* mov [0512h], al */
        0x4a,                   /* dec dx */
        0xb8, 0xf6, 0x00,       /* mov ax, 00f6h */
        0xef,                   /* out dx, ax */
        0xb8, 0xfa, 0x0e,       /* mov ax, 0efah (yellow) */
        0xef,                   /* out dx, ax */
        0xb8, 0xfb, 0x01,       /* mov ax, 01fbh (blue) */
        0xef,                   /* out dx, ax */
        0xb8, 0xfe, 0x06,       /* mov ax, 06feh (CPU-source solid) */
        0xef,                   /* out dx, ax */
        0x31, 0xff,             /* xor di, di */
        0xb0, 0xa5,             /* mov al, a5h */
        0xaa,                   /* stosb */
        0xc6, 0x06, 0x11, 0x05, 0x5a, /* mov byte ptr [0511h], 5ah */
        0xeb, 0xfe              /* controlled end */
    };
    uint8_t before[256];
    uint8_t copy_source[64];
    const void *bits;
    const void *info;
    unsigned long width;
    unsigned long height;
    long dirty_left;
    long dirty_top;
    long dirty_right;
    long dirty_bottom;
    uint8_t jump[3];
    uint16_t displacement;
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

    memset(copy_source, 0x3cu, sizeof(copy_source));
    if (softpc_machine_write_physical(machine, 0x600u, copy_source,
            sizeof(copy_source)) != SOFTPC_MACHINE_OK ||
        softpc_machine_write_physical(machine, 0x7c12u, release_wait,
            sizeof(release_wait)) != SOFTPC_MACHINE_OK ||
        !run_until(machine, 0x500u, 0x5au)) {
        result = fail("guest did not execute VGA write sequence");
        goto done;
    }
    {
        uint8_t v7_word_value;
        uint8_t v7_byte_value;
        uint8_t v7_unlock_value;
        uint8_t v7_word_index;
        uint8_t v7_masks[9];
        uint16_t v7_word_dx;
        if (softpc_machine_read_physical(machine, 0x502u, &v7_word_value,
                sizeof(v7_word_value)) != SOFTPC_MACHINE_OK ||
            softpc_machine_read_physical(machine, 0x503u, &v7_byte_value,
                sizeof(v7_byte_value)) != SOFTPC_MACHINE_OK ||
            softpc_machine_read_physical(machine, 0x504u, &v7_unlock_value,
                sizeof(v7_unlock_value)) != SOFTPC_MACHINE_OK ||
            softpc_machine_read_physical(machine, 0x505u, &v7_word_index,
                sizeof(v7_word_index)) != SOFTPC_MACHINE_OK ||
            softpc_machine_read_physical(machine, 0x507u, &v7_word_dx,
                sizeof(v7_word_dx)) != SOFTPC_MACHINE_OK ||
            softpc_machine_read_physical(machine, 0x508u, v7_masks,
                sizeof(v7_masks)) != SOFTPC_MACHINE_OK ||
            v7_word_value != 0x5au || v7_byte_value != 0x01u) {
            fprintf(stderr, "vga CCPU update smoke: V7 SR6=%02x F8 word=%02x index=%02x DX=%04x byte=%02x\n",
                (unsigned int)v7_unlock_value, (unsigned int)v7_word_value,
                (unsigned int)v7_word_index, (unsigned int)v7_word_dx,
                (unsigned int)v7_byte_value);
            result = fail("CCPU word I/O did not round-trip V7 F8 clock register");
            goto done;
        }
	if (memcmp(v7_masks, "\x03\x01\x0f\x0f\x0e\x09\x01\x33\x80",
                sizeof(v7_masks)) != 0) {
            result = fail("V7 revision-3 extension register masks did not round-trip");
            goto done;
        }
    }
    host_timer_event();
    host_timer_event();
    if (!softpc_standalone_dib_surface(&bits, &info, &width, &height) ||
        bits == NULL || memcmp(before, bits, sizeof(before)) == 0) {
        result = fail("CCPU VGA writes did not reach the original DIB painter");
        goto done;
    }
    if (memcmp((const uint8_t *)bits + 0x200u, copy_source,
            sizeof(copy_source)) != 0) {
        result = fail("CCPU REP MOVSD did not reach the V7 DIB painter");
        goto done;
    }
    if (memcmp(bits, "\x7e\x7e\x7e\x7e", 4) != 0) {
        result = fail("V7 CRTC bank-one virtual-page string write did not paint at the visible origin");
        goto done;
    }
    if (!softpc_standalone_dib_take_dirty(&dirty_left, &dirty_top,
            &dirty_right, &dirty_bottom)) {
        result = fail("CCPU VGA writes did not publish a dirty rectangle");
        goto done;
    }
    displacement = (uint16_t)(0x7dc0u - (vga_ccpu_final_jump + 3u));
    jump[0] = 0xe9u;
    jump[1] = (uint8_t)displacement;
    jump[2] = (uint8_t)(displacement >> 8);
    if (softpc_machine_write_physical(machine, 0x7dc0u, colour_expansion,
            sizeof(colour_expansion)) != SOFTPC_MACHINE_OK ||
        softpc_machine_write_physical(machine, vga_ccpu_final_jump, jump,
            sizeof(jump)) != SOFTPC_MACHINE_OK ||
        !run_until(machine, 0x511u, 0x5au)) {
        result = fail("CCPU did not execute V7 FE=06h colour expansion");
        goto done;
    }
    for (displacement = 0u; displacement != 8u; ++displacement)
        host_timer_event();
    if (!softpc_standalone_dib_surface(&bits, &info, &width, &height) ||
        bits == NULL || width != 640u || height != 480u ||
        memcmp(bits, "\x0e\x01\x0e\x01\x01\x0e\x01\x0e", 8u) != 0) {
        if (bits != NULL) {
            const uint8_t *observed = (const uint8_t *)bits;
            fprintf(stderr, "vga CCPU update smoke: FE=06h mode67 first pixels");
            for (displacement = 0u; displacement != 16u; ++displacement)
                fprintf(stderr, " %02x", (unsigned int)observed[displacement]);
            fputc('\n', stderr);
        }
        result = fail("V7 FE=06h STOSB did not paint 640x480 foreground/background pixels");
        goto done;
    }
    {
        uint8_t compatibility_control;

        if (softpc_machine_read_physical(machine, 0x512u,
                &compatibility_control, sizeof(compatibility_control)) !=
                SOFTPC_MACHINE_OK || compatibility_control != 0x6cu) {
            result = fail("V7 BIOS mode 67 did not retain its FC=6Ch extended-256 configuration");
            goto done;
        }
    }
    result = 0;

done:
    softpc_machine_destroy(machine);
    (void)softpc_test_remove_image(image_path);
    return result;
}
