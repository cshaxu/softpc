# M9 T49 S2 — Strict shared-library boundaries

## Original request

Audit and repair the shared library so its standalone build passes
`-Wall -Wextra -Wpedantic -Werror`; remove accidental public standard-library
dependencies and library-owned scalar aliases that leak through implementation
code; remove product terminology; and remove Linux Curses linkage for
presenter placeholders. The standalone CMake project name
`nxvm_shared_library` is an owner-approved exception and remains unchanged.

## Objective

Make `src/lib` independently strict-buildable without changing its public
behavior or introducing application policy. Public base types own only their
necessary C type definitions. Library-owned code uses `lib_size`, `lib_u32`,
and `lib_i64` at component boundaries; native adapters retain SDK types only
where their platform calls require them. Shared prose describes neutral
application/content/input behavior.

## Boundaries

- `src/lib/**`, its standalone CMake configuration, its manifest, and
  library-focused tests/documentation may change.
- `src/app/**`, `src/host/**`, and `src/mvdm/softpc.new/**` do not change.
- This task does not implement Linux presenters. Their current explicit
  `UNSUPPORTED` placeholders remain, but may not impose Curses as a build
  dependency.
- `Threads::Threads` remains only where Linux `host-sync` actually needs
  pthread support.
- `project(nxvm_shared_library ...)` remains the sole approved product-name
  exemption in independent library CMake configuration.

## Required repairs

1. Add a lib-target-only strict-warning configuration and make its standalone
   build pass. Do not impose these flags on the preserved MVDM corpus.
2. Replace union placeholder initializers with explicit designated members;
   make Win32 wait-array bounds/type proof explicit; and retain the Window
   startup status before freeing its state.
3. Reduce `base_interface.h` to its actual public C dependencies. Add direct
   private includes at each consumer. Sweep library-owned scalar usage to the
   neutral aliases while preserving native SDK types at adapter boundaries.
4. Replace product-facing `VM`, `guest`, and `machine` prose in library C/H
   files with neutral application/content/input wording; symbols and behavior
   remain unchanged.
5. Remove Curses discovery/includes/linkage. Keep `Threads::Threads` linked
   to Linux `host-sync`, which owns pthread use.

## Verification

- Fresh standalone x64 strict lib configuration and build using exactly
  `-Wall -Wextra -Wpedantic -Werror` for library targets.
- Fresh repository x64/x86 build and applicable CTest, plus manifest and
  source-boundary checks.
- Search proves no non-exempt product wording in library C/H, no unwanted
  public base includes, no direct library-owned `size_t`/`uint32_t`/`int64_t`
  in the admitted sweep, and no Curses dependency.
- Regenerate `src/lib/MANIFEST.sha256`, run `git diff --check`, then commit
  and push each completed part.

## Exit criteria

The reusable corpus passes strict compilation, retains exact runtime semantics,
does not expose accidental implementation/platform wording through its public
contract, has no unused Linux Curses dependency, and preserves the explicit
`nxvm_shared_library` project-name exception.
