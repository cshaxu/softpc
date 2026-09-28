# M9 T84 — completion audit

The owner approved closure of T84 on 2026-09-28.  This audit closes the task;
it does not claim to repair the separately deferred x86 BIOS[0x52] null BOP
dispatch recorded in [TODO](../states/TODO.md).

## Original objective

T84 began as the bounded 80x50 KVM text-frame upgrade: support active text
extents through 80 columns by 50 rows while retaining a fixed 80-cell stride,
an 80x25 monitor status surface, and no guest-specific presenter policy.

## Finite S ledger

- S1 delivers the connected 80x50 capacity change; S2 audits that initial
  delivery.  Both are owner accepted.
- S3 audits NXVM's shared corpus; S4 records its intake; S5 imports the
  accepted six-component corpus; and S10 later synchronizes the complete
  `test/lib`/`test/common` corpus.  Each has its own accepted record.
- S6 resolves product host-boundary failure completion; S7/S8 converge raw
  Console backing capacity, exact written coverage, tail clearing and retry
  behavior without changing the host viewport.
- S9 establishes KVM source input reset and its native-loss/rejected-delivery
  proof.  S11 documents fresh-clone package/source routes.  S12 imports the
  NXVM source-local orphan-release correction exactly.

The detailed change ledgers, commit identifiers and dual-width verification
are retained in the individual S records named from [Current](../states/CURRENT.md)
and in the archived [T84 proposal](M9-T84-kvm-text-80x50-proposal.md).  No
open T84 subtask remains.

## Boundary and completion check

The task leaves one KVM text-frame path: VM publishes active dimensions and
cells; Common UI forwards them; KVM Window/Console own their respective
renderers; the broker owns native Console backing/output.  S1's original
Core mirror remains unchanged.  Later shared imports remain byte-identical to
their accepted NXVM baselines, rather than accumulating product-local forks.

The outstanding TODO is explicitly outside this display/task boundary and has
its own admission condition. The XP mirror-rebase queue entry remains
unadmitted. The Windows 1.01 candidate begins only after this closure.
