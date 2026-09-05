#ifndef SOFTPC_VM_RUNTIME_H
#define SOFTPC_VM_RUNTIME_H

#include "machine.h"
#include "../lib/platform/win32/frame.h"
#include "../lib/platform/win32/event.h"
#include "../lib/platform/win32/mailbox.h"

#include <stdint.h>

#define SOFTPC_RUNTIME_TEXT_COLUMNS WIN32_PRESENTATION_TEXT_COLUMNS
#define SOFTPC_RUNTIME_TEXT_ROWS WIN32_PRESENTATION_TEXT_ROWS
#define SOFTPC_RUNTIME_DIB_MAX_WIDTH WIN32_PRESENTATION_DIB_MAX_WIDTH
#define SOFTPC_RUNTIME_DIB_MAX_HEIGHT WIN32_PRESENTATION_DIB_MAX_HEIGHT
#define SOFTPC_RUNTIME_DIB_MAX_BYTES WIN32_PRESENTATION_DIB_MAX_BYTES
#define SOFTPC_RUNTIME_DIB_INFO_BYTES WIN32_PRESENTATION_DIB_INFO_BYTES
#define SOFTPC_RUNTIME_PATH_MAX 1024u

typedef struct app_runtime app_runtime;

typedef enum app_runtime_state {
    SOFTPC_RUNTIME_STOPPED,
    SOFTPC_RUNTIME_STARTING,
    SOFTPC_RUNTIME_RUNNING,
    SOFTPC_RUNTIME_PAUSED,
    SOFTPC_RUNTIME_ERROR
} app_runtime_state;

typedef win32_presentation_frame app_runtime_frame;

int app_runtime_create(softpc_machine *machine, app_runtime **out);
int app_runtime_start(app_runtime *runtime);
int app_runtime_pause(app_runtime *runtime);
int app_runtime_resume(app_runtime *runtime);
int app_runtime_stop(app_runtime *runtime);
int app_runtime_set_floppy(app_runtime *runtime, const char *path);
app_runtime_state app_runtime_get_state(const app_runtime *runtime);
softpc_machine_result app_runtime_get_result(const app_runtime *runtime);
int app_runtime_enqueue_input_event(app_runtime *runtime,
    const win32_presentation_event *event);
int app_runtime_copy_frame(app_runtime *runtime,
    app_runtime_frame *destination);
/* A presentation client may cheaply test whether the runtime's copied frame
   changed before requesting the potentially large DIB snapshot. */
uint32_t app_runtime_published_frame_sequence(const app_runtime *runtime);
/* Auto-reset Win32 publication event for the sole presentation client.  The
   copied frame remains the ownership boundary; this only avoids waking that
   client when no new snapshot exists. */
void *app_runtime_frame_event(const app_runtime *runtime);
/* Opaque shared-presentation mailbox.  The app binding may pass this handle
   to a Win32 presenter; it never exposes machine-owned video state. */
win32_presentation_mailbox *app_runtime_presentation_mailbox(
    app_runtime *runtime);
void app_runtime_destroy(app_runtime *runtime);

#endif
