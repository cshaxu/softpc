# M9 T50 S2 — Shared-library CI

## Objective

Make every `src/lib/**` change receive an independent Windows manifest, build,
and CTest check in GitHub Actions.

## Scope

- Add one path-scoped workflow equivalent to NXVM's shared-library gate.
- Validate lib manifest, standalone MSVC build, and standalone CTest on
  GitHub-hosted Windows.

## Non-goals

- No full VM/package CI or source/runtime change. The workflow does not
  install or depend on MSYS2/MinGW; it uses the hosted Visual Studio toolchain.

## Exit criteria

- Workflow syntax, path scope, manifest target, build, and CTest commands are
  reviewed against the standalone lib CMake contract and pushed.
