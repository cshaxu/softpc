# M9 T98 S5 - Worker destruction

Owner accepted S5 and requested the next subtask on 2026-10-21.
Delivery: `8132d0db4e5d088c5176e2de8307498a8b5514ea`.

KVM destruction now requests terminal stop, wakes the native worker, joins
actual exit and only then disposes storage. The 5000ms destroy deadline and
its parameter plumbing are removed. Startup rollback uses the same ownership
rule. No product shutdown policy or unrelated timeout changed.

Recorded delivery evidence: both cached architecture builds and published
EXEs completed. Four focused lifetime cases per architecture passed, including
a synchronized hold beyond five seconds followed by release. Each background
suite qualified 185 cases: 184 passed normally and the Junction-permission
cleanup selftest passed separately elevated. Three Lib desktop cases passed
per architecture. Both App package desktop cases failed at machine composition
before monitor startup. That observation does not independently prove the
earlier attribution to MIDI initialization. Native audio was not rerun.
Owner acceptance does not turn these integration exclusions into passes.

Tracked text accounting: +253/-111, net +142, excluding EXE binaries.
Production C/H: +69/-39, net +30; test C: +135/-37, net +98;
other text: +49/-35, net +14. User snapshot.debug remains untracked and untouched.
S5 is owner-closed; T98 remains open. The next task does not claim a new
runtime qualification of this accepted delivery.
