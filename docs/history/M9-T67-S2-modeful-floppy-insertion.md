# M9 T67 S2 — modeful transactional floppy insertion

## Boundary and delivery

Owner required the monitor grammar `floppy insert [readonly | direct | overlay]
[image_file_path]`.  The complete delivery is pushed through
[`7f81853`](https://github.com/cshaxu/softpc/commit/7f81853), following the
implementation parts `0982938`, `c46a0da`, and `3b7d6e7`.

App parses and reports the required mode.  Common carries the existing copied
request.  VM forwards it to the single live-media boundary. Compat GFI prepares
the new medium before replacing the old drive.  No Lib or preserved-MVDM source
changed.

## Finite ledger

| Candidate | Disposition | Proof |
| --- | --- | --- |
| Path-only insertion | Removed | Command parser and Common request carry a copied mode. |
| Destructive failed insertion | Removed | GFI validates its candidate before swapping the live drive. |
| Case-normalized path | Removed | Command tests retain exact supplied path text. |
| Eject and running admission | Retained | Existing executor admission still owns those rules. |
| Lib / MVDM | Retained | No source diff. |

## Verification

- Command, Common-machine and dual-media focused coverage verified all three
  modes, invalid/missing mode rejection, eject, and failed replacement.
- Full x64/x86 suites were 107/107; both package EXEs were refreshed.

S2 is closed. T67 continued to snapshot-media restoration in S3.
