# M9 T92 S1: XP Core Migration Audit and Handoff

## Original request

“好，验收通过，收口本T，准入队列下一任务：导入winxp的softpc核心。”
Subsequent implementation objective: “按照治理规范和执行规范，单人双角色模式，
实现现有softpc核心迁移到winxp源码的基线上。”

CURRENT owns the active packet. The
[proposal](../proposals/m9-xp-sp1-softpc-mirror-rebase.md) owns the detailed
scope, finite source ledger, owner-approved two-task split and acceptance plan.
Sound capability transfers to its Queue-linked independent proposal; it is
neither implemented nor claimed by T92.

## Delivery ledger

- S1: immutable source audit delivered in 2616deb8, actual-change review in
  54ea218a; owner-approved planning split recorded in f55428f8. No runtime
  changes or new EXE qualification in these documentation deliveries.
- S2: bounded original-device repairs are activated by the implementation
  objective. DMA/disk/V7 red cases precede the +7/-7 production repair; a
  generated ROM protects full CS without a production change. Dual-width
  full tests pass 146/146 each, including five desktop cases per width.
  Detailed qualification, initial failures, counts and hashes are in the
  proposal. Delivery review and owner acceptance remain distinct from closure.
- S3-S6: planned migration and final qualification, not implemented.

## Closure standard

Account for every retained source path and local fix against the frozen XP
baseline, preserve accepted machine/media/input/video/snapshot behavior,
complete focused and full dual-width qualification and publish both EXEs.
Each implementation delivery has a pushed P and a separate actual-change
review; owner acceptance and the final whole-task audit precede closure.
