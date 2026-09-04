# Project Status

## Current Work

| Field | Required record |
| --- | --- |
| Identifier Mode | M9 T20 closed |
| Admission And Approval | Owner directed that standalone file names stop repeating `softpc_` and that an overlay file must share the mirrored source name. |
| Objective | Restore contextual standalone naming, remove false overlay helpers, preserve only the selected original source subset, and use a same-name overlay only where a direct source difference is materially large. |
| Non-goals | No change to C symbols, product/executable names, CPU, C-VID, controller, BIOS, ROM, BOP, media, timing, guest behavior, or external source import. |
| Baseline | M8 T19 has no source below `src/core` or `src/vm`; GCC x64/x86 both passed full CTest 20/20. T20 begins with 15 false overlay helpers, which have no source-mirror peer and must move to `host/compat`. `nt_com.c` and `nt_lpt.c` are wholly host-specific NT endpoint files and are intentionally absent from the selected mirror; their standalone replacements are `host/comms/serial.c` and `parallel.c`. |
| Affected Boundary | `src/{mvdm,host,overlay}`, test file names, CMake and transform input paths, direct-difference ledger, source-boundary test, and source-layout documentation. |
| Subtask Plan | S1 relocate non-mirror overlay helpers to `host/compat`; S2 rename standalone and test C/H files by contextual owner; S3 stop for owner design of the retained direct source differences; S4 only after that decision, assess same-name overlay feasibility and run clean dual-width proof. |
| Requirement Ledger | R1: no standalone C/H filename begins `softpc_`. R2: every overlay file, if any, has the same relative path and filename in the mirror. R3: all C symbols and product artifact names stay unchanged. R4: no recovered machine behavior changes; route-only include spelling is permitted where a renamed host header requires it. |
| Focused Verification | CMake direct-reference audit; transformed CCPU/C-VID/CMOS compilation; source-tree and ledger checks; clean GCC x64/x86 full CTest and package smoke. |
| Stop Conditions | Stop for owner direction if a rename exposes an external ABI consumer, requires an original-source semantic change, or needs a non-matching overlay to remain for a documented reason. |
| Exit Criteria | All required contextual moves are committed; false overlay helpers are gone; any retained overlay is a same-name mirror counterpart; the owner has selected a direct-difference design; and fresh x64/x86 builds/tests retain behavior. |
| Closure | Completed: faux overlays were relocated below `host/compat`, standalone files are contextual, no overlay remains, and original direct differences stay in their recovered files unless the selected mirror omits a wholly host-specific endpoint. Fresh GCC x64/x86 Release builds each passed CTest 20/20, including package smoke. |
| Original Owner Request | “非镜像和非overlay部分：怎么每个代码文件都是 softpc_ 开头? 文件名不必如此，我们知道这个repo是softpc了呀。overlay部分：文件名应该和镜像的源文件名相同才对啊，不然我哪知道你overlay了谁？？” |

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
