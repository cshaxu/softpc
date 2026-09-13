# M9 T55 S20: lib contract and boundary gates

## Admission

The owner approved the post-S19 audit findings. This is a bounded contract and
static-boundary cleanup, not a product or runtime redesign.

## Frozen ledger

| Item | Required change | Explicit exclusion | Proof |
| --- | --- | --- | --- |
| Pointer output contract | Every applicable public/component output-pointer API clears a non-null pointer output before validating other arguments, so every failure leaves it null. | Do not mutate scalar outputs or APIs whose documented invalid-call contract intentionally preserves an existing ownership slot. | Focused failure-output smoke covering every production hit. |
| Source dependency gate | Extend the component-DAG verifier to inspect every quoted lib source/header include. Internal includes must use canonical `lib/<component>/...` spelling, then validate the exact edge against the CMake graph. | Do not add a runtime layer or alter the admitted dependency graph. | Positive gate plus forbidden-edge and relative-include fixtures. |
| Local status cleanup | Make `ui_console_publish_text_frame` reject null required arguments while retaining graphics-frame no-op; remove the known post-transfer no-op storage close. | No new status framework or storage behavior. | Focused component/storage tests. |
| Local no-op cleanup | Remove the one-caller `ui_window_display_rect()` forwarding helper; its sole Window caller constructs the full client rect directly. | No UI geometry behavior change or public ABI change. | Existing bounds/full regressions. |
| Baseline truth | Update the active technical baseline to the already-proved S20 P1 57/57 dual-width and 5/5 strict-library result. | No product behavior change. | Documentation gate. |

## Design

Output-pointer initialization is a local API rule: once the output pointer
itself is known addressable, clear it before any unrelated input validation.
This makes failure state deterministic without inventing rollback state.

The component DAG has one source of truth in the existing verifier. It derives
the component owning each lib source/header path, requires every quoted
internal include to use canonical `lib/<component>/...` spelling, and validates
each resulting edge with the same allow-list used for CMake linkage. Thus a
relative include cannot bypass the edge check. Focused fixtures prove both a
forbidden edge and a forbidden relative include fail. Platform implementation
headers remain component-private.

The two status/ownership fixes use existing statuses and ownership transfer;
they add no callback, queue, worker, or retry path.

## Non-goals

Do not alter MVDM, package INI/media, UI lifecycle, broker transaction,
mailbox topology, storage format, public ABI signatures, Linux parity, or
runtime product policy.

## Verification

Run focused failure-contract, both DAG fixtures, console and storage tests; fresh
x64/x86 builds and full CTest; strict-library CTest; manifest, component-DAG,
standalone-source-boundary and documentation-governance gates. Record tracked
production/test line accounting and an independent post-commit review.
