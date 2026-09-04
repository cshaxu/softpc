/*
 * CPU_40 keyba.c historically accessed CCPU's interrupt map directly. The
 * generated standalone CCPU retains that map as private storage, so expose
 * its address only to this original source file through a generated ABI
 * accessor. No host, frontend or controller receives CCPU state access.
 */
#include "../../mvdm/softpc.new/base/inc/cpu4.h"

extern IU32 *softpc_ccpu_interrupt_map_address(void);
#define cpu_interrupt_map (*softpc_ccpu_interrupt_map_address())

/* CPU_40's selected CCPU representation reserves bit 14 for reset. */
#define CPU_RESET_EXCEPTION_MASK (1u << 14)
