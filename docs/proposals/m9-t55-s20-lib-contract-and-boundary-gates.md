# M9 T55 S20: lib contract and boundary gates

## Admission

The owner approved the post-S19 audit findings. This is a bounded contract and
static-boundary cleanup, not a product or runtime redesign.

## Frozen ledger

| Item | Required change | Explicit exclusion | Proof |
| --- | --- | --- | --- |
| Pointer output contract | Every applicable public/component output-pointer API clears a non-null pointer output before validating other arguments, so every failure leaves it null. | Do not mutate scalar outputs or APIs whose documented invalid-call contract intentionally preserves an existing ownership slot. | Focused failure-output smoke covering every production hit. |
| Source dependency gate | Extend the existing component-DAG verifier to inspect direct lib source/header includes as well as CMake links. | Do not add a runtime layer or alter the admitted dependency graph. | Positive gate plus a forbidden-edge fixture/probe. |
| Local status cleanup | Make `ui_console_publish_text_frame` reject null required arguments while retaining graphics-frame no-op; remove the known post-transfer no-op storage close. | No new status framework or storage behavior. | Focused component/storage tests. |
| Baseline truth | Update the active technical baseline to the already-proved 56/56 dual-width result. | No product behavior change. | Documentation gate. |

## Design

Output-pointer initialization is a local API rule: once the output pointer
itself is known addressable, clear it before any unrelated input validation.
This makes failure state deterministic without inventing rollback state.

The component DAG has one source of truth in the existing verifier. It will
derive the component owning each lib source/header path, inspect quoted direct
`lib/<component>/...` includes, and validate each resulting edge with the same
allow-list used for CMake linkage. A small fixture proves that a forbidden
include fails. Platform implementation headers remain component-private.

The two status/ownership fixes use existing statuses and ownership transfer;
they add no callback, queue, worker, or retry path.

## Non-goals

Do not alter MVDM, package INI/media, UI lifecycle, broker transaction,
mailbox topology, storage format, public ABI signatures, Linux parity, or
runtime product policy.

## Verification

Run focused failure-contract, DAG-fixture, console and storage tests; fresh
x64/x86 builds and full CTest; strict-library CTest; manifest, component-DAG,
standalone-source-boundary and documentation-governance gates. Record tracked
production/test line accounting and an independent post-commit review.
