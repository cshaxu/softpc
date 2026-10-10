# Source Layout

The current source tree is:

```text
src/
  app-softpc/
    product/
      main.c, config.c, command.c, composition.c, keyboard.c, firmware.rc
    softpc.new/
    compat/
      original host callback C/H files and port ABI support
      {ccpu,cvidc,bios,cmos,system,keymouse}/
    machine/
      concrete machine backend, driver, input, debug, trace; public vm_interface.h
  emulator/
    ui/          broker, monitor Console and KVM composition
    session/     neutral control FIFO, reduction and UI dispatch
    machine/     generic executor, lifecycle/input queues and frame publication
  x86/{xasm32,debug,product}/
    shared assembly/debug policy and session/UI composition
  lib/{types,base,console,console-broker,storage,audio,kvm-base,kvm-window,kvm-console}/
    canonical shared platform implementation, delivered for exact NXVM adoption
```

Directories appear only in their admitted migration task.
Emulator never contains win32/linux directories or platform-selected source.
Its independent source verification lives with the Emulator corpus; unit tests
live in test/emulator, never src/emulator/test. Lib tests and their fixtures live
in test/lib. `src/x86` and `test/x86` own the shared CLI, debugger,
assembly and surface corpus. Each suite has its own CMake entry and manifest.
All six source/test packages select C11 with extensions disabled, in
both standalone and embedded builds. GNU/Clang shared builds use
-Wall -Wextra -Wpedantic -Werror. This owner-approved shared-corpus baseline
supersedes the general standalone C17 rule only for these six directories;
App/Core retain their existing language settings. Package-local build settings
do not propagate warning policy or a new language requirement into the x86.
Only Lib provides the underlying platform implementation.
Shared `lib/types` is header-only. Its top level contains emulator C/compiler
vocabulary; explicit `win32/` and `linux/` headers group external platform
declarations by purpose. Emulator types headers never select an OS. Only the
owning component's corresponding platform implementation imports these
platform headers. Compiler selection for atomic primitives remains permitted.
Other components' `win32/` and `linux/` directories are implementation-only.
Cross-component declarations belong in root `*_interface.h` files, never in
platform directories or root headers forwarding to platform implementation.
`app-softpc/softpc.new` contains the selected repository-owned recovered-source
subset moved from the former baseline tree. Every retained path and name
permits a direct T14-ledger comparison with the selected read-only OpenNT
reference. Wholly host-specific original endpoint files may be intentionally
absent when the standalone host owns their replacement. Retained files are
C/H/ASM, required modern build configuration, and selected BIOS/VGA/CMOS
firmware only. Historical object,
library, and other compiler intermediate files are forbidden. Narrow,
mechanical compiler, declaration, calling-ABI, and pointer-representation
corrections live as reviewable source diffs at their affected points. Generated
transformed C/H files are not build inputs. `app-softpc/compat` owns original host
callbacks and larger functional adaptations. `app-softpc/machine` owns the injected SoftPC
driver, guest-input/frame conversion, debug adaptation and diagnostic trace.
`app-softpc/product` owns SoftPC configuration, private machine composition,
resources and the snapshot/media extension. Shared `x86/product` owns the
X86 CLI/hotkey policy and Emulator composition; it receives an already
composed machine/driver lifetime and two UI choices, never a SoftPC config.
It owns entity assembly, event wiring, the blocking session run and teardown,
without a separate state machine, command table or debugger. Main loads config.
Only app-softpc/product/composed_machine.c may include machine/vm_interface.h; no shared X86 file may include Compat
or MVDM, and no other X86 file may include VM. VM's public header exposes only
copied options, opaque identity and existing Emulator/Lib contracts. Compat
does not depend on app, VM or Emulator. Historical same-name replacement headers
remain isolated; no generic compat wrapper layer is retained. `emulator/machine` owns the one generic
executor, lifecycle/input queues, run generation and copied-frame publication.
`emulator/session` owns the control FIFO,
completed-fact reduction, prompt scheduling and dispatch to injected machine/
UI adapters. `emulator/ui` owns monitor/KVM composition and Console handoff.
`lib` owns copied-value platform mechanics:
mailbox, host input normalization, action registration, console/window loops,
routing, geometry, capture, clock, synchronization, and storage. It never
owns a project runtime, machine, renderer, product input queue, guest-input
mapping, lifecycle, or hotkey meaning.

## Build Output Layout

X86 tests are classified by ownership: test/app-softpc/unit/product covers configuration
and App-specific extensions; test/x86/product covers shared commands, keys and
composition; test/app-softpc/unit/{machine,compat,softpc.new} covers concrete
machine, compatibility host and recovered-mirror units; test/app-softpc/integration covers composed worker, command, snapshot,
frame and package flows. CTest names retain their owner path; labels identify
the owner and execution class, not a claim that every test labelled unit is
isolated. X86 fixtures and
checks live beside their owner: test/app-softpc/integration/machine_fixture.c/h is the
single product driver/Emulator assembly fixture, snapshot orchestration is in
test/app-softpc/integration, and x86 static gates are in tools/checks. Test execution is summarized in test/README.md.
The reusable test/lib, test/emulator and test/x86 packages are not destinations
for X86-owned fixtures.

Root CMake assigns all `src/app-softpc/machine/*.c` to `softpc-vm`; other targets link it
instead of copying its implementation list. The original machine OBJECT
groups remain intact. The build-ownership gate checks actual target source
membership and VM completeness at configure time; its negative tests reject
duplicate/foreign source ownership. x86 include gates check reverse
dependencies across the seven owners (including the three Core children),
including relative paths. Tests may use
private implementation contracts for focused proof; production consumers may
not bypass the VM public boundary.

The complete `build/` tree is ignored. It holds CMake build trees, generated
sources, test binaries, compiler intermediates, logs, captures, diagnostic
scripts, and disposable test media. Additional host-width configurations use
children of it (for example `build/x86/`). The only user-facing package is
`assets/binary/`: `softpc32.exe`, `softpc64.exe`, and their adjacent fixed
`softpc.ini`. That INI may use absolute paths or paths relative to
`assets/binary/`; the supplied default uses `../media/`. Reusable guest media
belongs in `assets/media/`. The fixed original ROM set remains embedded from
its source-mirror location `src/app-softpc/softpc.new/roms/`; no external-ROM asset
contract exists.
Repository-root executables and sibling `build-*` directories are forbidden.

The two GNU package presets explicitly select Release with `-O3 -DNDEBUG`,
including when reusing an existing build cache. Tests separately undefine
NDEBUG so their operations and assertions remain live in optimized builds.

Default `ctest --preset test-x64` and `test-x86` run background tests only
with a bounded eight-way schedule: they exclude the `desktop` label. Real
Window/Console tests use explicit
`test-desktop-x64` or `test-desktop-x86` presets, serially and only during a
reserved desktop testing period. Do not use an unfiltered CTest invocation
for background work; standalone suites use `-LE desktop`. Build/test commands
run with captured output (no new terminal window); any Start-Process helper
must use hidden launch. Report background and desktop results separately;
a background pass is not a full-suite pass. x86 rendering is not hidden
or otherwise changed to make a desktop test appear background-safe.

The preserved SoftPC baseline contains source, headers, scripts, and runtime
resources only. Checked-in or newly generated object files, libraries, and
other compiler outputs do not belong beneath `src/`; rebuild them in `build/`.
