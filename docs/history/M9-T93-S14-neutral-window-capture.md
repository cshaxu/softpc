# M9 T93 S14 — Neutral Window capture ownership

Owner accepts S14 and admits S15: "批准 验收通过 准入下一个s".
Executor fe6d9fe3, review 4aacc52e, corrective executor 683f85d4 and corrective
review e2de3d3f are pushed. Original requests, seven-area ledger, full failure/
retry evidence and corrected qualification remain in the
[S14 proposal record](../history/M9-T93-xp-sound-blaster-standalone-proposal.md#s14--capture-boundary-and-shared-contract-completion-audit).

Lib serializes native mouse registration query/update without holding the lock
across message-dispatching capture/focus calls. Native registration stays the
sole authority; there is no shadow ownership registry or new public API.
The downstream audit caught incomplete cleanup after successful capture followed
by focus loss. The existing rejection path now releases only its own capture,
always attempts unbinding and forwards cleanup errors. The regression fails
before that repair and passes afterward. No mouse motion algorithm changed.

Final S14 code against e85a0015: production +43/-21, net +22; tests +119/-5,
net +114. Three production files and one fixture; README/manifests, governance
and binaries are counted separately. Final cached builds and full tests pass
167/167 on each width, zero skips, including ten native desktop cases. Earlier
host-audio/session failures are not erased or described as product repairs.
No public backend completion claim; third-party raw input mixing restrictions
remain explicit in Windows integration documentation.

Both package EXEs are published; hashes are in the correction record. User INI,
media, snapshots, App/mirror and Emulator/X86 remain unchanged. S14 is closed;
T93 stays open. S15 is the only newly admitted task; no S15 implementation or
qualification is claimed by this handoff.
