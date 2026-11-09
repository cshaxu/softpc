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
