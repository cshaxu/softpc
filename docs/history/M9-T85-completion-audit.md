# M9 T85 — completion audit

The owner approved closure of T85 on 2026-10-09.  This audit closes the
task; it does not claim to resolve the separately queued raw-to-cooked Console
handoff or Win3.1 File Manager VGA width-transition defects.

## Original objective and admitted expansion

T85 began with the owner-requested investigation and repair of Windows 1.01
startup from the SoftPC package.  The owner then explicitly admitted its
successive bounded deliveries: Windows 1.01 display/input repairs, shared
corpus alignment, idle execution, neutral audio, IBM-PC/Product extraction,
build routes, the Emulator rename, and the final Product monitor correction.
Each delivery retained its own boundary rather than treating the task as a
single unbounded rewrite.

## Finite S ledger

- **S1--S4:** repair Windows 1.01 mono/InPort and indexed-pointer behavior,
  synchronize the initially admitted NXVM subset, and park verified idle DOS
  execution.  The owner accepted each.  Evidence: [S1](M9-T85-S1-windows-101-startup-repair.md),
  [S2](M9-T85-S2-indexed-pointer-repair.md), [S3](M9-T85-S3-nxvm-shared-subset-sync.md),
  and [S4](M9-T85-S4-dos-idle-cpu-repair.md).
- **S5--S10:** establish neutral Audio, strengthen shared-component and IBM-PC
  ownership boundaries, import the necessary IBM-PC corpus, repair complete
  frame publication, and simplify the imported IBM-PC surface.  Their
  delivery evidence is retained in [S5](M9-T85-S5-audio-delivery.md),
  [S6](M9-T85-S6-shared-boundary-delivery.md), [S7](M9-T85-S7-ibmpc-import-delivery.md),
  [S8](M9-T85-S8-common-frame-invalidation-delivery.md),
  [S9](M9-T85-S9-x86-bus-lifecycle-delivery.md), and
  [S10](M9-T85-S10-ibmpc-simplification-closure.md).
- **S11--S13:** converge the shared package corpus, refresh it with full
  package qualification, and investigate/repair the presentation-shutdown
  stall.  Evidence: [S11](M9-T85-S11-nxvm-eight-package-sync.md),
  [S12](M9-T85-S12-eight-package-refresh.md), and
  [S13](M9-T85-S13-shutdown-stall-investigation.md).
- **S14--S20:** move the surviving shared Product responsibilities into their
  final Product/Emulator ownership, preserve owner-local App composition and
  shutdown tests, establish Ninja/ccache developer routes, rename Common to
  Emulator, and correct Product monitor startup/error/load completion output.
  Evidence for the persisted closure stages is [S16](M9-T85-S16-product-ownership-closure.md),
  [S17](M9-T85-S17-product-ownership-closure.md),
  [S18](M9-T85-S18-ninja-build-route-closure.md),
  [S19](M9-T85-S19-emulator-rename-closure.md), and the accepted
  [S20 record](M9-Td-S20-s20-closure-and-display-queue.md).  S14/S15 are
  intermediate owner-directed delivery phases retained in the task proposal;
  they introduced no unresolved independent receiver at closure.

## Changed production ownership

The final retained structure is `lib < emulator < product`, with
`app-softpc` retaining product identity, INI/configuration, private machine
composition, snapshot/media extensions, and package policy.  Core/Compat
remain the SoftPC adapter and preserved machine baseline.  T85 did not leave a
second executor, presenter, command parser, or Product configuration owner.
The final S20 code change is intentionally narrow: Product Monitor prints the
generic pause completion only for `RUNNING -> PAUSED`; the App remains sole
owner of the successful snapshot-load result.

## Completion and outstanding disposition

Evidence is recorded per S because the task deliberately expanded through
owner-directed, separately accepted boundaries.  The final accepted code
delivery `d3bd20f9` passed focused x64/x86 Emulator monitor, corpus, manifest
and dependency checks and rebuilt both packages; task governance and display
queue recording are pushed in `3fdd94d3`.

The newly recorded raw Console/cooked monitor transition and Win3.1 File
Manager width transition are distinct queue candidates, not unclosed T85
failures.  Existing TODO debt, including intermittent RDP capture escape and
the x86 null-BOP investigation, is likewise retained with its own admission
conditions.  No active T85 subtask or task-owned dirty work remains.
