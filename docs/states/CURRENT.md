# Project Status

## Current Work

M9 T56 S12 is closed after implementation `edc7065` and coordinator review.
S13 is admitted for the separate T-level closure audit under the owner's
sequential-task authorization. T56 remains open; both refreshed packages are
available for asynchronous owner testing.

Debug/xasm32 integration now has all 17 machine debug operations connected,
including original-CPU trace/break/watch completion and copied operand records.
Final x64/x86 suites passed 63/63 each; strict lib passed 8/8. Architectural
CR1/CR4 non-applicability and the separately recorded x87 observation are not
claimed fixed. See [S12 closure](../history/M9-T56-S12-debug-watch-integration.md).

## M9 T56 S13 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner authorized sequential S completion without waiting for manual acceptance; this is the required T-level audit of the original T56 objective, not new product scope. |
| Objective | Map original common extraction and admitted S1–S12 deliveries to their boundaries, deletion ledger, debug/xasm integration and full regression evidence before deciding T56 closure. |
| Non-goals | No additional product features, CPU algorithm changes, NXVM writes or premature T closure. |
| Reference Baseline | Reviewed S12 implementation edc7065; unchanged x86/x64 packages passed 63/63 each and strict lib 8/8. |
| Candidate Proposal | [Common convergence](../proposals/m9-common-corpus-convergence.md). |
| Files And ABI Surface | Current/proposal/history and original-request coverage audit; no new production ABI admitted. |
| Applicable Rules | Execution, documentation, architecture, coding authorities and Product UI. |
| Verification | Actual S commits, complete migration/operation ledger, retained TODO ownership, dual-width final evidence and documentation gate. |
| Expected Markers | Every extracted owner has one production path; no dormant debug adapter or duplicate executor; deferred unrelated issues are explicit. |
| Asset Needs | Both fixed S12 EXEs remain available; no INI/media writes. Any necessary code repair requires bounded scope and refreshed dual-width proof. |
| Reporting Requirements | Audit against actual changes, not summaries; commit/push every completed P and preserve a clean tree. |
| Stop Conditions | Missing original-request proof or new behavior repair requires an explicit disposition before T closure. |
| Exit Criteria | T-level ledger supports a truthful closure or names the exact remaining in-scope work; no T closure merely because S12 passed. |
| Original Owner Request | Complete SoftPC-first common extraction and debug/xasm32 integration; automatically continue sequential S deliveries with both EXEs, tests, commits and pushes. |
| Similar-Issue Sweep | All transferred app/host owners, allowed direct lib calls, common component boundaries, dormant/duplicate paths, source provenance and actual command consumers. |

## Current Technical Baseline

- The fixed standalone package is `assets/binary/softpc32.exe` and
  `softpc64.exe`, with adjacent user-owned `softpc.ini`; reusable guest media
  is below `assets/media/`, and README captures are below `assets/readme/`.
- `src/mvdm/softpc.new/` is the preserved selected SoftPC baseline. Its ROM
  inputs are embedded source-mirror inputs; no runtime ROM artifact root is
  active.
- `src/lib/` is the canonical shared corpus for exact NXVM adoption. Its
  normalized `types`/`console`/`host`/`storage`/`kvm-*` corpus has S21 x64/x86
  builds with 59/59 CTest;
  strict standalone library gates pass 8/8. Its path-scoped standalone MSVC
  manifest/build/CTest gate is live in GitHub Actions.

## Recent M9 Closures

| T56 S11 | Original CPU trace/break execution and CLI completion connected; watchpoints remain S12. | [S11 closure](../history/M9-T56-S11-debug-execution-plans.md) |
| T56 S10 | Synchronous debug access completed; S11/S12 need explicit port-ABI decision. | [S10 closure](../history/M9-T56-S10-synchronous-debug-access.md) |
| T56 S4 | Generic executor, lifecycle/input queues, run generation and completed-frame publication now belong to `common/machine`; app retains only its injected SoftPC driver. | [S4 closure](../history/M9-T56-S4-common-machine-extraction.md) |
| T56 S5 | App/host direct lib ownership was audited; the speaker worker now has one explicit app-managed lifecycle. | [S5 closure](../history/M9-T56-S5-ownership-audio-lifecycle.md) |
| T56 S6 | Exact NXVM xasm32 source is a dormant `common-xasm32` component with copied byte/text contract tests. | [S6 closure](../history/M9-T56-S6-xasm32-source-provenance.md) |
| T56 S7 | Exact NXVM debug source is a dormant `common-debug` component behind a typed optional paused-state machine adapter. | [S7 closure](../history/M9-T56-S7-debug-source-provenance.md) |
| T56 S8 | Boundary gates prohibit app/host reaches into common implementation; common documentation and CMake ownership have converged. | [S8 closure](../history/M9-T56-S8-common-corpus-convergence.md) |

| Task | Closure | Evidence |
| --- | --- | --- |
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
