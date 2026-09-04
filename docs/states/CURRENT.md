# Project Status

## Current Work

| Field | Required record |
| --- | --- |
| Identifier Mode | M9 T26 closed |
| Admission And Approval | Owner requested a blank line immediately before the monitor help heading `While the guest is running:`. |
| Objective | Make the monitor help's local-command section visually distinct from its guest hotkey section. |
| Non-goals | No command, hotkey, input, lifecycle, window, machine, ROM, guest-media, executable-name, or user-owned `softpc.ini` behavior change. |
| Baseline | M9 T25 closed at `82134be`; the monitor lists local commands and then starts the running-guest hotkey heading without a separating blank line. |
| Applicable Rules | Documentation, execution, architecture, and coding rules; source layout; the original mirror remains a preserved baseline and OpenNT is read-only comparison material. |
| Affected Boundary | `src/app/main.c` monitor help text and package executables only; no machine or host interface. |
| Subtask Plan | S1 insert the separator at the fixed help boundary; S2 rebuild both GCC packages and verify the presentation text; S3 close and publish. |
| Requirement Ledger | R1: exactly one blank line precedes the running-guest heading. R2: local commands and hotkey strings remain unchanged. R3: the user-owned INI remains untouched. |
| Focused Verification | Source-level help-text check, GCC x64/x86 rebuild, package smoke. |
| Stop Conditions | Stop before changing command/hotkey behavior or any source outside the monitor presentation surface. |
| Exit Criteria | Both package executables contain the separated help layout and package smoke passes. |
| Closure | Closed. One empty monitor-output line now separates `exit` from `While the guest is running:`. GCC x64/x86 packages rebuilt and each passed package smoke; the user-owned INI was not changed. |
| Original Owner Request | “While the guest is running: 这句help行之前要加一个空行。” |

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
