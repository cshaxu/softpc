# M8 T13: Test Boundary And Artifact-Backed Integration

## Objective

Make every test belong to one unambiguous tier and enforce its permitted input
boundary.

```text
test/
  unit/                 # self-contained tests and test-only fixtures
  integration/          # artifact-backed executable acceptance tests only
  support/              # test-only shared helpers and CMake checks
```

`test/integration/` is the only integration-test directory.

## Required Boundaries

### Unit tests

Unit tests cover standalone `host` and `app` behavior first. Existing coverage
of original-mirror code may remain, but is not a reason to retain an invalid
input dependency.

- Source input is limited to `src/` and `test/`.
- They may generate disposable bytes/files in ignored `build/`, but may not
  read `artifacts/`, absolute local paths, installed tools' data, or other
  external files.
- They must not require a packaged EXE, adjacent production INI, ROM file, or
  guest disk image.
- CTest labels must include `unit`.

### Integration tests

Integration tests exercise the real runnable package, not a private machine
fixture.

- They launch only `artifacts/binary/softpc32.exe` or
  `artifacts/binary/softpc64.exe` with no command-line configuration.
- Configuration is loaded solely from that executable's adjacent
  `artifacts/binary/softpc.ini`.
- Guest media is loaded only through paths resolved from `artifacts/media/`.
- If firmware must be external rather than embedded in the package, it is
  loaded only from `artifacts/roms/`.
- Integration runs use the configured non-mutating media mode unless an
  admitted subtask explicitly permits disposable test output beneath `build/`.
- CTest labels must include `integration` and the host-width label (`x86` or
  `x64`) when run in that configured tree.

No test may silently combine the two contracts.

## Current-Gap Audit

The existing repository does not meet this layout yet:

- all test sources currently live under `tests/`, not `test/`;
- many registered `*-smoke` targets link the whole `softpc-machine` and embed
  `softpc_firmware.rc`, so their tier has not been classified;
- two command-line boot diagnosis probes have neither valid tier and need an
  explicit diagnostic disposition;
- the selected ROM inputs are currently kept in an artifact directory despite
  being embedded build inputs rather than runtime artifacts.

## Scope

1. Move all test source and support files from `tests/` to `test/`, without
   changing tested behavior.
2. Produce an inventory assigning every current target one disposition:
   `unit`, `integration`, split, superseded, or remove. A test that needs the
   original machine/ROM/media path is integration unless it can be made truly
   self-contained without weakening its assertion.
3. Add CMake/CTest labels and separate unit/integration entry points. The
   artifact-backed integration runner must reject external image overrides and
   verify the resolved INI/media/ROM roots before launching.
4. Move or replace test-only fixtures so unit tests have no artifact input.
   Do not delete useful coverage merely because it currently has the wrong
   directory or tier.
5. Keep embedded selected ROM inputs at their original mirror path
   `src/mvdm/softpc.new/roms/`. Establish `artifacts/roms/` only if a future
   package requires external ROM inputs; do not manufacture a redundant copy.
6. Update documentation and source-boundary checks so future tests cannot
   reintroduce `tests/`, artifact reads in unit tests, or a private integration
   configuration.

## Verification

- A static test-boundary check proves every unit target's declared source and
  runtime inputs are confined to `src/`, `test/`, and ignored `build/` output.
- `ctest -L unit` succeeds from clean x64 and x86 GCC build trees with no
  `artifacts/` dependency.
- `ctest -L integration` launches each architecture's package from
  `artifacts/binary/`, observes the adjacent INI, and proves its resolved media
  paths stay under `artifacts/media/`; external-ROM access, if present, stays
  under `artifacts/roms/`.
- Similar-issue sweep covers CMake, resource scripts, helper headers, test
  working directories, direct file APIs, environment variables, and all
  command-line media options.

## Stop Conditions

Stop for owner direction if a test needs to mutate supplied artifact media,
needs a host-local path outside the declared roots, or exposes a testability
gap that can only be addressed by adding DOS/NTVDM/product behavior. Do not
weaken a machine assertion simply to force it into the unit tier.

## Exit Criteria

All tests are below `test/`; every test has exactly one tier; unit tests are
self-contained; and integration tests use only the fixed package, adjacent
INI, declared artifact media, and optional declared ROM root on both x86 and
x64.
