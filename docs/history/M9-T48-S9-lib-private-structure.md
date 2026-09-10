# M9 T48 S9 — Shared-lib private structure

## Original request

Audit the shared-library code structure, beginning with the meaning and need
for component `internal/` directories.

## Objective

Keep the public/shared boundary explicit while flattening incidental directory
nesting.  A `*_interface.h` header is the only external contract; every other
header is private implementation and uses its short component-local name.

## Baseline

`base`, `host`, `storage`, and `ux-base` currently each have an `internal/`
directory.  They contain respectively the logical-Console binding bridge, the
host Console backend, storage file-backend helpers, and shared UX
mailbox/component implementation contracts.  They are not extra runtime
components or another public API.

## Planned work

- Audit every lib header by consumer and dependency direction.
- Replace one-off `internal/` directory paths. Rename the public headers to
  `*_interface.h`; retain concise names such as `console.h` and `mailbox.h`
  for private implementation contracts. Use `git mv` and repair direct
  includes.
- Retain private contracts shared by allowed dependents (`ux-window` and
  `ux-console` may consume `ux-base` private mailbox/component mechanics).
- Make the README state the public/private naming convention and verify no
  public header includes a private header or gains host/product meaning.
- Refresh the shared corpus manifest.  This repository's resulting `src/lib/`
  corpus is the canonical shared-library delivery for NXVM to adopt exactly.

## Non-goals

- No UX, Console, storage, lifecycle, threading, or public API semantic
  change.
- No MVDM change and no app integration rewrite.
- Do not make private implementation headers public merely to avoid an
  include-path change.

## Verification

- Source-boundary and manifest checks; targeted include-path audit.
- Fresh x64/x86 build and full CTest after structural relocation.
- `git diff --check`, clean worktree, and a delivery-ready canonical corpus for
  exact NXVM adoption.

## Exit criteria

Each shared component has a flat, self-explanatory header layout: public ABI
is visibly `*_interface.h`, while private implementation has no false public
appearance. The allowed dependency graph remains unchanged and proven.
