# Lib frame-copy simplification — T64 admission brief

Owner admitted the former Queue head as T64, with S1 auditing similar Lib
simplification opportunities. CURRENT is the sole active packet authority.
Source candidate: [architecture review](../proposals/m9-snapshot-architecture-simplification.md).

## S1 decision

The [source audit](../history/M9-T64-S1-lib-simplification.md) found repeated
thread-resource operations, but Base task has cancellation and infinite join,
Window needs ready-or-thread-exit startup waiting, and KVM uses bounded join.
Owner accepted retaining separate workers rather than expanding Base API.
Thread unification is cancelled, not unfinished implementation or new debt.

## S2 approved narrowed copy

Original owner approval: “批准做这个收窄版修复。”
Keep kvm_frame layout and copy every byte before graphics_pixels unchanged.
For text, do not copy that array; for graphics, copy graphics_stride times
graphics_height bytes. One KVM value helper owns this rule. No per-field lists,
new state, union, allocation, pool, refcount or zero-copy ownership.

Only graphics mode permits reading pixels, within the active extent. Audit
readers and all production full-frame copies before changing them. Keep dirty
union, palette comparisons, frame/control locks, sequence and ack semantics.
Common's published-frame copy may use the same helper; do not refactor Machine,
UI policy, VM/Compat producers, broker, snapshots or thread lifecycle.

Before estimate: 15–30 added and 4–8 removed production lines, with focused
tests for full prefix, untouched pixel tail, padded stride, maximum extent,
text/graphics transitions and invalid-source rejection. Reuse dirty/ack tests.
S2 requires dual-width full regression, refreshed package EXEs, shared manifests,
changed-path review and commit/push. T64 stays open for owner testing.
