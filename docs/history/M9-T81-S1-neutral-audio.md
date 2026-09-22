# M9 T81 S1: Neutral Audio Design And Review

## Request And Admission

The original owner request is retained verbatim in the
[proposal](M9-T81-neutral-audio-proposal.md). Owner admits design, task split,
commit/push and S1 closure only; later implementation awaits owner review.
Baseline: clean `2b17749a`. The S1 executable packet is preserved in executor
commit `c13630a5`, whose push to origin/main succeeded before this review.

## Actual-change Review

Coordinator reviewed `git show c13630a5` and all 157 proposal lines after
executor delivery. The only changed paths are the proposal, CURRENT and QUEUE.
The proposal is now retained here because S1 closes and no packet is active;
this archives the design deliverable, not T81 itself. Future admission must
restore its proposal pointer for the active packet. No rule changes required.

| Owner requirement | S1 disposition |
| --- | --- |
| Neutral src/lib/audio and test/lib/audio | Exact paths and ownership defined; implementation assigned S2/S3 |
| Function completion for NES adoption | PCM, bounds, short blocks, clear, failure and cleanup specified; isolated proof S4 |
| XP sound-card support later | Explicit non-goal; no XP import or Core change |
| Proposal and S task split first | Four S tasks, estimates, finite coverage and exit standards recorded |
| Commit/push and close S1, wait | Executor pushed; coordinator closes design only; S2-S4 unadmitted |

The design does not claim NNES's old call sites require no adaptation, Linux
playback works, or SoftPC gains a Windows guest sound card. Fixed slots may
fill with short blocks; this deliberate tradeoff avoids a second queue.
Native ownership/cleanup proofs remain mandatory S3 work, not proven by S1.
Existing machine pacing and the Beep path are untouched.

## Verification And Counts

- Documentation governance and `git diff --check` pass for executor delivery.
- Executor documentation: +182/-8, net +174 across three paths.
- Production/test/build code: +0/-0, net 0. EXEs, INI and media unchanged.
- No build, runtime, audible or downstream integration test was run; none is
  warranted by a documentation-only change. Future evidence is not claimed.
- Similar-design risks (duplicate queues, short-block starvation, silent output
  failure, incomplete native flush, platform leakage) all have explicit S2/S3
  ledger receivers; no unexplained reference behavior is adopted.

S1 exit criteria are met. T81 remains open pending owner design review;
its implementation and whole-task acceptance criteria are not yet met.
