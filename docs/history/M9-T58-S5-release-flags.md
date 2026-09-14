# M9 T58 S5: Release Package Flags Acceptance

Owner request: “okay, can you please investigate and fix this in a new S task.
always reply in chinese”. The [proposal](../proposals/m9-common-test-sync.md)
retains the finite ledger, cause evidence, verification and package hashes.

Coordinator reviewed pushed executor 118734b against 5d213af. Exactly two
preset cache entries now specify the normal GNU -O3 -DNDEBUG Release flags.
The prior x86 cache was empty despite Release configuration; presets previously
did not replace that value. No tracked exception explains it, and the exact
local clearing operation is unknown. No product workaround was introduced.

The existing source-boundary check covers both preset entries, with positive
and empty-flags negative proof. Actual regenerated production flags include
optimization; product/Common/Lib tests still undefine NDEBUG. Generic custom
build behavior is unchanged. Both full builds succeeded; x64 full 85/85
(54.73 s), x86 full 85/85 (68.97 s). Documentation checks pass.

x86 decreased from 5,109,397 to 3,503,225 bytes (31.44%) without stripping;
x64 rebuilt byte-identically at 2,849,636 bytes. Both fixed packages were
built/tested; only x86 needs a binary diff. No src, shared test, INI or media
change. Config +2/-0; product test guard +13/-0; no production C/H added.

All ledger members are satisfied. S5 closes; T58 remains open for owner
feedback. This is not a claim of equal binary sizes across toolchains or of
recovering the missing local command history.
