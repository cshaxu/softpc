# M9 T55 — Types Vocabulary and Component Platform Boundaries

## S10 admission: complete approved audit repair set

Owner: “按照这些协议，把前面我批准的所有修复做一下，好了就提交推送，然后让我来测。”
Baseline `bad9ab2`. S9 delivery is retained below; T55 stays open.
The fixed universe is the nine approved findings from the 93-file audit.
Each row must have implementation and focused proof before delivery.

| Item | Owner and single-path design | Required proof |
| --- | --- | --- |
| Capture loss | ui-window clears its existing capture/button/motion state once; never releases another window's capture. | Native loss and reentrant voluntary release. |
| Cooked line size | host collects a complete bounded line; oversized input drains to terminator and emits one rejection. | Exact limit, split terminator, overlong tail, next line and cancellation. |
| Capture bounds | ui-window refreshes current client clipping on geometry change; failure releases capture. | Move/resize, uncaptured noop, failed refresh. |
| Text glyphs | console owns a fixed PC-display glyph-to-Unicode mapping, not a font ROM or inferred guest encoding; host writes wide cells. | ASCII, low symbols, box/block and extended characters; no frame ABI expansion. |
| Native outcomes | Only successful native operations advance completion caches; unsupported palette capabilities may retry without a false applied cache. | Palette failure/retry, resize failure, title/cursor failure. |
| Input entry | Remove obsolete transition entry; tests/diagnostics use record normalization. | No production or test old caller, keyboard regressions. |
| Internal simplification | Single-file helpers static; storage exact read/write/flush directly own checks and CRT calls. | Existing storage and input proofs. |
| Geometry ownership | Pure sizing/anchor arithmetic in existing root geometry; Win32 only marshals. | All drag edges and geometry regressions. |
| Documentation | Correct focus and terminal broker-failure descriptions to approved behavior. | Governance and implementation comparison. |

Peer sweep found SoftPC ignored REJECTED_LINE. Its monitor adapter and existing
control queue now carry rejection explicitly to command reporting/prompt;
no line-boundary policy moves out of host. This narrow consumer adaptation is
required for the approved once-only visible rejection.
No changes to MVDM, product lifecycle, INI/media, Console mouse scaling,
frozen key semantics or Linux UI parity. Fixed display mapping is a deliberate
approximation for host fonts, not recognition of arbitrary uploaded fonts.
Executor performs full x86/x64 builds/tests and strict library checks, updates
manifest and packages, pushes P1; coordinator reviews actual P1 before P2.

### S10 executor evidence and nine-item disposition

All nine rows are implemented. Capture loss and bounds share the existing
Window cleanup and one refresh helper; HWND is ownership identity, not an
additional capture flag. Tests invoke the production message procedure for
WM_CAPTURECHANGED, voluntary reentrant release, move/resize and failed clipping.
Pure sizing now lives in root geometry; all eight drag anchors are checked
from non-proportional proposed bounds. Native resize failure leaves its cache
unchanged, and title failure enters the existing component fault path.

The host reader accumulates at most 1023 bytes, drains overflow to LF, and
delivers one rejection. Tests use one-byte and 1023-byte native chunks at
1022/1023/1024 lengths, split CR/LF, a following help line and mid-read
cancellation. Monitor rejection traverses the existing control queue and
command prompt path; no partial command is dispatched. No VM state changed.

The fixed 256-entry mapping replaces ASCII filtering at wide-cell output.
Proof covers blank/ASCII, low display symbols, accented characters, box/block
glyphs; it is not a bitmap font or code-page inference. Failed palette query
or application does not advance the palette cache; a later success does.
Cursor-info and cursor-position failures report IO_ERROR. Tests exercise the
production host backend with controlled native calls, without desktop sleeps.

The obsolete transition function has no implementation/declaration/caller;
all affected tests and the boot diagnostic use record normalization.
Unmapped status is explicitly preserved at boolean callers. File-local
failure/key helpers are static. Storage removes redundant read/write/flush
layers and the single-caller writer-open wrapper; stream ownership and public
writer behavior remain unchanged. No compatibility forwarding entry remains.
Types-layout now forbids restoring the obsolete entry or ANSI cell output.
The existing negative layout selftest still passes.

Peer sweep used rg over src/lib and test for the removed transition/open
names, both file-local helpers, capture APIs, previous caches, and native
text/cursor/title/size operations. Remaining helper references are local;
removed names occur only in negative gates. Palette and size are retryable
without false completion; required text/cursor/title errors are explicit.
Focus and retirement-failure docs now describe the implemented contract.
No MVDM, INI, media, Console mouse scaling, frozen filtering, hotkey policy,
or Linux UI parity change is included. Existing TODOs remain unchanged.

Verification: x64 full 52/52 (60.16 s), plus the final strengthened geometry
and cursor focused tests 2/2; x86 full final 52/52 (77.88 s). Both package
smokes passed. Strict -Wall -Wextra -Wpedantic -Werror build and standalone
3/3 passed; manifest, DAG and documentation gates passed. The initial full
run's only failure was the missing S9 handoff history, now recorded; it was
not a product failure. Early compilation caught a missing constant alias and
a test fake/local-name collision; both were corrected before final runs.

Against bad9ab2: 25 production C/H paths +242/-182 (net +60); seven test
paths +288/-40 (net +248); two build/gate paths +5/-2. Counts use git diff
--numstat with new test files included, excluding docs, manifest and EXEs.
The fixed mapping accounts for 42 production lines; cleanup removes duplicate
paths rather than introducing a new platform layer. Owned types-layout test
fixture copies are removed after verification; reusable build caches remain.

Package SHA256:
- softpc32.exe: `7EA56F88820519F17737A5EEB7B3319E8546909A90A4BADFDF13387B8B016927`
- softpc64.exe: `78147A7B42B9986E946621043A6D859C58E6270B0B605980144DBECC6E06B6BB`

### S10 P2 coordinator committed-diff review

Executor P1 `fe11510` was pushed before review. Coordinator inspected its
actual 43-path Git change, not just executor evidence, against all nine rows.
The shared corpus retains the same component DAG. Mouse cleanup is idempotent
before reentrant release; native capture belonging to another HWND is not
released. Bounds refresh is restricted to existing capture. Host retains one
reader/transaction path. Fixed glyph conversion occurs only at text-cell output;
no text-frame schema or application code-page policy was added. The only app
change carries rejected cooked lines to the existing command reporter/prompt.

Removed wrappers have no surviving declaration or caller, and changed tests
exercise the production adapters/worker helpers under controlled native
results. Geometry tests check non-proportional bounds and all anchors; native
output tests check both cursor failure sites and recovery. Final package hashes
match P1 and recorded evidence. Manifest and governance passed after commit.
No unrelated source/media/config changes are included; generated fixture
copies were removed. No additional blocker was found within the admitted
nine-item repair set. S10 is delivered for owner testing; T55 stays open,
and the existing intermittent package TODO is not claimed fixed.

## S9 admission: pending ordinary replay permission

Owner: “同意 如果frozen期间到frozen之后都没匹配 确实可以丢弃”;
“批准实施 干净为准”. Baseline `8761fad`.
The broader audit's make/break balancing proposal is explicitly NOT admitted.
Window continues per-event frozen filtering; releases do not gain exceptions.

| Frozen sweep | Owner/disposition | Proof |
| --- | --- | --- |
| All pending make creation/flush paths | Existing matcher entry captures generic allow_replay once. False skips only its ordinary make on flush, then follows the existing resolved state; matching and releases are unchanged. | Mismatch, release and TEXT flush; repeat cannot grant an old make permission. |
| Window vs other emit callers | Window supplies !frozen through existing emit_to; default component emit and Console supply true. No frozen state in ui-base. | Frozen-to-unfrozen successful chord still produces hotkey; ordinary Console path unchanged. |
| Native/control and failure boundaries | Existing Window final filter remains the only current-state output policy. No second table/reset/synthetic releases. | Actual control mailbox transitions, sink failure, both half-pair cases retain approved per-event semantics. |

No new component/queue/input event field or normalization change. Existing tests
gain the cross-boundary cases; complete x64/x86 build/test, strict gates and
manifest precede pushed P and actual-diff review. T55 stays open.
Only this admitted pending-replay defect is fixed; no claim of whole-lib closure.

### S9 executor evidence and peer sweep

The original pending-make insertion captures allow_replay. The single
ui_hotkey_flush_pending skips an ineligible make and advances its existing
disposition; it does not mark the chord consumed or alter release handling.
No predicate was added to registry matching. Repeats cannot rewrite the stored
permission. Window supplies !frozen through the existing emit_to path; ordinary
component emit supplies true, preserving Console behavior and source attribution.
The Window final frozen filter is unchanged. No app-facing input schema,
new heap allocation, pressed table, thread or callback was introduced.

All submit/emit callers were swept with rg; four existing test files adapt to
the explicit argument. The focused Window test adds six origin/flush cases
(mismatch, release and TEXT for each origin), repeats after thaw, both
cross-boundary hotkey directions, both deliberately unbalanced make/break
directions, and one terminal sink-failure case. Control changes use the real
mailbox consumer, without sleeps or an actual desktop-focus dependency.
This proves the approved small contract rather than silently broadening it.

Final x64 full passed 50/50 (34.61 s), x86 50/50 (41.22 s), including both
package tests; strict -Wall -Wextra -Wpedantic -Werror build and 3/3 gates passed.
The earlier focused five-test x64 run passed before adding the two explicit
half-pair non-regression cases; those final cases passed in both full runs.
Manifest, dependency and governance gates passed. No new test failure or
unrelated runtime repair occurred. Existing TODOs remain unchanged.

Against `8761fad`: five production C/H paths +20/-13, net +7; five test C
paths +107/-34, net +73 (including signature updates). Counts use
git diff --numstat, excluding docs/manifest and artifacts. The initial test
estimate was exceeded by explicit non-regression cases, not production scope.
No app/MVDM/standalone-host/INI/media edits. Owned types-layout-fixture copies
are removed after tests; fixed build caches remain.

Package SHA256:
- softpc32.exe: `095D2BA3F266F8D13497A8049EBAA2B8D7337CFE816ED06B60DB7747173ECBCC`
- softpc64.exe: `B43FA7EBB789F27C50E6C1B8DAFF8790F8368652A648663215BBB4E239FD41A3`

### S9 P2 coordinator committed-diff review

Executor P1 `8df0599` was pushed before coordinator review of its actual
19-path diff against the narrowed owner request. The five production C/H
paths add one stored permission and thread it through existing calls.
Registry matching, current frozen filter, release path, normalization and
Console native implementation remain unchanged. The only replay sink call
is gated; all three flush causes use it. Tests verify exact ordinary/hotkey
outputs rather than only success statuses, including the explicitly retained
half-pair semantics. Signature-only test changes preserve prior assertions.

Both fixed EXEs match recorded hashes. Full x86/x64 50/50, strict 3/3 and
governance passed; no app/MVDM/INI/media edits exist. Owned fixture copies
were removed. S9 is delivered for owner inspection, not T55 closure; the
existing intermittent package TODO is not claimed resolved.

## S8 admission: complete-character repetition and recovery

Owner: “批准修复s任务 干净”. Baseline `0ca0875`.
The preceding audit used controlled input, not a real-desktop reproduction.

| Frozen sweep | Owner / implementation | Proof |
| --- | --- | --- |
| All native repetition and UTF-16 producers | Host/Window copy repeat count; ui-base expands physical makes or decoded complete characters, not surrogate units. Console raw metadata gains count; zero means one, physical break stays single. | Both leaves: BMP and paired non-BMP count batches, single-record equivalence and sink rejection. |
| Malformed UTF-16 prefix transitions | ui-base discards an obsolete high surrogate but processes the next valid BMP/high unit; lone lows rejected, count-mismatched pair rejected and reset. | High-BMP, high-high-low, isolated low, intervening physical record, subsequent valid input. |
| Every scalar-to-physical map failure | Existing TEXT fallback covers both layout failure and unrepresentable mapped physical key. | Controlled platform map, scalar unchanged, no synthesized partial keys. |

Only pending high-surrogate/count state is required; no extra queue, matcher,
component or product callback. Both halves of a repeated pair must agree in
count; an inconsistent pair is malformed, never silently truncated or multiplied.
The existing one-unit helper becomes the same counted helper (all callers
updated), not a second forwarding API. Full x86/x64 and strict tests, manifest,
peer scan, complete P push and actual-diff review precede delivery. No app,
MVDM, media/INI, mouse-scale or lifecycle changes. T55 stays open.

### S8 executor evidence and similar-issue dispositions

The repeat owner is now ui-base. Native Window transition/character messages and
host wide raw records copy their original count; Console forwards it unchanged.
The common normalizer expands physical makes (one break), or full decoded
scalars. Only the pending UTF-16 prefix gains a count. The app-facing
ui_input_event, matcher ledger and failure/retirement owners are unchanged.
The Console raw struct and leaf-support helper are source-contract changes;
consumers must rebuild, and raw-input consumers must interpret copied count.
No compatibility forwarding path or duplicate repetition loop is retained.

Both current platform adapters run the same seven-case repeated/recovery
matrix: paired repeat and individual equivalence, replacing a bad high,
isolated low recovery, mismatched counts then recovery, zero-as-one and
high-then-BMP. Additional assertions cover a physical event interrupting a
prefix, break counts staying single, text-only releases not resetting prefixes,
and failure on the second decoded scalar stopping both components without retry.
The actual host reader fixture checks copied wide units and count values;
the controlled layout query returns VK_OEM_102, proving an unrepresentable
physical mapping produces three unchanged TEXT scalars. Linux's existing
normalizer contract is updated and still passes; no new Linux UI claim.

Sweep used rg over every submit_utf16/submit_record, repeat_count, raw_key and
native repeat producer in src/lib and tests. Production peers are Window,
host raw reader and Console adapter, all changed together. Isolated test
callers use count one; zero-initialized synthetic records remain count one.
No app, standalone host, MVDM, INI or media path changed. Unmapped transitions
still invoke native translation only once, carrying the native count onward.
Malformed data rejection remains distinct from an attempted sink failure.

Verification: x64 full 50/50 (58.25 s), x86 full 50/50 (76.22 s);
both package tests passed. Strict library build uses
-Wall -Wextra -Wpedantic -Werror and passed 3/3, including manifest,
DAG/types and Linux build contract. Focused x64 five-test run passed too.
The first manifest refresh accidentally listed the manifest itself; its gate
caught this and the entry was removed before final passing verification.
No runtime/test assertion was weakened for that tooling error.

Against `0ca0875`, production C/H six paths: +50/-46, net +4.
Test C five paths: +104/-39, net +65. Counts use git diff --numstat,
excluding docs/manifest/EXEs. Reused existing test targets and the shared
normalizer; no new component, queue, heap object or test executable.
Owned types-layout-fixture copies are removed after testing; build caches remain.
The S7 intermittent package timeout remains in TODO, not closed by these passes.

Package SHA256:
- softpc32.exe: `46126CA58E864FC4EB8D0BC4938098CCF68C7C5765839EA2972A07CB0123D67E`
- softpc64.exe: `87F58496944ABEFBB878F1F33A156E947B4B53CC8DA6A3A4620D44C20B66E294`

### S8 P2 coordinator review

Executor P1 `939755d` was pushed before this review. The coordinator checked
its actual 19-path committed diff against the three-row packet: one counted
decoder, both native adapters, the Console copied contract, five test files,
manifest, design/current/history and both artifacts. The two previous producer
loops are removed, not retained beside the common decoder. The only added
persistent decoder value is its pending prefix count; no matcher state is copied.

Old malformed-prefix assertions were deliberately changed because they encoded
the reported loss; adapter tests additionally verify the recovered output, not
merely successful return. Host tests now assert preservation of metadata rather
than expansion; the leaf tests assert the final expansion. Failure tests still
require terminal stop and exact attempt counts. Dependency gates, full-width
tests and strict warnings passed without waiver. Artifacts match recorded hashes.
The owned fixture copies were deleted; no app/MVDM/INI/media changes or remaining
worktree changes belong outside the packet. S8 is delivered for owner testing;
T55 stays open, and the historical CAP timeout remains explicitly unresolved.

## S7 admission: native input provenance and synthesis ownership

Owner: “请问这次修复会加代码还是减代码 是否能干净的实现”;
“准入修复 要求代码设计有全局观 干净”. Baseline `5be6fc4`.
S6 manual tests passed; that does not establish native-boundary completeness.

| Frozen input-boundary sweep | Disposition/owner | Required proof |
| --- | --- | --- |
| Separate Window transition/character production | ui-base reports unmapped transitions; Window invokes native character translation only for those. Remove character_key guessing rather than adding a second queue. | Interleaved scan-less physical keys generate no duplicate character; unknown keys still translate. |
| Text synthesis and all held key identities | Read the existing matcher ledger, never a second pressed table. Synthesis releases only keys it pressed, respects either modifier side and keeps a held trigger down. | Held modifiers/trigger, ordinary text, replay and sink rejection. |
| Native Unicode input, both leaves | Raw host reads W records; Window uses a Unicode class/message path. Existing narrow title API retains its native ANSI conversion. | Native reader record and actual Window procedure, non-ASCII/surrogate pair, not just pre-normalized fixtures. |
| Native repeat batches | Host expands raw record repetitions; Window handles message repeat count at its input boundary. Public event schema unchanged. | Batch versus individual equivalence and failure mid-batch. |

One normalization entry and one matcher remain. No new component, product
callback, input channel or duplicated pressed state. The Window ANSI class is
an encoding peer discovered in the same scan (including VkKeyScanW instead
of narrowing Unicode through VkKeyScanA): fake WM_CHAR tests had bypassed
its native conversion, just as fake Console records bypassed ReadConsoleInputA.
Do not rewrite MVDM/app policy, mouse scaling, cooked line encoding, storage or
Linux parity. Complete the four rows and native peers with focused tests,
full dual-width tests/builds, strict gates, manifest and actual-diff review.
Record production/test changes separately; leave T55 open for owner testing.

### S7 executor evidence and peer dispositions

Physical input no longer generates its own WM_CHAR echo: the one normalizer
returns UNMAPPED only when the selected platform cannot represent a transition.
Window translates only that case, inside its existing native message handler.
Removed character_key entirely instead of adding credits, a timer, or a queue.
Console still chooses the physical representation of a combined record first.
Independent Unicode text still uses the existing physical synthesis/TEXT policy.

Synthesis takes a synchronous read-only borrow of the component's existing
matcher ledger. A four-key stack chord snapshots identities before callbacks
can grow that ledger, reuses either held modifier side and a held trigger, and
releases only keys it introduced. No second persistent pressed state exists.
Both leaves pass their same matcher explicitly; focused standalone decoders
pass NULL. All sink failure routes still terminate through the existing owner.

Native peers: ReadConsoleInputW, Unicode Window class/create/dispatch/peek/default
procedure and VkKeyScanW preserve Unicode before normalization. Narrow titles
retain SetWindowTextA conversion before the hidden Window is first shown.
MapVirtualKeyA is retained for scan lookup (not character conversion); custom
integer messages, native userdata access, and existing cooked/output encoding
remain unchanged. Types contains only the corresponding declaration vocabulary.
Host expands raw make repeat counts, treating zero as the historical single
synthetic record; releases remain single. Window expands its message count too.
No public event-schema, app, MVDM, media, INI or mouse-scale change.

Focused proof includes the prior 432 chord permutations plus 128 combinations
of held modifiers, physical sides and held trigger during repeated text
synthesis; all four synthesis rejection positions; mid-repeat rejection;
interleaved scan-less transitions without translation, unknown-key translation,
separate/combined equivalence, and BMP/surrogate records. The host reader test
drives its actual read loop with a controlled wide API, including repeat batches,
zero count and single release. Actual hidden Window creation asserts a Unicode
window. This is native-adapter proof, not a claim of full IME or RDP desktop QA.
The static gate now rejects ANSI input producers; negative probes cover both
affected native components. Existing dependency and low-level bypass gates stay.

During verification the isolated vocabulary test initially lacked the now-used
matcher body; its fixture now includes that actual implementation. One x86
build was invoked without its compiler DLL PATH and failed without diagnostics;
the checked-in preset with SOFTPC_I686_BIN restored the declared environment.
Neither issue changed production semantics. The S6 handoff history was also
recorded so S7 satisfies the strictly increasing step gate.

An initial x64 full run passed 49/50 but its package probe timed out at stage 7
(CAP to monitor after reaching DOS). A direct rerun, the subsequent complete
x64 run (50/50, 56.22 s), and five consecutive package runs (26.38 s) passed.
No root cause was established; it is explicitly tracked in TODO, not claimed
fixed by these four repairs. Do not weaken that acceptance assertion or alter
unrelated product lifecycle on this evidence. Strict lib built successfully and
passed 3/3. Final x86 passed 50/50 (72.10 s); its package probe passed too.
The committed-diff coordinator review follows executor P1 delivery.

Sweep: rg over all keyboard submits, translation calls, character_key, input A/W
vocabulary and native repeat handling; every production hit is accounted above.
Production C/H: 12 paths, +144/-105, net +39. Test C: six paths,
+172/-34, net +138. Three CMake/gate paths: +18/-1. Counts use
git diff --numstat against `5be6fc4`, excluding docs, manifest and EXEs.

Package SHA256:
- softpc32.exe: `958BD3E15CCDFCCF93809D2386A42C646641A1799E530B57AA4DC28CFAAEDA1F`
- softpc64.exe: `DB4EAA31430C359BAEEF90E2DB24EF77FCE6097E6D7A87D7B74906DC83698CC3`

### S7 P2 coordinator committed-diff review

After executor P1 `e2e3e5c` was pushed, the coordinator reviewed the actual
`5be6fc4..e2e3e5c` production, test, vocabulary, build and documentary changes
(30 paths), not only the executor summary. The explicit shared-ledger borrow,
pre-delivery synthesis snapshot, selective native translation and wide record
boundaries implement the four rows without new persistent state or an app ABI.
Tests removed artificial physical WM_CHAR injection because the producer now
does not translate those keys; the replacement explicitly asserts no translation
and verifies unmapped-key translation rather than relaxing output counts.
Existing source retirement, capture/freeze, FIFO, geometry and product lifecycle
implementations are unchanged. No app/MVDM/INI/media diff exists.

All four bounded repairs have verification evidence; the single unresolved
package timeout remains visible in TODO and in this report, with no inferred
root cause or silent test waiver. Final serial x64/x86 full runs passed 50/50;
strict build/3-test gates and governance passed. Five consecutive x64 package
runs passed. Fixed build caches remain; this task's two temporary layout-fixture
directories were removed after testing. P2 records delivery for owner testing,
not T55 closure or an assertion that every library defect is absent.

## S6 admission: complete key lifetimes and common normalization

Owner: “准入下一个S任务修复以上问题；类似问题也要扫描同样思路处理”.
Original feedback: “有没有系统性的方案解决？我不想添油战术”;
“如何保证ui-console和ui-window能同样处理？…是不是应该统一制作和进入ui-base的字符处理?”;
geometry: “同意！类似问题记得一并处理。” Baseline `2f9d9f1`.
S5 is the delivered baseline, not proof that follow-up findings are absent.

| Frozen sweep | Sole owner/disposition | Proof |
| --- | --- | --- |
| All matcher transitions and leaf callers | ui-base replaces three key collections with one ordered held-key ledger: pending, delivered or consumed until release. Ordinary/modifier keys obey the same lifetime; repeats reuse entries; delivered never becomes consumed. | P-before-modifiers retains break; both sides; mismatch/mouse/repeat/chords/rejection and permutation matrix. |
| All lib key/text submissions | One ui-base copied-record entry: Window marshals separate messages, Console combined records; shared decoding, surrogate and deduplication before existing matcher/sink. | Equal separate/combined output, scan-less RDP recovery, BMP/surrogates, no text on break, malformed/rejected input and actual adapters. |
| All Window rectangle adapters/callers | Neutral arithmetic, conversions only at SDK boundaries; remove calculation-forwarding wrappers, keep actual native operations. | Bounds/aspect/cursor/dirty regression and no conversion round trip. |

Held-key storage grows for distinct simultaneous keys only, not repeats or
released keys. Allocation/delivery failure uses existing terminal failure;
retirement/destroy releases storage. No retry or new component.
A physical transition consumes its corresponding character; only text without
a usable physical representation enters UTF-16. State is per source. Only
attempted rejected delivery triggers sink failure, not unrecognized raw input.

Non-goals: MVDM, app lifecycle, INI/media, Console mouse scale, overlay indexing,
app ABI and Linux UI parity. DAG unchanged. Focused tests, full x64/x86, strict
lib/manifest/governance and peer dispositions precede complete P push and
actual-diff review. T55 remains open.

### S6 executor verification and peer dispositions

Implementation retains the existing registry and component failure/frozen
filter; one ordered dynamically sized ledger replaces three partial key lists.
All held keys use pending/delivered/consumed states. Repeat adds no entry;
release removes it in stable order; partial delivery and allocation failure
cannot retry the stream. Retirement clears storage and destroy also covers
workerless cleanup. types gained only header-only realloc/memmove/SIZE_MAX
vocabulary, not behavior or an OS dependency.

Every production leaf keyboard path now calls ui_keyboard_submit_record:
Window transition and WM_CHAR, and Console's combined record callback.
Low-level transition/UTF-16 operations remain in ui-base for implementation and
focused diagnostics; three old recovery/deduplication support entry points are
removed. The static source gate rejects low-level bypasses from either leaf,
including split-line calls; negative/positive probes cover both consumers.
Window's established translated-character scan convention is preserved, so
queued physical A/B followed by their characters does not duplicate input.
Independent character records use zero scan; raw Console physical records
consume their attached character, while text-only press records use shared
UTF-16. Text-only releases produce no duplicate. No text interpretation is
added to either leaf.

Rectangle sweep: removed display/map-dirty/fit-outer/fit-client forwarding
wrappers and updated all callers. Dirty and cursor paths no longer convert
neutral -> SDK -> neutral. Monitor work-area input converts once; invalidation
and cursor drawing convert once at their SDK boundary. Two rectangle marshalling
helpers and native resize/maximize/sizing operations are retained because they
actually cross the SDK boundary. Arithmetic remains in ui-window root.

Focused proof: 432 combinations (three registered trigger keys, four left/right
modifier choices, six make orders and six break orders), with sixteen trigger
repeats per combination, assert balanced delivered key lifetimes and correct
hotkey counts. Additional tests cover 64 distinct held identities, pending
repeats, both Ctrl sides, mouse interleaving, frozen filtering, allocation
failure and partial replay rejection. Both actual native adapter bodies are
exercised with controlled context lookup (no desktop focus ownership): physical
scan/no-scan input, delayed translated characters, a text-only surrogate pair
and its breaks, malformed input, rejected sink and no later input.

The new test initially exceeded the Windows stack with three large frame
fixtures and omitted the required failure sink; fixtures were corrected before
acceptance. Later assertion indices were corrected after adding the delayed
character case. No production error was hidden by weakening existing tests.

Final fixed-preset builds updated both package EXEs. x64 full CTest passed
50/50 (61.85 s); x86 passed 50/50 (70.05 s). Strict standalone lib built with
-Wall/-Wextra/-Wpedantic/-Werror and passed 3/3. Expanded whitespace-negative
source-gate self-test passed again after the full runs; manifest/governance
are rechecked before delivery. Linux common input/wait contract fakes pass;
Linux desktop execution is not claimed.

Sweep commands: rg over every lib keyboard submit/normalizer/matcher field
and every ui-window rect conversion/caller; actual diff inspection for all
changed paths. Obsolete matcher collections, recovery helpers and four
calculation wrappers have no live callers. Library component edges, external
declaration gate and source-boundary tests pass. No app, standalone host,
MVDM, media, user INI, Console mouse scaling or overlay implementation changed.
No new deferred debt. Fixed build caches are retained; no scratch build tree
or source diagnostic was introduced. T55 remains open for owner inspection.

Accounting against `2f9d9f1` using `git diff --cached --numstat`:
production C/H 10 paths, +212/-254 (net -42); test C 5 paths, +233/-26
(net +207); CMake/gates 3 paths, +21/-1 (net +20). Docs, manifest and EXEs
are excluded. The test growth is the deterministic lifecycle/adapter matrix;
production removes more code than it adds.

Package SHA256:
- softpc32.exe: `5A65F61C2A7AA946A65B3CE45BF33FA0E5167F5DC5824A22EC2A04A9BBB4C016`
- softpc64.exe: `F55BDB732FC2D091EB432B6651CF92E6652C729B1E370989763AD39E2C4F0BCD`

### S6 P2 coordinator actual-diff review

After P1 `5282f42` was pushed, the coordinator role reviewed the committed
`2f9d9f1..5282f42` diff: all 27 paths map to the three findings, necessary
typed C vocabulary, targeted tests/gates, documentary handoff or dual artifacts.
The matcher retains registry matching and failure semantics; it removes the
three-list ownership split rather than adding a second matcher. Destruction
joins before final ledger disposal; retirement's discard leaves a null pointer,
so worker and workerless cleanup are safe. Both native input producers use
one record entry, existing physical-key recovery and shared character handling.
No product action, frame schema, mouse scale or app queue behavior changes.

Removed geometry wrappers have no callers; native marshalling and actual SDK
operations remain private to Window. Static bypass checks are proven with
negative direct and split-line calls in both leaves. Rechecked strict lib
3/3 and governance after push; x86 focused layout/manifest/lifetime 3/3 passed.
Full x64/x86 results and artifact hashes agree with executor evidence.
No unresolved item was identified within this bounded S6 ledger. Delivery is
for owner inspection; neither whole-library perfection nor T55 closure is claimed.

## S5 admission: identities, independent gates and minimal ownership

Owner: “以上，开始清理。” Baseline `183fc8f`; T55 remains open.
Original approvals: use the existing ui-base normalizer, never one per leaf;
restore independent locks; remove storage orphan paths; audit and clean all
library pointer-only wrappers/dead code; move non-platform computation to its
own component root. Earlier approved items include physical-key state, one
Window failure entry and corrected cursor geometry.

Frozen universe: all C/H in types, console, host, storage, ui-base, ui-window,
ui-console and their callers. Per component record removed, made-local or
retained-with-real-responsibility dispositions. Externally useful public API,
one-to-one types vocabulary, selected-platform boundaries and unsupported Linux
UI placeholders are not dead merely because SoftPC does not use them.

| Ledger | Owner and required proof |
| --- | --- |
| Physical keys | Existing ui-base normalizer emits consistent scan/extended; matcher compares that identity. Both Ctrl sides/releases and leaf equality. |
| Chord state | Distinct physical held keys own pending/delivered/consumed states. Repeat cannot overflow pending; delivered makes retain breaks and cannot become consumed retrospectively. No timer or separate matcher. |
| Fault | Every Window post-start fault uses component failure entry immediately; STOP remains FIFO. No duplicate exit status. |
| Mailboxes | Separate frame/control locks; STOP/fault acquire frame then control to close both atomically. Ordinary controls never acquire frame lock. |
| Pure computations | Audit every platform source: extract Window geometry/cursor/scaling arithmetic into ui-window root; retain genuine native operations. Preserve renderer behavior. |
| Storage | Remove orphan internal create modes and localize file-only helpers; keep binary/public modes. |
| All-library ownership | Remove pointer-only event wrappers and duplicate join; inspect remaining allocated structs and internal symbols in every component. No new generic runtime. |

Proof: focused controlled tests before full x64/x86, strict lib, manifest/DAG/
governance checks. Match original request against actual diff after complete
P push. Do not change MVDM, media, INI, Console mouse scale, routing or overlay
algorithm. Each retained platform operation must describe a native responsibility,
not hide shared computation behind a platform name.

### S5 convergence audit

Frozen implementation inventory: 81 C/H files across seven components
(types 13, console 6, host 15, storage 7, ui-base 18, ui-window 17,
ui-console 5). Existing platform source coverage includes every win32/linux C
file, not just the Windows leaf. Operations/allocations and identifier callers
were inspected with `rg` plus a whole-corpus C/H identifier occurrence scan.
Externally exposed APIs were checked against application/test callers before
classifying an internal declaration as orphaned.

| Component | Disposition |
| --- | --- |
| types | Retained header-only C/SDK vocabulary and compiler atomics: definitions, not resource wrappers. A macro without a current app caller is not an obsolete component implementation. |
| console | Kept the logical object's two blocking gates and copied bindings; they enforce different callback/output barriers. Preserve actual mutex creation errors. Platform mutex objects own real OS mutexes. |
| host | Removed event's pointer-only parent allocation, separate thread-start allocation, duplicate platform destroy/join and unused Linux backend dummy struct. Keep task cancellation/entry state and broker/output binding snapshots: they carry real state and binding generation until old output drains. Linux condition deadlines and Windows handle waits remain platform implementation. |
| storage | Removed orphan allocating truncate/append entries and file-only external declarations. Writer binary modes remain public; medium/file handles own streams. Open locking/large-file offsets remain platform-local. Overlay's range check after allocation was unreachable after caller validation and removed; O(n) lookup remains explicitly deferred. |
| ui-base | One normalizer, low scan plus separate extended flag; shared matcher tracks physical pending/delivered/consumed modifiers. Separate locks. Removed test-only single-control forwarding entry; all producers use the existing atomic batch entry. Wake objects retain real OS event/condition state. |
| ui-window | Pure geometry/cursor, frame size/pixel conversion and relative motion moved to root. Worker/context/frame now one allocation. All post-start faults close admission through component failure. Native rectangle marshalling, WMSZ edge anchoring, GDI resources, capture and message cadence remain Windows-owned. |
| ui-console | Pure copied text-frame conversion moved to root; redundant event forwarding wrapper removed. Keep record modifier/button decoding and native cell-coordinate handling in Windows adapter; X8/Y16 scale unchanged. Thread state owns native worker and prior input position. Linux UI placeholder remains explicit unsupported. |

No remaining two-occurrence internal entry was found after this cleanup;
remaining declaration/definition-only corpus matches are supported public APIs
with app/test consumers. Platform function pairs have identical parent contracts.
No second matcher, alternate rendering route, product policy, types C file or
cross-component platform include is introduced. Struct ownership retained above
is intentional, not a transitional forwarding layer.

### S5 P1 executor evidence

Final fixed-preset builds completed for x64 and x86. Full
`ctest --preset test-x64 -j1 --output-on-failure` passed 49/49 (30.66 s);
x86 passed 49/49 (74.41 s). Strict standalone lib built with
`LIBRARY_STRICT_WARNINGS=ON` (-Wall/-Wextra/-Wpedantic/-Werror) and passed
3/3 layout/manifest/Linux-build-contract checks. All 18 public headers compiled
independently with C17 and the same strict warnings. Linux wait/text contract
fakes compiled and passed on both Windows widths; no Linux desktop run claimed.

Focused checks cover equal extended scan identity, simultaneous left/right
Ctrl and all breaks, 100 repeated pending makes, delivered-modifier mismatch,
existing repeated hotkeys, frozen/close filtering, partial sink rejection,
STOP/publish races, a control producer and consumer completing while the frame
lock is deliberately held, nine real hidden Window exit scenarios with immediate
fault-admission assertions, non-integral cursor row bounds, text pixel palette,
dirty accumulation, signed mouse scaling and host allocation/join ownership.
The new host test creates a real worker: event has one platform allocation,
thread startup has no separate allocation, and destroy issues exactly one join.

An initial extracted file lacked its explicit Win32 vocabulary includes and an
added variable violated the neutral naming gate; both were corrected before
final verification. The extended-key test fixture was corrected to clear its
right-Ctrl flag before constructing ordinary left Alt/P events. These were
caught by compilation/tests, not deferred to owner testing.

Accounting: `git diff --numstat 183fc8f -- src test CMakeLists.txt`, restricted
to C/H for code (exclude docs, manifest and EXEs): production 28 paths,
+563/-534 = +29; tests 8 paths, +151/-19 = +132; build files 2 paths,
+3/-2 = +1. Root helper extraction preserves platform behavior rather than
introducing a parallel implementation. Removed ownership and dead paths are
listed in the component ledger. MVDM, INI and guest-media diff is empty.

Package SHA256:
- softpc32.exe: `680EE4F32B1851B2BB68A6555DF3FB68C61E7E988422C11AC89A5216939BD506`
- softpc64.exe: `AF91CB4A86EBAF9658FD7DA83F341304C235FAA958684AB93F0E2281813B489E`

No task-specific scratch build tree was created; fixed x86/x64 trees and the
existing strict-lib configuration remain available for owner inspection.
T55 remains open. This executor delivery requires post-push actual-diff review.

### S5 P2 coordinator review

Reviewed actual committed diff `183fc8f..d6d801b` after executor push, not
only its report: all 47 changed paths map to S5 implementation, tests,
architecture/packet/ledger, prior S4 handoff, manifest or two package artifacts.
No app/MVDM/INI/media path changed. The normalizer and matcher remain single
ui-base paths; scan/extended reach the unchanged app binding correctly.
STOP acquires frame then control, with no reverse acquisition; fault closes
the same gates before worker cleanup. Normal STOP remains ordered FIFO.
Window frame/context storage is kept until worker join; native resources are
released once by the worker. Host cancellation remains root-owned and platform
thread disposal no longer joins. Geometry/render/motion are extracted existing
operations, not alternate implementations; retained SDK marshalling has no
resource allocation or duplicate arithmetic.

Verified the finite component dispositions and removed-symbol call sites;
rechecked neutral boundary/DAG and corpus manifest after push. Confirmed
49/49 at both widths, strict 3/3, independent 18-header proof and both artifact
hashes. No remaining blocker was identified in this admitted S5 scope.
S5 is delivered for owner inspection; T55 is explicitly not closed. The broader
T-level audit and owner acceptance are not inferred from these regression results.

## S4 admission: single input and lifetime paths

Owner: “按照这些准入新的S任务修复以上7条反馈意见。” Baseline
`5ea133c`. This admits S4, not T55 closure. Freeze these seven dispositions:

| Item | Sole owner / implementation | Proof |
| --- | --- | --- |
| 1 | Window uses its existing final normalized delivery filter for every event; remove duplicate sink filtering, retain local capture/blink guards. | Frozen Ctrl then X sends only close; hotkeys still work. |
| 2 | ui-base matcher changes keyboard prefixes only for key/text sequence; mouse/X pass without flushing. Keyboard order is retained, keyboard/mouse interleaving is not buffered. | Ctrl/mouse/Alt/P complete suppression; mismatch/release replay. |
| 3 | Partial sink failure clears pending state, closes input/admission, wakes worker and uses its single retirement path; no retry. | Rejected second replay emits first make once, no later ordinary events, fault/retirement once. |
| 4 | STOP and fault share mailbox admission closure; frame and ordinary control reject after closure, STOP idempotent. Accepted controls drain FIFO up to STOP. | Concurrent admission barrier, STOP ordering, failed worker rejection. |
| 5 | console event/output gates and host broker transaction use component-private blocking locks. Keep lock order and quiescence; callbacks cannot reenter binding/destruction. | Real contenders block behind output/callback/replace barriers; native backend output critical section unchanged. |
| 6 | Storage embeds stream state in the writer/file owner instead of separate pointer-only allocations. Public API and platform-specific open/lock/seek behavior remain. | Binary writer/read/medium tests, allocation/cleanup review. |
| 7 | Add TODO at linear overlay page lookup; defer algorithm by owner decision. | Only comment changes in medium, matching deferred debt entry. |

No new public lock API, dependency edge, app policy, mouse scale, MVDM/media/INI
change. Same-shape blocking helpers live inside console/host platform owners;
types supplies external declarations only. Full x64/x86, strict library,
manifest, DAG and documentation checks precede complete P push. Review actual
diff after push; deliver packages and leave T55 open.

### S4 P1 executor verification

All seven dispositions are implemented. The seventh is deliberately only an
O(n) lookup TODO and its long-term ledger entry, not an algorithm change.
The Window's old ordinary/lifecycle delivery wrappers are removed; the existing
final normalized sink is the sole frozen event filter. Local capture and blink
guards remain because they control native behavior rather than sink delivery.
Source retirement remains the base worker's terminal event after quiescence.

The matcher preserves keyboard ordering while mouse/close bypass pending-key
replay. A rejected replay clears its pending state and fails the component;
no later ordinary event can retry the partially delivered stream. Worker
retirement reports the stored failure and attempts SOURCE_RETIRED once.
STOP closes both mailbox admission paths under the same short lock without
skipping controls accepted before STOP. Fault closure uses that same boundary.

Console output and callback gates use private blocking mutexes; the host
transaction lock uses its backend critical section. The short field/mailbox
locks remain spin locks because they do not cover native I/O or callbacks.
Lock scopes and replacement cleanup ordering are preserved. Callback reentry
into binding/destruction remains forbidden. Linux logical Console mutexes use
pthread; the unsupported Linux host Console backend stays explicitly unsupported.
No Linux desktop execution is claimed.

Storage writer now embeds the stream-bearing file; medium file owns one stream
object. Separate file-platform and writer-file allocations are removed. Binary
open modes, platform locking/seeking and public writer/file ABI are unchanged.

Focused proof includes frozen Ctrl/close/release, mouse-interleaved CAP and all
breaks, rejected second replay with no duplicate make, actual Window worker
failure/retirement, FIFO controls before STOP, concurrent publish/STOP, and
proven contending output/callback/replace threads held behind completion barriers.
No Sleep-based scheduling assertion is used. Existing binary writer and media
tests cover the flattened storage path.

Fresh final tests: x64 48/48 (28.34 s), x86 48/48 (71.33 s), strict standalone
library 3/3. Both package EXEs rebuilt. Strict library build uses C11 with
`-Wall -Wextra -Wpedantic -Werror`; 18 public headers passed independent C17
compile checks. Manifest, exact DAG, Linux build contract and documentation
gates pass. An initial documentation test exposed missing S3 history; the
truthful S3 handoff was added, then both complete suites were rerun.

Accounting from `git diff --cached --numstat --no-renames` against `5ea133c`:
22 production C/H paths +235/-173 (net +62); five test C paths +263/-13
(net +250); four CMake/gate paths +9/-3 (net +6). Documentation, manifest and
two EXEs are excluded from those code totals. No app, standalone host, MVDM,
guest media, INI or Console mouse conversion changed. No owned temporary
diagnostic directory remains; reusable build caches are retained.

Similar-issue sweep used `rg` over Window `input_sink`/emit/accepting paths,
matcher pending replay, component stopping/failure/retire and mailbox admission;
Console event/output atomic gates and host transaction entry/exit; and storage
allocate/release/open/close paths. Each hit belongs to the seven dispositions
above: one final Window sink, one worker failure path, one admission boundary,
three replaced long-held gates, and one stream owner. No extra repair is
silently added. Overlay's only medium diff is its approved TODO.

### S4 P2 coordinator review

After P1 `fe620cb` was pushed, the coordinator role inspected the actual
`git diff 5ea133c..fe620cb` against this packet, including the event/filter,
matcher, admission, worker retirement, Console/host lock scopes, storage
ownership, selected-platform implementations, regression tests and CMake DAG.
The seven dispositions match the approved scope. Previously separate Window
delivery wrappers and storage allocations are removed; retained short locks,
capture/blink guards and source-retirement path have the distinct purposes
recorded above. No product routing or input ABI change was introduced.

P1's x64/x86 48/48 results and strict 3/3 checks are accepted as automated
evidence for this bounded repair, not Linux desktop parity or a whole-library
qualification. Both EXEs are tracked in P1. This P2 changes only review/status
documentation; documentation checks are rerun. S4 awaits owner package
inspection and T55 remains open. The O(n) overlay debt remains explicitly
deferred rather than being reported as a performance fix.

## S3 P5 admission: correctness and boundary convergence

Original owner requests: “以上。写入设计/任务文档，然后开始清理”,
“我有一些疑问的你先解答一下，然后可以执行/”, and
“照此办理，请你执行。” Baseline `3a71b50`; continue S3; keep T55 open.
Freeze these twelve approved classes and all callers of their changed contracts.

| Item | Change / sole owner | Focused proof |
| --- | --- | --- |
| 1 | ui-base mailbox accumulates unconsumed dirty bounds with latest complete pixels; incompatible mode/size/palette forces full refresh. | Skipped frames, consume reset, incompatible images. |
| 2 | Window worker has one post-start cleanup/retirement path, separate from startup failure. | STOP, render/wait failure, disappearance; retire once and join. |
| 3 | Console unexpected I/O failures follow logical events and UI failure sink; monitor forwards to control for fault termination, not prompt rearm. Cancellation/NOT_CURRENT are expected. | Reader/output failure and expected handoff; no product decision in lib. |
| 4 | Broker transaction lock includes old binding cleanup after backend output unlock. | A-to-B cleanup completes before B-to-A can enter; no lock inversion. |
| 5 | Window retains integer scaling remainders, reset on capture/release/ratio change. | Positive/negative/alternating/resize; integer ABI and Console X8/Y16 unchanged. |
| 6 | ui-base delivers valid Unicode scalars through TEXT and clears malformed surrogate state. | Supplementary, malformed/recovery, rejected sink. |
| 7 | Window-only message/key-state decoding returns to Window; shared normalizer stays ui-base with same-shape selected-platform API. | All callers, RDP/scan/chord regression, explicit Linux unsupported cases. |
| 8 | Actual consumers declare OS linkage; types retains declarations only. | Direct link contracts; storage does not inherit UI libraries. |
| 9 | storage root shares identical CRT stream operations; platform open/lock/seek differences remain selected implementations. | Storage smoke and both implementation shapes. |
| 10 | Remove duplicate Window component header. | Include sweep and independent headers. |
| 11 | Delete unused suppression field, empty discard-prepare and obsolete event/sink aliases after migrating callers. | Zero obsolete references and regression. |
| 12 | Enforce exact permitted include and CMake component DAG. | Every allowed/forbidden edge and platform/traversal exceptions. |

Execution order: shared frame/input fixes; worker and Console transactions;
platform/storage consolidation and dependency gates; full diff review and fresh
strict/x64/x86 build/tests. Every disposition needs focused evidence before
delivery; no silent deferral. Existing Linux paths remain connected, unavailable
UI parity stays explicit; no claim of Linux desktop execution. No MVDM, media,
INI, Console mouse conversion or product routing change. App changes only
propagate faults and adopt renamed contracts. Refresh only the two package EXEs;
commit/push complete delivery, account changed paths and leave T55 open. Ignored
build/t55-lib-audit probes are bounded diagnostics to remove after permanent
tests replace them.

### P5 executor verification

All twelve ledger items are implemented without deferral. Shared damage uses
latest complete pixels plus the enclosing pending rectangle, never an extra
pixel queue. Window mouse uses signed integer remainders, not floating-point
events; Console X8/Y16 is unchanged. The neutral keyboard path preserves the
existing Windows layout/scan recovery and hotkey sequence; Linux terminal key
mapping remains connected and text without a physical mapping uses TEXT.
Window-only message flags/key-state queries are now private Window helpers.

Worker proofs execute the actual production worker with controlled barriers:
STOP, wake failure, surface allocation failure, WM_QUIT, unexpected destruction,
retirement rejection and fault plus retirement rejection. Retirement is once,
exit failure is reported once, and destroy joins. Console tests cover reader
failure, output failure, expected NOT_CURRENT and callback/detach ordering.
The host reader reports generation-checked IO_FAILURE; ui-console retires via
its failure path. Monitor posts a copied control failure and the app terminates
the failed path without arming another prompt. Failure cleanup now destroys
runtime before monitor/control queue, keeping callback targets alive until join.
This is failure plumbing, not a lifecycle/routing policy change.

The broker test pauses A-to-B during old sink cleanup and proves the reverse
thread actually encounters the held transaction lock; only after cleanup can
B-to-A install A again. Its final output write succeeds. Static gates test all
49 include and 49 CMake edges, uppercase commands, computed/angle includes,
component-private platforms and traversal. OS libraries are attached to actual
consumers, not types. Repeated CRT stream bodies, obsolete event/sink aliases,
unused suppression state, empty prepare rollback and duplicate Window header
are removed, with every production caller migrated in this P.

Fresh full tests: x64 46/46 (53.10 s), x86 46/46 (69.26 s), including package
smoke. A subsequent test-only atomic cleanup of the broker probe passed again
at both widths. Strict C11 `-Wall -Wextra -Wpedantic -Werror` library build and
3/3 standalone checks pass; all 18 public/support interface headers independently
compile as strict C17. Manifest and documentation gates pass. Linux input/host contract fakes
are not evidence of Linux desktop execution; manual package acceptance is pending.

During verification, strict compilation caught a neutral key narrowed to u16
and a missing scalar vocabulary include; both were corrected before delivery.
The first focused run passed 7/9: the new Console IO_FAILURE kind was missing
from event validation. Adding its validation case made both tests pass, followed
by the full passes above. No failed verification is treated as acceptance.

Similar-issue sweep: inspect all worker STOP/fault/destruction exits, all Console
read/write sinks, both broker terminal failures and success cleanup; search
`ui_event`, `ui_event_sink`, `suppressed_virtual_key`, `discard_prepare` and old
`ui_win32_keyboard` names across src/test (zero obsolete references). Review all
changed input callers and selected platform operations; exact DAG negative
fixtures permanently guard the mechanically detectable boundary errors.

Accounting versus `3a71b50`, no rename detection, excluding documents/manifest/
EXEs: library C/H 30 paths +465/-438 (net +27); app 11 paths +43/-27 (net +16);
tests 14 paths +470/-49 (net +421); build/gates five paths +94/-14 (net +80).
No MVDM, media or user INI changes. The two package EXEs are refreshed.
S3 awaits owner inspection; this delivery does not close T55.

### P6 post-push review

Reviewer inspected the actual `3a71b50..996e4c6` changed paths and critical
worker, broker, input, mailbox and app failure diffs against the twelve-item
ledger, then checked the verification evidence and protected-path diff.
No unresolved delivery blocker was found. The generated standalone flags are
C11 with all four strict warning switches; the independent header probe used
C17. This evidence correction changes documentation only. Both package EXEs
belong to P5, and no MVDM/media/INI path changed. Owner acceptance remains
pending; neither S3 nor T55 is closed by this review.

## S3 P4 admission: component-private platform directories

Original owner request: “很好 所以所有的lib组件 win32和linux都只是组件内部的实现，组件外不能访问 包括隔壁其他的lib组件 对吗”,
then “没错 你说的完全正确 请开始实现”. Baseline `a6bf81f`.
Continue S3, keep T55 open. The frozen universe is all library C/H include
edges and all consumers of the two current Windows input/action headers.

- Move their actual copied-value declarations to ui-base root support
  interfaces with git mv; update every caller. Keep implementation, symbols,
  state and behavior unchanged, without forwarding headers or a second path.
- Platform-specific helper contracts remain explicitly Windows-specific; they
  are not the same-shape operations called by platform-neutral parent sources.
  Do not invent Linux implementations for Windows record decoding. Existing
  Linux mappings and the 41 same-shape parent operations remain unchanged.
- Enforce root-only cross-component interfaces and forbid importing platform
  implementation headers from parent sources or other components. Only types
  platform vocabulary is shared, from the matching platform implementation.
- Add positive and negative boundary probes, including a misleading interface
  suffix, root forwarding, sibling platform access and types exceptions.
- Refresh current design/READMEs and manifest; build both widths, run full
  regression, strict standalone tests and review the exact body-preserving diff.

No new API behavior, product policy, Linux parity, MVDM, media or INI changes.
The owner-provided command.c formatting is preserved unchanged and ships in
this P under the owner's additional instruction “请一起提交 不留尾巴”.
Completion requires all five dispositions, a complete pushed P, and package
links for owner inspection; it does not close T55.

### P4 verification

All five dispositions are implemented. The only library C-body differences
from `a6bf81f` are include substitutions in four files; exact comparisons after
those substitutions pass. Two declarations moved with git mv; guards and a
contract comment changed, not symbols, struct layout or signatures. Existing
Linux code, types, parent operation contracts and CMake selection are unchanged.
The complete C/H include sweep has no remaining cross-component platform
include. Tests deliberately retain forbidden path strings as negative probes.

The gate now requires canonical paths, rejects public headers in platform
directories, root forwarding, cross-component platform includes even with an
interface suffix, opposite-platform includes and relative traversal. Positive
controls cover root support access, own-platform helpers and matching types
vocabulary. Application production is separately barred from leaf-support APIs.
The two relocated headers independently pass C17 strict syntax compilation.

Both package EXEs were rebuilt. Full x64: 42/42 in 21.47 s; full x86: 42/42
in 27.34 s, serial including package smoke. Strict library build and 3/3
standalone checks, manifest, documentation governance and diff checks pass.
These are Windows tests, not a claim of Linux desktop parity.

Accounting (`git diff --numstat --no-renames a6bf81f`, excluding documentation,
manifest and EXEs): library C/H eight paths +59/-55, net +4 (the contract
comment); tests/support four paths +34/-5, net +29; library static gate one
path +18/-1, net +17. Separately, owner-provided command.c formatting is one
path +186/-61, net +125, included unchanged by explicit owner request.
No new compiled code, runtime layer, API route or platform behavior was added.
S3 remains for owner inspection; T55 is not closed.

## S3 P3 admission: nine-item audit repair

Owner request: “开始”, approving the preceding whole-library audit. Baseline
`d236757`. Continue S3; do not close T55. Freeze these nine dispositions before
implementation; completion requires each focused proof plus dual-width full
regression, strict build, manifest, actual diff review and pushed clean delivery.

| Finding | Repair and proof |
| --- | --- |
| UINT64_MAX bypass | types alias, both consumers migrated, negative gate probe. |
| Alias-derived gate blind spot | Independent finite external vocabulary patterns and unwrapped negative probes. |
| Window-only capture abstraction | Store its boolean in Window mouse state; delete unused shared abstraction, preserve capture behavior. |
| Unused UI color helper | Remove helper and build references after full caller sweep. |
| Shared header names | Cross-component contracts use `_interface.h`; own implementation headers remain unqualified. Enforce direct includes. |
| Repeated chord leaks releases | Preserve outstanding suppressed keys, suppress repeats, bound capacity; deterministic repeated-chord and mismatch tests. |
| Console wake fault skips retirement | One detach/retire exit for STOP and wake failure; failure notification, callback barrier tests. |
| Linux waits poll/sleep | Real condition waits, monotonic deadlines, safe initialization unwinding; deterministic fake-platform proof. |
| Missing Linux cooked-line operation | Same-signature UNSUPPORTED implementation and link proof. |

No new component or types implementation, product callback, MVDM/media/INI
change, or mouse scaling change. Linux existing behavior is repaired, not
expanded into UI parity. Fake-platform evidence must not be presented as a
real Linux desktop/runtime test. Relevant Linux debt is now admitted here.

Verification discovery: the release-build assertion list omitted the existing
leaf-control-capacity and control-reconciler-integration tests. Add both and the
new Linux test to that list, so full regression executes their operations
instead of compiling out assertions. This is test proof repair, not a product
state-machine change. The Console startup review also found that starting the
worker before installing the input sink could reattach it after an immediate
worker fault; install it first and detach on thread-create failure.
The shared-mailbox review found its enqueue error return rereading
`stop_queued` after unlocking. Snapshot the result under the existing lock;
capacity/STOP semantics are unchanged and covered by the enabled tests.
The x86 repeated full run exposed a stale negative-probe file in the types
layout self-test: its initial positive control read `UI_WIN32_KEY_CONTROL`
from a prior probe. Explicitly remove the three owned probe files at entry;
the production gate must still reject every deliberate negative probe.

### P3 executor verification

All nine dispositions are implemented. The resulting library contains 71 C/H
files, including 41 headers. No new production file or execution layer was
added: six contracts were renamed with `git mv`, and Window-only capture plus
unused UI color helpers were removed. Types remains header-only. The mouse
capture field substitution preserves the exact Win32 calls/order and no raw
Console coordinate conversion changed. No app, standalone host, MVDM, INI or
media file changed.

Evidence by ledger member:

1. `LIB_UINT64_MAX` owns the external constant; both production uses migrated.
2. The layout gate adds independent integer/SDK/POSIX/CRT families and
   cross-component include validation. Negative tests cover unwrapped tokens
   and calls; both widths passed three consecutive self-test runs. It remains
   a finite static check, supplemented by the whole C/H call/include sweep.
3. `ui_win32_mouse` alone owns the boolean capture field; the old shared
   capture header has no remaining production caller.
4. `ui_win32_colorref_from_rgb` and its CMake entry are gone; the separate
   host Console color path remains untouched.
5. Shared binding, worker, mailbox, wake and Windows input/action headers now
   have `_interface.h` names; application access to these leaf-support
   contracts is rejected by the source-boundary gate.
6. The repeated Ctrl/Alt/P regression failed against the baseline matcher,
   then passed with preserved suppression state. Tests include auto-repeat,
   trigger re-press, modifier repeats, final breaks, mismatch replay, and
   capacity rejection without overwriting existing entries.
7. The actual Console worker runs under real Windows threads with a controlled
   wake result. Both STOP and fault wait for an in-flight callback, retire once,
   join before destroy returns, and reject later input on a retained logical
   Console. Fault reports IO_ERROR. No new lifecycle callback was introduced.
8. Actual Linux host/mailbox sources run against deterministic POSIX fakes:
   all initialization failures unwind, spurious wakes reuse one monotonic
   deadline, infinite waits wake, event reset semantics hold, wait/clock errors
   report failure, and interrupted sleep uses the remaining interval. The wait
   paths make zero sleep calls. Platform sources also pass strict syntax checks
   against the available Windows pthread headers; this is not Linux execution.
9. The Linux cooked-line placeholder is linked and returns UNSUPPORTED with
   the same signature as its Windows implementation.

Final regression: x64 42/42 (17.02 s), x86 42/42 (20.33 s), serial and including
both fixed-package smokes. Strict library build and standalone 3/3 pass; all
41 headers independently pass C17 Wall/Wextra/Wpedantic/Werror on the available
compiler. Documentation governance, manifest and diff checks pass. Earlier
x86 runs failed only the stale-fixture self-test (41/42); that failed evidence
is retained here, not replaced by a claim that every run passed. Linux desktop
execution/parity remains explicitly unverified and outside this delivery.

Changed-path accounting uses `git diff --numstat --no-renames d236757`:
production C/H 37 paths, +369/-366, net +3; tests/support 12 paths,
+281/-13, net +268; build/static-check 3 paths, +28/-3, net +25.
Documentation, manifest and EXEs are excluded. The increase is predominantly
tests, not new abstraction. T55 remains open and S3 awaits owner inspection.

## S3 follow-up admission and convergence ledger

Original request: “准入使用当前或者新的S任务修复以上所有问题，完成后工作区清理干净并让我检查。”
Continue S3 from `2f54899`; do not close T55. The frozen universe is every
library C/H external vocabulary consumer, plus the nine audit classes below.
Each requires implementation, direct-call sweep and focused/full proof before
delivery. Types wraps original external definitions even for a single consumer;
component-defined behavior and contracts stay in their owning component.

| Audit class | Required disposition |
| --- | --- |
| Include-only types shells | Replace with actually consumed external aliases; no duplicate SDK definitions. |
| Windows host and UI SDK use | Adopt types vocabulary without moving/reimplementing worker behavior. |
| Linux pthread/time use | Adopt existing wrappers and fill missing ones; retain platform ownership. |
| Storage CRT/OS use | Wrap original file definitions in types; storage retains file policy. |
| Copied UI Windows key constants | SDK-backed definitions in types; neutral mapping stays ui-base. |
| VkKeyScan modifier mismatch | Translate raw SHIFT=1/CTRL=2/ALT=4 to UI masks; deterministic tests. |
| Console raw numeric contract | Document raw platform values at the Console boundary; UI alone normalizes. |
| Root policy leakage | Move NOT_CURRENT to Console; make text length direct CRT and audit null callers. |
| Weak static proof | Reject raw external tokens outside types, with negative tests, not only includes. |

No new types implementation files, component edges, product policy, MVDM or
media changes. Refresh both EXEs, test both widths, review the actual diff,
commit/push all changes and leave a clean worktree for owner inspection.

### S3 P2 result

All nine audit classes above are addressed. The complete library C/H universe
is 74 files (73 at P2 entry plus the shared Windows scalar declaration header).
Twenty-one non-types C/H consumers changed; component sources retain their
original operations and sequencing. Windows/POSIX/CRT types, calls and constants
now use types-owned aliases. UI virtual keys bind SDK values rather than a
second numeric table. Types remains header-only/INTERFACE; no component DAG
or SoftPC product path changes.

The only executable semantic correction is the UI-owned conversion of
VkKeyScan's high-byte SHIFT=1, CONTROL=2, ALT=4 into the existing UI masks.
The deterministic test compiles the actual input implementation with a fake
layout query and covers all eight combinations, emitted make/break symmetry,
and failed-query output preservation. Existing hotkey/input tests still pass.
Console's raw platform key/button/cell-position contract is documented without
moving event types or UI normalization to types/host. NOT_CURRENT retains value
6 but is declared by Console. Both production text-length callers already
reject null before calling; the root wrapper now matches strlen exactly.

The static gate derives raw tokens from external aliases and rejects their use
outside types. Eight new negative probes cover SDK/CRT/POSIX types and calls,
SDK key constants and the removed UI key table; all original probes remain.
Manual call-site and type sweeps supplement this finite-token gate: it is not
a claim that a regex understands arbitrary future C code. No speculative unused
wrappers were retained.

Verification: x64 41/41 (42.53 seconds), x86 41/41 (58.38 seconds), each serial
and including fixed-package smoke; strict library build and 3/3 standalone
checks; ten common/Windows headers independently compile with C17 and
Wall/Wextra/Wpedantic/Werror. Linux platform aliases are source-reviewed but
not executed on Linux. Existing Linux wait-policy debt described below is not
claimed repaired by this vocabulary migration. Documentation and diff gates
pass. Both package EXEs are rebuilt. T55 remains open for owner review.

After removing unused new aliases, both widths were rebuilt and the full suites
repeated: x64 41/41 in 42.47 seconds, x86 41/41 in 53.60 seconds; strict library
3/3 repeated. The pre-existing Linux execution gaps are explicitly retained in
[TODO](../states/TODO.md), not silently certified by these Windows results.

P2 changed-path accounting (`git diff --numstat --no-renames 2f54899`, including
the added scalar header): production C/H 32 paths, +1038/-684, net +354;
tests 3 paths, +59/-9, net +50; static gate 1 path, +33/-0. Documentation,
manifest and package binaries are excluded. Added declarations replace direct
SDK usage, not parallel production execution. The original component workers,
storage operations, Console broker and UI normalizer remain the sole owners.

## Objective

Make `lib/types` the header-only shared vocabulary for C-runtime, SDK, POSIX,
and compiler-atomic declarations. Preserve every observable library behavior
while moving all compiled platform behavior into the component that owns its
meaning.

`types` is not a runtime component. It has no `.c` files, worker, handle
ownership, state machine, product policy, or component dependency. Its typed
inline façades are one-to-one external vocabulary only.

## Final boundary

Each component has a platform-neutral base source and a selected platform
source with one identical component-private operation shape:

```text
host/clock.c       -> host_clock_platform_counter(...)
host/sync.c        -> host_sync_platform_*(...)
storage/file.c     -> storage_file_platform_*(...)
ui-base/mailbox.c  -> ui_mailbox_wake_*(...)
```

The `win32` and `linux` implementations of each shape are peers. CMake selects
exactly one. A base source does not use platform preprocessor branches, raw SDK
types, or platform function declarations. A platform source may implement only
its owning component's internal shape; it cannot introduce a cross-component
runtime, dependency, or product policy.

`types` centralizes the external header and typed wrapper vocabulary used by
those sources. It does not normalize UI keys, define a storage file object,
own events/tasks, or implement Console/Window behavior. In particular:

- `storage` owns file access, exact-transfer and ownership policy;
- `host` owns synchronization, task/cancellation, clock and Console policy;
- `ui-base` owns event normalization, hotkey matching and mailbox meaning;
- `ui-window` and `ui-console` own their lifecycle and rendering behavior.

## Required changes

- Delete every `src/lib/types/**/*.c`; make CMake target `types` INTERFACE.
- Move compiled raw file work to `storage/win32/file.c` and
  `storage/linux/file.c`, behind `storage_file_platform_*`.
- Move compiled synchronization/task work to `host/win32/sync.c` and
  `host/linux/sync.c`, behind `host_sync_platform_*`.
- Keep mailbox wake work in the existing `ui-base/win32` and `ui-base/linux`
  sources, with the identical `ui_mailbox_wake_*` contract.
- Keep platform clock work in `host/win32/clock.c` and
  `host/linux/clock.c` behind `host_clock_platform_counter`.
- Move the Win32-only modifier query to `ui-base/win32/actions.c`; do not
  retain a generic UI source which includes a platform adapter.
- Move text-to-native-key layout interpretation into `ui-base/win32/input.c`.
  `types` exposes only raw SDK facts, never UI input mapping policy.
- Centralize external SDK/CRT/POSIX declaration headers under `types`; the
  component source consumes that vocabulary and exposes no native type through
  any public `*_interface.h`.

## Invariants

- Implementation filenames and identifiers describe their operation, without
  `native`, `internal`, or `private` qualifiers. Platform vocabulary lives in
  `types/win32/` or `types/linux/`; `host/sync_interface.h` is the public sync
  contract and `host/sync.h` declares the selected platform operations.
  UI worker start/join/state names describe actual lifecycle responsibilities.
  This naming pass changes no control flow or synchronization behavior.

- Public interfaces expose only `lib_*` copied values and opaque component
  objects; no `FILE`, `HANDLE`, `HWND`, `DWORD`, or `pthread_*` value leaks.
- The component DAG remains:
  `types -> console + host + storage + ui-base + ui-window + ui-console`,
  `console -> host + ui-console`, and `ui-base -> ui-window + ui-console`.
- No MVDM source, guest media, `softpc.ini`, or SoftPC product behavior is
  changed.
- Existing x86/x64 observable behavior is retained.

## Verification

- Static gate proves `types` has no `.c` and CMake declares it as an INTERFACE
  target.
- Static audit proves every neutral base delegates platform work through its
  component-private same-shape operation, rather than a platform `#ifdef`.
- Public interface audit rejects native SDK/POSIX types.
- Fresh strict library, x86/x64 full CTest, package smoke, manifest and
  governance checks pass after the complete migration.

## P16 naming audit

The owner requested removal of ambiguous `native`, `internal`, and `private`
names. The bounded universe is library filenames and C identifiers, together
with their test references. Both remaining prefixed headers were renamed;
all matching identifier qualifiers now name their actual operation or value.
Comments describing OS behavior are prose, not another API layer. No control
flow changed. The source-boundary gate rejects recurrence in paths and code
identifiers, excluding comments. x64 and x86 full CTest each passed 38/38;
the strict library build and its two checks passed. Both package EXEs were
rebuilt. T55 S2 remains active pending owner acceptance.

## P17 file declaration header naming

Rename the sole `*_runtime` header, `types/win32_runtime.h`, to
`types/win32/file.h` and update its only consumer, `storage/win32/file.c`.
This is a declaration-header relocation, not a completed typed-wrapper
migration: the external declarations remain unchanged and file implementation
stays in storage. No runtime layer is introduced. T55 and S2 remain active
by explicit owner direction.

Verification: no `*_runtime` path or old header reference remains in the
library; x64 and x86 rebuilt and each passed 38/38 CTest, including package
smoke. Strict library build, 2/2 standalone checks, documentation governance
and diff checks passed. Production changes are limited to the header move,
include guard and its one include site; no executable logic changed.

## S3 admission: types platform layout

Original owner request: “同意，按照这个准入一个S任务清理lib types，要求符合收口标准后供我检查。”
Baseline: `1b368d4`, plus the owner's formatting-only atomic.h changes.
S3 supersedes S2's unfinished layout/ownership cleanup; it does not certify
the broader S2 external-vocabulary migration or close T55.

The frozen source universe is all seven baseline types headers and every
direct consumer. Disposition requires both a source review and an executable
layout gate, plus focused tests where function bodies move.

| Baseline header | Disposition / owner |
| --- | --- |
| types_interface.h | Retain common scalar/status and C-runtime wrappers; no OS selection. |
| atomic.h | Retain compiler-only MSVC/C atomics selection and owner formatting. |
| win32.h | Remove umbrella; explicit purpose headers under types/win32. |
| posix.h | Remove umbrella; explicit purpose headers under types/linux. |
| win32/file.h | Keep Windows file declarations; common stdio declarations move to types/file.h. |
| clock.h | Move combined counter validation/conversion to host/{win32,linux}/clock.c; only raw clock declarations/wrappers in platform types headers. |
| input.h | Move modifier interpretation to ui-base/win32/actions.c; direct key-state/layout wrappers in types/win32/input.h; remove unused Linux zero-result fallback. |
| README.md (documentation companion, not a header) | Describe the one platform layout and compiler exception. |

Only actually consumed purpose headers are created. No new platform runtime,
implementation C file in types, duplicated registry, state owner or generic
platform dispatcher is allowed. CMake continues selecting the owning
component's same-shape platform implementation. Platform headers themselves
do not need OS-selection branches. Windows SDK declarations keep their exact
external signatures; this task does not manufacture a second OS API or claim
that header organization alone completes S2's entire wrapper audit.

Verification: layout/forbidden-include checks, header-only and DAG gates,
counter/atomic smoke, existing keyboard coverage, strict library compilation,
x86/x64 full CTest and package smoke, manifest and documentation governance.
Existing Linux function bodies are preserved (clock is relocated verbatim).
Linux execution coverage must be reported separately from Windows results.
No MVDM, app policy, INI or media change. S3 is delivered for owner inspection;
T55 remains open.

## S3 P1 verification and review

All seven baseline header dispositions are implemented. The resulting types
tree has 12 headers: three common, six Windows and three Linux. Every old
umbrella/dispatcher reference is removed from production and tests. The
layout gate rejects OS branching, platform imports from common headers,
non-C common includes, platform control flow and external include bypasses;
its self-test deliberately submits seven invalid forms and valid controls.

Focused proof exercises the actual moved Windows clock/actions bodies with
controlled external queries: null arguments do not query, query failures and
invalid counters preserve outputs, valid counters are copied, all eight
modifier combinations retain their original meaning. Common C/atomic and
real public monotonic-clock calls pass at both widths. All nine common/Windows
types headers compile independently with strict GCC warnings. The existing
keyboard/hotkey tests remain in the full suite.

- x64 full CTest: 41/41 passed.
- x86 simultaneous parallel run: 40/41; package smoke failed at stage 7
  (CAP sent, cooked SoftPC prompt not observed within its existing deadline).
  No source or timeout was changed in response. Isolated package smoke then
  passed three consecutive runs (5.52, 5.35, 5.55 seconds), followed by a full
  serial x86 41/41 pass. The concurrent failure is recorded, not claimed fixed
  or conclusively attributed to scheduling. Owner review remains required.
- Strict library build and standalone CTest: 3/3 passed, including manifest,
  types layout and the Linux CMake contract. The latter is not a Linux runtime
  test. WSL is not installed; no Linux execution claim is made.
- Documentation governance, source boundary and diff checks passed.

Changed-path accounting uses `git diff --numstat --no-renames 1b368d4`:
production C/H is 32 paths, +146/-132 (net +14); test/support is 5 paths,
+156/-3 (net +153); build/check CMake is 3 paths, +85/-0. Documentation,
manifest and package EXEs are excluded. The atomic formatting was present on
entry and retained; only its old BASE include guard was renamed by S3.
storage/file.h gained its missing direct types include, eliminating implicit
include-order dependence without changing its contract.

Reviewer compared clock/error behavior and modifier bits against `1b368d4`;
other Windows and Linux component changes are include substitutions, not
worker/reader/mailbox rewrites. MVDM, app, standalone host, INI and media have
no changes. Ownership remains host clock and ui-base input, with no types
platform control flow. Delivery is for owner inspection, not T55 closure.

The remaining T55 whole-library review is still required. In particular, the
pre-existing Linux mailbox wait currently uses a timed sleep rather than
waiting on its signaled condition; Linux host sync also polls. Those bodies
were not introduced or rewritten by S3. Their behavior and S2's wider raw-call
wrapper coverage are not certified by this layout-only delivery.
