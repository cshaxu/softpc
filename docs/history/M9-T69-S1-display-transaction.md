# M9 T69 S1 display transaction closure

## Owner-approved result

S1 began as the Win3.1 MS-DOS Prompt text-surface investigation. It repaired
the detached logical Console fill/geometry model, then exposed a separate
graphics publication defect. Two intermediate attempts classified DIB writers
as painter versus overlay; the owner rejected that architecture. The final P4
removes those classifications and was explicitly accepted for S1 closure.

The retained model has one Compat transaction record. Original
`host_start_update()`/`host_end_update()` callbacks delimit normal painter
updates. DIB bind, palette changes, painter output and the V7 pointer all
record the same accumulated dirty region. The outer end makes it observable.
When an original host callback has no enclosing update, the same generic
damage primitive creates and ends a one-mutation transaction; it does not
inspect producer, mode, size or dirty shape. There is no KVM Window filter,
overlay path, `painter_ready` gate or second renderer.

## Finite closure ledger

| Member | Disposition | Evidence |
| --- | --- | --- |
| Logical text Console fill/geometry | Repaired | Compat text smoke covers full fill, clipping and shared surface state. |
| DIB bind publication | Repaired | Bind remains private until a completed transaction. |
| Painter/palette/pointer publication | Unified | One `damage` path, nested transaction and standalone callback proofs. |
| Unconsumed consecutive frames | Preserved | Dirty bounds merge while the newest pixel surface remains latest-wins. |
| Original update ordering | Retained | Only `nt_start_update`/`nt_end_update` receive mechanical Compat bridge calls. |
| V7 geometry calculation | Retained separately | Existing V7 mode matrix proves its controller-geometry role; it is not a publication gate. |
| Initial-windowed Prompt width alternation | Transferred to S2 | Owner reports it remains after S1, with a first-initialization-only discriminator. |

## Accounting and verification

Final P4 (`10d6c0f`) changed tracked production paths by +66/-58 (net +8),
and focused tests by +41/-11 (net +30); docs and package EXEs are excluded
from that source accounting. Both x64 and x86 rebuilt successfully. Focused
`softpc-text-console-compat-smoke` and `softpc-vga-frame-smoke` passed on both
widths. Documentation governance and diff checks passed.

An existing package Window-restart integration probe still fails at stage 16;
it does not assert the removed transaction route and was not repaired under
this scope. The owner nonetheless explicitly directed S1 closure and admitted
S2 for the remaining distinct width alternation. T69 remains open.
