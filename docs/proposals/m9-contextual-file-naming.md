# M9: Contextual File Naming

## Objective

Make the standalone tree self-describing without changing the recovered
machine. A source file outside the mirror must derive its meaning from its
owner directory, rather than repeat the repository name in every filename.
An overlay is reserved only for a large, directly attributable difference from
one recovered source file. Current compatibility helpers are standalone host
code, not alternate copies of recovered machine sources.

## M9 T20: Contextual Naming And Selective Overlay Audit

**Work:**

- Rename standalone host and test C/H files by `git mv` to contextual names;
  for example, `host/machine/machine.c`, `host/media/hdd_media.c`, and
  `host/video/dib_surface.c`.
- Keep product executable names, C target names, and exported `softpc_*`
  symbol spellings unchanged. This is a file-layout task, not an ABI or guest
  behavior change.
- Move the existing CCPU, C-VID, CMOS, BIOS, keyboard, and legacy-header
  helper files out of `src/overlay/`: none has a corresponding mirror path.
  They are standalone compatibility support and belong below
  `src/host/compat/` by subsystem.
- Do not create placeholder overlays. Keep small direct differences in their
  recovered file. A large difference may move only to the same relative
  overlay filename after its original baseline is proven available without a
  new source import.
- Repair CMake, transformations, source-boundary checks, tests, and the
  direct-difference ledger after every move.

**Verification:** clean GCC x64/x86 builds, full CTest, package smoke, a
source-tree assertion that no standalone C/H filename begins `softpc_`, and an
assertion that every overlay path is a same-relative-path mirror counterpart.

**Exit:** standalone file names are contextual; no faux overlay remains; the
owner has selected the treatment for direct source differences; and both
packages retain current behavior. A route-only include spelling change in a
recovered renderer is permitted when it is required to reach a renamed host
header; it does not change machine behavior.

## Direct-Difference Audit

The initial audit compared 56 direct recovered-source differences against the
read-only reference. Forty-six change at most 30 lines, and thirty change at
most 10 lines; they remain in the recovered file. `nt_com.c` and `nt_lpt.c`
are intentionally absent from the selected mirror because they are wholly
host-specific NT endpoint files; their independent standalone endpoints live
in `host/comms/`. The remaining high-churn files are `nt_keycd.c` (449) and
`nt_sound.c` (222). `nt_graph.c` (79) and `host_cpu.h` (65) remain readable
enough to retain.
