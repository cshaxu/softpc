# M9 T54 S1 — Console Mouse Scale Closure

## Closure decision

T54 is closed. It withdraws T53's equal-axis Console conversion and restores
the original relative raw-Console ingress: one Console cell delta enters the
original InPort as eight horizontal or sixteen vertical mickeys. The owner
accepted that this is the balanced, product-neutral physical-input contract.

The closed task does not claim a strict host-Console-cell to guest-cursor-cell
mapping. The standalone VM intentionally exposes only the original InPort;
the fixed guest `MOUSE.COM` owns sensitivity, acceleration, and cursor
mapping. No shared library or application-specific driver calibration remains.

## Final retained paths

- `src/lib/ui-console/win32/component.c` restores the raw Console conversion
  from T53's `Y×8` to `Y×16`.
- `src/app/input_queue.c` no longer merges adjacent mouse records. This keeps
  application input records copied and ordered; the original InPort remains
  the sole owner of hardware-level accumulation.
- `test/unit/input_queue_mouse_fifo_smoke.c` proves two same-button mouse
  records remain two queue records.
- `test/unit/mouse_smoke.c` proves the original InPort accumulates `(3,4)`
  and `(5,6)` before guest HOLD into the exact latched `(8,10)` record.
- `CMakeLists.txt`, `src/lib/MANIFEST.sha256`, records, and the two agent-owned
  package executables contain only the corresponding build, manifest, and
  evidence updates. T53's one-axis scale smoke was removed.

P2/P3's guest packet splitting and vertical scaling were fully removed by P4;
they have no remaining production or test path.

## Verification and audit

- P4 rebuilt the fixed x64 and x86 packages and ran complete CTest successfully:
  37/37 at each width.
- P5 ran the added original-InPort accumulation smoke successfully at x64 and
  x86; documentation governance and `git diff --check` passed.
- Library manifest verification passed. No `src/mvdm/`, guest-media, or
  user-owned `assets/binary/softpc.ini` path changed.
- The similar-issue sweep inspected app-side mouse coalescing. The only
  retained coalescing is private to the UI Window native-message implementation;
  no app queue coalescing remains.

## Change accounting

Relative to T53 (`87df10f`), retained production/configuration paths are
`CMakeLists.txt`, `src/app/input_queue.c`,
`src/lib/ui-console/win32/component.c`, and `src/lib/MANIFEST.sha256`.
Retained test paths are `test/unit/input_queue_mouse_fifo_smoke.c` and
`test/unit/mouse_smoke.c`; T53's
`test/unit/ui_console_mouse_scale_smoke.c` is removed. Documentation and
package executables are excluded from source-line accounting. Executor
deliveries were P1 `0c95397`, P4 `483342d`, and P5 `496f67c`.
