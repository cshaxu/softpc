# Project Status

## Current Work

Active: T72 S4, native mouse motion repair after owner-assisted diagnosis.
T72 and S4 remain open pending owner acceptance after delivery.

S1 design accepted; S2 implementation and review complete; S3 technical audit
and actual-change review complete. T72 is not formally closed. Both Release packages are
ready for owner testing; no next queued task is admitted.
See [readiness audit](../history/M9-T72-S3-completion-readiness-audit.md).

## M9 T72 S4 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner approved diagnosis, then "批准照此修复，完成后编译测试提交推送，等我测试再收口S". |
| Objective | Replace pointer/recenter-derived motion with native relative motion or successive absolute positions. Remove all recentering and diagnostic logging; preserve capture, buttons, hotkeys and existing copied delta ABI. |
| Non-goals | No edge thresholds, RDP detection branches, second input path, Common/VM/Compat/media/config change; no claim of unlimited absolute-device motion or native desktop acceptance before owner testing. |
| Reference Baseline | 7e080236; clean worktree at admission. |
| Candidate Proposal | [T72 proposal](../proposals/m9-kvm-text-cell-glyph-refactor.md) |
| Files And ABI Surface | Window native mouse.c/h, component.c, root motion.c/h, Types Windows declarations, Window contract documentation and existing focused tests. Public event/API signatures unchanged. Capture exclusively acquires mouse raw registration only if none exists; release removes only its own binding. |
| Applicable Rules | Execution, architecture, coding and documentation rules; design architecture/source layout/UI; linked governance skills. |
| Verification | Both Release builds, strict Lib compile, both full background test presets, focused relative/absolute/capture/failure/duplicate-path tests, shared manifests/DAG/docs gates. Owner runs desktop and RDP after push; no desktop automation while owner works. |
| Expected Markers | WM_INPUT alone produces motion; legacy buttons remain, WM_MOUSEMOVE never produces motion. First absolute sample is baseline; repeated sample zero; relative input continues at clip edge. No SetCursorPos, trace paths or remote-session policy. |
| Asset Needs | Existing owner setup unchanged. Bounded probes used ignored build/t72-rdp, each below 512 KiB, single instance and owner-assisted short runs. Findings retained in S4 record; traces/instrumentation removed. Only packaged x86/x64 EXEs refreshed. |
| Reporting Requirements | Repair estimate six production C/H files +140--200/-45--80, tests +130--200/-60--100 versus 7e080236; report actual counts and reason for necessary ownership/error handling. Documentation/artifacts separate. |
| Stop Conditions | No arbitrary thresholds, undocumented fallback, stealing process registration, desktop interference or media/configuration change. Missing native capability must not silently switch to old broken path. |
| Exit Criteria | Implementation and dual-width proof committed/pushed, worktree clean, diagnostics removed; S4 and T72 stay open until owner accepts desktop/RDP behavior. |
| Original Owner Request | "实测：在本机desktop上测试没问题，只有rdp出问题了"; "可以，开始，你说，我做". |
| Similar-Issue Sweep | All capture/resize/move recenter sites removed; check release/recapture, device/type/desktop geometry switches, raw registration failure/conflict, message cleanup and single motion source. |

Repair delivered/pushed as aba5022c; actual-change review complete. Both full Release builds and focused motion/capture tests
pass. x86 background 105/105; x64 104/105 then corrected documentation gate 1/1.
No runtime test failure; five desktop tests per width not run. Diagnostics removed.
See [S4 evidence and boundaries](../etc/evidence/softpc/m9-t72-s4-native-mouse-motion.md).
Native relative input bypasses host pointer acceleration; absolute-device range
remains finite. Owner will validate desktop/RDP after delivery; S4 remains open.

## Current Technical Baseline

- S4 repair replaces pointer recentering with native motion; public input ABI
  unchanged. Source +148/-44 (net +104), test +142/-34 (net +108). Package hashes
  and exact verification results are in the linked S4 evidence. Await owner
  desktop/RDP testing before closing S4 or T72.
- Code delivery 12bf7c96; S2 actual-change review recorded in
  [S2 history](../history/M9-T72-S2-neutral-text-migration.md).
- Final background suites: x64 105/105 (164.85s), x86 105/105 (147.06s),
  including headless Win3.1 PIF roundtrips, snapshots and restart.
  Five native desktop tests per width were not rerun in S2; no native Linux
  execution or whole-emulator correctness claim.
- VM owns character mapping. KVM Base transports opaque control FIFO and
  latest-wins complete frames; leaf presenters interpret their own frames.
  Window compares frames with its existing surface, not upstream dirty history.
- Renderer readiness, required fonts, terminal request completion, fixed-80 text
  layout and cursor normalization are recorded in the
  [T71 completion audit](../history/M9-T71-completion-audit.md).
- T72 uses neutral parallel text arrays, with device decoding only in VM and
  native Console colour encoding only in Broker. No frame transport change.
- TODO tracking is retired by owner decision, not proof of repair. Owner visual
  acceptance and formal T72 closure remain pending.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T71 | S1--S10 complete; owner approved; final dual-width 105/105 background. | [Audit](../history/M9-T71-completion-audit.md) |
| T70 | S1--S12 complete; owner approved; disclosed Linux limit. | [Audit](../history/M9-T70-completion-audit.md) |
| T69 | S1--S4 complete; reopened cleanup accepted. | [Audit](../history/M9-T69-completion-audit.md) |
| T68 | S1 complete; owner approved. | [Audit](../history/M9-T68-completion-audit.md) |
| T67 | S1--S4 complete; owner accepted. | [Audit](../history/M9-T67-completion-audit.md) |
| T66 | S1--S4 complete; owner accepted. | [Audit](../history/M9-T66-completion-audit.md) |
| T65 | S1 complete; narrowed after audit. | [Audit](../history/M9-T65-completion-audit.md) |
| T64 | S1 audit and narrowed S2 accepted. | [Audit](../history/M9-T64-completion-audit.md) |

## Recent Governance

- **M9 Td S18:** T71 closure audit, TODO retirement and new floppy-identification
  candidate second in queue; documentation only. P1 957c809d pushed;
  actual-change coordinator review and documentation checks passed; Td closed.
  [Record](../history/M9-Td-S18-t71-closure-and-floppy-queue.md).
- **M9 Td S17:** Earlier text/frame proposal subsequently admitted as T71;
  [retained proposal](../history/M9-T71-kvm-text-frame-contract-proposal.md).
