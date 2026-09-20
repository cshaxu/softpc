# T71 S4 Typed Frames And Opaque Latest-Wins

## Request And Scope

Owner: "好的，接下来准入修复，开始执行。" Report additions, removals and net
change separately. This closes S4 of the [approved design](../history/M9-T71-kvm-text-frame-contract-proposal.md),
not T71. Preflight baseline b30cc0da; executor delivery 17c9da90 is pushed.
Inventory-adjusted estimate was production +650/-550, tests +650/-500.

## Actual-Change Review

The coordinator reviewed the pushed diff against the original request, active
packet, governance skills and finite ledger, not merely the executor summary.

| Coverage | Retained owner and proof |
| --- | --- |
| Base frame/control/mailbox | Common text fields only; opaque leaf-provided frame storage and byte lengths, separate generation. No leaf commands/graphics/maps/fonts in transport. Capacity, independent locks, STOP and stale-ack tests pass. |
| Window types/helpers/worker | Own text bitmaps, graphics and dirty merge; active-union copies only. Actual publish-entry damage test covers pending bounds, palette/size/mode changes and late acknowledgement; font-bank and cursor tests pass. Native input/lifecycle unchanged. |
| Console/types/worker | Text base plus two maps, no graphics or bitmap storage. Actual conversion/binding tests prove bank selection, Unicode output, activation/NOT_CURRENT retention and generation acknowledgement. |
| Logical Console/broker | u16 cells copied directly; cache uses the same width. Existing output binding/barriers remain unchanged. Full-write/cooked-raw roundtrip tests pass. |
| Common machine/session/UI | One copied upstream composition; both maps/fonts participate in comparison. UI passes Window data and constructs only the small Console value. No extra buffer owner, queue, executor or worker. Dependency gate rejects machine access to leaf instance APIs. |
| VM producer | Sole CP437 table, all 256 values identical to baseline. Produces both maps and only initializes/copies active payload. Old ABI/API search across src/test C/H has zero hits. |
| Unchanged endpoints | Logical binding/root broker pass the same pointer contract without inspecting cells. Linux workers remain explicitly unsupported. Compat/MVDM, snapshot codecs, INI and media unchanged. |
| Direct test consumers | 28 C/H paths migrated with no compatibility alias. Removed graph-in-Console fixtures are superseded by its text-only type; Common still rejects a graphical Console route without a status surface. |

The frame storage count remains two machine values, one session value, and
pending/captured values in each active leaf. Actual x86/x64 layout matches:
Window 984100 bytes, Common 985128, Console 7112 (old shared value 998384).
Window text copy is 14288 bytes; graphics prefix 1060. Seven main values with
both leaves shrink by 2050880 bytes before small metadata/alignment differences;
UI temporary shrinks by 991272 bytes. No whole-process memory claim.

## Accounting And Verification

Reproduce with git diff --numstat b30cc0da 17c9da90, classified by paths/suffixes.
No unrelated Queue/cell-attribute proposal edit is included.

| Category | Added | Removed | Net |
| --- | ---: | ---: | ---: |
| Production C/H, 31 paths | 469 | 345 | +124 |
| Test C/H, 28 paths | 406 | 312 | +94 |
| Build/dependency/test gates, 4 paths | 9 | 3 | +6 |
| Manifests, 4 paths | 65 | 62 | +3 |
| Documentation, 10 paths | 198 | 29 | +169 |

Total textual P1: +1147/-751, net +396. This closure is documentation-only,
separate from implementation accounting. Most code edits are relocated fields
and call sites; additional metadata enables opaque storage, not another pipeline.

Final full serial CTest: x64 110/110 (71.78 seconds), x86 110/110 (97.91 seconds).
Both Release builds succeeded. Package, restart, snapshot transaction/cross-process,
shared manifests/DAG and negative gates pass. Documentation/whitespace gates pass.
Earlier failures and their corrections are recorded in the proposal: u16 fixture
byte fills, incomplete test migrations and one source/manifest edit overlapping
a test run. No production workaround hides those failures. Native Linux and new
manual Windows installation tests are not claimed.

| Package | Bytes | Change | SHA-256 |
| --- | ---: | ---: | --- |
| softpc32.exe | 3654118 | +1514 | 66BAA0C96DEC3E8DF57D08289AC33232FAE901FF181C77E28CB096DE376B9921 |
| softpc64.exe | 3058686 | +1523 | 58A5E4B67FB74D3F7092E09E8CD68E0CE3BB4129EB9D87F61C226F9E0F1DBDF5 |

## Closure

S4 exit criteria are met; coordinator accepts and closes S4. S5 remains required
for strict component-owned capacity/status validation and removing VM clipping.
S6 remains required for final simplification/integration audit. T71 stays open
for owner acceptance; no whole-Lib or whole-machine correctness claim is made.
Separate queue/candidate work is preserved. Temporary owned layout probes are
removed after their results above are retained; build/output and media untouched.
