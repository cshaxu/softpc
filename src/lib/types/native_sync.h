#ifndef LIB_TYPES_NATIVE_SYNC_H
#define LIB_TYPES_NATIVE_SYNC_H

#include "lib/types/types_interface.h"

typedef struct lib_native_event lib_native_event;
typedef struct lib_native_task lib_native_task;
typedef void (*lib_native_task_entry)(void *context);

/* Raw event/thread calls shared by host and ui-base. They report only OS-call
 * status plus copied facts; neither wait policy nor mailbox meaning lives here. */
void lib_native_sleep_milliseconds(lib_u32 milliseconds);
void lib_native_yield(void);
lib_status lib_native_event_create(lib_bool manual_reset,
    lib_native_event **out_event);
void lib_native_event_destroy(lib_native_event *event);
void lib_native_event_signal(lib_native_event *event);
void lib_native_event_reset(lib_native_event *event);
lib_status lib_native_event_wait(const lib_native_event *event,
    lib_u32 timeout_milliseconds, lib_bool *out_signaled);
lib_status lib_native_event_wait_many(const lib_native_event *const *events,
    lib_u32 event_count, lib_u32 timeout_milliseconds,
    lib_bool *out_signaled, lib_u32 *out_event_index);
lib_status lib_native_event_wait_messages(const lib_native_event *event,
    lib_u32 timeout_milliseconds, lib_bool *out_wake, lib_bool *out_message);
lib_status lib_native_task_create(lib_native_task_entry entry, void *context,
    lib_native_task **out_task);
void lib_native_task_join(lib_native_task *task);
void lib_native_task_destroy(lib_native_task *task);

#endif
