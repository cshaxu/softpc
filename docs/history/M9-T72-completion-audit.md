# M9 T72 Completion Audit

## Authority And Frozen Scope

Original request: "准入队列第一条，KVM文本属性中性化，先来个设计稿，我审一审".
Owner approved implementation, subsequently extended it through S4--S8, reported
S8 testing passed, and explicitly authorized "行，收口T72".
Baseline bea66d3f through accepted delivery/review ae1dc6e0. This is a T-level
closure, not a new S or an assertion of whole-emulator correctness.

Frozen universe: original neutral-attribute contract and every admitted S1--S8
extension below. Every member requires implementation/review evidence and owner
acceptance or explicit disposition; none remains deferred. The
[retained proposal](M9-T72-kvm-text-cell-glyph-refactor-proposal.md) preserves
estimates and changes of direction; CURRENT is the sole current status source.

## Request And S Coverage

| Member | Result and proof |
| --- | --- |
| S1 design | [Design](M9-T72-S1-neutral-text-design.md): owner approved independent glyph/bank/colour facts and initially parallel arrays. |
| S2 migration | [Delivery](M9-T72-S2-neutral-text-migration.md): VM decodes PC attributes; leaf consumers use neutral facts; Broker packs native colours. Ten production paths, 512-case matrix and dual-width 105/105. |
| S3 audit | [Review](M9-T72-S3-completion-readiness-audit.md): decoding owners, layout/cost, corpus/DAG and changed paths verified; initial array choice later superseded by approved S5. |
| S4 native mouse | [Acceptance](M9-T72-S4-native-mouse-acceptance.md): native relative/absolute input replaces recentering; one delta stream, capture/resource boundaries retained; owner desktop/RDP passed. |
| S5 cells | [Acceptance](M9-T72-S5-text-cell-acceptance.md): four-byte cell array replaces four arrays; frame sizes and leaf resources retained; owner passed. |
| S6 Lib cleanup | [Acceptance](M9-T72-S6-text-render-simplification.md): remove redundant text clear; bounded full-write/sentinel tests. Other initialization retained for its distinct purpose. |
| S7 dirty | [Acceptance](M9-T72-S7-window-pixel-damage.md): both decoders directly use one compare/write/damage helper; no row scratch. Cursor repair and latest-wins retained; owner passed. |
| S8 Common | [Acceptance](M9-T72-S8-common-cleanup.md): all A/B/C/D/F proved; final dual-width 105/105 and owner passed. E was completed in S6. |
| Snapshot inclusion | c0355b2f includes owner-approved snapshot unchanged, not a test dependency or snapshot-format change. |
| TODO/Queue | TODO empty under prior owner policy. Win95 floppy identification, XP SP1 mirror rebase and overlay lookup remain independent unadmitted candidates, not unfinished T72 work. |

## Changed Paths And Costs

Recomputed endpoint `git diff --numstat bea66d3f ae1dc6e0`, tracked C/H under
src/test, not the sum of intermediate rewrites:

| Scope | Files | Added | Removed | Net |
| --- | ---: | ---: | ---: | ---: |
| Production | 23 | 294 | 183 | +111 |
| Tests | 17 | 619 | 90 | +529 |

Docs, manifests, CMake/gates and artifacts excluded. Per-S counts are linked
above. Production paths are Lib text/native input/pixel comparison, VM's device
decoder, Common text comparison/status/input/ownership/debug, and App's terminal
UI teardown receiver. MVDM/Compat and snapshot format unchanged; no new thread,
presenter, frame-cache framework or second input route.

S2 increases each text copy by 1996 bytes; graphics-dominated allocation stays
unchanged. S5 retains sizes. S4 adds native packet/registration handling. S7
adds text comparisons but removes duplicate rendering and row scratch, not a
universal speedup claim. S8 embeds 256 pointers instead of separately allocating
them; unopened Debug object size is not reduced. These accepted costs do not
support a net-reduction claim for the entire T.

## Verification And Closure

Accepted EXEs match S8 evidence SHA256. Both Release builds passed; final
background x64 105/105 (134.41s), x86 105/105 (135.13s), including headless
Win3.1 roundtrips, snapshot/restart and shared-contract regression. Earlier
initial failures and corrected reruns remain in per-S evidence. Five desktop
tests per width excluded; owner acceptance does not claim those tests ran.
Native Linux runtime and unlimited absolute-device motion are not promised.

Rechecked actual delivery paths, source counts, hashes, all-S reviews and owner
acceptance. No admitted item is hidden in TODO or Queue. Incremental build trees
retained; closure creates no runtime process, capture or disposable medium.
Production/test/EXE changes at closure are zero; no rebuild required. Governance,
reference and whitespace checks apply to archival changes. T72 is closed;
no next task is admitted. Post-delivery reviews already precede this closure.

Closure validation: documentation governance passed; x64's documentation gate,
governance self-test and naming check passed 3/3 (2.47s). Whitespace check passed,
all references to the old proposal path were removed, and the closure diff has
no src/test/assets changes. The accepted packages were not rebuilt or replaced.
