# M9 T83 S3 C-VID Closure Audit

## Delivered boundary

S3 selects the existing `base/cvidc/evidgen.h` header as the direct C-VID
declaration owner for `setReadPointers(IUH)` and `setWritePointers(void)`.
`base/video/cga.c` now defines its two BIOS channel-2 write pointers with the
same two-`ULONG` signature already declared by `base/inc/video.h`.

The direct OpenNT comparison has exactly the two required `cga.c` declaration
differences. The two header declarations are a local port-ABI correction: the
read-only OpenNT source retains the historical implicit-declaration form.
No handler, callback assignment, read/write algorithm, configuration, media,
or runtime branch changed.

## Self-review and similar-issue sweep

The review inspected every occurrence of `setReadPointers`,
`setWritePointers`, `bios_ch2_byte_wrt_fn`, and `bios_ch2_word_wrt_fn` under
the selected recovered source tree. `evidgen.h` is the one C-VID declaration
site; `video.h` remains the sole owner of the two pointer declarations;
`ev_glue.c` remains their sole selector implementation; and all assignments
continue to target functions with the existing two-`ULONG` ABI.

The source delivery contains four tracked paths: the two admitted source paths
and the rebuilt `softpc32.exe`/`softpc64.exe`. Text accounting is production
+5/-2 (net +3): three header declaration lines and two replacement definition
lines. The executables are admitted package evidence. `softpc.ini`, snapshots,
ROMs, and guest media are unchanged.

The closure audit found one in-scope governance gap: once S3 finished, the
completed three-mirror candidate could no longer remain under
`docs/proposals/`, where governance requires every proposal to be active or
queued. P2 moves that completed proposal to `docs/history/`, replaces the
active packet with T83's owner-waiting state, and re-runs documentation
governance. This is documentation ownership repair only; it changes no source
or package payload.

## Verification

- x64 and x86 Release test builds completed after the declaration change.
- Focused VGA-frame, text-console compatibility, and documentation-governance
  CTest coverage passed 3/3 on each width.
- Full background CTest passed 116/116 on x64 (261.15 s) and 116/116 on x86
  (254.69 s).
- `git diff --check` and documentation governance passed.

S3 is delivered for owner acceptance. T83 remains open; this audit does not
claim T-level closure.
