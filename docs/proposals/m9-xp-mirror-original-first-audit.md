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

## Owner clarification: diff ranges versus differences

Owner confirms: 尽力恢复；diff 区块数不能准确反映实际情况。
A contiguous diff range is not an independent semantic difference point.
Restoring an original line can split one existing range into two without
introducing any new difference. Report ranges transparently, but do not retain
unnecessary adaptations merely to prevent this fragmentation. Prefer exact
original restoration; verify changed locations and added/deleted diff lines.
The prohibition on new unjustified mirror differences remains in force.
Repeated generated lines can also change heuristic alignment and displayed
line totals. When a fixed original correspondence is provable, report that
anchored count alongside heuristic output; do not claim alignment artifacts
as restored code.

## Original-defect decision ownership

Owner correction: original-code defects discovered during this mirror audit
belong to its pending diff-point decisions, not TODO or a separate queued repair.
Record the original behavior, actual consumers, evidence and limits, and the
proposed difference requiring a decision. An unchanged original defect is not
an existing local patch or approved new diff. Preserve the no-increase rule;
record the decision without implementing a new patch or stopping ordered work.

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

Owner later authorizes temporary deferral of desktop-dependent verification:
"允许你暂时跳过需要这个桌面验证的。等到我回到桌面环境再告诉你。你可以继续去做其他的。"
Continue ordered audit, builds, publication and P delivery with explicit pending
online tests. Preserve failed results; do not turn deferral into a pass, remove
tests or claim full native acceptance. Resume these checks when the owner
reports desktop availability. Other evidence and delivery obligations remain.

Owner further clarifies after reconnecting: later batches that do not affect
native desktop, audio or package paths may omit those tests until final
acceptance. Select tests by the actual changed path, not merely because a
production file changed. Required cached builds/publication, focused proofs
and background regression remain; final native acceptance is not waived.

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

## S2 owner-admitted candidate review

The owner closes S1 and admits S2: "好 加上测试一共发现9个问题待修复 是吧
收口s1 准入s2 我们来过一下". The actual preceding list contains eight items:
seven mirror candidates and one shared-fixture timing issue. Do not invent a
ninth issue or split the timing observations merely to match that estimate.
CURRENT owns activation. This admission authorizes review/design, not blanket
implementation of the proposed new patches or changes to the mirror policy.

| Item | Observed problem | Review outcome required |
| --- | --- | --- |
| D005 | Active PIT backlog reaches zero event delay and stack overflow in the controlled real Machine/PIT probe. | Recheck the two-line candidate's ordering and timing semantics; propose permanent boundary regression and actual diff. |
| D006 | Mode-2 byte write checks bit8 while writing plane2; mask4/value4 yields 00 instead of ff. | Recheck bit4 correction and neighboring masks/paths; give minimal diff and regression scope. |
| L001 | PIC snapshot omits the private ICW sequence phase; restored remaining writes become mask operations. | Specify same-owner archive/schema change and validation, without replay or another state machine. |
| D001 | Generated GenericByteFill loses caller locals across nested GenericByteWrite and crashes. | Establish real consumers and minimal caller-local preservation; do not activate a batch path without need. |
| D002 | Generated DWORD's second WORD goes to +8 rather than +2. | Verify related entries and multi-DWORD fill ordering before prescribing the offset change. |
| D003 | Two overlapping WORD fills produce ten bytes instead of four complete DWORDs. | Establish minimal original-path fill repair separately from D002 and disclose consumer reachability. |
| D004 | Production-wired CHAIN4 mode-2 WORD/DWORD results disagree with the fixture. | Determine actual hardware semantics first; disposition may be test correction rather than production repair. |
| TEST01 | Shared verifier-negative and Types-layout tests time out in aggregate but pass alone within unchanged budgets. | Establish scheduling/startup cost with evidence; preserve negative checks, no blanket timeout increase. |

TEST01 is transferred from TODO into this active review. Its historical evidence
is [P37](../etc/evidence/softpc/m9-t100-progress.md#p37-final-qualification-and-actual-change-review)
and [P48](../etc/evidence/softpc/m9-t100-progress.md#p48-final-qualification-and-actual-change-review):
verifier-negative x86/x64 60.09/60.16 seconds time out, isolated 22.67/19.20
seconds pass; x64 Types reaches 180 seconds then passes alone in 58.65 seconds,
and x86 aggregate passes in 174.99 seconds. No established scheduling root cause.

For each of these eight review units, record actual path, reproducible failure
or unsupported inference, smallest coherent correction, source/XP diff estimate,
regression and remaining uncertainty. Finite completion means every item has a
review disposition; no unproved defect is called fixed and no proposed patch
is silently implemented. Inactive native MIDI observations remain with their
existing S1 decisions, not silently added as a ninth active candidate.

Owner subsequently authorizes D005 and D006 implementation with minimal permanent
regressions and full cached dual-width qualification as S2 P2. The bounded
[repair evidence](../etc/evidence/softpc/m9-t100-s2-repairs.md) records their
actual changes and verification; the other six items remain review-only.

Owner review after P2 declines L001 and D001-D003 repairs for this round.
L001 has a narrow partial-initialization snapshot trigger without a reported
user incident; D001-D003 have no established current guest bulk-entry consumer.
Retain their evidence without patching, activating entries or changing snapshots.
Owner requests D004 hardware semantics and actual production-path investigation
before deciding whether to repair; this does not authorize a production patch.

D004 now has [fresh hardware and dual-width evidence](../etc/evidence/softpc/m9-t100-s2-d004.md):
the scalar BYTE path shares the WORD defect, and actual CCPU DWORD writes
inherit it. A twelve-line replacement candidate passes the bounded matrices
in isolated objects; no production repair is yet authorized or applied.

Owner approves D004 implementation after reviewing the twelve-line candidate.
P4 delivers those scalar stores and permanent regression; all other stated
dispositions and TEST01 pending investigation remain unchanged.

### TEST01 investigation scope

Owner says "好 下一个" after P4, then approves "批准调研". This is a
read-only investigation of test scheduling and fixture cost, not permission
to change production code, timeout budgets or test coverage. Its finite scope
is `emulator.verifier-negative` and `lib.types-layout-selftest`; inspect the
two X86 verifier-negative scripts only for the same process-launch pattern.
Measure the unchanged scripts in isolation and the unchanged x86/x64
eight-job background suites. Separate host-load inference from measured
elapsed time and preserve setup failures. No native desktop run or EXE rebuild
is required for this investigation because no runtime code changes.

### TEST01 measured findings and proposed disposition

Investigation baseline: `403ee272`, November 14, 2026. Unchanged registered
eight-job background suites pass 188/188 on each architecture, without retries:
x86 158.71 seconds, x64 157.65 seconds. No historical timeout reproduces in
this run; this is 376 background tests, not a fresh 392-test desktop qualification.

| Fixture | Isolated script | Aggregate x86 | Aggregate x64 | Child checks |
| --- | ---: | ---: | ---: | ---: |
| Emulator verifier-negative | 20.557 s | 54.64 s | 52.77 s | 103 |
| Types layout selftest | 71.255 s | 106.63 s | 106.22 s | 321 |

Isolated timings use the original scripts with parent-only CMake JSON tracing
and task-owned fixture directories. Trace intervals around `execute_process`
total 20.212/70.470 seconds respectively, roughly 99% of their script duration.
These intervals include process startup and verifier execution, not CPU time.
A separate 100-launch empty `cmake -P` control takes 18.246 seconds: startup
alone is a substantial cost. The Types direct-edge matrix accounts for 200
checks (ten by ten owner/dependency combinations, include and link separately).
Both fixtures use script mode, not compiler invocation or CMake configuration.
Fatal-error rejection is deliberately isolated in child processes; replacing
it with an in-process error protocol would change the checker design.

Emulator has no fixture lock. Types' process-scoped file lock has a measured
sub-millisecond wait in the corrected run. Its registered x86/x64 work paths
are distinct. No shared-lock deadlock is demonstrated. Aggregate runtime
amplification is measured; the precise OS contribution (CPU scheduling, file
scanning, security software or other builds) is not isolated. An unrelated
repository's CMake process was present and was not modified or stopped.

Setup failures are not product failures: the first hidden launcher produced
no test execution evidence; the confirmed replacement completed. The first
Types trace attempt passed Windows backslashes into its generated include and
stopped after 34 checks with an invalid escape. Using forward-slash roots,
as registered CTest already does, completes all 321 checks. The failed setup
is excluded from the timing comparison. A later inline trace-summary command
had a shell quoting error; the file-based analyzer produced the figures above.

Recommended minimal implementation, pending owner approval:

```cmake
# test/emulator/CMakeLists.txt: extend the existing property assignment
set_tests_properties(emulator.verifier-negative PROPERTIES TIMEOUT 60 RUN_SERIAL TRUE)
# test/lib/CMakeLists.txt: extend the existing property assignment
set_tests_properties(lib.types-layout-selftest PROPERTIES TIMEOUT 180 RUN_SERIAL TRUE)
```

Expected code/config diff: two replaced lines (+2/-2), plus two manifest hash
updates and documentation. No source, test case, budget or EXE change. CTest's
documented RUN_SERIAL property excludes every other test in the same invocation;
a shared RESOURCE_LOCK on just these two would still permit unrelated expensive
checks alongside them. All other tests remain parallel. This does not control
another CTest invocation or external machine load and is not a guarantee under
arbitrary host contention. Actual aggregate qualification of the proposed
property is still required after implementation; it was not applied here.

Retain the positive/negative cases and exact dependency matrix: no redundant
case has been proved safe to delete in this investigation. The two X86 negative
fixtures have the same child-process technique but only 13/12 launches by source
enumeration, with fresh aggregate maxima 13.84/10.79 seconds against 30-second
budgets. No scheduling change is proposed for them. Do not introduce a test
runner framework, automatic retry, larger blanket timeout or checker error API.

Single-person counter-review checked actual scripts, original timeout records,
trace counts, registered budgets and the distinction between fresh measured
amplification and an unproven historical OS-level cause. Only this proposal and
CURRENT change. Task-owned traces/scripts are discarded after recording these
results; cached build trees are retained. TEST01 remains implementation-pending,
not fixed; S2/T100 remain open.
