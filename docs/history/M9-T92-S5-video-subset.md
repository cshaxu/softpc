# M9 T92 S5: Necessary XP Video Subset

## Request and boundary

Upgrade the selected video core to the fixed XP baseline; retain local ABI,
dirty/geometry, snapshot and detached-renderer repairs. Exclude unneeded
PC-98/regional and NT host paths, including proven unused existing branches.
No public component, new renderer, sound device or user asset change.

## Delivery and evidence

Executor `41aa0bc24bac4de61c6a5dd32b4fefb78bde95bb` is pushed. The 71-path
review changes 27 mirror files, +3,644/-6,544 (net -2,900); 44 remain unchanged,
including 40 C-VID paths. One existing test adds 73 lines and removes none.
The pinned XP subset excludes 12,095 lines; current-to-subset patches are
+22,908/-23,257, including historical C-VID ABI patches, not just this delivery.
All 71 fixed upstream bodies, disjoint ranges and subset hashes independently
reconstruct; both existing video snapshot callbacks remain unchanged modulo
whitespace. The linked [proposal](../proposals/m9-xp-sp1-softpc-mirror-rebase.md)
retains the complete range/count ledger and semantic dispositions.

Both Release builds and 148/148 tests per architecture pass: 90 shared, 53
other background, five reserved native desktop tests. Actual video BIOS calls
verify 256 DAC entries, modified-palette restoration and memory boundaries;
existing mode/dirty/snapshot/package tests remain. No Computer Use or invented
manual guest verification. Documentation and non-mirror whitespace gates pass;
19 explained XP/selected-tail whitespace diagnostics are retained.

- x86 EXE SHA-256: `f35cb644c0bead39baf7b2d29f8fa51c8ca44323c40adf2c2edae8dfc9003a38`.
- x64 EXE SHA-256: `6e57b7e29088fdda0c80b219bdf6cdf1dd3eda7e013e4a9bb065c93eae8f6423`.

## Coordinator review and closure

Actual pushed changes match the admitted source/test/artifact/ledger scope.
Selected renderer and original marker paths remain single-owner; unused native
NT routes are distinct from the application's debugger and KVM Window. No
blocking finding remains after source, range, artifact and test evidence review.
Origin/main matches the executor delivery. S5 is implementation-closed under
the owner's async delegation; this does not claim fresh manual acceptance.
T92 remains open and S6 is admitted for final baseline accounting and gate
qualification. Owner confirmation remains required before T closure.
