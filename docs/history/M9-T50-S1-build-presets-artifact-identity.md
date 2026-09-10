# M9 T50 S1 — Build presets and artifact identity

## Objective

Provide versioned, reproducible CMake preset entry points for the fixed Windows
x64 and x86 packages, and reject an artifact architecture declaration that does
not match the selected compiler.

## Scope

- Add release configure/build/test presets for x64 and x86.
- Use `SOFTPC_I686_GCC` as the explicit local x86 compiler input.
- Validate declared architecture against CMake's actual pointer width.
- Document the canonical commands and add a structural verifier.

## Non-goals

- No change to machine, lib, media, user `softpc.ini`, package names, or
  compiler/toolchain installation.

## Exit criteria

- Both presets configure/build/test on this host and produce only their fixed
  width-correct executable names.
- Architecture mismatch is rejected and preset shape is machine-checked.
- x64/x86 regression passes; changes are committed and pushed cleanly.

## Completion evidence

- `CMakePresets.json` provides the two release configure paths plus separate
  package and complete-test build presets.
- `SOFTPC_I686_BIN` and `SOFTPC_I686_GCC` make the x86 compiler, resource
  tools, and make program an explicit matching toolchain rather than an
  accidental host-PATH choice.
- CMake rejects an explicit artifact architecture that disagrees with the
  selected compiler pointer width.
- Fresh x64 and x86 preset builds each passed all 36 CTest cases and refreshed
  only their fixed package executable. `softpc.ini` and media were unchanged.
