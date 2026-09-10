# M9 T50 S2 — Shared-library CI

## Objective

Make every `src/lib/**` change receive an independent Windows manifest, build,
and CTest check in GitHub Actions.

## Scope

- Add one path-scoped workflow equivalent to NXVM's shared-library gate.
- Validate lib manifest, standalone MSVC build, and standalone CTest on
  GitHub-hosted Windows.
- Keep that standalone MSVC path buildable through a private, neutral atomic
  implementation boundary; public library ABI remains unchanged.

## Non-goals

- No full VM/package CI or product/runtime behavior change. The workflow does
  not install or depend on MSYS2/MinGW; it uses the hosted Visual Studio
  toolchain.

## Exit criteria

- Workflow syntax, path scope, manifest target, build, and CTest commands are
  reviewed against the standalone lib CMake contract and pushed.
- The standalone MSVC build proves the private atomics boundary without
  exposing compiler or platform types in public headers.
