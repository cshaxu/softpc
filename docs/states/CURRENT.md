# Project Status

## Current Work

T74 S1 and T74 are closed and pushed in bffbf413 after owner testing passed.
T75 S1 is closed after pushed delivery 47de48b1 and actual-change review.
T75 S2 is active: repair outer executor cancellation using existing Base wait.
S3/S4 are serially authorized; T75 stays open for final owner acceptance.

## M9 T75 S2 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner supplied report, corrected layout, required six C11 packages and admitted all four S deliveries without intermediate manual gates. |
| Objective | Repair terminal wake ownership with a deterministic old-fails/new-passes test and existing cancellable outer wait. |
| Non-goals | No Lib API/state/thread additions, polling, timeouts as recovery, Core or user-media/config edits. |
| Reference Baseline | 47de48b1; S1 passing dual-width baseline. |
| Candidate Proposal | [Lib/Common code quality audit](../proposals/m9-lib-common-quality-audit.md). |
| Files And ABI Surface | src/common/machine/machine.c, test/common/machine_wait_smoke.c, test/x86/debug_machine_smoke.c and affected manifests; ABI unchanged. |
| Applicable Rules | Execution and Document now; Architecture/Coding and their referenced skills before source audit. |
| Verification | Deterministic lost-command schedule before/after; repeated native Machine/x86 tests; both Release builds and full background presets; manifests/DAG and documentation gate. |
| Expected Markers | Cancellation exits despite consumed command wake; faults remain errors; pending requests complete once; callback targets survive join. |
| Asset Needs | Refresh both EXEs only; preserve INI, media and snapshots. |
| Reporting Requirements | Estimate production +5/-3, test +45/-3 (net +44 total), three C files plus manifests/docs; record actual counts and dual EXEs at delivery. |
| Stop Conditions | No new cancellation framework or unapproved semantic/ABI expansion; no desktop interaction. |
| Exit Criteria | Focused old-fails/new-passes proof, full background regression, pushed complete P, actual-change review and closure; then S3. |
| Original Owner Request | Close T74 after successful tests; admit a new T to audit Lib/Common quality; owner will paste an existing audit report after closure. |
| Similar-Issue Sweep | All Common event/wait_any sites: worker waits versus synchronous caller completion versus Session queue wait; classify each owner and cancellation need. |

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
