# T99 S42: original EGA mouse coordinate restoration

Baseline 2c0b0646; pinned XP SP1 a2f6d7c93aa4f11efc51147566d5961c59a8ef89.
NTVDM64 incident/commit 3766b9221 is an owner-supplied lead, not imported code or
SoftPC test evidence. Original guest binaries, snapshots and media are unchanged.

## Compile and reachability

Both Ninja device targets compile base/keymouse/mouse_io.c with V7VGA,
CPU_40_STYLE, CCPU, C_VID and PROD. host_def.h defines EGG/VGG; NEC_98,
REAL_VGA, NTVDM and MOUSE_16_BIT branches remain excluded. Thus the actual
EGA_graphics_cursor_display body is selected, not replaced by a stub/#error.

The old driver's mouse_io_interrupt computes jap_mouse from BDA video mode,
Currently_emulated_video_mode and alpha_num_mode. Cursor operations reach
cursor_display -> EGA_graphics_cursor_display. However, this product's
softpc_device_bop_register_machine_services does not install the old mouse
BOPs; bop_smoke explicitly asserts that they remain NULL. Normal DOS/Win3.1
mouse input uses mouse.c InPort hardware -> PIC/guest IRQ -> guest driver.
No normal-guest reachability of the reported old-driver incident is claimed.

## Root cause and repair

Scaling occurs before intersection/exclusion checks, but restoration was
inside the eligible-draw block and reread mutable jap_mouse. Off-screen or
excluded calls retain scaled state; a callback clearing jap_mouse also skips
restoration. A false-to-true change could read uninitialized saved values.

One local BOOL captures entry scaling; the existing paired restore moves
outside drawing eligibility. No return/early exit exists between save and
restore, no new lifecycle or second rendering path is added. Drawing, clipping,
font, guest state encoding and existing #if/#error/#line boundaries are intact.

Production Git diff +7/-6, net +1. Against the same pinned XP whole-file text,
LF-normalized SequenceMatcher(autojunk=False): +687/-943 becomes +694/-949;
mirror added+deleted diff increases by 13. Existing subset exclusions are
unchanged and not re-audited or booked as functional edits. The selected-subset
and upstream hashes remain fixed; only the reviewed local manifest hash changes.

## Actual-function proof

The source-local fixture includes the actual mouse_io.c body; no production
accessor, rewritten function, copied algorithm or test-only API. Whole-program
compilation internalizes unrelated old driver entry points. It links real
machine/Compat, uses real initialized video memory, and existing PCM/MIDI mocks.
A controlled existing mark_string callback counts drawing and changes jap_mouse.
The fake media is freshly generated disposable test data, never original guest.

Cases 0-5 are: outside screen seven calls; excluded area; normal scaled draw;
unscaled draw; scaled-to-unscaled callback; unscaled-to-scaled callback.
Every call checks both original Y and height; draw cases check 16 marks,
recorded display position and actual cursor pixels. Non-draw cases check no marks.

Before repair, the same fixture produced Y and height:
200 -> 475 -> 1128 -> 2679 -> 6362 -> 15109 -> -29653 -> -4889.
Excluded state became 475; callback clearing jap_mouse left Y=19/height=475.
Normal scaled/unscaled controls passed. False-to-true undefined behavior is
not required to fail deterministically in the old code. After repair all six
cases passed; screen-outside values stayed 200 on each of seven calls.
Both architectures reproduce the old-code failure and pass all repaired cases.

## Similar-issue review and limits

Scanned jap_mouse and 19/8 transformations across base/keymouse, plus saved
coordinate values and adjacent text/EGA/VGA display/undisplay. Only this function
temporarily scales shared Y/height. Adjacent undisplay uses saved screen position;
mouse_set_text changes cursor masks intentionally, not temporarily. Interrupt
register save/restore spans the existing guest callback protocol and is not
an equivalent local clipping-exit leak. No additional matching defect confirmed.

This does not claim a full audit of every temporary state in all devices.
The controlled callback proves consistent restoration despite mode flag changes;
it does not claim a naturally occurring callback transition in this product.

## Qualification

Final cached dual builds and full regression pass: 189 background + eight
native cases per architecture, 394/394, zero failures or skips. Applicable tests include mouse_smoke's
real InPort/PIC/CCPU guest IRQ path, vga_frame renderer assertions, bop_smoke's
registration boundary, and runtime_restart_boot_smoke booting the fixed Win3.1
image and cycling DOS prompt window/fullscreen in both initial directions.
These remain distinct from forcing the retained INT 33h function in a fixture.
No manual desktop or NTVDM64 results are represented as SoftPC verification.


## Verification corrections

The first fixture invocation was rejected by machine_create because no media
was supplied; this was fixture setup, not evidence of the cursor defect. The
fixture now creates and removes its own disposable blank sector and uses the
existing audio mocks. Original-function before/after runs then reached drawing.

The first x64 full suite passed 187/189. The two failures were the new fixture's
direct memset (changed to lib_memory_set) and a raw-CRLF manifest hash (corrected
to the existing LF-normalized text convention). No gate was weakened. The
underlying cursor and real Win3.1 runtime tests passed that run; final full
qualification is recorded separately, never counting these failures as passes.


## Final review and delivery

Actual source review confirms only two local hunks: entry BOOL/condition and
restoration moved one block outward. No public eight-tree changes, input
synthesis, new BOP registration, clamp, duplicate renderer or restore state.
Test C +86/-0; root registration/mock wiring +12/-1; production +7/-6.
The two original saved values retain their original types and assignments.
Native desktop/audio/package tests pass in the active Console session.

Mirror-only compiler diagnostics remain (final x64 build 92, x86 184 warning
lines); zero diagnostics outside preserved mirror source/headers. This does
not claim the legacy mirror is warning-free. CRLF mirror and LF test/docs,
475-file mirror manifest and documentation/Types gates pass. No special
Japanese mode was driven by an original guest: that retained driver is not
registered. Real guest checks prove the existing product paths still work,
not that they reached this repaired function. No manual mouse UX acceptance
is claimed; owner acceptance remains pending.

Published artifacts:

- softpc32.exe: SHA-256 `f34862ac0d9ec827798981d05f91bcccd521dfa2aa77b1fd8b9e79a6988afe51`.
- softpc64.exe: SHA-256 `35f2a74942b616fc638efd5f6f651c1181eee69dcf675279579ba4542f7c2ef7`.

Task-owned scratch/negative binaries are removed after evidence promotion;
reusable Ninja/ccache roots remain.

## P2 owner-approved whole-file retirement

Owner request: 太好了 准入s42 整个删除本文件及其接线.
Baseline 42533c7d. This supersedes P1's repaired unused implementation within
S42; it does not reopen or renumber another S.

Call-graph review finds mouse_io.c compiled unconditionally in the devices
object target, but no old driver BOPs registered by device_bop.c. The existing
bop_smoke checks the absent registrations. Outside callers are only snapshot
capture/restore and the legacy-only checkpoint/P1 fixtures. Normal mouse
input uses mouse.c, InPort, PIC and guest handlers. No guest driver is rebuilt.

Removal units: mouse_io.c (6546 lines), dedicated mouse_instance.c (32 lines),
P1 source-local cursor fixture (86 lines), their CMake and mock wiring, legacy
checkpoint initialization tests and obsolete audit inventory entry. Capture
and restore declarations/calls are removed, with no replacement stubs.
The shared mouse_io.h stays: mouse.c uses host_deinstall_host_mouse and reset
uses HOSTMOUSEFUNCS. virtual.h/reboot is also still used by platform/reset.
Original header declarations are not executable registrations and are retained
to avoid unrelated changes to the selected mirror.

Archive compatibility remains in the existing archive owner: the 1808-byte
legacy wire slot and all subsequent field positions/widths are unchanged.
Capture writes the absent state. Archive read rejects initialized != 0 with
LIB_STATUS_UNSUPPORTED and destroys its temporary archive before publishing
an output pointer; it invokes no device restore. This deliberately rejects an
installed legacy-driver snapshot, rather than silently dropping its state.
Previously normal production snapshots have initialized == 0; no claim is
made that every externally constructed or historical snapshot is inactive.

The existing checkpoint fixture identifies the video-plane write in the real
serialized stream and edits the immediately preceding 1808-byte legacy slot.
It checks active-state rejection/null output, then inactive full-byte roundtrip
and real InPort restore/IRQ continuation. No test-only production API is added.
The old field map is unchanged, not reconstructed from native struct padding.

The selected mirror inventory is 474 files instead of 475. Removing the whole
source is subset retirement, not 6546 lines of local patch reduction. Its
previous fixed-XP +694/-949 difference and all associated local patches retire
with that file. Remaining mirror bodies and their hashes are unchanged.

Similar-issue disposition: all runtime references to the removed driver,
instance allocator and snapshot entries were scanned. Shared mirror headers
are retained for their original declarations; no live runtime caller remains.
No public eight-tree source or test is changed. No new renderer, input route,
recovery logic or product special case is introduced.

### P2 verification and actual-diff review

Cached Ninja/ccache builds published both EXEs. x64 and x86 each pass
188/188 background tests and 8/8 native tests: 392/392, zero failures/skips.
Native coverage includes five desktop and three audio tests per architecture.
Real Win3.1 restart/mode-switch integration passes (x86 53.82 seconds); this
is product regression, not a claim of a newly exercised legacy driver.
Checkpoint checks include unsupported installed-driver archives, unchanged
inactive wire roundtrip, InPort state restoration and subsequent IRQ delivery.

Both EXEs' defined symbols contain InPort mouse_init/send/inb/outb, with no
legacy installation, interrupt, snapshot or instance allocation symbols.
Mirror verification covers all 474 remaining files; source/Types/boundary,
documentation and other full-suite gates pass. Build diagnostics are exclusively
original mirror/header warnings (893 x64, 892 x86), not new shared/App warnings.
Timestamp-bearing objects rebuild and both publication timestamp gates pass.

Actual git diff --numstat from 42533c7d, excluding artifacts/docs:
production src +6/-6593, net -6587; test +13/-114, net -101;
root CMake, checker and audit script +2/-16, net -14.
Reviewer independently inspected the actual diff after executor verification:
no live driver callers, no fake restore success, no shared-component changes,
no altered media/INI/snapshots, no additional state owner or lifetime machinery.
Historical shared mirror declarations stay unchanged; they do not register or
link the retired driver. No manual mouse UX test or exhaustive guest claim.

Published P2 SHA-256:

- softpc32.exe: `19fa54a2d45f90f8ef95af49ec115411927f2ecfd6ac08bcbd19c0133b7b847b`.
- softpc64.exe: `b013d6c28ac4301fb132786508e81ae483b945748a07ad7e8e26f6a19433540f`.

Owned background job completed successfully; raw scratch is removed before
P2 delivery. Acceptance remains with the owner; T99 remains open.

## P3 owner-approved unused mirror-file retirement

Owner asks to delete unused nop.c, then approves vglob.c deletion after the
XP wiring investigation. Baseline 421b2fcb. The current generated C-VID route
is retained; no experimental direct-accessor substitution is performed.

The dual-architecture object audit covers 263 non-public production objects,
including all 228 mirror C files. Reachability starts at the non-mirror
production boundary and follows object references, including data/function
pointer relocations. Only nop.c and vglob.c have no reachable production
owner; there is no additional isolated mirror-object cycle. All 476 root
production/test C objects in each architecture have no unresolved reference
to their 77 function symbols. Final EXE symbol checks likewise identify only
these two mirror objects with no named code/data contribution. This is a
whole-translation-unit audit, not a claim that every function in retained
files executes in a guest workload.

Original XP evidence: ccpu386/vglob.c provides 76 direct field accessors and
explicitly excludes latches. The original ccpu386/sources does not compile
it; cvidc/sources lists sinit011-013 and vglfunc instead. The five-entry
ccpu386/evidgen.h and complete cvidc/evidgen.h are different interfaces.
Local 3ccd67fa initially added both paths; 757053cb selected the complete
header and added Video = C_Video, leaving direct accessors unused. The local
bootstrap comment is not proof of an identical original XP assignment; none
was found in the inspected mvdm C/H/ASM/INC sources. No claim is made about
all historical/external XP build configurations.

P3 removes the two entire files (38 + 452 lines), the nop.c c-files entry,
the explicit vglob.c CMake entry and obsolete audit inventory ownership.
All vglob.c local include/GDP patches retire with that file. The only new
retained mirror difference is the one-line c-files source selection; the
original XP and subset hash provenance are unchanged. The manifest now owns
472 retained files. CPU opcode 0x90 still directly breaks in c_main.c;
Video/C_Video and generated accessors, shared VGA storage, ABI mappings and
snapshot paths are unchanged. No new runtime or test code is needed.

P3 cached x64/x86 builds publish both EXEs and pass all tests: each has
188/188 background and 8/8 native cases, 392/392 total, no failures/skips.
Both incremental build logs contain zero warnings. This does not assert that
a clean build of all retained original sources would be warning-free.
Mirror (472 paths), documentation and the full-suite boundary gates pass.

Binary comparison with 421b2fcb finds identical executable .text and all
sections except .rdata; the changed bytes are only compiler time literals,
PE timestamps and checksums. EXE sizes are unchanged. Timestamp refresh and
publication gates pass. No runtime instructions changed by this retirement.

Actual diff: production source/manifests +0/-491, tests +0/-0; root CMake
+0/-1; audit script +1/-1. The two removed source bodies total 490 lines.
Relative to fixed XP, nop.c was identical and vglob.c had +2/-1 local lines;
those retire with the files. c-files changes from +0/-8 to +0/-9. Whole-file
retirement is not misreported as 490 lines of reduced local mirror patch.

Published SHA-256:

- softpc32.exe: `c18966063b29c06a72ce99ac52cbfc8683328dfff72d37c019a7dc3b08520dec`.
- softpc64.exe: `b35b47844e9b7c8d78df0f751eb6e619a81bd7a2a61c21647ce70f31f5bdfc74`.

Actual-change self-review confirms the original decoder NOP and generated
C-VID remain the only active implementations, with no altered shared trees,
INI, snapshot or guest media. Owned jobs completed; scratch is removed before
delivery. S42 remains awaiting owner acceptance, T99 open.

## P4 owner-approved snapshot compatibility retirement

Baseline b12bc8be. Owner explicitly requests complete cleanup without backward
compatibility. This supersedes P2's retained historical DOS mouse wire slot.
Remove its 12 constants, state structure, archive member, zero-fill, field map,
read/write calls and initialized-state rejection. The real InPort controller,
its queued timer restoration and the guest mouse driver remain unchanged.

The existing format discriminator advances from SCP2 to SCP3. The existing
header check rejects SCP2 with INVALID_ARGUMENT before reading any section or
restoring machine state. There is no compatibility reader, padding, fallback,
or migration. Existing user snapshots are untouched and must be recreated
with the new executable. New snapshots omit the former 1808-byte device slot.

The checkpoint test removes the retired wire-offset fixture and instead
substitutes the exact previous format tag, checking rejection after four bytes.
It retains current-format archive/image round trips, truncated-data rejection,
wrong-memory-size rejection and actual InPort controller restoration.

Source review: all dos_mouse/DOS_MOUSE/video_offset references in src/test
are gone; the snapshot decoder rejects a different format before allocations
and device changes. Production +4/-135 (net -131); test +4/-14 (net -10).
Mirror and public eight trees have no changes in P4. No additional runtime
path or test-only production interface is introduced.

Cached x64/x86 builds publish both EXEs. Full regression passes 392/392:
188 background and eight native tests per architecture, zero failures/skips.
The changed checkpoint tests pass on both widths. Background includes seven
integration tests per width; native includes five desktop and three audio.
Build logs have 893/892 warnings, all located in unchanged mirror sources;
no non-mirror warning occurs. This is automated regression proof, not a new
manual DOS/Win3.1/Win95 guest acceptance claim.

Published SHA-256:

- softpc32.exe: `d9c5e955bcbeac011eaf1f0ea84d81339163237512754741688bc6f08099f5d0`.
- softpc64.exe: `bd4d3ff9c795c3670751667c872eab4e58be2011e51934250863d54e244c98aa`.

Actual-change review confirms symmetric deletion from writer and reader,
header rejection before section interpretation, unchanged InPort state,
no new ownership or recovery path, and no modified media/configuration.
All four proposal units are complete. Owned jobs exited successfully; durable
evidence is promoted here and scratch is removed before delivery. S42 waits
for owner acceptance; T99 remains open.
