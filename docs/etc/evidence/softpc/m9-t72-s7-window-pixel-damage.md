# M9 T72 S7: Unified Window Pixel Damage

## Request And Boundary

Owner accepts S6, moves the planned Common repair to S8 and admits a new S7:
unify text/graphics conversion to bitmap, compare with the current bitmap and
draw only dirty regions to reduce possible flicker. Baseline 2cf87250.
The proposal section fourteen is the finite design; no Common/VM/Compat/MVDM,
public API/schema, mailbox, input, mouse, INI, media or snapshot changes.

Both formats decode colours directly into one inline comparison/write/damage
helper against the existing surface. Owner requested removing P1's intermediate
5120-byte row array; no replacement buffer is introduced. Existing graphics_valid becomes
surface_valid, with no extra persistent state. First/recreated surfaces fully
invalidate; mode names alone do not force repaint at identical dimensions.
Native paint retains its original stretch and clipping; native dirty regions
accumulate until paint. Text cursor remains a separate inversion overlay with
the same blink phase; changed old/new overlay rectangles also invalidate.

## Finite Verification Ledger

| Boundary | Proof / disposition |
| --- | --- |
| Pixel decoding and full coverage | Retain S6 51 poisoned-surface cases, two banks, blank/pattern glyphs, heights0..16, small/max grids; now also assert full initial bounds and repeated-frame no damage. Original 512 attribute pixel checks remain. |
| Graphics comparison | Existing skip-A/latest-B, palette-only, equal-colour indices, stride padding and recreated-surface checks use the unified function. |
| Text comparison / native publication | Mock consumes font-only/palette-only changes, erase-to-background, repeated text, two text publications collapsed before consumption; both changes reach pixels/dirty bounds. |
| Mode transitions | Equal-size graphics-to-text compares actual changed pixels; subsequent equal black graphics needs no invalidation. |
| Cursor-only changes | Native mock tests visible cursor admission, old/new position rectangles, repeated cursor, blink off/on, hide; existing geometry and product cursor tests retained. |
| Surface / lifecycle | Surface recreate invalidates full size even same RGB; existing native allocation/failure/retirement/freeze/resize tests retained, no new failure loop. |
| Similar-issue sweep | rg across src/test for render_text/render_graphics/graphics_valid: none remain. Reviewed leaf invalidations: resize, freeze and native exposure retain independent repaint responsibility; bitmap consume uses one damage path. |
| Deferred Common | Original audit A/B/C/D/F are S8, unchanged and not silently dropped. |

No extra full-frame cache or previous-frame copy. Text adds comparisons but
avoids unchanged surface writes/full native invalidation; graphics no longer adds
row scratch traffic. Both remain O(pixels). No benchmark or visual flicker elimination
claim. Cursor overlay cannot be omitted merely because bitmap pixels match.

## Change Accounting

Estimate: production3 files +60--90/-65--90; tests2 +90--140/-25--45.
Actual C/H (`git diff --numstat 2cf87250 -- src test`): production3 +49/-45
(net+4), tests3 +97/-27 (net+70). Extra test is the product runtime-cursor
consumer of the private render API: +3/-1 only, no product source change.
Manifest, README/design/proposal/state/history/evidence and EXEs counted separately.
Lower production churn comes from retaining existing comparison and invalidation;
no one-line compression or removal of verification to meet an estimate.

## Verification

Both Release builds complete; Lib strict warnings remain enabled. Focused
frame-damage/native-consume/runtime-cursor checks pass3/3 on each width.
First builds found the product cursor fixture still using render_text; it was
migrated, all old names searched globally, and both builds/focused checks rerun.
Full background: x64 105/105 (163.33s), x86 105/105 (152.46s), including
Win3.1 PIF roundtrips (58.95s/58.09s), snapshots, restart, manifests/DAG and
documentation checks. Final comment-only fixture correction rebuilt and focused
rerun on x64; x86 full run used the rebuilt fixture. Desktop/RDP visual acceptance
is explicitly left to owner; no native Linux presenter claim.
No custom trace or disposable probe was made; existing incremental build trees
retained. Five desktop tests per width remain excluded.

| Package | Bytes | SHA256 |
| --- | ---: | --- |
| softpc32.exe | 3659580 | 32B131AE93F98853D5D7ABA45A46B388D17217A506FCDA3DC599CEEE86031366 |
| softpc64.exe | 3062585 | B348E985C77A9C9986E093F363916CCF0F1EA635A92FF3B0742DB4739ED07C7C |

Compared with S6, package sizes change -63/+450 bytes; this is not a runtime
memory or performance measurement. S7 owner acceptance pending after delivery.

## P2: Remove Intermediate Row Buffer

Owner request: 各自解码直接调用共同比较更新，去掉不必要的行缓冲；
保留当前语义、文本/图形同一 dirty 逻辑及光标旧、新区域补刷。
Baseline 035d2845. One static inline pixel helper owns comparison, surface write
and damage bounds; both decoders call it directly. Text still selects each
cell/font row once before its eight pixels. No intermediate colour array,
allocation, new state, API or native-worker change. The 5120-byte source-level
scratch array is removed; compiler stack/code-size effects are not a benchmark.

Actual production render.c +26/-34 (net -8), tests unchanged. Cumulative S7
production relative to 2cf87250 is +58/-62 (net -4); tests stay +97/-27 (net +70).
Existing pixel coverage, palette/stride, latest-wins, repeated-frame and cursor
tests are retained rather than rewritten around the new implementation.
Similar-issue search in kvm-window confirms no colours array or text-row staging
helper remains; exactly two decoder call sites use the one update helper.
Cursor invalidation and native rendering are byte-for-byte unchanged from P1.

Both Release builds passed. Full background regression passed x64 105/105
(169.98s), x86 105/105 (160.76s), including the existing pixel/cursor cases,
restart/PIF roundtrips, snapshots, shared manifests, DAG and governance checks.
Five desktop tests per width excluded; no native Linux/RDP visual claim.
No new probes or disposable build children were created. Existing incremental
build trees retained; user INI/media unchanged.

| P2 package | Bytes | SHA256 |
| --- | ---: | --- |
| softpc32.exe | 3659580 | 2E51DFDDA6131310B6A7FD568A75F407BE7996F05F468D5D67F0890AA1A49A4C |
| softpc64.exe | 3062073 | BD1438F5CAF1D732853B1A80DB8A72C41F097E9E140ADE785A90BB5ADB3CE841 |

## Coordinator Actual-Change Review

After P2 c813f698 was pushed, reviewed its actual diff against P1 035d2845
and the owner request. Both decoders call one comparison/write/bounds helper;
there is no intermediate buffer or duplicate comparison loop. Validation occurs
before surface mutation, full-first-frame and no-damage results are retained,
and text traversal preserves fixed stride, both banks and font-height handling.
Native cursor, paint, mouse, all tests, Common/VM/Compat/MVDM and INI/media
have no P2 changes. Package hashes match the verification record. Counts and
scope match the packet; no further code correction found. Documentation gate
and diff check pass. S7/T72 await owner testing, not closed; S8 remains deferred.
