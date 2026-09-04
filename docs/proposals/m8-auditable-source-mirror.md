# M8: Auditable Source-Mirror Migration

## Objective

Make the recovered OpenNT SoftPC source independently auditable without
changing the selected machine. The repository must distinguish, by path and
by build rule, between the original machine, a source overlay required to
compile it, the standalone host, and the user-facing application.

The resulting source shape is:

```text
src/
  mvdm/softpc.new/       # repository-owned recovered source, reference-shaped
  overlay/mvdm/softpc.new/
                          # patch files with the same relative paths
  host/                  # independent platform and compatibility adapters
  app/                   # monitor, runtime, console, Win32 presentation
```

`src/mvdm/softpc.new/host/` is original SoftPC host source, not the new
standalone host. `src/host/` is exclusively new standalone code. No
`src/guest/` tree is created: guest firmware and media are runtime artifacts,
not standalone source code.

## Non-goals

- Do not change CPU, C-VID, controller, BIOS, ROM, VGA ROM, BOP, timing, or
  guest-media behavior.
- Do not introduce DOS, DPMI, WOW, NTVDM, CSR, VDD, session, or product
  services.
- Do not create a persistent disk COW/overlay format. This proposal's
  "overlay" means a source patch relative to the repository-owned recovered
  source, if a later task proves that one is necessary.
- Do not copy, import, generate, or retain source files from `opennt-src-2`.
  That tree is a read-only path-and-content comparison reference only.
- Do not rewrite an original algorithm merely to make the directory move
  easier.

## Why Port ABI Remains

"ABI" here means the binary calling/data-layout contract between separately
compiled original and standalone code: function signatures, pointer widths,
structure layout, function-table entries, and calling convention. It is not a
guest feature. The current CCPU/C-VID width bridges stay only where x86/x64
compilation proves they are needed; they become ordinary, documented host
compatibility implementation details rather than a product-facing subsystem.

## Composition Rule

`src/mvdm/softpc.new/` must retain the relative paths and names needed for a
path-by-path comparison with the selected OpenNT tree, but contains only
repository-owned recovered files moved from the former source location. An
overlay, if later needed, is a small repository-owned patch under
`src/overlay/mvdm/softpc.new/` whose relative path identifies the affected
source. No material is copied from the external reference. CMake may compile
the moved repository-owned source directly; generated forms remain in ignored
`build/`.

Each overlay has a manifest record containing the original path, patch path,
reason, owner, selected defines, and x86/x64 evidence. New standalone source
never belongs in the mirror or overlay tree.

## Remaining Candidate Tasks

### M8 T15: Pristine mirror and patch-composition harness

**Boundary:** build assembly only; no semantic source migration yet.

**Work:** move the existing recovered SoftPC source from `src/core/softpc/`
into `src/mvdm/softpc.new/` with its reference-shaped relative paths;
introduce a provenance/retained-file manifest; route CMake to the moved
repository-owned files. Do not copy any external source or retain historical
build intermediates.

**Verification:** prove moved paths existed in the repository before the move;
audit retained file classes; build current x64/x86 targets and compare focused
machine, renderer, input, timer, media, and BOP test results to the pre-task
baseline.

**Exit:** the compiler reads only the moved repository-owned source paths; no
external source or compiler intermediate has entered the repository.

### M8 T16: Original-diff evacuation

**Boundary:** original source changes only.

**Work:** classify and, where justified, move standalone adaptation out of the
recovered-source tree into repository-owned host/app code or a small mirrored
overlay. Keep generated CCPU/C-VID transformations reproducible from current
repository inputs in `build/`; do not check generated sources into `src/`.

**Verification:** x64/x86 build; all applicable CTest; path/hash manifest;
similar-issue sweep over headers, generated inputs, CCPU, C-VID, BIOS,
controllers, and original `nt_*` sources.

**Exit:** every direct-difference row has a manifest disposition, and no
external reference content has been added to the repository.

### M8 T17: Standalone-host decomposition

**Boundary:** new standalone code only.

**Work:** split the current mixed `softpc_standalone_platform`, raw-image,
DIB, machine-assembly, and compatibility files into `src/host/{platform,
media,video,input,compat,machine}`. Preserve existing original entry points;
each `host_*` callback has one implementation owner. In particular, raw HDD
and floppy backends remain host media ports, not replacement controllers.

**Verification:** controller, FDC, HDD, dual-media, BOP, serial/printer,
audio, timer, V7 frame, and mouse focused tests on x64/x86.

**Exit:** no mixed standalone platform aggregation remains, and no host source
directly owns guest RAM, device state, or renderer/controller algorithms.

### M8 T18: Application-shell migration

**Boundary:** monitor, runtime, input queue, and presentation only.

**Work:** move `src/vm/` to `src/app/`, grouping monitor/runtime and console/
Win32 presentation beneath it. Keep the one-executor and copied-frame
contracts unchanged. Update tests and CMake without adding a public SDK or
multi-session surface.

**Verification:** x64/x86 runtime, keyboard, window, monitor, pause/resume,
mouse-capture, and direct-launch tests; launch both packages from
`artifacts/binary/` using only their adjacent INI.

**Exit:** `src/app/` contains all user-visible shell code, and app code has no
direct machine-state access.

### M8 T19: Transitional-layout removal and proof

**Boundary:** removal only after T13–T18 evidence is complete.

**Work:** remove obsolete source locations and obsolete CMake routing; update
architecture/source-layout documentation; publish the final source/diff
ledger. Do not remove any source merely because it is hard to classify.

**Verification:** clean x64 and x86 GCC builds, full CTest, A:, C:, and dual
media startup, Windows 3.1 text-to-graphics handoff, V7 cursor/mouse,
console/window/auto presentation, and artifact-package launch proof.

**Exit:** no source file remains under the transitional `src/core/softpc` or
`src/vm` layouts; all original-source differences are patch-visible and all
new code is owned by `host` or `app`.

## Common Stop Conditions

Stop for owner direction if a candidate would require a new machine policy,
guest product service, controller/ROM/BOP semantic change, media mutation, or
an unproved CCPU/C-VID representation change. A path move that exposes an
unexplained behavior difference is a defect to diagnose, not permission to
rewrite the original code.
