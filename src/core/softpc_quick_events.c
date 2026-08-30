/* The fixed standalone machine drives time from run slices, not host events. */
#include "insignia.h"
#include "host_def.h"
#include "quick_ev.h"

void q_event_init(void) { }
void tic_event_init(void) { }
void dispatch_q_event(void) { }
void delete_q_event(q_ev_handle h) { UNUSED(h); }
void delete_tic_event(q_ev_handle h) { UNUSED(h); }
q_ev_handle add_tic_event(Q_CALLBACK_FN f, unsigned long t, long p) { UNUSED(f); UNUSED(t); UNUSED(p); return (q_ev_handle)0; }
q_ev_handle add_q_event_i(Q_CALLBACK_FN f, unsigned long t, long p) { UNUSED(f); UNUSED(t); UNUSED(p); return (q_ev_handle)0; }
q_ev_handle add_q_event_t(Q_CALLBACK_FN f, unsigned long t, long p) { UNUSED(f); UNUSED(t); UNUSED(p); return (q_ev_handle)0; }
IU32 calc_q_inst_for_time(IU32 t) { return t; }
IU32 calc_q_time_for_inst(IU32 i) { return i; }
q_ev_handle add_q_ev_int_action(unsigned long t, Q_CALLBACK_FN f, IU32 a, IU32 l, IU32 p) { UNUSED(t); UNUSED(f); UNUSED(a); UNUSED(l); UNUSED(p); return (q_ev_handle)0; }
