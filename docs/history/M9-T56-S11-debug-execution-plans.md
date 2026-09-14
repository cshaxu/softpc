# M9 T56 S11: original CPU debug execution plans

## Closure and authority

Owner approved “批准对mvdm的小范围必要改动”. P1 `8feab8b` implements
the bounded instruction port and was pushed before coordinator review of the
actual commit, CPU bypass inventory, tests and scope. S11 is closed; T56 stays
open and S12 owns remaining watch/observation work.

Original CCPU still executes every instruction and owns interrupts, exceptions,
TF/DR and REP. Its only changed file is c_main.c: +12/-4 (net +8), two observer
declarations, predecode stop/refetch and successful-completion notifications.
Plan/result state is owned by the injected product driver, bound only while
its original CPU runs on the sole executor. No second executor or guest patch
bytes are introduced. The existing PAUSED event carries completion to control;
the debugger queries the result through the existing paused rendezvous.

## Requirement evidence

- Real programs prove exact TRACE counts, real/linear break-before-execution,
  repeat breakpoint progress, MOV/POP SS, STI/CLI, IRET and REP boundaries.
- Faulting instructions do not retire; the handler's completed instruction
  does. Guest TF still enters its own INT 1 handler before the next instruction.
- External cancellation and reset discard the plan/result. Close/destroy/reopen
  share cancellation; stop/run exit clears adapter state. HLT retains the
  original wait and host pause/stop, rather than fabricating a completed step.
- Real command provider T2 and shipping EXE G/T2 prove asynchronous completion,
  state text, registers, prompt and automatic continuation through control.
  The package's disposable program uses CLI to isolate its fixed-IP assertion;
  the product never suppresses timer interrupts for a debug plan.
- Renamed host/debug.h to machine_debug.h because recompiling CCPU exposed
  shadowing of its original debug.h. The old product path is removed.
- One app output converter handles both synchronous and completed debug text.
  Library, configuration and media are unchanged.

The [proposal](../proposals/m9-common-corpus-convergence.md) records the finite
operation/bypass/cancellation ledger and source-difference disposition.

## Verification and accounting

x64/x86 full CTest: 63/63 each. Strict standalone lib: 8/8. Final additional
real-break/reset assertions also passed x64 focused debug/package tests; x86
full includes them. Documentation governance and diff checks passed.

Relative to `6483f63`, production 11 paths including header removal/addition:
+217/-49, net +168. Two test files: +168/-4, net +164. No manual GUI testing
is claimed beyond the automated shipping native Console test.

EXE SHA-256:
- x86: `658DFEB69B9312864C43B94F91224D88532AA2B8436BC03E07A70952F8A30A7E`
- x64: `72ED204622B6B615073C158921A9520A11022E1B7BF83764BA2C1FCFAAF87859`

Review found no remaining blocker in S11's bounded delivery. S12 watchpoints
remain explicitly unsupported, so this is not a complete debugger acceptance.
