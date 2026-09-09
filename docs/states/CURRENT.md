# Project Status

## Current Work

| Field | Required record |
| --- | --- |
| Identifier Mode | M9 T43 S7 active |
| Admission And Approval | Owner approved a bounded historical-semantics experiment: successful raw broker activation restores native Console focus; owner reproduction then established that an unprompted cooked reader consumes the first input after raw takeover. |
| Objective | Restore raw VM Console input after a Window-to-Console transition: raw activation owns its reader and focus; cooked input is armed only by an explicit monitor prompt request. |
| Non-goals | No MVDM modification, guest timer/device change, public app focus API, configuration/media change, or reconciler ordering change. |
| Affected Boundaries | `lib/host/win32` Console activation/reader lifecycle, focused tests, shared-library manifest, refreshed packages, and T43 records. |
| Applicable Rules | Execution, architecture, coding, and documentation authorities; [T43 proposal](../proposals/m9-t43-lib-console-ux-test-repairs.md); [System Architecture](../design/ARCHITECTURE.md); [Product UX](../design/UI.md). |
| Focused Verification | Prove only successful raw activation restores Console input focus; prove cooked activation does not arm `ReadConsoleA` until an explicit prompt request; verify raw VM Console inputs after Window retirement and owner-evaluate Window-plus-raw coexistence. |
| Full Regression | Focused broker/presentation tests plus x64/x86 full package builds and CTest. Preserve user-owned `assets/binary/softpc.ini` and media. |
| Similar-Issue Sweep | Verify raw/cooked activation, restoration, Window destroy, and Window creation leave no unprompted cooked reader or unintended Console activation path. |
| Stop Conditions | Stop if raw takeover can still be preceded by an unprompted cooked reader after owner reproduction. |
| Exit Criteria | No public app/broker focus API exists; only successful raw activation restores Console foreground; cooked reader starts only at explicit prompt request; x64/x86 package builds and full CTest pass; owner validates the reported transition. |
| Original Owner Request | “就是在你最新的修改之后呀，set focus相关的。” |

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

- M9 T43 S6 supersedes the S1 foreground policy: Current Console activation,
  replacement, and restoration configure I/O only and never foreground
  Console. Window retains its creation self-focus; only SoftPC, after actual
  Window retirement into a Console-only state, asks host to focus Current
  Console. Fresh x64/x86 full CTest each passed 32/32. See [S6 history]
  (../history/M9-T43-S6-console-only-focus.md).

- M9 T43 S2 closed the shared-library split-UX contract documentation and
  manifest: README now records the component graph, mailbox semantics,
  latest-frame delivery, source retirement, and neutral logical Console;
  source-boundary and manifest tests passed for both widths. See [S2 history]
  (../history/M9-T43-S2-library-contract-closure.md).

- M9 T43 S3 closed fail-closed UX control capacity. Ordinary control requests
  return/report capacity failure without mutation; multi-record mouse disable
  is atomic; STOP retains its documented terminal reserve. Both leaf mailboxes
  and full x64/x86 regressions passed. See [S3 history]
  (../history/M9-T43-S3-ux-control-capacity.md).

- M9 T42 S5 closed with the SoftPC monitor/derived-state reconciler, immutable
  two-buffer frame publication, unchanged-text publication gate, staged
  Current Console replacement, and refreshed x64/x86 packages. Both local
  widths passed full 26/26 CTest. Residual owner-observed interaction edges
  remain outside this closure; see [S5 history](../history/M9-T42-S5-softpc-monitor-reconciler.md).

- M9 T42 S6 closed the generic Current Console and UX lifecycle contracts;
  x64 full CTest passed 27/27 and x86 library probes passed. See [S6
  history](../history/M9-T42-S6-console-ux-lifecycle-contracts.md).

- M9 T42 S7 closed real-thread Console/UX barrier verification: x64 full
  CTest passed 29/29 and x86 Console/broker barriers passed 3/3. See [S7
  history](../history/M9-T42-S7-real-thread-barriers.md).

- M9 T42 closed at S9 after owner acceptance of the completed Console/Window
  integration. S8 also completed generic work-area-aware initial Window
  bounds; any later issue must be separately admitted. See [S8 history]
  (../history/M9-T42-S8-work-area-window-bounds.md) and [S9 history]
  (../history/M9-T42-S9-owner-runtime-acceptance.md).

- M9 T41 closed by owner direction after completing the imported-lib baseline
  and non-MVDM ownership ledger. Its incomplete Console/Window runtime work is
  transferred to active T42; see [T41 history](../history/M9-T41-nxvm-library-adoption-and-binding.md).

- M9 Td S6 promoted the Console-object design to `design/UI.md` and
  `design/ARCHITECTURE.md` and retained the reviewed decision record as
  evidence. See [Td S6 history](../history/M9-Td-S6-console-object-governance.md).

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
