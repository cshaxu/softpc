# Source Layout

The current source tree is:

```text
src/
  mvdm/
    softpc.new/
  host/
    ordinary host C/H files
    compat/{ccpu,cvidc,...}/
  lib/{base,host,storage,ux-base,ux-window,ux-console}/
    canonical shared platform implementation, delivered for exact NXVM adoption
  app/
    main.c, runtime.c, presentation.c, keyboard.c, firmware.rc
```

Directories appear only in their admitted migration task.
`mvdm/softpc.new` contains the selected repository-owned recovered-source
subset moved from the former baseline tree. Every retained path and name
permits a direct T14-ledger comparison with the selected read-only OpenNT
reference. Wholly host-specific original endpoint files may be intentionally
absent when the standalone host owns their replacement. Retained files are
C/H/ASM, required modern build configuration, and selected BIOS/VGA/CMOS
firmware only. Historical object,
library, and other compiler intermediate files are forbidden. Narrow,
mechanical compiler, declaration, calling-ABI, and pointer-representation
corrections live as reviewable source diffs at their affected points. Generated
transformed C/H files are not build inputs. `host` owns platform capability
implementations and larger functional adaptations; `app` owns
orchestration, machine snapshot production, guest-input conversion, monitor
UI, and product lifecycle policy. `lib` owns copied-value platform mechanics:
mailbox, host input normalization, action registration, console/window loops,
routing, geometry, capture, clock, synchronization, and storage. It never
owns a project runtime, machine, renderer, product input queue, guest-input
mapping, lifecycle, or hotkey meaning.

## Build Output Layout

The complete `build/` tree is ignored. It holds CMake build trees, generated
sources, test binaries, compiler intermediates, logs, captures, diagnostic
scripts, and disposable test media. Additional host-width configurations use
children of it (for example `build/x86/`). The only user-facing package is
`assets/binary/`: `softpc32.exe`, `softpc64.exe`, and their adjacent fixed
`softpc.ini`. That INI may use absolute paths or paths relative to
`assets/binary/`; the supplied default uses `../media/`. Reusable guest media
belongs in `assets/media/`. The fixed original ROM set remains embedded from
its source-mirror location `src/mvdm/softpc.new/roms/`; no external-ROM asset
contract exists.
Repository-root executables and sibling `build-*` directories are forbidden.

The preserved SoftPC baseline contains source, headers, scripts, and runtime
resources only. Checked-in or newly generated object files, libraries, and
other compiler outputs do not belong beneath `src/`; rebuild them in `build/`.
