# Project Status

## Current Work

| Field | Required record |
| --- | --- |
| Identifier Mode | M9 T31 S4 active |
| Admission And Approval | Owner requested T30 S3 closure from evidence and admitted S4 to optimize graphics presentation while investigating Win3.1 windowed MS-DOS Prompt characters. |
| Objective | Carry the original renderer's dirty rectangle through the standalone indexed-DIB-to-RGB window path, convert and invalidate only changed graphics regions, and retain diagnostic evidence that distinguishes original DIB updates from outer RGB/window loss. |
| Non-goals | No CCPU, V7/VGA controller, original renderer, BIOS, ROM, BOP, timer, input, INI, media, or guest/Win3.1 driver change; no console/window lifecycle restructuring. |
| Baseline | T29 S2 is closed at `bb3c67e`; T30 S3 closes without code in `history/M9-T30-S3-evidence-led-text-geometry.md`. |
| Applicable Rules | Documentation, execution, architecture, and coding rules; source layout; the original mirror remains a preserved baseline and OpenNT is read-only comparison material. |
| Affected Boundary | Standalone copied graphics-frame metadata, RGB surface conversion, Win32 dirty invalidation/paint; graphics unit coverage; package executables. |
| Subtask Plan | Preserve the original dirty rectangle with each copied graphics frame; force a full conversion only for first frame, surface geometry, or palette change; otherwise convert and repaint the mapped dirty rectangle. Emit compact input/frame/RGB diagnostic evidence for the owner Prompt reproduction. |
| Requirement Ledger | R1: the original indexed DIB and `nt_ega`/`nt_vga` painter remain unmodified. R2: a first frame, dimension change, or palette change forces a full RGB conversion/repaint. R3: ordinary dirty frames convert/invalidate only their clipped region. R4: diagnostic records identify input delivery, indexed frame, RGB conversion, and invalidation without mutating guest state. R5: no INI/media changes. |
| Focused Verification | Unit proof for dirty-rectangle clipping, palette/full-refresh selection, and mapped invalidation; existing renderer/window tests; full x64/x86 CTest and package smoke; owner Win3.1 windowed Prompt text reproduction. |
| Stop Conditions | Stop after dual-width packages and the compact diagnostic capture are ready. Do not alter V7/Win3.1 driver semantics or start S5. |
| Exit Criteria | An original dirty rectangle yields a corresponding partial RGB conversion and paint invalidation; full-refresh conditions remain correct; diagnostics can assign an absent Prompt glyph to the original DIB or the outer presentation path; both packages and full suites pass. |
| Original Owner Request | “哦对，做好S3收口，准入S4，优化显示” |

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
- M9 T29 S2 closed with a standalone queued-input continuation wake; owner
  confirmed normal operation after the dual-width 21/21 regression package.
- M9 T30 S3 closed with no code: recorded Prompt text geometry is already
  stable at 80 by 25, so its reported graphical character loss is not a text
  surface sizing issue.
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
