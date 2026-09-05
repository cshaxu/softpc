# Project Status

## Current Work

| Field | Required record |
| --- | --- |
| Identifier Mode | M9 T36 — active implementation/build version |
| Admission And Approval | Owner explicitly authorized investigation and repair of the Windows 3.1 graphical MS-DOS Prompt rendering defect. |
| Objective | Identify and recover the missing V7VDD virtual-display character-update path while commands continue to execute. |
| Non-goals | No DOS/DPMI/Windows-driver/NTVDM semantics, frontend VRAM decoder, INI/media/ROM change, or presenter-lifecycle redesign. |
| Baseline | T35 is closed. The defect predates T28–T35 outer-runtime dirty-frame and scheduling work. |
| Applicable Rules | Documentation, execution, architecture, coding and source-layout authorities; OpenNT is read-only comparison material. |
| Affected Boundary | Selected original V7 revision-3 extension registers and the original C-VID CPU-side VGA write dispatch that consumes their per-plane ALU input. |
| Subtask Plan | Trace V7VDD's selected proprietary hardware transactions and correlate each with original bank/plane state, C-VID ALU input, painter output and dirty publication; repair only the failed V7 stage. |
| Requirement Ledger | [M9 T36 task record](../history/M9-T36-windows31-msdos-prompt-rendering.md). |
| Focused Verification | The baseline V7 CCPU/DIB regression passes; the keyboard-refresh hypothesis failed owner acceptance and was removed. Pending: graphical Prompt visible text plus command execution after a V7-specific repair. |
| Stop Conditions | Stop and queue a design task if evidence requires a new VGA controller, Windows driver/DOS semantics, or broad unclassified mirror rewrite. |
| Exit Criteria | The task record's evidence and dual-width package/regression requirements are complete. |
| Original Owner Request | “对啊，你来帮我研究和修复一下看看怎么回事” |

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
- M9 T34 S7 replaces the standalone window's fixed 16 ms polling timer with
  frame-publication, UI-message, and cursor/title-deadline waits; GCC x64 and
  x86 full CTest each passed 21/21.
- M9 T35 S8 replaces the console's fixed 10 ms sleep loop with input/frame
  waits plus a bounded state deadline; GCC x64 and x86 full CTest each passed
  21/21.
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
