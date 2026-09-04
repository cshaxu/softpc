# Project Status

## Current Work

| Field | Required record |
| --- | --- |
| Identifier Mode | M9 T22 closed |
| Admission And Approval | Owner reported that console pause/resume leaves the guest keyboard unresponsive and that Ctrl+Alt+F fails to toggle the Win3.1 MS-DOS Prompt back to its graphics window. |
| Objective | Make host hotkeys leave no guest modifier held, and make Ctrl+Alt+F deliver guest Alt+Enter rather than guest Ctrl+Alt+Enter. |
| Non-goals | No SoftPC/CCPU/controller/BIOS/BOP change; no host fullscreen action; no synthetic frontend switch; no agent edit to `softpc.ini`, media, or guest. Owner-provided INI changes ship with this task. |
| Baseline | Console and window both submit ordinary Ctrl and Alt transitions before receiving a reserved chord. Console exits its input loop on pause, so the later host modifier key-up records are not delivered. |
| Affected Boundary | `src/app/{keyboard,console,window}`, CMake package-output ownership, and focused app-input tests only. |
| Subtask Plan | S1 normalize reserved-chord modifier release; S2 apply it before console/window pause and Alt+Enter delivery; S3 dual-width test and package refresh. |
| Requirement Ledger | R1: after Ctrl+Alt+P then resume, ordinary console keys reach the same runtime queue. R2: Ctrl+Alt+F produces guest Alt+Enter with Ctrl released before Enter. R3: no host window action or machine-state shortcut replaces the guest transition. R4: P/D/F/M all clear their already-forwarded host Ctrl/Alt modifiers before their respective action. |
| Focused Verification | Keyboard transition-order unit checks; runtime queue pause/resume proof; full GCC x64/x86 CTest and package smoke. |
| Stop Conditions | Stop if remediation requires a machine/controller change or if the original input contract cannot accept ordinary modifier releases. |
| Exit Criteria | Console pause/resume remains keyboard-live, Ctrl+Alt+F reaches the guest as Alt+Enter, regressions pass on both widths, executables are refreshed, and any owner-provided INI change ships alongside them. |
| Closure | Completed: P/D/F/M now neutralize the already-forwarded host Ctrl/Alt modifiers in both frontends. P can no longer strand modifiers across a console pause/resume, and F clears the host chord before injecting a fresh guest Alt+Enter. CMake no longer copies a template over the user-owned package INI. Fresh GCC x64 and x86 builds each passed full CTest, 20/20. |
| Original Owner Request | “pause机器以后resume进去，机器无法接受键盘输入” and “Ctrl+Alt+F … 应该让win3.x的msdos prompt变成窗口模式”. |

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
