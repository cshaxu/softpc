# T99 S36 CHAIN2 DWORD read closure

Owner accepted and approved closure on 2026-11-07. Implementation P1 is
8c27a3b6, committed and pushed to main. T99 remains open; no next S is admitted.

The two CHAIN2 DWORD readers retain their existing WORD calls, use consecutive
addresses (+2), and explicitly combine both halves. The dependent mode-1 odd
WORD next-group offsets were corrected without changing CHAIN2 layout.
Production +12/-20 (net -8); existing regression +33/-0. Public components,
configuration, guest media and snapshot format were unchanged.

Both EXEs were published. All 388 distinct x86/x64 test cases qualified,
including the documented x64 documentation-gate correction and rerun. The
actual committed source/test diff and verification evidence were reviewed in
the coordinator role before closure. See the finite coverage ledger and
limitations in [S36 evidence](../etc/evidence/softpc/m9-t99-s36-chain2-dword-read.md).

The independent GDP byte-mask alias and Unchained DWORD findings remain in
TODO; acceptance does not claim those defects or historic Win31 corruption
were repaired. Task scratch was removed and the build residue check passed.
This closure changes documentation only; no rebuild or repeated runtime tests
are needed for unchanged production and test artifacts.
