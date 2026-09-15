# Storage ownership and Console disposal cleanup

## S17 admission

Original owner request: 批准，请进行一个新的S任务 完成以上2个目标。编译测试提交推送。

M9 T59 S17 continues from 5847799, clean main. The approved two goals are:
Storage medium embeds its file; read_owned uses a stack file; remove the private
file allocation/ownership-transfer shell while preserving public APIs. KVM
Console uses one disposal function for startup failure and normal disposal.
S16 is retained in [history](../history/M9-T59-S16-base-task-flattening.md).
Admission does not claim owner acceptance of prior binaries; T59 remains open.

The selected platform open/lock implementation initializes caller-owned file
storage. Common close consumes its stream once but never frees its container.
Medium/writer own their allocations; read_owned owns its stack file and returned
bytes. All failed opens leave a null stream; Linux lock failure must clear its
closed pointer so unified cleanup cannot close it twice. No new abstraction,
mode, public API, lock policy, worker lifecycle or Common/App/VM/MVDM changes.
KVM disposal still follows successful worker join and sink drain; do not merge
the distinct worker stop and native-resource ownership contracts.

## Finite convergence ledger

| Member | Required result | Proof |
| --- | --- | --- |
| Storage ownership | Medium/writer embed file; read_owned uses stack; no heap file shell | Allocation-count tests, complete private caller scan |
| Storage behavior | Direct/readonly/overlay, zero-backed overlay and close-error semantics retained | Binary/storage tests and full dual-width regression |
| Failure cleanup | Allocation/open/size/read failures unwind; close consumes once; Linux lock failure clears stream | Focused failure tests and platform source review |
| KVM disposal | One Console disposal body, startup failure and normal join both use it | Existing failure/retirement tests and source scan |
| Delivery | Strict x86/x64 builds, full tests, manifests/docs, EXEs and pushed P1/P2 | Actual-change coordinator review; wait for owner |

Sweep scope: all storage file allocation/close callers, both platform opens,
and both KVM leaf disposal paths. Existing overlay indexing and unrelated TODOs
are out of scope. Reuse existing build trees and test-owned temporary binary
fixtures (removed by tests); never change user INI or guest media.
Count tracked production/test C/H separately using git diff 5847799 --numstat.
No public header changes; source changes should reduce duplicated ownership.

## Executor implementation and sweep

Medium embeds file and opens directly into it; failures unwind through its
existing destroy. read_owned opens a stack file. Writer already embedded file
and now shares the same close implementation. All three consume streams once;
close errors still consume ownership. Read/seek private signatures are const
correct so embedding does not require casting away medium constness. Platform
open and locking behavior are unchanged apart from clearing the already-closed
Linux stream on lock failure. Console normal and startup-failure disposal now
call the single existing body; worker stop/join remains distinct.

Sweep commands: rg for lib_storage_file_open_readonly/open_readwrite and
kvm_console_dispose over src/test returns no obsolete callers. Allocation and
close scan of storage finds only medium, pages, writer and returned bytes;
no heap file shell remains. Both platform failure paths leave zero-initialized
file storage empty. Window already has one disposal body and needs no change.
Public interfaces and Common/App/VM/Compat/MVDM have zero baseline diff.
Existing overlay lookup TODO is retained, not claimed solved.

storage_file_writer_binary adds allocation counts, allocation/open/limit/close
failures, all three file modes, overlay persistence isolation and zero-backed
reads. Existing exact-read failure cleanup was reviewed: it closes the stream
then releases bytes without exposing outputs. linux_storage_contract compiles
the actual Linux file source against controlled native-call fakes and proves
read/write lock success/failure closes exactly once. It is not native Linux
runtime evidence. Existing Console failure and retirement-barrier tests exercise
normal/fault disposal and callback drain before freeing the object.

Production C/H: six files, +42/-90, net -48. Test C/H: two files, +126/-0,
net +126. Method: git diff 5847799 --numstat, C/H only, including new test;
docs, manifests, CMake and binaries excluded. No new production state or layer.
Existing build trees retained; test-owned fixture removed by the test. INI and
guest media unchanged. No new scratch tree or asynchronous process retained.

## Executor verification

All five ledger rows are satisfied. Strict GNU builds via tests-x86/tests-x64
passed (-O3 -DNDEBUG -std=c17 -Wall -Wextra -Wpedantic -Werror for Storage).
Full CTest: x86 93/93 (66.24 s), x64 93/93 (75.57 s). Four corpus manifests,
documentation governance, component DAG and diff whitespace pass. No runtime
test failure observed; native Linux and owner manual acceptance are not claimed.

Fixed EXEs: softpc32.exe 3,484,346 bytes, SHA256
00F1DB1FF1431CB57320F20C482C974004D65E70D4B880DAF96B541F375F9C4A;
softpc64.exe 2,843,968 bytes, SHA256
A78FD70FD79EB6FB4BFC0778907E637CF6A2D26986B46FCB194699F3BFD288B4.
P1 is the complete executor delivery; actual-commit coordinator review follows.
