# M9 T50 S2 — Shared-library CI

## Objective

Make every `src/lib/**` change receive an independent Windows manifest, build,
and CTest check in GitHub Actions.

## Scope

- Add one path-scoped workflow equivalent to NXVM's shared-library gate.
- Validate lib manifest, standalone build, and standalone CTest on Windows.

## Non-goals

- No full VM/package CI, source/runtime change, or external service dependency.

## Exit criteria

- Workflow syntax, path scope, manifest target, build, and CTest commands are
  reviewed against the standalone lib CMake contract and pushed.
