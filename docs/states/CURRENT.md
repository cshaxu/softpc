# Project Status

## Current Work

| Field | Required record |
| --- | --- |
| Identifier Mode | M8 T19 S1 |
| Admission And Approval | Owner authorized sequential SoftPC queue execution through the active code-layout objective on 2026-09-03; T17 was explicitly approved for push on 2026-09-04. T19 is the next ordered implementation task. |
| Objective | Remove the remaining transitional source-layout routes only after proving every retained source, overlay, host, app, test, firmware, and artifact path has a durable owner. Publish the final source/diff ledger and source-layout documentation. |
| Non-goals | No import or copy from `opennt-src-2`; no controller/BIOS/ROM/BOP/CCPU/C-VID semantic change; no generated source or compiler intermediate checked into `src/`; no deletion based solely on difficult classification. |
| Baseline | T18 moved all ten app-shell files to `src/app/`, removed `src/vm/`, updated all 98 direct-difference ledger rows, and passed full GCC x64/x86 CTest 20/20. The original BIOS/VGA/CMOS ROMs remain embedded from `src/mvdm/softpc.new/roms/`. |
| Affected Boundary | Transitional `src/core/softpc` routing, obsolete CMake references, source-layout documentation, and the final direct-difference ledger only. Recovered machine/firmware and the approved `src/{mvdm,overlay,host,app}` owners remain unchanged unless an auditable structural move is necessary. |
| Subtask Plan | S1 inventory residual transitional paths and classify each against the ledger; S2 remove only obsolete routing or relocate retained sources with `git mv`; S3 make final source-boundary/ledger invariants executable; S4 run clean x64/x86 builds, full CTest, package, media, text-to-graphics, V7 cursor/mouse, and presentation proof. |
| Requirement Ledger | R1: no source file remains beneath transitional `src/core/softpc` or `src/vm`. R2: every original-source difference is patch-visible in the final ledger. R3: all repository-owned code belongs to recovered machine, overlay, host, app, or test. R4: no compiler output is tracked under source paths. |
| Focused Verification | Source-tree and CMake audit; ledger regeneration; clean GCC x64/x86 builds and complete CTest; A:, C:, and dual-media startup; Windows 3.1 text-to-graphics handoff; V7 cursor/mouse; console/window presentation; artifact-package launch proof. |
| Stop Conditions | Stop for owner direction if a residual path needs a machine semantic change, guest policy, a new runtime facility, an unproved ABI representation, or deletion without a proven replacement/owner. |
| Exit Criteria | No source remains under transitional `src/core/softpc` or `src/vm`; all original-source differences are patch-visible; all new code has a host or app owner; x64/x86 builds/tests and package behavior satisfy the recorded verification. |
| Original Owner Request | Persistent objective: “单人双角色模式执行SoftPC 的队列任务：代码布局优化。” T19 is the active ordered queue task. |

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
