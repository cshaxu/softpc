# M9 T83 completion audit

Owner request: close T83 and admit T84 for 80x50 text-frame support.
Closure baseline is `1abcb0f2`; working tree and origin/main were synchronized.
This closure is documentation-only and reuses recorded implementation evidence.

## Finite completion ledger

The frozen universe is the six admitted S scopes, including the owner snapshot
refresh. Completion means every scope has a delivered implementation and its
recorded verification, with no unfinished implementation silently transferred.

- S1: twelve CRLF checkout corrections and owner-supplied snapshot refresh;
  `b44942f3`/`6e0f09b9`. [Record](M9-T83-S1-crlf-and-snapshot.md).
- S2: product and test relocation into app-softpc, preserving mirror contents;
  `0e91ada1`. [Audit](M9-T83-S2-layout-closure-audit.md).
- S3: C-VID selector declarations and BIOS channel-2 pointer definitions;
  `ff4c54f9`. [Audit](M9-T83-S3-cvid-closure-audit.md).
- S4: App Types/Base convergence with explicit mirror ABI bridge exemptions;
  `3ae07afb`. [Audit](M9-T83-S4-lib-types-base-closure-audit.md).
- S5: exact 227-path six-component import from NXVM `440ae83bc`;
  `b1d5ece6`. [Audit](M9-T83-S5-nxvm-six-component-import-audit.md).
- S6: remaining read/write glue and EGA dot-read declarations, shared header
  ownership, duplicate test declaration removal; `776c02e2`/`f7ebb19f`.
  [Audit](M9-T83-S6-cvid-abi-closure-audit.md).

The actual endpoint diff was inspected with rename detection. Mirror source
changes beyond relocation/EOL are confined to the declaration corrections in
cga.c, evidgen.h, cpu_vid.h, egavideo.h, ega_read.c and ega_writ.c; no device
algorithm, WOW diagnostic or legacy mover isolation was imported. Product
changes are the recorded layout and Types/Base migration; shared changes are
the recorded NXVM import. Generated package EXEs and the owner snapshot are
separate from source accounting. Queue retains its unadmitted XP proposal;
TODO is empty. The text-capacity request is explicitly owned by T84.

## Evidence and limitations

S5 final complete suites passed 125/125 on each width. Its initial native-modal
failure and successful reruns remain disclosed in its audit. The latest S6
Release builds pass; background suites passed x64 120/120 and x86 116/116.
Those counts are reported as recorded, not asserted to be identical test
inventories. Desktop-labelled tests were excluded for S6; no new runtime,
Linux or downstream NTVDM64 acceptance claim is made by this closure.

Endpoint accounting command: `git diff --numstat -M 07c53c9c 1abcb0f2 -- src test tools CMakeLists.txt`.
Numeric entries excluding manifests total +5409/-4992, net +417. This includes
build/check scripts and tests, counts rename-only entries as zero, and excludes
documentation, binary artifacts and manifest metadata.

Accepted EXE SHA-256 values remain:

- softpc32.exe: `48FF9108A2873F61AB98E45A07B05CD6F8DEFFC1488DEB3F24658EE3730FC08E`.
- softpc64.exe: `FCEA577D1B3B799C88FE84AB43F2D7FB90F3E9BA83EE42BD9C93C3D5C7857046`.

The owner explicitly approves T-level closure. No implementation is reopened
by the new capacity task; accepted binaries and media remain untouched here.
