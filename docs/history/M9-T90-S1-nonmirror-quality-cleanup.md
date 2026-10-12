# M9 T90 S1: Non-mirror quality cleanup delivery

Status: S1 and T90 owner-accepted and closed on 2026-10-11.
Baseline: `1684475c` (accepted T89). The
[retained proposal](M9-T90-nonmirror-quality-cleanup-proposal.md) defines the finite five-item scope.

## Original request

“准入一个独立的T任务，完成以上5项代码质量治理工作；完成后编译发布测试提交推送后等我验收。”
“原生桌面测试也一起安排起来！”
The owner also reserved this round's desktop period without keyboard/mouse use.

## Requirement and actual-change audit

1. Snapshot READY reuse: delete the ten-line alternate write branch from
   `machine/driver.c`. The normal capture observer already finishes the capture
   before reporting pause; READY is retained only on clock-restart failure.
   Such a state must not reuse an archive as a successful save. The existing
   non-IDLE rejection and one checkpoint/capture/write path remain. The real
   command-provider test saves, edits paused EAX and RAM, saves again, loads
   and checks both values. It does not infer correctness from success text.
2. Audio diagnostics: preserve actual enqueue/writable/flush failures and route
   them through one stop-request/diagnostic/worker-exit path. Record task
   cancellation before interrupting its Audio wait, so normal INVALID_STATE
   during shutdown is silent; genuine I/O errors remain visible. No retry,
   new failure flag, worker restart or Machine ERROR transition. Existing
   audio failure injection now covers finite/continuous output, both gate-off
   flush routes, wait/reset faults and cancellation versus real failure.
3. Media mode parsing: expose the existing App-private parser in config.h;
   delete the duplicate extension parser. INI still accepts lowercase values;
   CLI retains its own case normalization. Tests cover all three modes,
   uppercase CLI acceptance, uppercase INI rejection and invalid values.
4. Save destination: keep path plus optional writer on the existing synchronous
   caller's stack. The first nonempty callback opens/truncates the destination.
   Failure before that point preserves old contents and creates no absent file.
   The owner-local extension test checks zero-byte callback, pre-write failure,
   successful multi-write contents, actual open failure and partial-write
   failure. Partial output is intentionally not rolled back.
5. Documentation: correct actual App filenames, X86 versus Emulator Product
   assembly/debug ownership, and Lib Audio's producer FIFO/one stream worker.
   Compat still owns speaker synthesis and timing. No runtime architecture
   or public API is introduced by these documentation corrections.

## Similar-issue sweep

Search scope: App driver snapshot begin/observe/run/finish, Compat Audio worker
and stream calls, App config/extension mode parsing and snapshot writer lifecycle.
`rg` for `SOFTPC_SNAPSHOT_READY|softpc_snapshot_image_write` confirms one
production archive-write call in the driver observer. The remaining READY
checks in snapshot.c and driver run preserve safe clock/failure semantics.
`audio_stream_(flush|wait_writable|enqueue)` finds one enqueue helper, two
writable waits and three flush calls; each now propagates or checks its result.
Audio creation/shutdown retain their existing ownership and error return paths.
Mode literals now map to storage enums only in the existing config parser;
extension literals are usage text, not a second mapping. Save has one lazy open
and one close, while Load's separate reader is unchanged. No new outside-scope
defect or TODO was added.

## Verification

Both Ninja Release builds and publication succeeded. Existing intermediates
were reused where valid; no cache deletion or alternate build framework.
The first x64 attempt caught a new test's C99 loop declaration in a C90 legacy
fixture. The declaration was moved to the existing function scope and rebuilt;
no compiler standard or assertion was weakened. x86 Ninja recovered its prior
incomplete bookkeeping and rebuilt its targets successfully; legacy mirror
warnings remain outside this repair, with no changed-source diagnostic.

Per width:

- Shared background: 84/84 (Lib 48, Emulator 21, X86 15).
- Other background: 48/48 (App 37, repository checks 11); includes eight App
  integration cases, snapshot transaction/cross-process and content validation.
- Native desktop: 5/5, serial in the owner-reserved period: both package
  variants, KVM Window retirement, KVM Window modal, Console broker display.
- Total: 137/137. No desktop retry or exclusion. x64's final two expanded
  Product fixtures were incrementally rebuilt and rerun after the main suite.

Hidden bounded supervisors retained logs and exit codes under the packet's
ignored T90/S1 build evidence directory. Both background and desktop supervisor
exit codes are zero. No Computer Use, user media mutation or INI edit occurred.
Documentation governance and whitespace checks pass.

## Diff accounting

Method: `git diff --numstat 1684475c` over the named production/test/build roots;
new tracked files are included. Documentation and binary artifacts are separate.

| Owner | Files | Added | Removed | Net |
| --- | ---: | ---: | ---: | ---: |
| App Product production | 3 | 23 | 25 | -2 |
| Machine driver production | 1 | 0 | 10 | -10 |
| Compat Audio production | 1 | 31 | 22 | +9 |
| Production total | 5 | 54 | 57 | -3 |
| App tests | 4 | 209 | 5 | +204 |
| Root test build wiring | 1 | 13 | 6 | +7 |

Public six roots and original `softpc.new` mirror: zero changes.
No duplicate implementation, extra component or recovery state machine remains
from this work. The new test links only Base/Storage/Types, uses its own machine
fixture, and does not embed firmware or depend on another test package.

Published SHA-256:

- softpc32.exe: `035156ef699b90f308c808c4aa46ca4a57eb2dc836d0a0a46c5e66c43a09fa38`
- softpc64.exe: `c44181ff263fa9e6520d260bd98ce4323f310a3d026f14e06afb753c2a1832dc`

Executable sizes change by +1,082 bytes (x86), +57 bytes (x64). INI, disks,
snapshots, public manifests and queue order are unchanged by the implementation.

## T90 completion audit and owner acceptance

Owner: “验收通过。TODO：RDP 下鼠标偶发逃出捕获窗口。已经修复，可以删掉。
据此，TODO和QUEUE都各自只有一条任务。请你照此提交推送”。

Coordinator review checked the actual `1684475c..dae5d2bb` production changes,
the five-item requirement ledger, test evidence and both published hashes.
All five items map to the existing owners and focused proofs above; the only
admitted S is S1, and all its exit criteria are met. Both background and native
desktop qualification succeeded on x64/x86 (137/137 each), followed by owner
acceptance. There is no unfinished T90 item or new debt to transfer.

The RDP TODO is removed on the owner's confirmation of its prior repair, not
claimed as a T90 code change or a new reproduction. The remaining null BOP TODO
and XP SP1 Queue candidate are outside T90 and remain unadmitted. Each ledger
now has exactly one pending item. S1 and T90 close without admitting a successor.
This P2 changes documentation only; the accepted binaries and implementation
remain `dae5d2bb`, so no redundant rebuild or runtime test is required.
