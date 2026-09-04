# M8 T13: Test Boundary And Artifact-Backed Integration

## Outcome

Completed the test-tree migration and separated unit coverage from runnable
package acceptance.

- All registered test sources now live below `test/`.
- `test/unit/` contains the self-contained machine, host, runtime, keyboard,
  window, and static-boundary checks. Their working directory is the configured
  build tree, so generated fixture images never touch the repository root.
- `test/integration/softpc_package_smoke.c` is the package acceptance runner.
  It launches the architecture-matching launcher from `artifacts/binary/` with
  no command line, reads the adjacent fixed INI, confirms both configured media
  paths stay below `artifacts/media/`, requires non-mutating media mode, and
  observes monitor readiness.
- Command-line image probes were retained as unbuilt historical diagnostics in
  `test/support/diagnostics/`, because they meet neither tier contract.
- The selected original ROM inputs now live at
  `src/mvdm/softpc.new/roms/`, matching their OpenNT mirror location. They are
  embedded into the executable; the obsolete `artifacts/firmware/` directory
  was removed.

## Evidence

Clean CMake/Ninja builds and complete CTest runs passed on 2026-09-03:

| Host build | Unit | Integration | Full CTest |
| --- | ---: | ---: | ---: |
| WinLibs GCC x64 (`build/test-x64`) | 18/18 | 1/1 | 19/19 |
| MSYS2 MinGW GCC x86 (`build/test-x86`) | 18/18 | 1/1 | 19/19 |

The three embedded ROM SHA-256 values match
`opennt-src-2/nt/private/mvdm/softpc.new/roms/` exactly:

| File | SHA-256 |
| --- | --- |
| `bios1.rom` | `1CE33544249FEF59865B91308FAF60F354563CF854F80EB196D47C968899A574` |
| `bios4.rom` | `B09FDFF1A500971D54233378CE9C36F1B6A58BF577E4A544DA03A0D78D7E0D6F` |
| `v7vga.rom` | `970F105CD9E42EE56F07AAE695BAC89786D3455AB9D4C1EA9A1D1643B1E8F6F0` |

`test/support/standalone_source_boundary.cmake` additionally rejects a legacy
`tests/` directory, unit source references to product artifacts, missing test
tiers, and missing fixed-package integration source.
