# Project Status

## Current Work

M9 T56 S14 passed owner visual testing ("确认没问题，跟之前不一样了").
[S14 closure](../history/M9-T56-S14-console-display-handoff.md) records its delivery.
T56 S15 is verified and delivered in `864a74f` / `4cf8629`.
T56 S16 implementation is verified: Common physical-key identity corrected;
shared tests relocated to test/common and test/lib. Both final product suites
passed 79/79; four-directory isolation passed Lib 37/37 and Common 14/14.
Executor delivery `09d8c90` is pushed. Coordinator actual-diff review passed:
existing test assertions retained, no public ABI change, no product-source
dependency in the isolated shared suites. Owner rejected S16 testing: first
start exits with host I/O error. The S16 correction now positions the viewport
before ensuring frame capacity; x86/x64 full regression passes 80/80 and strict
standalone Lib passes 37/37. Corrective commit and actual-diff review follow.
T56 remains open; the suspended S13 whole-task audit is not claimed complete.

## M9 T56 S16 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner admits S16 and requires removal of common/test; src/lib, src/common, test/lib and test/common must transfer unchanged to NXVM. |
| Objective | Correct source-local physical key retirement, externalize shared tests, and repair the owner-reproduced first-start Console I/O failure without changing lifecycle policy. |
| Non-goals | No lifecycle, MVDM, CLI, focus, display or executor topology changes; no Common win32/linux implementations; no T closure. |
| Reference Baseline | 4cf8629; S15 x86/x64 68/68, strict Lib 8/8 and isolated Common 4/4. |
| Candidate Proposal | [Common convergence](../proposals/m9-common-corpus-convergence.md). |
| Files And ABI Surface | Common pressed-key comparison; shared suites; Host Win32 text-surface geometry and native display regression; no new public ABI. |
| Applicable Rules | Execution, documentation, architecture and coding authorities. |
| Verification | Corrected x64/x86 80/80 and strict Lib 37/37; native narrow-frame regression fails before and passes after; compact first-command start, DOS input/CLS, pause/stop/restart pass. Prior Common 14/14 isolation remains unchanged. Intermittent stage-14 observation is not claimed fixed. |
| Expected Markers | No src/common/test; shared tests have no app/host/MVDM or product-root dependency; EXTENDED distinguishes physical keys. |
| Asset Needs | Both fixed EXEs; bounded logs and disposable standalone verification trees under build; preserve INI and media. |
| Reporting Requirements | Complete P commit/push, independent actual-diff review, counted production/test changes and clean tree. |
| Stop Conditions | Product semantic changes, MVDM changes or a new synchronization framework require separate approval. |
| Exit Criteria | Platform boundary and standalone proof pass; dual-width packages and regression delivered, reviewed and pushed. |
| Original Owner Request | 准入。同时增补任务要求：common/test这个玩意要去掉。我需要你在 test/目录下增加 common 和 lib 用于覆盖当前 common 和 lib 的单元测试。src/lib, src/common, test/lib, test/common，这几个将来都要原样导入给nxvm的。反馈：失败：启动后输入start直接闪退。继续 找到原因然后彻底修复。 |
| Similar-Issue Sweep | Shared-test and Common key-identity sweeps retained; all Host native geometry writers examined. Palette precedes frame-capacity proof; output switching preserves existing owner. Horizontal-scrollback metadata probe recorded separately in TODO, not claimed fixed. |

## Current Technical Baseline

- The fixed standalone package is `assets/binary/softpc32.exe` and
  `softpc64.exe`, with adjacent user-owned `softpc.ini`; reusable guest media
  is below `assets/media/`, and README captures are below `assets/readme/`.
- `src/mvdm/softpc.new/` is the preserved selected SoftPC baseline. Its ROM
  inputs are embedded source-mirror inputs; no runtime ROM artifact root is
  active.
- `src/lib/` is the canonical shared corpus for exact NXVM adoption. Its
  normalized `types`/`console`/`host`/`storage`/`kvm-*` corpus has S16 x64/x86
  corrected product builds with 80/80 CTest;
  strict standalone library suite passes 37/37. Its path-scoped standalone MSVC
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
