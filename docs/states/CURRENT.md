# Project Status

## Current Work

T74 S1 and T74 are closed and pushed in bffbf413 after owner testing passed.
T75 S1/S2 are closed after delivery/review; S2 executor is 9b314372.
T75 S3 is active: six shared source/test packages use strict C11.
S4 is serially authorized; T75 remains open for final owner acceptance.

## M9 T75 S3 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner supplied report, corrected layout, required six C11 packages and admitted all four S deliveries without intermediate manual gates. |
| Objective | Select C11 for all six shared package scopes and fix strict compiler diagnostics without semantic changes. |
| Non-goals | No App/Core language migration, ABI/CLI changes, warning suppression, new platform implementation or user data edits. |
| Reference Baseline | 9b314372; S2 verified dual-width delivery. |
| Candidate Proposal | [Lib/Common code quality audit](../proposals/m9-lib-common-quality-audit.md). |
| Files And ABI Surface | Six CMake entries, Session control.c, xasm32 warnings, affected tests/readmes/manifests and source-layout design. No public ABI changes. |
| Applicable Rules | Architecture/Coding/Execution/Document and referenced skills; owner explicitly overrides C17 for the six shared packages only. |
| Verification | Actual compile flags C11/no extensions and strict GNU warnings; dual-width builds and background tests; manifests/DAG and documentation gates. |
| Expected Markers | Every compiled shared source/test target uses C11 and passes strict warnings; Core/App compile settings remain unchanged. |
| Asset Needs | Refresh both EXEs only; preserve INI, media and snapshots. |
| Reporting Requirements | Estimate 12--16 code/build/test files, production C/H +20/-10, build/test +55/-65, aggregate net near zero; report actual deviations and both EXEs. |
| Stop Conditions | Stop if diagnostic repair needs behavior or public ABI changes; do not suppress warnings or alter imported instruction semantics. |
| Exit Criteria | Six-package strict C11 compilation, focused/full background regression, pushed complete P and review/closure; then S4. |
| Original Owner Request | Close T74 after successful tests; admit a new T to audit Lib/Common quality; owner will paste an existing audit report after closure. |
| Similar-Issue Sweep | All six CMake entries and actual compile flags; strict compiler output across all configured production/test sources; explicit disposition for platform-only tests and negative fixtures. |

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
