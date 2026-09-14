# M9 T56 closure: canonical common corpus

## Outcome

T56 is closed. SoftPC is the canonical common corpus for downstream adoption:
`common/ui`, `common/session`, `common/machine`, `common/xasm32`, and
`common/debug` each have one production owner and one bounded responsibility.
The accepted product behavior, MVDM source, command vocabulary, and lib
contracts were not changed by the final convergence work.

## Ownership closure

- `common/ui` alone owns the monitor logical Console, broker, raw Console and
  Window/KVM composition.
- `common/session` alone owns generic control reduction and command dispatch;
  SoftPC CLI remains the injected product provider.
- `common/machine` alone owns the executor, lifecycle/input queues, run
  generation and completed-frame publication; app retains only the injected
  SoftPC driver and guest-input conversion.
- `common/xasm32` and `common/debug` match the frozen NXVM source corpus
  byte-for-byte. Debug remains dormant behind the optional paused-state
  machine adapter; no SoftPC debug command was introduced.
- App/host may use public lib contracts for their own resources, but the S8
  source gate rejects any app/host include of a common implementation or
  non-root common contract. It also retains the retired app generic
  runtime/session source checks.

`common-session` no longer has app/host CMake include directories: it uses
only its declared common/lib contracts. `src/common/README.md` and the current
architecture/source-layout authorities now describe all five components,
including dormant xasm32/debug dependencies.

## Verification

- Fresh fixed-width build and full CTest: x64 **62/62**, x86 **62/62**.
- Strict standalone lib build and CTest: **8/8**.
- Source boundary, documentation governance, KVM naming, manifest and
  component-DAG gates all passed.
- Exact source comparison against NXVM commit
  `e894ef8949a0d92719678f9ebd6cec2793256ac0` passed for all five debug files
  and all seven xasm32 files.
- Fixed packages were rebuilt and tested at both widths. SHA-256:
  - `softpc32.exe`: `A5A783C254A9A8E3C56A70CFA318E883CF0A66CD08D81A7F8E5182790692B8DA`
  - `softpc64.exe`: `03AC62E19F5A8A90BCDCDE48722367A15119D2EB9121CD398493141C5F95D6D8`

The earlier S1 one-width package omission is remedied by this S8 dual-width
rebuild. Dual package output and testing is now an explicit active-packet
verification requirement for every future S; one-width output is not a
deliverable.
