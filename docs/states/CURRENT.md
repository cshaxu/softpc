# Project Status

## Current Work

T60 is closed after owner manual acceptance and the whole-task audit.
M9 T61 S1-S6 are closed as verified implementation deliveries. T61 remains
open; owner admits S7 for remaining mirror and VM/Compat reuse auditing.
The three queued candidates are unchanged.

## M9 T61 S7 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner requests remaining MVDM diff and VM/Compat duplication/reuse audit; single-person dual-role. |
| Objective | Review remaining mirror differences, original/local support duplicates, additional OpenNT mirror reuse and bidirectional Mirror/VM/Compat responsibility placement, preserving semantics and minimizing total complexity. |
| Non-goals | No production change, source import, shared-corpus edit, ABI change, behavior withdrawal or new EXEs. |
| Reference Baseline | SoftPC 34f7182; S6 498-path ledger; read-only OpenNT 5e4619ab61c2aa76151e03973cce340be2933e61. |
| Candidate Proposal | [Remaining mirror and support ownership audit](../proposals/m9-mirror-support-ownership-audit.md); original requests remain in the retained T61 plan. |
| Files And ABI Surface | Read all 9 VM and 39 Compat files, remaining 95 mirror differences and original counterparts/build consumers; write audit/governance documentation only. |
| Applicable Rules | Execution, Document, Architecture, Coding, source-research policy and their governance skills. |
| Verification | Finite path coverage, function/state ownership, real callers/build selection, original counterpart and dependency closure; report removal/import/adapter costs and required equivalence tests; doc gate and unchanged production/artifact hashes. |
| Expected Markers | Evidence-backed candidates distinguish duplicates, necessary adapters, local repeats and new-mirror opportunities; no unreviewed member labelled safe. |
| Asset Needs | Keep S6 dual-width artifacts, user INI and media unchanged; read-only audit requires no rebuild. |
| Reporting Requirements | Per-candidate paths/functions, reason, retained owner, line ranges, dependency/behavior risks and verification plan; complete coverage and explicit limits. |
| Stop Conditions | No speculative deletion or import, second implementation, weakened compatibility or assumed external license; implementation requires owner decision after audit. |
| Exit Criteria | Reconciled coverage ledger and findings, actual-code coordinator review, committed/pushed report and clean tree; T61 remains open for owner decisions. |
| Original Owner Request | 再次审计剩余mvdm的diff；特别关注vm和compat与原始代码重复、内部重复，以及引入新OpenNT镜像文件减少支持代码的机会。同时也要审计：镜像里应当归属vm或compat的部分，vm和compat应当归属镜像的部分，vm和compat应当互相归属的部分。 |
| Similar-Issue Sweep | All VM/Compat C/H and retained mirror diff groups; functions/tables/constants, lifecycle/state and wrappers including compile-time selection and transitive original dependencies. |

## Current Technical Baseline

- T61 S6 implementation 4f7171d completes the 498-file original-diff audit:
  403 identical, 95 divergent; +23,141/-22,335, 5,132 hunks. Comparable original
  diff reduced by 737 changed lines, production net -539. Full suites 98/98
  and actual-commit focused 8/8 per width; fixed EXEs refreshed. T61 awaits
  owner testing. [S6 review](../history/M9-T61-S6-final-mirror-audit.md).

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
