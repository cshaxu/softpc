# M8: Auditable Source-Mirror Migration

## Objective

Make the recovered OpenNT SoftPC source independently auditable without
changing the selected machine. The repository must distinguish, by path and
by build rule, between the original machine, a source overlay required to
compile it, the standalone host, and the user-facing application.

The resulting source shape is:

```text
src/
  mvdm/softpc.new/       # byte-accounted OpenNT source mirror
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
  "overlay" means a source patch relative to the OpenNT mirror.
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

`src/mvdm/softpc.new/` must remain directly comparable to the selected OpenNT
tree. An overlay is a small patch file under `src/overlay/mvdm/softpc.new/`
whose relative path identifies the original it changes. CMake composes the
mirror and selected patches into the ignored `build/` tree before compiling.
No edited copy of an original source file is compiled directly from the mirror.

Each overlay has a manifest record containing the original path, patch path,
reason, owner, selected defines, and x86/x64 evidence. New standalone source
never belongs in the mirror or overlay tree.

## Remaining Candidate Tasks

### M8 T15: Pristine mirror and patch-composition harness

**Boundary:** build assembly only; no semantic source migration yet.

**Work:** create `src/mvdm/softpc.new/` as the selected original mirror;
introduce the mirrored overlay-patch and manifest layout; compose into
`build/`; make CMake fail if a patch no longer applies or a source is selected
outside the manifest.

**Verification:** prove composed source hashes equal pristine hashes for
unpatched files; build current x64/x86 targets and compare focused machine,
renderer, input, timer, media, and BOP test results to the pre-task baseline.

**Exit:** the compiler no longer reads a directly edited original source file
from the source mirror.

### M8 T16: Original-diff evacuation

**Boundary:** original source changes only.

**Work:** move every current direct modification under the old recovered tree
to a named mirrored overlay patch or remove it when the original code already
works. Keep generated CCPU/C-VID transformations reproducible from pristine
inputs in `build/`; do not check generated sources into `src/`.

**Verification:** x64/x86 build; all applicable CTest; path/hash manifest;
similar-issue sweep over headers, generated inputs, CCPU, C-VID, BIOS,
controllers, and original `nt_*` sources.

**Exit:** `src/mvdm/softpc.new/` is pristine and every applied diff is visible
in the mirrored overlay tree with a manifest disposition.

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
