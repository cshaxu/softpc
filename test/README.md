# Test ownership

| Directory | Owner and purpose |
| --- | --- |
| [lib](lib/README.md) | Independently reusable Lib tests and fixtures. |
| [emulator](emulator/README.md) | Independently reusable neutral Emulator tests and fake machine. |
| [x86](x86/) | Independently reusable X86 command, debugger, assembler and product tests. |
| [app-softpc/unit](app-softpc/unit/) | SoftPC configuration and owner-local `machine`, `compat`, and preserved `softpc.new` unit tests. |
| [app-softpc/integration](app-softpc/integration/) | SoftPC composed command, worker, frame, snapshot, shutdown and package flows. |
| [../tools/checks](../tools/checks/) | Repository source, build, package and documentation boundary checks. |

Root CMake registers every suite. Public CTest names begin with their actual
owner path (`lib`, `emulator`, `x86`, `app-softpc`, or `checks`) and retain
any nested test directory. For App tests the registered owner path is also the
single source of truth for its `unit` or `integration` label. Labels identify
the test owner and execution class; they are not a guarantee of isolated
execution.
Self-contained tests create disposable media in their build working directory.
Only `runtime_restart_boot_smoke` reads the fixed installed image;
`package_smoke` exercises the matching packaged EXE and owner configuration.
They use non-mutating media modes; tests must not overwrite supplied media/INI.

`app-softpc/integration/machine_fixture.c/h` assembles one SoftPC driver and Emulator machine
for three worker/boot tests. It owns only these two test objects; callers own
the original machine. Tests call Emulator directly after assembly. It is not the
neutral fake machine from test/emulator and is not part of any reusable corpus.
`app-softpc/integration/snapshot_cross_process.cmake` orchestrates disposable save/load
processes using the existing snapshot transaction test.

Configure with `cmake --preset mingw-gcc-x64-release` (or x86), then build with
`cmake --build --preset tests-x64` (or x86). `ctest --preset test-x64` and
`test-x86` exclude the five desktop cases per width and are safe for background
work. `test-desktop-x64` / `test-desktop-x86` require reserved desktop access;
never use an unfiltered invocation while the owner is using the desktop.
List exact cases with `ctest --preset test-x64 -N` or the desktop preset.

The old unregistered direct-slice and Setup diagnostics are retired, not
supported runners. Their source and historical evidence remain in Git; no
registered test was removed. Full ownership and build policy belongs to
[Source Layout](../docs/design/CODING.md).
