# M9 T72 S5: Text Cell Array Evidence

## Request, Scope And Design

Owner: "我希望是代码更干净，更易懂。看起来我们应该合并。准入一个新的S任务做这个。"
Baseline e118f058, clean worktree. The earlier parallel-array choice is explicitly
superseded, not the neutral meanings, capacities or leaf-resource ownership.
Six production C/H files and nine existing test C consumers form the frozen
universe in the proposal. Completion requires each boundary migrated/proved or
explicitly retained for its independent responsibility. No whole-repo claim.

One four-byte kvm_text_cell holds glyph_index, glyph_bank, foreground, background.
Compile-time size assertion forbids padding; tests also pin every offset. The
whole cells array can be compared without padding-dependent results. No packing,
new wrapper, allocation, cache, thread, queue, generation or conversion pass.
Public KVM source ABI changes atomically; old names/aliases are removed. Shared
corpus importers must update callers too. Snapshot storage does not serialize
this presentation schema and its format is unchanged.

## Finite Boundary Ledger

| Boundary | Result / proof |
| --- | --- |
| kvm-base/frame_interface.h | One cells array; original dimensions, row stride and visible-cell validation retained. sizeof/offset and frame-copy/bounds/hidden-tail assertions. |
| vm/driver.c | Same font readiness/data source order and device decode, complete default initialization, one cell assignment. Existing vga-frame 512 cases and cursor tests migrated. |
| common/ui/ui.c | Original status loop initializes complete cells, writes glyph_index only for message text. Composition verifies both message rows and all attributes. |
| common/machine/machine.c | Four array comparisons become one; cursor/palette/font/map comparisons unchanged. Publication test changes each of four cell fields separately, checks duplicates suppressed. |
| kvm-window/render.c | Same glyph bank, palette and bitmap interpretation; existing 512 cases plus two-row/two-column distinct cells verify fixed-80 stride and field separation. |
| kvm-console/console.c | Existing map loop also copies per-cell colours; original Unicode Console contract unchanged. Existing 512 cases plus distinct second-row cells verify glyph/map/colours. |
| Remaining test consumers | win32-presentation, runtime-cursor, vga-frame and restart-boot migrated. Prompt scanning reads glyph fields rather than treating cell bytes as strings. |
| Deliberate retention | Logical Console/Broker Unicode and colour arrays have a different lower-level contract; no reverse KVM dependency or second implementation added. |

Similar-issue sweep: searched all src/test C/H for old base.text/foreground/
background/glyph_bank array accesses; no hits remain. Examined bulk initialization,
copy, comparison, string scanning and graphics-to-text union reuse. VM clears the
text active extent before constructing cells. Frame-copy tests initialize every
cell. Mock-only composition/session fixtures retain their existing explicitly
prepared data. Historical nonbuilt diagnostics are not newly supported consumers.
No MVDM/Compat, mouse/native input, mailbox, logical Console/Broker, INI or media
changes. Public KVM input signatures/events remain unchanged.

## Accounting And Cost

Estimate: production +45--70/-40--70, near-zero net; tests +80--130/-60--100.
Actual git diff --numstat e118f058, C/H only: production six files +31/-33 (net -2);
test nine files +94/-57 (net +37). Reused existing loops and one array comparison;
no compressed formatting to manufacture a reduction. Four manifests and docs/
artifacts are separate. No claim of measurable whole-machine speed improvement.

Both widths retain Base text 8084, Window text 16276, Console text 9108, Window
full frame 984084, Common frame 985112 bytes. Logical Console remains unchanged.
All four cell fields occupy four bytes total, as before; frame capacity and
transferred byte counts are unchanged. Per-cell access locality differs; no
benchmark is presented as proof of performance improvement.

## Verification And Corrections

Both full Release builds pass; Lib retains -Wall -Wextra -Wpedantic -Werror.
Focused frame-copy/Window/Console/comparison/composition tests pass 5/5 per width.
Full background suites pass: x64 105/105 (159.37s), x86 105/105 (146.32s),
including Win3.1 PIF restart, snapshots, corpus and component DAG checks.
Five desktop tests per width are excluded to avoid disturbing owner input.
Owner acceptance is pending; no native Linux execution is claimed.

The first x64 build found one repeated test-only old glyph_bank field reference,
fixed before the successful rebuild. A focused rejection test initially failed:
it reused graphics union bytes as a text fixture, so the new layout exposed an
invalid colour before its intended font-height failure. The test now initializes
a complete text fixture; production validation/order was not weakened. Both
focused suites reran successfully after this correction. Existing preserved-core
TEXT macro redefinition warning in the VGA product test remains outside this
layout task; strict Lib compile passes.

| Package | Bytes | SHA256 |
| --- | ---: | --- |
| softpc32.exe | 3660155 | 0A9611586441D4E5646B98C3F0B0EBE531983FE567212C1AFE0ED1CA0A9A2C01 |
| softpc64.exe | 3062135 | 24A87846A00A3EB0B7AAF0F57D19E8F430B10B949B08D3F6771D7535CB74D55C |

Packages decrease by 2048/3072 bytes from S4 respectively; not a speed claim.
No custom diagnostic outputs created. Build trees retained for incremental proof.
S5 and T72 remain open for owner testing after delivery and actual-change review.

## Actual-Change Coordinator Review

Reviewed committed P1 da26717b against e118f058 after pushing to origin/main.
The six production paths match the admitted boundary ledger; actual C/H counts
remain +31/-33 and tests +94/-57. Default initialization covers every cell,
comparison covers all four bytes without padding, and both leaf consumers keep
their original font/map interpretation. No legacy field references remain in
src/test C/H. Protected MVDM, Compat, logical Console/Broker, native mouse,
motion, mailbox and INI paths have zero diff. Package hashes match the table.
Documentation governance and diff whitespace checks pass. No unresolved scope
expansion found; this review does not replace pending owner visual acceptance.
P2 records this review only; S5 and T72 are not closed.
