# Project Status

## Current Work

T60 is closed after owner manual acceptance and the whole-task audit.
T61 is closed on owner direction after the [whole-task audit](../history/M9-T61-completion-audit.md).
T62 is closed on owner direction after the
[whole-task audit](../history/M9-T62-completion-audit.md).
M9 T63 S1 is closed after the [design review](../history/M9-T63-S1-machine-snapshots.md).
M9 T63 S2 is closed after the field inventory, cross-width state-owner index
and running-save safe-stop boundary proof. M9 T63 S3 is active: binary
container and media-base transaction pre-audit.
[Delivery and acceptance ledger](../history/M9-T62-common-lib-simplification.md).
T62 closure was pushed in 54b2009 before T63 admission. The three older
candidates remain queued. [Snapshot proposal](../proposals/m9-machine-snapshots.md).

## Current Technical Baseline

- T63 S1 source baseline ea7e982, admission after 54b2009. No snapshot runtime
  exists yet. Current pause retains a native execution stack; timer producers,
  device queues, CPU hidden state and media need the proposed capture boundary.
  Owner requires one binary snapshot, direct/readonly references only, and
  complete FDD/HDD overlay pages. Existing fixed EXEs remain unchanged.
  Owner narrowed scope after S1: Lib/test-lib unchanged; Common only the two
  machine-state read/write operations and their necessary executor wiring.
  Snapshot file policy stays in App, state encoding/restoration in VM/Compat/MVDM.
  Latest owner admission: read only while running, write only init/stopped;
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
  state API or user command. S3 is now a binary-container/media pre-audit.

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

## M9 T63 S3 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner explicitly requests one binary snapshot under running-only read and init/stopped-only write; S2 is closed and S3 is admitted under that unchanged product contract. |
| Objective | Pre-audit the versioned binary container and explicit media-base transaction before encoding any CPU/device payload; retain App path/file ownership and VM/Compat format ownership. |
| Non-goals | No save/load CLI or partial container; no direct storage dump, second overlay implementation, changed device semantics or user media. No Lib changes and no Common changes in this pre-audit. |
| Reference Baseline | S2 closure d4dda9d; fixed package code unchanged since P5. |
| Candidate Proposal | [Snapshot design](../proposals/m9-machine-snapshots.md) |
| Files And ABI Surface | Audit App command/file seams, VM media adapters and existing Lib storage usage only. Do not add an ABI or encode payload until exact container field limits, media fingerprints and failure transaction are written and reviewed. |
| Applicable Rules | docs/rules/EXECUTION.md and DOCUMENT.md; design/ARCHITECTURE.md, CODING.md, UI.md; referenced execution, architecture and documentation skills. |
| Verification | Container parser/writer plan, direct/readonly/overlay media caller sweep and failure-atomicity design. If code changes later, fixed x86/x64 package builds and full CTest plus focused storage/media tests and manifest/boundary gates. Documentation gate and actual-commit review. |
| Expected Markers | One bounded little-endian format, no pointer/struct dump, direct/readonly reference-only media, full overlay differences, and an explicit failure path that leaves source media/INI untouched. |
| Asset Needs | No user media mutation, no user snapshot file and no raw trace/recording. Future container tests must own and remove build-local fixtures. |
| Reporting Requirements | Before code report exact files/ABI and estimated churn; after proof report production/test numstat and original mirror diff separately, tests and both EXE links. |
| Stop Conditions | Missing bounded file/media capability, required Lib/other Common expansion, or an unrepresentable media-base transaction: report before editing; no direct raw dump or guest reset substitute. |
| Exit Criteria | Complete container/media design, exact adapter ownership and failure semantics; no partial command or user-media mutation; required evidence, commit/push and independent actual-change review. |
| Original Owner Request | One binary file, direct/readonly references and FDD/HDD overlays; no Lib edits, Common only two state operations and necessary wiring. Latest: save only running, load only init/stopped, success ordinary paused with working resume/reset; VM safety timeout 1 second. Verbatim and matrix in proposal. |
| Similar-Issue Sweep | Every executor callback caller including nested host_simulate and HLT, timer producer/consumer and queued input; classify CPU/FPU/memory/video/controllers/media/host resources as save, rebuild, external or unselected with evidence. |
