# M9 T72 S2: Neutral Text Migration

## Admission And Result

Owner approved the parallel-array design and execution to T-level audit readiness,
reserving formal T closure. Baseline e0ad4e8f; complete implementation delivery
12bf7c96 was pushed to origin/main. The ten-path finite ledger and test mapping
are in the [proposal](../proposals/m9-kvm-text-cell-glyph-refactor.md#八s2-有限收敛账本).

VM decodes device attributes once, Common status text writes neutral values,
and Window/Console consume independent foreground/background/bank arrays.
Only Win32 Broker packs native output colours. The old shared attribute fields
are removed, not aliased. Both fonts/maps, fixed capacity, glyph indexing and
current intensity/background output are preserved. Source ABI changes together.
No new allocation, cache, state machine, queue, thread or resource lifecycle.

## Evidence And Accounting

Both Release builds pass, with strict Lib C17 warnings on both widths. Serial
background presets: x64 105/105 in 164.85s; x86 105/105 in 147.06s. Included are
512-case real VM decoding, leaf pixels/maps, 256 native colour encodings,
invalid-frame rejection, resource-only publication, sizeof assertions, snapshots,
restart and headless Win3.1 full/window PIF roundtrips. Five desktop tests per
width were not run; no native Linux or current visual acceptance claim.
Documentation governance, four shared manifests/DAG and diff --check pass.

Counts: git diff --numstat e0ad4e8f 12bf7c96, C/H only:

| Scope | Files | Added | Removed | Net |
| --- | ---: | ---: | ---: | ---: |
| Lib production | 7 | 35 | 22 | +13 |
| Common production | 2 | 4 | 4 | 0 |
| VM production | 1 | 11 | 6 | +5 |
| Production total | 10 | 50 | 32 | +18 |
| Tests | 10 | 132 | 7 | +125 |

Four manifests +25/-25, documentation and binaries are separate. No MVDM,
Compat, configuration, media or snapshot-format change. The estimates were
production +83/-40 and tests +180/-35; reuse of existing loops/checks kept the
actual patch smaller without omitting the finite acceptance matrix.

Base/Window-text/Console-text sizes are 8084/16276/9108 bytes (+1996 each).
Window full frame 984084 and Common frame 985112 are unchanged. Logical Console
is unchanged at 8084; S1's 8080 estimate was a four-byte arithmetic error, caught
by the first assertion rather than hidden. The first strict build's integer
promotion warning was fixed without weakening warnings. Final reruns pass.
The bounded validation probe measured 1.931 us x64 / 1.709 us x86 per 2000-cell
call, not an end-to-end speed claim; call-site accounting is in the proposal.

Packages and hashes are in the proposal's S2 P1 delivery table: x86 3659288 bytes
(+4096), x64 3063343 (+4608). Owned build/t72-proof probes/logs were removed after
recording; existing incremental build trees and all owner inputs remain intact.

## P2 Coordinator Review

After the P1 push, the same agent switched to review and inspected committed
production/tests/docs/manifests, compared the actual path list to the packet,
rechecked removal/ownership of the old fields, visible-cell bounds, successful
cache updates and font-read failure propagation. HEAD equalled origin/main and
the worktree was clean before this record. No unreviewed production path or
parallel implementation was found within the finite scope.

S2 meets its exit criteria and closes. S3 is admitted by the existing owner
authorization for audit only; formal T72 closure remains reserved for owner review.
