# M9 T72 S4: Native Mouse Motion Delivery Evidence

## Admission And Evidence

Owner approved repair, dual-width build/test/commit/push, then manual testing
before S closure. Baseline 7e080236. This explicitly extends the text-only T72
scope into Window input; it does not change the text schema or other components.

The T70 recenter path sampled pointer position rather than native motion.
Repeated positions produced repeated displacement. Owner-assisted bounded probes
showed 514 local raw records, all relative, and 341 RDP raw records, all absolute
with virtual-desktop flags. RDP X 40209 -> 40072 -> 39936 decreased while the old
path emitted positive X. This supports removing recenter-derived displacement,
not a general claim about every RDP device. Earlier diagnostic compilation found
a missing scalar include, corrected before both diagnostic builds passed. The
repair replaces that uncommitted probe; no logging remains in production.

## Finite Repair And Similar-Issue Ledger

| Boundary | Implementation / proof |
| --- | --- |
| One motion producer | WM_INPUT alone; legacy WM_MOUSEMOVE ignored. Mock verifies no second read/output. Buttons retain their existing path with zero motion. |
| Relative motion | Existing signed remainder scaling accepts raw delta directly. Repeated packets continue beyond clip limits; positive/negative remainder tests. |
| Absolute motion | Desktop-unit conversion then successive differences; first/repeated positions emit zero. Recorded RDP direction sequence tested. |
| Baseline lifetime | Capture/release, move/resize, device, relative/absolute, virtual/primary and desktop-size changes covered. |
| Native ownership | Existing registration declined; only own binding removed. Query/register/remove failure and foreign replacement tested. External concurrent registration changes prohibited by documented contract. |
| Loss/failure | Existing release path for clip/capture loss, deactivation and raw read failure; malformed packets rejected; no fallback to legacy motion. |
| OS message cleanup | WM_INPUT always delegates to DefWindowProc after handling. No new worker or message bridge. |
| Hotkeys/freeze/buttons | Existing matcher and final input filter unchanged; capture tests retain freeze, release, retirement and button checks. |
| Same-class sweep | No SetCursorPos/recenter sites or diagnostic/RDP policy remain in Lib/test sources. No Common/VM/Compat/MVDM/media/config source changes. |

Estimate: six production C/H +140--200/-45--80; test +130--200/-60--100.
Actual against baseline: seven production C/H +148/-44, net +104 (the seventh
is public-header ownership documentation); one test C +142/-34, net +108.
Extra code handles native records and process-wide registration, not another
input architecture. Manifests/docs/binaries are counted separately.

## Verification And Delivery

Both Release full builds passed; Lib uses strict warning-as-error flags.
Focused x64 capture and motion tests passed 2/2. Full background x86 passed
105/105 (150.75s). The x64 full run passed 104/105 (169.13s): only documentation
governance failed because the open S delivery record was initially named as a
closed history step. Moved that evidence to the indexed evidence subtree; the
governance test rerun passed 1/1 (0.20s). No production/test change was needed.
Thus all 105 cases per width have passing results, not two first-pass green runs.
Manifest, component DAG and documentation checks passed. Five native desktop
tests per width are deliberately excluded to avoid interfering with owner input;
no visual acceptance or Linux runtime claim.

| Package | Bytes | SHA256 |
| --- | ---: | --- |
| softpc32.exe | 3662203 | A3AB8E85856BA5AEBA551155A393E91257F68B8471BF7E175B0B0952EE197E59 |
| softpc64.exe | 3065207 | 31D7D25CB37529F300E47990B25BA7930EF71421F847A3A138025E15BA9F9E0B |

Bounded diagnostic CSV files were removed after retaining findings; no processes
were running at cleanup. Existing build trees retained. Owner INI/media untouched.

Native relative records bypass Windows pointer acceleration, so owner must test
local sensitivity as well as RDP direction/stability. Absolute input has finite
device coordinates; unlimited absolute-device movement is not claimed. Raw mouse
registration is exclusive during capture; external registrants must coordinate.
S4 and T72 remain open pending owner testing after delivery.

## Post-Push Actual-Change Review

Reviewed delivered aba5022c against 7e080236 after switching from executor to
coordinator. All 17 changed paths belong to the admitted source/test/docs/package
set. Recomputed C/H counts match +148/-44 and +142/-34. Reviewed native ownership,
packet bounds, scale extraction, release/failure calls, button ordering and
DefWindowProc cleanup against the finite ledger; no parallel motion sender or
recenter remains. Public signatures are unchanged. Protected component and INI
diffs are empty. Both package hashes match this record; HEAD and origin/main
matched with a clean worktree after P1 push. This is a technical delivery review,
not owner desktop/RDP acceptance and not S4 closure.
