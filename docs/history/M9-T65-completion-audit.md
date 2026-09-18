# M9 T65 completion audit

Owner admission and closure instruction: “准入。请你修复。完成以后收口编译测试提交推送并直接收口这个T任务。”

## Finite closure ledger

Universe: the admitted Common synchronous-request publication review, its only
necessary state read/write ordering repair, shared-corpus verification and
dual-width package delivery.

| Requirement | Disposition | Evidence |
| --- | --- | --- |
| State read running-executor completion ordering | Complete | `common_machine_read_state()` now resets `state_event` before it publishes `state_read_requested`. |
| State write ordering | Complete, same operation contract | `common_machine_write_state()` uses the same reset-before-publish order. |
| Media/debug sweep | Retained unchanged | Both already use payload -> reset completion event -> request visibility. |
| Public API and product semantics | Preserved | No interface/header, Lib, VM, Compat, MVDM, Session/UI or thread change. |
| Request-slot consolidation | Cancelled by contract audit | The public contract requires one serialized control-thread caller; media, debug and state have materially different executor lifetimes. A union/slot would add a second state machine. |
| Focused Common proof | Complete | `common.common_machine` passed on both widths; the existing fake exercises an immediate state-read completion on the executor. |
| Full regression and package builds | Complete | x64 107/107 (105.42 s); x86 107/107 (110.11 s). Both package EXEs refreshed; owner INI/media unchanged. |
| Corpus gates | Complete | Common manifest and corpus tests passed in both full runs. |

## Actual change accounting

Tracked production source: `src/common/machine/machine.c`, four deletions and
two additions (net -2): each API moves its existing completion reset before
the request flag publication and removes the redundant exchange-result branch
that only guarded contract-forbidden concurrent control callers. No test source was added because the existing
Common Machine fake already executes the immediate-completion driver path; the
order itself is additionally reviewable in the two public submission functions.

The Common manifest changed only to record the revised shared corpus. Proposal,
state and closure records are governance metadata; package EXEs are build
artifacts. No obsolete production path exists to remove: the rejected unified
request slot was never introduced.
