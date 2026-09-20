# M9 T72 S6: Lib Text Render Simplification Evidence

## Admission And Boundary

Owner: "分成两个s任务 第一个先优化lib 第二个处理common".
Baseline c635988a. This is audit item E only; A/B/C/D/F belong to planned S7.
S5 is closed; S6 and T72 require owner acceptance after delivery. The current
packet and proposal section thirteen define execution, not a new architecture.

The sole production change removes two lines clearing the text pixel surface.
Validated width and height exactly equal columns*8 and rows*cell_height. The
existing four nested loops cover every pixel, selecting foreground or background
even for an empty glyph. No preceding pixel value is read. Invalid input returns
before writing, as before. Public ABI, graphics, native input, frame transport,
Common, VM, Compat, MVDM, INI and media are unchanged.

## Finite Coverage And Similar-Issue Review

| Unit | Disposition / proof |
| --- | --- |
| render.c text clear | Removed; 51 cases: three grids (1x1, 3x2, 80x25) times heights 0..16, with default zero meaning16. Every pixel must equal its expected nonzero colour, not a poisoned prior surface. |
| Glyph selection | Blank and patterned glyphs, both banks and both colours; existing 512-attribute and fixed80-stride tests remain. |
| Bounds and rejection | Pre/post surface sentinels unchanged; invalid dimensions/frame do not write the first pixel. Existing validation tests retain full contract coverage. |
| Other explicit leaf clears | `rg -n 'lib_memory_set\(' src/lib/kvm-window src/lib/kvm-console`: retain native surface creation initialization, cursor masks and input/motion reset. They are different initialization/lifetime boundaries, not the per-frame redundant text pass proved here. |
| Common findings | A/B/C/D/F assigned to S7, not silently repaired or dropped. |

Only this finite loop-coverage claim is made, not universal graphics correctness.
No new cache, helper layer, object, state or allocation. Maximum text rendering
avoids 1,024,000 bytes of redundant writes per call; no whole-machine speed
benchmark or improvement percentage is claimed.

## Accounting

Estimate: production +0/-2; test about +40--60.
Actual `git diff --numstat c635988a -- src test`, C/H only:
one production file +0/-2 (net -2); one test file +46/-1 (net +45).
Two manifest entries/revision labels and two rebuilt EXEs are separate.
No new test executable or CMake target; the existing rendering test is extended.

## Verification And Delivery

Both full Release builds complete with strict Lib warnings enabled. Focused
library.kvm_frame_damage_mouse passes both widths. Full background results:
x64 104/105 (161.37s), with only the documentation placement error below;
corrected documentation rerun 1/1 (0.23s). Every runtime test passed. x86
105/105 (147.31s). Win3.1 restart/PIF roundtrips passed in 57.92s/57.31s;
snapshots, component DAGs, manifests and strict layout gates passed.
Five desktop tests per width remain excluded; no desktop/RDP or native Linux
acceptance is claimed. Owner INI/media untouched; no bespoke trace/probe created.
Existing build trees remain for incremental checks.

The first documentation gate rejected explanatory text after the enum-valued
Identifier Mode field. It was corrected to literal Continuation and the gate
then passed; this was packet formatting, not a build or runtime failure.
The initial delivery record was placed under history too early; the gate treats
such a record as a completed identifier. It now resides in indexed evidence
while S6 is active; history is reserved for its later owner-approved closure.

| Package | Bytes | SHA256 |
| --- | ---: | --- |
| softpc32.exe | 3659643 | 3B43318B6D3D3CBB87EB7AE8C73318C2EAE9B1F1C377FCE5828AF348A188E3F2 |
| softpc64.exe | 3062135 | 73B91BF13590BC0A93428F61B14FD76C31AA7F5E617A484CAB32A1D6E3E33BC3 |

x86 package is 512 bytes smaller than S5; x64 size is unchanged. These are PE
file sizes, not a claim about runtime memory or timing. P1 carries delivery;
post-push actual-change review follows separately. S6 remains open for testing.
