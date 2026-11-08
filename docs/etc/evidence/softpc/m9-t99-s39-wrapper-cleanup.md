# T99 S39 wrapper cleanup

## Scope and actual change

Owner authorizes implementation and closure after build/publication/test/review.
Implementation baseline: `4524002b`. Remove only the three locally introduced
SOFTPC_STANDALONE wrapper pairs in host/src/nt_cga.c, base/system/ica.c and
base/keymouse/ppi.c. Preserve #undef BIGWIN, master-INTR withdrawal and the
snapshot HostPpiState call, respectively. No original XP conditional, #line,
#error body, runtime statement, interface or state changes.

Production **+0/-6, net -6**, three files; tests **0/0/net 0**, no new tests
or framework. Existing tests exercise the preserved behavior. The current
manifest changes only three local hashes; fixed raw/subset hashes stay intact.
Normalized-LF SequenceMatcher(autojunk=False) against fixed XP SP1 confirms
local additions decrease by two per file while deletions stay unchanged:
nt_cga +91 to +89, ica +319 to +317, ppi +51 to +49. This is six fewer
mirror-diff lines, not removed device logic. Excluded contents were not audited.

## Compile and source proof

Ninja compdb identifies one production compile entry per changed file per
architecture. Every entry defines SOFTPC_STANDALONE=1; repository searches
find no test-private inclusion or separate compilation of these three files.
Tests consume the existing device object group. For each actual command,
remove object/dependency output flags and preprocess with -E -P before/after.
All six outputs are byte-identical; no token normalization hides differences.
The corresponding preprocessed SHA-256 values (same before/after) are:

| Architecture | File | SHA-256 |
| --- | --- | --- |
| x64 | host/src/nt_cga.c | bd056056d71d42f1b4e14da8cce132580021c835d4f210bfd4b56ffd444dcd6c |
| x64 | base/system/ica.c | 5f8eebf9dd77f9ef51eec143aa0bdf90a8a8fae0e3fcd5c6899c2265a378f93c |
| x64 | base/keymouse/ppi.c | e9db9822c353948fb696cb957745ce5825588a6b7c51f83b58309bbbc0be27e6 |
| x86 | host/src/nt_cga.c | fecbee1d15f1833e035edc0a579cff573e275a2aab7a7b31a3c6ec0176208bd5 |
| x86 | base/system/ica.c | ebfb627086e65d88671cb3dd8f872ce0743b50870ffe8142899350a8815a8cfb |
| x86 | base/keymouse/ppi.c | 9e079ae78262b5a65059d99f92d105c9b0aed06b5e1d4e380b8c8d732e36d1ca |

## Build, publication and full regression

Both cached Ninja/ccache release builds complete and refresh assets/binary.
No clean/rebuild loop or second build tree was introduced. Tests run through
the background presets, then serial native selections; no Computer Use.

| Architecture | Selection | Passed | Elapsed |
| --- | --- | --- | --- |
| x64 | background | 187/187 | 90.23 s |
| x64 | native | 8/8 | 24.16 s |
| x86 | background | 187/187 | 80.26 s |
| x86 | native | 8/8 | 24.41 s |

Total 390/390, no failed, disabled or skipped cases. The native selection
includes real PCM/MIDI, Window retirement, Console broker display and both
package Console layouts. This is automated regression evidence, not a new
manual Win3.1/Win95 acceptance session. Each rebuild reports 89 warning lines,
all within the preserved mirror; zero non-mirror warning lines. No new warning
policy or unrelated mirror cleanup was introduced.

Both new EXEs have identical PE section bytes to their previous publications.
The only changed bytes are COFF timestamp and PE checksum; sizes match.
Re-link publication is therefore not a guest behavior change.

| Artifact | Published SHA-256 |
| --- | --- |
| softpc32.exe | a295d86ef5ad85f1f8b7dece4370c21e53a0342a4c51a0e52734731c1c69b563 |
| softpc64.exe | 93b2a9ffae6756fae1144949b65a80f2ce2a81536eefd8e33416c31b81956634 |

## Quality review and boundaries

Executor reviewed the actual three source diffs: six directives removed,
unchanged bodies and diagnostic directives, mirror CRLF preserved. Shared
eight trees, tests, App/Compat code, user INI, media and snapshots are unchanged.
The local branch-removal candidate set is 3/3 resolved. The original NTVDM
alternatives, ABI selection and already-#error branches are explicitly retained.
No repeated runtime path, product test, allocation or synchronization appears.

The original S39 [bounded audit](m9-t99-s39-mirror-necessity.md) and inventory
remain frozen historical evidence. This implementation does not certify every
hunk of its 110 mixed residual files minimal or necessary, and creates no
implicit authorization to delete them. That limitation is not a failed test
or an unresolved defect in these three wrapper removals.

Documentation, 475-body XP manifest and whitespace gates pass. Owned scripts
finish before evidence promotion and scratch cleanup; reusable caches remain.
Coordinator review and owner-authorized S closure follow the implementation
commit as a separate governance delivery. T99 itself remains open.
