# M9 T56 S10: synchronous debugger machine access

## Closure

Implementation P1 `6e11d50` was committed and pushed, then reviewed in the
coordinator role against the S10 packet, actual production/test diff and
original CPU interfaces. S10 is closed; T56 remains open. S11/S12 are not
implemented and need the separate preserved-source port-ABI decision recorded
in the [proposal](../proposals/m9-common-corpus-convergence.md).

## Delivered and verified

- Original special/general register setters, exception-safe segment setters,
  effective segment/system snapshots and byte device port I/O are connected.
- Debug memory uses original external page translation followed by physical
  SAS access. Whole-request preflight rejects invalid mapping/ROM writes
  without partial mutation, guest page faults or inspection-induced A/D changes.
- Actual executor tests cover register restoration, invalid CR0/segment writes,
  port read/write/restore, mapped/unmapped/cross-page access and changed CS:EIP
  executing a disposable test program. Existing assembly/disassembly roundtrip
  and independent debug CLI tests remain enabled.
- CR1/CR4, execution plans and watchpoints are not represented as successful
  operations. LDTR reports only the cache fields the original CPU exposes.

Both full CTest suites passed 63/63; strict standalone lib passed 8/8.
Final declaration cleanup was rebuilt at both widths and both real
debug-binding focused tests passed. Documentation governance and diff checks
passed. No MVDM/lib, user configuration or media changes were made.

Production: four existing files +142/-43, net +99. Tests: one existing file
+151/-4, net +147. No new production layer, executor or library API.

SHA-256:
- x86: `2DC7CEC7B417805B9672C4D6045002351657CD3D133287C3631A0738D08FBFA7`
- x64: `C6363E7662817E38A26C9FD6D93B73EB7757D2C0F33721DF7A7413D40C411BE3`

## Review disposition

No unresolved S10 blocker found in the reviewed scope. The next-stage audit
found no complete existing instruction-retirement or operand-read notification
contract. Stop at the approved scope boundary: do not fake single-step using
timer cadence, borrow guest TF/DR, or add a second executor. Obtain explicit
port-ABI approval before modifying preserved CPU sources. No claim is made
that the complete debugger is finished or that GUI manual testing occurred.
