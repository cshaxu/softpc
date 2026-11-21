# T100 S7 shared import closure

Owner acceptance: 验收通过，收口S7，保持T100开放，确保工作区干净。
S7 is closed; T100 remains open. S8 is the next unused S, not admitted.

## Completion audit

Accepted delivery cd218746a12dd6e5a6e66d1551ddc4177015b151 imports the eight
shared source/test trees exactly from NXVM
f45c34b85f01587aad1d830def04551a781bdc13. All 296 file bytes and eight committed
Git tree identities match the pinned source; all 288 manifest entries match.
The finite import ledger is complete: 286 unchanged files and ten imported files.
Only X86 IBMPC changes, adding optional App hotkeys through the existing KVM
matcher and Product dispatch/help path; SoftPC retains its five default bindings.
No local fork, App adaptation, external runtime/build dependency or mirror change.

Against 549714c1, production C/H +89/-37 (net +52), tests +71/-14 (net +57),
manifest +8/-8. Both cached architectures built and published without warnings
or errors, then each passed 188 background and eight native tests, 392/392 on
the first attempt. This closure does not rebuild or claim new runtime testing.
[Detailed evidence and artifact hashes](../proposals/m9-xp-mirror-original-first-audit.md#s7-p1-delivery-evidence).

Post-push coordinator review checked the actual committed scope, shared tree
identity, publication hashes and original request against the packet. The
preceding source review covered unique registration/dispatch, conflict rejection,
borrowed lifetimes, default initialization, teardown and inward dependencies.
No S7 requirement or blocking finding remains. Existing package Console TODO
and runtime-performance Queue candidate remain separate, unchanged and open.

Owner acceptance completes S7 only. Closure changes documentation alone;
source, tests, EXEs, INI, media and snapshots remain the accepted delivery.
Documentation governance, whitespace and build-residue checks pass. Temporary
S7 work was already removed; only reusable build caches remain. The worktree
was clean before this closure and must be clean after its commit/push.

## Retained admission packet

## M9 T100 S7 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner explicitly approves import, dual EXEs, publication, full tests, commit/push, quality review and clean worktree; wait for acceptance. |
| Objective | Import the eight shared trees byte-for-byte from pinned NXVM, preserving SoftPC behavior; qualify local products independently. |
| Non-goals | No NXVM writes, App/mirror/guest/configuration/snapshot changes, new hotkey bindings, alternative implementation or S/T closure. |
| Reference Baseline | SoftPC 549714c167c41c83c1836e20af0d0988c638668c; NXVM f45c34b85f01587aad1d830def04551a781bdc13, whose eight trees equal reviewed 8018a03dd. |
| Candidate Proposal | [S7 import brief](../proposals/m9-xp-mirror-original-first-audit.md#s7-verbatim-shared-import). |
| Files And ABI Surface | src/test pairs lib, emulator, x86-debug, x86-ibmpc; only six X86 IBMPC production files, two tests and two manifests differ. Optional borrowed hotkey extensions replace old separate help/registration helpers; existing SoftPC initializers remain valid. |
| Applicable Rules | Execution, architecture, coding, documentation, source/research policy and linked governance skills; single-session executor then coordinator actual-diff review. |
| Verification | Exact eight-tree bytes/manifests; real command matcher and entry tests; cached x86/x64 full builds, all background gates/unit/integration tests and serial desktop/native-audio tests; timestamp/publication hashes; final diff and ownership audit. |
| Expected Markers | All eight trees equal pinned source; unchanged default five hotkeys; no new non-mirror warnings; all local registered tests pass; fresh matching dual publications; clean pushed worktree. |
| Asset Needs | Existing immutable guest/media and owner INI only; refresh assets/binary EXEs. Own disposable build/t100-s7; two-hour job, fifteen-minute no-progress, 1 GiB scratch budget; this session owns jobs/logs and cleanup. |
| Reporting Requirements | Source revision, exact path/line accounting, interface compatibility, actual local tests versus upstream reports, hashes/timestamps, self-review and limitations. |
| Stop Conditions | Unexpected shared drift, overlapping SoftPC build, unidentified user edits, incompatible callers, failed qualification or publication; investigate without modifying sibling checkout or claiming success. |
| Exit Criteria | Implemented/verified delivery committed and pushed; jobs joined, temporary outputs cleaned, worktree clean; await owner acceptance without closing S7/T100. |
| Original Owner Request | 批准，准入S7原样导入nxvm最新公共八组件，老规矩，编译双EXE、发布、跑全量测试、提交推送、自审代码质量，确保工作区干净后等我验收。 |
| Similar-Issue Sweep | Enumerate all eight trees and all extension/helper callers; verify sole KVM registry/matcher, Product dispatch/help ownership, no App parsing duplication or old helper references, safe borrowed lifetimes and duplicate rejection. |
