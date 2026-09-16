# M9 T62 S1: Session fault slots

Owner request and four-step scope are retained in the
[proposal](../proposals/m9-common-lib-simplification.md). Baseline 58d3d75;
implementation dce462e is pushed. S1 is closed after actual-diff coordinator review.

The sole Session queue retains two fault classes, first-wins within each,
normal FIFO before fault delivery and KVM before control fault. One copied-event
latch and one take path replace duplicate metadata/locking/drain code. No
public interface or pressed-key behavior changed. Full event storage replaces
small bespoke metadata slots; the bounded memory tradeoff avoids another schema.

Production 1 path +35/-69 = -34; test source 1 path +51/-0; test CMake +1/-0;
governance verifier +9/-3. No other production owner changed. The governance
change recognizes owner-admitted next work while the prior T awaits acceptance,
without fabricating T61 closure; a self-test covers this case.

Both preset builds passed, fixed EXEs refreshed, INI/media untouched. Full
x64 99/99 (87.91s), x86 99/99 (73.45s). Post-commit queue/sync/monitor/corpus
and documentation checks: 6/6 each (1.95s/1.91s). Manifests pass. Reviewed
actual dce462e diff against original request; no normal/failure path was dropped.
Same-class search covered Session latch/take and Machine/KVM queues; the latter
retain distinct bounded/worker contracts, not candidates for a generic queue.

Proceed automatically to S2; T62 remains open for final owner testing.
