#ifndef TEST_RUNTIME_COMPATIBILITY_H
#define TEST_RUNTIME_COMPATIBILITY_H

#include "common_machine_test.h"
#include <stdlib.h>

typedef common_machine_test app_runtime;
typedef common_machine_state app_runtime_state;
typedef kvm_frame app_runtime_frame;
typedef common_machine_state_sink app_runtime_state_sink;
typedef common_machine_frame_sink app_runtime_frame_sink;

#define SOFTPC_RUNTIME_STOPPED COMMON_MACHINE_STOPPED
#define SOFTPC_RUNTIME_STARTING COMMON_MACHINE_STARTING
#define SOFTPC_RUNTIME_RUNNING COMMON_MACHINE_RUNNING
#define SOFTPC_RUNTIME_PAUSED COMMON_MACHINE_PAUSED
#define SOFTPC_RUNTIME_ERROR COMMON_MACHINE_ERROR
#define SOFTPC_RUNTIME_RESET_COMPLETED COMMON_MACHINE_RESET_COMPLETED
#define SOFTPC_RUNTIME_TEXT_COLUMNS KVM_TEXT_COLUMNS
#define SOFTPC_RUNTIME_TEXT_ROWS KVM_TEXT_ROWS

static inline int app_runtime_create(softpc_machine *machine, app_runtime **out)
{
    app_runtime *runtime;
    if (out == NULL) return 0;
    *out = NULL;
    runtime = calloc(1u, sizeof(*runtime));
    if (runtime == NULL || !common_machine_test_create(machine, runtime)) {
        free(runtime);
        return 0;
    }
    *out = runtime;
    return 1;
}
static inline void app_runtime_destroy(app_runtime *runtime)
{
    if (runtime == NULL) return;
    common_machine_test_destroy(runtime);
    free(runtime);
}
static inline void app_runtime_set_state_sink(app_runtime *runtime,
    app_runtime_state_sink sink, void *context)
{ if (runtime != NULL) common_machine_set_state_sink(runtime->machine, sink, context); }
static inline void app_runtime_set_frame_sink(app_runtime *runtime,
    app_runtime_frame_sink sink, void *context)
{ if (runtime != NULL) common_machine_set_frame_sink(runtime->machine, sink, context); }
static inline int app_runtime_start(app_runtime *runtime)
{ return runtime != NULL && common_machine_start(runtime->machine); }
static inline int app_runtime_pause(app_runtime *runtime)
{ return runtime != NULL && common_machine_pause(runtime->machine); }
static inline int app_runtime_resume(app_runtime *runtime)
{ return runtime != NULL && common_machine_resume(runtime->machine); }
static inline int app_runtime_stop(app_runtime *runtime)
{ return runtime != NULL && common_machine_stop(runtime->machine); }
static inline int app_runtime_reset(app_runtime *runtime)
{ return runtime != NULL && common_machine_reset(runtime->machine); }
static inline int app_runtime_set_floppy(app_runtime *runtime, const char *path)
{ return runtime != NULL && common_machine_set_removable_media(runtime->machine,
    path, LIB_STORAGE_MEDIUM_OVERLAY); }
static inline app_runtime_state app_runtime_get_state(const app_runtime *runtime)
{ return runtime == NULL ? COMMON_MACHINE_ERROR : common_machine_state_get(runtime->machine); }
static inline int app_runtime_enqueue_input_event(app_runtime *runtime,
    const kvm_input_event *event)
{ return runtime != NULL && common_machine_enqueue_input(runtime->machine, event); }
static inline int app_runtime_copy_frame(app_runtime *runtime, app_runtime_frame *frame)
{ return runtime != NULL && common_machine_copy_published_frame(runtime->machine, frame,
    common_machine_run_generation(runtime->machine)); }
static inline int app_runtime_copy_published_frame(app_runtime *runtime,
    app_runtime_frame *frame, lib_u32 *generation)
{
    lib_u32 run;
    if (runtime == NULL) return 0;
    run = common_machine_run_generation(runtime->machine);
    if (!common_machine_copy_published_frame(runtime->machine, frame, run)) return 0;
    if (generation != NULL) *generation = run;
    return 1;
}
static inline lib_u32 app_runtime_published_frame_sequence(const app_runtime *runtime)
{ return runtime == NULL ? 0u : common_machine_published_frame_sequence(runtime->machine); }
static inline lib_u32 app_runtime_published_frame_run_generation(const app_runtime *runtime)
{ return runtime == NULL ? 0u : common_machine_published_frame_run_generation(runtime->machine); }
static inline lib_u32 app_runtime_run_generation(const app_runtime *runtime)
{ return runtime == NULL ? 0u : common_machine_run_generation(runtime->machine); }

#endif
