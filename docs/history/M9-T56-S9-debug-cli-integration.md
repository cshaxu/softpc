# M9 T56 S9: monitor spacing and state-independent debugger CLI

## Bounded closure

Owner admitted continuation on 2026-09-13: “非常好，准入实施，完成debug和xasm32
彻底接通测试。” S9's CLI integration is the accepted implementation baseline
for S10–S12, not a claim that the remaining debugger machine capabilities work.
T56 remains open. This records the actual P5 delivery `d193638`, independently
reviewed against its diff and S9 packet; P6 `f4a8648` records the approved plan.

## Requirements and results

- Nonempty monitor results retain their blank line; empty input is unchanged.
- Debug entry/stay/exit is independent of INIT/STOPPED/RUNNING/PAUSED.
  Opening does not read CPU state; machine commands require paused state.
- The existing control-thread provider selects the CLI; only the existing
  executor calls the product debug adapter. CAP and q retain their approved
  semantics. No second reader/executor or MVDM changes were introduced.
- Real register/memory, assembly and disassembly tests exercise the adapter.
  Disassembly now separates text length from instruction length; U's
  zero-progress/wrap defect was found by the real test and repaired.
- Remaining special-register, paging, snapshot, port, execution-plan and
  watch capabilities are explicitly assigned to S10/S11/S12 in the
  [proposal](../proposals/m9-common-corpus-convergence.md), not silently closed.

## Evidence and accounting

P5 x86/x64 each passed 63/63 CTest including shipping-package CLI interaction;
strict standalone lib passed 8/8. Documentation/source/manifest gates passed.
No user INI/media or lib/MVDM source changed. Both packages were pushed.

Relative to `2ab3f1f`, production/CMake 16 paths +590/-222 (net +368);
tests 5 paths +219/-5 (net +214), using `git diff --numstat`, excluding
documents/binaries. Provider code moved out of main with the old copy removed.

P5 SHA-256:
- x86: `05A4DAD7E88D1369B909522C6AA9D3E886E123D438B895D6819225CDB93BE65C`
- x64: `8B1979A256E85E24034655D6B82A932E0A62603037E9A8BD58C8DA1797472272`
