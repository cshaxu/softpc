# M9 T66 S3: snapshot ownership correction

The original S3 hypothesis was deliberately rejected after code audit. The VM
retains `captured_image` after a save because a later PAUSED save writes the
same stable checkpoint without advancing guest execution. `staged_image` must
also outlive state-file read because it is consumed by the next executor run.
Moving either object into a callback would break an accepted product contract.

## Delivered cleanup

`softpc_snapshot.entry` was different: checkpoint copied it, but production
code never read it. The executor callback already owns the live entry and the
serialized image owns the restore entry. S3 removes the redundant field and
assignment, and changes the boundary smoke to assert the actual observable
clock-stop condition instead of inspecting cache-only data.

## Actual accounting

| Scope | Files | Added | Removed | Net | Disposition |
| --- | ---: | ---: | ---: | ---: | --- |
| VM production | `snapshot.h`, `snapshot.c` | 0 | 2 | -2 | Deleted the unread checkpoint-entry cache. |
| Tests | `snapshot_boundary_smoke.c` | 0 | 1 | -1 | Asserted the real stop side effect. |
| MVDM / Compat / Common / Lib / App | none | 0 | 0 | 0 | Persistent image owners intentionally retained. |
| OpenNT MVDM mirror | none | 0 | 0 | 0 | Untouched. |

## Evidence

- Snapshot focused suites passed 4/4 for each width.
- x64 full CTest final run: 107/107 in 91.29 s.
- x86 full CTest: 107/107 in 108.19 s.
- During verification, the unrelated compact-console integration fixture
  intermittently failed once at its existing stage 14 (`ver` input after raw
  Console activation). A five-run repetition passed four then failed once;
  its standalone retry and the final full x64 run both passed. S3 does not
  alter Console, input, session or package-fixture code, so the issue is
  recorded rather than hidden or patched in this ownership-only step.
- Both fixed package EXEs were rebuilt; owner INI and media were not modified.

S3 is closed. All admitted T66 implementation stages are complete; T closure
waits for owner package validation.
