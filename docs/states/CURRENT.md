# Project Status

## Current Work

| Field | Required record |
| --- | --- |
| Identifier Mode | M8 T14 S1 |
| Admission And Approval | Owner admitted the next ordered queue item on 2026-09-03. |
| Objective | Produce a rerunnable, source-only pristine-map and divergence manifest for the current recovered SoftPC tree against the selected OpenNT `mvdm/softpc.new` comparison tree. |
| Non-goals | No source move, overlay creation/application, CMake change, generated-source change, firmware/media change, controller/BIOS/CCPU/C-VID/BOP semantic change, or new host/product capability. |
| Baseline | `daa565d` established the staged standalone package and T13 test boundary; `7add9fe` documented the product identity. `src/core/softpc/` remains the transitional recovered tree; the selected comparison root is read-only `O:\repos.external\opennt-src-2\nt\private\mvdm\softpc.new`. |
| Affected Boundary | Read-only inventory of the transitional recovered tree, its build-selected source/header/resource inputs, and the selected external comparison tree. The deliverable is evidence and a proposed disposition only. |
| Subtask Plan | S1 enumerate selected comparison roots and exclusions; S2 generate path/hash classification; S3 manually review every non-identical controller, BIOS, CCPU, C-VID, and `nt_*` entry; S4 publish the manifest and review evidence. |
| Requirement Ledger | R1: classify every relevant path as identical original, direct original diff, omitted original, local standalone, generated input, or generated output. R2: record a proposed overlay, host/app, remove, or unresolved disposition for every direct diff. R3: record selected source revision and excluded binary/object inventory. R4: no path may be described as original if it differs. |
| Focused Verification | Re-run the manifest without builds; inspect every required non-identical family; validate source paths against CMake's selected inputs; run the documentation gate for the evidence/index changes. |
| Full Regression | No code or build behavior may change. If a verification helper is added, run its deterministic self-check only; x64/x86 build and CTest are deferred to T15/T16 where compilation routing changes. |
| Similar-Issue Sweep | Scan headers, scripts, resources, generated inputs, build manifests, and checked-in object/library files—not only `.c` files—so that non-source build inputs are classified or explicitly excluded. |
| Stop Conditions | Stop for owner direction if the selected external baseline is ambiguous, comparison reveals a semantic discrepancy needing a machine decision, or completing the inventory would require copying/importing source or modifying a recovered file. |
| Exit Criteria | A rerunnable manifest identifies selected roots and exclusions, classifies all scoped paths, proposes a disposition for every direct diff, includes manual-review evidence for the required source families, and makes no source/build behavior change. |
| Original Owner Request | “准入队列的下一个任务”. The next candidate is M8 T14, pristine-map and divergence manifest. |

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
- Implementation task identifiers are repository-wide build versions: the
  12 historical tasks end at T12, the completed test-boundary work is M8 T13,
  and the queued work begins with M8 T14. Td governance work does not consume
  a build-version number.
