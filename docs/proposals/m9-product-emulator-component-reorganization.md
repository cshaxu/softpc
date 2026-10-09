# Product and Emulator component reorganization

## Intent

The reusable stack becomes:

```text
lib < emulator < product
```

`lib` remains the innermost platform capability layer. `emulator` is the
renamed neutral machine/UI/session layer. `product` is the outer shared
application-facing surface: command/debug policy, assembly utility and
session/UI composition.

This is a structural reorganization, not a new runtime path.  Every move uses
`git mv`; exactly one implementation remains after each move.

## S17 — establish Product

Move `src/x86/xasm32` to `src/product/xasm32`, `src/x86/debug` to
`src/product/debug`, and `src/ibmpc/product` to `src/product/surface`.
Move their test owners into `test/product`.  Rename the moved component's
internal/public symbols, target names, test registrations, manifest entries
and direct references to `product_*`, so its vocabulary matches its ownership.

After relocation, delete the now-unused `src/x86`, `src/ibmpc`, `test/x86` and
`test/ibmpc` trees, their CMake entry points and their obsolete Product build
helpers. Product's corpus gate verifies the positive dependency rule: Lib has
no outer dependency, Emulator may consume Lib only, and Product may consume
Lib or Emulator only. This S does not rename `common`.

## S19 — rename Common to Emulator

Rename `src/common` and `test/common` to `src/emulator` and `test/emulator`.
Rename the component-local `common_*` interfaces, targets, directories,
manifests and all direct consumers to `emulator_*`.  This is mechanical naming
and ownership normalization: lifecycle/session/UI semantics must not change.

## S20 — qualification without retired machine subsystems

Build and test Lib, Emulator and Product on x64/x86, then run the product
regression suite. Package builds and EXE refresh occur here only if source
integration is runnable and all required checks pass.
