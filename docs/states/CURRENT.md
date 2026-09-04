# Project Status

## Current Work

| Field | Required record |
| --- | --- |
| Identifier Mode | M9 T24 closed |
| Admission And Approval | Owner admitted the next queued task: contextual standalone symbol ownership. |
| Objective | Make modern application globals explicitly `app_*`, without changing the recovered SoftPC machine ABI or behavior. |
| Non-goals | No rename below `src/mvdm/softpc.new`; no change to original host-ABI callbacks, CCPU/C-VID/V7/BIOS/BOP/controller code, guest behavior, UI behavior, media, ROMs, executable names, or `softpc.ini`. The compatibility host remains under its established historical spellings where generated or original machine code imports them. |
| Baseline | The proposal's initial broad scan found 320 unique modern `softpc_*` candidates. The admission audit proved that the host/compat candidates include direct original imports and generated CCPU/C-VID ABI: their spellings are not standalone naming debt. The executable scope is therefore app-owned globals and their direct consumers only. |
| Applicable Rules | Documentation, execution, architecture, and coding rules; source layout; the original mirror remains a preserved baseline and OpenNT is read-only comparison material. |
| Affected Boundary | Application-owned symbols and matching declarations/callers/tests/source-boundary checks below `src/app/` and `test/`; original and generated machine imports remain unchanged. |
| Subtask Plan | S1 classify candidate definitions and generated/import consumers; S2 rename the app owner families and repair direct consumers; S3 add a regression gate, build both GCC widths, run full CTest and package smoke, then close. |
| Requirement Ledger | R1: every renamed application global identifies its `app` owner. R2: original and generated SoftPC-required imports retain their spelling. R3: all declarations, callers, tests and static checks use the selected name. R4: x64/x86 package behavior remains unchanged. |
| Focused Verification | Definition/declaration/import audit; static app-ownership gate; GCC x64/x86 builds; full CTest; package smoke for both launchers. |
| Stop Conditions | Stop before renaming an original mirror import, a generated ABI entry point, an externally required Windows entry point, or an ambiguous symbol lacking a provable owner; record such cases rather than guessing. |
| Exit Criteria | Application-owned standalone globals use `app_*`; original and generated SoftPC ABI spelling is intact; static ownership gate and dual-width full regression pass. |
| Closure | Closed: application-owned globals now use `app_*`; all host/compat `softpc_*` spellings were classified as required original/generated machine ABI and retained. GCC x64 and x86 rebuilt and each passed full CTest, 20/20, including package smoke. |
| Original Owner Request | “准入执行” for the approved M9 contextual standalone symbol ownership proposal. |

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
- M9 T24 is admitted to make modern application symbol ownership explicit. Its
  admission audit establishes that host/compat `softpc_*` spellings form an
  original/generated machine ABI and must remain intact.
- M9 T24 closed with dual-width 20/20 CTest proof and package smoke. The next
  reported issue is a guest Ctrl+Alt+Del warm-reset hang after `Starting
  MS-DOS ...`; it requires a dedicated lifecycle investigation.
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
