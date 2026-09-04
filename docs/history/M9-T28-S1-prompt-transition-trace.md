# M9 T28 S1: Prompt Transition Trace

## Result

Closed.  The standalone runtime records original presentation transitions in
`build/softpc-prompt-trace.log` without changing the SoftPC machine, media, or
the required one-presenter-at-a-time console/window structure.

## Owner Evidence

The owner exercised repeated Windows 3.1 MS-DOS Prompt text/graphics returns.
The retained trace shows stable 80 by 25 text geometry, `TEXT/WINDOWED` and
`GRAPHICS/WINDOWED` routing, every console-to-window and window-to-console
transition, and the original transient 640 by 350 then 640 by 480 graphics
geometry.

## Verification

- GCC x64 CTest: 20/20 passed.
- GCC x86 CTest: 20/20 passed.
- Both package launchers were rebuilt; `softpc.ini` was untouched.
