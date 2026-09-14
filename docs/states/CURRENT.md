# Project Status

## Current Work

M9 T56 S9 is active: owner reopened the closed task; retain the completed
monitor-spacing repair and connect the admitted common debug command path.
S9 P5 is implemented and verified: x86/x64 each pass 63/63 tests, strict
standalone lib passes 8/8, and both package EXEs are refreshed. Await owner
testing; neither S9 nor T56 is closed by this delivery.
Owner approved the follow-on debug capability design: S10 synchronous access,
S11 execution control, S12 watchpoints and end-to-end acceptance, detailed in
the linked proposal. These are sequential planned scopes, not concurrent active
packets; S9 remains the sole active step pending feedback.

## M9 T56 S9 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Owner-Reopen |
| Admission And Approval | Owner explicitly withdrew the premature S9 closure and admitted debug-command connection within S9; permanent commit/push authority remains in effect. |
| Objective | Retain monitor spacing and connect debug CLI independently of VM state; only synchronous machine access requires PAUSED through common/machine. |
| Non-goals | Do not bypass common/machine, create a second executor, directly access MVDM from common/debug, or alter existing monitor command/lifecycle semantics. |
| Reference Baseline | S9 P1 `1750ed7` normalized all nonempty monitor output; P2 dual packages built at `de2ff79`, but the owner kept S9 open for debug connection. |
| Candidate Proposal | [Common convergence](../proposals/m9-common-corpus-convergence.md). |
| Files And ABI Surface | Common debug/session injection, SoftPC debug driver adapter and focused tests; machine access stays behind copied request/result. Disassembly now returns instruction byte count separately from text length: the live U test exposed their conflation and a zero-progress loop. |
| Applicable Rules | Execution, architecture, coding and documentation authorities; Product UI; common ownership boundary. |
| Verification | Audit every debug operation against the product driver; test debug entry/stay/exit in INIT, STOPPED, RUNNING and PAUSED without implicit machine access or pause; test CAP while debug remains active, lazy register defaults, explicit non-paused access errors, lease invalidation and unsupported operations; fresh package-x86/x64, test-x86/x64, strict lib and governance gates. |
| Expected Markers | Monitor dispatches a debug session only through common/session injection; common/debug reaches SoftPC only through common/machine's lease callback; no MVDM include outside the product adapter. |
| Asset Needs | Refresh only `assets/binary/softpc32.exe` and `softpc64.exe`; preserve adjacent user-owned INI and all media bytes. |
| Reporting Requirements | Every P is committed and pushed; final report includes both EXE links, hashes and x86/x64 test evidence. |
| Stop Conditions | Stop if a required debug operation has no safe product adapter or needs an MVDM semantic change; record each unsupported operation rather than inventing an unsafe direct path. |
| Exit Criteria | Supported debug commands work via the typed machine lease, unsupported ones are explicit, focused and full dual-width tests pass, packages refresh, review/push complete and worktree is clean. |
| Original Owner Request | S9 暂不收口，因为还有一个问题：debug命令没有接通无法使用。后续批准：是否可以进入和保持在debug cli，是用户决定 和机器状态无关；只有debug调用机器的同步读写接口，才要paused；如果此时没有paused状态便会报错。好 准入s9。 |
| Similar-Issue Sweep | CLI provider injection, debug lifetime/open/close, all debug operations, machine lease invalidation, product driver callbacks, prompt ownership and no-direct-MVDM boundary. |

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

| T56 S4 | Generic executor, lifecycle/input queues, run generation and completed-frame publication now belong to `common/machine`; app retains only its injected SoftPC driver. | [S4 closure](../history/M9-T56-S4-common-machine-extraction.md) |
| T56 S5 | App/host direct lib ownership was audited; the speaker worker now has one explicit app-managed lifecycle. | [S5 closure](../history/M9-T56-S5-ownership-audio-lifecycle.md) |
| T56 S6 | Exact NXVM xasm32 source is a dormant `common-xasm32` component with copied byte/text contract tests. | [S6 closure](../history/M9-T56-S6-xasm32-source-provenance.md) |
| T56 S7 | Exact NXVM debug source is a dormant `common-debug` component behind a typed optional paused-state machine adapter. | [S7 closure](../history/M9-T56-S7-debug-source-provenance.md) |
| T56 S8 | Boundary gates prohibit app/host reaches into common implementation; common documentation and CMake ownership have converged. | [S8 closure](../history/M9-T56-S8-common-corpus-convergence.md) |

| Task | Closure | Evidence |
| --- | --- | --- |
| T56 | SoftPC-first common corpus closed through S8: UI, session, machine, xasm32 and debug each have one owner; S9 is owner-reopened for debug-command connection. | [S8 closure](../history/M9-T56-S8-common-corpus-convergence.md) |
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
