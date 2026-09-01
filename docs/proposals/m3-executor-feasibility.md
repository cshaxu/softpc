# M3 Executor-Boundary Feasibility

## Objective

Identify and prove the original SoftPC control-transfer point at which a
standalone executor may process lifecycle and input requests safely.

## Scope

Trace original `host_simulate`, unsimulate, timer, event, BOP, reset, and stop
paths. Establish which transitions return to the host and which may be queued
until the next return. This proposal does not implement a runtime loop.

## Acceptance

A diagram and focused harness prove one safe boundary for command processing,
without thread suspension, frontend locking, synthetic ticks, or
instruction-budget pacing.

## Stop Condition

If no original return boundary exists, stop and present the smallest possible
port-ABI adapter for owner approval before changing CCPU.
