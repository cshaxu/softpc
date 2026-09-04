# Project Status

## Current Work

| Field | Required record |
| --- | --- |
| Identifier Mode | M9 T27 closed |
| Admission And Approval | Owner approved flattening the two-file standalone communications endpoint directory, then requested the architecture decision for `compat/`. |
| Objective | Remove the needless `src/host/comms/` nesting while retaining `src/host/compat/` as the original-host ABI compatibility boundary. |
| Non-goals | No original SoftPC mirror movement or edits; no serial/parallel behavior, host contract, configuration, ROM, guest-media, ABI, or frontend change; do not move `compat/` to `src/compat/`. |
| Baseline | M9 Td S3 closed at `6f7294a`; `src/host/comms/` contains only `serial.c` and `parallel.c`, which have no private header or shared implementation boundary. |
| Applicable Rules | Documentation, execution, architecture, and coding rules; source layout; the original mirror remains a preserved baseline and OpenNT is read-only comparison material. |
| Affected Boundary | The two standalone endpoint source paths, their CMake source-list entries, source-layout documentation, and package executables only. |
| Subtask Plan | S1 use `git mv` to flatten `serial.c` and `parallel.c`; S2 repair direct build/documentation references; S3 run dual-width source-boundary, unit, integration, and package evidence; S4 close and publish. |
| Requirement Ledger | R1: `src/host/comms/` no longer exists. R2: serial and parallel endpoint source is directly below `src/host/`. R3: `src/host/compat/` remains in place and retains its ABI-only ownership. R4: package INI and media remain untouched. |
| Focused Verification | Direct-reference sweep, source-boundary test, both GCC configurations' unit and integration tests, and package smoke. |
| Stop Conditions | Stop before changing any endpoint logic, machine source, configuration contents, or compatibility-ABI location/meaning. |
| Exit Criteria | The tree is structurally flatter, CMake builds x86/x64, the existing test tiers pass, and the diff is only relocations plus direct path/documentation repairs. |
| Closure | Closed. `serial.c` and `parallel.c` now live directly below `src/host/`; the empty `comms/` directory is gone. `src/host/compat/` remains the original-host ABI compatibility boundary. Fresh GCC x64/x86 builds and full 20/20 CTest passed for both widths; user-owned INI and media contents were untouched. |
| Original Owner Request | “src/host是否可以继续扁平化？comms目录有必要存在吗？”; “批准。那么，src/host/compat这个该放host里面还是src/compat里面？” |

## Current Technical Baseline

- The fixed standalone package is `assets/binary/softpc32.exe` and
  `softpc64.exe`, with adjacent user-owned `softpc.ini`; reusable guest media
  is below `assets/media/`, and README captures are below `assets/readme/`.
- M8 T19 completes the source layout as `src/{mvdm,overlay,host,app}` and
  removes transitional `src/core` and `src/vm` routes. Fresh GCC x64/x86
  builds each passed full CTest, 20/20.
- The selected original ROM inputs are byte-identical to OpenNT and embedded
  from `src/mvdm/softpc.new/roms/`; no runtime ROM artifact root is active.

## Recent Governance

- M9 T23 retired 294 demonstrably unselected historical paths (15 C files and
  279 headers) from the original mirror without changing its selected machine
  behavior; fresh GCC x64/x86 CTest each passed 20/20.
- M9 T24 closed with dual-width 20/20 CTest proof and package smoke. The next
  reported issue is a guest Ctrl+Alt+Del warm-reset hang after `Starting
  MS-DOS ...`; it requires a dedicated lifecycle investigation.
- M9 T25 closed by reconnecting the original keyboard `reboot()` callback to
  the CCPU hardware reset line. This is a machine reset, not a DOS/DPMI or
  frontend restart; dual-width GCC CTest passed 20/20 at closure.
- M9 T26 separates local monitor commands from running-guest hotkeys with one
  blank output line; both GCC package-smoke tests passed.
- M9 T22 normalizes frontend host hotkeys without changing SoftPC: Ctrl+Alt+P
  no longer strands guest modifiers across console pause/resume, and
  Ctrl+Alt+F now supplies guest Alt+Enter rather than Ctrl+Alt+Enter.
- M9 Td S1 makes `assets/binary/softpc.ini` permanently user-owned package
  configuration: agents may refresh the executable pair but cannot edit or
  normalize its contents; an owner change or move ships with its task.
- M9 Td S3 renamed the package root to `assets/`, with `binary/`, `media/`,
  and `readme/` as its only contracts. ROMs remain embedded source-mirror
  inputs, not package assets.
- M9 T27 flattens the two standalone communications endpoint sources into
  `src/host/` while retaining `src/host/compat/` as the original-host ABI
  compatibility boundary; dual-width full CTest passed.
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
