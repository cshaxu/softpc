# Project Status

## Current Work

T60 is closed after owner manual acceptance and the whole-task audit.
T61 is closed on owner direction after the [whole-task audit](../history/M9-T61-completion-audit.md).
T62 is closed on owner direction after the
[whole-task audit](../history/M9-T62-completion-audit.md).
M9 T63 S1 is closed after the [design review](../history/M9-T63-S1-machine-snapshots.md).
M9 T63 S2 is closed after the field inventory, cross-width state-owner index
and running-save safe-stop boundary proof. M9 T63 S3 is closed after the
existing-Storage/media-ownership decision: bounded readonly medium reads and
ordinary truncate publication require no Lib change. M9 T63 S4 is closed after
the private CPU/SAS/RAM archive proof. M9 T63 S5 is closed after the private
controller/queue archive and dual-width restore proof. M9 T63 S6 is closed
after its independent receiver/archive review. M9 T63 S7 is closed after the
bounded Common machine-state operations and VM's single executor-owned
save/restore transaction. M9 T63 S8 is closed after the App command/file
boundary. M9 T63 S9 is active for canonical streaming snapshot I/O without
host-width or artificial-size contracts.
[Delivery and acceptance ledger](../history/M9-T62-common-lib-simplification.md).
T62 closure was pushed in 54b2009 before T63 admission. The three older
candidates remain queued. [Snapshot proposal](../proposals/m9-machine-snapshots.md).

S7 P7 is locally verified: VM now composes private CCPU/SAS and device slices
into a canonical versioned image with dual-width checkpoint proof. S7 P8 binds
the existing Common state-transfer rendezvous to the VM-owned safe capture and
stopped restore transaction: running read returns ordinary PAUSED, stopped
write stages first and restores into ordinary PAUSED. P9 reconstructs host
video resources only after controller replay and proves that restore publishes
only a complete graphics frame when a graphics painter is available. It is not yet an App
snapshot command and media sections remain in scope. P10 suppresses only
reset/archive-replay executor callbacks; it reopens the existing callback
before restored CCPU re-entry, so the PAUSED completion cannot precede the
completed restored surface or deadlock the parked executor.
P11 makes staged decoding independent of an already initialized CCPU: the
image's declared RAM must match the target machine configuration, and its SAS
page-type length is validated from that declaration before any reset. A real
two-process save/exit/load/resume CTest proves a fresh stopped target reaches
ordinary PAUSED without importing live SAS state.
S8 P1 adds the product boundary only: `save <file>` is admitted from running
and pauses on successful capture; `load <file>` is admitted from stopped
(including the just-started monitor) and reaches ordinary paused.  App owns copied paths, the RAM-plus-allowance
input bound, wording and prompt flow; Common and Lib remain unchanged.
P2 removes the redundant App-only `INIT` monitor state: initial and stopped
are one stopped command-policy state, while Common retains its own lifecycle
representation. P3 fixes the legacy `illegalp.c` error-header overlay for a
fresh 32-bit MSYS2 build; it selects the original Base error enum after the
historical shared include guard, without changing App, Common or Lib behavior.
P6 attempted to preserve P9's presentation route with a restored-graphics
text fallback; S9 P3 supersedes it: graphics has no text fallback. Until its
rebuilt painter supplies a complete dirty frame, it publishes nothing and
retains no false 80x25 presentation. This restores the Window/Console route
invariant. P7 briefly
used a fixed `32` machine-width marker for cross-width images. Owner has
superseded that format: S9 removes every width field altogether and replaces
the bounded App buffer/container path with canonical streaming I/O.
S9 P4 archives the missing V7 current-mode semantic byte. Restore resets only
the original host geometry cache before reselecting its painter; standalone
graphics resize no longer mistakes the retired NTVDM fullscreen state for
permission to retain a stale DIB. The image revision is now 3, so pre-P4
images are intentionally rejected and must be re-saved.
P5 completes that synchronous restored-graphics rebuild by applying the
restored indexed DAC before the original full repaint; a newly rebuilt DIB
therefore cannot publish valid indices through its zeroed default palette.
P6 clears the old DIB's deferred host-presentation state as part of that same
rebuild. A pre-save graphics tick can no longer re-run a stale mode change or
flush after restore and overwrite the synchronously rebuilt surface.
P7 identifies the reproduced black-frame cause: VGA reset writes are owned by
EGA's receiver, while capture read VGA's unused reset member as zero. Capture
now reads the actual receiver register. Image revision 4 rejects earlier
images whose reset value was lost. Tests preserve all four reset states and
save/load real VGA pixels, including a fresh process. S9 remains open for
owner graphics acceptance; earlier P5/P6 passing smokes did not prove pixels.

## Current Technical Baseline

- T63 S1 source baseline ea7e982, admission after 54b2009. No snapshot runtime
  exists yet. Current pause retains a native execution stack; timer producers,
  device queues, CPU hidden state and media need the proposed capture boundary.
  Owner requires one binary snapshot, direct/readonly references only, and
  complete FDD/HDD overlay pages. Existing fixed EXEs remain unchanged.
  Owner narrowed scope after S1: Lib/test-lib unchanged; Common only the two
  machine-state read/write operations and their necessary executor wiring.
  Snapshot file policy stays in App, state encoding/restoration in VM/Compat/MVDM.
  Latest owner admission: read only while running, write only from the
  initial/stopped product state;
  success is ordinary paused (resume/reset/stop retain their meanings).
  VM owns the safe-stop condition and a single 1-second monotonic deadline;
  ordinary pause/debug semantics stay unchanged. S2 adds the isolated Compat
  timer producer capture barrier; save/load and CPU restoration are not yet
  implemented. [Implementation record](../history/M9-T63-machine-snapshots.md).
  P3/P4 add executor-bound CPU observation and copied FETCH/HLT reentry. Real
  nested-BOP, HLT counter/trap and STI/PIC shadow tests pass. The product does
  not bind snapshot operations yet. P5 adds the executor-owned one-second VM
  deadline and combines CPU observation with the timer capture barrier; real
  nested return/HLT timeout and deterministic failure tests pass. Both fixed
  packages rebuilt; full x64/x86 suites each pass 103/103. P6 inventories 88
  CPU/FPU/RAM symbols and proves TLB flush changes a cached translation after
  a page-table edit; the design now preserves TLB data. S2 stays active for
  the remaining selected-state inventory; save/load are not available.
  P7/P8 define lossless pending-event reconstruction and classify the 25
  scheduler data symbols. Real quick/tick tests cover order, remaining count,
  cancellation and zero-delay behavior; both full suites remain 103/103.
  Common owns final PAUSED notification; capture does not drain device queues.
  P9 proves that actual breakpoint tables and deferred NPX exception IP must
  also survive capture, rather than being rebuilt from visible registers.
  P10 classifies SAS/native CPU state; P11 records the selected controller
  receivers; P12 classifies video/input payload versus host resources.
  P13 completes receiver classification for remaining selected host/BIOS/media
  families. P14 then freezes the complete archive-object convergence index and
  explains the only compiler-shape x86/x64 differences. P15 closes S2: final
  full x64/x86 suites pass 103/103; there is still no snapshot container,
  state API or user command. S3 re-audit corrects its initial file-boundary
  conclusion: existing readonly media supplies bounded chunk reads; ordinary
  truncate writer publication is owner-approved, so Lib remains unchanged.
  S4 adds the private fixed-width CPU/SAS/RAM archive and real round-trip
  proof only; it exposes no user command or incomplete snapshot file. S5 P1
  adds a VM/Compat-private controller/queue archive: semantic q/tic callback
  IDs, transactional queue replacement, and PIC/PIT/RTC/DMA/FDD/HDD state.
  P2 rejects an unrepresentable HDD continuation at capture; P3/P4 prove
  port-driven PIC pending IRQ, DMA, FDC and HDD restoration. There is still no
  product save/load path and no Lib/Common change. S6 P2 archives PPI's guest
  latch and speaker edge baselines; S6 P3 archives only finite InPort mouse
  hardware state (deltas, latches, registers and diagnostic handshake). S6 P4
  archives the finite 8042 keyboard-controller state and both reviewed delayed
  callbacks (`do_int` and `allowRefill`) as semantic queue IDs. S6 P5 archives
  the DOS INT 33h driver through a fixed-width semantic map, including its
  inactive/installed distinction, cursor backing, handler segment:offset data
  and saved callback registers; instance allocation, EGA addresses and host
  cursor resources rebuild on restore. S6 P6 adds only the fixed-size four-
  plane VRAM/font bytes and 256-entry DAC, then invalidates host rendering.
  P7 freezes the remaining controller boundary; P8 implements it as a
  fixed-width register/index/attribute-flip-flop/DAC-cursor map plus the two
  live C-VID latch values. Restore replays original controller handlers and
  rebuilds GDP-derived bindings, dirty state and host resources; it does not
  copy bitfield carriers, GDP allocations, pointers or vectors. Any live GDP
  slot outside that map remains a future capture blocker, never implicit
  payload or reset. S6 P9 archives the finite UART/LPT controller and virtual
  host-carrier state, and gives their delayed callbacks semantic queue IDs.
  Configured serial/printer output files remain explicit capture rejection:
  no host file, external output, native handle or path enters the archive.

- T62 S5-S8 implementation 44e9d0f removes four duplicate state/ownership
  paths: nine production C/H files +56/-99 = -43; nine test C/H files
  +141/-38 = +103. Final dual full suites 101/101; post-commit 8/8 each.
  Both fixed EXEs rebuilt, all P commits pushed; public APIs and product
  source/INI/media unchanged. Owner has now authorized T62 closure.
  Delivery recheck: x64 101/101; x86 first 100/101 with an intermittent BIOS
  tick assertion, then five focused passes and full 101/101 unchanged.
  The unconfirmed timing/test issue is recorded in TODO, not claimed repaired.

- T62 S1-S4 implementation aa1eabd: four bounded Common/Lib simplifications,
  production +170/-214 = -44 across 12 C/H paths. Final x64/x86 101/101;
  actual-commit 14/14 each. Both fixed EXEs refreshed; public APIs, App, VM,
  Compat and MVDM unchanged. S1-S4 implementation and review commits are pushed;
  owner manual validation passed for S1-S4; T62 is now closed on owner direction.

- T61 S13 audit delivery 891e642 verifies all S7 ownership/reuse candidates.
  S8-S12 production net -1847; mirror 498 retained/404 identical/94 divergent,
  +23132/-22334, 5131 hunks. Final dual-width 98/98; actual-commit 7/7 each.
  Fixed EXEs remain S12 e2ef91f, hashes in the
  [final ledger](../etc/evidence/softpc/pristine-divergence-current.md#s13-最终归属复核).
  Lib/Common and shared tests unchanged within T61; owner has now closed T61.

- T61 S6 implementation 4f7171d completes the 498-file original-diff audit:
  403 identical, 95 divergent; +23,141/-22,335, 5,132 hunks. Comparable original
  diff reduced by 737 changed lines, production net -539. Full suites 98/98
  and actual-commit focused 8/8 per width; fixed EXEs refreshed. T61 is now
  closed by the whole-task audit linked above. [S6 review](../history/M9-T61-S6-final-mirror-audit.md).

- T61 S5 implementation a65dc18 shares original sound state; original content
  additions reduced from 222 to 64, production net -158. Dual full suites
  98/98 and post-commit sound checks 4/4 each; fixed EXEs refreshed.
  [S5 review](../history/M9-T61-S5-original-sound.md).

- T61 S4 implementation 82fb169 shares original keyboard tables and algorithm;
  production net -436, original content diff +449 reduced to +13. Dual full
  suites 97/97, post-commit 3/3 each, selected tokens identical. Original bounds
  defect separately recorded in TODO. [S4 review](../history/M9-T61-S4-original-keyboard.md).

- T61 S3 implementation dc71195 restores unselected originals with identical
  selected preprocessing and EXE code/data sections. Final dual full suites
  97/97; post-commit focused 4/4 each. First x64 package failure and successful
  repeats are recorded, not erased. [S3 review](../history/M9-T61-S3-original-conditionals.md).

- T61 S2 implementation 6df4104 restores eight mirror files, production +17/-34;
  dual full suites 97/97 and post-commit focused 4/4 each. Fixed EXEs refreshed.
  [S2 review](../history/M9-T61-S2-mirror-format-cleanup.md).

- T61 S1 inventory complete: 498 retained/401 identical/97 divergent; raw and
  whitespace-ignored totals separately recorded. No production changes.
  [S1 review](../history/M9-T61-S1-mirror-inventory.md).

- T60 S7 repairs FIST64 rounding and its representation read, without replacing
  the FPU. Implementation 64d26a0 passed 97/97 on each width, five repeated
  command-provider runs per width and post-commit 3/3 per width. Both fixed EXEs
  are refreshed. [S7 evidence](../history/M9-T60-S7-fpu-assessment.md).

- T60 S6 closes as non-adoption: D6 BOP and C4/C4 BOP remain unchanged on owner
  decision. [Assessment](../history/M9-T60-S6-d6-bop-contract.md). No new EXEs.

- T60 S5 corrects two C-VID writer declarations without changing the generated
  register protocol. Both widths pass 97/97 full and 3/3 post-commit checks;
  fixed EXEs match 3dbaf32. [S5 review](../history/M9-T60-S5-cvid-writer-contract.md).
  This is a latent declaration repair, not reproduced current-build truncation.

- T60 S4 adds planar pel panning in the original standard EGA painters,
  including split cancellation and preceding dirty dependencies. Both widths
  pass 97/97 full suites and 3/3 post-commit checks; fixed EXEs match 8ac53ac.
  Shared corpora are unchanged. [S4 review](../history/M9-T60-S4-pel-panning.md).
- T60 S3 preserves dirty source addresses and corrects split/wrap boundaries
  within the original update entries. Both widths pass the 864-configuration
  matrix, 97/97 full suites and 3/3 post-commit checks. Fixed EXEs match
  6931729; shared corpora are unchanged. [S3 review](../history/M9-T60-S3-dirty-address.md).
- T60 S2 bounds the seven selected EGA/VGA painters before pointer arithmetic
  and loops. Its 91-case pixel/canary matrix and full 97/97 suites pass at both
  widths; post-commit focused tests pass 3/3 each. Fixed EXEs are refreshed.
  [S2 review](../history/M9-T60-S2-painter-bounds.md). Shared corpora unchanged.
- T60 S1 rejects PIC acknowledgement -1 at the original CPU interrupt entry.
  Dual-width full suites pass 97/97 and post-commit focused checks pass 3/3
  at each width. Fixed EXEs are refreshed; shared corpora remain unchanged.
  [S1 review](../history/M9-T60-S1-pic-rejection.md).
- T59 final delivery retains dual-width 97/97 full-suite results and 6/6
  post-commit focused checks. [S22](../history/M9-T59-S22-debug-dos-semantics.md)
  records DOS memory-command semantics, retained interactive E/environment
  behavior and X extensions. Closure changes documentation only; fixed EXEs
  remain the verified S22 builds.
- The fixed standalone package is `assets/binary/softpc32.exe` and
  `softpc64.exe`, with adjacent user-owned `softpc.ini`; reusable guest media
  is below `assets/media/`, and README captures are below `assets/readme/`.
- `src/mvdm/softpc.new/` is the preserved selected SoftPC baseline. Its ROM
  inputs are embedded source-mirror inputs; no runtime ROM artifact root is
  active.
- T58 S4 product baseline retains the T57 app/config, vm and flat compat
  layout; app/composition now owns all entity assembly and event wiring.
  Main loads config. Both fixed packages pass 85/85 CTest. Only composition
  includes the VM interface. Machine shutdown joins callbacks before ordered
  UI/session/command/machine/VM disposal. Common API/tests/manifests changed
  under S4 approval; Lib, VM, Compat and MVDM remain unchanged.
- T58 S5 restores explicit -O3 -DNDEBUG in both GNU package presets; both
  builds and full 85/85 suites pass. x86 is 3,503,225 bytes; rebuilt x64 is
  byte-identical to S4. No source corpus or product semantics changed.
- `src/lib/` is the canonical shared corpus for exact NXVM adoption. Its
  normalized `types`/`console`/`console-broker`/`storage`/`kvm-*` corpus has S18 one-time
  wake selection; the T56 baseline passed x64/x86 83/83 CTest;
  strict standalone library suite passes 38/38. Its path-scoped standalone MSVC
  manifest/build/CTest gate is live in GitHub Actions.

## Recent M9 Closures

T56: Common extraction, debugger integration and bounded follow-up repairs
closed after the [whole-task audit](../history/M9-T56-S19-completion-audit.md).
Known TODOs and external NXVM acceptance remain separate, not claimed fixed.

| T56 S11 | Original CPU trace/break execution and CLI completion connected; watchpoints remain S12. | [S11 closure](../history/M9-T56-S11-debug-execution-plans.md) |
| T56 S10 | Synchronous debug access completed; S11/S12 need explicit port-ABI decision. | [S10 closure](../history/M9-T56-S10-synchronous-debug-access.md) |
| T56 S4 | Generic executor, lifecycle/input queues, run generation and completed-frame publication now belong to `common/machine`; app retains only its injected SoftPC driver. | [S4 closure](../history/M9-T56-S4-common-machine-extraction.md) |
| T56 S5 | App/host direct lib ownership was audited; the speaker worker now has one explicit app-managed lifecycle. | [S5 closure](../history/M9-T56-S5-ownership-audio-lifecycle.md) |
| T56 S6 | Exact NXVM xasm32 source is a dormant `common-xasm32` component with copied byte/text contract tests. | [S6 closure](../history/M9-T56-S6-xasm32-source-provenance.md) |
| T56 S7 | Exact NXVM debug source is a dormant `common-debug` component behind a typed optional paused-state machine adapter. | [S7 closure](../history/M9-T56-S7-debug-source-provenance.md) |
| T56 S8 | Boundary gates prohibit app/host reaches into common implementation; common documentation and CMake ownership have converged. | [S8 closure](../history/M9-T56-S8-common-corpus-convergence.md) |

| Task | Closure | Evidence |
| --- | --- | --- |
| T62 | S1-S8 complete; owner-directed closure; final dual-width 101/101 with separately recorded intermittent tick debt. | [Whole-task audit](../history/M9-T62-completion-audit.md) |
| T61 | S1-S13 complete; owner-directed closure; 498-file mirror and VM/Compat ownership audit, final dual-width 98/98. | [Whole-task audit](../history/M9-T61-completion-audit.md) |
| T60 | S1-S7 complete; owner manual acceptance; D6 retained, dual-width 97/97, protected corpora unchanged. | [Whole-task audit](../history/M9-T60-completion-audit.md) |
| T59 | S1–S22 closed on owner request; TODO cleared by explicit retirement, final dual-width 97/97. | [Whole-task audit](../history/M9-T59-completion-audit.md) |
| T58 | Common test synchronization and owner-admitted App/build follow-ups closed; S1–S5 complete, dual-width 85/85. | [Whole-task audit](../history/M9-T58-completion-audit.md) |
| T57 | App/config, VM and flat Compat refactor closed on owner approval; protected corpora unchanged, dual-width 85/85. | [S3 closure](../history/M9-T57-S3-completion-audit.md) |
| T56 | Common extraction, debug integration and follow-up repairs closed on owner request after whole-task audit; known debts retained. | [S19 final audit](../history/M9-T56-S19-completion-audit.md) |
| T55 | Canonical shared library refreshed, normalized, simplified, boundary-gated, and renamed to the KVM corpus; owner accepted final package testing. | [T55 closure](../history/M9-T55-closure-kvm-corpus.md) |
| T49 | Shared-library quality sequence S1–S6 closed after owner x86/x64 package acceptance. | [T49 S6 history](../history/M9-T49-S6-lib-neutral-corpus-prose.md) |
| T50 | Build presets/artifact identity and standalone shared-library MSVC CI closed. | [T50 S2 history](../history/M9-T50-S2-lib-ci.md) |
| T51 | Shared library normalized to `types`, `console`, `host`, `storage`, and independent presentation components; dual-width verification closed. | [T51 S4 history](../history/M9-T51-S4-lib-component-normalization-closure.md) |
| T52 | Native Console pointer capture was withdrawn: supported terminal hosts cannot provide a reliable visible client rectangle; no implementation remains. | [T52 record](../history/M9-T52-console-pointer-capture-withdrawn.md) |
| T53 | Raw Console one-cell input now maps to eight logical units on both axes; dual-width 37/37 verification closed. | [T53 S1 closure](../history/M9-T53-S1-console-logical-mouse-scale.md) |
| T54 | Restored the original X×8/Y×16 raw-Console InPort conversion, removed app-side mouse merging, and proved original InPort accumulation; owner accepted the balanced physical-input behavior. | [T54 S1 closure](../history/M9-T54-S1-console-mouse-scale-closure.md) |

- **M9 T50 S1:** Checked-in x64/x86 presets now bind compiler width to package
  identity and passed fresh 36/36 CTest at both widths.
  [Record](../history/M9-T50-S1-build-presets-artifact-identity.md)

- **M9 T50 S3:** The final post-library source tree rebuilt both fixed package
  widths and passed 36/36 CTest at each width.
  [Record](../history/M9-T50-S3-dual-width-post-lib-proof.md)

- **M9 T51 S2:** The entire library and every active SoftPC consumer moved in
  one buildable rename to `types`, `console`, and presentation components; fixed x64/x86 CTest
  each passed 36/36. [Record](../history/M9-T51-S2-lib-component-normalization.md)

- **M9 T51 S3:** Current architecture, presentation design, source-layout, component README,
  and executable boundary authority now agree on the normalized component DAG.
  [Record](../history/M9-T51-S3-component-boundary-authority.md)

- **M9 T51 S4:** Final manifest, boundary/governance, zero-old-name, and fresh
  fixed x64/x86 36/36 proofs passed; T51 is closed.
  [Record](../history/M9-T51-S4-lib-component-normalization-closure.md)

## Recent Governance

- **M9 Td S13:** Common extraction now permits responsibility-owned direct lib
  calls outside common; migration and acceptance reject duplicate ownership,
  not legitimate dependencies. [Record](../history/M9-Td-S13-common-lib-boundary.md)

- **M9 Td S12:** SoftPC-first common extraction now has five component ownership
  boundaries and eight sequential dual-width executable acceptance stages.
  [Record](../history/M9-Td-S12-common-extraction-plan.md)

- **M9 Td S6:** Console-object design was promoted to current architecture/presentation
  authorities. [Record](../history/M9-Td-S6-console-object-governance.md)

- **M9 Td S7:** Active-packet contract, identifier-mode checks, Queue/TODO
  integrity, CTest self-test, and all stale-proposal archival are now
  executable. [Record](../history/M9-Td-S7-executable-governance-state.md)

- **M9 Td S8:** Source provenance, research, legacy-material, and
  license-decision boundaries are now explicit and verifier-backed.
  [Record](../history/M9-Td-S8-source-research-policy.md)

- **M9 Td S9:** Future-task closure audit, whole-domain convergence, complete
  P discipline, path accounting, and build hygiene now match the relevant
  NXVM governance standard. [Record](../history/M9-Td-S9-execution-closure-quality.md)

## M9 T63 S9 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner admitted S9 after S8 P7. Owner explicitly rejects retained `32`/`64` header markers and arbitrary RAM+allowance/device limits. A minimal Lib Storage streaming-reader addition is approved. P2 is owner-reported corrective scope: a stopped-to-paused restore must not create a Window solely from a restored graphics frame. P3 is owner-reported corrective scope: after `load` then `resume`, Window must wait for a complete restored graphics frame rather than display a text fallback as a black, wrongly sized Window. |
| Objective | Replace the bounded whole-file snapshot load and buffered two-section VM container with one width-free, versioned streaming format. Snapshot size follows actual serialized RAM/device/media content and ordinary allocation/I/O failure, not fixed product allowances. |
| Non-goals | No Common public API change, no Session/UI/debug API, no generic callback/task escape hatch, no second executor, no raw legacy structure dump, and no change to ordinary pause/debug/KVM behavior. This S does not claim the separately planned overlay-page payload is already implemented. |
| Reference Baseline | S8 P7 at `46baf5c`; it proves a transitional fixed-`32` header cross-width but still buffers reads and imposes RAM+8 MiB/device 4 MiB limits. |
| Candidate Proposal | [Snapshot design](../proposals/m9-machine-snapshots.md) |
| Files And ABI Surface | `src/app/{command,composition}.c/.h`, `src/vm/{snapshot_image,driver}.c`, `src/compat/{video,devices/*}.c/.h`, necessary original SoftPC video receiver lines, `src/lib/storage/file*`, `src/common/session/control_state.*`, storage/App/VM/Common snapshot tests and this proposal/evidence only. Lib adds one neutral reader handle mirroring its writer; App passes that stream through the existing opaque Common callback. Session's private paused-Window retention bit and VM's restored-graphics publication are corrected without changing public ABI. The VM-private header is width-free fixed-endian data, not a public/Common/Lib ABI. |
| Applicable Rules | `docs/rules/EXECUTION.md`, `ARCHITECTURE.md`, `CODING.md`, `DOCUMENT.md`; `docs/design/ARCHITECTURE.md`, `CODING.md`, `UI.md`; active proposal and source-boundary gates. |
| Verification | Storage streaming-reader tests; VM malformed/short-section proof; a real App-provider save/stop/load/resume transaction; x64→x86 and x86→x64 fresh-process proof; full sequential x64/x86 CTest, package builds and actual-commit review. |
| Expected Markers | Only `save` from running is admitted and succeeds as ordinary PAUSED; only `load` from stopped (including the just-started monitor) is admitted and succeeds as ordinary PAUSED. A stopped-to-paused load does not create a Window; a paused Window that already exists remains visible, and resume resumes normal presentation routing. A restored graphics route publishes only a complete graphics frame—never a text fallback—so resumed Window geometry/content follows the restored painter. Direct/readonly media remain referenced by their configured source; existing VM image semantics govern all currently archived state. New snapshots have no width field and load across x86/x64 packages when machine configuration matches. App produces one explicit result and one prompt, never a false success or an extra lifecycle request. |
| Asset Needs | No user media, user snapshot file or INI mutation. Tests create/remove snapshot fixtures below their own build working directory only. Packaging refreshes only the two approved EXEs. |
| Reporting Requirements | Before code report exact App action, callback, file and size-limit flow; after proof report actual production/test numstat, command/state matrix, tests and both EXE links. |
| Stop Conditions | A necessary Common API; current canonical image cannot represent a promised medium state; or required streaming cannot be expressed by the approved minimal Storage reader. Record evidence before expanding scope. |
| Exit Criteria | No Common public API changes; VM sections stream without width or arbitrary size fields; App never preloads a snapshot; every accepted save/load state/result is correct; a load never synthesizes a Window; errors never claim success; bidirectional cross-width proof and required gates are committed/pushed. |
| Original Owner Request | One binary snapshot eventually, direct/readonly references and FDD/HDD overlays; Common only two state operations and necessary wiring. Save only running, load only from the initial/stopped product state, success ordinary paused with working resume/reset; VM safe-stop deadline one second. Owner later requires one width-free x86/x64 format and rejects arbitrary snapshot-size allowances. |
| Similar-Issue Sweep | Existing synchronous media/debug rendezvous, lifecycle completion ordering, run-generation invalidation, CCPU restore/reentry, timer capture finish paths and every Common driver callback that can mutate executor-owned state. |
