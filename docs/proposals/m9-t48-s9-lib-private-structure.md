# M9 T48 S9 — Shared-lib private structure

## Original request

Audit the shared-library code structure, beginning with the meaning and need
for component `internal/` directories.

## Objective

Keep the public/shared boundary explicit while flattening incidental directory
nesting.  A private implementation header remains private because of its
contract and name, not because it lives behind a one-file directory.

## Baseline

`base`, `host`, `storage`, and `ux-base` currently each have an `internal/`
directory.  They contain respectively the logical-Console binding bridge, the
host-native Console bridge, storage native-file helpers, and shared UX
mailbox/component implementation contracts.  They are not extra runtime
components or another public API.

## Planned work

- Audit every lib header by consumer and dependency direction.
- Replace one-off `internal/` directory paths with component-local,
  explicitly named private headers such as `console_internal.h` and
  `mailbox_internal.h`; use `git mv` and repair direct includes.
- Retain private contracts shared by allowed dependents (`ux-window` and
  `ux-console` may consume `ux-base` private mailbox/component mechanics).
- Make the README state the public/private naming convention and verify no
  public header includes a private header or gains host/product meaning.
- Refresh the shared corpus manifest and compare the resulting lib surface
  with NXVM before closure.

## Non-goals

- No UX, Console, storage, lifecycle, threading, or public API semantic
  change.
- No MVDM change and no app integration rewrite.
- Do not make private implementation headers public merely to avoid an
  include-path change.

## Verification

- Source-boundary and manifest checks; targeted include-path audit.
- Fresh x64/x86 build and full CTest after structural relocation.
- `git diff --check`, clean worktree, and exact NXVM adoption comparison.

## Exit criteria

Each shared component has a flat, self-explanatory private-header layout;
the public ABI and allowed dependency graph remain unchanged and proven.

