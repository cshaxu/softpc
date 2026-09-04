# Source Layout

The current source tree is:

```text
src/
  mvdm/
    softpc.new/
  overlay/
    mvdm/softpc.new/
  host/
    ordinary host C/H files
    comms/{serial,parallel}.c
    compat/{ccpu,cvidc,...}/
  app/
    main.c, runtime.c, console.c, keyboard.c, window.c, firmware.rc
```

Directories appear only in their admitted migration task.
`mvdm/softpc.new` contains the selected repository-owned recovered-source
subset moved from the former baseline tree. Every retained path and name
permits a direct T14-ledger comparison with the selected read-only OpenNT
reference. Wholly host-specific original endpoint files may be intentionally
absent when the standalone host owns their replacement. Retained files are
C/H/ASM, required modern build configuration, and selected BIOS/VGA/CMOS
firmware only. Historical object,
library, and other compiler intermediate files are forbidden. If used,
`overlay/mvdm/softpc.new` contains only repository-owned mirrored patches
necessary to compile the baseline on x86/x64; generated forms remain in
ignored `build/`. `host` owns platform capability implementations; `app` owns
orchestration, mailboxes, and user interaction/presentation.

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
