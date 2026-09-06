# Retire XP-Obsolete Non-x86 Mirror Headers

## Objective

Remove the 30 remaining, unselected MIPS/PPC historical headers and three
separately audited legacy headers from
`src/mvdm/softpc.new/` so the selected mirror no longer retains paths present
in the OpenNT reference but absent from XP SP1.  Rebuild the fixed package
executables `assets/binary/softpc32.exe` and `assets/binary/softpc64.exe` from
the resulting checked-in source.

This was admitted and closed as M9 T40: a mechanical, non-machine-behaviour
prerequisite cleanup for the XP SP1 mirror rebase.  The closure evidence is
in [T40 history](../history/M9-T40-xp-obsolete-header-retirement.md).

## Frozen Comparison Basis

```text
OpenNT: O:/repos.external/opennt-src-2/nt/private/mvdm/softpc.new @ 5e4619ab6
XP SP1: O:/repos.external/winxpscodes/Source/XPSP1/NT/base/mvdm/softpc.new @ a2f6d7c93
Local:  src/mvdm/softpc.new
```

At admission, the OpenNT-only inventory had 126 C/H paths.  Local already
lacked 96.  The remaining 30 were all headers, all below `host/inc/mips/` or
`host/inc/ppc/`, and were absent from the XP SP1 tree.  They had no exact-path
selection in CMake, sources, scripts, or tests, and no active MIPS/PPC host
build existed.  After T40, all 126 OpenNT-only paths are absent locally.
The owner also added three independent candidates to the admitted audit:
`get_env.h` only declared the already-retired `get_env.c` wrappers;
`gfisflop.h` only supported the previously retired GFI slave-PC/serial-RPC
backends; and `monsim32.h` has neither a selected direct include nor an
x64/x86 compiler dependency (its only historical parent, `sim32.h`, is absent
locally).

## Exact Deletion Manifest

```text
host/inc/mips/pig/cpu_c.h
host/inc/mips/pig/cpu4gen.h
host/inc/mips/pig/cpuint_c.h
host/inc/mips/pig/evidgen.h
host/inc/mips/pig/gdpvar.h
host/inc/mips/pig/sas4gen.h
host/inc/mips/prod/cpu_c.h
host/inc/mips/prod/cpu4gen.h
host/inc/mips/prod/cpuint_c.h
host/inc/mips/prod/evid_c.h
host/inc/mips/prod/evidfunc.h
host/inc/mips/prod/evidgen.h
host/inc/mips/prod/fpu_c.h
host/inc/mips/prod/gdpvar.h
host/inc/mips/prod/pigger_c.h
host/inc/mips/prod/pigreg_c.h
host/inc/mips/prod/sas4gen.h
host/inc/mips/prod/univer_c.h
host/inc/ppc/pig/cpu_c.h
host/inc/ppc/pig/cpu4gen.h
host/inc/ppc/pig/cpuint_c.h
host/inc/ppc/pig/evidgen.h
host/inc/ppc/pig/gdpvar.h
host/inc/ppc/pig/sas4gen.h
host/inc/ppc/prod/cpu_c.h
host/inc/ppc/prod/cpu4gen.h
host/inc/ppc/prod/cpuint_c.h
host/inc/ppc/prod/evidgen.h
host/inc/ppc/prod/gdpvar.h
host/inc/ppc/prod/sas4gen.h
base/inc/get_env.h
base/inc/gfisflop.h
host/inc/monsim32.h
```

## Non-goals

- Do not remove any selected x86/AT/V7 MVDM source, nor alter guest machine,
  BIOS, controller, media, rendering, input, timing, or host ABI behavior.
- Do not import any XP source or change the XP A/B hunk disposition ledger.
- Do not delete empty directories merely as a consequence of the manifest.
- Do not change `assets/binary/softpc.ini`, guest media, ROM inputs, or
  unrelated worktree changes.

## Required Proof And Delivery

1. Before deletion, repeat exact-path searches over `src/`, `CMakeLists.txt`,
   `scripts/`, and `test/`; prove no manifest member is selected or included.
2. Configure, build, and run full CTest for GCC x64 and i686 x86.
3. Confirm those builds refresh only the package executables
   `assets/binary/softpc64.exe` and `assets/binary/softpc32.exe`; preserve the
   adjacent user-owned `softpc.ini` byte-for-byte.
4. Run both package-smoke tests and record the dual-width evidence.
5. Run the documentation-governance verifier before closure.

## Exit Criteria

All 33 manifest paths are absent, no selected source or compiler dependency
names one of them, both fixed-width package executables are rebuilt and smoke
tested, and the selected standalone machine is otherwise unchanged.
