# Project Status

## Current Work

M9 T69 S1 is admitted with an owner-revised scope: replace the rejected
source-specific DIB publication workaround with one standalone Compat
display-update transaction.

## M9 T69 S1 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | New |
| Admission And Approval | S1 repaired the independently proven logical Console fill/geometry gap. Its later P2/P3 DIB publication changes reduced severe flicker but owner testing still reports stutter and a two-width Window. Owner explicitly rejected all source-specific branches: “禁止特判，禁止添油战术，找出最合理的干净的方案处理视频问题”, and revised this still-open S1 to replace that path. Standing commit/push approval applies. |
| Objective | Make one source-neutral display-update transaction the sole publication boundary. A DIB bind creates staging state; every display mutation only records damage; the original display update's completion publishes the current geometry, palette and pixels together. No producer identity, DIB size, mode family, palette, pointer, or dirty shape may decide whether a frame is publishable. Retain S1's logical Console buffer/viewport repair. |
| Non-goals | No CLS, guest-application, V7, palette, pointer, Window-size, timing-delay, or fullscreen special case; no forced repaint; no customer image/configuration change; no Lib/Common/VM API change; no duplicate rendering path. Do not add a generalized terminal/scroll system. |
| Reference Baseline | `nt_graph.c` exposes original host update delimiters `nt_start_update()`/`nt_end_update()` through `host_start_update()`/`host_end_update()`, called around normal text and graphics paints. Today `nt_end_update()` is empty. S1 P2/P3 instead classified input sources through `painter_ready`/`invalidate_overlay`, which is explicitly rejected and must be removed. The existing standalone-only V7 geometry branch in `check_win_size()` is also in scope for an evidence-based disposition; it may not remain as an unexamined workaround. |
| Candidate Proposal | [Win3.1 MS-DOS prompt display roundtrip repair](../proposals/m9-win31-display-roundtrip.md) |
| Files And ABI Surface | `src/compat/dib_surface.[ch]` owns staging damage and the one completed-update publication record; `src/compat/v7_pointer.c` may only mutate the current staging surface and report ordinary damage; `src/compat/graphics_console_compat.c` retains the logical Console model. A minimal mechanical host-callback bridge in `src/mvdm/softpc.new/host/src/nt_graph.c` may call Compat begin/end functions from its already-existing host update delimiters; it must not alter guest policy, controller state or original update ordering. Compat-focused tests prove the transaction. No public product ABI changes. |
| Applicable Rules | EXECUTION, ARCHITECTURE, CODING, DOCUMENT and PRODUCT UI. One shared text-surface owner remains required; no presenter or native Console bypass is introduced. |
| Verification | Preserve the existing logical Console fill/geometry proofs. Prove a bind and every mutation type leave no externally consumable dirty frame before the matching end-update; prove one end-update publishes the final bound geometry and accumulated damage once; prove a nested or empty transaction does not create a false frame; prove pointer/palette/render writes obey the same rule without a source tag. Audit all original `host_start_update`/`host_end_update` call sites and every standalone DIB mutation. Run x86/x64 focused tests and full suite; build both package EXEs. |
| Expected Markers | Compat reports the original renderer's current logical Console geometry while retaining its fixed shared text surface. A single transaction record, not producer kind, is the only way for VM frame copying to observe graphics damage. The MVDM mirror diff is limited to calling the standalone host boundary at existing start/end callbacks, with x86/x64 proof and an explicit pristine-ledger disposition. |
| Asset Needs | Existing owner package/media only; no image writes. Refresh only package EXEs, preserving the owner INI. |
| Reporting Requirements | Record removed source-specific branches, changed-path line accounting, every `host_start_update`/`host_end_update` and DIB-mutation call-site disposition, focused proof, full regression and manual Win3.1 route checklist. |
| Stop Conditions | Any need for a Lib/Common/VM API, a second rendering transaction, an MVDM change beyond the two existing host callbacks, or a guest/mode-specific workaround stops S1 for owner direction. Any unrelated Console operation found reachable becomes a separately proposed S. |
| Exit Criteria | Source-specific S1 P2/P3 publication paths are removed; one transaction contract is proven at every admitted mutation path; both widths build/test; only intended Compat/minimal-host-callback/test/docs/package-EXE paths change; owner package INI is preserved. |
| Original Owner Request | “禁止特判，禁止添油战术，找出最合理的干净的方案处理视频问题” |
| Similar-Issue Sweep | Audit all redirected original Console operations plus all original host update delimiters and standalone DIB/palette/pointer mutations. Every hit either participates in the one transaction or has a recorded non-display disposition. |

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

T68 closes the bounded original keyboard-table safety repair. T67's independent
media-policy ledger remains recorded in its T-level audit.
