# Project Status

## Current Work

| Field | Required record |
| --- | --- |
| Identifier Mode | M8 T15 S1 |
| Admission And Approval | Owner authorized sequential SoftPC queue execution through the active code-layout objective on 2026-09-03. |
| Objective | Move the repository's existing recovered SoftPC sources from `src/core/softpc/` into `src/mvdm/softpc.new/`, preserving their original relative paths and names so the tree can be compared path-by-path with the T14 read-only reference. |
| Non-goals | No import or copy from `opennt-src-2`; no controller/BIOS/ROM/BOP/CCPU/C-VID semantic change; no generated source or compiler intermediate checked into `src/`; no relocation of standalone host/app implementation (T17/T18 own those). |
| Baseline | T14 (`a072bb8`) establishes the selected OpenNT revision and the 60 direct-difference ledger. The current repository-owned recovered tree is `src/core/softpc/`; 39 direct rows propose later overlay treatment, 17 host placement, and four unselected XMS/allocator removals. |
| Affected Boundary | Repository-owned source layout and CMake source routing. The external OpenNT tree remains a read-only comparison input only: no file, generated patch, or build input is imported from it. |
| Subtask Plan | S1 record the no-import rule and the allowed retained-file classes; S2 move selected existing source with Git-preserved history into the matching `mvdm/softpc.new` paths; S3 route CMake to those paths; S4 prove x64/x86 regression parity and retain the T14 divergence ledger. |
| Requirement Ledger | R1: target paths match the reference relative layout. R2: every moved file already exists in this repository before the move. R3: retained machine files are C/H/ASM, required modern build configuration, or selected BIOS/VGA/CMOS firmware. R4: no object/library/intermediate output is retained. R5: CMake selects only the moved repository-owned sources. |
| Focused Verification | Git path/provenance inventory; retained-file-class audit; static CMake selected-input audit; then x64/x86 build and focused machine/renderer/input/timer/media/BOP tests. |
| Stop Conditions | Stop for owner direction if retaining a non-listed file type is required to build, if a source move exposes a machine-semantic difference, or if a later patch would require copying material from the external reference. |
| Exit Criteria | The build uses the repository-owned `src/mvdm/softpc.new` layout on x64/x86; its paths remain comparable against the T14 manifest; no external source or compiler intermediate has entered the repository; focused regression matches the T14 baseline. |
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
