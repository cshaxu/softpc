#ifndef SOFTPC_VM_RUNTIME_H
#define SOFTPC_VM_RUNTIME_H

#include "machine.h"
#include "lib/ux-base/event.h"
#include "lib/ux-base/frame.h"

#include <stdint.h>

#define SOFTPC_RUNTIME_TEXT_COLUMNS UX_TEXT_COLUMNS
#define SOFTPC_RUNTIME_TEXT_ROWS UX_TEXT_ROWS
#define SOFTPC_RUNTIME_DIB_MAX_WIDTH UX_GRAPHICS_MAX_WIDTH
#define SOFTPC_RUNTIME_DIB_MAX_HEIGHT UX_GRAPHICS_MAX_HEIGHT
#define SOFTPC_RUNTIME_DIB_MAX_BYTES UX_GRAPHICS_MAX_PIXELS
#define SOFTPC_RUNTIME_PATH_MAX 1024u

typedef struct app_runtime app_runtime;

typedef enum app_runtime_state {
    SOFTPC_RUNTIME_STOPPED,
    SOFTPC_RUNTIME_STARTING,
    SOFTPC_RUNTIME_RUNNING,
    SOFTPC_RUNTIME_PAUSED,
    SOFTPC_RUNTIME_ERROR,
    /* Completion-only fact: runtime is actually paused after it performed
     * its own atomic cold reset.  app_runtime_get_state() never returns it. */
    SOFTPC_RUNTIME_RESET_COMPLETED
} app_runtime_state;

/* State transitions and completed frame publication are independent facts.
 * Keeping their callbacks separate prevents an executor paint callback from
 * being mistaken for a lifecycle completion by the product control queue. */
typedef void (*app_runtime_state_sink)(void *context,
    app_runtime_state state, uint32_t run_generation);

typedef void (*app_runtime_frame_sink)(void *context,
    uint32_t frame_sequence, int frame_graphics, uint32_t run_generation);

typedef ux_frame app_runtime_frame;

int app_runtime_create(softpc_machine *machine, app_runtime **out);
void app_runtime_set_state_sink(app_runtime *runtime,
    app_runtime_state_sink sink, void *context);
void app_runtime_set_frame_sink(app_runtime *runtime,
    app_runtime_frame_sink sink, void *context);
int app_runtime_start(app_runtime *runtime);
int app_runtime_pause(app_runtime *runtime);
int app_runtime_resume(app_runtime *runtime);
int app_runtime_stop(app_runtime *runtime);
int app_runtime_reset(app_runtime *runtime);
int app_runtime_set_floppy(app_runtime *runtime, const char *path);
app_runtime_state app_runtime_get_state(const app_runtime *runtime);
softpc_machine_result app_runtime_get_result(const app_runtime *runtime);
int app_runtime_enqueue_input_event(app_runtime *runtime,
    const ux_event *event);
int app_runtime_copy_frame(app_runtime *runtime,
    app_runtime_frame *destination);
int app_runtime_copy_published_frame(app_runtime *runtime,
    app_runtime_frame *destination, uint32_t *out_run_generation);
/* A presentation client may cheaply test whether the runtime's copied frame
   changed before requesting the potentially large DIB snapshot. */
uint32_t app_runtime_published_frame_sequence(const app_runtime *runtime);
uint32_t app_runtime_published_frame_run_generation(const app_runtime *runtime);
uint32_t app_runtime_run_generation(const app_runtime *runtime);
void app_runtime_destroy(app_runtime *runtime);

#endif
