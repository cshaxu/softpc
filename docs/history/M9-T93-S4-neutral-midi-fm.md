# M9 T93 S4 — Neutral MIDI Output And FM Capability

## Request and boundary

The owner's active goal is to build reusable Lib audio source/test capabilities,
then attach the selected XP NTVDM sound devices to standalone SoftPC. Sequential
S execution is delegated; T closure still waits for owner acceptance. S4 owns
host MIDI and truthful raw-FM capability, not guest synthesis or XP device code.
The [proposal ledger](../history/M9-T93-xp-sound-blaster-standalone-proposal.md#s4-implementation-and-qualification-ledger)
preserves the frozen coverage universe, API decisions, estimates, source
references, exact qualification and package hashes.

## Executor delivery

`46ad2e3f` is committed and pushed to main. Production C/H +444/-0, net +444,
across eight paths; test C +356/-0, net +356, across four paths. Source build
and edge checks add +22/-5; test registration/matrix +8/-2. Documentation,
manifests and two published EXEs are separately accounted. The mirror and
existing PCM/Compat/Machine/Emulator implementation are unchanged.

The transport has four fixed copied SysEx slots and native completion/cancel
events, no additional worker, synthesizer, polling or second device parser.
Ordinary short messages preserve prior SysEx order; realtime output can
interleave. Reset returns native buffers and sends EOX through the existing
long-message path when needed. Failure retains native-owned storage until
cleanup succeeds. FM acquisition explicitly returns UNSUPPORTED/null.

Both architectures pass all 154 tests: 94 shared, 55 other background and five
native desktop cases. The three isolated MIDI admission/unsupported/native-mock
tests each additionally pass 50 repetitions per width. Real default MIDI
open/release succeeds on both widths, without an audible-output claim. Both
Release EXEs are refreshed. Manifest, dependency/Types, documentation and
whitespace gates pass. The first test-only output-helper typo and the x86
Ninja log recovery are retained in the proposal, not erased by final passes.

## Coordinator actual-change review

After executor push, the same agent switched to coordinator and inspected the
actual commit/stat/diff, complete native MIDI implementation, copied-message
admission, unsupported implementation and native failure fixture against the
packet and original request. The Types/build edges and exact negative edge
matrix are included, not just the executor summary. HEAD and origin/main both
resolve to `46ad2e3f` at review; no uncommitted user changes were present.

The review confirms one native output owner, bounded copied storage, no public
native headers, no unapproved guest behavior or shared Emulator change, and
no fake FM success. Existing PCM retains its distinct WASAPI stream contract;
it is not another MIDI route. Tests exercise real adapter control flow with
native-call substitution, while the native probe separately establishes host
availability. Linux implementation compiles in the unsupported fixture;
Linux runtime and manual sound are not claimed. No unresolved S4 finding is
silently transferred or declared fixed by narrow evidence.

S4 is closed under delegated sequential acceptance. S5 is admitted next for
the already-planned original DMA decrement/wrap prerequisite; no S5 code is
part of this closure. Owned S4 scratch may be removed after confirming all
jobs have terminated; retained evidence is this record and the proposal.
