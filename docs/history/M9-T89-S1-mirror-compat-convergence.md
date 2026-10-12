# M9 T89: Mirror and Compat Boundary Convergence

## Admission and requirements

Owner request: "准入队列1的T任务，并进入S1审计、调研和设计，给我一个可执行结论".

Baseline: `3c2e6e6939651187f17b97495b6018f82f258667` (accepted T88).
The [current record](../states/CURRENT.md) owns status; the
[proposal](M9-T89-mvdm-compat-boundary-convergence-proposal.md) defines the
eleven-group finite review universe. S1 investigates/designs only. Later
source-changing steps require their own admission.

Required results: preserve working behavior and component ownership, reduce
unnecessary original-mirror changes, avoid duplicated original algorithms or
header overlays, and give a concrete serial plan with separate code and
original-mirror diff estimates. T88's accepted fixes are retained.

## S1 executed admission brief

| Field | Required record |
| --- | --- |
| Identifier Mode | New: T89 S1, following owner-closed T88. |
| Admission And Approval | Owner admits queue head and directs S1 audit, research and executable design. |
| Objective | Audit the finite mirror/Compat universe against source peers and compiled owners; select minimal responsibility-preserving migrations. |
| Non-goals | No production repair, shared change, user INI/media/snapshot modification or S2 implementation. |
| Reference Baseline | SoftPC 3c2e6e6939651187f17b97495b6018f82f258667; read-only OpenNT 5e4619ab61c2aa76151e03973cce340be2933e61. |
| Candidate Proposal | [Mirror/Compat convergence](M9-T89-mvdm-compat-boundary-convergence-proposal.md). |
| Files And ABI Surface | Eleven member groups, Compat endpoints, CMake source/include selection and focused tests; diagnostic files only under ignored build/t89/s1. |
| Applicable Rules | Task Reading Set and linked execution/architecture/coding/document governance skills; source research and mirror-diff policy. |
| Verification | Complete token/peer/owner ledger, source call-chain audit, guest INT33 probe and focused baseline tests on both widths; documentation and whitespace gates. |
| Expected Markers | Every occurrence has behavior, owner, disposition and receiver; no duplicated algorithm or large shadow header. |
| Asset Needs | Existing accepted objects, disposable synthetic boot image, unchanged package assets; hidden diagnostic processes, 300 seconds each and bounded outputs, executor cleanup. |
| Reporting Requirements | Serial executable plan and separate production/test/original-mirror estimates; committed/pushed audit delivery with evidence limits. |
| Stop Conditions | Guest-visible feature expansion, unapproved increased mirror diff, shared API change or duplicated device/state ownership. |
| Exit Criteria | Record finite ledger, concrete design and diagnostic evidence; pass governance; deliver for review without automatic next-step admission. |
| Original Owner Request | 准入队列1的T任务，并进入S1审计、调研和设计，给我一个可执行结论 |
| Similar-Issue Sweep | All standalone directives, adjacent host calls, include lookup, snapshot consumers and test macro profiles. |

## S1 result and scope

Research baseline: OpenNT `5e4619ab61c2aa76151e03973cce340be2933e61`,
same relative paths under its `nt/private/mvdm/softpc.new`. No external source
is imported or linked. The finite scan found **36 conditional/definition
directives in 14 files**, covering all eleven proposal groups. Endif comments
containing the token are not extra directives. Adjacent snapshot, host callback,
build and test consumers are included in each disposition below.

Disposition vocabulary: migrate through an existing endpoint; migrate a small
host definition without copying original declarations; retain with an explicit
reason and receiver. Completion does not require zero occurrences of a macro.
It requires one implementation owner, all 36 occurrences accounted for,
preserved behavior and measured original-mirror differences.

The old candidate's zero-standalone-branch preference is too broad. In
particular its instruction to move PIC predicates into Compat conflicts with
the original PIC's ownership. Its mouse hypothesis conflates instance memory
initialization with guest driver installation. The revised design below is a
recommendation for subsequent admission, not authorization to implement S2.

### Frozen occurrence and comparison ledger

Counts below are the whole current file versus OpenNT, including unrelated
accepted differences. They are not T89 changes. Method: `git diff --no-index
--ignore-space-at-eol --numstat` per original/current file; no source rewriting.

| Mirror path | Directive lines at baseline | OpenNT + / - | Receiver / disposition |
| --- | --- | --- | --- |
| host/src/nt_keycd.c | 11,442 | 16 / 2 | S2; existing keycode table fingerprints and keyboard ingress |
| host/src/nt_sound.c | 1,8,42,60,113,125,184,190,209,437 | 29 / 0 | Retain selected host sink; sound_state and sound/first-delivery tests |
| host/src/nt_graph.c | 773,1138,1578,1602,2113 | 62 / 14 | Retain lifecycle/geometry ports; VGA and snapshot receivers |
| host/src/nt_cga.c | 22,41 | 21 / 3 | S2 include; retain local BIGWIN profile; text/CGA receiver |
| host/src/nt_ega.c | 38 | 40 / 12 | S2 include; EGA pixel/panning tests |
| host/src/nt_vga.c | 32 | 45 / 16 | S2 include; VGA/V7 pixel tests |
| host/src/nt_munge.c | 27 | 29 / 1 | S2 include; planar/panning tests |
| base/bios/reset.c | 527,751,838 | 14 / 6 | Retain hardware topology and guest-driver exclusion; BOP/media/mouse tests |
| base/keymouse/ppi.c | 70,137,168,212,228,234 | 41 / 3 | Retain complete post-gate notification and snapshot state |
| base/system/ica.c | 1362 | 136 / 0 | Retain PIC-owned deassertion; irq_smoke |
| host/inc/cfpu_def.h | 160 | 58 / 1 | S3 host FENV only; preserve endian layouts |
| host/inc/nt_graph.h | 212 | 9 / 2 | S3 executor-owned lock policy only |
| base/inc/ios.h | 23 | 1 / 1 | Retain 64 KiB selection; high port and COM/InPort tests |
| host/inc/host_def.h | 158 | 20 / 2 | S2 target-owned standalone definition; never restore NTVDM |

All listed C files are in `softpc-machine-devices`. `cfpu_def.h` is consumed
by original CCPU FPU and its layout test; `host_def.h`/`ios.h` cross original
CCPU/device/Compat and focused test translation units. CMake currently defines
SOFTPC_STANDALONE on original OBJECT targets, VM and the machine archive,
but the archive's definition is PRIVATE. Some tests rely on host_def.h defining
it. Removing that header definition alone would silently change their profile.

### 1. Keyboard: reuse the original record lookup

Both the VM and Compat use original static Scan-1 tables. VM already calls
`KeyMsgToKeyCode`; Compat currently calls the added `softpc_host_scan1_to_key`.
For the latter, construct a zeroed `KEY_EVENT_RECORD` locally in the existing
`softpc_platform_keyboard_scancode` function, assign `scan_code & 0x7f` to
`wVirtualScanCode`, call `KeyMsgToKeyCode`, and retain current make/break
dispatch. This removes the extra lookup function, not the original table.

Delete the now-unused Compat `input.h` and its includes in keyboard/platform
and the old helper-only test references. Keep the normal/extended table
fingerprints and one-past-end tests; replace helper comparisons with proof of
the actual Compat scancode-to-controller route. Do not simply delete coverage.
Keep the conditional exclusion of `BiosKeyToInputRecord`: that unrelated NT
OEM conversion uses `RtlOemToUnicodeN` and is not needed by this product.
Retain the accepted `>= sizeof` table-boundary repair.

### 2. Sound: preserve the existing state machine and endpoint

The original `PlaySound`, `HostPpiState`, `PulsePpi`, Timer2 calculations and
`LazyBeep` de-duplication already execute in nt_sound.c. The standalone fork
replaces the retired NT Beep-device open/IOCTL/close operations with
`softpc_standalone_audio_set_tone`. Compat owns PCM synthesis, wakeup and
the accepted first-onset handoff. This is already the intended owner split.

All ten directives select headers, locking or that unavailable NT endpoint;
they do not select a second frequency algorithm. Deleting original NT blocks
would increase mirror deletions; emulating NtCreateFile/IOCTL just to erase
the directives adds needless host machinery. Retain this bounded port. A new
audio state machine, generic sink object or moved PlaySound is rejected.

### 3. Update transaction: retain the existing two calls

`nt_start_update` and `nt_end_update` already bound the original renderer's
batch. Compat's begin/end calls preserve T88 body-generation readiness and
pointer composition. `InvalidateConsoleDIBits` occurs inside a batch and
cannot replace its completion boundary. Keep the calls and existing DIB owner;
do not move dirty algorithms or add another publication route.

### 4. Lifecycle and V7 geometry: preserve semantics, do not invent a wrapper

The five nt_graph directives suppress NT-native fullscreen/lifecycle guards
or use the selected V7 painter's width contract. In the actual host, global
`sc.ScreenState` has no production writer selecting native NT fullscreen;
it is not the App's Window/Console choice. Nevertheless two existing regression
tests intentionally set FULLSCREEN and require clear/cursor metadata to work.
Restoring the original guards on the strength of today's zero initialization
would break that established detached-surface contract.

`check_win_size` runs before the historical `soft_reset` integration phase;
setting soft_reset early would also alter BIOS reset behavior. Keep its narrow
exclusion. Packed width derives from the selected painter and row-byte units;
Compat must not second-guess those units. Preserve accepted DAC, cache-reset,
geometry and T88 predicate fixes. No new readiness/geometry forwarding API is
needed for this task.

### 5. Four painter includes: restore original spellings

Compat already supplies `conapi.h`, which includes the same `dib_surface.h`.
There is no competing conapi.h in the selected mirror. The original device
target already searches Compat first. Restore `"conapi.h"` in nt_cga/nt_ega/
nt_vga and `<conapi.h>` in nt_munge, deleting four five-line selection blocks
in favor of their original one line each. No new overlay or renderer is needed.

The called output primitive in these bodies is InvalidateConsoleDIBits, whose
mapping remains identical through the existing Compat header. Include smoke
and real paint receivers must still confirm no macro side effect. The separate
nt_cga BIGWIN suppression stays: the original host header defines it itself,
so compiler `-U BIGWIN` would be overwritten. Adding another header and
per-file search path just to move six lines is not worthwhile here.

### 6. DOS mouse: allocation succeeds; service installation is separate

`mouse_driver_initialisation()` creates and initializes MM_INSTANCE_DATA;
it does not hook INT33 or register BOP entries. `mouse_install1()` is a separate
guest-driver operation that patches vectors and uses the guest MOUSE.COM
protocol. Compat's BOP registration intentionally omits B8--BE; existing
bop_smoke asserts this absence. Physical InPort input/IRQ handling is separate.

The ignored diagnostic links existing accepted machine/VM objects and firmware.
It creates its own 1.44 MiB boot image, invokes actual guest INT33, and records
the vector and post-interrupt marker, both with default state and with a direct
initializer call before running. On **x64 and x86**, default initialized=0,
direct initialized=1, unchanged INT33 vector F000:6F00, B8/BD remain null,
and the guest reaches its marker. Terminate/reinitialize also succeeds.
AX after the absent-driver interrupt varies; it is not treated as a supported
service result or used as an oracle.

This is a synthetic BIOS/INT33 experiment, not a DOS installation or a full
historical MOUSE.COM acceptance. It proves the existing NIDDB storage works
and that initialization alone is insufficient. The current reset comment
overstates initialization as installation; a comment-only correction is
appropriate with final contract cleanup. Enabling the software driver would
need separate guest-visible feature admission and complete BOP/install/IRQ/
snapshot validation. This structural task preserves its exclusion and working
InPort behavior; it does not silently add a guest mouse driver.

### 7. PPI: complete post-gate state remains in the hardware owner

The original non-NT branch notifies only speaker-data edges before changing
Timer2 gate. The selected original NT branch sends full state after timer_gate.
These are not equivalent when gate/frequency changes independently of bit 1.
Keep the complete-state selection and the existing HostPpiState call. Snapshot
capture derives the speaker bit; restore notifies the same endpoint. Moving
this sequencing to Compat or restoring the edge-only path risks the accepted
audio behavior and creates duplicate PPI state.

### 8. PIC: reject the candidate's proposed move

`SWPIC_clear_int` updates private adapter_state, scans pending IRR and lowers
the CPU interrupt using the existing `host_clear_hw_int` endpoint. The new
predicate determines hardware line state, not presentation or OS policy.
Compat cannot own it without importing PIC internals or a second state copy.
Keep the bounded original-source repair. Removing its two guard lines would
reduce textual diff, but would widen its use to untested historical host
profiles; that is not necessary to improve this product's boundary.

### 9. FPU: move only the host environment definitions

Keep FP32/64/80 and FPU_I64 endian corrections in the mirror. Create one small
Compat `ccpu/cfpu_def.h` selected before host/inc: include the original header
explicitly, undefine its eight Host environment macros, then define the same
current ISO fenv operations. Restore the original NTVDM/non-NT host selection
block in the mirror. Do not copy the types, implement FP arithmetic, or emulate
the obsolete fpsetround API. This wrapper has an explicit ABI responsibility.

The existing x87_layout_smoke proves layouts only, not rounding/exceptions.
S3 must add nearest/down/up/zero, clear and raised overflow/underflow/inexact
tests and verify the preprocessed selected definitions on both architectures.
All direct consumers must select the overlay; the test currently puts the
original host include path first and must be adjusted deliberately.

### 10. Graph locking: move only the single-executor host policy

An equally small Compat nt_graph.h can include the original header, undefine
GrabMutex/RelMutex, and install the existing no-op definitions. Restore only
the original mutex macro block in the mirror. Do not duplicate the roughly
500-line header or import NTVDM handle ownership. Existing single-executor
ownership and copied-frame consumers remain the reason the no-op is valid.
Test targets and production consumers must all resolve the same overlay.

### 11. Build profile and I/O extent

Move the redundant standalone definition out of host_def.h to the existing
CMake target contracts. Keep the removal of original NTVDM and CPU_30_STYLE;
restoring those would select different machine paths. Export only the required
standalone definition from the concrete machine target to its own consumers,
while maintaining private OBJECT definitions; audit independent test targets.
No new global project flags or shared Lib/Emulator/X86 definitions.

Retain ios.h's one-line 64 KiB selection. Restoring 1 KiB aliases high ports;
defining NTVDM, GISP_SVGA or SFELLOW merely to reach that size changes other
profiles. A new override guard/profile macro would add machinery for no mirror
diff improvement. The declaration's current +1/-1 is a justified port ABI.

## Proposed executable sequence and estimates

Only S1 is admitted. These estimates exclude documents and published binaries;
each source S must replace estimates with actual end-to-end Git counts.

| Step | Production / build scope | Estimated + / - / net | Proof |
| --- | --- | --- | --- |
| S2: existing-endpoint cleanup | Six mirror files (four painters, nt_keycd, host_def), Compat keyboard/platform and deleted input.h, root CMake; keycode and static owner tests | Mirror about +1/-28, net -27. Total production about +7--12/-43--49, net -31--42. Build/test about +25--55/-5--12. | Original key table/OOB checks plus real make/break ingress; selected macro consistency; text/CGA/EGA/VGA/V7 and snapshot regression. |
| S3: narrow host-header ownership | Two mirror headers, two small Compat overlays, consumer include ordering, focused FPU/renderer tests and gate | Mirror about +1/-28, net -27. Compat about +45--65/-0; total production net +18--38. Build/test about +40--80/-2--8. | Both-width host rounding/exceptions and layouts; resolved include provenance; frame, cursor, snapshot and shutdown regression. |
| S4: final retained-port review | Ledger/design and inaccurate reset comment only; no planned hardware/audio rewrite | Runtime +0/-0; comment about +3/-6 (to be measured); no promise of additional mirror savings. | All 36 entries dispositioned; whole-T actual-change audit and dual full/background plus separately scheduled native desktop acceptance. |

S2/S3 together should reduce about 55 original-mirror added+removed lines
(approximately 27 and 28 respectively). This is a forecast, not a measured
patch or a reduction in machine functionality. Production net reduction and
test additions are separate from original-mirror delta. S3 slightly increases
total code to give host-only definitions their correct owner; it is optional
in the recommendation until the owner accepts that tradeoff. Do not import
an entire copied header to make the mirror number look better.

Retained ports are final proposed dispositions, not hidden TODOs. S2/S3/S4
admission should confirm this narrower completion standard in place of the
old candidate's unconditional standalone-branch removal.

## Verification and delivery limits

- Read all fourteen original peers, current implementations, object/include
  selection, immediate Compat endpoints and existing owner tests.
- Existing accepted binaries: eight focused tests **8/8 x64, 8/8 x86**.
  Names: irq_smoke, mouse_smoke, keycode_smoke, vga_frame_smoke,
  text_console_compat_smoke, bop_smoke, x87_layout_smoke, sound_state_smoke.
  Real times 4.30 and 7.19 seconds. This qualifies the unchanged baseline only.
- Four guest probe runs (default/init on each width) pass; probe builds reuse
  accepted original/VM objects, compile only the diagnostic main, and do not
  overwrite either package executable. First sandbox launcher produced no
  result; the hidden owned launch outside that process restriction completed.
- Logs/probe source are ignored S1 evidence. All children exit; temporary guest
  images self-delete. No Computer Use or native foreground desktop test.
- This S changes no production, test suite, build contract or artifact. Full
  regression and package rebuilds are not claimed for an audit-only delivery.
  Subsequent source steps require dual builds, shared-first background suites,
  relevant integration and reserved desktop cases, updated two EXEs and a
  pushed complete P before owner acceptance.

S1 is delivered for design review; neither S1 owner acceptance nor T89 closure
is inferred. The XP baseline-upgrade candidate remains queued separately.
