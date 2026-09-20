# Project Status

## Current Work

T75 is closed and pushed in eacf1a5e after owner authorization and completion audit.
T76 S1 is active: Storage overlay lookup baseline and design qualification.
[T75 audit](../history/M9-T75-completion-audit.md).

## M9 T76 S1 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | New |
| Admission And Approval | Owner: close T75 and admit the next T for lib/storage overlay lookup performance. |
| Objective | Freeze existing behavior/snapshot baseline and quantify linked-list cost versus direct-index memory before implementation. |
| Non-goals | No production algorithm change in S1, public API change, Core/Common/x86 change, guest media edits, or desktop interaction. |
| Reference Baseline | eacf1a5e; accepted strict C11 shared corpus and dual-width 110/110 background suites. |
| Candidate Proposal | [Overlay page lookup optimization](../proposals/m9-overlay-page-index.md). |
| Files And ABI Surface | Read storage medium/file and callers; existing storage/snapshot tests and bounded measurement fixtures if needed. ABI unchanged. |
| Applicable Rules | Execution/Document/Architecture/Coding and referenced governance skills; strict C11 and existing ownership boundaries. |
| Verification | Existing public behavior/failure tests, repeatable small/dense/sparse workloads, snapshot media-byte baseline; dual-width Release and applicable background suites before S1 delivery. |
| Expected Markers | Measured lookup costs, size-safe pointer-array estimates for both widths, explicit decision on sparse capacity tradeoff, no changed production behavior. |
| Asset Needs | Only task-owned ignored build/t76-s1-* fixtures/logs; bounded workloads, stop any probe beyond 60 seconds; preserve checkpoints, remove owned temporary data when no longer needed. No assets media edits. |
| Reporting Requirements | Production estimate +0/-0; refine test/tool file and line estimates before edits; report actual counts and both EXEs at completed S delivery. |
| Stop Conditions | Direct indexing materially narrows usable capacity or sparse behavior: report evidence and review revised design before S2; no silent algorithm expansion. |
| Exit Criteria | Baseline evidence and finite ledger, reviewed design decision, relevant dual-width verification, complete pushed delivery and actual-change review. |
| Original Owner Request | 收口T75，准入下一个T任务：优化 lib/storage overlay查找表现 |
| Similar-Issue Sweep | All medium creation/read/write/destroy paths, file versus zero bases, direct/readonly exclusion, failure ownership and snapshot API consumers. |

## Current Technical Baseline

- T75 uses the existing task cancellation object for the outer Machine wait;
  deterministic lost-command-wake coverage and repeated native shutdown pass.
  All six shared packages select strict C11 in standalone and embedded builds.
  Public API, App/Core, INI and media remain unchanged. Endpoint source/test/build
  +126/-79 (net +47), of which production C/H is +34/-33 (net +1).
- T75 S4 verifies sixteen independent four/six-package builds on both widths;
  each width passes isolated Lib 41/41, Common 18/18, and optional x86 9/9.
  All 180/204 copied files remain identical. Final product background x64
  110/110 (127.19s), x86 110/110 (182.23s); both Release EXEs rebuilt and match
  S3 hashes. Desktop cases excluded; no Linux runtime or NNES integration claim.
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
  changed by T73. T75 S2 subsequently repairs the debug-close/paused-destroy
  wake race; its bounded schedule proof does not certify all interleavings.
- TODO remains empty under the owner's tracking policy, not proof of universal
  correctness. Queue candidates are unadmitted.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T75 | S1--S4 complete; owner approved closure; strict C11 six-package and dual-width acceptance. | [Audit](../history/M9-T75-completion-audit.md) |
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
