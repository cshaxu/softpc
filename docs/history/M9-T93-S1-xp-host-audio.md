# T93 XP Host Audio

## Request and retained design

Owner objective: construct shared Lib Audio source/test capabilities, then
integrate the necessary XP NTVDM sound subset into standalone SoftPC in
single-agent executor/coordinator mode. Final T acceptance remains owner-owned.
The owner prioritizes XP behavior/source fidelity, selects host MIDI with
approximate restoration, and accepts XP-compatible FM silence when no native
raw-FM capability exists. No software synthesizer or instrument bank is admitted.

The [proposal](../history/M9-T93-xp-sound-blaster-standalone-proposal.md) holds the full
source/API census, ownership, estimates and serial S2-S8 plan.
[CURRENT](../states/CURRENT.md) is the only active packet.

## S1 design delivery

Executor 2860b484 is pushed. Its actual committed changes are two planning
files, +557/-181; production/tests and package artifacts are unchanged.
Documentation governance and git diff --check pass. The coordinator reviewed
the actual changes against the original requests and the pinned source-call
ledger. All 24 loaded WinMM names are dispositioned; unused pause/restart
entry points are not invented requirements. FM native availability is distinct
from guest status emulation. This is design evidence, not runtime acceptance.

The continuing implementation goal admits S2's pure PCM relocation. PCM
behavioral extensions, MIDI/FM, DMA, device attachment and snapshot remain
separate later steps. The proposed shared pause-hook change retains its
explicit implementation approval boundary. No full T completion is claimed.

Handoff correction: 83aa592f initially named this record without S1, so the
Continuation gate could not find a completed step. The shell sequence wrongly
continued to commit/push despite that failed gate. Rename the record with S1;
the documentation gate and diff check then pass. No production code ran or
changed during this correction. Future delivery commands stop on a nonzero
verification exit before staging or committing.
