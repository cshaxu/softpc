# M9 T85 S2: Indexed Pointer Repair Closure

Owner request: repair the Win3.1 pointer regression, build/test/commit/push,
then await verification. Owner accepts and approves closure on 2026-09-28.
T85 remains open. [Proposal and finite proof ledger](../proposals/m9-windows-101-startup-repair.md).

## Actual-change review

Reviewed `aa719ce5..f8fe3df8`: eight paths, comprising two Compat C/H files,
two product tests, two status/evidence documents and two package EXEs.
Production +13/-12 (net +1); tests +75/-0; combined +88/-12 (net +76).
Counts use `git diff --numstat aa719ce5 f8fe3df8 -- src test`.
Indexed painters and V7 composition now share one buffer; only packed mono
requires translation. No new API, state or allocation; original mirror,
shared six-component corpus, snapshot ABI, INI and guest media unchanged.

## Coverage and acceptance

The proposal's frozen ledger is discharged: indexed aliasing and completed
pointer paint/move/hide/clipping, palette preservation, generation invalidation,
mono/indexed rebind, real Win3.1 x86/x64 pointer frames, retained Win1.01 mono,
dual Release builds and background regression. The new pointer test is red
on S1 and green after the fix. All eight DIB writer/reader sweep hits have
explicit dispositions in the proposal; no extra compositor or redraw path.

Focused tests pass 4/4 per width. Serial background regression passes x64
121/121 (163.92s), x86 121/121 (135.71s), including snapshot tests.
Five native desktop tests per width excluded; no new RDP or Linux claim.
Owner manual acceptance supplements, not replaces, that recorded evidence.

P1 `8c15e893` and review P2 `f8fe3df8` are pushed. Accepted package hashes:

- x86: `B13CD6242C69B479609F61EA1E4DCCDAD6A956BD65812DF511E72E6093956DDF`.
- x64: `A3D6DAD0702FFDF9E11D71ADEC7D149BA70865D18B775D107B455636476D123F`.

Closure is documentation-only; binaries need no rebuild. User's pre-existing
snapshot modification is preserved and excluded from closure. The separate
color investigation changes neither implementation nor guest installation.
