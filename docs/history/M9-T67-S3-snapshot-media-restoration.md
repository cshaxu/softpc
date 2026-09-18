# M9 T67 S3 — snapshot media independent of startup configuration

## Boundary and delivery

Owner required snapshot floppy restoration to use its saved path and mode, and
fixed-disk restoration to use its saved path with conversion through the
current fixed-disk mode policy. The complete delivery is pushed at
[`d2f969f`](https://github.com/cshaxu/softpc/commit/d2f969f).

Compat serializes each present media path and overlay pages in the existing
untagged binary stream. It derives the fixed-disk target mode in one place.
VM supplies the current fixed-disk policy. No Common, Lib, App grammar, or
preserved-MVDM source changed.

## Finite ledger

| Candidate | Disposition | Proof |
| --- | --- | --- |
| Snapshot path comparison to startup INI | Removed | Archive restores copied snapshot paths. |
| Floppy startup mode policy during restore | Removed | Floppy selects its saved mode. |
| Fixed readonly/direct conversion | Implemented | Archive mode matrix tests. |
| Overlay to readonly fixed disk | Rejected | Explicit invalid conversion test. |
| Overlay to direct fixed disk | Implemented | Saved pages materialize into the direct medium. |
| Binary discriminator/version | Not added | Fixed field order remains width-independent and untagged. |

## Verification

- Media archive and snapshot transaction tests covered the approved conversion
  matrix on x64/x86.
- Both full suites were 107/107 and package EXEs were refreshed.

S3 is closed as delivered. Its subsequent preparation/replacement defects are
the owner-reopened T67 S4 boundary.
