# Current

## Current Work

### M3 T3 S2 P1: Establish a safe single-executor rendezvous

- **Owner and mode:** repository owner; single-person dual-role execution.
- **Admission and approval:** the owner approved the NXVM-style standalone
  architecture and instructed continued execution: one executor owns SoftPC,
  frontends communicate by mailbox, and original core/device/BIOS/BOP behavior
  remains intact. S1 proved that an instruction budget is not a return
  boundary.
- **Input boundary:** the transitional console and window both call
  `softpc_machine_run()`, apply input directly, and lock/read machine state.
  This creates frontend pacing, cross-thread device mutation, and RDP latency.
- **Objective and output boundary:** identify a generated CCPU rendezvous
  that can drain copied records on the executor thread without repurposing
  `c_cpu_unsimulate()`, BOP FE, instruction accounting, or timer delivery.
  A runtime implementation is not admitted until that rendezvous has a
  bounded real-media proof.
- **Finding so far:** a trial that combined mailbox draining with CCPU timer
  delivery was rejected. It changed original POST timing and left the
  dual-media reset probe CPU-bound. The trial was removed rather than retained
  as an unverified runtime layer.
- **S2 P1 result:** original `nt_timer.c` proves that `host_timer_event()` is
  a CCPU-thread event-consumption point, not a general CCPU return boundary.
  Its required producer is the original heartbeat sequence
  `ICA lock -> time_tick/RTC -> CPU_TIMER_TICK`; the detached platform has
  only an unconsumed timer-queue counter and a no-lock `ica.c` branch. M3 may
  not recreate that producer because the resulting host-thread controller
  mutation is this packet's stop condition. The M2 timer/ICA compatibility
  package is therefore a hard predecessor; see the updated S1 evidence.
- **Non-goals:** no controller/device rewrite, ROM/BOP change, guest-media
  mutation, selectable profile/session, frontend styling, or Windows Setup
  completion claim in this S.
- **Verification:** source-generation audit; a bounded dual-media POST probe;
  x64/x86 existing BOP, timer and VGA checks. Only then admit a runtime
  command/input/snapshot fixture.
- **Asset needs:** synthetic fixture only; no guest media is required.
- **Similar-issue sweep:** console, window, real-boot probe, HLT wake, timer
  callback, nested `host_simulate`, and both width rows.
- **Stop condition:** stop if the hook needs to mutate controller state from a
  host thread, reinterpret BOP FE, inject a device tick, or retain raw machine
  surface pointers outside the executor callback.
- **Exit criteria:** a supported rendezvous contract with no timing or BOP
  semantic change, then one executor and copied mailbox proof on both widths.

## Current Technical Baseline

The current runnable baseline is the direct-launch SoftPC VM on `main`. It is
transitional: standalone-specific behavior still appears in original CCPU and
original host-renderer paths. The target architecture is defined in
[System Architecture](../design/ARCHITECTURE.md).  Local real-boot traces are
uncommitted evidence and are not task inputs unless a later packet names them.
