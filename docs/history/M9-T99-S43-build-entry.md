# M9 T99 S43: build entry closure

Owner accepts and explicitly closes S43 on 2026-11-11, keeping T99 open.
Implementation 51d0bc15 is committed and pushed. No next S is admitted.

The original request is a double-click tools/Build.cmd that builds and publishes
32-bit and 64-bit packages without PowerShell in the build chain. The entry
adds 42 batch lines, uses existing CMake/Ninja/ccache presets and publication,
checks tools, stops on failure with its exit code, and pauses unless --no-pause
is supplied. README documents requirements and partial-publication behavior.
No runtime, mirror, shared-component, preset or CMake implementation changes.
Production src and test code added/deleted counts are both zero.

All frozen validation units pass: missing dependencies, invalid arguments,
configure/build failure propagation, spaced/! path, pause behavior, two real
dual-width builds, cache reuse, timestamp refresh, PowerShell-free Ninja target
commands, unchanged user assets, and four package integration cases. See
[S43 design and evidence](../history/M9-T99-public-handoff-simplification-proposal.md#t99-s43-double-click-build-entry-audited-design).
The full runtime suite was not rerun for this build-entry-only change.

Coordinator review confirms a single build/publication owner with no hidden
PowerShell runner, duplicate copy step, cache purge or configuration rewrite.
No unresolved defect was found in this bounded review. Cleanup and its
PowerShell selftest remain independent maintenance tools, outside Build.cmd.

This closure changes documentation only. Documentation, whitespace and build
residue checks pass; no redundant build is run. Two post-delivery local EXE
modifications are preserved outside this closure commit, without claiming
the implementation delivery hashes describe those local files.
