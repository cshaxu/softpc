# M9 T85 S10 IBM PC simplification closure

## Owner requests and decision

Original admission: 批准新的s任务进行以上各项优化，一个解决common，一个解决x86，一个处理ibmpc！
Continuation: S9完成以后不要停下来了，你把S10完成以后再等我。
Acceptance: 验收通过 收口s10 保持t开放。

S10 closes after owner testing. T85 remains open, with no active S or newly
admitted work. This does not infer manual acceptance for earlier deliveries.

## Requirement and evidence review

Implementation f97ecbea and actual-change review a51bf19b are pushed. All 48
paths match the admitted IBM PC scope. The [proposal](../proposals/m9-windows-101-startup-repair.md)
retains the pre-change estimate, exact per-path code accounting, frozen
54-path test ledger and coordinator review.

- UI teardown and failed binding cleanup retain callback dependencies until
  destruction succeeds. The same-owner Machine rollback retains its driver;
  existing composition failure tests prove retained ownership and later cleanup.
- Executor atomics are embedded in private control, without a separate
  allocation or test-only public lifecycle. Existing memory orders remain;
  reset/start/stop assertions pass.
- Disk slot-zero aliases are removed in favor of existing arrays; protocol,
  resource ownership and opaque product ABI remain unchanged.
- All 54 dormant paths have dispositions: 26 retained paths enter compilation,
  including 25 newly registered executables; 28 unused or obsolete paths are
  removed. Retained test C/H dependency closure is 234/234 on both widths.

Production eleven paths are +71/-91, net -20. Test/build 31 paths are
+106/-7511, net -7405. Combined code is +177/-7602, net -7425, excluding
documents, manifests and artifacts. Removed test files remain recoverable
from Git; the reduction is primarily dormant material, not runtime footprint.

Dual full Release builds and focused 27/27 per width pass. Serial background
x64 passes 468/468 in 275.11s; x86 passes 468/468 in 254.56s. Source/test
manifests, corpus/DAG, Types/test ownership, documentation and diff gates pass.
Five desktop cases per width are excluded; no Linux/RDP or imported IBM PC
guest-runtime qualification is claimed. Earlier S8 shutdown timing debt is
still tracked and is not declared repaired.

Lib/Common/x86/SoftPC runtime and mirror are unchanged. Rebuilt/up-to-date
EXEs remain byte-identical to S9 because this IBM PC runtime is not linked into
SoftPC: x86 SHA-256
9B32C0C5D8F0AEAD062B326E930D0BD9A658E0C7CCFE5C521990AFADF163C98B;
x64 SHA-256 516F8545678A2EED03F711E0B41F5D8D9D1CC644AAD2FDCDCA74C946B3984E20.
Six bounded generated JSON outputs were removed; ignored build caches remain.
Original owner INI/snapshot edits are preserved and excluded as recorded.

## Closure scope

Only CURRENT, proposal and this historical record change. The active packet
is removed, Queue/TODO remain untouched, and accepted code/EXEs are not rebuilt
or altered. The first governance check rejected a noncanonical idle-state
wording; CURRENT now uses the existing exact idle/open-task markers, with no
gate change. The corrected governance and diff checks pass. No runtime tests
are rerun for this document-only closure; accepted build evidence is unchanged.
