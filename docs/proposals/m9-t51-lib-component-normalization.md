# M9 T51 — Shared-Library Component Normalization

## Owner Request

Normalize the shared library around these component names and dependency
boundaries:

- `base` becomes `types`, a root-only component which any other library
  component may depend on;
- the neutral logical `lib_console` contract leaves `base` and becomes its own
  `console` component;
- `ux-base`, `ux-window`, and `ux-console` become `ui-base`, `ui-window`, and
  `ui-console`;
- inside the library, only the two UI leaves may depend on `ui-base`.

SoftPC remains an external application consumer of public interfaces.  It may
consume `ui-base` copied input/frame ABI; that is not a library-component DAG
edge.

## Objective

Make the source directories, public interface paths, CMake targets, and actual
dependency graph describe the same small, product-neutral library:

```text
types
  ├─ console ──> host, ui-console
  ├─ storage
  └─ ui-base ──> ui-window, ui-console
```

`types` contains only scalar aliases, status values, and other universal ABI
definitions. `console` contains the platform-neutral logical Console object,
copied Console input/output values, and its lifetime/output contracts. `host`
continues to own native binding; `ui-window` and `ui-console` remain entirely
independent leaf lifecycles.

## Non-goals

- No change to MVDM source, guest behavior, lifecycle policy, UX input,
  Console handoff semantics, threads, mailbox behavior, or Windows rendering.
- No compatibility aliases, forwarding headers, duplicate targets, or old
  `base`/`ux-*` production path after the atomic migration.
- No change to public value semantics beyond the intentional include-path and
  `ux_*` to `ui_*` naming migration.
- No Linux implementation work, package/media/configuration change, or
  modification of user-owned `assets/binary/softpc.ini`.

## Convergence Ledger

The frozen universe is every tracked production, test, CMake, README, and
manifest path under `src/lib/`, plus every tracked SoftPC source/test/CMake
path that includes a moved public interface or links a moved target. Each is
classified in S1 as one of: renamed/moved, rewritten to the new public name,
unchanged with no old reference, or removed as obsolete aggregation. Completion
requires zero tracked production/build references to `lib/base/`, `ux-base`,
`ux-window`, `ux-console`, `base-console`, `host-sync`, `storage-medium`,
`ux`, `ux-contract`, or `ux-native`, except historical records deliberately
outside the active source/build corpus.

## Subtasks

### S1 — Frozen inventory and migration contract

Record the complete path/target/API ledger, dependency graph, external SoftPC
consumer boundary, exact prohibited-old-name scan, and tests that prove no
behavior contract moves. Revise current design/architecture authority only as
needed to state the intended graph before implementation.

### S2 — Atomic library corpus normalization

Use `git mv` to split `types` and `console`, rename the three UX components to
their `ui-*` names, update all library-local public/private includes and CMake
targets, remove obsolete aggregation aliases, and regenerate the exact library
manifest. The tree must build after this S; no forwarding compatibility path is
allowed.

### S3 — SoftPC consumer adoption and boundary proof

Migrate all application/host/test consumers to the new public interface and
target names. Make the executable target explicitly link the public components
it directly consumes; retain only intentional application consumption of
`types`, `console`, and `ui-base` ABI. Update static boundary tests and
component README dependency declarations.

### S4 — Whole-corpus verification and T closure

Run the ledger’s old-name scans, manifest verification, documentation and
source-boundary gates, standalone MSVC library verification, and proportionate
x64/x86 full package build/CTest proof. Perform the required changed-path,
line-count, obsolete-path, and task-level closure audit.

## Verification Standard

- Component-DAG checks prove the graph above and reject forbidden library
  edges.
- A complete source/build scan proves no active old component/target name
  remains.
- Library manifest and independent Windows MSVC build/CTest pass.
- Fresh x64 and x86 package builds and full CTest suites pass from the final
  source tree; package executables alone may refresh, never `softpc.ini` or
  media.
- Every implementation P is pushed, reviewed against its packet, and recorded
  with reproducible changed-path accounting.
