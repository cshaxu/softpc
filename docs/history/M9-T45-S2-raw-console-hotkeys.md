# M9 T45 S2 — Raw Console registered hotkeys

## Outcome

Raw Console registered hotkeys now take their Ctrl/Alt/Shift state from the
copied native input record.  `ux-console` maps the neutral logical-Console
mask to the generic UX chord mask and passes it to the common Win32 keyboard
normalizer.  The normalizer no longer consults process-global `GetKeyState()`
for raw Console chord matching.  Window retains its own native message-state
sampling before it calls the same normalizer.

This fixes the reported RDP raw-Console CAP, CAD, and CAF failure without
teaching lib any SoftPC hotkey meaning or changing guest keyboard injection.

## Evidence

- The focused Win32 keyboard probe supplies raw-record masks for CAP, CAD,
  and CAF; it proves each becomes one registered identifier and that every
  make/break in the chord is suppressed.
- The same probe proves a Window-supplied mask is retained by the common
  normalizer.
- Fresh x64 and x86 full CTest each passed 32/32.
- The owner accepted RDP raw-Console behavior.

## Local-desktop scope

Local and RDP raw Console use the same `ReadConsoleInputA` record path, so
CAP and CAF receive the same repair.  Native Windows may intercept local
Ctrl+Alt+Del as the secure-attention sequence before an application receives
it; that is an OS boundary, not a lib hotkey failure.

## Closure

T45 is closed.  No MVDM source, SoftPC hotkey policy, user-owned package
configuration, or guest media changed.
