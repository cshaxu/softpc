# M9: Contextual Standalone Symbol Ownership

## Objective

Replace the generic `softpc_*` namespace in modern standalone source with
the owning source-layer namespace. File names already identify their owner;
the remaining global symbols should do the same without changing the recovered
SoftPC machine ABI.

## Candidate Work

- Rename modern application-owned globals below `src/app/` to `app_*`:
  runtime, window, console, keyboard, monitor and startup symbols.
- Rename modern standalone host-owned globals below `src/host/` to `host_*`:
  machine façade, platform, media, GFI, V7 pointer, DIB/video, audio and
  standalone presentation helpers.
- Name independent compatibility helpers below `src/host/compat/` as
  `compat_*`, with a subsystem component where needed (`compat_ccpu_*`,
  `compat_cvidc_*`).
- Remove a prefix from a function or object with internal linkage only when
  its enclosing file already makes ownership unambiguous.
- Repair every direct caller, test, declaration, CMake transform and static
  source-boundary check in the same change.

## Preserved ABI

Do not rename an original SoftPC-required entry point, even when it begins
with `host_*`. Those symbols are the recovered machine's historical host ABI,
including callbacks such as `host_read_resource`, `host_key_down`, and other
imports declared or called below `src/mvdm/softpc.new/`. This task renames only
modern standalone-owned symbols currently named `softpc_*`.

## Baseline

The initial audit found 320 unique `softpc_*` identifiers outside the mirror:
1,590 occurrences below `src/host/` and 688 below `src/app/`. The largest
families are `softpc_window_*` (62), `softpc_platform_*` (32),
`softpc_machine_*` (31), `softpc_compat_*` (22), and `softpc_runtime_*` (21).

## Non-goals

- Do not change guest behavior, configuration, product names, executable
  names, BIOS/ROM/BOP semantics, controller logic, CCPU/C-VID or media.
- Do not rename symbols under `src/mvdm/softpc.new/`.
- Do not rename original-host ABI callbacks or imply that `host_*` is an
  external public API.

## Verification

1. Assert that no modern standalone definition or declaration retains a
   `softpc_*` name, except an explicitly documented compatibility alias if
   unavoidable.
2. Assert every original machine import keeps its original symbol spelling.
3. Configure, build and run full CTest with GCC for x64 and x86.
4. Run package smoke for both launchers.

## Exit

Modern standalone symbols express their `app`, `host` or `compat` owner;
original SoftPC ABI names are untouched; and dual-width behavior remains
unchanged.
