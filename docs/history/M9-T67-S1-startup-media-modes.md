# M9 T67 S1 — independent startup media modes

## Boundary and delivery

Owner admitted independent `floppy_mode` and `hard_disk_mode`, and later
required removal rather than compatibility support for `media_mode`.
Implementation commit [`b88aa2b`](https://github.com/cshaxu/softpc/commit/b88aa2b)
is pushed to `origin/main`.

App owns parsing and copies two values into VM options. VM owns fixed-machine
options and passes each device policy to Compat. Compat's existing snapshot
archive validates its existing per-slot mode field against the corresponding
live floppy or hard-disk mode. No archive bytes, Common API, Lib, or preserved
machine source changed.

`media_mode` is an unknown configuration key. The shipped configuration now
uses `floppy_mode=overlay` and `hard_disk_mode=overlay`, explicitly authorized
by the owner; no media or other INI setting changed.

## Finite ledger

| Candidate | Disposition | Proof |
| --- | --- | --- |
| Shared startup option | Removed | App, VM and machine option structs carry two copied mode values. |
| Parser alias | Rejected | `config_smoke` proves `media_mode=overlay` fails. |
| Independent startup attach | Implemented | HDD uses `hard_disk_mode`; floppy uses `floppy_mode`. |
| Snapshot shared-mode validation | Removed | Archive receives two modes and maps them to its existing A/B versus C/D slots. |
| Archive format | Retained | No format/version/order field changed. |
| Common / Lib / MVDM | Retained | No source diff. Interactive insertion is S2. |

## Actual accounting

Tracked source accounting excludes documentation, INI and EXEs:

| Surface | Added | Removed | Net |
| --- | ---: | ---: | ---: |
| App | 24 | 11 | +13 |
| VM | 15 | 9 | +6 |
| Compat | 14 | 9 | +5 |
| Production total | 53 | 29 | +24 |
| Tests | 69 | 28 | +41 |
| Lib / Common / OpenNT mirror | 0 | 0 | 0 |

The net production addition is the necessary second policy value at each of
the three existing ownership boundaries. It introduces no state owner, generic
mode registry, compatibility flag or alternate attach route.

## Verification

- Focused config, archive, snapshot-transaction and package suites: 4/4 on
  both x64 and x86.
- Full CTest suite: 107/107 on x64 and 107/107 on x86. The slow
  `library.types-layout-selftest` was allowed to complete independently on
  each width (39.28 s and 38.76 s) after bounded batch execution.
- `Verify-DocumentationGovernance.cmake`, diff whitespace check, and search
  for production `media_mode` consumers passed.
- x64 and x86 package presets refreshed
  [softpc64.exe](/O:/repos.hobby/softpc/assets/binary/softpc64.exe) and
  [softpc32.exe](/O:/repos.hobby/softpc/assets/binary/softpc32.exe).

S1 is closed. T67 remains open for explicit transactional floppy insertion in
S2.
