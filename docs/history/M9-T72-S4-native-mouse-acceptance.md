# M9 T72 S4: Owner Acceptance And Closure

## Request And Scope

Owner: "desktop和rdp测试均通过，收口本次S任务，并告诉我本T任务的原始目标、
更新目标和收口条件满足清空"; correction: "清空 => 情况".
This approves S4 closure and asks for T72 readiness, not deletion of records or
automatic T closure. No new implementation subtask or queued work is admitted.

S4 delivery aba5022c and post-push actual-change review 9f65aa67 are recorded in
[the repair evidence](../etc/evidence/softpc/m9-t72-s4-native-mouse-motion.md).
The finite input/capture/ownership/failure ledger there is the scope of this
acceptance, not a claim that every desktop or remote-input configuration works.

## S4 Exit Criteria

| Criterion | Evidence / disposition |
| --- | --- |
| One native motion route, no recenter | Delivered and reviewed; relative/absolute, repeated samples, device/geometry transitions and duplicate-stream tests pass. |
| Capture lifecycle and API boundary | Existing release/freeze/hotkey paths retained; foreign raw registration not stolen; public event/API signatures unchanged. |
| Builds and regression | Both Release builds with strict Lib warnings; x86 105/105; x64 104/105 then documentation-only failure corrected and 1/1 rerun passed. |
| Manual behavior | Owner now explicitly reports desktop and RDP both passed. Five excluded desktop automation tests are not retroactively claimed executed. |
| Delivery and hygiene | P1/P2 pushed; package hashes rechecked against evidence; clean worktree at this closure's start. Temporary diagnostics removed; INI/media unchanged. |
| Review | Post-push review already completed; this turn rechecked committed file accounting, hashes, original request and all criteria. |

All S4 exit criteria are met. S4 is closed by owner approval. This turn changes
documentation only: production/test +0/-0, binaries unchanged, no rebuild needed.
Documentation governance and diff checks are required for this closure commit.

## T72 Original And Updated Goals

Original: neutralize the shared KVM text attribute contract. Keep parallel arrays
and the existing leaf resources; VM alone decodes device attribute bits into
glyph index, bank, foreground and background. Window and Console consume those
independent facts, with native Console encoding confined to Broker. Preserve
existing output, capacities, frame transport and lifecycle; do not redesign
text cells, font registration, graphics or the machine core.

Updated: retain that completed text migration and add owner-approved S4 repair
of the pre-existing T70 recenter/input defect discovered during acceptance.
Remove pointer warping, use native relative records or successive absolute
positions, preserve copied integer-delta events and capture/hotkey behavior.
This is not an unapproved consequence of the text ABI migration.

## T-Level Readiness Ledger

Frozen universe: original S1--S3 text-migration ledger plus the explicitly
admitted S4 input ledger. Completion requires each admitted S, original request,
changed-path proof and owner test to be accounted for; queued unrelated work is
not a blocker. This extends, rather than overwrites, the
[S3 audit](M9-T72-S3-completion-readiness-audit.md).

| Requirement / S | Status |
| --- | --- |
| S1 design | Approved and closed; retained parallel-array and two-resource-bank design. |
| S2 migration | Closed; all ten production paths migrated; 512 attribute/bank combinations plus validation/comparison/cache tests; dual-width regression passed. |
| S3 audit | Closed; actual changes, ABI/layout costs, manifests/DAG, protected boundaries and finite ledger reviewed. |
| S4 additional repair | Closed above; native input proof plus owner desktop/RDP acceptance. |
| Compatibility and cost | Text source ABI changed together with consumers; text copies +1996 bytes, graphics-dominated allocations unchanged. No MVDM/Compat/media/snapshot-format change. |
| Outstanding transfers | No T72 implementation deferred to TODO or Queue. Existing three candidates are unrelated; TODO tracking policy is not a proof of global correctness. |
| Build, test, artifacts | Evidence linked above; both binaries retain delivered SHA256 values. No new runtime code in closure. |
| Formal T closure | Ready, but NOT performed: owner requested this S closure and T readiness report only. |

Git diff --numstat bea66d3f 9f65aa67, restricted to tracked C/H: production
17 files +198/-76, net +122; tests 11 files +274/-41, net +233. S2 production
net +18 and S4 net +104 account for the total; manifests/docs/binaries excluded.
No new unresolved technical blocker in the admitted scope was found during
this evidence review. Native Linux execution and unlimited absolute-device
motion remain outside the promised scope, not silently asserted capabilities.
