#ifndef x86_debug_INTERFACE_H
#define x86_debug_INTERFACE_H

#include "lib/types/types_interface.h"
#include "emulator/machine/machine_interface.h"
#include "x86/debug/protocol_interface.h"

typedef struct X86_DEBUG X86_DEBUG;


#define x86_debug_LINE_CAPACITY 256u
#define x86_debug_PROMPT_CAPACITY 64u

typedef enum x86_debug_lifecycle_request {
    x86_debug_LIFECYCLE_NONE,
    x86_debug_LIFECYCLE_RESUME,
    x86_debug_LIFECYCLE_STEP,
    x86_debug_LIFECYCLE_STOP
} x86_debug_lifecycle_request;

typedef struct x86_debug_result {
    /* Borrowed, NUL-terminated output without a terminal line ending. It is valid
     * until the next submit/observe, open, or destroy on this debug object. Copy before retaining longer.
     * Output grows as needed; allocation/format failure returns lib_status. */
    const char *text;
    char prompt[x86_debug_PROMPT_CAPACITY];
    lib_bool prompt_ready;
    lib_bool keep_active;
    x86_debug_lifecycle_request lifecycle_request;
} x86_debug_result;

typedef enum x86_debug_machine_state {
    x86_debug_MACHINE_RUNNING,
    x86_debug_MACHINE_PAUSED,
    x86_debug_MACHINE_RESET,
    x86_debug_MACHINE_STOPPED,
    x86_debug_MACHINE_FAULT
} x86_debug_machine_state;

lib_status x86_debug_create(X86_DEBUG **out_debug);
void x86_debug_destroy(X86_DEBUG *debug);
lib_status x86_debug_open(X86_DEBUG *debug, emulator_machine *machine);
void x86_debug_close(X86_DEBUG *debug);
lib_status x86_debug_submit_line(X86_DEBUG *debug, const char *line,
    x86_debug_result *out_result);
lib_status x86_debug_observe_machine(X86_DEBUG *debug,
    x86_debug_machine_state state, lib_status status,
    x86_debug_result *out_result);

#endif
