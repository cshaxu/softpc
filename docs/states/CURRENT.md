# Project Status

## Current Work

T60 S1-S3 are closed after actual-commit review. T60 remains open under the
owner-approved continuing goal; S4 is next in the
[reference-repair plan](../history/M9-T60-reference-repairs-proposal.md).
T60 S4 is active under that continuing goal.
Lib/Common and their shared tests/manifests remain protected: changes need
separate owner approval throughout T60. The other queue candidates retain order.

## M9 T60 S4 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner-admitted T60 serial plan and continuing goal; single-person executor/coordinator. |
| Objective | Correct horizontal pel-panning register state, invalidation and selected graphics output. |
| Non-goals | No shared-corpus edits, new renderer, global alignment owner or opportunistic MVDM refactor. |
| Reference Baseline | Closed S3 implementation 6931729; read-only NTVDMx64 84a13d2 and original OpenNT. |
| Candidate Proposal | [Pel-panning repair](../proposals/m9-pel-panning.md). |
| Files And ABI Surface | Original attribute/video state and selected painters after inspection; product VGA test; no public ABI change planned. |
| Applicable Rules | Architecture, Coding, Execution, Document; source-research policy and governance skills. |
| Verification | Register-only refresh, zero-pan, displacement, split and dirty/full tests; serial full x86/x64 suites and EXEs. |
| Expected Markers | Panning-only changes visibly update pixels without altering zero-pan output. |
| Asset Needs | Existing product fixtures; no guest media or INI changes. |
| Reporting Requirements | Original-source diff necessity, complete finite ledger, exact dual-width proof and actual-commit review. |
| Stop Conditions | Shared-corpus change, new renderer or ownership requires owner decision. |
| Exit Criteria | Focused and full proofs, executor commit/push, coordinator actual-diff review and closure commit/push. |
| Original Owner Request | Execute the admitted ntvdmx64 reference-guided MVDM repairs; preserve minimum original-code diff. |
| Similar-Issue Sweep | Register writes, panning state readers, invalidation, selected painters and split-region panning behavior. |

## Current Technical Baseline

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
