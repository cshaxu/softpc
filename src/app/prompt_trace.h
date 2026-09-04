#ifndef SOFTPC_PROMPT_TRACE_H
#define SOFTPC_PROMPT_TRACE_H
int app_prompt_trace_enabled(void);
void app_prompt_trace_reset(void);
void app_prompt_trace(const char *format, ...);
#endif
