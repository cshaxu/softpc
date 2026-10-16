#ifndef SOFTPC_COMPAT_SB_MIDI_H
#define SOFTPC_COMPAT_SB_MIDI_H

#include "lib/audio/midi/output_interface.h"

/* Prepare/dispose bracket the machine lifetime with callers quiescent.
 * Other operations belong to its executor; cancel alone may run concurrently.
 * Native destruction is serialized against cancel to protect handle lifetime;
 * its preceding reset wait is cancellable. Failure may retain ownership.
 * Volume is optional host capability. */
lib_status softpc_sb_midi_prepare(void);
lib_status softpc_sb_midi_dispose(void);
lib_status softpc_sb_midi_open(lib_u16 gain);
lib_status softpc_sb_midi_close(void);
lib_status softpc_sb_midi_reset(void);
lib_status softpc_sb_midi_gain(lib_u16 gain);
lib_status softpc_sb_midi_cancel(void);
/* Sticky cancellation failure, read by the existing executor status path.
 * The handle guard also synchronizes this cross-thread result. */
lib_status softpc_sb_midi_status(void);
void softpc_sb_midi_report_oversize(void);

/* Executor-owned complete-message submission. On rejection no bytes are
 * accepted; the device retains its complete message for service/cancellation.
 * This host boundary never parses guest bytes or owns running status. */
lib_status softpc_sb_midi_write(const lib_u8 *bytes, lib_u32 count);

#endif
