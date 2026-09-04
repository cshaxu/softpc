# Project Status

## Current Work

| Field | Required record |
| --- | --- |
| Identifier Mode | M8 T16 S1 |
| Admission And Approval | Owner authorized sequential SoftPC queue execution through the active code-layout objective on 2026-09-03. |
| Objective | Classify and evacuate the remaining direct standalone adaptations from the recovered SoftPC tree without copying source from the external OpenNT reference or changing machine behavior. |
| Non-goals | No import or copy from `opennt-src-2`; no controller/BIOS/ROM/BOP/CCPU/C-VID semantic change; no generated source or compiler intermediate checked into `src/`; no standalone-host/application decomposition (T17/T18 own those). |
| Baseline | T15 relocated the repository-owned recovered SoftPC base/host tree to `src/mvdm/softpc.new/`; GCC x64/x86 both rebuilt and passed 20/20 CTest. T14 (`a072bb8`) retains the 60 direct-difference dispositions. |
| Affected Boundary | Repository-owned direct-difference ledger, source-local compatibility adaptation, and CMake routing. The external OpenNT tree remains read-only comparison material only. |
| Subtask Plan | S1 remap the T14 ledger from the former source paths to the T15 layout and identify every active direct adaptation; S2 remove adaptations whose original code now builds; S3 relocate remaining standalone-only adaptation to its rightful existing compatibility boundary without copying reference content; S4 rebuild and prove x64/x86 regression parity. |
| Requirement Ledger | R1: each direct-difference row has a current path and disposition. R2: no external reference file or generated patch enters the repository. R3: machine algorithms remain in the recovered source tree; standalone adaptations are explicitly owned outside it or by a small repository-owned compatibility patch. R4: no object/library/intermediate output is retained. |
| Focused Verification | Ledger/path audit; static source-ownership sweep; CMake selected-input audit; then x64/x86 build and focused machine/renderer/input/timer/media/BOP tests. |
| Stop Conditions | Stop for owner direction if a proposed evacuation changes machine semantics, needs an unproved ABI representation change, or would require copying content from the external reference. |
| Exit Criteria | Every active direct adaptation has a current owner and visible disposition; no standalone-only behavior remains hidden in recovered-machine code; x64/x86 regression matches the T15 baseline. |
| Original Owner Request | Persistent objective: “单人双角色模式执行SoftPC 的队列任务：代码布局优化。” T15 is the next ordered queue task. |

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
