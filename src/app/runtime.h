#ifndef SOFTPC_VM_RUNTIME_H
#define SOFTPC_VM_RUNTIME_H

#include "machine.h"

#include <stdint.h>

#define SOFTPC_RUNTIME_TEXT_COLUMNS 80u
#define SOFTPC_RUNTIME_TEXT_ROWS 25u
#define SOFTPC_RUNTIME_DIB_MAX_WIDTH 1280u
#define SOFTPC_RUNTIME_DIB_MAX_HEIGHT 768u
#define SOFTPC_RUNTIME_DIB_MAX_BYTES \
    (SOFTPC_RUNTIME_DIB_MAX_WIDTH * SOFTPC_RUNTIME_DIB_MAX_HEIGHT)
#define SOFTPC_RUNTIME_DIB_INFO_BYTES 1064u
#define SOFTPC_RUNTIME_PATH_MAX 1024u

typedef struct app_runtime app_runtime;

typedef enum app_runtime_state {
    SOFTPC_RUNTIME_STOPPED,
    SOFTPC_RUNTIME_STARTING,
    SOFTPC_RUNTIME_RUNNING,
    SOFTPC_RUNTIME_PAUSED,
    SOFTPC_RUNTIME_ERROR
} app_runtime_state;

typedef struct app_runtime_frame {
    uint32_t sequence;
    uint32_t graphics;
    uint32_t valid;
    int32_t cursor_column;
    int32_t cursor_row;
    uint32_t cursor_size;
    uint8_t text[SOFTPC_RUNTIME_TEXT_COLUMNS * SOFTPC_RUNTIME_TEXT_ROWS];
    uint16_t attributes[SOFTPC_RUNTIME_TEXT_COLUMNS * SOFTPC_RUNTIME_TEXT_ROWS];
    /* Copied RGB values for the original renderer's active VGA text palette.
       Text attributes are palette indices, not fixed host colours. */
    uint32_t text_palette[16u];
    uint8_t font[256u * 16u];
    uint8_t secondary_font[256u * 16u];
    uint32_t font_height;
    uint32_t attribute_font_select;
    uint32_t dib_width;
    uint32_t dib_height;
    /* Original nt_graph dirty rectangle, clipped to the copied DIB. */
    int32_t dirty_left;
    int32_t dirty_top;
    int32_t dirty_right;
    int32_t dirty_bottom;
    uint8_t dib_info[SOFTPC_RUNTIME_DIB_INFO_BYTES];
    uint8_t dib_bits[SOFTPC_RUNTIME_DIB_MAX_BYTES];
} app_runtime_frame;

int app_runtime_create(softpc_machine *machine, app_runtime **out);
int app_runtime_start(app_runtime *runtime);
int app_runtime_pause(app_runtime *runtime);
int app_runtime_resume(app_runtime *runtime);
int app_runtime_stop(app_runtime *runtime);
int app_runtime_set_floppy(app_runtime *runtime, const char *path);
app_runtime_state app_runtime_get_state(const app_runtime *runtime);
softpc_machine_result app_runtime_get_result(const app_runtime *runtime);
int app_runtime_enqueue_key(app_runtime *runtime, uint8_t key_number,
    uint8_t released);
int app_runtime_enqueue_mouse(app_runtime *runtime, int32_t delta_x,
    int32_t delta_y, uint8_t left_down, uint8_t right_down);
int app_runtime_copy_frame(app_runtime *runtime,
    app_runtime_frame *destination);
/* A presentation client may cheaply test whether the runtime's copied frame
   changed before requesting the potentially large DIB snapshot. */
uint32_t app_runtime_published_frame_sequence(const app_runtime *runtime);
void app_runtime_destroy(app_runtime *runtime);

#endif
