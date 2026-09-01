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
                text[2] == '>') return 1;
        }
    }
    return 0;
}

static void send_enter(softpc_runtime *runtime)
{
    (void)softpc_runtime_enqueue_key(runtime, 0x1cu, 0u);
    (void)softpc_runtime_enqueue_key(runtime, 0x1cu, 1u);
}

int main(int argc, char **argv)
{
    softpc_machine_options options = { NULL, NULL, SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    softpc_runtime *runtime = NULL;
    softpc_runtime_frame *frame;
    DWORD deadline;
    int date_sent = 0, time_sent = 0, success = 0;
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
    deadline = GetTickCount() + 60000u;
    do {
        if (softpc_runtime_copy_frame(runtime, frame) && frame->graphics == 0u) {
            if (!date_sent && frame_contains(frame, "Enter new date")) {
                send_enter(runtime); date_sent = 1;
            }
            if (!time_sent && frame_contains(frame, "Enter new time")) {
                send_enter(runtime); time_sent = 1;
            }
            if (frame_has_prompt(frame)) { success = 1; break; }
        }
        if (softpc_runtime_get_state(runtime) == SOFTPC_RUNTIME_ERROR) break;
        Sleep(10u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
    free(frame);
done:
    if (runtime != NULL) { (void)softpc_runtime_stop(runtime); softpc_runtime_destroy(runtime); }
    softpc_machine_destroy(machine);
    if (!success)
        fprintf(stderr, "softpc-runtime-boot-smoke: prompt not reached\n");
    return success ? 0 : 1;
}
#else
int main(void) { return 0; }
#endif
