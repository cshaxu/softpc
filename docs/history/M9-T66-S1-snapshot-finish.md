# M9 T66 S1: snapshot finish result ordering

S1 repairs the one VM completion path that previously discarded
`softpc_snapshot_finish()`.

## Delivered path

At the checkpoint, the VM captures and writes the image, then completes the
existing snapshot clock transaction before publishing the existing state-read
result. A clock restart failure therefore replaces the pending success with its
actual status before Common receives its normal completion callback. The
callback, pause rendezvous, writer result and image disposal remain one path.

`softpc_snapshot_finish()` retains `READY` on restart failure. After the
callback returns, the existing CCPU outer-exit request prevents another guest
instruction; `vm_driver_run()` reads that retained phase and returns failure.
No VM/Common public API, event, request slot, recovery loop or second error
route was introduced.

## Actual accounting

| Scope | Files | Added | Removed | Net | Disposition |
| --- | ---: | ---: | ---: | ---: | --- |
| Production | `src/vm/driver.c` | 9 | 1 | +8 | VM-only completion ordering and retained-phase termination check. |
| Tests | none | 0 | 0 | 0 | Existing boundary injection and transaction integration tests cover the two necessary levels. |
| OpenNT MVDM mirror | none | 0 | 0 | 0 | No original-source behavior or diff changed. |

The retained `READY` phase is already the snapshot module's authoritative
failure state; using it avoids an additional driver latch. `snapshot.c` keeps
the only clock start/stop decision, Common keeps the only generic operation
completion state, and the driver only orders its own callback around them.

## Evidence

- x64 and x86 focused snapshot transaction, cross-process and boundary tests:
  3/3 each.
- `snapshot_boundary_smoke` deterministically makes clock restart fail and
  proves `LIB_STATUS_IO_ERROR` with retained READY state.
- x64 full CTest: 107/107, 108.35 s.
- x86 full CTest: 107/107, 101.90 s.
- Both Release package EXEs were rebuilt; owner INI/media were not modified.

S1 is closed. S2 may only proceed with the separately admitted A2--A4
original-mechanism reuse and no-behavior cleanup.
