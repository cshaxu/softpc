# Project Status

## Current Work

| Field | Required record |
| --- | --- |
| Identifier Mode | M9 T42 S1 active |
| Admission And Approval | Owner explicitly closed T41 and admitted T42 for the complete approved Console-object/host/UX/SoftPC design. T42 begins from T41's imported NXVM-library baseline and its governing Td S6 design closure. |
| Objective | Implement the generic `base` Console object, `host` one-current-object broker, independently composable `ux` Window/Console presenters, and the non-MVDM SoftPC reconciler; then obtain NXVM's byte-identical adoption and re-import that corpus into SoftPC. |
| Outcome | T42 is newly active. T41's imported 52-file `base`/`host`/`storage`/`ux` corpus and its non-MVDM ownership ledger are the baseline. The prior Window/Console behavior is not accepted product behavior and is replaced, not incrementally patched, by the approved derived-state design. |
| Non-goals | No modification to `src/mvdm/softpc.new/`, its MVDM-local Win32 calls, or WinNT-derived implementation; no SoftPC lifecycle, DISPLAY policy, `console_control`, monitor syntax, or guest-hotkey in lib; no permanent SoftPC-only lib fork or NXVM runtime/build dependency; no package configuration or guest-media change. |
| Affected Boundaries | `src/lib/{base,host,ux}`, their CMake/public-header wiring, and non-MVDM `src/app/`/`src/host/` binding and tests. `storage` is not redesigned. `src/mvdm/softpc.new/` remains excluded and immutable. |
| Applicable Rules | Execution, architecture, coding, and documentation authorities; [T42 proposal](../proposals/m9-t42-console-object-ux-recomposition.md); [Product UX](../design/UI.md); [System Architecture](../design/ARCHITECTURE.md). |
| Focused Verification | First prove pure SoftPC state derivation and completion-gated reconciler traces. Then prove host one-current-object transactional replacement, stale-event/lifetime behavior, and UX presenter completion/retirement with controllable fakes. Exercise real threads only using barriers/events and bounded deadlock guards, never sleeps. |
| Full Regression | Before closure, run local x64 and x86 configure/build/CTest evidence proportional to the runnable change, preserve `assets/binary/softpc.ini` and guest media, run the Console/Window owner acceptance matrix, and pass documentation governance and diff checks. |
| Similar-Issue Sweep | Audit every non-MVDM direct Console reader/writer, `stdin`/`fgets` handoff, native Console mode/handle use, presenter-type switch, mouse/input route, and lifecycle callback. Each must become a single approved host/UX/SoftPC path or be removed; MVDM remains out of scope. |
| Stop Conditions | Stop for owner direction if an API encodes SoftPC policy, changes MVDM, leaves two native readers/output owners, permits a zero-current-object state outside broker teardown, or cannot be adopted by NXVM unchanged. |
| Exit Criteria | Deterministic tests prove the approved state/transition matrix and thread boundaries; x64/x86 evidence and owner runtime acceptance pass; NXVM adopts the generic candidate byte-identically; SoftPC re-imports and manifest-verifies that adopted corpus. |
| Original Owner Request | “请你直接收口T41吧，我们开T42来做刚才的全套设计” |

## Current Technical Baseline

- The fixed standalone package is `assets/binary/softpc32.exe` and
  `softpc64.exe`, with adjacent user-owned `softpc.ini`; reusable guest media
  is below `assets/media/`, and README captures are below `assets/readme/`.
- M8 T19 completes the source layout as `src/{mvdm,host,app}` and
  removes transitional `src/core` and `src/vm` routes. Fresh GCC x64/x86
  builds each passed full CTest, 20/20.
- The selected original ROM inputs are byte-identical to OpenNT and embedded
  from `src/mvdm/softpc.new/roms/`; no runtime ROM artifact root is active.

## Recent Governance

- M9 T41 closed by owner direction after completing the imported-lib baseline
  and non-MVDM ownership ledger. Its incomplete Console/Window runtime work is
  transferred to active T42; see [T41 history](../history/M9-T41-nxvm-library-adoption-and-binding.md).

- M9 Td S6 promoted the Console-object design to `design/UI.md` and
  `design/ARCHITECTURE.md`, retained the reviewed decision record as evidence,
  and fixed SoftPC-first candidate delivery: NXVM must adopt it exactly and
  SoftPC must re-import it before T42 closure. See
  [Td S6 history](../history/M9-Td-S6-console-object-governance.md).

- M9 Td S5 reconciled the XP SP1 proposal and Chinese A/B audit after T40:
  the local C/H inventory is 493, all 126 OpenNT-only paths are now absent,
  while the 86 A/B conclusions and 21 XP-only inventory remain unchanged.
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
- M9 T36 first-stage source extracts copied-frame ABI, normalized keyboard
  input, DIB geometry, sizing, and explicit mouse capture to
  `src/lib/platform/win32/`; it is reopened because mailbox/event ownership,
  reusable window/console loops, and registered product actions remain in
  `src/app`.
- M9 T36 closed after its completion audit moved the final router-to-presenter
  dispatcher from app to lib and removed app-local console/window wrappers.
  GCC x64/x86 full CTest each passed 23/23; the new source boundary check
  forbids the app from recovering target dispatch. See [T36 history](../history/M9-T36-shared-win32-presentation-library.md).
- M8 Td S1 established the NXVM-style authority topology, linked the four
  applicable shared governance skills, and added the documentation gate.
- M8 Td S2 established the public product identity as Insignia SoftPC and
  added current, owner-provided SoftPC product captures to the root README.
- M9 Td S4 admitted direct source-visible mechanical portability diffs and
  external host compatibility ownership; it removed the former generated-source
  portability rule without changing machine source or build selection. See
  [Td S4 history](../history/M9-Td-S4-direct-source-portability-governance.md).
- M9 T38 retired every mechanical C/Python source transform. Its 52 direct
  source-visible portability corrections build from checked-in source for both
  widths; GCC x64 and i686 full CTest each passed 23/23. See [T38
  history](../history/M9-T38-direct-source-transform-retirement.md).
- M9 T39 audited the complete NXVM `src/lib/` as a prospective verbatim shared
  import. Its subsequent reviewed `7038e0ef` corpus now supplies the admitted
  T41 S1 baseline; MVDM remains immutable. See [T39
  history](../history/M9-T39-nxvm-shared-lib-admission-audit.md).
- M8 T14 established a deterministic, 1,222-row source-map against the
  selected OpenNT revision. It found 60 direct differences and assigned every
  one a future direct-source, host, or removal disposition without altering code.
- Implementation task identifiers are repository-wide build versions: the
  12 historical tasks end at T12 and M8 completes at T19. Td governance work
  does not consume a build-version number.
