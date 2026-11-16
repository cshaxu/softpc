# Long-Term Review Ledger

CURRENT owns active work; the runtime-performance candidate remains in Queue.

## P2: package Console startup interruption

- Observed in T100 S4 P84: x86 package/compact tests pass in isolation but
  sequential starts sometimes interrupt before Console attachment. Captured
  stage=1, error=6, child exit=0xc000013a; desktop remained Active. No orphan
  product process. This is not diagnosed as an RDP disconnect or DSP regression.
- P86 repeats the compact startup interruption on x64 and reports stage3/error0,
  child 0xc000013a on x86 before the one-line source restoration. Both pass
  isolated against final publications. Root cause remains unproven; see
  [P86 results](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p86-nt_cgac-retained-difference-proof).
- Admission condition: owner admits a separate package/host Console lifecycle
  investigation; reproduce and compare baseline before proposing any repair.
- Evidence: [P84 results](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p84-restore-dsp-internal-ownership).

Shared-fixture timing TEST01 was resolved and accepted
in [T100 S2](../history/M9-T100-S2-candidate-review.md). Remaining mirror approval
decisions and the unfinished full classification belong to the open T100
[proposal](../proposals/m9-xp-mirror-original-first-audit.md#remaining-work-after-s3-owner-closure),
not TODO. S3 is owner-closed; the owner has admitted S4 for that review.
