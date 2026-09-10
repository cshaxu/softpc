# M9 T49 S5 — Linux placeholder build contract

## Objective

Remove build dependencies that no current Linux shared-library implementation
uses, while preserving required pthread support and the approved standalone
project-name exception.

## Boundaries

- Only `src/lib/CMakeLists.txt`, relevant library documentation/tests, and the
  manifest may change.
- Linux Window and Console presenters remain explicit `UNSUPPORTED`
  placeholders; this task does not implement them.
- `project(nxvm_shared_library ...)` remains unchanged as the owner-approved
  exception.

## Required work

1. Remove `find_package(Curses REQUIRED)`, Curses includes, and Curses
   linkage from the Linux presenter path.
2. Retain `find_package(Threads REQUIRED)` and link `Threads::Threads` only
   to Linux `host-sync`, whose `host/linux/sync.c` owns pthread use.
3. Add/adjust a build assertion proving Linux configuration no longer depends
   on Curses.

## Verification and exit

Linux CMake configuration succeeds without Curses discovery; host-sync keeps
its pthread linkage; project identity remains `nxvm_shared_library`; existing
Windows x64/x86 regression and manifest checks remain clean.

## Completion evidence

- The Linux branch no longer discovers, includes, or links Curses.  Its only
  platform dependency is `Threads::Threads` on `host-sync`, the sole target
  whose Linux implementation uses pthread.
- `verify_linux_build_contract.cmake` proves that source-level contract, while
  the standalone shared-library configuration and CTest run passed 2/2.
- Strict Windows x64 and x86 root builds each completed with the existing
  regression suite passing 34/34.  Manifest and whitespace checks passed.
- `project(nxvm_shared_library ...)` remains the owner-approved exception.
- Implementation: `7631880` (`M9 T49 S5 P2`).
