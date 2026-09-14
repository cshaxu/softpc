# M9 Td S14: Win3.1 display-roundtrip queue promotion

## Owner request

> 接下来 td治理：把todo里 修复windows msdos提示符显示异常的任务做成proposal加入队列 放在队尾

## Delivery and review

Baseline: `e0c9ae3`, T58 closed. P1 `f41d8a5` was committed and pushed.
The reviewer inspected its actual proposal and Queue/TODO diff.

- The [proposal](../proposals/m9-win31-display-roundtrip.md) preserves the
  Win3.1 MS-DOS prompt fullscreen -> CAF -> Window -> Alt+Enter -> fullscreen
  -> CLS stale-content reproduction, with ordinary DOS CLS as a passing control.
- Root cause remains unknown; future investigation must locate the first
  incorrect frame/state boundary rather than assume a CLS or MVDM defect.
- XP SP1 remains first; this candidate is second, at queue tail.
- Only the promoted TODO was removed; the other eight debts are unchanged.
- No implementation T was allocated or admitted. No source, test, build,
  executable, configuration or media changes were made.

## Verification and closure

Documentation governance, both documentation-governance CTests and diff hygiene
passed for delivery and are rerun for closure. No runtime verification is
claimed for this documentation-only task. P2 records the review and returns
Current to idle; the repair remains unresolved and queued.
