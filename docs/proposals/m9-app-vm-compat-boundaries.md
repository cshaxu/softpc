# T57: App / VM / Compat boundary refactor

## Owner request and scope

Original requests: “要求：app部分config抽取合理；将softpc转换成common/vm的适配层应该整理到组件 src/vm 中；为softpc本体提供host支持的部分，应该是src/compat; 还有 app只能和common和lib产生联系，不应该认识mvdm compat或者vm。”
Clarification: “本次修改不得改动lib或common 我说的vm是src/vm”；
“组装入口还是可以接触vm的 但是仅此一处”；“准入一个新的t任务实施重构”。

Baseline cfc88cf, T56 closed, clean main, x64/x86 83/83.
No changes to src/lib, src/common, src/mvdm or their shared test corpora.
No guest timing, media, input, rendering, debug or lifecycle policy changes.
No new registration mechanism, executor or forwarding layer. Existing INI and
media remain untouched. The queued XP mirror task is not admitted.

## Finite ownership ledger

Coverage is every tracked app/host file at baseline plus affected CMake,
product tests and source-boundary gates. Every file is retained with a distinct
role or moved once; no old production path survives.

| Current responsibility | Destination |
| --- | --- |
| main configuration/path parser | app/config.c,h; main only calls it |
| CLI/debug selection, title/hotkey policy | app; only common/lib contracts |
| Common driver, machine input/frame/debug conversion | src/vm |
| concrete machine creation and original host callbacks | src/compat, with VM public creation boundary for main |
| host/compat CPU/GDP/header replacements | src/compat, no nested compat; isolated same-name headers retained |
| machine execution trace | VM-owned, no VM dependency on app |
| original source and shared corpora | unchanged |

Only app/main.c may include VM public interfaces. It may not include Compat
or MVDM. Other app sources cannot include them directly or through app headers.
VM consumes existing common/machine driver contracts and Compat; Compat
provides original host services and does not depend on app. Debug adaptation
using Common requests belongs to VM, not Compat. Original ABI symbols remain
unchanged unless purely local app-driver names move to VM ownership.

## Sequential steps and exits

1. S1: extract config with preserved defaults/parser/path behavior; focused
   configuration regression, full x86/x64 suites and fixed EXEs, commit/push.
2. S2: move VM and Compat responsibilities from current implementation;
   remove app machine-header dependencies, wire only main to VM, enforce
   include boundaries including negative probes; dual-width full regression,
   protected-tree zero diff, docs and artifact delivery; commit/push.
3. S3: separately audit entire ledger, old path removal, source accounting,
   preserved behavior evidence and debt; archive and close only after exits.

Single agent, executor then actual-commit reviewer. Each implementation P is
complete and pushed, each S has both fixed EXEs. No synchronous user test wait
is required between steps. Unexpected need to edit protected corpora stops
that part for owner decision. Historic known TODOs remain distinct.

This task does not split platform.c algorithms, replace timer APIs or redesign
borrowed frame contracts. Such changes are unnecessary for the approved
ownership relocation and must not be hidden in a mechanical move.

## S2 executor evidence

Existing Common driver, debug adapter, input conversion and trace now belong
to VM. Main creates/describes/destroys that concrete backend through one
public header; it still composes the same Common entities. VM creation owns
the former main machine/audio acquisition and failure cleanup. The original
borrowed-machine driver binding remains private and is used by production
creation and product test fixtures. No second driver or allocation wrapper
was introduced. Display remains app-to-Common policy; the legacy machine's
unused presentation option is not exposed in the new VM options.

Host support moved to flat Compat; same-name replacement headers retain
their required small directories. Aside from its lifecycle include path,
Compat machine.c and the other relocated host implementations keep their
original algorithms. Keyboard product hotkeys remain app-owned; physical
SoftPC conversion moved to VM. All product build/test consumers were updated.

The boundary gate validates app, VM and Compat edges, including six negative
fixtures. Initial x64 regression found one negative-fixture failure: relative
includes whose targets did not exist were not normalized. Fixing normalization
and fixture cleanup made the focused probes and final full suites pass.
No product assertion was weakened. Final x64: 85/85, 53.98 seconds; x86:
85/85, 92.27 seconds. Both fixed EXEs were rebuilt with the checked-in presets.
Final focused boundary rerun: 2/2; documentation gate and diff check pass.

Rename-aware staged numstat against S1: src has 55 changed path entries,
+259/-215 (net +44); test has 22 entries, +129/-56 (net +73).
This excludes CMake, docs and EXEs. The added production boundary replaces
main's direct backend wiring; the input mapper is moved, not duplicated.
The five protected source/test corpora have zero diff from cfc88cf.
Historical T14/T16 evidence generators retain historical src/host paths;
they are not current runtime/build/acceptance dependencies. Current product
sources and source lists contain no surviving src/host implementation.

## S3 whole-request audit

Reviewed the actual pushed S1/S2 commits, not only their test summaries.
Frozen source universe: 51 baseline app/host paths from
`git ls-tree -r --name-only cfc88cf src/app src/host`. Every path resolves to
one retained or relocated implementation; no missing destination.

| Ledger member | Final disposition / proof |
| --- | --- |
| Main/config | Existing parser moved to config C/H; config smoke and package suites pass. |
| App command/binding/keyboard | Product policy retained; physical injection moved to VM. Only main includes vm/vm_interface.h. |
| App driver/trace, host debug | VM driver/trace/debug; mechanical names/includes and one public creation boundary. |
| Remaining host paths | Compat with nested compat removed; original callbacks and replacement-header ownership retained. |
| Firmware resource | app/firmware.rc unchanged, embedded source ROM inputs unchanged. |
| Build and product tests | All current source lists/quoted headers relocated; boundary gate plus six rejecting fixtures. |
| Lib/Common/MVDM/shared tests | git diff cfc88cf HEAD --quiet on all five roots exits zero. |
| User configuration/media | No changed paths; no new runtime/build external dependency. |

Whole-task rename-aware `git diff cfc88cf HEAD --numstat -- src`: 57 changed
entries, +455/-398, net +57 production lines. Test accounting: 23 entries,
+188/-56, net +132. CMake/docs/binaries excluded. Growth is the explicit config
and VM boundary plus regression/boundary proofs, not extra runtime states,
queues, threads, or a forwarding framework. Old app driver/trace and src/host
are absent. Historical source evidence scripts are intentionally historical.

Final executable SHA256:

- softpc32.exe: 21517FC0A451F70438B2950D8563DC36778F5723ED542608F8A6F3428170EC8B
- softpc64.exe: 6528DCE4413619297ECBF2EF881ADCD9D35CEBD861F92A99063305EB883F02E5

S3 changes no code, so the S2 dual-width 85/85 proof and binaries apply
unchanged. This is automated package evidence, not a claim of owner GUI/RDP
acceptance. Existing six TODO entries retain their original owners/admission
conditions; none is claimed solved by relocation. The XP proposal is not
admitted. Implementation and audit are complete; T closure awaits the owner.
