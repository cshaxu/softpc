# NXVM Shared Glyph-Capacity Import

## Admission And Original Request

Owner, 2026-10-12: 准入一个T任务，S1负责审计和引入nxvm最新的公共六组件。你先审计它的diff，给我一个汇报，到底改了什么

Admitted as M9 T91 S1. Audit/report comes before source import. This record
holds the initial finite audit while CURRENT owns the active packet.
The existing XP queue candidate and null-BOP TODO remain outside this task.

## Reference And Complete Difference Ledger

### Approved Implementation Baseline (supersedes initial audit below)

Owner: 行吧 那批准开始导入 按照单人双角色模式治理规范执行 完成后进行编译 测试 部署 提交 推送 要求全量通过所有公共三组件测试和所有其他单元测试和集成测试

NXVM is now clean at 73d7967244c1591038d0c2f4dcec819192d809a1.
All 246 paths: 224 identical, 22 modified, none added/deleted; six manifests
match. src/lib +34/-23, src/emulator unchanged, src/x86 +3/-4,
test/lib +55/-15, test/emulator +3/-3, test/x86 +8/-3: total +103/-48,
net +55 including docs/manifests. No source notice or licensing change.

The current capacity name is KVM_WINDOW_FONT_HEIGHT=32; the shared default
is KVM_TEXT_FONT_DEFAULT_HEIGHT=16, owned by Base and used by both presenters.
Console line capacity and Debug argument capacity names are clarified without
behavior changes; the unused Debug assembler capacity define is deleted.
SoftPC driver cursor fallback must use the new default, not Window capacity.
Six existing receiving files are expected, no original mirror edits. Retain
the existing Compat loop and original FONT_MAX_HEIGHT source-slot contract.

The initial nine-path ledger below is retained as superseded research, not
the import identity. Qualification is local, not inferred from NXVM results.

SoftPC baseline: cc7365a686338d44b9537422ec3e77661bfebf48.
Read-only NXVM HEAD: 468e191ff4d41226bc0f3df6b751c02ce615b25e.
All nine differences below are currently uncommitted in NXVM. HEAD alone is
not their immutable identity; the content hashes below identify this audit.
Import must recheck the baseline and record its final identity before copying.
Existing shared-source notices remain unchanged; no license grant is inferred.

All 246 paths were compared with SHA256: 237 identical, nine modified, no
added/deleted paths. All six NXVM manifest entries match current file hashes.

| Component | Paths | Different | Added lines | Deleted lines | Net |
| --- | ---: | ---: | ---: | ---: | ---: |
| src/lib | 109 | 4 | 15 | 7 | +8 |
| src/emulator | 30 | 0 | 0 | 0 | 0 |
| src/x86 | 24 | 0 | 0 | 0 | 0 |
| test/lib | 49 | 3 | 46 | 12 | +34 |
| test/emulator | 22 | 2 | 3 | 3 | 0 |
| test/x86 | 12 | 0 | 0 | 0 | 0 |
| Total | 246 | 9 | 64 | 22 | +42 |

Counts include README and manifests. Production C/H alone is +6/-4, net +2;
test C alone is +46/-12, net +34. No CMake, dependency or test registration
change occurs in these six roots.

| Path | NXVM SHA256 |
| --- | --- |
| src/lib/kvm-window/frame_interface.h | 5E5BFFF6256AD2FA1730C55A13F16D0B44B9D752AE7BE19D5CF272552231D7A3 |
| src/lib/kvm-window/render.c | 01DA3373B46C5CB031CCCFEA09F03E814A53CA4E1C169DC74F42FBCEDDC279D3 |
| src/lib/README.md | 96A84384A66668B1A426361DE141CE2472FB92CF39FA53DE8BC7C9454D62F666 |
| src/lib/MANIFEST.sha256 | 9F6A8550669F7F8361AF3BF7FBE08B6F592CC023CCD909DC7B461F249F40E181 |
| test/lib/kvm_frame_copy_smoke.c | 1EE92CC84DF6524F3589B6C32D76819AFBEF8DC654E4CDF08AE2DABD882084FA |
| test/lib/kvm_frame_damage_mouse_smoke.c | 6243CACF5F2DA81113862F37532A3DDD8C7729E3FBCAF6E8915027BE85E78BAE |
| test/lib/MANIFEST.sha256 | CB057B4A1C58BF7FBD0D1693415A3BCF6039CA094BB86387B55759025957950B |
| test/emulator/machine_wait_smoke.c | 978A575FFC2F5ED2DC2CF52C3DF3672F273E694633CF11A37731F216D5976FBF |
| test/emulator/MANIFEST.sha256 | 47D0C706AB763C2A8EF3414A4DA38BB4B8A1EC33C35A5B780156ADDC1FB56C88 |

## What Changes And Why

Window retains default font height 16 and adds maximum height 32. Both existing
256-glyph banks now have fixed 32-byte slots, independently of active height.
The existing renderer changes its glyph-address multiplier; validator admits
1--32 and rejects 33, while zero still means 16. No new state, function, worker,
queue or rendering path is introduced. Console, graphics, input, Audio,
Emulator production and X86 production remain byte-identical.

The text payload grows from 24,276 to 32,468 bytes (+8,192). The outer Window
frame remains 984,080 bytes because its graphics union arm dominates. Text
copy/compare traffic increases; this is not a cost-free change, nor a claim
that every machine now produces valid 32-row fonts.

Lib tests extend height coverage and text storage guards to 32, exercise both
banks, glyph 255/row 31, populated row 20 at height 24, exact damage, unchanged
frame deduplication and rejection at 33. Emulator's test merely replaces its
two packed-16 fixture indices with the new slot-stride constant.

## Receiving Gap And Proposed Minimal Adaptation

SoftPC cannot only replace the shared roots: compat/video.c currently zeros
256*16 bytes per bank and writes character*16+row. It returns early above
height 16. The new renderer would therefore read the wrong glyphs even for
ordinary DOS text; heights 17--32 could become accepted but empty.

Adapt the existing producer's destination capacity/stride and active-height
bound together, keeping the original EGA-plane source addressing and its one
copy loop. Update machine/machine.c and machine.h array declarations and the
two existing receiving fixtures: unit/machine/vga_frame_smoke.c and
integration/runtime_cursor_smoke.c. Do not introduce a repacking cache,
second renderer or change the original source mirror. Compat must retain its
inward Lib Types boundary rather than importing the KVM leaf implementation.
Validate the actual App-to-Window layout at the Machine boundary.

Preliminary receiving estimate: five existing files, production roughly
10--20 replaced lines plus a small contract assertion if needed; focused
test additions roughly 30--70 lines. Final count depends on the approved
receiving contract and must be reported separately from the exact import.

## Verification And Delivery Plan

1. Recheck upstream identity and source admission facts; import six roots
   exactly, including manifests, without any NXVM runtime/build dependency.
2. Adapt the existing SoftPC font producer and prove known glyph pixels,
   both banks, short/default/tall heights, last slot and rejection bounds.
3. Rebuild both Release EXEs in the existing supervised Ninja trees; shared
   suites first, then remaining background tests, desktop qualification
   separately during a reserved period. Preserve INI/media/snapshots.
4. Audit final component differences, refresh both published EXEs, commit/push
   the complete implementation P, then wait for owner acceptance.

Initial audit is source/hash analysis only: no local build, runtime test or
source import has occurred. NXVM's in-progress T555 S6 evidence reports shared
and unit results, but its remaining integration/delivery work is not complete
and is not SoftPC acceptance evidence. No S1/T91 closure is claimed.

## Implementation And Executor Qualification

The approved 73d796724 corpus is imported: 246/246 paths compare byte-identical.
SoftPC receiving changes use the original FONT_MAX_HEIGHT for destination
slots in the existing Compat copy loop, retain height zero's 16-row default,
update Machine declarations and assert the Window/machine slot agreement.
The driver uses KVM_TEXT_FONT_DEFAULT_HEIGHT for cursor fallback. No source
mirror, snapshot codec, input path, configuration or guest medium changes.

Measured receiving production: four files +13/-9, net +4. Existing two App
tests: +64/-16, net +48. They cover real primary/secondary plane output,
zero/default/8/14/16/24/32/33 heights, slot 255, zero padding, cursor conversion,
and full-driver acceptance at 17/24/32 with rejection at 33. Source-copy loops
and frame publication keep their original owners; no intermediate font buffer.

The first x64 App run passed 47/48: an existing test still expected height 17
to be unsupported. The corrected boundary now proves 17/24/32 success and 33
rejection. This was a receiving-test migration omission, not a reason to relax
production validation or retry away a failure. Final x64 background: 84/84
shared (61.23 s), 48/48 other (74.66 s). Final x86 background: 84/84 shared
(61.88 s), 48/48 other (75.09 s). Both supervised builds and the background
supervisor exited zero. The first build compiled changed dependencies; the
x64 fixture correction reused the existing graph (only two Ninja actions).

Finite test inventory per width: Lib 51, Emulator 21, X86 15, App unit 29,
App integration 10, checks 11 = 137. The five desktop tests are Lib
Window retirement/modal, Console display, and both App package variants.
Owner approved the reserved desktop period and confirmed no keyboard/mouse
operation. Both serial native runs passed 5/5 without retry: x64 13.81 s,
x86 13.54 s. Combined qualification is 137/137 per width; no waiver or skipped
test. No Computer Use was used. Implementation is ready for the executor P;
coordinator actual-change review follows its push. Owner acceptance remains.

Published EXE SHA256:

- x86: 7E8F74C30EED9C518D8BCB9052E8EB93CE0B59EC991C7C99C0643BBB7B84055F
- x64: B8714346A29C3127B6B77EF7CE28D0D2205DE1C6D52E2E2EC616D7B18345198F

Compared to cc7365a6, x86 executable size is -512 bytes and x64 unchanged.
Source/code accounting (excluding docs, manifests and EXEs): Lib production
+13/-11, X86 production +2/-3, App production +13/-9: total +28/-23, net +5.
Shared tests +60/-15 and App tests +64/-16: total +124/-31, net +93.
The source review confirms unchanged original source addressing, one Compat
font loop, one frame publisher and no new resource/state lifecycle. The
current six roots remain byte-identical to the frozen upstream. User assets
and mirror have no diff. Full execution evidence is retained in the packet's
ignored T91/S1 final logs; first-run failure logs are retained separately.
