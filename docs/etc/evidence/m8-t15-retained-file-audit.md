# M8 T15 Retained-file Audit

## Scope

This is a local, repository-owned inventory of the transitional
`src/core/softpc/` tree. It neither reads nor imports files from the external
OpenNT comparison tree.

## Retained Set For The Reference-shaped Move

| Class | Count | Disposition |
| --- | ---: | --- |
| C source (`.c`) selected by the standalone build | 252 | Move from `src/core/softpc/` to the matching `src/mvdm/softpc.new/` path. |
| C header (`.h`) in the selected SoftPC tree | 575 | Move from `src/core/softpc/` to the matching `src/mvdm/softpc.new/` path. |
| CCPU CMake input list (`base/ccpu386/c-files`) | 1 | Retain as required modern build configuration. |
| Existing selected firmware (`bios1.rom`, `bios4.rom`, `v7vga.rom`, `cmos.ram`) | 4 | Already tracked at `src/mvdm/softpc.new/roms/`; preserve byte-for-byte. |

No assembly source currently exists in the transitional `src/core/softpc/`
tree. Compiler outputs are not present in the retained set.

Twelve C/H files under `suballoc/` and `xms.486/` remain outside the move.
They are not selected by CMake, map outside the original `softpc.new` root in
the T14 ledger, and are already assigned to the later unselected-source
removal work. Moving them would make the reference-shaped tree less accurate.

## Excluded Historical Files

The following 18 files are neither opened by the current CMake build nor named
by the source-boundary test. They are old NMAKE/build metadata, generated-build
inputs, or superseded reference material; none is C/H/ASM, a required modern
build configuration file, or selected firmware.

```text
src/core/softpc/dirs
src/core/softpc/base/dirs
src/core/softpc/base/ccpu386/ccpudefs.inc
src/core/softpc/base/ccpu386/makefile
src/core/softpc/base/ccpu386/sources
src/core/softpc/base/cvidc/ccpudefs.inc
src/core/softpc/base/cvidc/makefile
src/core/softpc/base/cvidc/sources
src/core/softpc/base/support/makefile
src/core/softpc/base/support/sources
src/core/softpc/base/support/obj/_objects.mac
src/core/softpc/base/system/ica.old
src/core/softpc/base/system/makefile
src/core/softpc/base/system/sources
src/core/softpc/base/system/obj/_objects.mac
src/core/softpc/host/dirs
src/core/softpc/host/inc/dfa.gi
src/core/softpc/host/inc/kybdcpu.gi
```

They are pending explicit deletion confirmation because this task changes
repository layout, not machine behavior. Their retained-file audit evidence
will remain after deletion.

## Verification Command

The pre-move audit was obtained with the following read-only checks:

```powershell
Get-ChildItem src/core/softpc -Recurse -File |
  Where-Object { $_.Extension -in '.c', '.h', '.asm' -or $_.Name -eq 'c-files' }
rg -n "\\b(sources|makefile|ccpudefs\\.inc|dfa\\.gi|kybdcpu\\.gi|_objects\\.mac|ica\\.old)\\b" \
  CMakeLists.txt src/core/softpc_standalone_platform.c test/support/standalone_source_boundary.cmake
```
