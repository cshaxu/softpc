# Quick Start for a fresh Windows checkout

## Problem

A fresh checkout already contains the x64 package, its adjacent default
configuration, and the default guest media. A Windows user can therefore run
the package without building. The root README does not say this, does not name
an installable build environment, and its x86 CMake preset embeds one
developer's drive path. A new user cannot reliably derive the supported
source-build path from the repository alone.

## Objective

Make the repository's first-run and source-build routes explicit, short, and
portable:

1. Windows x64 users may clone and run the checked-in package immediately.
2. A source build uses an ordinary MSYS2 UCRT64 installation and the existing
   x64 preset.
3. The optional x86 build uses the existing x86 preset from an MSYS2 MINGW32
   shell, without repository-specific toolchain paths or environment names.
4. The instructions identify the default guest media, safe overlay policy,
   `start` command, and background regression command.

## Design

The project keeps the current CMake preset model. It does not add a bootstrap
script, a second build system, a downloader, or a runtime setup step.

The x86 preset selects `gcc`, just as the x64 preset does. The user chooses
the target architecture by launching the matching MSYS2 shell; its PATH is the
toolchain selection mechanism. This removes the hard-coded local path and two
artificial `SOFTPC_I686_*` variables while preserving the existing
artifact-width validation.

The root README has two ordered routes: **Run the bundled package** and
**Build from source**. Source-build installation directions link to MSYS2's
official documentation and use its UCRT64 package names. x86 is explicitly
optional and documented separately. The media README only corrects its stale
source-mirror path.

## Scope and non-goals

In scope: `README.md`, `CMakePresets.json`, `assets/media/README.md`, the
active packet, this proposal, and S11 closure evidence.

Out of scope: source/runtime behavior, Lib/Common/x86 corpus, App/Core,
package INI, guest media, release automation, Linux product support, and
automatic toolchain installation.

## Verification

- Configure and build an x64 package in a fresh build tree using the edited
  preset's exact settings; confirm it produces `softpc64.exe` without using
  an existing build cache or package configuration.
- Configure the x86 preset with a real x86 MinGW PATH; confirm it detects a
  32-bit compiler and preserves the architecture validation.
- Run the existing documentation governance gate and whitespace check.
- Inspect every changed path and verify no user INI, guest media, package EXE,
  source, or shared-corpus file is changed in the primary worktree.

## Completion condition

A Windows newcomer can follow one compact documented route to run the bundled
x64 package and one to build it from a fresh clone; no preset retains a
machine-specific absolute toolchain path. The packet's verification passes and
the implementation is reviewed, committed, pushed, and recorded.

## Executor evidence

Against `0e59f84d`, a fresh `build/quickstart-x64` configured with the exact
x64 preset cache settings identified GNU 16.1.0 as a 64-bit compiler and
built the complete `softpcvm` package successfully. The generated package
output was verified, then restored in the primary worktree so this
documentation/preset task does not replace the accepted executable.

A separate fresh `build/quickstart-x86` with a real 32-bit MinGW PATH
identified GNU 16.1.0 as a 32-bit compiler and configured successfully with
the same cache settings as the portable x86 preset. The normal x64 preset also
reconfigured successfully, and the x86 preset reconfigured successfully with
that 32-bit PATH.

The package x64 executable remains checked in beside the default INI and
default Windows 3.1 disk; its imports are only standard Windows system DLLs.
Documentation governance and whitespace checks pass. The stale private
toolchain markers have no remaining production-document or preset occurrence.
