# M9 T81 S6: Non-running Window Creation Admission

## Contract

A paused Window is a retained view, not a reason to create a new presenter.
Accordingly, Session may issue `CREATE_WINDOW` only when its actual runtime
fact is `RUNNING`. This applies independently of display mode and frame type.
It leaves the existing desired presentation plan intact, so an existing Window
still remains visible and freezes while paused.

The entire production change is the one action-derivation guard in
`common/session/control_state.c`. It neither introduces state nor changes a
public contract: the existing `window_actual` fact is sufficient. The
independent `window_suppressed` state remains responsible for a user-closed
Window while an in-flight destroy is reconciled.

## Similar-Issue Sweep

The admitted search was:

```powershell
rg -n "COMMON_UI_ACTION_CREATE_WINDOW" src/common test/common -g "*.c" -g "*.h"
```

`common_session_state_next_action()` is the sole production emitter. Common UI
only executes a Session-supplied action, and test-only direct UI calls are not
state derivation paths. The new matrix covers `INIT`, `STOPPED`,
`RESET_COMPLETED` (normalized to paused presentation), `PAUSED` and `ERROR`
for both Console and Window display modes, with an available graphics frame.
Existing coverage retains an already-created Window through reset/pause.

## Actual Change And Proof

Against `a2fbf548`, implementation C is +4/-1 (net +3), test C is +32/-0
(net +32), and the two corpus manifests replace one hash each (+2/-2). The
implementation is smaller than the estimate's upper bound because no new
helper, state flag, callback or plan variant was needed.

- Direct state-matrix smoke passes on x64 and x86.
- Common source and test manifests verify.
- Documentation governance and `git diff --check` pass.
- Background product regression passes 119/119 on x64 and 119/119 on x86;
  desktop-labelled tests remain excluded by the prescribed presets.

| Package | SHA256 |
| --- | --- |
| [softpc32.exe](/O:/repos.hobby/softpc/assets/binary/softpc32.exe) | EF3110FCEBA20F7A267E1712324CCB517A87410A9810D414C1994D9B999A5D5F |
| [softpc64.exe](/O:/repos.hobby/softpc/assets/binary/softpc64.exe) | 40B8C3D964442773B0BB491EA78A780834CE4FC22FC41FD88A15BB0B9E8CA8D9 |

Coordinator review of `37562abb` confirms that the one production condition is
the sole `CREATE_WINDOW` emitter, matches the admitted invariant, preserves
paused retention, and contains no out-of-bound path. S6 is closed. T81 remains
open: S5 still needs the independent audible PC-speaker acceptance, followed
by the required whole-task closure audit.
