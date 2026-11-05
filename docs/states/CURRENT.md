# Project Status

## Current Work

Owner admits T99 S32 to normalize tracked text working-tree line endings,
then perform cached full dual-architecture build, publication and tests.
S31 delivery and evidence are retained in its
[record](../history/M9-T99-S31-mirror-condition-audit.md); T99 remains open.
S32 normalization, dual builds and test results are in its
[review](../etc/evidence/softpc/m9-t99-s32-eol-review.md); delivery is in progress.

## M9 T99 S32 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner admits S32 for line-ending correction and full cached verification. |
| Objective | Make tracked text follow Git attributes in the worktree and verify the entire product. |
| Non-goals | Preserve user INI, media, snapshots, binary assets and file contents except line endings; do not change shared APIs or machine behavior. |
| Reference Baseline | SoftPC dff86c90; .gitattributes specifies LF generally and CRLF for the mirror and user INI. |
| Candidate Proposal | [Public handoff simplification](../proposals/m9-public-handoff-simplification.md). |
| Files And ABI Surface | Tracked text with nonconforming worktree line endings, task records and dual EXE artifacts; no ABI changes. |
| Applicable Rules | Execution, documentation, architecture and coding rules; Git attributes. |
| Verification | Audit all tracked text, confirm normalized content unchanged, cached x86/x64 full builds, publish both EXEs, full background and desktop/native-audio tests, governance and mirror gates. |
| Expected Markers | LF outside mirror; CRLF within mirror; explicit INI exception and binaries preserved. |
| Asset Needs | Update both package EXEs, preserve INI, guest media and snapshots. |
| Reporting Requirements | Report normalized paths, Git and worktree representation, test results, release hashes and any limitations. |
| Stop Conditions | Do not normalize user INI or binary files; preserve unrelated changes; do not terminate tests on observation timeout. |
| Exit Criteria | Tracked text conforms to attributes, full builds and required tests pass, release and records committed and pushed. |
| Original Owner Request | 准入一个 S，按照规范修复所有换行，然后 cached 全量编译发布测试提交推送。 |
| Similar-Issue Sweep | Classify every tracked file by Git index and worktree EOL, include mixed endings and binary exceptions. |

Disposable work belongs to build/t99-s32; owner is this executor. Build and
test jobs have a 60-minute overall budget and a 15-minute no-progress review;
logs and exit files remain until work is verified and are then cleaned.

## Current Technical Baseline

S30 final delivery and binary hashes are recorded in its
[final review](../etc/evidence/softpc/m9-t99-s30-final-review.md). The last
full suite passed at P43; later builds had unchanged loadable PE sections and
were not retested per owner direction. The prior intermittent compact-console
initialization failure is not claimed resolved.

## Recent M9 Closures

S30 is owner-closed; its retained evidence and archived packet are in the
[closure](../history/M9-T99-S30-mirror-structure.md).

| Task | Closure |
| --- | --- |
| T98 | [Completion audit](../history/M9-T98-completion-audit.md). |

## Recent Governance

Build caches and disposable work follow the execution rules.
