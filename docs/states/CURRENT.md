# Project Status

## Current Work

| Field | Required record |
| --- | --- |
| Identifier Mode | M8 T17 S2 |
| Admission And Approval | Owner authorized sequential SoftPC queue execution through the active code-layout objective on 2026-09-03. |
| Objective | Decompose standalone host implementation into `src/host/{platform,media,video,input,compat,machine}` without changing the recovered SoftPC machine, its firmware, controllers, BOP behavior, or public user experience. |
| Non-goals | No import or copy from `opennt-src-2`; no controller/BIOS/ROM/BOP/CCPU/C-VID semantic change; no generated source or compiler intermediate checked into `src/`; no application-shell move (T18 owns `src/app`). |
| Baseline | T16 completed direct-difference evacuation: all 98 ledger rows have current paths, `src/mvdm/softpc.new/` has zero local-standalone rows, and GCC x64/x86 both passed 20/20 CTest. |
| Affected Boundary | Standalone host code is progressively separating from the platform aggregate beneath `src/host/`; recovered source and firmware stay together under `src/mvdm/softpc.new/`, including `roms/`. |
| Subtask Plan | S1 inventory direct owners and route a minimal host taxonomy; S2 move platform/machine/media/video/input/compat files without content edits; S3 update CMake/tests and establish one implementation owner per host callback; S4 verify x64/x86 media, renderer, input, timer, BOP, and package behavior. |
| Requirement Ledger | R1: every standalone host source has one taxonomy owner. R2: host owns no guest RAM, controller state, or original renderer algorithm. R3: the recovered SoftPC tree remains reference-shaped. R4: no object/library/intermediate output is retained. |
| Focused Verification | CMake selected-input audit; static ownership sweep; x64/x86 builds and focused controller/FDC/HDD/dual-media/BOP/serial/printer/audio/timer/V7/mouse tests. |
| Stop Conditions | Stop for owner direction if a move needs machine semantic change, guest policy, a new runtime facility, or unproved ABI representation. |
| Exit Criteria | No mixed standalone-platform aggregate remains; all new host code is beneath the host taxonomy; host code has no direct ownership of guest machine state; x64/x86 regressions match the T16 baseline. |
| Original Owner Request | Persistent objective: “单人双角色模式执行SoftPC 的队列任务：代码布局优化。” T17 is the active ordered queue task. |

## Current Technical Baseline

- The fixed standalone package is `artifacts/binary/softpc32.exe` and
  `softpc64.exe`, with adjacent `softpc.ini` and guest media below
  `artifacts/media/`.
- M8 T13 separates `test/unit`, `test/integration`, and `test/support`; clean
  GCC x64/x86 CTest was 19/19 for each host width.
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
  12 historical tasks end at T12, the completed test-boundary work is M8 T13,
  and the queued work begins with M8 T14. Td governance work does not consume
  a build-version number.
