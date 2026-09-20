# Project Status

## Current Work

T74 S1 and T74 are closed and pushed in bffbf413 after owner testing passed.
T75 S1--S3 are closed after delivery/review; S3 executor is e078dcc6.
T75 S4 is active: independent exact-copy four/six-package acceptance.
After S4 stop for owner testing; T75 remains open.

## M9 T75 S4 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner supplied report, corrected layout, required six C11 packages and admitted all four S deliveries without intermediate manual gates. |
| Objective | Prove exact-copy four-package neutral and six-package x86 builds/tests independently of product sources. |
| Non-goals | No new runtime/source changes, flexible layout framework, NNES import, desktop interaction, or user-data edits. |
| Reference Baseline | e078dcc6; S3 verified strict C11 dual-width packages. |
| Candidate Proposal | [Lib/Common code quality audit](../proposals/m9-lib-common-quality-audit.md). |
| Files And ABI Surface | Task records only; exact copies of six shared directories in owned ignored build/t75-s4-* directories. ABI unchanged. |
| Applicable Rules | Architecture/Coding/Execution/Document and referenced skills; owner explicitly overrides C17 for the six shared packages only. |
| Verification | Both widths, four/six-directory source hashes and independent CMake builds, standalone background tests, actual flags/source paths, manifests/DAG; final product Release/background suites. |
| Expected Markers | Neutral copy lacks x86; all copied targets use C11; source inputs resolve inside each copy; all required builds/tests pass. |
| Asset Needs | Refresh both EXEs only. Disposable isolated corpora/build logs under build/t75-s4-*; no guest media; clean after recording evidence. |
| Reporting Requirements | Estimate production/test/build +0/-0, net 0; record isolated matrix counts, final full regression, hashes and dual EXE links. |
| Stop Conditions | Stop on actual portability gap needing out-of-scope semantics; no external project writes or hidden desktop tests. |
| Exit Criteria | Independent four/six-package and product evidence, pushed P/review/closure, clean workspace; stop and await owner acceptance of T75. |
| Original Owner Request | Close T74 after successful tests; admit a new T to audit Lib/Common quality; owner will paste an existing audit report after closure. |
| Similar-Issue Sweep | Six manifests and compiled source roots: no App/Core/external repository paths, no inherited C17/extension flags; all three standalone source entry points also build. |

## Current Technical Baseline

- T74 S1 moves 556 files into src/core/{machine,compat,softpc.new}.
  498 mirror blobs are identical; 107 product/test C/H/RC files pass path-only
  comparison. Runtime symbols and behavior remain unchanged. Both Release
  builds and background suites pass: x64 110/110 (165.45s), x86 110/110
  (157.47s); five desktop cases excluded per width. Shared corpora, INI and
  guest media are untouched. [T74 proposal](../history/M9-T74-core-layout-rename-proposal.md)
  holds the +421/-409 (net +12) source/test/build/tool ledger and EXE hashes.
- Accepted T73 implementation 879c30ac, actual-change review ad665a86.
  Owner manual S5 testing passed. Closure changes documents only.
- Shared source corpora are src/lib, src/common, src/x86; matching suites are
  test/lib, test/common, test/x86. The six-directory set serves x86 products;
  the four Lib/Common directories build/test with x86 absent.
- Common contains machine/session/ui only. Machine retains one executor and
  copied opaque 128/1536-byte debug transport with its existing paused lease.
  x86/debug owns CPU protocol and DOS/X CLI; x86/xasm32 owns assembly/disassembly.
  VM validates x86 requests; App explicitly connects optional x86 capabilities.
- Lib and test/lib are unchanged by T73. Host-PC key identity and existing KVM
  capacities are retained; receiving adapters own guest input mapping.
- Final background x64 110/110 (181.72s), x86 110/110 (165.87s); both Release
  builds passed. Five desktop cases per width were excluded. Native Common/x86
  tests each passed 50 repetitions per width. Owner testing does not imply
  those excluded automated cases or Linux/NEC integration were exercised.
- Isolated neutral copy: Lib 41/41, Common 18/18. Isolated six-directory copy:
  Lib 41/41, Common 18/18, x86 9/9. No product resources/build are required.
- T73 endpoint C/H production +487/-428 (net +59), tests +1104/-819 (net +285);
  complete ledger and artifact hashes are in the completion audit. S5 runtime
  code is unchanged; its EXEs remain byte-identical to accepted S4.
- VM owns character mapping and device-attribute decoding. KVM uses four-byte
  text cells; Window owns fonts and Console owns character maps. Base transports
  opaque control FIFO and latest-wins frames. Window compares decoded pixels
  against its own surface; native relative/absolute mouse records remain.
- Common Session/UI, Compat, MVDM, snapshot format, user INI and media were not
  changed by T73. The existing debug-close/paused-destroy wake race is recorded
  by T75 S2; T73 closure does not certify all runtime interleavings.
- TODO remains empty under the owner's tracking policy, not proof of universal
  correctness. Queue candidates are unadmitted.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T74 | S1 complete; owner accepted; dual-width 110/110 background; pure Core relocation. | [Audit](../history/M9-T74-completion-audit.md) |
| T73 | S1--S5 complete; owner accepted; final dual-width 110/110 background. | [Audit](../history/M9-T73-completion-audit.md) |
| T72 | S1--S8 complete; owner approved; final dual-width 105/105 background. | [Audit](../history/M9-T72-completion-audit.md) |
| T71 | S1--S10 complete; owner approved; final dual-width 105/105 background. | [Audit](../history/M9-T71-completion-audit.md) |
| T70 | S1--S12 complete; owner approved; disclosed Linux limit. | [Audit](../history/M9-T70-completion-audit.md) |
| T69 | S1--S4 complete; reopened cleanup accepted. | [Audit](../history/M9-T69-completion-audit.md) |

## Recent Governance

- T74 closure archives the proposal and records S1/T-level requirement coverage,
  actual-change review and owner acceptance. Sources and tested EXEs unchanged.
- T73 closure records S5 acceptance, all-S requirement/changed-path audit and
  the separate queued wake-race receiver, and archives its proposal. No new
  implementation task or T number is allocated.
- M9 Td S18: T71 closure, TODO retirement and floppy-identification candidate;
  [record](../history/M9-Td-S18-t71-closure-and-floppy-queue.md).
