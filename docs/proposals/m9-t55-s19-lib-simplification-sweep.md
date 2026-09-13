# M9 T55 S19: narrow lib simplification sweep

## Admission

The owner approved a focused cleanup of the three findings from the post-S18
read-only library audit. This work does not redesign the accepted
Window/Console split, broker transaction, frame snapshot, or hotkey model.

## Frozen ledger

| Item | Required change | Explicit exclusion | Proof |
| --- | --- | --- | --- |
| Public creation contract | Correct ui-window and ui-console interface prose: failed create leaves output null. | No create/destroy ABI change. | Header/API smoke and documentation gate. |
| Storage result flow | Replace medium open/read/direct-write/fill OR-chains that collapse a prior result to I/O error with one local sequential status flow and one cleanup exit. | No app-wide status propagation; no storage format or medium semantic change. | Storage unit smoke including concrete propagated result paths. |
| Window notifier selection | Make the one startup-only replacement of the default mailbox wake impossible to repeat after selection. | No second notifier, queue, worker, or Window lifecycle path. | Existing Window startup/modal/control tests plus focused repeated-selection rejection proof. |

## Design

`lib_status` stays local to its component unless a documented live-worker or
native-reader terminal boundary is reached. Storage returns the concrete result
from its file primitive; it does not create a product fault path. The Window
notifier's first selection consumes the default wake object and marks selection
complete. A later selection returns `LIB_STATUS_INVALID_STATE` without changing
the active notifier.

## Non-goals

Do not combine frame/control mailboxes, replace copied frames with borrowed
buffers, abstract broker rollback branches, merge UI leaves, alter MVDM, change
guest media or user configuration, or add retry/recovery behavior.

## Verification

Run focused component/storage tests, x64/x86 builds and full CTest, strict-lib
CTest, manifest, component-boundary and documentation-governance gates. Record
tracked production/test line accounting and a post-commit coordinator audit.
