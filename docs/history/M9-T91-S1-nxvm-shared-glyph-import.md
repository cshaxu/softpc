# M9 T91 S1: Shared glyph-contract import delivery

Status: implemented and verified; waiting for owner acceptance, not closed.
Executor P1: `62ec8e06e4c704fc46179abd40035a2b2c8e52cc`, pushed to main.
Baseline: `cc7365a686338d44b9537422ec3e77661bfebf48`.
Upstream: `73d7967244c1591038d0c2f4dcec819192d809a1`.
[Retained admission and finite ledger](M9-T91-nxvm-shared-glyph-import-proposal.md).

## Coordinator actual-change review

After the executor push, the same session switched to coordinator and reviewed
the committed source/test diff, owner-approved import boundary, test logs,
artifact hashes and six-root file/hash inventories. NXVM remains read-only,
clean and at the reviewed revision. All 246 paths match byte-for-byte, including
the manifests; 22 differ from the original SoftPC baseline, with no added or
deleted shared files. No external build/runtime dependency was introduced.

The App adaptation changes the existing glyph producer's destination slot
stride and capacity together; original plane addressing and the single copy
loop remain. Machine declarations agree and a compile-time assertion protects
the receiving contract. Default cursor height remains 16, distinct from the
new 32-byte storage capacity. No second renderer, cache, state or fallback path.

The similar-issue sweep covers both glyph banks, packed-16 offsets, font-height
fallbacks, frame validation/copy/change detection and receiving App fixtures.
Reviewed production hits are the Compat producer, Machine declarations and
driver default; all are adapted. Shared renderer/geometry and Console defaults
are covered by the exact upstream import. Original mirror and snapshot codec
are unchanged; user INI/media have zero diff. No new deferred item was found
within this bounded import audit. Existing Queue and TODO are unchanged.

## Measured component changes

Counts use `git diff --numstat cc7365a6 62ec8e06` and exclude artifacts.
Shared component figures include their README/manifest changes:

| Component | Files | Added | Removed | Net |
| --- | ---: | ---: | ---: | ---: |
| src/lib | 11 | 34 | 23 | +11 |
| src/emulator | 0 | 0 | 0 | 0 |
| src/x86 | 2 | 3 | 4 | -1 |
| test/lib | 5 | 55 | 15 | +40 |
| test/emulator | 2 | 3 | 3 | 0 |
| test/x86 | 2 | 8 | 3 | +5 |
| App production | 4 | 13 | 9 | +4 |
| App tests | 2 | 64 | 16 | +48 |

Code-only totals: production +28/-23 (net +5); tests +124/-31 (net +93).
Original mirror changes: zero files/lines. x86 EXE shrank 512 bytes; x64 size
is unchanged. Text-frame payload grows 8192 bytes; enclosing Window frame size
is unchanged because the existing graphics member remains larger.

## Verification and release

Both supervised Release builds succeeded and refreshed both package EXEs.
Shared background tests ran before other tests for each architecture.

| Architecture | Shared background | Other background | Native desktop | Total |
| --- | ---: | ---: | ---: | ---: |
| x64 | 84/84 | 48/48 | 5/5 | 137/137 |
| x86 | 84/84 | 48/48 | 5/5 | 137/137 |

Inventory per width: Lib 51, Emulator 21, X86 15, App unit 29, App integration
10 and checks 11. Native tests ran serially in the owner-approved reserved
period. No Computer Use, waived case or desktop retry. All supervisor and
phase exit files are zero. The first x64 run exposed a stale test expecting
17-row rejection; it was corrected to the approved 32-row contract and the
complete final suites rerun. This initial failure is retained in the proposal.

Focused proof includes actual two-bank font production, zero padding, last
glyph/last row, default and short/tall fonts, 33-row rejection, cursor percentage
and scaling conversion. Shared rendering also checks precise dirty output.
Documentation governance and whitespace checks pass.

Published SHA256:

- softpc32.exe: `7E8F74C30EED9C518D8BCB9052E8EB93CE0B59EC991C7C99C0643BBB7B84055F`
- softpc64.exe: `B8714346A29C3127B6B77EF7CE28D0D2205DE1C6D52E2E2EC616D7B18345198F`

Coordinator disposition: delivery satisfies technical qualification. Wait for
owner testing; neither S1 nor T91 is closed by this review.
