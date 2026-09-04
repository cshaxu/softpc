# M9 T26: Monitor Help Separator

## Result

Closed. The monitor prints one blank line after its local `exit` command and
before the `While the guest is running:` heading. Command and hotkey text are
otherwise unchanged.

## Verification

- GCC x64 package rebuild and package-smoke integration test passed.
- GCC x86 package rebuild and package-smoke integration test passed.
- The package update refreshed only `softpc32.exe` and `softpc64.exe`; the
  adjacent user-owned `softpc.ini` was not changed.
