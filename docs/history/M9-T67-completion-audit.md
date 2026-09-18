# M9 T67 completion audit

Owner admission: split floppy and hard-disk access policy.  Owner reopened S4
after the delivered snapshot-media path had defects, explicitly accepting that
a failed new snapshot mount may leave its previous slot detached.  Owner final
instruction: “好，收口T任务。”

## Finite closure ledger

Universe: independent startup modes, explicit floppy insertion, and snapshot
media restoration as defined in
[the retained proposal](m9-independent-disk-modes.md).  Each item closes
only through code, bounded proof, or an explicit retained policy.

| Requirement | Final disposition | Evidence |
| --- | --- | --- |
| Independent startup policy | Complete | `floppy_mode` and `hard_disk_mode` are separately parsed, copied and attached; legacy `media_mode` is rejected. |
| Explicit removable-media mode | Complete | `floppy insert <readonly|direct|overlay> <path>` carries one explicit request through App, Common, VM and GFI. |
| Failed interactive insert | Complete | GFI validates the candidate before replacing the live floppy. |
| Snapshot path and mode policy | Complete | Floppy restores its copied attachment; fixed disk derives its target once from saved mode plus current hard-disk policy. |
| Fixed-disk conversion matrix | Complete | readonly/direct interchange, overlay reconstruction, overlay-to-direct materialization and overlay-to-readonly rejection are covered by media archive tests. |
| S4 direct preflight | Complete | Preparation only verifies a temporary readonly base and closes it before direct target opening. |
| S4 overlay cleanliness | Complete | Every overlay target is rebuilt; post-capture dirty pages cannot survive a restore. |
| Fresh snapshot load | Complete | VM copies prepared snapshot attachments before initial hardware reset, avoiding INI attachment leakage. |
| Failed new snapshot mount | Retained owner policy | Restore detaches before new native opening; a failure can leave that slot detached, with no rollback state. |
| Lib, Common, MVDM and snapshot container | Preserved | No source change; the binary stream remains untagged and width-independent. |

## Actual accounting

| Step | Production delta | Test delta | Protected OpenNT mirror delta | Result |
| --- | ---: | ---: | ---: | --- |
| S1 | +53/-29 (net +24) App/VM/Compat | +69/-28 (net +41) | 0 | Independent startup policies. |
| S2 | recorded in four P commits | command/Common/VM coverage | 0 | Modeful staged floppy insertion. |
| S3 | recorded at `d2f969f` | media/snapshot conversion coverage | 0 | Copied snapshot media paths and conversion matrix. |
| S4 | +112/-66 (net +46) VM/Compat | +66/-9 (net +57) | 0 | Pure preparation, clean restoration and fresh-machine attachment correction. |

S4 accounting uses `git diff --numstat d2f969f..cb5c05f` for tracked source and
tests, excluding governance records and package artifacts.  The added code is
limited to necessary private attachment copies and the single prepare/mutate
split; obsolete retained-candidate and deferred-open state was removed.

## Verification and delivery

- S4 focused media, checkpoint, transaction and cross-process tests passed
  5/5 on x64 and x86.
- Final x64 full suite: 107/107 in 91.91 seconds.
- Final x86 full suite: 107/107 in 96.96 seconds.  One prior full run had an
  unrelated `softpc-command-provider-smoke` timing failure; a standalone
  retry passed, and the final full run passed without source changes.
- Documentation governance and cached-diff checks passed.
- Both package EXEs were refreshed. The owner’s current `softpc.ini` edit
  ships with this closure; no agent rewrote it and no guest media changed.

T67 is closed. The ordered candidates remain in
[Queue](../states/QUEUE.md); this record does not admit new work.
