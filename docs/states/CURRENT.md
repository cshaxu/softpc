# Project Status

## Current Work

| Field | Required record |
| --- | --- |
| Identifier Mode | M9 T23 closed |
| Admission And Approval | Owner admitted the next queued task: retire demonstrably unused source from the selected original MVDM mirror. |
| Objective | Reduce `src/mvdm/softpc.new` to the original source subset actually selected by the fixed standalone machine, without changing machine behavior. |
| Non-goals | No rewrite, semantic edit, or removal of active CCPU/C-VID/V7/BIOS/BOP/controller code; no host-ABI, media, ROM, guest, UI, or `softpc.ini` change. |
| Baseline | The approved proposal identifies 15 unselected C files (4,921 lines) and 279 header candidates. Only the C list is pre-confirmed; every header requires exact x64/x86 compiler dependency proof. |
| Applicable Rules | Documentation guide and execution rules; source-layout, architecture, and coding rules; original mirror remains a selected baseline and OpenNT stays read-only comparison material. |
| Affected Boundary | Selected inactive paths below `src/mvdm/softpc.new`, CMake/transform reference audits, source-boundary evidence, and test documentation only. |
| Subtask Plan | S1 regenerate exact selection/dependency evidence for both widths; S2 remove only proven inactive C/header paths and repair stale references; S3 fresh dual-width build, full CTest, package smoke, and source-map closure. |
| Requirement Ledger | R1: each removed C path is absent from CMake and transformation inputs. R2: each removed header is absent from exact compiler dependency output for both widths. R3: active original source spelling/behavior remains unchanged. R4: dual-width behavior and package launch remain unchanged. |
| Focused Verification | Exact path audits, x64/x86 dependency manifests, CMake stale-reference scan, fresh GCC x64/x86 builds, full CTest, and package smoke. |
| Stop Conditions | Stop before deleting a path referenced by CMake, a transform, a compiler dependency manifest, or any active source. Defer ambiguous headers rather than guessing. |
| Exit Criteria | Every pre-confirmed inactive C file is gone; only headers with exact dual-width proof are gone; no stale build/reference path remains; and dual-width regression preserves current packages. |
| Closure | Closed 2026-09-04. Removed all 15 pre-confirmed inactive C files and all 279 manifest headers after exact fresh x64/x86 dependency proof found zero consumers. Fresh GCC x64/x86 builds and full CTest each passed 20/20, including package smoke, source-boundary, and documentation-governance gates. |
| Original Owner Request | “好 准入” following the approved M9 unused-MVDM-source-retirement proposal. |

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
