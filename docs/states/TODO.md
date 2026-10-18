# Long-Term Review Ledger

- TODO(Medium): Propagate consumed-medium close failures through the real
  App-SoftPC FDD/HDD attach and snapshot-restore adapters. An isolated T98 S1
  production-code probe confirms HDD detach returns OK after injected IO_ERROR;
  the archive unit substitutes adapters and misses this path. Owner: Compat
  media adapters. Admission condition: complete existing metadata cleanup and
  return the error once, without retrying the consumed lease; cover both real
  adapters. See [F1](../proposals/m9-nonpublic-quality-performance-audit.md#f1---p2-media-retirement-can-falsely-report-success).

- TODO(Medium): Validate HDD attachment/restoration size against the existing
  signed host-offset ABI and prevent 32-bit extent multiplication overflow.
  T98 S1's mocked 4-GiB medium attaches but cannot transfer its first sector.
  Owner: Compat HDD. Admission condition: prove the addressable bound, reject
  unsupported sizes and use checked transfer arithmetic; no geometry spoof or
  second backend. See [F2](../proposals/m9-nonpublic-quality-performance-audit.md#f2---p2-accepted-hdd-size-exceeds-its-address-representation).

- TODO(Medium): Reject finite machine instruction budgets beyond the existing
  32-bit counter. 2^32 currently narrows to zero and bypasses budget completion;
  normal continuous UINT64_MAX execution is separately handled. Owner:
  App-SoftPC Machine. Admission condition: boundary check and targeted test,
  preserving the sentinel without widening the hot-loop counter. See
  [F3](../proposals/m9-nonpublic-quality-performance-audit.md#f3---p2-a-finite-instruction-budget-can-become-unbounded).

- TODO(Medium): Align optional COM/LPT output paths with config/media UTF-8
  semantics; current narrow fopen uses the Windows CRT code page. Owner:
  App-SoftPC host file exits. Admission condition: preserve lazy open and
  flush visibility; Storage writer currently lacks public flush, so decide
  that narrow capability before migrating, without importing private file.h
  or duplicating path conversion. See
  [F4](../proposals/m9-nonpublic-quality-performance-audit.md#f4---p2-comlpt-file-paths-have-inconsistent-encoding).

- TODO(High): Resolve unsupported Sound Blaster MIDI command 38h parser
  collision. T95 S1 observes sndblst2.drv sending MIDI bytes after 38h;
  a following 91h is interpreted as DSP high-speed PCM, leaving the guest
  polling busy status. The supported MPU-401 route plays Canyon after the
  Mapper configuration is committed correctly. Owner: App-SoftPC sound-device
  command boundary. Admission condition: separately establish the original
  SB-MIDI command/data contract and choose capability or explicit unsupported
  semantics; do not add a second audio path or silently broaden the admitted
  paused-DMA repair. See the
  [S1 evidence](../history/M9-T95-win31-midi-wav-playback-proposal.md#paused-transfer-and-midi-follow-up).

- TODO(High): Correct Compat memory.c's physical read/write adapters, which
  currently call linear c_sas_loads/stores. T94 S2's nonidentity-paging test
  exposed the incorrect read oracle; direct c_sas_PRS confirms the DMA write
  reached physical RAM while the named physical API inspected another page.
  Sound DMA now uses original physical SAS directly. Owner: App-SoftPC Compat
  memory boundary. Admission condition: separately scope the two adapters and
  all callers, preserve SAS_VIDEO dispatch and test paging plus video mapping;
  do not enlarge the approved four-site DSP repair silently.

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

- TODO(Medium): Diagnose intermittent Win3.1 prompt-roundtrip integration
  readiness. T96 S7 final1 x86 remained in a graphics frame (sequence 36)
  while runtime_restart_boot_smoke expected the first DOS prompt transition;
  the unchanged independent rerun passes. The test starts menu input after a
  fixed 15-second graphics observation, not a confirmed Program Manager ready
  state. That is an audit lead, not a proven cause. Owner: App integration
  fixture. Admission condition: capture guest/menu state at each injected
  transition and compare baseline/current before changing readiness or budgets;
  do not weaken mode assertions or add automatic test retries as a fix.
