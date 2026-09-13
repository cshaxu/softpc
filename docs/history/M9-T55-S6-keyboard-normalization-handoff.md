# M9 T55 S6 — Keyboard Normalization Handoff

S6 delivered the bounded input/geometry change in `5282f42`, followed by
actual-diff review `5be6fc4`. x64/x86 passed 50/50; strict lib passed 3/3.
Owner reported “测试通过”, then requested another audit. That audit reproduced
scan-less character deduplication loss and text synthesis releasing a held
modifier, and found ANSI raw reads plus ignored native repeat counts.
These findings are explicitly admitted to S7; neither S6's tests nor manual
acceptance certified their absence. T55 remains open.
Original requests and evidence remain in the
[T55 proposal](../history/M9-T55-lib-types-external-boundary-proposal.md).
