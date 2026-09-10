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
