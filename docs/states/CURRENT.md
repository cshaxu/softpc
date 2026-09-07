# Project Status

## Current Work

| Field | Required record |
| --- | --- |
| Identifier Mode | M9 T41 S1 active; M9 Td S6 governance closed before S5 implementation |
| Admission And Approval | Owner reordered the queue to place NXVM library adoption first and directed its admission. The current reviewed source is `O:/repos.hobby/nxvm/src/lib` at `c7b5e668b7d9fc8e8710c3dd77c863ce07801553`, with `MANIFEST.sha256` file hash `C22012E985A30E130369AAC8A5E0C0E93DA9F56C842843A4C5B3685769E3E9A6`. The owner additionally admitted S5 and explicitly changed its delivery order: SoftPC implements the reviewed generic host/UX candidate in local `src/lib/`; NXVM then adopts that exact implementation; SoftPC re-syncs the adopted corpus before T41 closure. |
| Objective | Preserve the admitted corpus as baseline, complete the S1 ownership ledger, and implement the owner-approved S5 generic Console-object/UX candidate in SoftPC `src/lib/` plus its non-MVDM SoftPC binding. NXVM must adopt the candidate byte-identically and SoftPC must re-import that adopted corpus before closure; product runtime policy remains in SoftPC. |
| Outcome | Imported the complete 52-file corpus byte-identically and verified its manifest. Local CMake now selects the imported targets; prompt tracing uses the shared storage writer; HDD and GFI media use shared byte-medium leases; and the runtime/presenter plus audio worker use shared UX and host synchronization. The file-level ledger is [T41 S1 evidence](../etc/evidence/softpc/m9-t41-s1-nonmvdm-lib-ledger.md). The UX input queue is SoftPC runtime coordination, and binary COM/LPT file sinks are SoftPC device endpoints; neither is an NXVM-lib prerequisite. SoftPC owns frame routing policy in `app_runtime`: console graphics requests window, and three stable text frames request console, with requests suppressed when the target is unchanged. The binding sends `0x00RRGGBB` frame pixels to lib, updates Window-only titles as runtime state changes, and enables mouse capture only for a running Window target. **Owner runtime acceptance remains pending.** The later experimental monitor/frontend coordinator and global-`stdin` handoff is withdrawn from the working tree and may not be revived; S5 replaces it with a generic host Console-endpoint contract and completed UX surface facts. |
| Non-goals | No NXVM build/runtime dependency; no `mvdm` change; no migration, wrapping, replacement, or other alteration of any MVDM-local Win32 API use or WinNT-derived implementation; no movement of SoftPC product policy, original-host ABI, media geometry, executor ownership, or presentation decisions into lib; no package or user-configuration change. Local `src/lib/` edits are limited to the approved generic S5 candidate and cannot become a permanent SoftPC fork. |
| Affected Boundaries | `src/lib/`, local CMake/source-selection and public-header wiring, and every existing direct non-MVDM clock/task/event/wait, file/media, and UX/presentation route in `src/app/` and `src/host/`. `src/mvdm/softpc.new/` is excluded and immutable, including its MVDM-local Win32/WinNT implementation. |
| Applicable Rules | Execution, architecture, coding, and documentation authorities; the approved adoption proposal; S5 candidate edits remain generic and must be adopted by NXVM byte-identically, then re-imported into SoftPC before closure. |
| Focused Verification | Verify the upstream manifest before import and the local corpus after import; prove no former SoftPC library file remains in `src/lib/`; configure/build the imported library locally without modifying it; complete the non-`mvdm` ownership ledger. The upstream `src/lib` path is clean at the selected revision; its manifest verifies and its README now matches the three-component corpus. The current GCC x64 build produces `assets/binary/softpc64.exe`; its manifest and source diff checks pass. The current local Clang i686 MinGW-target configuration cannot link because its required i686 import libraries are absent on this host, so no x86 acceptance is claimed for this checkpoint. The runtime smoke covers three-text-frame return; owner must manually verify console-to-graphics window creation on both package widths. |
| Full Regression | Before S1 closure, run proportionate local GCC x64 and i686 x86 configuration/build/CTest evidence, preserve `assets/binary/softpc.ini` and guest media, and run documentation governance and diff checks. |
| Similar-Issue Sweep | Search source selection, includes, and callers for every displaced `src/lib/` route and every direct non-MVDM platform/file/media use; classify each as a shared `host`/`storage`/`ux` migration, retained SoftPC-specific runtime/device owner, candidate deletion, or explicit upstream blocker where both projects actually require the missing generic capability. |
| Stop Conditions | Stop for owner direction if a candidate edit is not generic, would encode SoftPC lifecycle/monitor semantics, reaches a machine/original-host contract, leaves two independent Console readers, or cannot be adopted by NXVM unchanged. Do not manufacture an upstream prerequisite from a product-specific runtime queue or device-endpoint protocol. |
| Exit Criteria | The S5 candidate is fully built/tested in SoftPC, adopted byte-identically by NXVM, and re-imported/verifiable in SoftPC; every displaced current library file and active non-`mvdm` owner has a recorded disposition; local build integration is explicit; no unproved deletion or routing change has occurred. |
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

- M9 Td S6 promoted the S5 Console-object design to `design/UI.md` and
  `design/ARCHITECTURE.md`, retained the reviewed decision record as evidence,
  and fixed SoftPC-first candidate delivery: NXVM must adopt it exactly and
  SoftPC must re-import it before T41 closure. See
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
