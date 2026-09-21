# M9 T80 S4: frame sequence wrap

## Admission and implementation boundary

The owner accepted S3, admitted S4, then approved the narrowed repair.
Execution baseline: `3284fd89`. The proposal's S4 section records the initial
estimate, finite sequence-use ledger and exact changed-path counts. S4 remains
open for owner testing; this record does not close S4 or T80.

Machine keeps the existing u32 publication counter and skips reserved zero.
The notification gate and copied-frame reducer use one private inline modular
comparison, accepting a first nonzero frame and rejecting duplicates, stale
serials and exactly half-range distance. Ordering assumes compared positions
are less than 2^31 apart. UI retains equality-only deduplication. Existing run
generation checks and complete-frame latest-wins ownership remain unchanged.

No new state, allocation, cache, queue, public ABI, field width or presenter
route. Lib, Core, x86, snapshot format, INI and guest media are unchanged.

## Changed paths and verification

Production: machine.c +4/-1, session/control_state.h +9/-0,
session/control_state.c +2/-2, session/session.c +1/-1: +16/-4, net +12.
Tests: machine_wait +12, session_frame +40, composition +17: +69/-0.
Total code +85/-4, net +81; manifests, task documents and EXEs separate.
Counts use `git diff --numstat 3284fd89` with the seven C/H paths selected.

New Machine and Session tests first failed against baseline production
(zero publication and rejected MAX-to-1 transition). After the repair, the
three focused tests passed on both widths. Coverage includes first/zero,
half-range, latest-wins skips, duplicate/stale notices, cold run, run mismatch
and presenter recreation. Both Release builds pass strict Common C11 warning
flags. Full regression and artifact hashes are retained in the proposal.
Desktop interaction and Linux execution are not claimed.

## Actual-change review

The executor delivery must be pushed before the coordinator's actual Git
diff review is recorded here. Owner testing remains the closure gate.
