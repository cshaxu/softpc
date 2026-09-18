# Project Status

## Current Work

M9 T69 S3 is closed at the owner's direction; T69 remains open. S4 is active:
read-only accounting and simplification audit of every retained T69 production
and test change against the pre-task commit and the original OpenNT mirror.
No production optimization is admitted by this audit packet.

S3 P1 has reproduced BIOS-record-dependent width with unchanged registers.
The bounded correction replaces the host packed-mode table override with the
existing painter's one-byte/one-pixel row contract. Both packages build and the
VGA smoke passes on x86/x64. Each full run is 106/108: the two package monitor
tests fail at stage 16; no controlled baseline comparison establishes cause.
The previous label "current-controller geometry" was inaccurate: the helper
reads BIOS bookkeeping. Actual Win3.1 roundtrip acceptance remains outstanding.

S3 P2 removes S2's generation-rearmed delay after a failing continuous-
invalidation regression. The original bounded countdown is restored and both
packages build. VGA tests pass; full runs are x64 105/108 and x86 106/108.
The x64 BIOS-tick assertion passed on isolated rerun; both package stage-16
failures remain unresolved. This is not a new timing workaround or S closure.

The owner reports that P2 no longer jumps width for either initially windowed
Prompt or fullscreen-to-windowed return. A real Win3.1 windowed-PIF A/B run
now reproduces old 1280-wide published frames at BDA E0h with an unchanged
packed painter; current code keeps those selections at 640. A transient
chain4-off 1280 DIB was also observed on current code, but not as a published
frame in that run. See the proposal's S3 P5 evidence; this is not acceptance
of text/fullscreen corruption or proof that all transient geometry is safe.

The owner subsequently reports that width jumping is gone. A separate report
of fullscreen-PIF native Window height oscillation is currently not reproducible
by the owner or bounded probes. The owner directs ending that investigation
without a speculative repair. Prior windowed-PIF use is a possible precondition;
the reported change affected the actual outer Window, not just its contents.
See S3 P8 in the proposal. This disposition does not close T69's other defects.

## M9 T69 S4 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner: record the intermittent height problem in TODO, close S3, and admit the next S to account for and audit all T69 repair changes and opportunities to reduce original-MVDM diff. |
| Objective | Freeze the complete T69 changed-file universe; account for retained, superseded and unnecessary changes; identify behavior-preserving simplifications and original-mirror restorations with exact evidence. |
| Non-goals | No KVM Window debounce/filter, PIF/application/mode-specific branch, timer retry, forced repaint, guest-media/configuration change, Lib/Common API change, second renderer or silent geometry coercion. |
| Reference Baseline | Pre-T69 86de8eb versus S4 admission 662f7d4. Read-only OpenNT 5e4619ab61c2aa76151e03973cce340be2933e61 is the pristine comparator. Owner-tested package remains 19533be. |
| Candidate Proposal | [Win3.1 MS-DOS prompt display roundtrip repair](../proposals/m9-win31-display-roundtrip.md) |
| Files And ABI Surface | Nine changed production files in Compat/MVDM, two changed tests and their root CMake registration; inspect removed intermediate paths too. Only task/status/evidence documents may change. No source, API, package, INI or media edits. |
| Applicable Rules | EXECUTION, ARCHITECTURE, CODING, DOCUMENT and PRODUCT UI. The original renderer/Compat surface remains the only display state owner; KVM consumes copied complete frames only. |
| Verification | git diff --numstat 86de8eb 662f7d4 for task accounting; no-index Git comparisons for each changed mirror file; full caller search for deletion candidates; inspect focused test assertions and retain previous full-suite failures. Documentation governance and diff checks apply to this read-only delivery; no rebuild needed for unchanged code. |
| Expected Markers | No completed frame combines pixels/stride/geometry from different renderer states. No geometry is inferred downstream from dirty bounds or a previous frame. The selected correction has no customer/source/mode identity condition. |
| Asset Needs | None. Preserve existing dual-width packages, owner INI and guest media. |
| Diagnostic Budget | Read-only Git/source analysis only. No raw runtime tracing or new media fixtures are required for this accounting audit. |
| Reporting Requirements | Record each geometry source and disposition, the exact pre/post mirror diff, tracked code accounting, focused/full evidence and a manual checklist for initial windowed Prompt, fullscreen roundtrip and Win95 Setup transition. |
| Stop Conditions | A downstream filter, mode/PIF/application branch, timing workaround, unproven MVDM behavior change, second rendering route or Lib/Common scope expansion stops work for owner direction. |
| Exit Criteria | Every file in the frozen change universe has rationale, owner, proof and keep/remove/investigate disposition; quantify mirror-diff opportunities separately from net code size; report recommendations without implementing unapproved optimizations. |
| Original Owner Request | “你先把这个问题记录到TODO里面。然后收口当前S任务，准入下一个S任务对本次T任务整个调查和修复的代码改动进行一次统计、梳理、审计，看看这个修复是否有值得优化的地方、相对原始mvdm产生的diff是否有可以消灭或者减少的机会。” |
| Similar-Issue Sweep | Inspect all T69 retained production/test hunks and superseded implementation commits; search the entire src/test trees for callers of helpers proposed for deletion. |

## Current Technical Baseline

- Source: T68 is closed. The preserved Scan-1 table mapper rejects both first
  out-of-range input values without changing valid key mappings. No Lib,
  Common, App, VM, Compat interface or input behavior changed.
- Both widths have 107 passing test cases; package EXEs were refreshed without
  changing owner INI/media. See [T65 audit](../history/M9-T65-completion-audit.md).
- Snapshots are width-independent fixed-order binary streams with no magic,
  version or section identifier. They restore CPU/device/media state into a
  normal PAUSED machine; Window creation remains deferred until resume.
- `save` accepts RUNNING or PAUSED. A paused save writes an already-held VM
  checkpoint directly, or privately advances the existing executor to one
  while the product remains PAUSED and guest input stays gated.
- DIRECT/READONLY media retain and verify their external references; FDD/HDD
  OVERLAY effective differences and cylinder state are in the same binary.
- T63 snapshot behavior remains the owner-accepted baseline. Current package
  EXEs include T67; the owner’s current package INI edit ships with T67.
- T69 is reopened: its source-neutral Compat display transaction, V7
  controller geometry, and detached text/cursor updates remain in force, but
  a periodic initial-windowed Win3.1 geometry transition still requires audit.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T68 | S1 complete; owner-directed safety repair; final dual-width 107/107. | [Audit](../history/M9-T68-completion-audit.md) |
| T67 | S1--S4 complete; owner accepted; final dual-width 107/107. | [Audit](../history/M9-T67-completion-audit.md) |
| T66 | S1--S4 complete; owner-validated dual-width package. | [Audit](../history/M9-T66-completion-audit.md) |
| T65 | S1 complete; request-slot refactor cancelled after contract audit; dual-width 107/107. | [Audit](../history/M9-T65-completion-audit.md) |
| T64 | S1 audit accepted; S2 narrowed frame copy verified; owner accepted. | [Audit](../history/M9-T64-completion-audit.md) |
| T63 | S1–S12 complete; owner acceptance; final dual-width 106/106. | [Audit](../history/M9-T63-completion-audit.md) |
| T62 | S1–S8 complete; owner acceptance; dual-width 101/101 with recorded intermittent tick debt. | [Audit](../history/M9-T62-completion-audit.md) |
| T61 | S1–S13 complete; mirror and VM/Compat ownership audit; dual-width 98/98. | [Audit](../history/M9-T61-completion-audit.md) |

## Recent Governance

T69 remains open. S3 has an owner-directed
[bounded closure](../history/M9-T69-S3-width-investigation.md); S4 is active.
