#ifndef SOFTPC_VM_RUNTIME_H
#define SOFTPC_VM_RUNTIME_H

#include "softpc_machine.h"

#include <stdint.h>

#define SOFTPC_RUNTIME_TEXT_COLUMNS 80u
#define SOFTPC_RUNTIME_TEXT_ROWS 25u
#define SOFTPC_RUNTIME_DIB_MAX_WIDTH 1024u
#define SOFTPC_RUNTIME_DIB_MAX_HEIGHT 768u
#define SOFTPC_RUNTIME_DIB_MAX_BYTES \
    (SOFTPC_RUNTIME_DIB_MAX_WIDTH * SOFTPC_RUNTIME_DIB_MAX_HEIGHT)
#define SOFTPC_RUNTIME_DIB_INFO_BYTES 1064u
#define SOFTPC_RUNTIME_PATH_MAX 1024u

typedef struct softpc_runtime softpc_runtime;

typedef enum softpc_runtime_state {
    SOFTPC_RUNTIME_STOPPED,
    SOFTPC_RUNTIME_STARTING,
    SOFTPC_RUNTIME_RUNNING,
    SOFTPC_RUNTIME_PAUSED,
    SOFTPC_RUNTIME_ERROR
} softpc_runtime_state;

typedef struct softpc_runtime_frame {
    uint32_t sequence;
    uint32_t graphics;
    uint32_t valid;
    int32_t cursor_column;
    int32_t cursor_row;
    uint8_t text[SOFTPC_RUNTIME_TEXT_COLUMNS * SOFTPC_RUNTIME_TEXT_ROWS];
    uint16_t attributes[SOFTPC_RUNTIME_TEXT_COLUMNS * SOFTPC_RUNTIME_TEXT_ROWS];
    uint8_t font[256u * 16u];
    uint8_t secondary_font[256u * 16u];
    uint32_t font_height;
    uint32_t attribute_font_select;
    uint32_t dib_width;
    uint32_t dib_height;
    uint8_t dib_info[SOFTPC_RUNTIME_DIB_INFO_BYTES];
    uint8_t dib_bits[SOFTPC_RUNTIME_DIB_MAX_BYTES];
} softpc_runtime_frame;

int softpc_runtime_create(softpc_machine *machine, softpc_runtime **out);
int softpc_runtime_start(softpc_runtime *runtime);
int softpc_runtime_pause(softpc_runtime *runtime);
int softpc_runtime_resume(softpc_runtime *runtime);
int softpc_runtime_stop(softpc_runtime *runtime);
int softpc_runtime_set_floppy(softpc_runtime *runtime, const char *path);
softpc_runtime_state softpc_runtime_get_state(const softpc_runtime *runtime);
softpc_machine_result softpc_runtime_get_result(const softpc_runtime *runtime);
int softpc_runtime_enqueue_key(softpc_runtime *runtime, uint8_t key_number,
    uint8_t released);
int softpc_runtime_enqueue_mouse(softpc_runtime *runtime, int32_t delta_x,
    int32_t delta_y, uint8_t left_down, uint8_t right_down);
int softpc_runtime_copy_frame(softpc_runtime *runtime,
    softpc_runtime_frame *destination);
/* A presentation client may cheaply test whether the runtime's copied frame
   changed before requesting the potentially large DIB snapshot. */
uint32_t softpc_runtime_published_frame_sequence(const softpc_runtime *runtime);
void softpc_runtime_destroy(softpc_runtime *runtime);

#endif
