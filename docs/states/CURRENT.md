# Project Status

## Current Work

Owner admits M9 T100 S1 on 2026-11-12: original-first mirror audit.
Owner confirms the method and activates S1; existing #error exclusions are
skipped without new bookkeeping. T99 remains closed; see its
[completion audit](../history/M9-T99-completion-audit.md).

## M9 T100 S1 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | New |
| Admission And Approval | Owner confirms the method and admits S1 execution on 2026-11-12; existing #error exclusions need no new accounting. |
| Objective | Audit/restore mirror differences in frozen literal directory order; replace duplicate Compat behavior with original mirror implementation first, then delete whole mirror files left without necessary integration. |
| Non-goals | No guest changes, excluded-body re-audit, automatic retention, new implementation paths or sibling modifications. |
| Reference Baseline | SoftPC ac9d8dc85a6d9b0cc66d0a6649f60e8903406f18; XP SP1 a2f6d7c93aa4f11efc51147566d5961c59a8ef89; existing manifest records 472 retained files, not a new audit result. |
| Candidate Proposal | [Original-first audit method](../proposals/m9-xp-mirror-original-first-audit.md). |
| Files And ABI Surface | Selected softpc.new mirror, its provenance and necessary coupled callers/tests; disclose ABI consequences per point. |
| Applicable Rules | docs/README.md reading set; EXECUTION, DOCUMENT, CODING mirror policy and source-research-policy; owner order supersedes comparison project's ordering. |
| Verification | Per complete substantive file: cached x86/x64 build, publish, focused/full regression, manifest and documentation gates; final-source identity required. Owner permits later batches unaffected by native desktop/audio/package paths to defer those tests until final acceptance; preserve pending coverage explicitly. |
| Expected Markers | Ordered file/point progress, exact restoration, evidence-qualified owner decisions; no excluded-body ledger. |
| Asset Needs | Preserve configuration/media/snapshots. S1 owns build/t100-s1 if needed; background jobs bounded to 2 hours and 15 minutes no progress, scratch removed after delivery. |
| Reporting Requirements | Explain evidence threshold, exact restoration, pending decisions, file order, diff metrics and proposed delivery cadence. |
| Stop Conditions | Unproven necessity remains investigation; evidence-backed retained differences require owner decision; new unjustified XP-relative differences or added/deleted diff-line increases are prohibited, including unused-file/wiring removal; exact restoration may split contiguous ranges without adding a difference. Discard genuinely increasing candidates without requesting an exception. |
| Exit Criteria | Reconcile ordered audit dispositions and delivered changes with actual diff review; no unresolved point may be reported closed. |
| Original Owner Request | Preserved verbatim in the linked proposal. |
| Similar-Issue Sweep | All non-excluded mirror differences, including previously approved ones, are subject to the same evidence threshold. |

## Current Technical Baseline

T100 S1 P140 completes files 438-443. host_gen/host_gfx/host_qev equal XP;
host_hfx retains only qualified notice R0560. host_inc original blank and
host_rrr original wide-character declarations are restored. Production +4/-3,
tests unchanged; XP diff decreases by five lines. Both cached builds publish
fresh EXEs; VGA/BOP/quick-time tests pass 3/3 per width. EXEs equal P139 after
permitted metadata/time normalization; no repeated full suite. P133 background
188/188 per width remains applicable; final native acceptance and prior owner
decisions remain pending. Frontier 443/472; next file 444 host/inc/insignia.h.
S1/T100 remain open. See the [audit evidence](../etc/evidence/softpc/m9-t100-progress.md).


S43 implementation 51d0bc15 adds tools/Build.cmd: PowerShell-free x86/x64
build/publication via existing cached Ninja presets. Two dual-width runs and
four package integration tests pass. The owner requested reverting subsequent
local EXE changes; published files match the accepted 51d0bc15 delivery.


S42 final implementation 60bdbaa4 removes unused mouse_io.c, nop.c, vglob.c
and their obsolete wiring/state. Both published EXEs pass 392/392 tests.
Snapshots now use SCP3; older SCP2 files are rejected without compatibility.
The retained mirror has 472 files. User media/configuration are preserved;
the owner-provided assets/binary/snapshot update ships in S42 P6.


S41 delivery ecf263a8 completes the five admitted mirror cleanup/guard units:
production +16/-29, with XP mirror added+deleted diff reduced by 13 lines.
Snapshot command IDs and the existing device paths remain unchanged.
See [S41 closure](../history/M9-T99-S41-mirror-cleanup.md).

Td S23 delivery db76bb60 repairs the cached compiler timestamp, updates
execution rules and verifies embedded EXE literals. Both architectures pass
188 background plus eight native tests each: 392/392. Published EXEs carry
Nov 8 timestamps. See [timestamp closure](../history/M9-Td-S23-build-timestamp.md).
Only Emulator build configuration/verifier/manifest changed within the public
eight trees; no fresh NXVM alignment is claimed.

Original GenericByteFill, Unchained DWORD and Chain4 bulk-fill defects belong to the
[pending diff decisions](../etc/evidence/softpc/m9-t100-progress.md#pending-original-defect-diff-decisions), not TODO.
[TODO](TODO.md) retains shared-fixture aggregate timing debt. The runtime-performance candidate
remains in [Queue](QUEUE.md), unnumbered and unadmitted. JIT/WHP discussion
did not admit S44 or a new implementation task.

## Recent M9 Closures

- [S43 build entry](../history/M9-T99-S43-build-entry.md).

- [S42 unused mirror retirement](../history/M9-T99-S42-unused-mirror-retirement.md).

- [S41 mirror cleanup](../history/M9-T99-S41-mirror-cleanup.md).
- [S40 retained-difference audit](../history/M9-T99-S40-retained-difference-audit.md).

- [S39 mirror wrapper cleanup](../history/M9-T99-S39-mirror-wrapper-cleanup.md).
- [S38 C-VID rules](../history/M9-T99-S38-cvid-rules.md).

- [S37 original-device audit](../history/M9-T99-S37-original-device-audit.md).
- [S36 CHAIN2 DWORD reads](../history/M9-T99-S36-chain2-dword-read.md).
| Task | Closure |
| --- | --- |
| T99 | [Completion audit](../history/M9-T99-completion-audit.md). |
| T98 | [Completion audit](../history/M9-T98-completion-audit.md). |

## Recent Governance

[Td S23 timestamp governance](../history/M9-Td-S23-build-timestamp.md) is closed.
Task scratch is removed; reusable Ninja/ccache roots remain. Follow existing
build-cache and task-owned probe cleanup rules.
