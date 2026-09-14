#ifndef VM_TRACE_H
#define VM_TRACE_H
int vm_trace_enabled(void);
void vm_trace_reset(void);
void vm_trace(const char *format, ...);
#endif
