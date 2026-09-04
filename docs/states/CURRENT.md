# Project Status

## Current Work

| Field | Required record |
| --- | --- |
| Identifier Mode | M9 Td S3 closed |
| Admission And Approval | Owner renamed the package root from `artifacts/` to `assets/`, then clarified its sole child contracts are `binary/`, `media/`, and `readme/`; ROMs remain in the source mirror. |
| Objective | Make the package, build, integration, and current-documentation contracts consistently use `assets/`. |
| Non-goals | No ROM relocation or external-ROM contract; no `softpc.ini` content change; no guest, machine, host, frontend, or executable behavior change beyond the package-output path. |
| Baseline | M9 T26 closed at `89ce6b0`; user has physically moved tracked package files from `artifacts/` to `assets/`. |
| Applicable Rules | Documentation, execution, architecture, and coding rules; source layout; the original mirror remains a preserved baseline and OpenNT is read-only comparison material. |
| Affected Boundary | Package output CMake variables, package integration contract, current package documentation, and tracked package-directory rename. |
| Subtask Plan | S1 replace active package paths and preserve the ROM source contract; S2 verify CMake and documentation governance; S3 stage the owner-moved package files as renames, then close and publish. |
| Requirement Ledger | R1: packages build to `assets/binary/`. R2: integration reads only `assets/binary/softpc.ini` and `assets/media/`. R3: `assets/readme/` remains documentation-only. R4: ROMs remain embedded from `src/mvdm/softpc.new/roms/`; no `assets/roms/` exists. R5: the agent does not modify INI contents. |
| Focused Verification | Path-reference sweep, CMake configure/build and package smoke, documentation-governance check, and tracked rename review. |
| Stop Conditions | Stop before altering user-owned INI contents, ROM inputs, media bytes, or machine behavior. |
| Exit Criteria | No active runtime/package reference uses `artifacts/`; both package widths and integration contract use `assets/`; documentation governance passes. |
| Closure | Closed. The package root is `assets/` with only `binary/`, `media/`, and `readme/` child contracts. CMake and the integration runner now use `assets/binary/` and `assets/media/`; x64 and x86 GCC package smoke passed. ROMs remain embedded from the original source mirror, and the user-owned INI contents were not modified. |
| Original Owner Request | “softpc/artifacts，整体目录改名成了 softpc/assets。请你治理文档，以后都在assets里面放binary，media，rom这些”； corrected: “binary, media, readme；rom还是在src mvdm里面的。” |

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
