#ifndef COMMON_X86_DEBUG_INTERFACE_H
#define COMMON_X86_DEBUG_INTERFACE_H

#include "lib/types/types_interface.h"
#include "common/machine/machine_interface.h"
#include "common/x86-debug/protocol_interface.h"

typedef struct common_x86_debug common_x86_debug;


#define COMMON_X86_DEBUG_LINE_CAPACITY 256u
#define COMMON_X86_DEBUG_PROMPT_CAPACITY 64u

typedef enum common_x86_debug_lifecycle_request {
    COMMON_X86_DEBUG_LIFECYCLE_NONE,
    COMMON_X86_DEBUG_LIFECYCLE_RESUME,
    COMMON_X86_DEBUG_LIFECYCLE_STEP,
    COMMON_X86_DEBUG_LIFECYCLE_STOP
} common_x86_debug_lifecycle_request;

typedef struct common_x86_debug_result {
    /* Borrowed, NUL-terminated output, valid until the next submit/observe,
     * open or destroy on this debug object. Copy before retaining longer.
     * Output grows as needed; allocation/format failure returns lib_status. */
    const char *text;
    char prompt[COMMON_X86_DEBUG_PROMPT_CAPACITY];
    lib_bool prompt_ready;
    lib_bool keep_active;
    common_x86_debug_lifecycle_request lifecycle_request;
} common_x86_debug_result;

typedef enum common_x86_debug_machine_state {
    COMMON_X86_DEBUG_MACHINE_RUNNING,
    COMMON_X86_DEBUG_MACHINE_PAUSED,
    COMMON_X86_DEBUG_MACHINE_RESET,
    COMMON_X86_DEBUG_MACHINE_STOPPED,
    COMMON_X86_DEBUG_MACHINE_FAULT
} common_x86_debug_machine_state;

lib_status common_x86_debug_create(common_x86_debug **out_debug);
void common_x86_debug_destroy(common_x86_debug *debug);
lib_status common_x86_debug_open(common_x86_debug *debug, common_machine *machine);
void common_x86_debug_close(common_x86_debug *debug);
lib_status common_x86_debug_submit_line(common_x86_debug *debug, const char *line,
    common_x86_debug_result *out_result);
lib_status common_x86_debug_observe_machine(common_x86_debug *debug,
    common_x86_debug_machine_state state, lib_status status,
    common_x86_debug_result *out_result);

#endif
