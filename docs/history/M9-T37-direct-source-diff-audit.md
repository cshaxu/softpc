# M9 T37 S1: Direct-Source-Diff Audit

## Outcome

Closed as an audit-only admission gate. The current comparison now targets
`src/mvdm/softpc.new/`, classifies all 47 discovered source differences, and
fails closed for an unclassified path. The retained evidence is
`docs/etc/evidence/m9-t37-direct-source-diff-audit.md`.

The audit found four build-time source transforms producing 52 selected C/H
inputs. It separates narrow ABI candidates from external compatibility owners
and three behavior blockers (`tape_io.c`, `c_main.c`, and `mouse.c`). It did
not alter machine source, CMake source selection, package files, or runtime
behavior.
