#ifndef COMMON_MACHINE_INTERFACE_H
#define COMMON_MACHINE_INTERFACE_H

#include "lib/kvm-base/event_interface.h"
#include "lib/kvm-base/frame_interface.h"
#include "lib/types/types_interface.h"

#include <stdint.h>

#define COMMON_MACHINE_PATH_CAPACITY 1024u

typedef struct common_machine common_machine;
typedef void (*common_machine_executor_callback)(void *context);

typedef enum common_machine_state {
    COMMON_MACHINE_STOPPED,
    COMMON_MACHINE_STARTING,
    COMMON_MACHINE_RUNNING,
    COMMON_MACHINE_PAUSED,
    COMMON_MACHINE_ERROR,
    COMMON_MACHINE_RESET_COMPLETED
} common_machine_state;

/* State transitions and completed frame publication are independent facts.
 * Keeping their callbacks separate prevents an executor paint callback from
 * being mistaken for a lifecycle completion by the product control queue. */
typedef void (*common_machine_state_sink)(void *context,
    common_machine_state state, lib_u32 run_generation);
typedef void (*common_machine_frame_sink)(void *context,
    lib_u32 frame_sequence, lib_bool graphics, lib_u32 run_generation);

typedef struct common_machine_driver {
    void *context;
    lib_bool (*reset)(void *context);
    lib_bool (*run)(void *context);
    void (*request_stop)(void *context);
    void (*request_wake)(void *context);
    void (*set_heartbeat)(void *context, lib_bool enabled);
    void (*set_executor_callback)(void *context,
        common_machine_executor_callback callback, void *callback_context);
    void (*deliver_input)(void *context, const kvm_input_event *event);
    lib_bool (*copy_frame)(void *context, kvm_frame *out_frame);
    lib_bool (*set_removable_media)(void *context, const char *path);
    /* Optional product-owned observation after a complete frame has been
     * published. It must not call machine lifecycle APIs. */
    void (*frame_published)(void *context, const kvm_frame *frame);
} common_machine_driver;

lib_status common_machine_create(common_machine **out_machine,
    const common_machine_driver *driver);
void common_machine_set_state_sink(common_machine *machine,
    common_machine_state_sink sink, void *context);
void common_machine_set_frame_sink(common_machine *machine,
    common_machine_frame_sink sink, void *context);
lib_bool common_machine_start(common_machine *machine);
lib_bool common_machine_pause(common_machine *machine);
lib_bool common_machine_resume(common_machine *machine);
lib_bool common_machine_stop(common_machine *machine);
lib_bool common_machine_reset(common_machine *machine);
lib_bool common_machine_set_removable_media(common_machine *machine,
    const char *path);
common_machine_state common_machine_state_get(const common_machine *machine);
lib_bool common_machine_enqueue_input(common_machine *machine,
    const kvm_input_event *event);
lib_bool common_machine_copy_published_frame(common_machine *machine,
    kvm_frame *destination, lib_u32 *out_run_generation);
lib_u32 common_machine_published_frame_sequence(const common_machine *machine);
lib_u32 common_machine_published_frame_run_generation(const common_machine *machine);
lib_u32 common_machine_run_generation(const common_machine *machine);
void common_machine_destroy(common_machine *machine);

#endif
