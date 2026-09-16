# M9 T61 S11: VM backend ownership

Owner request: “每个S任务执行前先审计，预估需要移动的文件/功能/行数，预估可以达成的diff和自主实现的增减情况；结束后也进行同类实际汇报。”
The serial S8–S13 implementation admission remains in the
[proposal](../proposals/m9-mirror-support-ownership-audit.md).

## Delivery and review

Implementation `64a7668` is committed and pushed. Coordinator reviewed the
actual rename, all media callers, build selection and test-boundary changes.
VM owns concrete configuration/lifecycle and debugger preflight. Compat retains
original host memory/bus/media contracts and does not import VM. No second
backend, initialization rewrite, original mirror or shared-corpus change.

The two machine files move 606 lines; debugger preflight moves separately.
Production 13 paths +67/-78, net -11; tests 22 paths +34/-32; build +8/-2.
The estimate was production -10 to -35, excluding moves. Mirror diff remains
498 retained / 404 identical / 94 changed, +23137/-22334, 5131 hunks.
[Evidence](../etc/evidence/softpc/pristine-divergence-current.md#s11-实施前审计)
records the initial missing explicit test include and replacement full run.

Full suites: x64 98/98 (58.53s), x86 98/98 (75.29s). Actual-commit coordinator
retests: product-boundary negative, dual-media, runtime-restart-boot,
command-provider, standalone-source-boundary and build-ownership negative:
6/6 each, x64 7.73s and x86 7.71s. An initial underscore test regex selected
zero tests; it was corrected to actual hyphenated names, not counted as proof.
Documentation/diff checks pass. Both fixed EXEs refreshed; INI/media unchanged.
Owned temporary logs removed, existing build trees retained.

S11 closes; S12 is admitted serially. T61 remains open and no manual product
acceptance is claimed.
