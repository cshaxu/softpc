# M9 T69 completion audit

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

T69 is closed. The remaining candidates are listed in
[Queue](../states/QUEUE.md).
