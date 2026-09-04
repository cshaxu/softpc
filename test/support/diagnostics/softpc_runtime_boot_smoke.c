#include "runtime.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>

extern unsigned long softpc_standalone_dib_palette_history(
    const RGBQUAD **entries);
/* Original 8042/SAS state, observed only by this integration harness to
   distinguish a guest loader issue from an A20 physical-address wrap. */
extern int gate_a20_status;
extern int sas_twenty_bit_wrapping_enabled(void);

static int frame_contains(const softpc_runtime_frame *frame, const char *needle)
{
    unsigned int row;
    for (row = 0u; row < SOFTPC_RUNTIME_TEXT_ROWS; ++row) {
        char line[SOFTPC_RUNTIME_TEXT_COLUMNS + 1u];
        unsigned int column;
        for (column = 0u; column < SOFTPC_RUNTIME_TEXT_COLUMNS; ++column) {
            unsigned char c = frame->text[row * SOFTPC_RUNTIME_TEXT_COLUMNS + column];
            line[column] = c >= 0x20u && c < 0x7fu ? (char)c : ' ';
        }
        line[SOFTPC_RUNTIME_TEXT_COLUMNS] = '\0';
        if (strstr(line, needle) != NULL) return 1;
    }
    return 0;
}

static int frame_has_exact_line(const softpc_runtime_frame *frame,
    const char *needle)
{
    unsigned int row;
    size_t length;

    if (frame == NULL || needle == NULL) return 0;
    length = strlen(needle);
    if (length > SOFTPC_RUNTIME_TEXT_COLUMNS) return 0;
    for (row = 0u; row < SOFTPC_RUNTIME_TEXT_ROWS; ++row) {
        const unsigned char *line = &frame->text[row * SOFTPC_RUNTIME_TEXT_COLUMNS];
        if (memcmp(line, needle, length) != 0) continue;
        if (length == SOFTPC_RUNTIME_TEXT_COLUMNS || line[length] == ' ')
            return 1;
    }
    return 0;
}

static int frame_has_prompt(const softpc_runtime_frame *frame)
{
    unsigned int row;
    for (row = 0u; row < SOFTPC_RUNTIME_TEXT_ROWS; ++row) {
        unsigned int column;
        for (column = 0u; column + 2u < SOFTPC_RUNTIME_TEXT_COLUMNS; ++column) {
            const uint8_t *text = &frame->text[row * SOFTPC_RUNTIME_TEXT_COLUMNS + column];
            if (((text[0] >= 'A' && text[0] <= 'Z') ||
                 (text[0] >= 'a' && text[0] <= 'z')) &&
                ((text[1] == ':' &&
                  (text[2] == '>' || (column + 3u < SOFTPC_RUNTIME_TEXT_COLUMNS &&
                  text[2] == '\\' && text[3] == '>'))) ||
                 /* The original text surface can coalesce the ':' cell in
                    the DOS 5 short prompt.  The guest command processor
                    still exposes its unambiguous drive-letter prompt. */
                 text[1] == '>')) return 1;
        }
    }
    return 0;
}

/* Runtime frames are host-owned copies.  Reading this snapshot therefore
   exercises the standalone presentation boundary without racing the original
   renderer or its CCPU executor. */
static int graphics_frame_has_visible_pixel(const softpc_runtime_frame *frame)
{
    const BITMAPINFO *dib;
    uint32_t stride;
    uint32_t row;
    uint32_t non_black_pixels = 0u;
    uint32_t first_colour = 0u;
    int have_first_colour = 0;
    int have_second_colour = 0;

    if (frame == NULL || frame->graphics == 0u || frame->dib_width == 0u ||
        frame->dib_height == 0u || frame->dib_width > SOFTPC_RUNTIME_DIB_MAX_WIDTH ||
        frame->dib_height > SOFTPC_RUNTIME_DIB_MAX_HEIGHT) return 0;
    dib = (const BITMAPINFO *)frame->dib_info;
    stride = (frame->dib_width + 3u) & ~3u;
    for (row = 0u; row < frame->dib_height; ++row) {
        uint32_t column;
        for (column = 0u; column < frame->dib_width; ++column) {
            unsigned char index = frame->dib_bits[row * stride + column];
            const RGBQUAD *colour = &dib->bmiColors[index];
            uint32_t packed = (uint32_t)colour->rgbRed |
                ((uint32_t)colour->rgbGreen << 8) |
                ((uint32_t)colour->rgbBlue << 16);
            if (packed != 0u) {
                ++non_black_pixels;
                if (!have_first_colour) {
                    first_colour = packed;
                    have_first_colour = 1;
                } else if (packed != first_colour) {
                    have_second_colour = 1;
                }
            }
        }
    }
    /* A palette-only update can turn an untouched frame into one flat
       non-black colour.  Windows Setup's graphical handoff must instead
       produce both detail and a meaningful painted area. */
    return have_second_colour && non_black_pixels >=
        (frame->dib_width * frame->dib_height) / 1000u;
}

/* Opt-in evidence capture for a real rendered guest frame.  The runtime
   frame is already an owned BI_RGB snapshot, so writing it here neither
   reads controller state nor changes the original SoftPC renderer. */
static int write_graphics_frame_bmp(const softpc_runtime_frame *frame,
    const char *path)
{
    const BITMAPINFO *dib;
    BITMAPFILEHEADER file_header;
    BITMAPINFOHEADER info_header;
    uint32_t stride;
    size_t bits_bytes;
    FILE *file;

    if (frame == NULL || path == NULL || path[0] == '\0' ||
        frame->graphics == 0u || frame->dib_width == 0u ||
        frame->dib_height == 0u || frame->dib_width >
        SOFTPC_RUNTIME_DIB_MAX_WIDTH || frame->dib_height >
        SOFTPC_RUNTIME_DIB_MAX_HEIGHT) return 0;
    dib = (const BITMAPINFO *)frame->dib_info;
    stride = (frame->dib_width + 3u) & ~3u;
    bits_bytes = (size_t)stride * frame->dib_height;
    if (bits_bytes > SOFTPC_RUNTIME_DIB_MAX_BYTES) return 0;
    memset(&file_header, 0, sizeof(file_header));
    file_header.bfType = 0x4d42u;
    file_header.bfOffBits = sizeof(file_header) + sizeof(info_header) +
        256u * sizeof(RGBQUAD);
    file_header.bfSize = file_header.bfOffBits + (DWORD)bits_bytes;
    info_header = dib->bmiHeader;
    info_header.biClrUsed = 256u;
    info_header.biClrImportant = 0u;
    file = fopen(path, "wb");
    if (file == NULL) return 0;
    if (fwrite(&file_header, 1u, sizeof(file_header), file) !=
            sizeof(file_header) ||
        fwrite(&info_header, 1u, sizeof(info_header), file) !=
            sizeof(info_header) ||
        fwrite(dib->bmiColors, sizeof(RGBQUAD), 256u, file) != 256u ||
        fwrite(frame->dib_bits, 1u, bits_bytes, file) != bits_bytes ||
        fclose(file) != 0) {
        fclose(file);
        return 0;
    }
    return 1;
}

static void dump_setup_text_attributes(const softpc_runtime_frame *frame)
{
    unsigned long foreground[16] = { 0u };
    unsigned long background[16] = { 0u };
    unsigned int index;

    if (frame == NULL || frame->graphics != 0u) return;
    for (index = 0u; index < SOFTPC_RUNTIME_TEXT_COLUMNS *
            SOFTPC_RUNTIME_TEXT_ROWS; ++index) {
        unsigned short attribute = frame->attributes[index];
        foreground[attribute & 0x0fu]++;
        background[(attribute >> 4) & 0x0fu]++;
    }
    fprintf(stderr, "softpc-runtime-boot-smoke: text foreground counts");
    for (index = 0u; index < 16u; ++index)
        fprintf(stderr, " %u=%lu", index, foreground[index]);
    fprintf(stderr, " background counts");
    for (index = 0u; index < 16u; ++index)
        fprintf(stderr, " %u=%lu", index, background[index]);
    fputc('\n', stderr);
    fflush(stderr);
}

static void dump_setup_text_palette(const softpc_runtime_frame *frame)
{
    unsigned int index;

    if (frame == NULL || frame->graphics != 0u) return;
    fprintf(stderr, "softpc-runtime-boot-smoke: text RGB palette");
    for (index = 0u; index < 16u; ++index) {
        COLORREF colour = (COLORREF)frame->text_palette[index];
        fprintf(stderr, " %u=%02x%02x%02x", index,
            (unsigned int)GetRValue(colour), (unsigned int)GetGValue(colour),
            (unsigned int)GetBValue(colour));
    }
    fputc('\n', stderr);
    fflush(stderr);
}

/* Keep palette diagnosis at the copied runtime boundary.  This records the
   exact RGB table sent to the standalone window, rather than inferring a
   colour from a screenshot or from guest controller state. */
static void dump_graphics_palette(const softpc_runtime_frame *frame)
{
    const BITMAPINFO *dib;
    unsigned int index;
    if (frame == NULL || frame->graphics == 0u) return;
    dib = (const BITMAPINFO *)frame->dib_info;
    fprintf(stderr, "softpc-runtime-boot-smoke: graphics palette");
    for (index = 0u; index < 16u; ++index) {
        const RGBQUAD *colour = &dib->bmiColors[index];
        fprintf(stderr, " %u=%02x%02x%02x", index,
            (unsigned int)colour->rgbRed, (unsigned int)colour->rgbGreen,
            (unsigned int)colour->rgbBlue);
    }
    fputc('\n', stderr);
    fflush(stderr);
}

/* The indexed bitmap alone cannot tell whether a wrong graphical colour
 * originated in the original VGA attribute-controller map or at the final
 * DIB palette outlet.  Record both sides for real Setup diagnostics without
 * changing guest state. */
/* Diagnose an incomplete graphical handoff strictly through the public
   physical-memory boundary.  This avoids peeking at or mutating CCPU state
   while its executor thread is still alive. */
static void dump_current_instruction(const softpc_machine *machine)
{
    uint32_t address = 0u;
    uint32_t start;
    unsigned char bytes[96];
    unsigned int index;

    if (machine == NULL ||
        softpc_machine_instruction_address(machine, &address) !=
            SOFTPC_MACHINE_OK ||
        softpc_machine_read_physical(machine, address, bytes,
            sizeof(bytes)) != SOFTPC_MACHINE_OK)
        return;
    start = address >= 48u ? address - 48u : 0u;
    if (softpc_machine_read_physical(machine, start, bytes,
            sizeof(bytes)) != SOFTPC_MACHINE_OK)
        return;
    fprintf(stderr, "softpc-runtime-boot-smoke: instruction window %05lx (IP %05lx):\n",
        (unsigned long)start, (unsigned long)address);
    for (index = 0u; index < sizeof(bytes); ++index) {
        if (index % 16u == 0u) fprintf(stderr, "  %05lx:",
            (unsigned long)(start + index));
        fprintf(stderr, " %02x", (unsigned int)bytes[index]);
        if (index % 16u == 15u) fputc('\n', stderr);
    }
}

/* The stalled Windows loader invokes the same current LDT code selector at a
   fixed offset.  Keep this as a smoke-harness observation: the production
   machine API remains physical-memory-only and does not expose CCPU's
   descriptor internals. */
static void dump_current_code_target(const softpc_machine *machine,
    uint32_t instruction_address, uint32_t eip)
{
    uint32_t code_base;
    uint32_t target;
    unsigned char bytes[64];
    unsigned int index;

    if (machine == NULL || instruction_address < eip) return;
    code_base = instruction_address - eip;
    if (code_base > UINT32_MAX - 0x4f59u) return;
    target = code_base + 0x4f59u;
    if (softpc_machine_read_physical(machine, target, bytes,
            sizeof(bytes)) != SOFTPC_MACHINE_OK) return;
    fprintf(stderr, "softpc-runtime-boot-smoke: protected loader target %05lx:\n",
        (unsigned long)target);
    for (index = 0u; index < sizeof(bytes); ++index) {
        if (index % 16u == 0u) fprintf(stderr, "  %05lx:",
            (unsigned long)(target + index));
        fprintf(stderr, " %02x", (unsigned int)bytes[index]);
        if (index % 16u == 15u) fputc('\n', stderr);
    }
    fflush(stderr);
}

static void dump_machine_palette(const softpc_machine *machine)
{
    const void *bits;
    const void *info;
    uint32_t width;
    uint32_t height;
    const BITMAPINFO *dib;
    unsigned int index;

    if (machine == NULL || !softpc_machine_presentation_dib(machine, &bits,
            &info, &width, &height) || info == NULL) return;
    (void)bits; (void)width; (void)height;
    dib = (const BITMAPINFO *)info;
    fprintf(stderr, "softpc-runtime-boot-smoke: final DIB palette");
    for (index = 0u; index < 16u; ++index) {
        const RGBQUAD *colour = &dib->bmiColors[index];
        fprintf(stderr, " %u=%02x%02x%02x", index,
            (unsigned int)colour->rgbRed, (unsigned int)colour->rgbGreen,
            (unsigned int)colour->rgbBlue);
    }
    fputc('\n', stderr);
    fflush(stderr);
}

static void dump_palette_history(void)
{
    const RGBQUAD *history;
    unsigned long count = softpc_standalone_dib_palette_history(&history);
    unsigned long begin = count > 8u ? count - 8u : 0u;
    unsigned long sequence;

    for (sequence = begin; sequence < count; ++sequence) {
        const RGBQUAD *palette = history + (sequence % 8u) * 16u;
        fprintf(stderr, "softpc-runtime-boot-smoke: palette[%lu]",
            sequence);
        for (unsigned int index = 0u; index < 16u; ++index)
            fprintf(stderr, " %u=%02x%02x%02x", index,
                (unsigned int)palette[index].rgbRed,
                (unsigned int)palette[index].rgbGreen,
                (unsigned int)palette[index].rgbBlue);
        fputc('\n', stderr);
    }
    fflush(stderr);
}

static int send_key(softpc_runtime *runtime, uint8_t key_number)
{
    return softpc_runtime_enqueue_key(runtime, key_number, 0u) &&
        softpc_runtime_enqueue_key(runtime, key_number, 1u);
}

/* The runtime queue takes SoftPC key numbers, rather than PC scan codes.
   Keep this probe on exactly the same Win32 -> original key-code path as the
   interactive console and window frontends. */
extern BYTE KeyMsgToKeyCode(PKEY_EVENT_RECORD KeyEvent);

static int enqueue_virtual_key(softpc_runtime *runtime, WORD virtual_key,
    DWORD control_state, uint8_t released)
{
    KEY_EVENT_RECORD event;
    BYTE key_number;
    WORD scan;

    ZeroMemory(&event, sizeof(event));
    event.bKeyDown = released == 0u;
    event.wVirtualKeyCode = virtual_key;
    /* Match win32_keyboard.c exactly: the Setup menu distinguishes the
       extended cursor Up key from keypad 8. */
    scan = (WORD)MapVirtualKeyExW(virtual_key, MAPVK_VK_TO_VSC_EX,
        GetKeyboardLayout(0u));
    /* This test runs in RDP as well as locally.  Some RDP keyboard layouts
       report a bare 48h for VK_UP through MapVirtualKeyEx even though a real
       window WM_KEYDOWN carries E0 48h.  Model that frontend message exactly
       so the smoke tests the original arrow, rather than keypad 8. */
    if (virtual_key == VK_UP) scan = 0xe048u;
    if (virtual_key == VK_DOWN) scan = 0xe050u;
    event.wVirtualScanCode = (WORD)(scan & 0xffu);
    event.dwControlKeyState = control_state;
    if ((scan & 0xff00u) == 0xe000u)
        event.dwControlKeyState |= ENHANCED_KEY;
    key_number = KeyMsgToKeyCode(&event);
    return key_number != 0u && softpc_runtime_enqueue_key(runtime, key_number,
        released);
}

/* Match the Win32 frontend's ordinary key transition contract: the host
   posts the make and break together before the executor's next timer turn.
   Deliberately holding a dialog key for 100 ms enters the original keyboard
   INT 15 nesting frame first and can strand the synthetic break outside it;
   that is not how the actual console/window frontend presents a tap. */
static int enqueue_virtual_key_pair(softpc_runtime *runtime, WORD virtual_key)
{
    return enqueue_virtual_key(runtime, virtual_key, 0u, 0u) &&
        enqueue_virtual_key(runtime, virtual_key, 0u, 1u);
}

static void send_enter(softpc_runtime *runtime)
{
    (void)enqueue_virtual_key_pair(runtime, VK_RETURN);
}

static int send_setup_navigation_key(softpc_runtime *runtime, WORD virtual_key)
{
    return enqueue_virtual_key_pair(runtime, virtual_key);
}

static int send_character_slow(softpc_runtime *runtime, WCHAR character)
{
    SHORT translation = VkKeyScanW(character);
    BYTE modifiers;
    WORD virtual_key;

    if (translation == -1) return 0;
    modifiers = HIBYTE(translation);
    virtual_key = LOBYTE(translation);
    if ((modifiers & 1u) != 0u && !enqueue_virtual_key(runtime, VK_SHIFT,
        0u, 0u)) return 0;
    if (!enqueue_virtual_key_pair(runtime, virtual_key)) return 0;
    if ((modifiers & 1u) != 0u && !enqueue_virtual_key(runtime, VK_SHIFT,
        0u, 1u)) return 0;
    /* Human input reaches the original 8042 as closely spaced make/break
       pairs, but not an entire DOS command in one controller turn. */
    /* The real frontend naturally has a human inter-key delay.  Windows 3.1
       boot traces execute substantial disk work between controller services,
       so leave enough time for each make/break pair to retire before issuing
       the next command character.  This is test-shell pacing only. */
    Sleep(100u);
    return 1;
}

static int send_windows_setup_command(softpc_runtime *runtime)
{
    static const WCHAR command[] = L"c:\\ewin31\\setup.exe";
    WCHAR overridden_command[260];
    DWORD overridden_length;
    const WCHAR *selected_command = command;
    unsigned int index;

    overridden_length = GetEnvironmentVariableW(L"SOFTPC_WINDOWS_SETUP_COMMAND",
        overridden_command, (DWORD)(sizeof(overridden_command) /
            sizeof(overridden_command[0])));
    if (overridden_length != 0u && overridden_length <
        sizeof(overridden_command) / sizeof(overridden_command[0]))
        selected_command = overridden_command;
    for (index = 0u; selected_command[index] != L'\0'; ++index)
        if (!send_character_slow(runtime, selected_command[index])) return 0;
    return enqueue_virtual_key_pair(runtime, VK_RETURN);
}

/* The first graphical Windows 3.1 Setup page owns a username edit field.
 * Keep this probe outside the machine and use the same individual make/break
 * transitions as a human window frontend.  It deliberately omits Enter: the
 * acceptance condition is that typed input neither terminates the executor
 * nor returns Setup to DOS. */
static int send_setup_username_probe(softpc_runtime *runtime)
{
    static const WCHAR username[] = L"SoftPC User";
    unsigned int index;

    for (index = 0u; username[index] != L'\0'; ++index)
        if (!send_character_slow(runtime, username[index])) return 0;
    return 1;
}

/* This is only used against an in-memory media overlay after the graphical
 * handoff has remained text-only.  A returned COMMAND.COM prompt will echo
 * it; an active loader will not, distinguishing stale text paint from a
 * genuine DOS return without mutating the supplied image. */
static int send_dos_prompt_probe(softpc_runtime *runtime)
{
    static const WCHAR command[] = L"echo SOFTPC_SETUP_RETURNED";
    unsigned int index;

    for (index = 0u; command[index] != L'\0'; ++index)
        if (!send_character_slow(runtime, command[index])) return 0;
    return enqueue_virtual_key_pair(runtime, VK_RETURN);
}

static int send_dos_errorlevel_probe(softpc_runtime *runtime)
{
    static const WCHAR command[] = L"if errorlevel 1 echo SOFTPC_SETUP_ERROR";
    unsigned int index;

    for (index = 0u; command[index] != L'\0'; ++index)
        if (!send_character_slow(runtime, command[index])) return 0;
    return enqueue_virtual_key_pair(runtime, VK_RETURN);
}

static void dump_frame(const softpc_runtime_frame *frame)
{
    unsigned int row;
    if (frame == NULL || frame->valid == 0u) return;
    for (row = 0u; row < SOFTPC_RUNTIME_TEXT_ROWS; ++row) {
        char line[SOFTPC_RUNTIME_TEXT_COLUMNS + 1u];
        unsigned int column;
        for (column = 0u; column < SOFTPC_RUNTIME_TEXT_COLUMNS; ++column) {
            unsigned char c = frame->text[row * SOFTPC_RUNTIME_TEXT_COLUMNS + column];
            line[column] = c >= 0x20u && c < 0x7fu ? (char)c : ' ';
        }
        line[SOFTPC_RUNTIME_TEXT_COLUMNS] = '\0';
        fprintf(stderr, "%s\n", line);
    }
}

/* Keep the Setup-navigation trace outside the machine boundary.  It is
   opt-in because it records the host-owned presentation snapshot, not guest
   memory; this makes an unattended stock SETUP /i run diagnosable without
   changing a controller, BIOS service, or renderer. */
static uint32_t trace_setup_frame(const softpc_runtime_frame *frame)
{
    uint32_t hash = 2166136261u;
    unsigned int index;

    if (frame == NULL) return 0u;
    for (index = 0u; index < SOFTPC_RUNTIME_TEXT_COLUMNS *
            SOFTPC_RUNTIME_TEXT_ROWS; ++index) {
        hash ^= frame->text[index];
        hash *= 16777619u;
        hash ^= (uint8_t)frame->attributes[index];
        hash *= 16777619u;
    }
    hash ^= frame->graphics;
    hash *= 16777619u;
    hash ^= frame->dib_width;
    hash *= 16777619u;
    hash ^= frame->dib_height;
    hash *= 16777619u;
    hash ^= (uint32_t)frame->cursor_column;
    hash *= 16777619u;
    hash ^= (uint32_t)frame->cursor_row;
    return hash;
}

/* The graphical guest cursor is rendered by the selected Windows MOUSE.DRV
   into the original SoftPC VGA surface.  Hash that owned presentation copy,
   not a frontend cursor or host pointer, so a motion regression proves the
   physical InPort -> IRQ9 -> guest-driver -> original-renderer path. */
static uint32_t graphics_frame_hash(const softpc_runtime_frame *frame)
{
    uint32_t hash = 2166136261u;
    uint32_t stride;
    size_t bytes;
    size_t index;

    if (frame == NULL || frame->graphics == 0u || frame->dib_width == 0u ||
        frame->dib_height == 0u || frame->dib_width >
        SOFTPC_RUNTIME_DIB_MAX_WIDTH || frame->dib_height >
        SOFTPC_RUNTIME_DIB_MAX_HEIGHT) return 0u;
    stride = (frame->dib_width + 3u) & ~3u;
    bytes = (size_t)stride * frame->dib_height;
    if (bytes > SOFTPC_RUNTIME_DIB_MAX_BYTES) return 0u;
    for (index = 0u; index < bytes; ++index) {
        hash ^= frame->dib_bits[index];
        hash *= 16777619u;
    }
    return hash;
}

/* Regression for the executor's original HLT rendezvous.  The guest reaches
 * HLT with IF enabled; only a host key queued through runtime may release it.
 * This proves that an event wake is consumed on the CCPU HLT path rather than
 * merely waking and immediately re-waiting. */
static int run_halted_keyboard_probe(void)
{
    static const char image_path[] = "softpc-runtime-hlt-keyboard.img";
    unsigned char sector[512] = { 0 };
    softpc_machine_options options = { image_path, NULL,
        SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    softpc_runtime *runtime = NULL;
    FILE *image;
    DWORD deadline;
    uint8_t marker = 0u;
    uint8_t ready = 0u;
    softpc_runtime_frame frame;
    uint32_t sequence_before = 0u;
    uint32_t sequence_after = 0u;
    DWORD key_queued_at = 0u;
    DWORD key_delivered_at = 0u;
    int success = 0;

    /* The probe owns IRQ1 rather than relying on whatever the boot ROM put
       in the BIOS keyboard vector.  That keeps this specifically about the
       original 8042 -> PIC -> CCPU HLT chain: the handler marks 0500h,
       acknowledges the master PIC, and returns. */
    static const unsigned char program[] = {
        0xfau, 0x31u, 0xc0u, 0x8eu, 0xd8u,
        0xc6u, 0x06u, 0x01u, 0x05u, 0x55u,
        /* IVT[9] = CS:7c21, where the local IRQ1 handler starts. */
        0xb8u, 0x21u, 0x7cu, 0xa3u, 0x24u, 0x00u,
        0x0eu, 0x58u, 0xa3u, 0x26u, 0x00u,
        /* STI's one-instruction inhibition is deliberately followed by
           NOP, so HLT sees an already-enabled INTR exactly as CCPU does. */
        0xfbu, 0x90u, 0xf4u,
        0xc6u, 0x06u, 0x00u, 0x05u, 0xa5u, 0xfau, 0xf4u,
        0xebu, 0xfdu,
        /* 7c21: mov byte [0500],a5; mov al,20; out 20,al; iret */
        0xc6u, 0x06u, 0x00u, 0x05u, 0xa5u,
        0xb0u, 0x20u, 0xe6u, 0x20u, 0xcfu
    };
    memcpy(sector, program, sizeof(program));
    sector[510] = 0x55u;
    sector[511] = 0xaau;
    image = fopen(image_path, "wb");
    if (image == NULL) goto done;
    if (fwrite(sector, 1u, sizeof(sector), image) != sizeof(sector) ||
        fclose(image) != 0) {
        image = NULL;
        goto done;
    }
    image = NULL;
    options.media_mode = SOFTPC_MEDIA_OVERLAY;
    if (softpc_machine_create(&options, &machine) != SOFTPC_MACHINE_OK ||
        !softpc_runtime_create(machine, &runtime) ||
        !softpc_runtime_start(runtime)) goto done;
    deadline = GetTickCount() + 5000u;
    do {
        if (softpc_machine_read_physical(machine, 0x501u, &ready,
                sizeof(ready)) == SOFTPC_MACHINE_OK && ready == 0x55u)
            break;
        Sleep(10u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
    if (ready != 0x55u) {
        fprintf(stderr, "halted keyboard probe: boot marker=%02x state=%d\n",
            ready, (int)softpc_runtime_get_state(runtime));
        goto done;
    }
    memset(&frame, 0, sizeof(frame));
    if (softpc_runtime_copy_frame(runtime, &frame))
        sequence_before = frame.sequence;
    key_queued_at = GetTickCount();
    if (!softpc_runtime_enqueue_key(runtime, 31u, 0u)) goto done;
    deadline = GetTickCount() + 3000u;
    do {
        if (softpc_machine_read_physical(machine, 0x500u, &marker,
                sizeof(marker)) == SOFTPC_MACHINE_OK && marker == 0xa5u) {
            key_delivered_at = GetTickCount();
            (void)softpc_runtime_enqueue_key(runtime, 31u, 1u);
            /* This is the whole outer input path: queue, wake, original
               8042/PIC delivery and the CCPU HLT return.  It must not be
               deferred to the 50 ms timer heartbeat or a frontend repaint. */
            success = (DWORD)(key_delivered_at - key_queued_at) <= 250u;
            if (!success)
                fprintf(stderr, "halted keyboard probe: delivery took %lu ms\n",
                    (unsigned long)(key_delivered_at - key_queued_at));
            break;
        }
        Sleep(10u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
    memset(&frame, 0, sizeof(frame));
    if (softpc_runtime_copy_frame(runtime, &frame))
        sequence_after = frame.sequence;
    if (!success)
        fprintf(stderr, "halted keyboard probe: resume marker=%02x state=%d frame=%lu->%lu\n",
            marker, (int)softpc_runtime_get_state(runtime),
            (unsigned long)sequence_before, (unsigned long)sequence_after);
done:
    if (image != NULL) fclose(image);
    if (runtime != NULL) {
        (void)softpc_runtime_stop(runtime);
        softpc_runtime_destroy(runtime);
    }
    if (machine != NULL) softpc_machine_destroy(machine);
    (void)remove(image_path);
    return success;
}

int main(int argc, char **argv)
{
    softpc_machine_options options = { NULL, NULL, SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    softpc_runtime *runtime = NULL;
    softpc_runtime_frame *frame = NULL;
    DWORD deadline;
    int date_sent = 0, time_sent = 0, input_stage = 0, success = 0;
    int overlay = 0, direct = 0, windows_setup = 0, windows_setup_complete = 0;
    int windows_setup_custom = 0;
    int skip_halted_keyboard_probe = 0;
    int setup_command_sent = 0;
    DWORD setup_prompt_visible_at = 0u;
    int setup_enter_stage = 0;
    int setup_text_attributes_dumped = 0;
    int setup_handoff_text_attributes_dumped = 0;
    int setup_loading_text_attributes_dumped = 0;
    int setup_enter_release_sent = 0;
    int setup_failed_install_declined = 0;
    int setup_directory_accepted = 0;
    int setup_directory_retry_sent = 0;
    int setup_prompt_probe_sent = 0;
    int setup_errorlevel_probe_sent = 0;
    DWORD setup_errorlevel_probe_at = 0u;
    int setup_hardware_mouse_requested = 0;
    int setup_hardware_inport_mouse_selected = 0;
    int setup_hardware_display_requested = 0;
    int setup_hardware_v7_display_selected = 0;
    int setup_hardware_summary_accepted = 0;
    int setup_hardware_summary_retry_sent = 0;
    int setup_mouse_motion_sent = 0;
    int setup_mouse_surface_changed = 0;
    unsigned int setup_display_summary_moves = 0u;
    unsigned int setup_display_choice_moves = 0u;
    unsigned int setup_summary_mouse_moves = 0u;
    unsigned int setup_mouse_choice_moves = 0u;
    DWORD setup_summary_navigation_at = 0u;
    DWORD setup_mouse_choice_navigation_at = 0u;
    DWORD setup_display_summary_navigation_at = 0u;
    DWORD setup_display_choice_navigation_at = 0u;
    DWORD setup_hardware_summary_accepted_at = 0u;
    int setup_frame_trace = 0;
    int setup_mouse_motion = 0;
    int guest_command_expects_graphics = 0;
    const char *setup_frame_bmp = NULL;
    int setup_frame_bmp_written = 0;
    uint32_t setup_last_frame_hash = 0u;
    uint32_t setup_mouse_surface_baseline_hash = 0u;
    uint32_t setup_mouse_surface_before_hash = 0u;
    DWORD setup_mouse_surface_baseline_at = 0u;
    DWORD setup_enter_make_at = 0u;
    WORD setup_enter_virtual_key = VK_RETURN;
    DWORD setup_graphics_started_at = 0u;
    DWORD setup_handoff_at = 0u;
    int setup_username_probe_sent = 0;
    DWORD setup_directory_enter_at = 0u;
    DWORD setup_timeout = 60000u;
    int final_state = SOFTPC_RUNTIME_ERROR;
    int final_machine_result = SOFTPC_MACHINE_INVALID_ARGUMENT;
    uint16_t final_cs = 0u;
    uint32_t final_eip = 0u;
    uint32_t final_address = 0u;
    int argument;
    const char *floppy_path = NULL;
    const char *hard_disk_path = NULL;
    if (argc >= 4 && strcmp(argv[1], "--no-floppy") == 0 &&
        strcmp(argv[2], "--hdd") == 0) {
        hard_disk_path = argv[3];
        argument = 4;
    } else if (argc >= 5 && strcmp(argv[1], "--floppy") == 0 &&
        strcmp(argv[3], "--hdd") == 0) {
        floppy_path = argv[2];
        hard_disk_path = argv[4];
        argument = 5;
    } else return 2;
    for (; argument < argc; ++argument) {
        if (strcmp(argv[argument], "--overlay") == 0 && !overlay) {
            overlay = 1;
        } else if (strcmp(argv[argument], "--direct") == 0 && !direct) {
            direct = 1;
        } else if (strcmp(argv[argument], "--windows-setup") == 0 && !windows_setup) {
            windows_setup = 1;
        } else if (strcmp(argv[argument], "--windows-setup-complete") == 0 &&
            !windows_setup_complete) {
            windows_setup = 1;
            windows_setup_complete = 1;
        } else if (strcmp(argv[argument], "--windows-setup-custom") == 0 &&
            !windows_setup_custom) {
            windows_setup = 1;
            windows_setup_custom = 1;
        } else if (strcmp(argv[argument], "--skip-halted-keyboard-probe") == 0 &&
            !skip_halted_keyboard_probe) {
            skip_halted_keyboard_probe = 1;
        } else if (strcmp(argv[argument], "--windows-setup-mouse-motion") == 0 &&
            !setup_mouse_motion) {
            windows_setup = 1;
            windows_setup_complete = 1;
	    windows_setup_custom = 1;
            setup_mouse_motion = 1;
        } else return 2;
    }
    if (!skip_halted_keyboard_probe && !run_halted_keyboard_probe()) {
        fprintf(stderr, "softpc-runtime-boot-smoke: halted keyboard probe failed\n");
        return 1;
    }
    /* Normal probes require an explicit overlay.  --direct exists solely for
       an expendable diagnostic clone supplied by the caller. */
    if (!overlay && !direct) return 2;
    {
        char memory_text[32];
        DWORD memory_length = GetEnvironmentVariableA(
            "SOFTPC_TEST_MEMORY_MB", memory_text, (DWORD)sizeof(memory_text));
        if (memory_length != 0u && memory_length < sizeof(memory_text)) {
            char *end;
            unsigned long mib = strtoul(memory_text, &end, 10);
            if (*end != '\0' && (mib < 1u || mib > 4095u)) return 2;
            if (*end == '\0') options.memory_bytes = (uint32_t)(mib * 1024u * 1024u);
        }
    }
    options.floppy_path = floppy_path;
    options.hard_disk_path = hard_disk_path;
    if (overlay && direct) return 2;
    options.media_mode = direct ? SOFTPC_MEDIA_DIRECT : SOFTPC_MEDIA_OVERLAY;
    if (softpc_machine_create(&options, &machine) != SOFTPC_MACHINE_OK ||
        !softpc_runtime_create(machine, &runtime) || !softpc_runtime_start(runtime))
        goto done;
    frame = (softpc_runtime_frame *)calloc(1u, sizeof(*frame));
    if (frame == NULL) goto done;
    /* This is an integration probe, not an unattended boot soak.  Leave
       enough room for firmware POST but emit its captured frame before any
       shutdown work if the guest does not reach the prompt. */
    if (windows_setup) {
        char timeout_text[32];
        DWORD timeout_length = GetEnvironmentVariableA(
            "SOFTPC_WINDOWS_SETUP_TIMEOUT_MS", timeout_text,
            (DWORD)sizeof(timeout_text));
        if (timeout_length != 0u && timeout_length < sizeof(timeout_text)) {
            char *end;
            unsigned long timeout = strtoul(timeout_text, &end, 10);
            if (*end == '\0' && timeout >= 60000u && timeout <= 900000u)
                setup_timeout = (DWORD)timeout;
        }
        setup_frame_trace = GetEnvironmentVariableA("SOFTPC_SETUP_FRAME_TRACE",
            timeout_text, (DWORD)sizeof(timeout_text)) != 0u;
        setup_frame_bmp = getenv("SOFTPC_SETUP_FRAME_BMP");
        guest_command_expects_graphics = GetEnvironmentVariableA(
            "SOFTPC_GUEST_COMMAND_EXPECTS_GRAPHICS", timeout_text,
            (DWORD)sizeof(timeout_text)) != 0u;
    }
    deadline = GetTickCount() + (windows_setup ? setup_timeout : 20000u);
    do {
        if (windows_setup && setup_enter_stage != 0 &&
            !setup_enter_release_sent &&
            (DWORD)(GetTickCount() - setup_enter_make_at) >= 100u) {
            if (!enqueue_virtual_key(runtime, setup_enter_virtual_key, 0u, 1u)) break;
            setup_enter_release_sent = 1;
        }
        if (softpc_runtime_copy_frame(runtime, frame)) {
            if (windows_setup && setup_frame_trace) {
                uint32_t frame_hash = trace_setup_frame(frame);
                if (frame_hash != setup_last_frame_hash) {
                    fprintf(stderr, "setup frame: graphics=%u dib=%lux%lu cursor=%ld,%ld hash=%08lx\n",
                        (unsigned int)frame->graphics,
                        (unsigned long)frame->dib_width,
                        (unsigned long)frame->dib_height,
                        (long)frame->cursor_column,
                        (long)frame->cursor_row,
                        (unsigned long)frame_hash);
                    if (frame->graphics == 0u) dump_frame(frame);
                    fflush(stderr);
                    setup_last_frame_hash = frame_hash;
                }
            }
            if (windows_setup && setup_command_sent) {
                if (frame->graphics != 0u) {
                    if (setup_enter_stage >= 4 &&
                        graphics_frame_has_visible_pixel(frame)) {
                        if (!setup_mouse_motion && !setup_frame_bmp_written &&
                            setup_frame_bmp != NULL &&
                            setup_frame_bmp[0] != '\0') {
                            if (!write_graphics_frame_bmp(frame, setup_frame_bmp)) {
                                fprintf(stderr,
                                    "softpc-runtime-boot-smoke: could not write graphics frame %s\n",
                                    setup_frame_bmp);
                                break;
                            }
                            fprintf(stderr,
                                "softpc-runtime-boot-smoke: wrote graphics frame %s\n",
                                setup_frame_bmp);
                            fflush(stderr);
                            setup_frame_bmp_written = 1;
                        }
                        if (!windows_setup_complete) {
                            success = 1;
                            break;
                        }
                        if (setup_graphics_started_at == 0u)
                        {
                            setup_graphics_started_at = GetTickCount();
                            dump_graphics_palette(frame);
                        }
                        /* Let the original graphical loader establish its
                           first dialog before testing its username field.
                           A whole edit string is still serialized one scan
                           transition at a time by send_character_slow(). */
                        if (windows_setup_complete && !setup_mouse_motion &&
                            !setup_username_probe_sent &&
                            (DWORD)(GetTickCount() - setup_graphics_started_at)
                                >= 1000u) {
                            if (!send_setup_username_probe(runtime)) break;
                            setup_username_probe_sent = 1;
                            fprintf(stderr,
                                "setup stage: graphical username input sent\n");
                            fflush(stderr);
                        }
                        /* The original Microsoft InPort driver writes the
                           guest-owned cursor into the graphical surface.
                           Exercise that physical IRQ path after the first
                           dialog so the standalone DIB outlet is verified
                           beyond the text-to-graphics transition. */
                        if (setup_mouse_motion && !setup_mouse_motion_sent &&
                            (DWORD)(GetTickCount() - setup_graphics_started_at)
                                >= (guest_command_expects_graphics ? 100u : 2500u) &&
                            setup_mouse_surface_baseline_hash == 0u) {
                            setup_mouse_surface_baseline_hash =
                                graphics_frame_hash(frame);
                            setup_mouse_surface_baseline_at = GetTickCount();
                            if (setup_mouse_surface_baseline_hash == 0u) break;
                        }
                        /* Do not accept a later palette or dialog refresh as
                           mouse evidence.  The original V7 surface must be
                           stable immediately before the physical input edge. */
                        if (setup_mouse_motion && !setup_mouse_motion_sent &&
                            setup_mouse_surface_baseline_hash != 0u &&
                            (DWORD)(GetTickCount() - setup_mouse_surface_baseline_at)
                                >= 250u &&
                            graphics_frame_hash(frame) ==
                                setup_mouse_surface_baseline_hash) {
                            setup_mouse_surface_before_hash =
                                graphics_frame_hash(frame);
                            if (setup_mouse_surface_before_hash == 0u) break;
                            if (!softpc_runtime_enqueue_mouse(runtime, 96, 96,
                                    0u, 0u)) break;
                            setup_mouse_motion_sent = 1;
                            fprintf(stderr, "setup stage: physical mouse motion sent\n");
                            fflush(stderr);
                        }
                        if (setup_mouse_motion && setup_mouse_motion_sent &&
                            graphics_frame_hash(frame) !=
                            setup_mouse_surface_before_hash) {
                            setup_mouse_surface_changed = 1;
                            if (!setup_frame_bmp_written &&
                                setup_frame_bmp != NULL &&
                                setup_frame_bmp[0] != '\0') {
                                if (!write_graphics_frame_bmp(frame,
                                        setup_frame_bmp)) {
                                    fprintf(stderr,
                                        "softpc-runtime-boot-smoke: could not write mouse frame %s\n",
                                        setup_frame_bmp);
                                    break;
                                }
                                fprintf(stderr,
                                    "softpc-runtime-boot-smoke: wrote InPort motion frame %s\n",
                                    setup_frame_bmp);
                                fflush(stderr);
                                setup_frame_bmp_written = 1;
                            }
                        }
                        /* The historical smoke stopped at the first Setup
                           graphics frame.  Windows 3.x can still return to
                           DOS immediately after that handoff, so diagnostic
                           completion requires the graphical guest to remain
                           active through several original timer/service
                           turns. */
                        if ((DWORD)(GetTickCount() - setup_graphics_started_at)
                            >= 15000u) {
                            if (setup_mouse_motion && !setup_mouse_surface_changed) {
                                fprintf(stderr,
                                    "softpc-runtime-boot-smoke: InPort motion did not change the guest VGA surface\n");
                                break;
                            }
                            /* Keep evidence representative of the sustained
                               graphical state, not merely the first palette
                               transition that entered graphics mode. */
                            if (!setup_mouse_motion && setup_frame_bmp != NULL &&
                                setup_frame_bmp[0] != '\0' &&
                                !write_graphics_frame_bmp(frame, setup_frame_bmp)) {
                                fprintf(stderr,
                                    "softpc-runtime-boot-smoke: could not update graphics frame %s\n",
                                    setup_frame_bmp);
                                break;
                            }
                            fprintf(stderr,
                                "softpc-runtime-boot-smoke: Windows Setup graphics sustained for 15000 ms\n");
                            fflush(stderr);
                            success = 1;
                            break;
                        }
                    }
                    goto next_frame;
                }
                if (setup_enter_stage >= 4 &&
                    !setup_handoff_text_attributes_dumped) {
                    dump_setup_text_attributes(frame);
                    setup_handoff_text_attributes_dumped = 1;
                }
                if (!setup_loading_text_attributes_dumped &&
                    frame_contains(frame, "Please wait while Setup loads Windows")) {
                    /* A pristine installation target has no upgrade page.
                       Express Setup proceeds directly to this original
                       Windows loader handoff, so it is the authoritative
                       point after which the probe must require graphics. */
                    if (setup_enter_stage < 4) {
                        setup_enter_stage = 4;
                        setup_handoff_at = GetTickCount();
                        fprintf(stderr, "setup stage: graphical handoff requested\n");
                        fflush(stderr);
                    }
                    dump_setup_text_attributes(frame);
                    dump_setup_text_palette(frame);
                    setup_loading_text_attributes_dumped = 1;
                }
                if (windows_setup_complete && setup_enter_stage >= 4 &&
                    setup_handoff_at != 0u && !setup_prompt_probe_sent &&
                    (DWORD)(GetTickCount() - setup_handoff_at) >= 10000u) {
                    if (!send_dos_prompt_probe(runtime)) break;
                    setup_prompt_probe_sent = 1;
                }
                if (setup_prompt_probe_sent &&
                    frame_contains(frame, "SOFTPC_SETUP_RETURNED")) {
                    fprintf(stderr,
                        "softpc-runtime-boot-smoke: Setup returned to DOS before graphics handoff\n");
                    if (!setup_errorlevel_probe_sent) {
                        if (!send_dos_errorlevel_probe(runtime)) break;
                        setup_errorlevel_probe_sent = 1;
                        setup_errorlevel_probe_at = GetTickCount();
                    }
                }
                if (setup_errorlevel_probe_sent &&
                    frame_has_exact_line(frame, "SOFTPC_SETUP_ERROR")) {
                    fprintf(stderr, "softpc-runtime-boot-smoke: Setup returned nonzero errorlevel\n");
                    break;
                }
                if (windows_setup_complete && setup_graphics_started_at != 0u &&
                    frame_has_prompt(frame)) {
                    fprintf(stderr,
                        "softpc-runtime-boot-smoke: Windows returned to DOS %lu ms after graphics handoff\n",
                        (unsigned long)(GetTickCount() - setup_graphics_started_at));
                    break;
                }
                if (windows_setup_complete && setup_enter_stage >= 4 &&
                    frame_has_prompt(frame) && setup_errorlevel_probe_sent &&
                    (DWORD)(GetTickCount() - setup_errorlevel_probe_at) >= 1000u) {
                    fprintf(stderr,
                        "softpc-runtime-boot-smoke: Setup returned zero errorlevel before graphics handoff\n");
                    break;
                }
                if (frame_contains(frame, "Welcome to Setup.")) {
                    if (!setup_text_attributes_dumped) {
                        dump_setup_text_attributes(frame);
                        setup_text_attributes_dumped = 1;
                    }
                    if (setup_enter_stage == 0) {
                        /* Model the separate make/break messages received by
                           the console/window. Feeding both into one 8042
                           service turn can collapse this real Setup key. */
                        if (!enqueue_virtual_key_pair(runtime, VK_RETURN))
                            break;
                        setup_enter_make_at = GetTickCount();
                        setup_enter_stage = 1;
                        fprintf(stderr, "setup stage: welcome accepted\n");
                        fflush(stderr);
                        setup_enter_release_sent = 1;
                    }
                    goto next_frame;
                }
                if (windows_setup_custom && !setup_hardware_mouse_requested &&
                    frame_contains(frame, "Setup has determined that your system")) {
                    DWORD now = GetTickCount();

                    /* The stock /i summary initially highlights “No Changes”.
                       Mouse is five items above it (Network, Language,
                       Keyboard Layout, Keyboard, Mouse). Select the original
                       Mouse Systems serial or bus mouse entry rather than
                       accepting Setup's PS/2 auto-detection. Send exactly
                       one complete transition per original keyboard turn;
                       queuing five cursor presses in one host callback loses
                       their menu semantics in the 8042/BIOS pipeline. */
                    if ((LONG)(now - setup_summary_navigation_at) < 0)
                        goto next_frame;
                    if (setup_summary_mouse_moves < 5u) {
                        if (!send_setup_navigation_key(runtime, VK_UP)) break;
                        setup_summary_mouse_moves++;
                        setup_summary_navigation_at = now + 250u;
                    } else {
                        if (!send_setup_navigation_key(runtime, VK_RETURN))
                            break;
                        setup_hardware_mouse_requested = 1;
                        fprintf(stderr, "setup stage: mouse alternatives requested\n");
                        fflush(stderr);
                    }
                    goto next_frame;
                }
                if (windows_setup_custom && setup_hardware_mouse_requested &&
                    !setup_hardware_inport_mouse_selected && frame_contains(frame,
                    "You have asked to change the type of Mouse")) {
                    DWORD now = GetTickCount();

                    /* Setup opens this page at Genius serial mouse on COM1.
                       SoftPC implements Microsoft InPort hardware at
                       23Ch--23Fh on IRQ9, not the distinct Mouse Systems
                       serial/bus protocol.  Windows 3.1's MOUSE.DRV probes
                       InPort first; it is the fifth visible item here, so
                       four complete Down transitions choose the Setup label
                       "Microsoft, or IBM PS/2". */
                    if ((LONG)(now - setup_mouse_choice_navigation_at) < 0)
                        goto next_frame;
                    if (setup_mouse_choice_moves < 4u) {
                        if (!send_setup_navigation_key(runtime, VK_DOWN)) break;
                        setup_mouse_choice_moves++;
                        setup_mouse_choice_navigation_at = now + 250u;
                    } else {
                        if (!send_setup_navigation_key(runtime, VK_RETURN))
                            break;
                        setup_hardware_inport_mouse_selected = 1;
                        fprintf(stderr, "setup stage: InPort MOUSE.DRV selected\n");
                        fflush(stderr);
                    }
                    goto next_frame;
                }
                if (windows_setup_custom && setup_hardware_inport_mouse_selected &&
                    !setup_hardware_summary_accepted &&
                    frame_contains(frame, "No Changes") &&
                    frame_contains(frame, "Microsoft, or IBM PS/2")) {
                    /* Preserve Setup's detected Video 7 display selection;
                       this probe changes only the mouse driver. */
                    if (!send_setup_navigation_key(runtime, VK_RETURN)) break;
                    setup_hardware_summary_accepted = 1;
                    setup_hardware_summary_accepted_at = GetTickCount();
                    fprintf(stderr, "setup stage: custom hardware summary accepted\n");
                    fflush(stderr);
                    goto next_frame;
                }
                if (windows_setup_custom && setup_hardware_display_requested &&
                    !setup_hardware_v7_display_selected && frame_contains(frame,
                    "You have asked to change the type of Display")) {
                    DWORD now = GetTickCount();

                    /* Setup's 386-compatible list suppresses the three
                       "286 only" entries.  The original 512 KiB V7 core
                       must not be configured as the driver's 1 MiB model:
                       start at 8514/a and select the stock VGA entry after
                       nine complete Down transitions. */
                    if ((LONG)(now - setup_display_choice_navigation_at) < 0)
                        goto next_frame;
                    if (setup_display_choice_moves < 9u) {
                        if (!send_setup_navigation_key(runtime, VK_DOWN)) break;
                        setup_display_choice_moves++;
                        setup_display_choice_navigation_at = now + 250u;
                    } else {
                        if (!send_setup_navigation_key(runtime, VK_RETURN))
                            break;
                        setup_hardware_v7_display_selected = 1;
                    fprintf(stderr, "setup stage: VGA display selected\n");
                        fflush(stderr);
                    }
                    goto next_frame;
                }
                if (windows_setup_custom && setup_hardware_v7_display_selected &&
                    !setup_hardware_summary_accepted && frame_contains(frame,
                    "VGA")) {
                    if (!send_setup_navigation_key(runtime, VK_RETURN)) break;
                    setup_hardware_summary_accepted = 1;
                    setup_hardware_summary_accepted_at = GetTickCount();
                    fprintf(stderr, "setup stage: custom hardware summary accepted\n");
                    fflush(stderr);
                    goto next_frame;
                }
                if (windows_setup_custom && setup_hardware_summary_accepted &&
                    !setup_hardware_summary_retry_sent && frame_contains(frame,
                    "VGA") &&
                    (DWORD)(GetTickCount() - setup_hardware_summary_accepted_at) >= 3000u) {
                    /* The original INT 16 service is still waiting with an
                       empty BDA keyboard ring.  Retry the one summary key
                       through the public frontend queue so this probe can
                       distinguish a lost host transition from a guest-mode
                       startup fault. */
                    if (!send_setup_navigation_key(runtime, VK_RETURN)) break;
                    setup_hardware_summary_retry_sent = 1;
                    fprintf(stderr, "setup stage: custom hardware summary Enter retried\n");
                    fflush(stderr);
                    goto next_frame;
                }
                /* SETUP /i is the stock Windows 3.1 route that suppresses
                   automatic hardware detection.  It first asks for the
                   target directory, then exposes the real hardware list so
                   an original InPort bus mouse can be selected without
                   inventing a PS/2 controller. */
                if (!setup_directory_accepted &&
                    frame_contains(frame, "If this is where you want to set up")) {
                    if (!enqueue_virtual_key_pair(runtime, VK_RETURN))
                        break;
                    setup_enter_virtual_key = VK_RETURN;
                    setup_enter_make_at = GetTickCount();
                    setup_directory_enter_at = setup_enter_make_at;
                    setup_enter_release_sent = 1;
                    setup_directory_accepted = 1;
                    fprintf(stderr, "setup stage: directory accepted\n");
                    fflush(stderr);
                    goto next_frame;
                }
                if (setup_directory_accepted && !setup_directory_retry_sent &&
                    frame_contains(frame, "If this is where you want to set up") &&
                    (DWORD)(GetTickCount() - setup_directory_enter_at) >= 1000u) {
                    /* This DOS-portion dialog samples the keyboard on a
                       slower turn than the opening screen.  Retry once via
                       the same public queue, with a complete make/break, to
                       distinguish a frontend timing defect from a guest
                       configuration failure. */
                    send_enter(runtime);
                    setup_directory_retry_sent = 1;
                    fprintf(stderr, "setup stage: directory Enter retried\n");
                    fflush(stderr);
                    goto next_frame;
                }
                if (setup_enter_stage == 1 && setup_enter_release_sent &&
                    frame_contains(frame, "To use Express Setup, press ENTER.")) {
                    setup_enter_virtual_key = windows_setup_custom ? (WORD)'C' : VK_RETURN;
                    if (!enqueue_virtual_key_pair(runtime, setup_enter_virtual_key))
                        break;
                    setup_enter_make_at = GetTickCount();
                    setup_enter_stage = 2;
                    fprintf(stderr, "setup stage: %s accepted\n",
                        windows_setup_custom ? "custom" : "express");
                    fflush(stderr);
                    setup_enter_release_sent = 1;
                    goto next_frame;
                }
                if (setup_enter_stage == 2 && setup_enter_release_sent &&
                    !setup_failed_install_declined && frame_contains(frame,
                    "Setup has detected a failed Microsoft Windows 3.1 installation")) {
                    /* A disposable test disk may contain files copied before
                       an intentionally interrupted prior probe.  This is a
                       Setup recovery prompt, not a guest failure: select its
                       documented N=New Setup branch so the graphics-handoff
                       regression remains repeatable on that same disk. */
                    if (!enqueue_virtual_key_pair(runtime, (WORD)'N'))
                        break;
                    setup_enter_make_at = GetTickCount();
                    setup_failed_install_declined = 1;
                    fprintf(stderr, "setup stage: failed install declined\n");
                    fflush(stderr);
                    goto next_frame;
                }
                if (setup_enter_stage == 2 && setup_enter_release_sent &&
                    frame_contains(frame, "To upgrade, press ENTER.")) {
                    if (!enqueue_virtual_key_pair(runtime, VK_RETURN))
                        break;
                    setup_enter_make_at = GetTickCount();
                    setup_enter_stage = 3;
                    fprintf(stderr, "setup stage: upgrade accepted\n");
                    fflush(stderr);
                    setup_enter_release_sent = 1;
                    goto next_frame;
                }
                if (setup_enter_stage == 3 && setup_enter_release_sent &&
                    frame_contains(frame, "To have Setup perform an upgrade")) {
                    if (!enqueue_virtual_key_pair(runtime, VK_RETURN))
                        break;
                    setup_enter_make_at = GetTickCount();
                    setup_enter_stage = 4;
                    fprintf(stderr, "setup stage: graphical handoff requested\n");
                    fflush(stderr);
                    setup_enter_release_sent = 1;
                    goto next_frame;
                }
            }
            if (frame->graphics != 0u) goto next_frame;
            if (!date_sent && frame_contains(frame, "Enter new date")) {
                send_enter(runtime); date_sent = 1;
            }
            if (!time_sent && frame_contains(frame, "Enter new time")) {
                send_enter(runtime); time_sent = 1;
            }
            if (windows_setup && !setup_command_sent) {
                if (!frame_has_prompt(frame)) {
                    setup_prompt_visible_at = 0u;
                } else if (setup_prompt_visible_at == 0u) {
                    /* COMMAND.COM can paint its first prompt before its
                       post-boot controller work has retired.  A human sees
                       that prompt and naturally waits; do the same before
                       testing the user-verified `win` launch path. */
                    setup_prompt_visible_at = GetTickCount();
                } else if ((DWORD)(GetTickCount() - setup_prompt_visible_at)
                    >= 1500u) {
                    if (!send_windows_setup_command(runtime))
                        fprintf(stderr, "softpc-runtime-boot-smoke: Setup command enqueue failed (state=%d)\n",
                            (int)softpc_runtime_get_state(runtime));
                    else setup_command_sent = 1;
                }
                if (setup_command_sent) {
                    if (guest_command_expects_graphics) {
                        /* WIN.COM, unlike SETUP.EXE, has no textual handoff
                           page.  Its graphical frame is nevertheless the
                           same guest-owned V7/VMD/MOUSE.DRV integration
                           boundary that this test needs to observe. */
                        setup_enter_stage = 4;
                        setup_handoff_at = GetTickCount();
                    }
                    fprintf(stderr, "setup stage: command entered\n");
                    fflush(stderr);
                }
            } else if (!windows_setup && input_stage == 0 && frame_has_prompt(frame)) {
                /* The frontend receives distinct Windows key messages.  Feed
                   the original controller the same way: one complete key at
                   a time, then observe its guest-visible result. */
                if (!send_key(runtime, 31u))
                    fprintf(stderr, "softpc-runtime-boot-smoke: input enqueue failed (state=%d)\n",
                        (int)softpc_runtime_get_state(runtime));
                input_stage = 1;
            }
            if (!windows_setup && input_stage == 1 && frame_contains(frame, ">a")) {
                if (!send_key(runtime, 50u))
                    fprintf(stderr, "softpc-runtime-boot-smoke: input enqueue failed (state=%d)\n",
                        (int)softpc_runtime_get_state(runtime));
                input_stage = 2;
            }
            if (!windows_setup && input_stage == 2 && frame_contains(frame, ">ab")) {
                if (!send_key(runtime, 48u))
                    fprintf(stderr, "softpc-runtime-boot-smoke: input enqueue failed (state=%d)\n",
                        (int)softpc_runtime_get_state(runtime));
                input_stage = 3;
            }
            if (!windows_setup && input_stage == 3 && frame_contains(frame, ">abc")) {
                success = 1;
                break;
            }
        }
next_frame:
        if (softpc_runtime_get_state(runtime) == SOFTPC_RUNTIME_ERROR) break;
        if (softpc_runtime_get_state(runtime) == SOFTPC_RUNTIME_STOPPED &&
            setup_command_sent) {
            fprintf(stderr,
                "softpc-runtime-boot-smoke: executor stopped during Windows Setup (result=%d)\n",
                (int)softpc_runtime_get_result(runtime));
            break;
        }
        Sleep(10u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
done:
    if (runtime != NULL) {
        final_state = (int)softpc_runtime_get_state(runtime);
        final_machine_result = (int)softpc_runtime_get_result(runtime);
        /* Freeze the one executor before reading CCPU registers or guest
           physical bytes.  The public address getter is deliberately lock
           free, so taking this snapshot while the loader is executing can
           pair a later EIP with an earlier instruction byte sequence. */
        (void)softpc_runtime_stop(runtime);
    }
    if (machine != NULL) {
        (void)softpc_machine_instruction_pointer(machine, &final_cs, &final_eip);
        (void)softpc_machine_instruction_address(machine, &final_address);
    }
    if (windows_setup_complete) {
        dump_machine_palette(machine);
        dump_palette_history();
    }
    if (!success) {
        dump_frame(frame);
        dump_current_instruction(machine);
        dump_current_code_target(machine, final_address, final_eip);
        fprintf(stderr,
            "softpc-runtime-boot-smoke: A20 gate=%d wrapping=%d\n",
            gate_a20_status, sas_twenty_bit_wrapping_enabled());
        fflush(stderr);
    }
    free(frame);
    if (runtime != NULL) {
        softpc_runtime_destroy(runtime);
    }
    softpc_machine_destroy(machine);
    if (!success) {
        fprintf(stderr, "softpc-runtime-boot-smoke: %s not reached (state=%d)\n",
            windows_setup_complete ? "Windows Setup sustained graphics" :
            (windows_setup ? "Windows Setup graphics after Welcome" : "prompt"),
            final_state);
        fprintf(stderr, "softpc-runtime-boot-smoke: machine result=%d at %04x:%08x (%05x)\n",
            final_machine_result, (unsigned int)final_cs,
            (unsigned int)final_eip, (unsigned int)final_address);
    }
    fprintf(stderr, "softpc-runtime-boot-smoke: completed success=%d setup_stage=%d custom=%d\n",
        success, setup_enter_stage, windows_setup_custom);
    return success ? 0 : 1;
}
#else
int main(void) { return 0; }
#endif
