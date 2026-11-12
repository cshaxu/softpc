# Project Status

## Current Work

M9 T99 S43 P1 is verified, awaiting owner acceptance: tools/Build.cmd builds and publishes both architectures without PowerShell.
S42 remains owner-accepted and closed. Implementation follows the audited minimal design.

## M9 T99 S43 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner approves implementation and testing of a double-click tools/Build.cmd for x86/x64 build and publication without PowerShell anywhere in that build chain. |
| Objective | Reuse existing CMake/Ninja/cache/publication ownership through a minimal cmd.exe entry. |
| Non-goals | No runtime, mirror, shared-component or snapshot changes; no replacement build system or automatic tool installation. |
| Reference Baseline | 6acc1193; clean worktree at admission. |
| Candidate Proposal | [T99 retained scope and S43 design](../proposals/m9-public-handoff-simplification.md). |
| Files And ABI Surface | tools/Build.cmd and README build usage; existing presets, timestamp and publication wiring audited. No ABI change. |
| Applicable Rules | Execution and documentation governance; architecture/coding rules before implementation. |
| Verification | Two real dual-width build runs, entry failure/pause/path fixtures, complete Ninja product-command scan, four package integration tests and asset hash preservation pass. |
| Expected Markers | cmd.exe -> CMake -> Ninja -> compiler/ccache/linker; CMake owns EXE destination and timestamp check. |
| Asset Needs | Implementation will refresh only softpc32.exe and softpc64.exe; preserve INI, snapshots and media. |
| Reporting Requirements | Report actual entry size, no-PowerShell build chain, verification scope and published artifacts. |
| Stop Conditions | No duplicate publication, hidden PowerShell wrapper, automatic dependency download or destructive cleanup. |
| Exit Criteria | Implemented, reviewed, built and tested; commit/push this P, then wait for owner acceptance. |
| Original Owner Request | 在 Tool 里面建立一个 Build.cmd，双击执行32位和64位构建和发布；整个构建禁止使用 PowerShell；先看看怎么弄。 |
| Similar-Issue Sweep | Inspect root/component CMake, presets and tool commands for PowerShell dependencies; distinguish build from cleanup selftest. |

Verification used hidden Python-launched cmd.exe jobs, no PowerShell runner.
Both build rounds passed with zero compiler warnings and fresh timestamps;
each second-round architecture executed four timestamp/relink steps only.
Entry fixtures and four package integration tests pass. Source runtime did not
change; the full runtime suite was not rerun for this build-entry-only change.
INI/snapshot/media hashes are unchanged. Owned jobs have finished; disposable
build/t99-s43 is removed before delivery. Reusable caches remain.

## Current Technical Baseline

S42 final implementation 60bdbaa4 removes unused mouse_io.c, nop.c, vglob.c
and their obsolete wiring/state. Both published EXEs pass 392/392 tests.
Snapshots now use SCP3; older SCP2 files are rejected without compatibility.
The retained mirror has 472 files. User media/configuration are preserved;
the owner-provided assets/binary/snapshot update ships in S42 P6.


S41 delivery ecf263a8 completes the five admitted mirror cleanup/guard units:
production +16/-29, with XP mirror added+deleted diff reduced by 13 lines.
Snapshot command IDs and the existing device paths remain unchanged.
See [S41 closure](../history/M9-T99-S41-mirror-cleanup.md).

Td S23 delivery db76bb60 repairs the cached compiler timestamp, updates
execution rules and verifies embedded EXE literals. Both architectures pass
188 background plus eight native tests each: 392/392. Published EXEs carry
Nov 8 timestamps. See [timestamp closure](../history/M9-Td-S23-build-timestamp.md).
Only Emulator build configuration/verifier/manifest changed within the public
eight trees; no fresh NXVM alignment is claimed.

[TODO](TODO.md) has no deferred entries. The runtime-performance candidate
remains in [Queue](QUEUE.md). T99 still requires its overall completion audit
and owner-approved closure.

## Recent M9 Closures

- [S42 unused mirror retirement](../history/M9-T99-S42-unused-mirror-retirement.md).

- [S41 mirror cleanup](../history/M9-T99-S41-mirror-cleanup.md).
- [S40 retained-difference audit](../history/M9-T99-S40-retained-difference-audit.md).

- [S39 mirror wrapper cleanup](../history/M9-T99-S39-mirror-wrapper-cleanup.md).
- [S38 C-VID rules](../history/M9-T99-S38-cvid-rules.md).

- [S37 original-device audit](../history/M9-T99-S37-original-device-audit.md).
- [S36 CHAIN2 DWORD reads](../history/M9-T99-S36-chain2-dword-read.md).
| Task | Closure |
| --- | --- |
| T98 | [Completion audit](../history/M9-T98-completion-audit.md). |

## Recent Governance

[Td S23 timestamp governance](../history/M9-Td-S23-build-timestamp.md) is closed.
Task scratch is removed; reusable Ninja/ccache roots remain. Follow existing
build-cache and task-owned probe cleanup rules.
