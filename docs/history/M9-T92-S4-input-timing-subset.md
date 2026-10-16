# M9 T92 S4: Necessary XP Input And Timing Subset

## Request and boundary

Upgrade the selected SoftPC core to the fixed XP baseline, retaining necessary
local repairs and excluding foreign product/hardware implementations. The owner
also permits removal of proven useless existing conditionals. Async serial S
admission is authorized; T closure still requires owner acceptance.

## Delivery and verification

Executor `5c5af37a811eb8239beb4d32661a9ae9ec6b0a53` is pushed. The reviewed
18-body input/reset/IRQ/ROM/timing candidate set produces 16 changed bodies;
nt_uis.h adds the two-declaration dependency cleanup. Production is +5843/-8750,
net -2907 across 17 paths. Two tests are +164/-3, net +161. The independent
necessary-XP-subset patch is +1389/-189, including historical local repairs;
5416 excluded XP lines are separately ledgered, not called local patches.

Both Release builds publish the existing package EXEs. Each width passes
90 shared background, 53 other background and five native desktop tests,
148/148 total. All phase/supervisor exits are zero. The source-range audit
reconstructs all 18 selected XP bodies and verifies the tracked ledgers;
16 existing selected snapshot callback bodies remain unchanged modulo whitespace.
Actual controller tests cover translation, make/break, per-key commands and
snapshot roundtrips. No public package, INI, media or snapshot file changed.

- softpc32.exe SHA-256: `eaeae823c62d16a434908a33d8c16f3deb2d77cdb01d54ebe090c25af373ad83`
- softpc64.exe SHA-256: `49c0c725c852241c9c5fdd9e97a8cb0f131188664da8464071d58f88cf9e3ce9`

## Actual-change review and closure

The coordinator reviews the pushed commit, not only the executor test summary.
The 27 changed paths match the packet and count ledger. Necessary local native
pointer, InPort, snapshot, IRQ, ROM CS/mouse BX and audio endpoint repairs remain.
The speculative key-14 inverse patch was correctly removed after examining its
non-key classification; its no-output regression remains. XP idle threshold
narrowing is rejected for the selected int storage. Required tone callbacks
remain in the local-patch comparison instead of being excluded wholesale.

No new blocking finding remains. Initial fixture/merge mistakes, retained XP
whitespace warnings and the limits of source versus runtime evidence remain
explicit in the [proposal](../history/M9-T92-xp-sp1-softpc-mirror-rebase-proposal.md).
S4 closes under delegated async authority, without inventing manual owner
testing. T92 remains open; S5 is admitted for the video subset and preservation
of accepted C-VID changes.
