# Project Status

## Current Work

| Field | Required record |
| --- | --- |
| Identifier Mode | M9 T25 closed |
| Admission And Approval | Owner reported the next priority bug: guest Ctrl+Alt+Del works, but its warm restart hangs at `Starting MS-DOS ...`. |
| Objective | Restore a guest Ctrl+Alt+Del warm restart that continues through DOS startup by repairing only the standalone lifecycle boundary that violates the original machine's reset contract. |
| Non-goals | No DOS, DPMI, NTVDM, WOW, VDD, or frontend-only reboot implementation; no change to selected BIOS/ROM/BOP/controller behavior, guest media, executable names, or user-owned `softpc.ini`. No source change below `src/mvdm/softpc.new/` absent a demonstrated port-ABI need. |
| Baseline | Cold boot is functional and Ctrl+Alt+D demonstrably reaches the guest. The resulting warm boot reaches `Starting MS-DOS ...` then stops progressing. M9 T24 closed at `34975cd` with GCC x64/x86 20/20 CTest proof. |
| Applicable Rules | Documentation, execution, architecture, and coding rules; source layout; the original mirror remains a preserved baseline and OpenNT is read-only comparison material. |
| Affected Boundary | The app keyboard hotkey route, input queue, executor/reset lifecycle, and required host reset callbacks below `src/{app,host}` and `test/`; original machine imports remain intact. |
| Subtask Plan | S1 trace cold and guest-warm lifecycle state including modifiers, timer/event and media controller boundaries; S2 add a bounded regression and implement the narrow boundary repair; S3 run dual-width GCC build, full CTest and package smoke, then close. |
| Requirement Ledger | R1: Ctrl+Alt+D sends guest Ctrl+Alt+Del with no stranded host or guest modifiers. R2: warm reset keeps the original keyboard/reset hardware as the owner. R3: the post-reset execution path continues beyond the DOS startup banner. R4: no product-service semantics or user configuration/media mutation is introduced. |
| Focused Verification | Cold-versus-warm lifecycle trace; bounded non-artifact regression; GCC x64/x86 builds; full CTest; package smoke for both launchers. |
| Stop Conditions | Stop before substituting an app-only reboot for guest hardware reset, changing guest media, or modifying original mirrored source without an evidenced port-ABI requirement. |
| Exit Criteria | Guest Ctrl+Alt+D warm restart progresses beyond `Starting MS-DOS ...` under the fixed machine profile, with dual-width regression proof and no new guest-semantic implementation. |
| Closure | Closed. `reboot()` had set only the SoftPC reset classification flag, leaving CCPU executing the pre-reset stream. It now asserts the original CCPU hardware reset line (`CPU_HW_RESET`), which restarts at the ROM reset vector and leaves BIOS/controller initialization to the original machine. GCC x64/x86 rebuilt and passed full CTest, 20/20 each; package smoke passed at both widths. |
| Original Owner Request | “下一个bug：按了ctrl alt d以后确实起作用，但是热重启的机器，死机在 Starting MS-DOS ...” |

## Current Technical Baseline

- The fixed standalone package is `artifacts/binary/softpc32.exe` and
  `softpc64.exe`, with adjacent `softpc.ini` and guest media below
  `artifacts/media/`.
- M8 T19 completes the source layout as `src/{mvdm,overlay,host,app}` and
  removes transitional `src/core` and `src/vm` routes. Fresh GCC x64/x86
  builds each passed full CTest, 20/20.
- The selected original ROM inputs are byte-identical to OpenNT and embedded
  from `src/mvdm/softpc.new/roms/`; no runtime ROM artifact root is active.

## Recent Governance

- M9 T23 retired 294 demonstrably unselected historical paths (15 C files and
  279 headers) from the original mirror without changing its selected machine
  behavior; fresh GCC x64/x86 CTest each passed 20/20.
- M9 T24 closed with dual-width 20/20 CTest proof and package smoke. The next
  reported issue is a guest Ctrl+Alt+Del warm-reset hang after `Starting
  MS-DOS ...`; it requires a dedicated lifecycle investigation.
- M9 T25 closed by reconnecting the original keyboard `reboot()` callback to
  the CCPU hardware reset line. This is a machine reset, not a DOS/DPMI or
  frontend restart; dual-width GCC CTest passed 20/20 at closure.
- M9 T22 normalizes frontend host hotkeys without changing SoftPC: Ctrl+Alt+P
  no longer strands guest modifiers across console pause/resume, and
  Ctrl+Alt+F now supplies guest Alt+Enter rather than Ctrl+Alt+Enter.
- M9 Td S1 makes `artifacts/binary/softpc.ini` permanently user-owned package
  configuration: agents may refresh the executable pair but cannot modify,
  stage, or commit that INI.
- M8 Td S1 established the NXVM-style authority topology, linked the four
  applicable shared governance skills, and added the documentation gate.
- M8 Td S2 established the public product identity as Insignia SoftPC and
  added current, owner-provided SoftPC product captures to the root README.
- M8 T14 established a deterministic, 1,222-row source-map against the
  selected OpenNT revision. It found 60 direct differences and assigned every
  one a future overlay, host, or removal disposition without altering code.
- Implementation task identifiers are repository-wide build versions: the
  12 historical tasks end at T12 and M8 completes at T19. Td governance work
  does not consume a build-version number.
