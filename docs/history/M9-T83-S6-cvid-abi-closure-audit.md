# M9 T83 S6: C-VID ABI declaration closure audit

The owner accepted S6 on 2026-09-24. Executor commit `776c02e2` is pushed to
`main`; this closure records the independent actual-change review.

## Scope and changed-path review

The requested read/write glue return/value declarations and `ega_read_dot()`
header ownership are repaired without changing a generated rule, glue body,
table order, device state, machine policy, or guest-visible behavior.

| Path | Actual change | Disposition |
| --- | --- | --- |
| `base/inc/cpu_vid.h` | Adds nine exact selected C-VID glue prototypes under the selected configuration. | Retained shared ABI owner. |
| `base/video/ega_read.c` | Removes three local read declarations. | Uses the shared ABI owner. |
| `base/video/ega_writ.c` | Removes six local write declarations, including the stale `IU8` word-fill type. | Uses the shared ABI owner. |
| `base/inc/egavideo.h` | Adds the exact `ega_read_dot(int, int, int)` contract. | Retained EGA service owner. |
| `test/app-softpc/unit/machine/vga_frame_smoke.c` | Removes two duplicate glue declarations exposed by the first build. | Uses the shared ABI owner; test behavior unchanged. |

Production code is `+18/-9` (net `+9`); focused test code is `+0/-3`; total
code is `+18/-12` (net `+6`). Documentation and refreshed permitted EXEs are
separate from that ledger. The source sweep finds each selected glue declaration
only in `cpu_vid.h`, with implementations only in `base/cvidc/ev_glue.c`;
`ega_read_dot()` has one header declaration, one `ega_vide.c` implementation,
and one `video.c` caller.

## Verification

- Release builds: x64 and x86 passed.
- Background CTest: x64 `120/120`, `304.83 s`; x86 `116/116`, `288.04 s`.
  Desktop-labelled tests were excluded by the prescribed presets.
- Documentation governance, source sweep, post-commit `git show --check`, and
  `git diff --check` passed.
- Only ignored `build/` holds logs. The user INI and media were not changed.

The first x64 build correctly failed because the newly shared declaration
exposed two exact duplicate test declarations and the historical `IPT2` empty
parameter-list expansion. The in-scope repair replaced the shared boundary
with direct standard-C prototypes and removed the duplicates; both final
builds and regressions passed.
