# M9 T80 S4: frame sequence wrap

## Admission and implementation boundary

The owner accepted S3, admitted S4, then approved the narrowed repair.
Execution baseline: `3284fd89`. The proposal's S4 section records the initial
estimate, finite sequence-use ledger and exact changed-path counts. S4 was
delivered for owner testing and is now owner-accepted; T80 remains open.

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

Executor `478d04ec` is pushed. The same session then switched to coordinator
and reviewed `git diff 3284fd89..478d04ec`: four production files, three tests,
two manifests, four task documents and two package EXEs, fifteen paths total.
The sole publisher retains its lock and state ownership; both ordering sites
share one private helper. Run filtering and UI equality remain intact; no
unapproved public interface or other component changed. The half-range limit
is explicit in code and proposal, not hidden behind an unlimited-wrap claim.

Focused 3/3 per width and full background x64 111/111 (173.24s), x86 111/111
(160.81s) pass. Five desktop tests per width were excluded. Strict Common C11,
six manifests, component checks and documentation governance pass. Binary
hashes match the proposal and sizes match S3; INI/media remain untouched.
No admitted implementation gap found in this bounded review. Owner testing
was the remaining closure gate. The owner subsequently requests
“批准收口S4，准入S5开始修复”: S4 is accepted and closed; T80 remains open.
