#include "runtime.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>

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

static int frame_has_prompt(const softpc_runtime_frame *frame)
{
    unsigned int row;
    for (row = 0u; row < SOFTPC_RUNTIME_TEXT_ROWS; ++row) {
        unsigned int column;
        for (column = 0u; column + 2u < SOFTPC_RUNTIME_TEXT_COLUMNS; ++column) {
            const uint8_t *text = &frame->text[row * SOFTPC_RUNTIME_TEXT_COLUMNS + column];
            if (((text[0] >= 'A' && text[0] <= 'Z') ||
                 (text[0] >= 'a' && text[0] <= 'z')) && text[1] == ':' &&
                (text[2] == '>' || (column + 3u < SOFTPC_RUNTIME_TEXT_COLUMNS &&
                text[2] == '\\' && text[3] == '>'))) return 1;
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
            if (colour->rgbRed != 0u || colour->rgbGreen != 0u ||
                colour->rgbBlue != 0u) return 1;
        }
    }
    return 0;
}

static void send_enter(softpc_runtime *runtime)
{
    (void)softpc_runtime_enqueue_key(runtime, 0x1cu, 0u);
    (void)softpc_runtime_enqueue_key(runtime, 0x1cu, 1u);
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

    ZeroMemory(&event, sizeof(event));
    event.bKeyDown = released == 0u;
    event.wVirtualKeyCode = virtual_key;
    event.wVirtualScanCode = (WORD)(MapVirtualKeyW(virtual_key,
        MAPVK_VK_TO_VSC) & 0xffu);
    event.dwControlKeyState = control_state;
    key_number = KeyMsgToKeyCode(&event);
    return key_number != 0u && softpc_runtime_enqueue_key(runtime, key_number,
        released);
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
    Sleep(100u);
    if (!enqueue_virtual_key(runtime, virtual_key, 0u, 0u)) return 0;
    Sleep(100u);
    if (!enqueue_virtual_key(runtime, virtual_key, 0u, 1u)) return 0;
    Sleep(100u);
    if ((modifiers & 1u) != 0u && !enqueue_virtual_key(runtime, VK_SHIFT,
        0u, 1u)) return 0;
    return 1;
}

static int send_windows_setup_command(softpc_runtime *runtime)
{
    static const WCHAR command[] = L"c:\\ewin31\\setup.exe";
    unsigned int index;

    for (index = 0u; command[index] != L'\0'; ++index)
        if (!send_character_slow(runtime, command[index])) return 0;
    if (!enqueue_virtual_key(runtime, VK_RETURN, 0u, 0u)) return 0;
    Sleep(100u);
    if (!enqueue_virtual_key(runtime, VK_RETURN, 0u, 1u)) return 0;
    return 1;
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
    int overlay = 0, windows_setup = 0, setup_command_sent = 0;
    int setup_enter_stage = 0;
    int setup_enter_release_sent = 0;
    DWORD setup_enter_make_at = 0u;
    int final_state = SOFTPC_RUNTIME_ERROR;
    int argument;
    if (!run_halted_keyboard_probe()) {
        fprintf(stderr, "softpc-runtime-boot-smoke: halted keyboard probe failed\n");
        return 1;
    }
    if (argc < 5 || strcmp(argv[1], "--floppy") != 0 ||
        strcmp(argv[3], "--hdd") != 0) return 2;
    for (argument = 5; argument < argc; ++argument) {
        if (strcmp(argv[argument], "--overlay") == 0 && !overlay) {
            overlay = 1;
        } else if (strcmp(argv[argument], "--windows-setup") == 0 && !windows_setup) {
            windows_setup = 1;
        } else return 2;
    }
    /* This probe is intentionally media-safe: unlike the launcher it has no
       direct-media option, and its real image runs require explicit overlay. */
    if (!overlay) return 2;
    options.floppy_path = argv[2];
    options.hard_disk_path = argv[4];
    options.media_mode = SOFTPC_MEDIA_OVERLAY;
    if (softpc_machine_create(&options, &machine) != SOFTPC_MACHINE_OK ||
        !softpc_runtime_create(machine, &runtime) || !softpc_runtime_start(runtime))
        goto done;
    frame = (softpc_runtime_frame *)calloc(1u, sizeof(*frame));
    if (frame == NULL) goto done;
    /* This is an integration probe, not an unattended boot soak.  Leave
       enough room for firmware POST but emit its captured frame before any
       shutdown work if the guest does not reach the prompt. */
    deadline = GetTickCount() + (windows_setup ? 60000u : 20000u);
    do {
        if (windows_setup && setup_enter_stage != 0 &&
            !setup_enter_release_sent &&
            (DWORD)(GetTickCount() - setup_enter_make_at) >= 100u) {
            if (!enqueue_virtual_key(runtime, VK_RETURN, 0u, 1u)) break;
            setup_enter_release_sent = 1;
        }
        if (softpc_runtime_copy_frame(runtime, frame)) {
            if (windows_setup && setup_command_sent) {
                if (frame->graphics != 0u) {
                    if (setup_enter_stage >= 4 &&
                        graphics_frame_has_visible_pixel(frame)) {
                        success = 1;
                        break;
                    }
                    goto next_frame;
                }
                if (frame_contains(frame, "Welcome to Setup.")) {
                    if (setup_enter_stage == 0) {
                        /* Model the separate make/break messages received by
                           the console/window. Feeding both into one 8042
                           service turn can collapse this real Setup key. */
                        if (!enqueue_virtual_key(runtime, VK_RETURN, 0u, 0u))
                            break;
                        setup_enter_make_at = GetTickCount();
                        setup_enter_stage = 1;
                        setup_enter_release_sent = 0;
                    }
                    goto next_frame;
                }
                if (setup_enter_stage == 1 && setup_enter_release_sent &&
                    frame_contains(frame, "To use Express Setup, press ENTER.")) {
                    if (!enqueue_virtual_key(runtime, VK_RETURN, 0u, 0u))
                        break;
                    setup_enter_make_at = GetTickCount();
                    setup_enter_stage = 2;
                    setup_enter_release_sent = 0;
                    goto next_frame;
                }
                if (setup_enter_stage == 2 && setup_enter_release_sent &&
                    frame_contains(frame, "To upgrade, press ENTER.")) {
                    if (!enqueue_virtual_key(runtime, VK_RETURN, 0u, 0u))
                        break;
                    setup_enter_make_at = GetTickCount();
                    setup_enter_stage = 3;
                    setup_enter_release_sent = 0;
                    goto next_frame;
                }
                if (setup_enter_stage == 3 && setup_enter_release_sent &&
                    frame_contains(frame, "To have Setup perform an upgrade")) {
                    if (!enqueue_virtual_key(runtime, VK_RETURN, 0u, 0u))
                        break;
                    setup_enter_make_at = GetTickCount();
                    setup_enter_stage = 4;
                    setup_enter_release_sent = 0;
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
            if (windows_setup && !setup_command_sent && frame_has_prompt(frame)) {
                if (!send_windows_setup_command(runtime))
                    fprintf(stderr, "softpc-runtime-boot-smoke: Setup command enqueue failed (state=%d)\n",
                        (int)softpc_runtime_get_state(runtime));
                else setup_command_sent = 1;
            } else if (input_stage == 0 && frame_has_prompt(frame)) {
                /* The frontend receives distinct Windows key messages.  Feed
                   the original controller the same way: one complete key at
                   a time, then observe its guest-visible result. */
                if (!send_key(runtime, 31u))
                    fprintf(stderr, "softpc-runtime-boot-smoke: input enqueue failed (state=%d)\n",
                        (int)softpc_runtime_get_state(runtime));
                input_stage = 1;
            }
            if (input_stage == 1 && frame_contains(frame, ">a")) {
                if (!send_key(runtime, 50u))
                    fprintf(stderr, "softpc-runtime-boot-smoke: input enqueue failed (state=%d)\n",
                        (int)softpc_runtime_get_state(runtime));
                input_stage = 2;
            }
            if (input_stage == 2 && frame_contains(frame, ">ab")) {
                if (!send_key(runtime, 48u))
                    fprintf(stderr, "softpc-runtime-boot-smoke: input enqueue failed (state=%d)\n",
                        (int)softpc_runtime_get_state(runtime));
                input_stage = 3;
            }
            if (input_stage == 3 && frame_contains(frame, ">abc")) {
                success = 1;
                break;
            }
        }
next_frame:
        if (softpc_runtime_get_state(runtime) == SOFTPC_RUNTIME_ERROR) break;
        Sleep(10u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
done:
    if (runtime != NULL) final_state = (int)softpc_runtime_get_state(runtime);
    if (!success) {
        dump_frame(frame);
        fflush(stderr);
    }
    free(frame);
    if (runtime != NULL) {
        (void)softpc_runtime_stop(runtime);
        softpc_runtime_destroy(runtime);
    }
    softpc_machine_destroy(machine);
    if (!success)
        fprintf(stderr, "softpc-runtime-boot-smoke: %s not reached (state=%d)\n",
            windows_setup ? "Windows Setup graphics after Welcome" : "prompt",
            final_state);
    return success ? 0 : 1;
}
#else
int main(void) { return 0; }
#endif
