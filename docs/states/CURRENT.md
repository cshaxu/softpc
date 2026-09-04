# Project Status

## Current Work

| Field | Required record |
| --- | --- |
| Identifier Mode | M8 T15 S1 |
| Admission And Approval | Owner authorized sequential SoftPC queue execution through the active code-layout objective on 2026-09-03. |
| Objective | Establish the selected OpenNT source mirror, mirrored overlay-patch layout, manifest enforcement, and build-tree source composition so the compiler no longer reads a directly edited original file from the mirror. |
| Non-goals | No controller/BIOS/ROM/BOP/CCPU/C-VID semantic change; no new machine capability; no generated source checked into `src/`; no relocation of standalone host/app implementation (T17/T18 own those). |
| Baseline | T14 (`a072bb8`) establishes the selected OpenNT revision and the 60 direct-difference ledger. The current tree is `src/core/softpc/`; 39 direct rows propose overlay, 17 host, and four unselected XMS/allocator removals. |
| Affected Boundary | Source provenance, CMake build routing, and ignored build-tree composed copies. The original external tree is comparison/import input for this admitted mirror task only; it is never a runtime dependency. |
| Subtask Plan | S1 create a byte-accounted pristine mirror and mirrored patch ledger; S2 compose selected sources into `build/`; S3 route CMake to composed inputs and fail closed on stale/missing patch mapping; S4 prove unpatched hashes and x64/x86 regression parity. |
| Requirement Ledger | R1: mirror path matches OpenNT relative layout. R2: each patch path mirrors its original and has manifest owner/reason/defines/x86/x64 evidence fields. R3: composition fails when an applied patch is stale or unregistered. R4: compiler selects composed rather than directly edited mirror sources. R5: unpatched composed hashes equal pristine source hashes. |
| Focused Verification | Hash check for mirror and unpatched composed paths; deliberately invalid patch/manifest fixture; static CMake selected-input audit; then x64/x86 build and focused machine/renderer/input/timer/media/BOP tests. |
| Stop Conditions | Stop for owner direction if a required direct diff cannot be represented as a reversible patch, if current behavior requires an unproved representation change, or if the target source layout would require a machine-semantic decision. |
| Exit Criteria | A byte-accounted original mirror and checked patch composition produce the existing build on x64/x86; no compiled original comes from a directly edited mirror file; every selected patch is manifest-visible; focused regression matches the T14 baseline. |
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
