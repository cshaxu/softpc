/*[
************************************************************************
	Name:			quick_ev.h
	Author:			WTG Charnell
	Created:		Unknown
	Sccs ID:		@(#)quick_ev.h	1.20 06/08/95

	(c)Copyright Insignia Solutions Ltd., 1992. All rights reserved.

	Purpose:		Quick Event Manager definitions
	Description:		Definitions for users of the quick
				event manager

************************************************************************
]*/

#include	"host_qev.h"	/* for q_ev_handle typedef */

typedef void (*Q_CALLBACK_FN) IPT1(long, parm);

/*
 * Snapshot records describe pending work only.  They deliberately contain no
 * queue links or callback addresses: the caller translates callbacks to its
 * own stable semantic identifiers, and quick_ev rebuilds its private lists.
 */
typedef struct q_event_snapshot_entry {
	unsigned long time_from_last;
	unsigned long original_time;
	q_ev_handle handle;
	long param;
	unsigned long event_type;
	unsigned long callback_id;
	unsigned long hash_next_index;
} Q_EVENT_SNAPSHOT_ENTRY;

typedef struct q_event_snapshot_state {
	q_ev_handle next_quick_handle;
	q_ev_handle next_tick_handle;
	unsigned long quick_count;
	unsigned long tick_count;
	unsigned long quick_entries;
	unsigned long tick_entries;
	unsigned long quick_hash_head[16];
	unsigned long tick_hash_head[16];
} Q_EVENT_SNAPSHOT_STATE;

typedef int (*Q_SNAPSHOT_ENCODE_CALLBACK) IPT2(Q_CALLBACK_FN, callback,
	unsigned long *, callback_id);
typedef Q_CALLBACK_FN (*Q_SNAPSHOT_DECODE_CALLBACK) IPT1(unsigned long,
	callback_id);

extern void q_event_init IPT0();
extern void delete_q_event IPT1(q_ev_handle, handle);
extern void dispatch_q_event IPT0();
extern void delete_tic_event IPT1(q_ev_handle, handle);
extern void tic_event_init IPT0();
extern q_ev_handle add_tic_event IPT3(Q_CALLBACK_FN, func, unsigned long, time, long, param);
extern q_ev_handle add_q_event_i IPT3(Q_CALLBACK_FN, func, unsigned long, time, long, param);
extern q_ev_handle add_q_event_t IPT3(Q_CALLBACK_FN, func, unsigned long, time, long, param);
extern void q_event_snapshot_measure IPT1(Q_EVENT_SNAPSHOT_STATE *, state);
extern int q_event_snapshot_capture IPT6(Q_EVENT_SNAPSHOT_STATE *, state,
	Q_EVENT_SNAPSHOT_ENTRY *, quick_entries, unsigned long, quick_capacity,
	Q_EVENT_SNAPSHOT_ENTRY *, tick_entries, unsigned long, tick_capacity,
	Q_SNAPSHOT_ENCODE_CALLBACK, encode);
extern int q_event_snapshot_restore IPT6(const Q_EVENT_SNAPSHOT_STATE *, state,
	const Q_EVENT_SNAPSHOT_ENTRY *, quick_entries, unsigned long, quick_capacity,
	const Q_EVENT_SNAPSHOT_ENTRY *, tick_entries, unsigned long, tick_capacity,
	Q_SNAPSHOT_DECODE_CALLBACK, decode);

#ifndef NTVDM
extern q_ev_handle add_q_ev_int_action IPT5(unsigned long, time, Q_CALLBACK_FN, func, IU32, adapter, IU32, line, IU32, param);
#endif

#ifndef CPU_40_STYLE
/*
 * Host access routines for 3.0 CPU (4.0 supplies its own prototypes
 * in generated include files).
 */
#ifndef host_calc_q_ev_inst_for_time
extern ULONG host_calc_q_ev_inst_for_time IPT1( ULONG, time );
#endif /* host_calc_q_ev_inst_for_time */
#ifdef NTVDM
extern ULONG host_calc_q_ev_time_for_inst IPT1(ULONG, inst);
#endif

extern void host_q_ev_set_count IPT1( ULONG, count );
extern ULONG host_q_ev_get_count IPT0();

#else	/* CPU_40_STYLE */
extern IU32 calc_q_inst_for_time IPT1(IU32, time);
extern IU32 calc_q_time_for_inst IPT1(IU32, inst);
#define host_calc_q_ev_inst_for_time      calc_q_inst_for_time
#define host_calc_q_ev_time_for_inst      calc_q_time_for_inst
#endif	/* CPU_40_STYLE */

#ifndef NTVDM
#ifdef CPU_40_STYLE

/* data structure used to control add_q_ev_int_action requests */
typedef struct aqeia_req {
	IS32 ident;		/* queue identifier, 0 - unused */
	Q_CALLBACK_FN func;
	IU32 adapter;
	IU32 line;
	IU32 param;
	struct aqeia_req *next;
} Q_INT_ACT_REQ, *Q_INT_ACT_REQ_PTR;

#define Q_INT_ACT_NULL	((Q_INT_ACT_REQ_PTR)0)

#endif  /* CPU_40_STYLE */

#endif  /* !NTVDM */
