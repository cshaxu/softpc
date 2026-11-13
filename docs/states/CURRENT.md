# Project Status

## Current Work

No implementation subtask is active. Owner closes M9 T99 on 2026-11-12.
See [T99 completion audit](../history/M9-T99-completion-audit.md).
No next T or S is admitted.

## Current Technical Baseline

S43 implementation 51d0bc15 adds tools/Build.cmd: PowerShell-free x86/x64
build/publication via existing cached Ninja presets. Two dual-width runs and
four package integration tests pass. The owner requested reverting subsequent
local EXE changes; published files match the accepted 51d0bc15 delivery.


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
remains in [Queue](QUEUE.md), unnumbered and unadmitted. JIT/WHP discussion
did not admit S44 or a new implementation task.

## Recent M9 Closures

- [S43 build entry](../history/M9-T99-S43-build-entry.md).

- [S42 unused mirror retirement](../history/M9-T99-S42-unused-mirror-retirement.md).

- [S41 mirror cleanup](../history/M9-T99-S41-mirror-cleanup.md).
- [S40 retained-difference audit](../history/M9-T99-S40-retained-difference-audit.md).

- [S39 mirror wrapper cleanup](../history/M9-T99-S39-mirror-wrapper-cleanup.md).
- [S38 C-VID rules](../history/M9-T99-S38-cvid-rules.md).

- [S37 original-device audit](../history/M9-T99-S37-original-device-audit.md).
- [S36 CHAIN2 DWORD reads](../history/M9-T99-S36-chain2-dword-read.md).
| Task | Closure |
| --- | --- |
| T99 | [Completion audit](../history/M9-T99-completion-audit.md). |
| T98 | [Completion audit](../history/M9-T98-completion-audit.md). |

## Recent Governance

[Td S23 timestamp governance](../history/M9-Td-S23-build-timestamp.md) is closed.
Task scratch is removed; reusable Ninja/ccache roots remain. Follow existing
build-cache and task-owned probe cleanup rules.
