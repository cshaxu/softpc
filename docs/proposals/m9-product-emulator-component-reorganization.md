# Product and Emulator component reorganization

## Intent

The reusable stack becomes:

```text
lib < emulator ~ [x86 < ibmpc] ~ product
```

`lib` remains the innermost platform capability layer.  `emulator` is the
renamed neutral machine/UI/session layer.  The remaining `x86` and `ibmpc`
trees stay available as independent machine/hardware components, but SoftPC
will no longer use them.  `product` is the outer shared application-facing
surface: command/debug policy, assembly utility and session/UI composition.

This is a structural reorganization, not a new runtime path.  Every move uses
`git mv`; exactly one implementation remains after each move.

## S17 — establish Product

Move `src/x86/xasm32` to `src/product/xasm32`, `src/x86/debug` to
`src/product/debug`, and `src/ibmpc/product` to `src/product/surface`.
Move their test owners into `test/product`.  Rename the moved component's
internal/public symbols, target names, test registrations, manifest entries
and direct references to `product_*`, so its vocabulary matches its ownership.

The retained x86 and ibmpc components must no longer contain, build or export
the moved Product code. Product must not link either retained component.
`ibmpc/machine` may instead consume `product/debug` as its external debug
protocol. SoftPC's current integration must use Product only; it must not
start linking the retained x86 CPU/device or IBM PC board/machine
implementations. This S does not rename `common`.

## S18 — rename Common to Emulator

Rename `src/common` and `test/common` to `src/emulator` and `test/emulator`.
Rename the component-local `common_*` interfaces, targets, directories,
manifests and all direct consumers to `emulator_*`.  This is mechanical naming
and ownership normalization: lifecycle/session/UI semantics must not change.

## S19 — qualification without retired machine subsystems

Build and test Lib, Emulator and Product on x64/x86, then run the product
regression suite with the retained x86/ibmpc test trees explicitly excluded.
The retained trees remain checked in but are outside this Product-stack
qualification.  Package builds and EXE refresh occur here only if source
integration is runnable and all required checks pass.
