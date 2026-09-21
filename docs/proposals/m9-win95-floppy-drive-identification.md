# Win95 A: Floppy Drive Identification Repair

## T79 Admission And Request Boundary

Owner: "队列第二位追加一个T任务，用于修复win95把软驱A盘识别为
“可移动磁盘”而不是软驱的故障，Td治理。"

Owner admitted this queue head as T79 after accepting T78. The reported
symptom is not yet independently reproduced; root cause remains unknown.

T79 S1 is an evidence-first investigation. It may inspect the source tree and
map the guest-visible drive classification contract, but it may not modify
production code, user configuration, guest media, Lib/Common, or the preserved
Core mirror. Reproduction uses only a declared disposable overlay after the
evidence plan identifies it. A repair requires a separately admitted S with
the first incorrect contract, exact owner and dual-width proof recorded first.

## Product Goal

Windows 95 should identify the emulated A: device as the configured floppy
drive, not present a generic removable-disk identity in its place. Establish
whether the difference is device classification, driver selection or only
shell description/icon before deciding a repair. Do not infer a hardware fault
solely from the generic word "removable", and do not patch guest labels.

Preserve DOS/Win3.1 access, empty-drive behavior, insertion/ejection, readonly,
direct and overlay policies, reset retention and snapshot media restoration.

## Investigation And Engineering Plan

1. Freeze x86/x64 baseline and reproduce with disposable media/overlay. Record
   actual Win95 drive properties, driver/device enumeration and A: access with
   media present and absent. Separate cold boot from snapshot restoration.
2. Trace the selected original floppy device, CMOS/BIOS equipment and disk
   service responses, controller behavior and VM/Compat media adaptation.
   Compare with original source and a known-good floppy classification where
   available. Identify the first incorrect contract; hypotheses are not fixes.
3. Repair at that owner with minimal scope. Prefer a general device-contract
   correction over Windows-version tests, shell workarounds or a second device
   implementation. VM/Compat own product/host adaptation; original device logic
   stays MVDM. Any proposed Lib/Common change requires explicit scope review.
4. Add focused regressions for the observed contract and analogous paths.
   Build both EXEs, run full background regressions, then obtain owner Win95
   verification. Reserve desktop interaction explicitly when required.

These are the T79 S boundaries: S1 investigation, then separately admitted
evidenced repair and acceptance as required. Before each admitted S, report affected files,
estimated additions/removals, original-mirror diff and acceptance evidence;
afterward report actual counts and package links.

## Acceptance And Non-goals

- Proven floppy identification on both widths, with evidence distinguishing it
  from mere wording; successful A: access, no-media handling and media changes.
- Readonly/direct/overlay and snapshot reload retain their established behavior;
  new tests create small disposable images outside committed assets.
- No modifications to owner INI, guest installation or original disk images.
  No speculative capability expansion, guest-specific branch or unrelated
  storage redesign. Native desktop tests must not interrupt owner activity.
- Stop for design review if evidence points to a guest-only configuration issue
  or an unapproved shared ABI change; do not manufacture a host fix.

Reference: [current baseline](../states/CURRENT.md),
[T71 closure](../history/M9-T71-completion-audit.md).

## T79 S1 Investigation Record

## Request and status

Owner requested finding why Windows 95 does not identify A: as a floppy drive,
and designing the correct repair. S1 is investigation only. Production and
tracked tests are unchanged. Repair and product acceptance remain outstanding.

## Frozen S1 evidence ledger

| Boundary | Evidence and disposition |
| --- | --- |
| Supplied snapshot | Existing canonical decoder accepts it. Its physical BIOS equipment word is `c820`, diskette-present bit zero. Both floppy media slots are absent; the HDD uses overlay. No guest execution or writes were needed for these observations. |
| Empty cold start | A diagnostic linked to the unchanged Core returns GFI type 0, CMOS 10h=00, equipment c820, INT 13h/AH=08h BX=CX=DX=0, CF=0. |
| Insert after cold start | GFI becomes type 4 but CMOS/equipment/BIOS parameters remain unchanged. This proves insertion does not correct the boot-time identity seen by DOS. |
| Reset with inserted 1.44 MB media | GFI=4, CMOS=40, equipment=c821, BIOS BX=0004/CX=4f12/DX=0101/CF=0. |
| Eject without reset | GFI becomes 0 while CMOS and BIOS still identify a 1.44 MB drive. This is a second inconsistency from the same ownership error. |
| Reset empty | All identity fields return to the no-drive state. Reset currently changes hardware topology according to media presence. |
| Width dependence | The five observations above are identical with existing x86 and x64 Core objects. Each probe exits zero, under one second; no Window or Console interaction. |
| Original host meaning | OpenNT `host/src/nt_rflop.c` documents drive_type as maximum drive capability, separate from inserted-media format. Switching to gfi_mpty disables the host device, not simply the disk inside it. |
| DOS and shell consumer | Comparison DOS `bios/msinit.asm` treats equipment absent plus BIOS drive-count zero as fake floppy drives and skips ordinary physical BDS setup. OpenNT shell's non-WINNT `drivesx.c` uses DOS IOCTL 440Dh/0860h device parameters to specialize generic removable drives to floppy icons/types. This source is comparison evidence, not proof that the supplied guest contains identical binaries. |
| Existing test gap | `test/core/machine_smoke.c` explicitly expects c820 for HDD-only configuration. `fdc_smoke.c` proves present-media CMOS=40 and no drive after machine destruction, but does not test an installed empty drive. |

The first incorrect contract is in `src/core/compat/gfi_image.c`:
`softpc_platform_floppy_attach(NULL, ...)` installs the original no-device GFI
server and zeroes the entire drive object; `softpc_platform_floppy_config_value`
also equates medium presence with drive existence. Image geometry supplies
drive_type, so media replacement additionally changes physical identity.
The original BIOS then consistently consumes these incorrect host facts.
Lib Storage, Common's removable-media request, and Win95 naming are downstream
of this error; none should own a correction.

This establishes the incorrect hardware facts in the actual supplied snapshot
and reproduces their cause using both compiled widths. A fresh Win95 shell
before/after acceptance is still required during repair. S1 did not operate
the guest desktop or claim such acceptance.

## Repair design

Separate physical-drive lifetime from inserted-media lifetime in the existing
Core host adapter. A: exists empty at initial hardware setup. Its default
profile is 1.44 MB. To preserve existing initial-image profiles, initial
hardware creation may retain the profile selected from that image; subsequent
insert/eject must only change the medium, its geometry, access mode and media
change indication. B: remains absent. The original FLA/FDC/BIOS still implement
their protocols. No new controller, Common state or Lib API is needed.

1. Keep the existing GFI callbacks installed for an existing empty A:. The
   already present no-medium read/write guards return the proper error, while
   drive-type/config queries continue reporting an installed drive. Clear the
   complete drive and install gfi_mpty only at hardware teardown, through the
   existing distinct teardown call.
2. Keep physical profile independent of candidate media geometry. Changing
   image format must not advertise a different physical drive halfway through
   a boot. Preserve readonly/direct/overlay semantics and all accepted image
   formats; explicitly test the non-default profiles already supported.
3. Sweep ready, write-protect, seek/recalibrate and disk-change responses for
   an existing empty drive. Use the original GFI contract; a valid empty drive
   must not reuse absent-drive equipment-check responses. Avoid guest-version
   branches and do not edit BIOS equipment words from the media setter.
4. Snapshot must preserve the physical profile even when its media slot is
   absent. Paths/modes/overlays remain media properties. Encode the two GFI
   physical drive-type values (A/B, including zero for absent hardware) in the
   existing Core-owned snapshot composition, independently of media-present
   flags; restore them before resuming. CMOS is guest-writable controller
   state and cannot replace this physical-host capability record. A variable
   physical profile cannot simply be re-derived from a missing media slot.
   Treat the supplied old no-drive snapshot as evidence of the old hardware
   state; silently patching only CMOS cannot repair DOS's already initialized
   device tables. Corrected Win95 acceptance must include a cold boot.

This requires a narrow Core snapshot-format adjustment, with no Common/Lib API
change or new snapshot subsystem. The old stream has no physical-profile
field for empty drives. Do not silently accept it as the new format or claim
backward compatibility. Retain the accepted baseline binary to inspect the
provided old snapshot; final repaired acceptance uses a fresh cold boot and
new snapshots. The owner must see this format consequence when admitting
implementation.

## Proposed next S and estimate

A single repair S should cover `core/compat/gfi_image.c`, its existing platform
declarations, `core/machine/machine.c` hardware setup/teardown, and the minimal
Core snapshot hooks needed for physical-drive identity. Approximate production
range is +50 to +100 / -25 to -50, net +25 to +50. Focused tests in
`test/core/{fdc_smoke,machine_smoke}.c` and existing snapshot integration tests
are expected +90 to +160 / -5 to -20. These are planning ranges, not measured
diffs. Lib/Common/x86 and preserved softpc.new target zero production changes.

Acceptance matrix: empty cold boot, insert, replace, eject, reset, destroy;
default/non-default startup profiles; A present/B absent; all three media
modes; snapshot with and without media; DOS/Win3.1 and Win95 boot/access. The
hardware identity must remain stable across media operations and the guest
must receive ordinary no-media errors while empty. Rebuild both EXEs and run
focused plus background regression before owner Win95 acceptance.

## S1 verification limits and cleanup

Disposable probes only read original media and decode the supplied snapshot;
no guest installation, package INI, original image or snapshot was altered.
Tracked production/test diff is +0/-0. Temporary probe code/binaries are
removed after recording the results. No GUI test or repair is claimed.

## S2 admitted repair ledger

Owner approved the above repair. Estimate: production +70..110/-25..45
(net approximately +45..65), tests +90..160/-5..20. Counts exclude docs/EXEs.
Use the existing GFI object and device-archive codec, not a new owner or API.

| Frozen path | Required disposition/proof |
| --- | --- |
| First hardware init | Default empty A: type 4; initial image profiles retained; B absent. |
| Insert/replace/eject | Medium geometry/mode may change; physical type remains stable. |
| Empty commands | Ready false, no-data errors; seek/recalibrate remain drive operations. |
| Reset/destroy | Reset preserves hardware/media; destroy alone removes GFI identity. |
| Snapshot | Canonical fixed-width physical identity independent of media-present flags; new-format cross-width and empty/non-default roundtrip. |
| BIOS consumer | Correct empty equipment/CMOS/INT13 identity without modifying BIOS. |
| Integration | Existing DOS/device/snapshot regression both widths; owner cold-boot Win95 validation remains required. |

Completion requires every row evidenced, no shared/mirror edits and a pushed
dual-width P. S2 waits for owner testing; T79 remains open.

## S2 implementation and verification record

The existing attach call initializes physical A once and thereafter replaces
only its medium/geometry. Only hardware teardown removes the GFI callbacks.
The geometry helper no longer writes drive_type, including during snapshot
media restoration. Existing Machine callers need no changes. The device
archive stores two canonical u32 drive types (eight wire bytes), independently
of its media archive. No version tag or old-format fallback was added.

### Changed-path accounting

Measured with `git diff --numstat f6dadddd -- src test`, excluding docs/EXEs:

| Path | Added | Removed | Net | Retained responsibility |
| --- | ---: | ---: | ---: | --- |
| core/compat/gfi_image.c | 40 | 23 | +17 | Existing GFI host drive/media lifecycle and callbacks. |
| core/compat/devices/snapshot.h | 7 | 0 | +7 | Private fixed-width physical type record. |
| core/compat/devices/archive.c | 9 | 0 | +9 | Existing capture/restore and canonical codec. |
| test/core/fdc_smoke.c | 112 | 2 | +110 | Empty/insert/eject/reset/destroy, modes and physical profiles. |
| test/core/machine_smoke.c | 2 | 2 | 0 | Correct HDD-only equipment expectation; real INT11 boot path. |
| test/core/checkpoint_smoke.c | 24 | 0 | +24 | Empty non-default identity, invalid types and missing fields. |
| test/integration/snapshot_transaction_smoke.c | 27 | 6 | +21 | New-process empty/present physical identity restoration. |
| test/integration/snapshot_cross_process.cmake | 22 | 27 | -5 | One matrix runner, optional different-width reader. |
| Production total | 56 | 23 | +33 | Three existing files; no new runtime object/thread/lock. |
| Test total | 187 | 37 | +150 | Includes CMake matrix orchestration. |
| Code/script total | 243 | 60 | +183 | No shared-corpus or original-mirror change. |

Production is below the estimate because existing init/eject/destroy callers
already provide distinct boundaries. Tests exceed the addition estimate to
cover five profiles, all three modes and cross-process empty-media identity;
the matrix runner removes duplicated orchestration.

### Similar-issue sweep and finite-ledger disposition

Search: `softpc_platform_floppy_(attach|detach)`, `softpc_floppy_media_restore`,
`gfi_empty_active`, `drive_type =`, and conditional `put_*` arguments in GFI.

- Initial attach, runtime media replacement and media restore all use the
  same GFI object. Initial type selection is the only media-derived hardware
  assignment; restoring physical snapshot state is the other deliberate writer.
- Existing machine reset does not remount media. Its destroy call is the sole
  hardware teardown caller; only teardown installs gfi_mpty. B/C/D remain
  uninstalled. Snapshot describes the product's physical A/B pair only.
- Sense-drive ready/write-protect, read-ID, read/write and format now
  distinguish empty media from hardware absence. All empty operations receive
  focused proof; seek/recalibrate retain the original successful host response.
- The original put-bit macros do not parenthesize their value parameter.
  Parenthesizing the write-protection expression fixes the same-path old
  precedence defect; the other conditional macro calls were checked.
- Disk-change response retains the existing image-server policy: clear with
  an attached medium, asserted with no medium. No new latch/controller behavior
  or Win95 branch is introduced.
- GFI and CMOS identity are checked for all five existing profiles. INT13
  type=4 is asserted for the product default and drive count=1 throughout.
  Low-density BIOS type selection also depends on its guest detection status;
  the direct host test does not claim to run that sequence or repair it.
- Snapshot media reconstruction finishes before the existing device archive
  restores hardware type. Empty and differently formatted media cannot redefine
  saved hardware. Missing identity bytes fail decoding; unsupported topology
  fails restoration. Existing media overlays/access-mode paths remain intact.

### Evidence and limitations

Release x64/x86 builds succeeded. Focused `fdc|checkpoint|machine-smoke|snapshot`
passes 7/7 per width (9.43s / 7.30s). Empty/present cross-process cases pass
with x64 writer/x86 reader and x86 writer/x64 reader; same-width cases are in
CTest. Each also checks rejected RAM mismatch, restored pixels, resume/input
and stop. Initial new-test failures were corrected: use the original byte
type and BIOS_DISKETTE_IO endpoint; do not assume low-density guest detection
was performed by a host-only test. The macro precedence failure led to the
in-scope production correction above; no failed run is reported as a pass.

Full background regression passes x64 110/110 (178.87s) and x86 110/110
(163.39s), including the restart/boot integration. The final refactored matrix
runner additionally passes x64 standalone (1.90s) and both cross-width routes;
x86's full run includes it. Five desktop tests per width are excluded. No
interactive Win95 acceptance, Linux runtime acceptance or
old-snapshot compatibility is claimed. Owner must cold boot the corrected
machine, then create new snapshots; the supplied old snapshot preserves the
old DOS device tables and lacks the new host-identity fields.

Package SHA256:

- softpc32.exe (3,660,332 bytes): `A0C1B40B05897F2C14E555808134F1CA1733320693D89618CA912C91019AA829`.
- softpc64.exe (3,062,821 bytes): `23B2DA9CF6BA9F8C1904B4F329F9AA76E388079ED373470A938F747B559394CF`.

Owner INI/media and all six shared source/test corpora are unchanged. The
pre-existing unrelated Queue/shared-audit-proposal edits are preserved and
excluded from this P. Disposable cross-width images/snapshots are removed by
the matrix runner; only normal ignored build/test outputs remain.

Documentation governance and `git diff --check` pass. Executor P1 `cba189b5`
is pushed to origin/main. S2/T79 stay open for owner validation, not closed
by these background results.

### Coordinator actual-change review

After P1 push, reviewed `git show cba189b5` and the actual source changes,
the original request, packet, matrix evidence and EXE hashes. All 14 paths
are accounted for: three production, five test/script, two binaries and four
design/governance documents. No shared source/test, original mirror, INI or
guest-media edits are present. The physical identity has one owner; archive
hooks reuse that owner and add no second controller or media path. The old
snapshot limitation and unperformed GUI acceptance remain explicit.

Implementation delivery is accepted for manual testing, not S/T closure.
Only the previously existing unrelated queue/proposal edits remain unstaged.
