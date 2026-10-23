# M9 T99 S10: Original VGA Cursor Disable

Original request: 收口s9 准入s10; 老规矩 调研审计 设计; after the
mirror accounting explanation: 批准实施 老规矩执行. Owner now accepts:
好，收口本S，转入下一个S任务。老规矩. S10 is closed; T99 remains open
and S11 starts research/design without skipping S12.

Research 2b20b2ae and actual review 65afb9be establish two original-path
cursor-disable defects, hardware references and the minimal repair. Executor
ad9d5fcf and coordinator actual-commit review 2866cbd5 deliver that repair:
unprotected CRTC 0A always stores its byte; the existing shape routine honors
cursor_off before applying its original shape rules. No new state, path,
public API, snapshot layout or frontend hardware interpretation is added.

Production is +5/-12, net -7; existing App VGA test is +70/-0. Full-file diff
against the fixed necessary XP subset changes from +221/-0 to +226/-12,
increasing changed lines by 17. The local manifest hash and reason ledger
are updated without changing raw/subset identity or source selection ranges.
The distinct end-row/wrap semantic finding remains in TODO, not silently fixed.

Cached x64/x86 builds refresh both EXEs. Each width passes 193/193: 185
background, five desktop and three native audio, with exact registration/
execution equality and zero failures/skips. Focused actual-port/Driver tests
cover disable-only changes, re-enable, hidden recomputation, four font heights,
V7 protection, invalid starts and original mode reset. The test-draft correction
distinguishes hidden nonchanges from required visibility publication.

Public eight trees, Compat/Driver, root infrastructure, INI, guest media and
snapshots remain unchanged. Non-mirror warning locations are zero; retained
original-header diagnostics are explicitly recorded. Mirror, documentation,
whitespace and residue checks pass. Task scratch is retired and both pushed
commits are clean. The [proposal](../proposals/m9-public-handoff-simplification.md#s10-approved-implementation)
retains complete evidence, artifact hashes, failed drafts and proof limits.
This closure adds no fresh build/test or downstream-sync claim.
