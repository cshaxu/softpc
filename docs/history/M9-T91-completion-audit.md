# M9 T91 completion audit

Owner accepted S5 and closed T91 on 2026-10-15, requesting admission of the
queued XP SoftPC core upgrade. Executor endpoint: `1f0c0f53`; coordinator
actual-change review: `087e512f`. This handoff changes documentation only.

## Scope completion

- S1 imported the reviewed glyph/font contracts and their SoftPC adapters;
  owner accepted the dual-width 137/137 qualification.
- S2 audited the next cursor contract import; owner approved its S3 execution.
- S3 imported that contract with HOST cursor adaptation; owner accepted both
  137/137 suites. Initial desktop input interference remains in its record.
- S4 separated debugger, shared PC product and NXVM-only contracts, including
  matching tests and namespaces. Owner accepted both 150/150 suites, including
  five native desktop cases per width.
- S5 removed the unused NXVM source/test pair and registrations. Both builds
  and all 138 background cases per width pass. The five native desktop cases
  were not rerun in S5; S4 evidence is prior evidence, not a fresh S5 run.

There are no deferred T91 subtasks. The original six-root import equality was
verified at each import revision; the later owner-approved split and removal
intentionally supersede that layout. Final reusable packages are Lib, Emulator,
X86 Debug and X86 IBM PC, each with source and test roots.

## Endpoint accounting

Rename-aware comparison from `cc7365a6` to `087e512f`:

| Category | Added | Deleted | Net |
| --- | ---: | ---: | ---: |
| Production C/H | 276 | 261 | +15 |
| Test C/H | 272 | 119 | +153 |
| CMake build/gates | 458 | 197 | +261 |

These are endpoint figures, not sums of intermediate edits. The preserved
SoftPC mirror and owner INI/media have no T91 endpoint changes.

Final accepted asset EXE SHA-256:

- x86: `A7652542623FDE0F4F6CB609FCB10755D2CCD8DACD7C2C95D8C12ADEA9068258`
- x64: `5DC11982BACDB80816BC186B9C1C64F0223DBA050C0BC8ED3677203E135A58BC`

S5 x64 was an incremental no-work build; x86 rebuilt after Ninja cache recovery.
Both outcomes and the absence of a forced clean rebuild remain disclosed in S5.
This closure does not rebuild accepted artifacts or claim new runtime tests.

## Coordinator closure and handoff

Reviewed S1-S5 delivery/acceptance records, actual endpoint diff categories,
clean worktree and accepted artifact hashes. The owner accepts all delivered
work. T91 closes without expanding its scope to the existing null-BOP TODO.
The sole queue candidate is separately admitted as T92 S1: immutable-source
audit and migration ledger before any XP core replacement. Documentation and
whitespace gates apply to this closure/admission handoff.
