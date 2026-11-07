# Project Status

## Current Work

Owner admits T99 S36 CHAIN2 DWORD read repair. S35 investigation was withdrawn
without a code change; its number is not reused. S34 import remains delivered.
T99 remains open.

## M9 T99 S36 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner requests a new S to verify and fix the two CHAIN2 DWORD readers reported by NTVDM64, with minimal real-rule regression tests. |
| Objective | Read consecutive guest bytes using the existing two WORD calls, offset +2 and explicit low/high 16-bit composition; correct the directly used mode-1 odd WORD next-group offsets. |
| Non-goals | No CHAIN2 layout, shared component, guest, media, INI, snapshot or other repository changes; no claim of a proven Win31 visual root cause. |
| Reference Baseline | SoftPC 26d118e5; preserved XP SP1 a2f6d7c93aa4f11efc51147566d5961c59a8ef89. NTVDM64 report is a lead, not an imported patch. |
| Candidate Proposal | [Public handoff simplification](../proposals/m9-public-handoff-simplification.md). |
| Files And ABI Surface | Mirror base/cvidc/sevid000.c, existing chain2_rule_smoke.c, mirror manifest/ledger and task evidence; no ABI or snapshot layout change. |
| Applicable Rules | Execution, architecture, coding, documentation and source-research policy; owner explicitly admits this original-rule semantic repair. |
| Verification | Prove new assertions fail before repair; cached x86/x64 builds and focused actual-controller tests, full unit/gate/integration tests, both EXEs published. |
| Expected Markers | Read modes 0/1 return complete consecutive DWORDs at odd/even and crossing addresses; preserve original WORD/latch path. |
| Asset Needs | Refresh assets/binary double EXEs only; preserve all configuration, guest media and snapshots. |
| Reporting Requirements | Root cause, exact source/test diff, pinned mirror delta, similar-rule sweep and verification limits; fixed commit for delivery. |
| Stop Conditions | No guest-specific workaround, second read path, unrelated mirror cleanup or timeout termination of progressing jobs. |
| Exit Criteria | Actual-diff self-review and coordinator review, complete prescribed verification, commit/push, clean worktree; wait for acceptance. |
| Original Owner Request | 准入新S修复 S_2089_RdMode0Chain2DwordRead / S_2101_RdMode1Chain2DwordRead：第二次WORD地址+4改为+2，明确组合两个WORD，补真实规则测试，不改变CHAIN2布局。 |
| Similar-Issue Sweep | Inspect all DWORD read entries and actual SAS routing; record independent existing write/fill defects without expanding this read repair. |

Disposable scratch: build/t99-s36, owned by this executor. Background jobs have
logs and exit files; review after 15 minutes without progress, clean on delivery.

## Current Technical Baseline

S36 is implemented and qualified, pending owner acceptance. CHAIN2 DWORD
readers and their odd mode-1 WORD dependency are repaired in one mirror file;
production +12/-20, existing test +33/-0. Both published EXEs and all 388
architecture/test cases are qualified, including the corrected x64 documentation
rerun. See [S36 evidence](../etc/evidence/softpc/m9-t99-s36-chain2-dword-read.md).
Public eight-tree content remains byte-identical to NXVM (296 files). S34's
import delivery remains intact; S35 was withdrawn without a code delivery.
Two independent findings are recorded in TODO, not concealed as passing coverage.
T99 remains open.

## Recent M9 Closures

See [S33 delivery](../history/M9-T99-S33-windowed-dos.md).

| Task | Closure |
| --- | --- |
| T98 | [Completion audit](../history/M9-T98-completion-audit.md). |

## Recent Governance

Follow existing build cache and task-owned probe cleanup rules.
