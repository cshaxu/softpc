# M9 T93 S8 — XP MIDI/FM Attachment

## Request and delivery

The owner accepts S7 and admits S8: "验收通过，准入s8". The continuing
goal and delegated sequential acceptance permit subsequent S work without
waiting for separate manual acceptance; formal T closure still waits.

Executor `980984d0` is pushed. The necessary XP MIDI/FM subsets use the existing
I/O tables and executor. One parser constructs bounded complete MIDI messages;
Compat owns host output, cancellation and optional volume. FM implements the
approved XP no-hardware status semantics, not a software synthesizer. No public
eight-directory source/test changes, user INI, media or snapshot changes occur.

Production C/H +715/-14 (net +701), tests +599/-8 (net +591). Mirror production
is +501/-11; Compat +205/-3; Machine +9/-0. Six changed mirror files have a
cumulative +1120/-2239 difference against their pinned necessary XP subsets,
including prior S6 patches; this is not the S8 incremental cost. The range
ledger separately excludes 1283 upstream lines. Root build +39/-1 and mirror
checker +1/-1 are separate from production/test C/H counts.

## Verification

The fourth complete qualification passes 165/165 per width, with zero skips:
95 shared, 65 other background tests and five native desktop cases. Both EXEs
are refreshed. Each of the two affected real stop integrations also passes
twenty repetitions per width. The finite eight-unit proof ledger is in the
[proposal](../proposals/m9-xp-sound-blaster-standalone.md#s8-complete-fourth-qualification).

Earlier failures are retained: unequal first/second sound-count windows were
corrected in the test; faulted MIDI cleanup and paused idle service were repaired
in their existing owners. A later checkpoint_smoke timeout was not reproduced
in ten isolated repeats, the unchanged parallel suite, or the fourth full run.
Its root cause is unproven and remains explicit test-reliability debt in TODO;
no timeout enlargement or product workaround was used.

Artifacts:

- x86 SHA-256: A059BAF16D5A630186FEC46895E7ACAF12E19BC2C593E464B8E27FE544D6BA35
- x64 SHA-256: BA980E0F46C63BF7F1DD86F872BA08E05102727DCE36368597D67F2E74C473E8

Native MIDI integration executes the real parser/Compat/Lib output path, not
acoustic measurement. No Windows guest-driver or audible FM acceptance is
inferred. Active sound-state serialization remains S9.

## Coordinator actual-change review

After executor push the same agent switched roles and inspected the committed
production bodies, port/lifecycle diff, parser/Compat/device/native tests,
build registration, source-subset ledger, final fourteen JUnit reports and
artifact hashes. HEAD and origin/main both identified the executor commit.
Review confirms no second parser, executor, device registry or synthesis
backend; paused service and byte admission remain distinct. Cancellation uses
the existing exit predicate; failed host cleanup retains ownership rather than
silently releasing an outstanding handle. FM's unsupported-host limitation is
explicit. Public corpora and owner assets other than the two EXEs are unchanged.

The eight admitted S8 units have evidence; the unexplained timeout is not
claimed fixed. S8 closes under delegated sequential acceptance, with S9 as
the sole next active packet. This does not close T93 or the overall goal.
