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

## Completion evidence

- `.github/workflows/lib.yml` is path-scoped to `src/lib/**` and itself. It
  uses GitHub-hosted Windows, `ilammy/msvc-dev-cmd`, CMake/Ninja, the manifest
  target, the standalone static-library build, and CTest. It has no MSYS2 or
  MinGW setup dependency.
- `.gitattributes` declares `src/lib/** text eol=lf`; the corpus was
  mechanically normalized and its manifest regenerated from LF bytes. This
  makes the manifest's byte contract hold in fresh Windows checkouts.
- `base/atomic.h` is private implementation glue. GNU/Clang continue to use
  C11 atomics; MSVC uses Interlocked operations behind the same library-owned
  names. No public interface names a compiler, Windows SDK, or atomic type.
- Local proof: manifest target, standalone library build, and 2/2 standalone
  CTest passed; representative base, host, ux-base, ux-window, and ux-console
  translation units compiled with local VS2022 Build Tools.
- Remote proof: GitHub Actions run
  [34532353567](https://github.com/cshaxu/softpc/actions/runs/34532353567)
  passed checkout, MSVC setup, manifest, standalone build, and CTest.
