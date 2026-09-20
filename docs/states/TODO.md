# Long-Term Review Ledger

- [ ] TODO(Medium) Native Window modal-test early exit: S8 observed occasional
  failure before STOP at the assertion that the native move loop remains open.
  Isolated repetitions and final suites passed unchanged; desktop interference
  is unproved. Owner: Lib native test/Window dispatch boundary. Re-admit on a
  reserved desktop with message/input evidence identifying who ends the loop.
  S10 statically confirmed one FIFO STOP -> WM_CANCELMODE path, but does not
  claim this observation repaired or bypass the assertion. See the
  [T71 investigation](../proposals/m9-kvm-mode-transition-regression.md).

- [ ] TODO(High) Win3.1 fullscreen MS-DOS Prompt native Window height
  oscillation: owner observed the actual outer Window repeatedly shrinking
  and recovering with display=window after launching the fullscreen PIF from
  Program Manager, possibly after previously testing a windowed Prompt.
  Owner and bounded probes can no longer reproduce it; no repair is claimed.
  Owner: VM/Compat display adaptation, then KVM sizing only if frame evidence
  warrants it. Re-admit on a reproducible sequence or correlated published
  text rows/font height and native client-size trace. Preserve the accepted
  packed-width repair; no debounce or application/mode special case. See
  [T69 S3 closure](../history/M9-T69-S3-width-investigation.md).

- [ ] TODO(Medium) Intermittent x86 BIOS tick smoke: delivery recheck after
  T62 S8 failed once at test/unit/machine_smoke.c:1048 (bda_tick_low != 0),
  then passed five focused repetitions and full 101/101 without changes.
  Owner: product machine test/VM timing boundary. Admit a bounded investigation
  of the fixed run-budget assertion versus timer delivery before changing
  machine semantics; root cause is not established. See the
  [S8 recheck](../history/M9-T62-S8-session-queue-ownership.md#delivery-recheck).
