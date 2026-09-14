# M9 Td S13: common/lib responsibility boundary

## Owner request

> 呃 common之外依然还是允许调用lib的 比如加载配置ini文件？

> 修订好proposal和s任务吧 更新好

## Delivery and review

Baseline: `2bc73d6`. P1 `0f70d54` was committed and pushed. The reviewer then
read the actual committed proposal diff and searched its dependency and
acceptance wording for blanket common-only restrictions.

The [live candidate](M9-T56-common-convergence-proposal.md) now allows
app/product adapters to use lib public APIs for their own responsibilities,
including app reading INI through storage. It prohibits bypassing common-owned
broker/KVM/executor lifecycles and prohibits duplicate implementations or
same-purpose forwarding layers.

- Ownership table and dependency graph explicitly permit these direct calls.
- S1 records retained direct calls with their actual resource owner.
- S5 removes duplicates and ownership violations, not all external lib calls.
- S8 and shared exit criteria accept reviewed legitimate direct calls.
- S2 still removes the old app-owned UI management path after its extraction;
  this narrow deletion requirement is not a ban on unrelated app lib usage.

This revision supersedes the common-only dependency planning recorded in Td
S12; historical records remain historical. Eight proposed implementation
stages, per-stage executable acceptance and user-visible behavior commitments
remain unchanged. No source, executable, configuration or media was changed.

## Verification and closure

Documentation governance and diff hygiene passed for P1 and are rerun for
closure. This was document-only work; no build/test results are claimed.
P2 records the review and returns Current to idle. The candidate stays first
in Queue; no numeric implementation T is admitted or allocated.
