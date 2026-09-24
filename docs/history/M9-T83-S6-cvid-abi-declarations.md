# M9 T83 S6: C-VID ABI declaration audit

## Owner request and bounded objective

The owner admitted T83 S6 to examine three reported declaration defects before
implementation: C-VID read-glue return declarations in `ega_read.c`, the
`fill_word_ev_glue` value width in `ega_writ.c`, and the missing public
`ega_read_dot()` declaration. The bounded repair centralizes selected C-VID
glue declarations in the existing `base/inc/cpu_vid.h` ABI header and places
the EGA service declaration in `base/inc/egavideo.h`.

## Read-only findings

SoftPC `b1d5ece6` has the following selected `CPU_40_STYLE + C_VID` defects:

| Consumer | Current declaration | Definition/table contract | Disposition |
| --- | --- | --- | --- |
| `base/video/ega_read.c` | `extern read_byte_ev_glue...`, `extern read_word_ev_glue...`, and `extern read_str_fwd_ev_glue...` omit explicit return types. | `base/cvidc/ev_glue.c` defines the first two as `IU32` and the string reader as `void`; `READ_POINTERS` in `cpu_vid.h` already requires those exact signatures for the selected configuration. | Confirmed defect. Move declarations to `cpu_vid.h`, include it in the consumer, and remove the three local externs. |
| `base/video/ega_writ.c` | `fill_word_ev_glue` declares its value as `IU8`. | The `ev_glue.c` definition and selected `MEM_HANDLERS.w_fill` both use `IU16`. | Confirmed defect. Declare all selected write glue functions in `cpu_vid.h` and remove their local declarations from the table consumer. |
| `base/video/video.c` | Calls `ega_read_dot(getBH(), getCX(), getDX())` with no public declaration. | `base/video/ega_vide.c` defines `void ega_read_dot(int page, int col, int row)`; both source files use `egavideo.h`. | Confirmed defect. Add the exact declaration to `egavideo.h`; do not keep a `video.c` local extern. |

`cpu_vid.h` is the correct C-VID boundary: it already declares the selected
read table as `IU32/IU32/void` under `CPU_40_STYLE + C_VID`, owns the C-VID
write-table types via `MEM_HANDLERS`, and is included by `ev_glue.c` and the
write consumer. `egavideo.h` is the correct EGA API boundary because it is
included by both `video.c` and the `ega_vide.c` implementation.

The read-only NTVDM64 worktree has precise local fixes in the three consumers:
explicit read return types, `IU16` word fill, and a `video.c` local
`ega_read_dot()` declaration. They corroborate the contracts but are not the
final header ownership: after SoftPC completes the shared-header repair,
NTVDM64 should adopt the resulting source patch verbatim in a separate task.

## Finite sweep and scope

The sweep universe is every C/H path in `src/app-softpc/softpc.new` containing
one of `read_byte_ev_glue`, `read_word_ev_glue`, `read_str_fwd_ev_glue`,
`write_byte_ev_glue`, `write_word_ev_glue`, `fill_byte_ev_glue`,
`fill_word_ev_glue`, `move_byte_fwd_ev_glue`, `move_word_fwd_ev_glue`, or
`ega_read_dot`.

The present selected definitions are all in `base/cvidc/ev_glue.c`; table
consumers are `base/video/ega_read.c` and `base/video/ega_writ.c`; the EGA
service definition is `base/video/ega_vide.c` and the sole caller is
`base/video/video.c`. No second production declaration or consumer was found
in the selected SoftPC tree. Alternative EVID source is reference-only and
must not be changed by S6.

The first x64 build found two exact duplicate write declarations in
`test/app-softpc/unit/machine/vga_frame_smoke.c`. It also proved that this
configuration expands historical `IPT2` to an empty parameter list, so the
shared boundary must use direct standard-C prototypes rather than `IPT` macros.
S6 therefore removes the two test duplicates and uses direct prototypes in
`cpu_vid.h`; this is a required same-contract extension, not a test-behavior
change.

## Planned evidence

After owner confirmation, S6 will retain a source-visible ABI marker, run the
finite sweep, build both Release widths, run the prescribed background CTest
presets, verify documentation governance and diff hygiene, refresh the two
package EXEs only, commit, push, and record the NTVDM64 exact-adoption handoff.

## Executor evidence

The implementation declares all nine selected read/write glue contracts once
in `base/inc/cpu_vid.h` using direct standard-C prototypes. `ega_read.c` and
`ega_writ.c` now consume that header declaration. `base/inc/egavideo.h` owns
the one `ega_read_dot()` declaration used by `video.c` and defined by
`ega_vide.c`. The focused test's two redundant write declarations were removed
after the first x64 build exposed the historical `IPT2` empty-list expansion.

The final finite sweep has one shared declaration for every glue function, one
`egavideo.h` declaration, one `ega_vide.c` definition, and one `video.c`
caller; it finds no targeted local declaration. Production paths are
`+18/-9` (net `+9`); the focused test is `+0/-3`; combined code is `+18/-12`
(net `+6`).

Both Release builds pass. Background CTest passes x64 `120/120` in `304.83 s`
and x86 `116/116` in `288.04 s`; desktop-labeled tests remain excluded by the
presets. Documentation governance and `git diff --check` pass. Temporary logs
are under `build/`; only the two permitted package EXEs changed.
