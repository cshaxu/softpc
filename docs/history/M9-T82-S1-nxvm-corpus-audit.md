# M9 T82 S1: NXVM Shared-Corpus Audit

## Result

S1 froze NXVM `d485a54e` as the read-only comparison reference and audited
the six shared source/test directories. The non-Audio union contained 213
paths: 74 exact, 130 same-path differences, one SoftPC-only x86 shim, and
eight NXVM-only Base process paths/tests. Audio was explicitly excluded.

The owner then removed all discretionary retention: selected non-Audio files
must converge exactly, Base process supplies SoftPC's existing adjacent-INI
directory lookup, and the old x86 command shim must disappear. NXVM manifests
were shown stale and therefore remain derived verification data rather than
an import authority.

## Scope and disposition

S1 changed documentation only. It did not modify shared production/test code,
Audio, App, Core, Compat, VM, package configuration or media. The retained
[proposal](../proposals/m9-nxvm-shared-corpus-import.md) contains the frozen
ledger, path classifications and initial import sequence.

The owner subsequently clarified that S2 is one dependency-closed full
non-Audio import across Lib, Common, x86 and their tests. This S1 record is
the required completed predecessor for that continuation; it makes no claim
that S2 has finished or that NXVM itself is a runtime dependency.

## Verification

- Six-directory hash/path enumeration completed against NXVM `d485a54e`.
- NXVM's Lib/Common/x86 DAG verifiers and strict-C11 package declarations were
  inspected successfully.
- Stale NXVM manifest mismatches were recorded rather than copied as truth.
- Documentation governance and source-clean checks passed for S1 commit
  `df9d1cf8`.

S1 is closed. T82 remains active at S2.
