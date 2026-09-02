# M5 Win32 Window Lifecycle

## Defect

The window frontend returned a stopped result after `WM_CLOSE`, but did not
stop the runtime.  The monitor could therefore report a stopped machine while
the sole executor remained active.

## Resolution

The window endpoint now closes the runtime with the same outer lifecycle
operation used by the console endpoint.  No window thread accesses SoftPC
state; it only ends its presentation loop and requests runtime stop.

## Verification

- `softpc-win32-window-smoke` starts a runtime, creates the actual
  `SoftPC VM` Win32 window, posts `WM_CLOSE`, waits for its loop to exit, and
  requires `SOFTPC_RUNTIME_STOPPED`.
- The smoke passed on x64 MinGW and x86 Clang.
- Full serial CTest passed `18/18` on each width.
- The delivered `build/output/softpc64.exe` and `softpc32.exe` were each
  started through their fixed INI in overlay mode, created `SoftPC VM`, and
  exited after a real `WM_CLOSE` message.
