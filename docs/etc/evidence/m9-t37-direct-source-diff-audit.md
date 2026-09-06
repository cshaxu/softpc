# M9 T37 S1 Direct-Source-Diff Audit

## Scope And Method

This audit runs `scripts/audit_pristine_divergence.ps1` with the selected
read-only OpenNT `softpc.new` tree and the current
`src/mvdm/softpc.new/` tree. The audit normalizes line endings, inventories
every current C/H difference, and fails when a path has no disposition.

The current build has four source-transform scripts. They produce 52 selected
C/H inputs: three device sources (`cmos.c`, `timer.c`, `reset.c`), 34 C-VID
rule sources, one C-VID event-glue source, and 14 CCPU sources/headers. The
scripts are `transform_cmos.py`, `transform_timer.py`, `transform_reset.py`,
and `transform_cvid_gdp.py`. Audit/mapping scripts and guest-media tooling are
not source transforms and are outside this removal count.

## Current Difference Ledger

The fresh comparison reports 47 current differences:

| Disposition | Count | S1 interpretation |
| --- | ---: | --- |
| `port-abi-overlay` | 28 | Candidate direct-source portability batches; each remains subject to source-level review. |
| `compat-host` | 14 | Existing external-host responsibility; do not move new state or policy into the mirror. |
| `restore-pristine` | 2 | Review whether the difference is obsolete before any source-transform retirement. |
| `blocked-behavior` | 3 | Not mechanical portability work; owner decision is required before migration. |

The three behavior blockers are:

- `base/bios/tape_io.c`: substitutes the fixed BIOS configuration-table
  segment for the C handler's caller context;
- `base/ccpu386/c_main.c`: adds a CCPU hardware-interrupt pending-bit clear
  endpoint; and
- `base/keymouse/mouse.c`: implements the Windows 3.1 InPort diagnostic
  transfer.

They may not be silently folded into a direct ABI batch or source-transform
retirement. Their later disposition is an external-host implementation,
explicitly approved in-mirror behavior repair, or restoration to the selected
reference.

## Review Batches

| Batch | Inputs | Proposed disposition | Required proof |
| --- | --- | --- | --- |
| C-VID GDP representation | 34 `sinit*`/`sevid*` rules, C-VID glue and CCPU GDP declarations | Narrow direct source diffs plus the existing external native-width GDP state owner | C-VID rule, graphics, x86/x64 frame tests |
| CCPU host ABI | generated CCPU headers/sources, including thread and SAS declarations | Narrow direct source diffs where ABI-only; block any machine-control addition | CCPU lifecycle, IRQ, BOP, x86/x64 tests |
| Device host representation | `cmos.c` and pointer-width portions of `reset.c` | Narrow direct source diffs; retain external media topology provider | CMOS/media/reset and x86/x64 tests |
| Timer and reset semantics | timer zero-delay handling and reset topology branches | Behavior review; do not classify as a mechanical width replacement | timer/reset and guest boot regression |
| Existing direct ABI headers | selected `base/inc`, C-VID, CCPU and original-host headers | Narrow direct-source review batches by declaration family | compiler dependency and dual-width build proof |
| Existing external compatibility | `src/host/compat/` | Retain external ownership unless a later owner review proves removal | affected ABI boundary tests |

## Audit Gate

The audit script now defaults to the current `src/mvdm/softpc.new/` layout and
emits every row before failing on an unclassified path. The 47 current rows
are classified; a future differing C/H path fails the audit until it receives
an explicit disposition.

## S1 Result

S1 establishes that retiring source transforms is not one undifferentiated
copy operation. The source-transform removal task may proceed only with the
direct-source batches above and must stop for an owner decision on the three
behavior blockers.
