# XP mirror original-first audit method

CURRENT owns admission and status. The owner approved this method on 2026-11-12
and activated S1, with existing #error exclusions skipped without new accounting.

## Original owner request

> 准入一个新T任务，对softpc.new镜像代码做diff审计，仿照隔壁ntvdm64项目正在进行的审计办法，找出每一处diff，做无罪推定，非语义diff直接恢复，语义diff在无法证明必要的情况下也恢复，有证据的话才列入清单，供我人工审批保留还是恢复原样。具体执行规范你先找出来给我，我确认后方可开始。审计和修复以dir /a/b/s文件顺序进行.

Owner supplement: 如果镜像文件清理后不需要被接入，请整体删除该文件。
Each file review therefore verifies remaining production, declaration and
resource consumers. Delete files made unnecessary by cleanup together with
obsolete build/test/manifest wiring. Preserve their frozen order and deletion
disposition; do not silently shrink the audit universe. Do not remove an
independently needed guest/resource asset merely because it is not compiled.

Owner supplement: 如果文件和已有compat的功能重复，则保留镜像文件的实现清理掉compat。
Original implementation takes precedence over duplicate Compat behavior.
Before deleting a mirror file, check whether restoring its original wiring
can replace a Compat implementation. Restore and use the mirror path, remove
the duplicate Compat implementation and obsolete callers/tests, and verify
the resulting single path. Existing Compat consumers do not prove a mirror
deviation necessary. Genuine native host/ABI adaptation remains at its proper
boundary; do not mistake required native bindings for duplicate device logic.

Owner clarification: 类似情况相同规则处理，不得擅自增加 diff 点位或行数。
Final owner instruction: 不要停下来等我批准，类似情况不得发生，一概禁止。
Within this audit, increases in XP-relative diff points or added/deleted diff
lines are prohibited, including removal of unused files/includes/declarations.
Do not seek an exception or stop for approval of an increasing candidate.
Abandon that candidate, preserve the original file and wiring, and continue
ordered auditing. Net source reduction does not justify more mirror differences.
Report point count and line delta separately. This supersedes the earlier
increase-approval clause and broad whole-file retirement interpretation.
The owner rejected bswap.h retirement on this basis; its original file and
include remain. Evidence-backed existing residuals still follow the owner's
original keep-or-restore decision process; this does not authorize new diffs.

## Method sources

Read on 2026-11-12 in the read-only NTVDM64 comparison tree:

- docs/rules/EXECUTION.md, Original-first mirror diff audit: consolidated
  17-rule policy and file-level reporting clarifications.
- docs/rules/CODING.md, Mirror difference register migration.
- artifacts/mvdm-audit/t483-remaining-reaudit/README.md: burden of proof,
  strict decision admission and complete-file delivery cadence.

These are method references, not SoftPC source or build dependencies. No
NTVDM64 patch is admitted for import. Neighbor worktree rules may be newer
than its committed revision; no immutable neighbor revision is claimed.

## Approved execution contract

1. Freeze the SoftPC starting commit and local file hashes; use the already
   pinned XP SP1 a2f6d7c93aa4f11efc51147566d5961c59a8ef89 counterparts and
   provenance manifest. Do not consult OpenNT source or substitute another
   upstream because its diff is smaller. Missing provenance is unfinished
   investigation, never permission to invent an original.
2. Capture the literal cmd directory listing from the mirror root using
   `dir /a /b /s`. Preserve its returned order without adding `/o:n` or
   re-sorting. Record directory entries separately; the file subsequence is
   the audit order. Account for non-source assets without altering guest or
   firmware. Within a file process differences in ascending current location,
   retaining original ranges and anchors for deletions. Read dependencies as
   needed without claiming later files audited.
3. Establish a finite file/point ledger from the actual frozen tree, not the
   historical patch register alone. Enumerate identical files too. Reconcile
   every in-scope diff hunk with an audit point; existing #error exclusions
   are skipped without new records, IDs, counts or approval items;
   insert discoveries at their proper position and explain ledger revisions.
4. Preserve established excluded-branch #error and required #line scaffolding;
   skip existing exclusions without new bookkeeping and do not reopen removed
   bodies. Outside that scope audit every difference, including historical accepted fixes,
   comments, declarations, omitted selected content and ABI adaptations.
5. Presume original code valid. Historical approvals, comments, hypothetical
   risk, current adapter dependencies and passing modified-code tests alone
   do not establish necessity. Inspect the real supported build/caller path
   and the coherent original restoration, not an artificially broken partial
   restoration.
6. Restore purely nonsemantic differences exactly to XP, including original
   syntax and formatting; do not substitute an equivalent rewrite. First
   check macro continuation, preprocessing tokens, line mapping and notices.
   Retain SoftPC's required mirror line-ending policy and classify any
   unavoidable format-policy difference explicitly rather than hiding it.
7. For semantic differences, investigate the bounded real context. Restore
   exact original code when no concrete necessity is established, and verify
   the restoration. Unknowns remain unfinished investigation, not approved
   retention or a request for the owner to solve the investigation.
8. Concrete source/ABI proof or observed build/link/runtime failure must
   demonstrate that coherent restoration breaks an actually supported
   contract before a residual difference enters the owner's decision list.
   State evidence type and limits; no claim of runtime reproduction from
   source reasoning. The owner chooses retention or original restoration.
9. Approved adoption of the neighbor's minimization rule: implement proven
   reductions in necessary differences, without new helpers/second paths or
   moving equal complexity outside the mirror. Residual differences still
   require owner approval; increases in original-relative diff points or lines
   are prohibited, not candidates for a new approval request. Existing out-of-mirror caller edits must
   be disclosed and limited to necessary coherent restoration, not redesign.
10. Complete all current-file investigations and reducible edits before a
    substantive batch delivery. Qualified owner decisions may remain pending
    while ordered auditing continues, but are not closed repairs. Coupled
    edits do not complete another file's audit. No fixed ten-file threshold
    is proposed: the neighbor's latest rule explicitly replaces numeric
    batches with complete-file, impact-based delivery.
11. Formatting/reference-only changes may accumulate. A complete file with
    substantive restoration/minimization triggers cached x86/x64 build,
    coherent dual-EXE publication, applicable focused and full regression,
    then commit/push. Do not treat an old candidate's test as final-source
    evidence. Test omission requires demonstrated artifact equivalence under
    the owner's existing equivalence exception, accounting for mandatory
    timestamp/version changes, not merely a claim of equivalent source.
12. Follow SoftPC background execution, fresh build-timestamp, cache and
    scratch-cleanup rules. No foreground timeout termination, new guest
    builds, configuration edits, or sibling-repository mutations.
13. Before/after each diff-bearing file report position, path, point IDs,
    exact XP-relative added/deleted scope, disposition, verification and
    delivery. Per point record initial analysis and independent counter-review
    separately under the single-person dual-role process.
14. Report source additions/deletions separately from XP-relative diff
    additions/deletions and their delta. Reduced source length is not reduced
    mirror diff. Do not create counts or ledger entries for existing #error exclusions.
15. Keep distinct counts for identical files, sequentially audited files,
    restored-and-delivered points, local pending verification, approved
    retention, qualified pending decisions, unfinished investigation and
    unreviewed points. Do not declare T closure with unresolved decisions or
    investigation. Reconcile the existing mirror register as restorations
    land; do not delete history or create conflicting retention authorities.

## Decision record and convergence

Each evidence-backed decision contains file and original/current ranges,
exact original/local difference, real supported compilation and caller chain,
the concrete failure of coherent original restoration, before/after proof,
minimal residual option, XP-relative size, owner choice and delivery identity.

Completion requires every frozen file and every non-excluded difference to
have a reconciled disposition, every retained difference to have owner approval,
every restoration to have final-source verification/delivery, and no unresolved
investigation. The ordered file ledger is frozen after method approval. The
existing 472-file manifest is a baseline reference, not fresh coverage evidence.

## Initial stages

- Method confirmation: read-only comparison and this proposal; no code changes.
- After approval: freeze ordered ledger, then process complete files in order
  through admitted implementation subtasks and P deliveries.
- Final task audit: reconcile every point, owner decision, artifact and test;
  wait for task closure approval. Performance work remains in Queue.

## Planning verification

Owner approved execution with the no-exclusion-bookkeeping correction.
[Task evidence](../etc/evidence/softpc/m9-t100-progress.md) records actual
ordered progress; no historical exclusion ledger is duplicated.

No mirror code, guest assets or binaries were changed or exercised by this
planning record. Documentation checks do not establish restoration safety.
