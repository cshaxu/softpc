# Project Status

## Current Work

| Field | Required record |
| --- | --- |
| Identifier Mode | M9 T21 closed |
| Admission And Approval | Owner directed implementation of queued `Ctrl+Alt+F` guest Alt+Enter in both console and window, with console help listing every Ctrl+Alt command on its own line. |
| Objective | Translate `Ctrl+Alt+F` into the guest's physical Alt+Enter make/break sequence through the existing runtime keyboard path. |
| Non-goals | No host fullscreen action; no change below `src/app`; no change to SoftPC, BIOS, BOP, controller, timing, capture policy, or existing Ctrl+Alt commands. |
| Baseline | T20 is closed and dual-width verified. Console/window already reserve Ctrl+Alt+P/D/M and use the shared keyboard normalizer. |
| Affected Boundary | `src/app/{keyboard,console,window,main}` and focused keyboard/frontend tests. |
| Subtask Plan | S1 add shared Alt+Enter injection; S2 reserve it in both frontends and rewrite monitor help; S3 dual-width proof and package refresh. |
| Requirement Ledger | R1: guest receives Alt make, Enter make, Enter break, Alt break. R2: host Ctrl+Alt+F and its release do not leak to the guest. R3: P/D/M behavior stays unchanged. R4: console help is one Ctrl+Alt command per line. |
| Focused Verification | Keyboard sequence smoke, frontend hotkey handling, help-text assertion, full GCC x64/x86 CTest and package smoke. |
| Stop Conditions | Stop if frontend routing requires direct machine or controller access, or if Alt+Enter conflicts with a documented existing host action. |
| Exit Criteria | Both frontends inject guest Alt+Enter without host window changes; help is complete and multiline; both package builds and regressions pass. |
| Closure | Completed: Ctrl+Alt+F is consumed in console and window, injects Alt+Enter through the shared original-keyboard route, and console help lists P/D/F/M one per line. Fresh GCC x64/x86 builds and CTest 20/20 each passed. |
| Original Owner Request | “ctrl+alt+f可以都在console和window模式里给客户机发送alt+enter” and “修改console，列出所有ctrl+alt命令，分行显示”. |

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
