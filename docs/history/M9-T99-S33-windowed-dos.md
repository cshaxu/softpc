# T99 S33 windowed DOS delivery

Owner accepted P4 (1ec0d551) and admitted S34. T99 stays open.
P1 repaired CHAIN2 plane addressing; P3 restored its uniform identity-call
pattern after the rejected P2 reduction. P4 corrected both text BIOS aperture
bounds through the existing BDA mode entry. No public-component change.

Both architectures passed 186 background and 8 native tests (388 total),
plus the real Win31 input/roundtrip/DIR/CLS probe. See
[verification](../etc/evidence/softpc/m9-t99-s33-windowed-dos-addressing.md).
Task scratch was removed. The separately recorded legacy odd dword-fill debt
remains in TODO; this acceptance does not claim it was repaired.
