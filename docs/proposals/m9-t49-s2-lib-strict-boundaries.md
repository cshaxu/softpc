# M9 T49 S2 — Strict shared-library warning contract

## Original request

Repair the shared library so its standalone build passes
`-Wall -Wextra -Wpedantic -Werror` without changing product behavior.

## Objective

Make `src/lib` independently strict-buildable, with warnings treated as
errors only for library implementation targets. This task clears the currently
reproduced initializer, Win32 wait, and lifetime diagnostics; it does not
alter public type ownership, wording, or Linux dependency policy.

## Boundaries

- The affected library implementation files, standalone CMake configuration,
  manifest, and library-focused tests/documentation may change.
- `src/app/**`, `src/host/**`, and `src/mvdm/softpc.new/**` do not change.
- The preserved MVDM corpus does not receive new warning flags.

## Required repairs

1. Add a lib-target-only strict-warning configuration and make its standalone
   build pass with exactly `-Wall -Wextra -Wpedantic -Werror` on GCC/Clang.
2. Replace union placeholder initializers with explicit designated members.
3. Repair `host/win32/sync.c`'s zero-valued `WAIT_OBJECT_0` comparison and
   make its wait-array initialization/count proof explicit.
4. Retain Window startup status in a local before freeing native state.

## Verification

- Fresh standalone x64 strict lib configuration and build using exactly
  `-Wall -Wextra -Wpedantic -Werror` for library targets.
- Fresh repository x64/x86 build and applicable CTest, plus manifest and
  source-boundary checks.
- Regenerate `src/lib/MANIFEST.sha256`, run `git diff --check`, then commit
  and push each completed part.

## Exit criteria

The reusable corpus passes strict compilation and retains exact runtime
semantics; all other shared-library hygiene changes remain separately planned.
