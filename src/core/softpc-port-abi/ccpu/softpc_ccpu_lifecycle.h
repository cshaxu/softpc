#ifndef SOFTPC_CCPU_LIFECYCLE_H
#define SOFTPC_CCPU_LIFECYCLE_H

/* This port-ABI contract records only original c_cpu_simulate frame nesting.
   It neither changes CCPU control flow nor interprets BOP/device behavior. */
void softpc_ccpu_lifecycle_enter(void);
void softpc_ccpu_lifecycle_leave(void);
void softpc_ccpu_lifecycle_request_exit(void);
void softpc_ccpu_lifecycle_clear_exit(void);
int softpc_ccpu_lifecycle_outer_exit_requested(void);

#endif
