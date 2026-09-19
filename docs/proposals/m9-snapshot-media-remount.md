# Snapshot media remount

## Request and baseline

Owner requests that loading a snapshot uses only its recorded floppy/HDD
paths, and detaches/reopens existing media even if the same file is attached.
The preceding clarification also makes saved access modes authoritative,
independent of INI. Baseline is S7, reconfirmed in `596f36e` after withdrawal
of all subsequent uncommitted work. This record describes admitted work;
it does not claim implementation or verification.

## Minimal design

Keep the existing pathname, size, SHA-256 and overlay-page codec. Preparation
verifies each saved base without changing live attachments. A live matching
non-overlay handle may be used for this read-only verification, since direct
access can exclusively lock its file; this never permits retaining it through
restore. Once preparation succeeds, restore detaches existing attachments and
opens the saved paths in saved modes. An absent saved slot leaves no medium.
Apply saved overlay pages to the new overlay and restore controller metadata.

Remove the restore-time retained-attachment branch and the INI HDD-mode
conversion. Saved mode becomes the sole mode, allowing removal of redundant
target-mode state if the caller audit confirms no remaining purpose. VM keeps
the restored attachments for subsequent reset as it does today.

Preserve current failure boundaries: preparation failure leaves live media
alone; failure after destructive handoff is reported and does not promise
rollback. No path search or same-digest alternate-file substitution is added.

## Estimate and finite verification ledger

Estimated production scope: three files in Compat/VM, net reduction of about
20--40 lines; tests: two files, approximately 40--100 changed lines. Recount
against the baseline at delivery rather than treating this estimate as proof.

| Member | Required proof |
| --- | --- |
| Floppy and HDD path selection | Different live/INI paths still restore the snapshot paths. |
| Same-path media | Instrument detach/open ownership; pointer address inequality alone is insufficient because allocators reuse addresses. |
| Saved modes | readonly/direct/overlay survive different startup modes for both drive kinds. |
| Content validation | Size/SHA mismatch and missing saved path reject preparation. |
| Overlay and absent slots | Restore saved pages without old dirty data; absent slots detach. |
| VM integration | Cross-process load uses restored media on first initialization; source audit confirms subsequent reset retains live attachments. |
| Delivery | x86/x64 focused/full regression, refreshed EXEs, changed-path audit and pushed implementation. |

## Pre-implementation audit

Confirmed gaps: `media_target_mode` overrides the saved HDD mode with INI;
`softpc_media_archive_restore` retains matching non-overlay handles. Snapshot
path serialization and selection already satisfy the request. Remove both
branches and redundant target-mode storage. Keep read-only preflight borrowing
of matching handles, required for exclusively open direct media. Restore must
detach all slots before reopening any, so an old slot cannot lock another
slot's saved path. The same-path helper remains solely a verification aid.

The finite sweep covers archive codec, prepare/attachment/restore, VM's one
prepare caller, and floppy/HDD restore endpoints. Endpoint no-replacement
support remains for controller metadata updates used by existing tests; the
archive will always install a newly opened medium. No second restore route is
introduced. Expected production reduction is 20--50 lines across three files;
test changes are about 60--120 lines across two files.

S8 implementation will be delivered for owner testing before closure. S7
evidence is not counted as S8 verification. Lib/Common and MVDM are outside
this change.

## Delivery evidence

Implementation removes the HDD policy argument, mode conversion helper,
target-mode field and same-path retention branch. Read-only preparation may
borrow a matching non-overlay handle from any live slot, so exchanged direct
paths can be verified without lock conflicts. Restore then retires every slot
before opening any saved source. One preparation caller remains in VM machine;
one restore caller remains in VM snapshot_image. Codec is unchanged.

Finite sweep command: `rg -n 'media_archive_prepare|media_same_path|target_mode|media_archive_restore' src/compat src/vm`.
Only preparation uses `media_same_path`; `target_mode` is absent. Endpoint
metadata-only calls remain valid independently of archive restore. VM reset
attaches configured sources only under `!hardware_initialized`; preparation
copies snapshot paths/modes before first initialization. Lib/Common/MVDM and
user INI/media have zero diff.

Focused media/transaction/cross-process tests pass 3/3 on each width. Tests
cover both drive kinds across all nine saved/live mode combinations, exactly
one detach/install per same-path slot, swapped exclusive direct paths, saved
path/size/SHA rejection, absent slots, stale overlay removal and saved overlay
base preservation. Cross-process load starts with different configured paths
and readonly/direct modes, then verifies restored overlay contents/modes.
The final added missing-path/size assertions were rebuilt and rerun on both
widths after the broad run had passed the earlier version of that same test.

Both complete builds passed. x64 full CTest: 106/108. x86 non-integration:
105/105; integration: 1/3, for the same 106/108 aggregate. On each width,
`softpc-package-smoke` and `softpc-package-compact-console` fail at stage 16
in the Window restart route after Machine started. That route performs no
snapshot load. S7 disclosed the same test names failing; this is not proof
of their root cause and no full-suite success is claimed. The unchanged
legacy sources also emit existing compiler warnings; no warning-free claim.
Documentation gate and `git diff --check` pass.

Accounting against `596f36e`, using `git diff --numstat 596f36e -- src test`:
three production files +39/-81, net -42; two test files +74/-42, net +32;
combined net -10. No new production object, state machine, file or public API.
The existing Compat archive remains the sole restore owner.

Package SHA-256:

- x86: `84D9AAF5631A325DD2FA0F9C97EEE3358D186C4AAF0CBD450DB251E603465E30`
- x64: `D5B7F2FB48436934CE0191F9AE7F492E49E5C2D8BD7D7AEBC4A422567C7B478D`

Executor self-review compared all five code/test paths to the original request
and confirmed every ledger member has the proof above. Test media was created
only under the build working directories and removed by successful tests.
This is a P delivery for manual testing, not S8 closure. Owner acceptance and
post-push coordinator review remain required before closure.
