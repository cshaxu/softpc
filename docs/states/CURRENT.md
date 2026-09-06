# Project Status

## Current Work

| Field | Required record |
| --- | --- |
| Identifier Mode | M9 T41 S1 active |
| Admission And Approval | Owner reordered the queue to place NXVM library adoption first and directed its admission. The current reviewed source is `O:/repos.hobby/nxvm/src/lib` at `71e59ec09d669050ba98faca410ed8b9e9259d7c`, with `MANIFEST.sha256` file hash `2578C3B60B35FBD33D98A950F01F43846894C7AA3FAD22DED412DCC7CD2597DF`. |
| Objective | Atomically replace SoftPC `src/lib/` with that complete, byte-identical NXVM corpus, then produce the S1 ledger that routes every MVDM-external generic host/UX/storage capability to lib or identifies the exact upstream lib gap before binding/deletion begins. |
| Outcome | Imported the complete 56-file corpus byte-identically and verified its manifest. Local CMake now selects the imported targets, and prompt tracing now uses the shared storage writer. The file-level ledger is [T41 S1 evidence](../etc/evidence/softpc/m9-t41-s1-nonmvdm-lib-ledger.md); runtime UX binding remains blocked only on the missing generic UX input queue. |
| Non-goals | No modification of imported library files; no NXVM build/runtime dependency; no `mvdm` change; no migration, wrapping, replacement, or other alteration of any MVDM-local Win32 API use or WinNT-derived implementation; no movement of SoftPC product policy, original-host ABI, media geometry, executor ownership, or presentation decisions into lib; no package or user-configuration change. |
| Affected Boundaries | `src/lib/`, local CMake/source-selection and public-header wiring, and every existing direct non-MVDM clock/task/event/wait, file/media, and UX/presentation route in `src/app/` and `src/host/`. `src/mvdm/softpc.new/` is excluded and immutable, including its MVDM-local Win32/WinNT implementation. |
| Applicable Rules | Execution, architecture, coding, and documentation authorities; the approved adoption proposal; library files remain verbatim and their manifest is authoritative. |
| Focused Verification | Verify the upstream manifest before import and the local corpus after import; prove no former SoftPC library file remains in `src/lib/`; configure/build the imported library locally without modifying it; complete the non-`mvdm` ownership ledger. The upstream `src/lib` path is clean at the selected revision; its manifest verifies and its README now matches the three-component corpus. |
| Full Regression | Before S1 closure, run proportionate local GCC x64 and i686 x86 configuration/build/CTest evidence, preserve `assets/binary/softpc.ini` and guest media, and run documentation governance and diff checks. |
| Similar-Issue Sweep | Search source selection, includes, and callers for every displaced `src/lib/` route and every direct non-MVDM platform/file/media use; classify each as a mandatory `host`/`storage`/`ux` migration, retained SoftPC-specific owner, candidate deletion, or explicit upstream blocker. |
| Stop Conditions | Stop for owner direction if the corpus or manifest is not exact, the upstream README/manifest scope is inconsistent, local compilation requires an imported-source edit, a shared generic facility is absent (including the input queue), an intended removal reaches a machine/original-host contract, or an x86/x64 difference lacks an explicit outer binding boundary. |
| Exit Criteria | `src/lib/` exactly matches the approved NXVM corpus and verifies by manifest; every displaced current library file and active non-`mvdm` owner has a recorded disposition; local build integration is explicit; no unproved deletion or routing change has occurred. |
| Original Owner Request | “队列里把lib adoption作为队首任务，准入” |

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
