# Source Layout

The current tree is transitional. The approved target is:

```text
src/
  mvdm/
    softpc.new/
  overlay/
    mvdm/softpc.new/
  host/
    {platform,media,video,input,compat,machine}/
  app/
    runtime/
    frontends/{console,win32}/
    main.c
```

Directories appear only in their admitted migration task. `mvdm/softpc.new`
is directly comparable to the selected OpenNT source and contains no
standalone behavior branches. `overlay/mvdm/softpc.new` contains only mirrored
patches necessary to compile that baseline on x86/x64; generated forms remain
in ignored `build/`. `host` owns platform capability implementations; `app`
owns orchestration, mailboxes, and user interaction/presentation.

## Build Output Layout

The complete `build/` tree is ignored. It holds CMake build trees, generated
sources, test binaries, compiler intermediates, logs, captures, diagnostic
scripts, and disposable test media. Additional host-width configurations use
children of it (for example `build/x86/`). The only user-facing package is
`artifacts/binary/`: `softpc32.exe`, `softpc64.exe`, and their adjacent fixed
`softpc.ini`. That INI may use absolute paths or paths relative to
`artifacts/binary/`; the supplied default uses `../media/`. Reusable guest
media belongs in `artifacts/media/`. The fixed original ROM set is embedded
from its source-mirror location `src/mvdm/softpc.new/roms/`; a future package
that genuinely needs runtime-external ROM input must use `artifacts/roms/`.
Repository-root executables and sibling `build-*` directories are forbidden.

The preserved SoftPC baseline contains source, headers, scripts, and runtime
resources only. Checked-in or newly generated object files, libraries, and
other compiler outputs do not belong beneath `src/`; rebuild them in `build/`.
