# M9 T85 S16 — shared Product ownership closure

The owner directs removal of NXVM-private configuration/startup composition
from SoftPC's shared IBM PC Product corpus and restoration of deleted SoftPC
shutdown coverage.

## Delivered boundary

- Removed `src/ibmpc/nxvm/**` (eight paths), its adapter target and all build
  references.  This code parses NXVM configuration and composes NXVM machine
  state; SoftPC neither linked nor used it.
- Removed its paired `test/ibmpc/product/ini_smoke.c` and
  `composed_machine_smoke.c`.  They prove NXVM INI semantics rather than common
  IBM PC Product behavior.
- Restored the five shared `vm_app_destroy()` failure/retry cases to
  `test/ibmpc/product/composition_smoke.c`.
- Restored App-owned presentation-shutdown integration coverage under
  `test/app-softpc/integration/`; it uses the current public Product entry and
  actual App composition, rather than including retired composition source.

The only deliberate SoftPC/NXVM shared-corpus divergence is the deleted NXVM
private subtree and tests.  NXVM must move that ownership to its App before
the corpora can become exact again.

## Verification

- x64/x86 Release package builds completed and refreshed the two package EXEs.
- Focused composition and presentation-shutdown proofs: 2/2 on x64, 2/2 on
  x86.
- x64 IBM PC suite: 174/174.
- IBM PC manifest and dependency gates: pass on both widths.

The owner redirected the task to a new component-layout S before the x86
whole-suite/background phase completed.  That phase is intentionally not
represented as passing.  No production behavior, public API, Core mirror,
configuration grammar, guest media or snapshot changed.
