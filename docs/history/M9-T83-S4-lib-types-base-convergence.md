# M9 T83 S4: Lib Types and Base consumer convergence

## Owner-admitted objective

Every tracked App-SoftPC source and test consumer that should use the existing
Lib Types or Lib Base contracts must do so. The preserved original machine under
`src/app-softpc/softpc.new/` is excluded in full: it remains an independently
audited mirror and this task must not alter it.

This is a consumer-convergence task. It does not add new Types/Base API,
forwarding wrapper or portability layer merely to remove a standard-library
spelling. Where Lib has no matching public contract, the owning platform or C
I/O leaf remains direct and receives a recorded disposition.

## Frozen convergence ledger

The frozen universe is every Git-tracked `*.c` and `*.h` below
`src/app-softpc/` and `test/app-softpc/`, excluding
`src/app-softpc/softpc.new/**`. The six shared components and their tests are
outside this packet. The unit is one tracked file, measured from
`git ls-files 'src/app-softpc/**' 'test/app-softpc/**'` at S4 admission.

For each member, the ledger records exactly one disposition:

| Disposition | Predicate | Proof |
| --- | --- | --- |
| Migrated Types | It consumes scalar, pointer-width, boolean/status, memory, text or allocation vocabulary and includes/uses the matching public `lib/types` contract. | Include/call-site diff plus compile. |
| Migrated Base | It consumes an existing clock, process-directory or synchronization service and calls the matching `lib/base` interface. | Include/call-site diff plus focused test. |
| Already conforming | It already reaches the required Types/Base contract through its owning public header. | Include graph and static scan. |
| Retained owner | It performs C stream I/O, product formatting, or a platform-leaf operation for which no public Types/Base contract exists. | Exact operation and owning layer recorded in the audit. |
| Excluded mirror | It is below the preserved `softpc.new` root. | Path predicate only; no edit. |

The completion predicate is no unclassified non-mirror consumer and no direct
use of a Types/Base-covered service outside its owning Lib component. A
permanent root static check must reject a representative newly introduced
bypass. Direct C I/O is not a bypass unless Lib exposes an equivalent public
contract.

The retained port-ABI bridge headers are
`compat/ccpu/snapshot.h`, `compat/cvidc/gdp_rule_access.h`,
`compat/cvidc/gdp_state.h`, and
`compat/devices/snapshot.h`. Preserved mirror translation units include them;
changing their vocabulary or include graph would alter the excluded mirror.

## Initial evidence and planned repair

The admission scan identifies direct `<stdint.h>`, `<stddef.h>`, `<stdlib.h>`,
`<string.h>`, allocation and memory/text calls in App product, compatibility,
machine and their tests. It also identifies direct Windows clock/sleep calls in
App test leaves. S4 replaces each call with an existing Lib equivalent; native
Console verification and C stream I/O remain direct because no matching public
contract exists.

The implementation adds the static check under `tools/checks/`, registers it
from root CMake, and supplies a negative fixture. It builds the actual
`CPU_40_STYLE + C_VID` selection on both widths, runs focused consumer tests,
then runs each complete background CTest suite. The closure audit reports the
finite ledger count, retained-owner list, changed source/test accounting,
EXE hashes and any corrective P.
