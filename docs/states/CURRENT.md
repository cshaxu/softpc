# Project Status

## Current Work

| Field | Required record |
| --- | --- |
| Identifier Mode | M9 T36 closed |
| Admission And Approval | Owner admitted and approved the shared Win32 presentation-library proposal. |
| Objective | Extract SoftPC's reusable copied-frame, Win32 input, geometry, and presentation mechanics into `src/lib/platform/win32/`, with `src/app` retaining a thin project binding. |
| Non-goals | No change to `src/mvdm/softpc.new`, machine/device/BOP/ROM/media behavior, user-owned `softpc.ini`, product lifecycle, fixed hotkeys, or the one-presenter-at-a-time console/window transition. |
| Baseline | T36 is closed; see [closure record](../history/M9-T36-shared-win32-presentation-library.md). |
| Applicable Rules | Documentation, execution, architecture, and coding rules; source layout; the original mirror remains a preserved baseline and OpenNT/NXVM/NTVDM64 are read-only comparison material. |
| Affected Boundary | `src/app` to local `src/lib/platform/win32` only; copied values and opaque runtime ownership remain mandatory. |
| Subtask Plan | Complete: established copied frame ABI and generic Win32 helpers; retained app bindings; added a source-hash manifest check; rebuilt/tested both widths. |
| Requirement Ledger | Met: shared component has no machine/runtime pointers; app owns input mapping, hotkeys, lifecycle and route decisions; source contents can be hash-compared by sibling projects. |
| Focused Verification | Generic input/geometry/mouse unit coverage; runtime/window/keyboard/package tests; full GCC x64 and x86 CTest. |
| Stop Conditions | Not reached: the extraction required no recovered-machine modification and changed no lifecycle policy. |
| Exit Criteria | Met: local component/thin binding built; manifest check passes; no original-machine file changed; dual-width tests and package builds pass. |
| Original Owner Request | “很好 准入 请你执行” |

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
- M9 T36 extracts copied-frame ABI, normalized keyboard input, DIB geometry,
  sizing, and explicit mouse capture to `src/lib/platform/win32/`. The app
  binding retains guest input encoding, hotkeys, routing, and lifecycle;
  GCC x64/x86 full CTest each passed 23/23.
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
