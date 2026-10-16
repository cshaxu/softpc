# M9 T93 S3 — Neutral PCM contract

S3 is implementation-closed under the owner's sequential async delegation;
T93 remains open. Executor `b31b6e17` is pushed. This is not an owner manual
hearing acceptance. The [proposal](../proposals/m9-xp-sound-blaster-standalone.md#s3-executor-qualification)
retains the complete path/line ledger, qualification matrix and artifact hashes.

## Request and delivery

The admitted PCM contract requires native rate admission, accurate copied
accepted/submitted/consumed counters with reset epoch, event-driven progress
after the final producer input, short-prefix submission, stream-local channel
gain and native startup result. One existing worker/FIFO/WASAPI path remains.
The public options structure is unchanged; callbacks are registered explicitly.
Clear cancels old ranges instead of reporting them completed. Host consumption
does not claim physical audibility.

Production six C/H files: +299/-28, net +271. Four tests: +374/-20, net +354.
Mirror and App/Compat production delta: zero. Two manifests and both EXEs are
updated; user configuration, media and snapshots are untouched.

## Verification and actual-change review

Each architecture passes all 150 registered tests: 90 shared background,
55 other unit/integration/checks, and five native desktop tests. The three
focused PCM/first-delivery tests additionally pass 50 repetitions per width.
Native SDK injection proves endpoint mechanics; actual worker tests prove
root ownership and wake handling. No Linux playback claim is made.

The initial full build exposed one incomplete App native test override;
missing new symbols pulled the real native leaf into the probe link. Both
existing native overrides were audited and completed, preserving their real
worker/device paths. The final full runs pass after that repair. The x86 Ninja
log recovered from a premature end and rebuilt 584 steps; no cache purge was
requested. Owned transient S3 jobs terminated and their scratch was removed;
normal Ninja intermediates remain reusable.

After executor push, the coordinator separately checked the committed 16-path
diff, PCM public/private/native ownership, Types aliases, both App test
overrides, hash manifests, binary hashes and protected-path absence. No second
worker/backend, guest policy or uninitialized optional field was introduced.
The source epoch guard and pending-wait clear test are recorded without an
unsupported claim to exhaustive concurrency coverage. Actual-change review
accepts S3 against its packet and admits S4 under the continuing owner goal.
