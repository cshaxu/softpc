# Ninja build and test acceleration

## Intent

Keep the existing MinGW Makefiles package route as the conservative release
path, while adding a separately rooted Ninja developer route.  Both routes use
the same compiler, Release flags and CMake target graph; neither changes a
runtime, ABI, package layout or user configuration.

The developer route uses a bounded parallelism level of eight.  Ninja is an
incremental build graph, not a cross-directory compiler cache: each build tree
keeps its own object/dependency state and is never mixed with a Makefiles tree.

## S18 — Ninja developer route and safe background parallelism

If the first parallel run exposes a pre-existing test-only corpus record still
referring to retired component ownership, repair that record in the same S;
do not serialize the suite merely to hide the stale gate.

Add isolated Ninja x64/x86 configure, build and background-test presets below
`build/ninja-*`.  Preserve the existing Makefile presets and package output
contract.  Use the same compilers and flags as the corresponding release
presets.

Audit every background App test that writes a relative artifact.  Give each
such test its own disposable build-tree working directory before enabling the
bounded parallel CTest preset.  Tests that deliberately consume repository
inputs retain their required read-only source working directory; native desktop
tests stay excluded and serial.  No test is removed, weakened or made
background-safe by changing product behavior.

Prove both Ninja routes configure, incrementally rebuild, run the background
suite, and retain the existing package route.  Record timings for the serial
baseline and bounded parallel route as evidence, not as a permanent benchmark
claim.

## S19 — rename Common to Emulator

Rename `src/common` and `test/common` to `src/emulator` and `test/emulator`.
Rename the component-local `common_*` interfaces, targets, directories,
manifests and all direct consumers to `emulator_*`.  This is mechanical naming
and ownership normalization: lifecycle/session/UI semantics must not change.

## S20 — qualification without retired machine subsystems

Build and test Lib, Emulator and Product on x64/x86, then run the product
regression suite. Package builds and EXE refresh occur here only if source
integration is runnable and all required checks pass.
