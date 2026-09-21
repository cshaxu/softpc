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
