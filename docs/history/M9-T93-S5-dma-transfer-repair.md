# M9 T93 S5 — DMA Decrement And Wrap Repair

## Request and delivery

The owner's complete goal is reusable audio support followed by XP NTVDM-parity
standalone sound. S5 is its admitted DMA prerequisite, not a replacement sound
device or a claim that XP audio is already integrated. Sequential S closure is
delegated; T closure still waits for the owner.

Executor `a34f858f` is pushed to main. One production file changes +28/-15,
net +13; one test file changes +107/-5, net +102. The existing two directional
helpers now copy controller units through SAS and advance host buffers; the
decrement split includes address zero. No new state, header, shared component
or snapshot format is introduced. XP selected-subset diff changes from
+81/-1 to +109/-16; the unchanged subset hash and reconstruction method are in
the [proposal ledger](../proposals/m9-xp-sound-blaster-standalone.md#s5--original-dma-prerequisite-repair).

The new regression fails against the old code and passes the repair on both
widths. Its 60 transfer cases and eight existing reload combinations cover
actual data, canaries, wrap, controller width/direction, VERIFY, partial/zero/
disabled/masked operations, TC status and mapped ROM. All three production
callers use a forward host buffer. Tests do not claim physical DMA hardware
coverage or real Sound Blaster guest playback.

Both Release EXEs are published. Each architecture passes 94 shared and 55
other tests plus all five original desktop tests in independent CTest
processes. Grouped desktop attempts exposed intermittent pre-main child exit
0xC0000142; diagnostic evidence and the unresolved follow-up are retained in
TODO, not hidden by the isolated passes. No test assertion or product startup
behavior was weakened. User configuration, media and snapshots are unchanged.

## Coordinator actual-change review

After executor push, the same agent switched roles and read the actual Git
commit, production and test diff, path statistics and remote identity. The
review checked inclusive decrement wrap, unit-preserving word order, forward
host bounds, existing SAS ROM authority, unchanged reload/TC owner, and that
the expected matrix does not call the production address helper. The mirror
manifest retains the pinned upstream/subset identities and updates only the
reviewed local identity. No public eight-directory file changed.

HEAD and origin/main both resolve to a34f858f at review. The finite S5 criteria
are met with the native-test isolation qualification explicitly disclosed.
S5 closes under delegated sequential acceptance; S6 is admitted for exact
XP DSP/PCM/Mixer subset review and integration. Its shared Emulator pause-hook
behavior remains separately approval-gated. S5 scratch is disposable after
all owned processes terminate; durable evidence is retained here and in the
proposal, while Ninja build trees remain available.
