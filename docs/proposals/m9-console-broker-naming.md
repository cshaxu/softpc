# Console broker naming

## S18 admission

Original owner request: 根据最新代码 准入一个新的s任务完成host组件重命名工作 完成后 编译测试提交推送

M9 T59 S18 continues from clean main 2c0217d. S17's completed executor and
coordinator evidence remains in [history](../history/M9-T59-S17-storage-ownership-cleanup.md); prior manual acceptance is not
assumed. T59 remains open.

Host now owns only the native Console broker: sync/time already belongs to
Base. Rename src/lib/host to src/lib/console-broker, its build target to
console-broker, and host_console vocabulary to console_broker (without a
duplicated broker suffix). Preserve the existing console file names and all
function bodies, layouts, return values, reader/output/rollback behavior.
Update consumers directly; no old aliases, forwarding headers or duplicate
targets. Console remains the neutral logical object; KVM Console still depends
on Console, Base, Types and KVM Base, never the broker. No component merge.

## Finite convergence ledger

| Member | Required result | Proof |
| --- | --- | --- |
| Component and symbols | Five Host files relocated; broker names consistent, no old production symbols | Rename-normalized baseline comparison and full source scan |
| Consumers and builds | Common UI and test callers use new API/target; platform implementations unchanged | Strict x86/x64 builds and full regression |
| Dependency contract | Same edges under new name; KVM Console cannot import broker | Existing exhaustive DAG positive/negative probes updated |
| Documentation and transfer | Current design, corpus docs and four manifests consistent; history immutable | Manifest/documentation gates and reference scan |
| Delivery | Both fixed EXEs, complete P1 push and actual-change P2 review | Recorded tests, hashes, changed-path counts and clean worktree |

Scope includes src/lib, src/common callers/gate, shared/product tests, build
references and current design documentation. No App/VM/Compat/MVDM semantics,
INI/media changes, new runtime state or failure-policy changes. Stop on a
required behavior change or overlapping main-thread changes. Preserve historical
uses of host and unrelated host-platform vocabulary. Renamed test filenames
and fixture directories must follow their CMake references.

Reuse existing x86/x64 trees; tests own their temporary fixtures. No new trace
or guest media. Report production/test C/H additions and removals relative to
2c0217d with rename detection, separately from documentation/build/binaries.

## Executor implementation and sweep

All five broker sources/headers are relocated with git mv. Public functions
are console_broker_create/replace/request_cooked_line/cancel_cooked_line/destroy;
the object and mode are console_broker and console_broker_mode. Internal backend,
helper and constant names use that same prefix. File names remain console.c,
console_interface.h and console_backend.h within the renamed component.
No forwarding alias or old build target remains. Common UI invokes exactly the
same operations in the same order. Three neutral-header comments name the broker
instead of the former Host component; no signature or layout changed there.

Compared all five implementation/header files and Common UI to 2c0217d after
applying only the declared identifier/path substitutions: all six match exactly.
Both platform bodies, enum order, singleton, locks, reader retirement, cancellation,
rollback, output bindings and focus operations are unchanged. Remaining host
words denote native/platform concepts or historical product boundaries, not a
second Lib component. App, VM, Compat, MVDM and package INI have no diff.

The source and CMake DAG maps, Common UI edge, shared test targets and forbidden
fixture paths now use console-broker. The existing 64-edge source/link self-test
still covers all allowed and forbidden pairs, including KVM Console to broker.
Added retired host_console/HOST_CONSOLE negative probes to the same test; the
existing dependency gate rejects old source directories/symbols/includes.
No new runtime checker, state or test executable was introduced.

Current design, component documentation and four transfer manifests are updated.
S17's original proposal is archived without a content change; its manual-test
status remains pending. No unrelated Queue/TODO/rule/history edits are included.

Production C/H: nine paths, +248/-248, net 0. Test C/H: eleven paths (including
two unchanged relocated fixtures), +234/-234, net 0. Reproduce using
git diff --find-renames=20% 2c0217d --numstat -- '*.c' '*.h', grouping src and test.
Documentation, CMake/gates, manifests and fixed EXEs are outside these counts.

## Executor verification

Final strict GNU builds passed for both widths: -O3 -DNDEBUG -std=c17
-Wall -Wextra -Wpedantic -Werror on the renamed broker. Final full CTest:
x86 93/93 (57.44 s); x64 93/93 (56.81 s). An earlier x64 run also passed
93/93 before three comment-only header edits; both widths were rebuilt and
the final suites ran after those edits. No test failure occurred.
Four manifests, Lib layout/DAG, Common corpus, documentation and whitespace
gates pass. Exhaustive negative probes verify the unchanged component edges.
No native Linux runtime or owner manual acceptance is claimed.

Fixed EXEs: softpc32.exe 3,484,363 bytes, SHA256
A7F66406BE190659EA7D27A666E0DC713F7FDC65ABF7BA7EAD065B7317064EB3;
softpc64.exe 2,843,985 bytes, SHA256
AF22B2D556C5FB0171B73ABCD163449439F80F330E471A5E425D829376D524A7.
Existing build trees are retained; generated types-layout fixtures are removed
after their completed tests. No new build/trace tree or live worker is retained.
P1 delivers the implementation and evidence; actual-commit review follows.
