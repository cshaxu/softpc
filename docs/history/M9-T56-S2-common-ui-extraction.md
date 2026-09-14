# M9 T56 S2: common UI extraction

## Owner requirement

Move the production broker, cooked monitor Console, raw VM Console and
Window/KVM composition to `common/ui`, while preserving SoftPC's injected
product policy and deleting the former app-owned implementation. The owner
authorized asynchronous package testing: each S remains fully built, tested,
committed and pushed before the next S begins.

## Executor delivery

P1 extracts the implementation into `src/common/ui/ui_interface.h`, `ui.c`
and its CMake target. `common/ui` owns the logical monitor Console, broker,
raw Console and Window objects; it receives copied state/action/frame data and
returns copied events. `src/app/main.c` injects the product title, hotkey
registry and graphics Console status surface, stamps its existing run
generation, and bridges copied common events to the existing control queue.

The old `src/app/monitor.[ch]` and `src/app/presentation.[ch]` are deleted.
There is no forwarding wrapper or second production UI route. App still owns
the session/reconciler/runtime state in this S, exactly as assigned to S3/S4.

Focused boundary review found every production broker, KVM creation/destruction,
frame/title/freeze and cooked line call under `src/common/ui`; the remaining
app references are product-state names and control policy, not library object
ownership. Source-boundary and naming gates were updated to make that division
executable.

## Verification

- x64: `cmake --build --preset tests-x64`, then `ctest --preset test-x64` —
  58/58 passed.
- x86: `cmake --build --preset tests-x86`, then `ctest --preset test-x86` —
  58/58 passed.
- strict standalone `src/lib` build and CTest — 8/8 passed.
- Documentation/DAG gates and `git diff --check` passed.
- Package SHA-256: `softpc32.exe`
  `D6E4959676EDC6F73B29F2236FBC825BA4B6219079A41A0AE8E8DE0A01350468`;
  `softpc64.exe`
  `7D055D6563A6FF923981F4B1DF747CD3351E3D172B217E1EE8ED16BE4B3CA45D`.

The build refreshed only those two package binaries; user-owned
`assets/binary/softpc.ini` and all media remain unchanged.

## Reviewer closure

Reviewed the actual source/CMake/test/documentation diff against the S2
packet, the frozen S1 ledger and product UI authority. Product-visible strings
and lifecycle decisions remain injected from SoftPC; common/ui does not parse a
CLI or decide lifecycle. Old implementation paths, headers and compile inputs
are removed, so there is one production UI ownership route.

S2 is closed. S3 may extract the session reducer and inject the existing CLI
provider without reopening UI ownership.
