# Long-Term Review Ledger

- TODO(Low): Diagnose repeated Ninja log recovery in the local x86 build tree.
  T93 S15 incremental invocations report "premature end of file; recovering"
  and rebuild hundreds of objects, including after test-only changes. Builds
  are not concurrent within this task. The damaged file and external cause
  have not been established; no compiler-cache hit-rate claim is made.
  Owner: repository build tooling. Admission condition: preserve the affected
  build metadata and compare command/dependency logs before any clean rebuild;
  do not mix a build-system redesign into device snapshot work.

- TODO(Medium): Diagnose intermittent package compact-Console raw input loss.
  T93 S12's first desktop x64 compact case reached DOS but timed out at
  stage=14/error=0 waiting for Version after injected ver; the captured screen
  contained only C:\>. Normal package passed immediately before it. A complete
  unchanged ten-case desktop rerun passed both widths. This differs from the
  stage=1 child-startup debt below and does not prove the cause or a repair.
  Owner: App-SoftPC package-test/input handoff. Admission condition: bounded
  native input/activation instrumentation around first raw command and hide/
  focus transition; do not add retry or relax assertions. See
  [S12 verification](../history/M9-T93-xp-sound-blaster-standalone-proposal.md#s12-implementation-and-review-record).

- TODO(Medium): Diagnose intermittent checkpoint_smoke timeout under aggregate
  parallel execution. T93 S8 third x64 qualification classified it as Timeout
  with no output; its configured budget is 15 seconds. Ten isolated repeats,
  an unchanged 65-test parallel rerun, and the fourth complete qualification
  pass without changing code or budget. This does not establish the cause.
  Owner: App-SoftPC test lifecycle. Admission condition: bounded phase/native
  wait instrumentation of the actual checkpoint test under parallel load;
  preserve the timeout evidence and do not add product retries or simply
  enlarge the deadline. See the [S8 evidence](../history/M9-T93-xp-sound-blaster-standalone-proposal.md#s8-repeated-stop-proof-and-third-qualification).

- TODO(Medium): Diagnose native package-test child startup under sequential
  CTest execution. T93 S5 x64/x86 compact-console cases intermittently fail
  before start at stage=1/error=6. A scratch diagnostic reproduces child exit
  0xC0000142 (DLL initialization failed), AttachConsole error 5, then code-page
  query error 6. Direct native/package probes pass; this does not identify the
  responsible DLL or prove a CTest defect. Admission condition: test-owned
  child-startup/Console lifetime investigation with loader evidence, without
  retrying product startup or changing guest/device behavior to hide it.
  Evidence: [T93 S5 checkpoint](../history/M9-T93-xp-sound-blaster-standalone-proposal.md#s5--original-dma-prerequisite-repair).

- TODO(Medium): Replace the host-time-dependent oracle in
  `test/app-softpc/unit/machine/machine_smoke.c:run_int1a_tick_boot_image`.
  T92 S2's first x64 full run failed `bda_tick_low != 0`; the next full run
  passed unchanged. The guest repeatedly calls INT 1Ah, whose selected
  non-NTVDM implementation refreshes the BDA from host time; a zero low byte
  is legal and does not prove that PIT delivery failed. Admission condition:
  bounded test-only time/IRQ contract audit with a deterministic progress
  oracle, not a timer production change or unconditional retry. The exact
  failing host timestamp was not captured; no runtime regression is inferred.

- TODO(High): Investigate the intermittent x86 runtime-smoke null BOP dispatch
  through `BIOS[0x52]`. Both unchanged T83 baseline `9fbf7369` and T84 builds
  reproduce the same call to address zero; the guest trigger remains unknown.
  Owner explicitly defers repair on 2026-09-25. Admission condition: a separate
  owner-admitted CPU/BOP investigation, capturing guest instruction bytes,
  CS:EIP and reset context before choosing a repair; do not mask it with a
  no-op handler. Evidence: [T84 investigation](../history/M9-T84-S1-kvm-text-80x50.md).
