# Project Status

## Current Work

| Field | Required record |
| --- | --- |
| Identifier Mode | M9 T29 S2 active |
| Admission And Approval | Owner accepted S1 evidence and admitted S2 from the queued Windows 3.1 MS-DOS Prompt frontend-performance proposal. |
| Objective | When a key remains in the standalone runtime queue after one original keyboard delivery, request another CCPU-safe executor wake without waiting for the next 50 ms host device tick. |
| Non-goals | No batch keyboard delivery; no change to the original keyboard service invocation count; no CCPU, device, BIOS, ROM, BOP, timer, INI, media, or guest behavior change; no change to the required exit-one-presenter/create-the-other structure. |
| Baseline | M9 T28 S1 is closed at `50df6b0` with owner transition evidence in `history/M9-T28-S1-prompt-transition-trace.md`. |
| Applicable Rules | Documentation, execution, architecture, and coding rules; source layout; the original mirror remains a preserved baseline and OpenNT is read-only comparison material. |
| Affected Boundary | Standalone runtime input queue and its existing CCPU-safe wake request; runtime unit coverage; package executables. |
| Subtask Plan | After one queued make or break is delivered through the existing original keyboard path, detect whether another key remains; request the existing executor wake only in that case; prove ordering and bounded delivery without changing the timer/device clock. |
| Requirement Ledger | R1: exactly one `softpc_machine_key_number` invocation per executor callback. R2: remaining queued keyboard work requests one subsequent CCPU-safe wake. R3: empty queue does not request a continuation wake. R4: frontend hotkeys, pause/resume, and mouse path retain their existing behavior. R5: no INI/media changes. |
| Focused Verification | New runtime queue-continuation unit proof; keyboard and lifecycle smokes; full x64/x86 CTest; package smoke; owner RDP/console/window typing, hotkey, and pause/resume check. |
| Stop Conditions | Stop after publishing rebuilt dual-width packages and verification. Do not start S3 or alter text geometry, presentation, timer, or CCPU generation. |
| Exit Criteria | A queued make/break sequence is delivered in order without one 50 ms device-tick wait per record; one keyboard service call remains the maximum per callback; both packages and full test suites pass. |
| Original Owner Request | “好的，那你做一下S2” |

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
- M9 T28 S1 closed with owner-proven Win3.1 MS-DOS Prompt transition evidence:
  original text is 80 by 25, while graphics legitimately transitions through
  640 by 350 before stabilizing at 640 by 480.
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
