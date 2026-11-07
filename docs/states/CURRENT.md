# Project Status

## Current Work

Owner approves T99 S33 repair following the reproduced investigation. S32 delivery is retained in its
[record](../history/M9-T99-S32-line-endings.md). T99 remains open.

## M9 T99 S33 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner approves minimal unified repair and delivery; follow-up audit must retain correct and newly discovered fixes, reverting only proven unnecessary changes. |
| Objective | Correct the CHAIN2/unchained video-plane layout mismatch and virtual text BIOS aperture; verify bidirectional DOS transitions and subsequent input/clear/scroll. |
| Non-goals | No guest modification, public-component redesign, second video-memory path or guest-specific workaround; preserve user INI, original media and snapshot. No Computer Use. |
| Reference Baseline | SoftPC 9250732e and owner-provided failing snapshot; fixed XP SP1 source for mirror comparison. |
| Candidate Proposal | [Public handoff simplification](../proposals/m9-public-handoff-simplification.md). |
| Files And ABI Surface | Existing mirror C-VID address arithmetic, video dirty/display consumers, existing video snapshot boundary and owner-local tests; no public frame/API change. |
| Applicable Rules | Execution, documentation, architecture, coding and source-research policy. |
| Verification | Focused read/write/plane/display and snapshot regressions on x86/x64; real Win31 bidirectional DOS transitions; cached dual builds, publish both EXEs, all unit/gate/integration tests including native desktop/audio. Background jobs only. |
| Expected Markers | Identify whether corruption first exists in guest state, original rendering, frame conversion or host presentation. |
| Asset Needs | Read owner snapshot; use task-owned configuration and overlay media only. |
| Reporting Requirements | Separate owner reproduction, local dynamic proof and source deductions; give root cause evidence, proposed files and diff estimate. |
| Stop Conditions | Do not overwrite user assets, infer cause solely from a shared symptom, or terminate long probes at tool observation timeout. |
| Exit Criteria | Minimal single-path repair reviewed, focused and full regressions passed, dual EXEs published, provenance updated, changes committed/pushed; wait for owner acceptance. |
| Original Owner Request | 准入新 S 调研 Win31 全屏 MS-DOS Prompt 经 CAF 切换后窗口字符错乱；使用已保存故障快照，可只读研究 Win3.x。 |
| Similar-Issue Sweep | Review VGA font/plane/palette/stride and video mode transition handling, including snapshot reconstruction. |

Disposable probes belong to build/t99-s33, owned by this executor; review
after 15 minutes without progress and preserve logs until findings are recorded.

## Current Technical Baseline

S32 full cached builds and background/native tests completed; first-run failures
and successful reruns are documented in its verification record.

S33 reproduced the windowed DOS corruption before host Window rendering and
traced it to CHAIN2/unchained video-plane address layout. See the
[investigation and repair boundary](../etc/evidence/softpc/m9-t99-s33-windowed-dos-addressing.md).
The minimal repair and snapshot format boundary are implemented. Both EXEs
are refreshed; x64/x86 each pass 186 background tests and 8 native integration
tests. Actual-diff review and final cold-boot Win31 bidirectional verification passed.
S33 P1 is delivered. The owner rejected P2's identity-call reduction because
all CHAIN2 sites should express one address-space contract. P3 restores the
production/tests/manifest/EXEs exactly to qualified P1; no behavioral fixes are
removed. Owner acceptance is partial: after windowed DOS -> full screen ->
windowed DOS, newly typed characters do not appear although the cursor moves.
The follow-up is reproduced: text BIOS used the physical graphics aperture
instead of the caller's BDA text aperture. Text read/write/scroll now share that
existing mode entry for both bounds. Dual focused tests and the real Win31
input/roundtrip/DIR/CLS probe pass. Final cached builds/publication and all
186 background plus 8 native tests pass on each architecture (388 total).
P4 is ready for owner acceptance after actual-diff review; S33 stays open. Legacy odd dword-fill
value assembly outside the selected CPU path is explicitly recorded in TODO.

## Recent M9 Closures

See [S32 delivery](../history/M9-T99-S32-line-endings.md).

| Task | Closure |
| --- | --- |
| T98 | [Completion audit](../history/M9-T98-completion-audit.md). |

## Recent Governance

Follow existing build cache and task-owned probe cleanup rules.
