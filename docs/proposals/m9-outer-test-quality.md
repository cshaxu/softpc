# Outer test quality normalization

## Problem

The reusable `test/lib`, `test/emulator`, and `test/product` corpora now have
component-local registration, path-derived public test identities and isolated
working directories. The importing SoftPC tests under `test/app-softpc` were
relocated correctly, but retain older root-CMake registration and inconsistent
ownership details.

- `runtime_smoke` repeats Emulator Session-FIFO and Machine-input-FIFO
  verification, and directly compiles three Emulator Session implementation
  sources into an App integration executable.
- `unit/machine/cleanup.h` is consumed by both Machine unit and Integration
  tests, so its path says it is narrower than its actual App-test ownership.
- three integration targets retain a nonexistent `test/integration` include
  directory; the compiler happens to find their local header without it.
- two background tests run from the source root. One only creates temporary
  media; the other reads a checked-in image through a relative path.
- outer CTest names are flattened `softpc-*` target names rather than their
  real owner paths, and several Integration tests retain a historical `unit`
  label.
- current README text still describes retired `test/unit` and `test/support`
  paths.

## Design

Keep the existing ownership model. No fixture, mock, source implementation or
product behavior moves into a reusable public corpus.

```text
test/lib, test/emulator, test/product  reusable component tests
test/app-softpc/                       SoftPC-only tests
  cleanup.h                            App-test-wide media cleanup helper
  time.h                               App-test-wide timing helper
  unit/{product,machine}/              concrete App/Core units
  integration/                         composed SoftPC runtime/package flows
tools/checks/                          repository static gates
```

The repair removes only duplicated Emulator assertions from the SoftPC runtime
test. It does not weaken the existing real driver/executor lifecycle proof.
The integration fixture remains under `integration/`: it is used only by three
composed-runtime tests and has no reusable neutral contract.

Every non-desktop test receives a unique ignored build working directory.
`runtime_restart_boot` receives its checked-in image path as a read-only CMake
definition; `command_provider` receives no special directory or global serial
reservation once its independently named disposable files are isolated.

CTest target names stay implementation-local where useful, but public CTest
names use actual owner paths:

```text
app-softpc/unit/product/config
app-softpc/unit/machine/machine
app-softpc/integration/runtime
checks/product-boundary-negative
```

Outer labels become additive ownership metadata (`unit`, `integration`,
`checks`, `desktop`) rather than a contradictory claim that an integration
test is a unit. Existing background presets continue to filter only `desktop`,
so this does not change normal test selection.

## Finite acceptance ledger

Universe: every App/Core test source under `test/app-softpc`, every root
`add_test` registration, and every live test-layout reference in root
`README.md`, `test/README.md`, `docs/design/CODING.md` and `tools/checks`.

For each member the final disposition is one of:

1. retained in its current owner and registered exactly once;
2. moved only within `test/app-softpc` to match actual shared-helper ownership;
3. removed only when the exact assertion is already covered by the owning
   reusable Emulator suite; or
4. documented as an intentional owner-local white-box test.

Acceptance requires: no App runtime test directly compiles Emulator Session
implementation merely to repeat reusable FIFO coverage; no live CMake path
references retired `test/integration`; all non-desktop tests have private build
working directories; public CTest names match owner paths with no aliases;
labels describe the actual owner; x64/x86 focused and complete non-desktop
tests pass. No production source, public ABI, package INI, guest media or
snapshot changes are permitted.

## T86 S2 delivery awaiting owner validation

The complete ledger has the following disposition.

- `runtime_smoke` retains its live SoftPC driver/executor exercise, but no
  longer compiles Emulator Session sources or repeats the owned Session FIFO
  and Machine-input-FIFO assertions.  Existing Emulator tests remain the sole
  proof of those two contracts.
- The App-test cleanup helper moved from `unit/machine/cleanup.h` to
  `test/app-softpc/cleanup.h`, matching its Machine-unit and Integration
  consumers.  The Integration-only machine fixture remains in place because
  only composed-runtime tests consume it.
- All root App/check CTest entries now use their real owner paths and truthful
  `unit`, `integration`, or `checks` labels.  Every non-desktop test, including
  reusable public-component tests, uses an isolated
  `build/.../test-work/<CTest-name>` directory.  Checked-in boot media is
  injected as an explicit read-only path rather than obtained from the source
  working directory.
- The build-ownership gate now rejects any `softpc-*` target that compiles an
  Emulator production source directly; its negative fixture proves that rule.
  No other App white-box test was moved because each includes only the concrete
  owner-local source required for failure injection.

Tracked executable/test configuration changes are `+154/-220` (net `-66`):
root test registration `+132/-164`, App test source `+19/-55`, and the static
gate `+3/-1`.  Production source and public APIs are unchanged.  Package EXEs,
INI, guest media, and snapshots are unchanged.

Verification from the final configured trees:

- Ninja configure/build succeeded for x64 and x86.
- `ninja-test-x64`: 129/129 non-desktop tests passed in 192.20 seconds.
- `ninja-test-x86`: 129/129 non-desktop tests passed in 103.06 seconds.
- The CTest JSON listing confirms 47 App/check entries, no flattened
  `softpc-*` CTest names, no non-desktop test without a working directory, and
  no `RUN_SERIAL` property on `app-softpc/integration/command_provider_smoke`.
- Documentation governance and the expanded build-ownership negative gate pass
  on both widths.
