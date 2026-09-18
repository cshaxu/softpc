# M9 T66 S2: original mechanism reuse

S2 removes two snapshot-side copies of already-owned original mechanisms and
one unused Compat import. It does not relocate device state or create an
abstraction layer around original code.

## Delivered paths

- Video-controller restore now replays each Graphics Controller register by
  calling original `vga_gc_outw(EGA_GC_INDEX, value << 8 | index)`. The
  original handler selects the current register callback and restores its
  native port mapping before applying the saved value. The deleted snapshot
  switch had duplicated all nine choices.
- PIT restore has one `snapshot_timer_state_function()` validator which maps a
  saved state identity to the original state routine. Current, optional prior,
  and optional gate fields consume that result directly. This removes two
  temporary `COUNTER_UNIT` objects that existed solely to transport a function
  pointer.
- `compat/video.c` no longer declares an unused imported video-mode byte.
- `com.c` is deliberately unchanged: its early declarations precede snapshot
  helpers that call them. Removing them would require reordering functional
  code solely to reduce a textual duplicate, increasing protected-diff noise.

## Actual accounting

| Scope | Files | Added | Removed | Net | Disposition |
| --- | ---: | ---: | ---: | ---: | --- |
| MVDM production | `vga_prts.c`, `timer.c` | 33 | 67 | -34 | Reused original graphics dispatch and one PIT decoder. |
| Compat production | `video.c` | 0 | 1 | -1 | Deleted unused import. |
| Common / Lib / VM / App | none | 0 | 0 | 0 | Outside this step. |
| Tests | `checkpoint_smoke.c` | 4 | 0 | +4 | Proves invalid PIT state identity is rejected, then a valid archive restores. |

OpenNT mirror comparison changed only by deletion: `vga_prts.c` is now
`+217/-0` (from `+234/-0`), and `timer.c` is `+384/-0` (from `+401/-0`).
`com.c` remains `+137/-3` by the intentional retention above. Thus S2 removes
34 MVDM added lines without adding any protected-mirror line.

## Evidence

- Focused x64 `softpc-vga-frame-smoke` and `softpc-checkpoint-smoke`: 2/2.
- x64 package/test build completed; full CTest: 107/107 in 117.84 s.
- x86 package/test build completed; full CTest: 107/107 in 114.12 s.
- Both package EXEs were rebuilt; owner INI and media were not modified.

S2 is closed. S3 may only perform the separately admitted VM-local ownership
cleanup after a new pre-implementation ownership audit.
