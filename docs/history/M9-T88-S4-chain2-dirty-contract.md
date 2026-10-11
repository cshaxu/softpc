# M9 T88 S4 acceptance and S5 admission

The owner accepts the S4 delivery by admitting the next S on 2026-10-11.
Executor 7cda5375 is pushed; the actual-change review confirms seven original
production files, one existing VGA test and one source gate, together with
the evidence/state documents and two EXEs. No shared component, INI, media
or snapshot changes are present.

All 28 inline CGA dirty blocks use the existing selected-marker ABI;
56 obsolete labels and their wrappers are removed. Original memory-write
algorithms remain, including the interleaved Copy address calculations.
CHAIN2 plane/bank conversion reuses the original Unchained bitmap marker.
Production +332/-1113 (net -781), test +89/-0, gate +21/-0; combined code
net -671. Full OpenNT touched-line delta increases by 1053, not decreases.
The finite rule ledger and limits are in the
[proposal](../proposals/m9-win31-file-manager-vga-width-transition.md#s4-implementation-and-source-accounting).

Dual Release builds and background 131/131 per width pass. At the owner's
subsequent request, native desktop tests run serially without Computer Use:
x64 5/5 in 12.19 s, x86 5/5 in 11.64 s. The complete registered selection
therefore passes 136/136 per width. Existing artifact hashes are unchanged by
this test-only run. The separate old nested compound-rule failure remains in
TODO and is not represented as repaired.

S4 is closed; T88 remains open. S5 is admitted for design first, with no
production modification made by this handoff. Its packet is owned by CURRENT.
