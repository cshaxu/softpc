# M9 T69 completion audit

> The initial closure below is retained as historical evidence. It was
> superseded by the owner-reopened S2/S3 investigation. The authoritative final
> closure is recorded at the end of this document.

Owner request: fix the Win3.1 MS-DOS Prompt display route without any
mode-/application-specific workaround: initial windowed Prompt width must not
alternate; guest fullscreen must retain a correct frame; and `cls` must keep
clearing text after fullscreen/windowed transitions.

## Finite closure ledger

| Surface | Disposition | Evidence |
| --- | --- | --- |
| Completed graphics-frame publication | Repaired | Compat publishes only at the existing outer `host_end_update()` boundary. DIB bind, painter, palette and pointer changes only mutate the staging surface. |
| V7 graphics geometry | Repaired | `check_win_size()` derives proprietary byte-painter width from current V7 controller/latch state, never the historical BIOS request. |
| Detached text clear | Repaired | Standalone no longer applies the original native-Console `FULLSCREEN` early return before its sole Compat text-surface clear. |
| Detached cursor metadata | Repaired | The same host-only guard no longer suppresses the Compat cursor update. |
| KVM Window, VM copied-frame path and Common | Retained | Audit shows each only copies or consumes completed source facts; no debounce, geometry inference, retry, second renderer or API change was added. |
| Compat scroll stub | Retained | Original `nt_scroll_up/down()` return before calling it in the selected standalone build; it is not a reachable cause of this route. |

## Actual accounting and boundaries

The retained production change is limited to the original renderer and its
narrow Compat host contract: one transaction bridge in `host/src/nt_graph.c`,
current V7 mode decoding in `base/video/v7_video.c`, reuse of that decode in
`base/video/ega_vide.c`, and standalone detached-Console policy guards in
`host/src/nt_graph.c`. `egavideo.h` carries the internal declaration. No Lib,
Common, App, VM public interface, configuration, guest media or presenter
policy changed.

The MVDM mirror ledger records every retained difference and its port-ABI
reason in [the current divergence ledger](../etc/evidence/softpc/pristine-divergence-current.md).
The test additions are confined to `test/unit/vga_frame_smoke.c`: completed
transaction damage, V7 historical-BIOS/current-controller disagreement,
fullscreen text clear and fullscreen cursor metadata. Package EXEs were
rebuilt with these source commits; the owner INI was not touched.

## Verification and delivery

- The focused `softpc-vga-frame-smoke` passed on x64 and x86, including the
  V7 width, detached fullscreen clear and cursor cases.
- Final x64 core suite: 106/106 passed. The two package-environment smoke
  tests remain excluded from that core count, as in prior task records.
- Final x86 core suite: 106/106 passed. An earlier combined run retained an
  old `LastTestsFailed.log` entry for VGA; immediate standalone replay and the
  subsequent complete x86 core run passed, so it is not attributed to T69.
- `git diff --check` passed and the worktree is clean.
- The owner reported the restored display route works after the final package
  update.

This closure record was superseded by the owner's later reproduction of the
initial-windowed Win3.1 width alternation. T69 is therefore owner-reopened as
S2; its active contract is in `states/CURRENT.md`. S2 traced the alternating
completed DIBs to the original renderer's one-bit EGA/VGA settle gate: a later
register write could not restart the already-running interval. The repair adds
one private core generation and makes the existing gate wait from the final
controller update; its focused x86/x64 proof is recorded in the active packet.
Manual Win3.1 verification remains required before another task-level closure.

## Final T69 closure

The owner has accepted the final repaired Win3.1 display route and approved
T69 closure after S4's finite accounting audit.

| Required outcome | Final disposition | Evidence |
| --- | --- | --- |
| Complete-frame publication | Kept | Compat commits deferred damage only at the original outer update boundary; no producer, PIF, mode or timing filter remains. |
| Packed display width | Kept | The selected packed painter's one-byte/one-pixel row contract supplies width; the old BIOS bookkeeping override cannot publish a 1280-wide frame for the same packed state. |
| Fullscreen/windowed text state | Kept | Detached text clear and cursor metadata continue through the single Compat text surface. |
| Scope boundary | Kept | Lib, Common and public product interfaces remain unchanged; KVM consumes copied complete frames only. |
| Further work | Deferred explicitly | The intermittent outer Window-height observation and two verified simplification candidates are recorded in `TODO.md` with admission conditions. |

The S4 ledger compares `86de8eb..662f7d4`: production is `+283/-79` (net
`+204`), tests/build `+359/-2` (net `+357`); owner media, INI and package
artifacts are excluded. The candidate cleanup is deliberately not folded into
this closure: it would restore obsolete preserved-MVDM helper churn and remove
dead Compat wrappers in a separately bounded task.

Verification retained for this closure:

- focused VGA/text smoke passed on both x64 and x86 during the accepted repair;
- the snapshot regressions were rerun after the final audit: all four focused
  snapshot tests passed on x64 and all four passed on x86;
- `git diff --check` and the documentation-governance gate pass for this
  closure; and
- the owner manually accepted the final package behavior. No package rebuild
  is claimed for this documentation-only closure.
