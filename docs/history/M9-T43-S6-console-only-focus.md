# M9 T43 S6 — Console-only focus after Window retirement

## Outcome

Current Console activation is again an I/O operation only. `lib/host` no
longer requests native foreground/focus while creating, replacing, or
restoring a raw/cooked binding. Its explicit focus operation remains host
owned; SoftPC reaches it through the monitor-facing host API without a Win32
handle.

`ux-window` still foregrounds itself at successful native Window creation.
SoftPC's reconciler now recognizes the completed actual state in which Window
is absent and the derived surface is Console-only. Only the Window-destroy
completion invokes the explicit host focus request. A living Window, a failed
or late completion, and every ordinary Console replacement issue none.

## Focused proof

The broker fake proves initial activation, raw/cooked replacement, failed
prepare, and successful old-binding restoration make zero focus requests;
one explicit request is observable only after an active Current Console
exists. The reconciler smoke proves the product predicate remains false while
Window is actual and becomes true only after the Window destroy completion for
both monitor-current and VM-Console-current Console-only routes.

## Regression

Fresh `softpc32.exe` and `softpc64.exe` packages were built. The focused
broker/reconciler tests and full serial CTest passed 32/32 on both x86 and
x64. The shared-library manifest was regenerated and independently verified.
