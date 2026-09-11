# Project Status

## Current Work

M9 T54 S1 is active: correct T53 raw-Console mouse-scale regression.

## M9 T54 S1 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | New |
| Admission And Approval | Owner directed withdrawal of T53's unsupported scale change and authorized corrective implementation, dual-width test, commit, and push. |
| Objective | Restore the established raw Console character-cell conversion of X×8 and Y×16 mickeys, preserve individual FIFO records, and split relative movement into stable InPort packets. |
| Non-goals | No host, MVDM, UI Window private coalescing, Console capture/broker, guest-driver, or `softpc.ini` changes. |
| Reference Baseline | `13c5075` / `87df10f`; production comparison also uses pre-T53 `291afe4`. |
| Candidate Proposal | [M9 T54 Console Mouse Scale Correction](../proposals/m9-t54-console-mouse-scale-correction.md). |
| Files And ABI Surface | `ui-console` conversion, app input FIFO and guest mouse-packet adapter, focused FIFO/packet smokes and CMake registration, T53-only smoke removal, library manifest, records, and rebuilt package EXEs; no public ABI. |
| Applicable Rules | Execution, Architecture, Coding, Documentation, Architecture design, and UI design authorities. |
| Verification | Exact path comparison to T51/T53 baselines; manifest/governance/diff gates; fresh x64/x86 build and CTest. |
| Expected Markers | Y conversion is sixteen; two same-button mouse events remain two FIFO entries; `{8,16}` becomes four packets of at most four units per axis with exact conserved sum; no T53 mouse-scale target remains. |
| Asset Needs | Refresh only `softpc32.exe` and `softpc64.exe`; preserve `softpc.ini` and media. |
| Reporting Requirements | Report exact retained/reverted path set, dual-width evidence, commits, and package links. |
| Stop Conditions | Stop for any required change outside the declared paths. |
| Exit Criteria | T53 source assumption is removed, exact scope audit passes, both widths pass, and all changes are pushed and closed. |
| Original Owner Request | “那我怎么知道你没有改掉其他不该改的地方？…行。你处理掉吧。” |
| Similar-Issue Sweep | Compare all T53-touched paths to `291afe4`; inspect every app-side mouse coalescing site and retain only UI-leaf-private coalescing. |

## Current Technical Baseline

- The fixed standalone package is `assets/binary/softpc32.exe` and
  `softpc64.exe`, with adjacent user-owned `softpc.ini`; reusable guest media
  is below `assets/media/`, and README captures are below `assets/readme/`.
- `src/mvdm/softpc.new/` is the preserved selected SoftPC baseline. Its ROM
  inputs are embedded source-mirror inputs; no runtime ROM artifact root is
  active.
- `src/lib/` is the canonical shared corpus for exact NXVM adoption. Its
  normalized `types`/`console`/`host`/`storage`/`ui-*` corpus passes 38/38
  fixed x64 and x86 CTest; its path-scoped standalone MSVC
  manifest/build/CTest gate is live in GitHub Actions.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T49 | Shared-library quality sequence S1–S6 closed after owner x86/x64 package acceptance. | [T49 S6 history](../history/M9-T49-S6-lib-neutral-corpus-prose.md) |
| T50 | Build presets/artifact identity and standalone shared-library MSVC CI closed. | [T50 S2 history](../history/M9-T50-S2-lib-ci.md) |
| T51 | Shared library normalized to `types`, `console`, `host`, `storage`, and independent `ui-*` components; dual-width verification closed. | [T51 S4 history](../history/M9-T51-S4-lib-component-normalization-closure.md) |
| T52 | Native Console pointer capture was withdrawn: supported terminal hosts cannot provide a reliable visible client rectangle; no implementation remains. | [T52 record](../history/M9-T52-console-pointer-capture-withdrawn.md) |
| T53 | Raw Console one-cell input now maps to eight logical units on both axes; dual-width 37/37 verification closed. | [T53 S1 closure](../history/M9-T53-S1-console-logical-mouse-scale.md) |

## Recent M9 Task Progress

- **M9 T50 S1:** Checked-in x64/x86 presets now bind compiler width to package
  identity and passed fresh 36/36 CTest at both widths.
  [Record](../history/M9-T50-S1-build-presets-artifact-identity.md)

- **M9 T50 S3:** The final post-library source tree rebuilt both fixed package
  widths and passed 36/36 CTest at each width.
  [Record](../history/M9-T50-S3-dual-width-post-lib-proof.md)

- **M9 T51 S2:** The entire library and every active SoftPC consumer moved in
  one buildable rename to `types`, `console`, and `ui-*`; fixed x64/x86 CTest
  each passed 36/36. [Record](../history/M9-T51-S2-lib-component-normalization.md)

- **M9 T51 S3:** Current architecture, UI, source-layout, component README,
  and executable boundary authority now agree on the normalized component DAG.
  [Record](../history/M9-T51-S3-component-boundary-authority.md)

- **M9 T51 S4:** Final manifest, boundary/governance, zero-old-name, and fresh
  fixed x64/x86 36/36 proofs passed; T51 is closed.
  [Record](../history/M9-T51-S4-lib-component-normalization-closure.md)

## Recent Governance

- **M9 Td S6:** Console-object design was promoted to current architecture/UI
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
