# M9 T57 S2: VM / Compat Relocation

Executor delivery bdc78a8 was pushed before this acceptance review.
Scope, changed-path accounting, verification and the full ownership ledger
remain in the [refactor proposal](../proposals/m9-app-vm-compat-boundaries.md).

Actual-commit review checked main acquisition/destruction and failure paths,
the relocated driver/input/debug/trace implementations, all Compat renames,
CMake include order and source lists, and boundary probes. Compat files are
byte-identical relocations except the machine lifecycle include path. Existing
Common callbacks and original machine ABI are unchanged. Main no longer
includes machine/audio internals; other app files only consume Common/Lib.
The VM adapter keeps one backend and no new execution thread. The cleanup
order still joins Common before freeing the backend. Keeping the adapter
allocation until backend destruction does not change worker ownership.

Final full x64 85/85 (53.98 s), x86 85/85 (92.27 s); final boundary rerun 2/2.
Protected Lib/Common/MVDM and shared test corpora remain byte-identical.
No claim is made about fixing historic mode-roundtrip or timing TODOs.
S2 exits pass; S3 performs the separate whole-request completion audit.
