# M9 T91 S2: NXVM cursor-contract import audit

Owner request: "准入一个新的s任务 请你审计nxvm最新6组件，代码质量情况 是否可原样导入".
Scope is audit only; no import or runtime modification. T91 remains open.
Owner approved proceeding with this audited import on 2026-10-13; the audit
is accepted and implementation is admitted separately as S3.
SoftPC baseline: `9295da5c`; previous imported NXVM: `73d7967244`.
Reviewed NXVM: `97d3cf9482913f2a0e0872158149ff646a563f3c`, clean working tree.

## Finite inventory and diff

All six roots contain the same 246 paths; 228 are byte-identical and 18 differ.
All six current NXVM manifests match their files. No file addition/deletion,
build-system, dependency declaration or source-notice change is in this delta.
Counts include manifests and component documentation, from upstream
`git diff --numstat 73d7967244 97d3cf948 -- src/lib src/emulator src/x86 test/lib test/emulator test/x86`.

| Component | Changed files | Added | Removed | Net |
| --- | ---: | ---: | ---: | ---: |
| src/lib | 8 | 58 | 20 | +38 |
| src/emulator | 2 | 2 | 2 | 0 |
| src/x86 | 0 | 0 | 0 | 0 |
| test/lib | 6 | 78 | 24 | +54 |
| test/emulator | 2 | 7 | 1 | +6 |
| test/x86 | 0 | 0 | 0 | 0 |
| Total | 18 | 145 | 47 | +98 |

## Reviewed semantics and quality

1. Base replaces cursor_visible with an explicit DISABLED/HOST/SOURCE enum.
   Mode validation occurs at the existing frame admission boundary. This is
   a real ownership distinction, not a machine-specific exception: HOST uses
   the existing native timer; SOURCE uses copied cursor_phase. Geometry still
   owns position/shape and does not select a second phase authority.
2. Window uses one small visibility helper for paint and old/new cursor damage.
   Source-only phase changes invalidate the old/new overlay without requiring
   bitmap damage. Native timer ticks cannot advance SOURCE. Entering HOST
   starts a bright interval; timer failure uses the existing terminal path.
   No additional worker, cached frame, retry or rollback mechanism is added.
3. Console deliberately maps both enabled modes to native host blinking and
   ignores source phase. This limitation is explicit in the common contract
   and leaf README, not silent guest-specific behavior. Logical Console keeps
   its own native cursor_visible field; it must not be mechanically renamed.
4. Emulator compares mode and phase in its existing copied-frame comparison.
   It does not interpret hardware phase or create a clock. X86 is unchanged.
5. Tests cover invalid modes, full-frame copy/layout, source phase with no bitmap
   change, ignoring host ticks, returning to HOST, disabled cursor, Console mode
   conversion and timer failure. Existing freeze/unfreeze and wrap tests remain.
   The delta introduces no external test-package dependency or Types bypass.

No blocking defect was found in this changed-path source review. Two small
nonblocking observations do not justify adding more state: SOURCE retains the
existing native timer but its handler returns without phase/drawing work;
Emulator still compares cursor_phase in HOST/DISABLED, potentially publishing
an unnecessary frame if a producer changes an ignored field. Neither creates
incorrect pixels or a competing phase owner.

## SoftPC receiving boundary

Shared roots can remain byte-identical; replacing only those directories is
not a compilable SoftPC migration. Three receiving files still name the removed
field: machine/driver.c, integration/runtime_cursor_smoke.c and
integration/runtime_smoke.c. Estimated adaptation is 3--6 replaced lines,
plus any focused assertion needed during qualification.

The production driver currently supplies cursor_phase=1, with presence derived
from the existing Compat position/visibility query. Preserve that behavior by
mapping presence to KVM_CURSOR_HOST, absence to KVM_CURSOR_DISABLED. SOURCE
would be wrong here: constant phase would produce a permanently bright cursor.
Keep Compat's native cursor visibility and original mirror untouched. UI's
zero-initialized status page remains disabled without adaptation.

The new enum/padding increases each text schema by eight bytes; Window's outer
frame remains 984080 bytes. No snapshot codec change is present or required by
this copied presentation contract. Import qualification must still test actual
SoftPC cursor behavior and both complete architecture suites.

## Evidence limits and recommendation

Every changed production/test hunk and receiving use was reviewed; inventories,
SHA256 manifests and diff whitespace were checked. This is not a fresh runtime
or full-tree correctness proof. No binaries were rebuilt and no guest was run.
Root-scoped Lib dependency/KVM naming and Emulator/X86 corpus checks pass.
Initial standalone check invocations omitted required root parameters; the
unscoped Lib check was interrupted and the naming invocation rejected its
arguments. Neither counts as evidence; corrected explicit-root checks passed.
Recommendation: approve exact six-root import plus the narrow SoftPC receiving
adaptation; no prerequisite NXVM code repair is identified. Do not equate this
recommendation with completed import/build/test acceptance.
