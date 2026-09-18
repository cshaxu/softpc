# M9 T66 completion audit

Owner admission: “开始吧 做完架构整理任务。”  Owner final validation:
“测试通过。”

## Finite closure ledger

Universe: the admitted snapshot and component simplification candidates in
`m9-snapshot-architecture-simplification.md`. A candidate closes only by an
implemented, evidenced simplification, a recorded retention decision, or a
transfer to its existing independent task.

| Ledger item | Final disposition | Evidence |
| --- | --- | --- |
| Snapshot finish result ordering | Implemented | S1 publishes clock-restart failure before Common completion and exits through the existing retained `READY` phase. |
| Duplicate VGA restore dispatch | Implemented | S2 calls original `vga_gc_outw()` rather than maintaining a snapshot-only nine-way switch. |
| Duplicate PIT function recovery | Implemented | S2 uses one validated state-ID decoder rather than temporary wrapper objects. |
| Unused Compat import / harmless local residue | Implemented where safe | S2 removes the unused video import; required early declarations remain because moving them would enlarge the protected mirror diff. |
| VM checkpoint entry cache | Implemented | S3 deletes the unread `softpc_snapshot.entry` field and assignment. |
| Retained snapshot images | Retained by proof | `captured_image` supports a second PAUSED save without guest execution; `staged_image` spans accepted load to executor restore. |
| CPU/device-state externalization | Rejected | CPU/device ownership remains beside original state; getters or a second state model would violate correctness and boundary priorities. |
| Descriptor/generic archive serializer | Rejected | Direct field order remains the auditable binary-format definition. |
| Common request consolidation | Transferred/closed separately | T65 owns the synchronous request publication review. |
| Frame copy, Console worker and overlay index | Independent dispositions | Frame copy is T64 S2; worker consolidation was rejected in T64 S1; overlay lookup stays the queued dedicated proposal. |

## Actual accounting

| Step | Production delta | Test delta | Protected OpenNT mirror delta | Result |
| --- | ---: | ---: | ---: | --- |
| S1 | +9/-1 (net +8) VM | 0 | 0 | Necessary result-order correction; no new state channel. |
| S2 | +33/-68 (net -35): MVDM -34, Compat -1 | +4/-0 | -34 added mirror lines | Reused original VGA/PIT behavior and deleted residue. |
| S3 | 0/-2 (net -2) VM | +1/-1 (net 0) | 0 | Removed only the unread entry cache. |
| **T66 total** | **+42/-71 (net -29)** | **+5/-1 (net +4)** | **-34 added mirror lines** | Correctness and ownership preserved. |

Documentation and package artifacts are excluded from source accounting.

## Verification and delivery

- S1: focused snapshot suites 3/3 each; x64 full 107/107 in 108.35 s; x86
  full 107/107 in 101.90 s.
- S2: focused VGA/checkpoint 2/2; x64 full 107/107 in 117.84 s; x86 full
  107/107 in 114.12 s.
- S3: focused snapshot suites 4/4 each; final x64 full 107/107 in 91.29 s;
  x86 full 107/107 in 108.19 s.
- Both package widths were refreshed after each implementation step. Owner INI
  and guest media were not modified. Owner validated the final package.

One pre-existing compact-console integration fixture intermittently failed at
its raw-console stage during S3 verification; a standalone retry and the final
full x64 run passed. T66 did not alter Console, Session or fixture code, so it
was recorded rather than hidden by an unrelated patch.

T66 is closed. The remaining ordered candidates stay in
[Queue](../states/QUEUE.md); no new work is admitted by this closure.
