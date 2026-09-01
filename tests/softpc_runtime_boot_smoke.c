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

int main(int argc, char **argv)
{
    softpc_machine_options options = { NULL, NULL, SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    softpc_runtime *runtime = NULL;
    softpc_runtime_frame *frame = NULL;
    DWORD deadline;
    int date_sent = 0, time_sent = 0, input_stage = 0, success = 0;
    int final_state = SOFTPC_RUNTIME_ERROR;
    if (argc != 5 || strcmp(argv[1], "--floppy") != 0 ||
        strcmp(argv[3], "--hdd") != 0) return 2;
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
    deadline = GetTickCount() + 20000u;
    do {
        if (softpc_runtime_copy_frame(runtime, frame) && frame->graphics == 0u) {
            if (!date_sent && frame_contains(frame, "Enter new date")) {
                send_enter(runtime); date_sent = 1;
            }
            if (!time_sent && frame_contains(frame, "Enter new time")) {
                send_enter(runtime); time_sent = 1;
            }
            if (input_stage == 0 && frame_has_prompt(frame)) {
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
    if (runtime != NULL) { (void)softpc_runtime_stop(runtime); softpc_runtime_destroy(runtime); }
    softpc_machine_destroy(machine);
    if (!success)
        fprintf(stderr, "softpc-runtime-boot-smoke: prompt not reached (state=%d)\n",
            final_state);
    return success ? 0 : 1;
}
#else
int main(void) { return 0; }
#endif
