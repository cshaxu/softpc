# M4 Monitor Console Mailbox Conversion

## Objective

Replace the transitional direct-control console with the fixed monitor UX over
the M3 command mailbox.

## Scope

Implement the `start`, `pause`, `reset`, `stop`, `floppy`, `status`, `help`,
and `quit` monitor commands. Read fixed configuration only from adjacent
`softpc.ini` and publish lifecycle results from the runtime.

## Acceptance

The monitor starts stopped, accepts commands without guest execution on its
thread, supports a safe configured floppy swap, and never reads or writes
SoftPC state directly.

## Stop Condition

Stop if a command requires a second session, a selectable machine profile, or
direct controller mutation from the monitor.
