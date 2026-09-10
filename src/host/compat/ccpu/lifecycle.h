#ifndef SOFTPC_CCPU_LIFECYCLE_H
#define SOFTPC_CCPU_LIFECYCLE_H

/* This standalone port-ABI boundary owns lifecycle-only access to the
   original CCPU. It does not interpret BOP/device behavior or express
   product lifecycle policy. */
void softpc_ccpu_lifecycle_enter(void);
void softpc_ccpu_lifecycle_leave(void);
void softpc_ccpu_lifecycle_request_exit(void);
void softpc_ccpu_lifecycle_clear_exit(void);
/* Clear asynchronous CCPU event bits only at the completed-run -> new-cold-run
   boundary. The original reset routine leaves this private map untouched. */
void softpc_ccpu_lifecycle_clear_pending_interrupts(void);
int softpc_ccpu_lifecycle_exit_requested(void);
void softpc_ccpu_lifecycle_return_outer(void);

#endif
