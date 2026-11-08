# Project Status

## Current Work

T99 S37 is implemented and verified, awaiting owner acceptance. Three proven
original-code defects are repaired; PIC and keyboard retain their existing
paths with regressions. See [S37 evidence](../etc/evidence/softpc/m9-t99-s37-original-device-audit.md).

## M9 T99 S37 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner admits a new S for independent verification and minimal fixes of five reported defects; no patch copying. |
| Objective | Verify CCPU BIOS page permissions, slave BIOS EOI, virtual CRTC text height, PIC reinitialization EOI cancellation, and keyboard early EOI ordering on actual SoftPC paths. |
| Non-goals | No guest binary edits/builds, external repository changes, foreign timer/keyboard mechanisms, recovery framework or unrelated TODO repairs. |
| Reference Baseline | SoftPC 2b7a097a; pinned XP SP1 a2f6d7c93aa4f11efc51147566d5961c59a8ef89; NTVDM64 observations are leads only. |
| Candidate Proposal | [Public handoff simplification](../proposals/m9-public-handoff-simplification.md). |
| Files And ABI Surface | Investigate ccpu386/ccpusas4.c, bios/slave_bs.c, video/ega_vide.c, system/ica.c, keyboard controller and actual Compat callbacks; existing owner-local tests and mirror ledger. No intended public ABI changes. |
| Applicable Rules | Execution, coding, architecture, documentation and source-research policy; owner authorizes proven original semantic repairs. |
| Verification | Same before/after fixtures, actual compile and call evidence for each item, x86/x64 focused and full cached unit/gate/integration qualification; distinguish guest observations and unverified boundaries. |
| Expected Markers | Correct CPL permissions/fallback, command-port EOI, no unproven virtual CRTC patch, no stranded actual EOI consumer, no unread keyboard overwrite from early EOI. |
| Asset Needs | Publish both EXEs after production edits; preserve INI, disks and snapshots. |
| Reporting Requirements | Five-item applicability ledger, actual defects and proof, minimal design/diff estimate before edits, source provenance, actual diff and full verification results. |
| Stop Conditions | No imported patch or guest modification; no invented NTVDM mechanisms; do not terminate progressing background jobs for interactive timeout. |
| Exit Criteria | All five items have evidenced dispositions, proven fixes and regressions, actual-diff dual-role review, build/publish/test/commit/push, clean worktree, await owner acceptance. |
| Original Owner Request | 准入新S检查并修复确定的问题：六处CPL/PG_U判断；D11从片EOI端口；仅证实适用才修虚拟CRTC高度；追踪实际EOI回调后判断重初始化取消；按本机键盘路径判断提前EOI覆盖。先整体审计，再整组实施；不得照搬NTVDM64或修改guest。 |
| Similar-Issue Sweep | Enumerate equivalent access flags, EOI port writes, callback registrations/cancellation and keyboard consumption/queue advancement; record out-of-scope findings explicitly. |

The executor-owned build/t99-s37 scratch and temporary guest target are cleaned
before delivery after result promotion. Only build/.ninja and build/.ccache remain.
Both EXEs are published; 390 architecture/test cases are qualified, including
one unchanged-artifact x86 package rerun after an initial startup failure.

## Current Technical Baseline

S36 delivery 8c27a3b6 is owner-accepted and closed. CHAIN2 DWORD
readers and their odd mode-1 WORD dependency are repaired in one mirror file;
production +12/-20, existing test +33/-0. Both published EXEs and all 388
architecture/test cases are qualified, including the corrected x64 documentation
rerun. See [S36 evidence](../etc/evidence/softpc/m9-t99-s36-chain2-dword-read.md).
Public eight-tree content remains byte-identical to NXVM (296 files). S34's
import delivery remains intact; S35 was withdrawn without a code delivery.
Two independent findings are recorded in TODO, not concealed as passing coverage.
T99 remains open.

## Recent M9 Closures

See [S36 closure](../history/M9-T99-S36-chain2-dword-read.md) and
[S33 delivery](../history/M9-T99-S33-windowed-dos.md).

| Task | Closure |
| --- | --- |
| T98 | [Completion audit](../history/M9-T98-completion-audit.md). |

## Recent Governance

Follow existing build cache and task-owned probe cleanup rules.
