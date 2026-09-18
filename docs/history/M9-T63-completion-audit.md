# M9 T63: machine snapshot completion audit

T63 implements the owner-requested reusable machine checkpoint: a running
machine can be saved, a stopped one can load it, and the restored machine can
resume rather than repeat a long installation path. The owner has manually
accepted the final behavior, including Overlay media and paused-save handling.

## Finite closure ledger

The bounded universe is the twelve admitted S records in the retained
[proposal](M9-T63-machine-snapshots-proposal.md) and their implementation
records below. No unrelated Queue/TODO item is claimed by this closure.

| S | Delivered owner | Closure evidence |
| --- | --- | --- |
| S1 | feasibility, ownership and safety-boundary design | [record](M9-T63-S1-machine-snapshots.md) |
| S2 | safe checkpoint and state inventory | [record](M9-T63-S2-snapshot-boundary.md) |
| S3 | Storage/media format decision | [record](M9-T63-S3-storage-media-decision.md) |
| S4 | CCPU/SAS/RAM private archive | [record](M9-T63-S4-cpu-state-archive.md) |
| S5 | controller and queued-event archive | [record](M9-T63-S5-controller-archive.md) |
| S6 | remaining device archive | [record](M9-T63-S6-device-archive.md) |
| S7 | opaque Common-Machine transfer | [record](M9-T63-S7-machine-state-transfer.md) |
| S8 | App `save`/`load` command boundary | [record](M9-T63-S8-snapshot-command-boundary.md) |
| S9 | width-independent stream and restored display/input | [record](M9-T63-S9-streaming-snapshot-acceptance.md) |
| S10 | FDD/HDD Overlay payload | [record](M9-T63-S10-overlay-snapshots.md) |
| S11 | Window-visible raw-Console mouse exclusion | [record](M9-T63-S11-window-console-mouse.md) |
| S12 | paused-save checkpoint reuse/internal advance and prompt repair | terminal P16/P17 |

## Final contract

- `save` while RUNNING reaches the VM-owned safe checkpoint then leaves the
  product PAUSED. `save` while PAUSED reuses that checkpoint when retained, or
  privately lets the existing executor reach one without a public RUNNING
  fact or guest-input admission.
- `load` remains stopped-only and produces a normal PAUSED machine. Resume,
  reset and stop use their ordinary lifecycle paths; loading neither creates a
  Window nor captures mouse by itself.
- The one binary contains CPU, memory, controller/device and Overlay state.
  DIRECT/READONLY media are references validated by mode, geometry and
  content identity. FDD/HDD Overlay differences and FDD cylinder state restore
  before CPU/device state is resumed.
- Lib is unchanged by the snapshot feature. Common retains only its existing
  two opaque state-transfer operations and executor rendezvous; VM decides
  safe-checkpoint readiness. MVDM changes are narrow state import/export hooks;
  stream layout, transaction and host resource reconstruction remain VM/Compat
  responsibilities.

## Changed-path accounting

Reproducible scope: `git diff --numstat 54b2009..08f82ea -- src test`, grouped
by ownership root; documentation and package assets are excluded. It covers
81 tracked production/test paths.

| Root | Added | Removed | Net | Disposition |
| --- | ---: | ---: | ---: | --- |
| `src/mvdm/softpc.new` | 2626 | 1 | +2625 | 24 selected original files expose/restore existing machine state only; no new standalone lifecycle or renderer path. |
| `src/compat` | 2809 | 17 | +2792 | fixed-width archive, media effective-view archive, host checkpoint/timer and resource reconstruction. |
| `src/vm` | 449 | 9 | +440 | fixed stream transaction, driver state transfer and frame re-publication. |
| `src/common` | 250 | 20 | +230 | the admitted opaque read/write rendezvous and paused-save continuation; no snapshot format or safety policy. |
| `src/app` | 181 | 25 | +156 | command parsing, binary reader/writer, result text and prompt behavior. |
| `src/lib` | 46 | 3 | +43 | pre-existing task-era supporting change; no snapshot-specific API or implementation. |
| `test` | 2129 | 14 | +2115 | checkpoint, archive, media, cross-process, command and presentation proof. |

The original-source deltas are accounted by the S2–S9 receiver ledger in the
retained proposal: CCPU/SAS/FPU, controller queues, keyboard/mouse, media and
video state remain where the original machine owns them; VM/Compat own stream
format, host continuation, media leases and presentation rebuild. No duplicate
restore executor or App-to-Compat/MVDM production path remains.

## Verification and disposition

- Fresh final-source x64 CTest: 106/106 passed.
- Fresh final-source x86 CTest: 106/106 passed.
- Focused S12 command/provider/package tests passed in both widths after the
  prompt repair; the provider test asserts the immediate `SoftPC> ` prompt.
- Documentation governance and diff-whitespace checks passed for closure.
- Owner accepted the final packages and explicitly authorized closure.

The two existing TODO entries remain outside T63: intermittent x86 BIOS tick
observation and the original keyboard-table upper-bound defect. Queue order is
unchanged. T63 is closed at terminal commit `08f82ea` plus this closure record.
