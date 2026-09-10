# M9 T48 S6 — Monitor completion transaction

Make every VM completion produce one product outcome decision at control.  If
the monitor is Current Console, output is exactly `outcome → prompt → cooked
line`; if raw VM Console is current, the monitor-only outcome is discarded and
never replayed after a later handoff.  Cover started, resumed, paused, stopped,
reset-completed, and error, independent of whether the initiating request was
a monitor line or a UX hotkey.
