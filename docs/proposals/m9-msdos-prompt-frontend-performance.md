# M9: Windows 3.1 MS-DOS Prompt Frontend Performance

## Purpose

Improve the standalone outer runtime's responsiveness and presentation while
Windows 3.1 switches its MS-DOS Prompt between its guest text and graphical
display states. The fixed machine remains the original recovered SoftPC
machine: no CCPU, controller, BIOS, ROM, VGA renderer, BOP selector, or guest
media behavior is a change surface.

For `display=console`, the product contract is intentionally:

```text
guest text mode      -> console presenter; Win32 window absent
guest graphics mode  -> console presenter frozen; Win32 window present
```

The existing structural transition is mandatory and must remain unchanged:
each presenter runs its own loop; a transition exits that loop and creates the
other presenter. In particular, no task may replace it with a simultaneously
live console/window pair, a shared persistent HWND, or an NTVDM fullscreen
implementation.

## Evidence

- The runtime currently drains one keyboard scan event in each executor
  callback. Its host clock is 50 ms, so ordinary make/break typing is bounded
  near ten characters per second when no further safe-boundary wake is
  scheduled.
- The original `nt_graph` renderer supplies dirty rectangles, but the outer
  runtime currently treats a dirty rectangle as only a Boolean and copies,
  palette-expands, and blits the complete DIB.
- The original text storage permits more than 25 rows, while runtime and both
  presenters publish a fixed 80 by 25 text frame. Actual Prompt transition
  geometry must be measured before adapting it.
- The outer runtime has a 1,000,000 interpreted-instructions-per-second
  governor. It is a standalone executor policy, not a SoftPC device feature,
  and needs measured evidence before any tuning.

## Serial Subtasks

### S1 — Mode and geometry trace

Add temporary or test-only observation for Prompt transitions: `sc.ModeType`,
`sc.ScreenState`, text columns/rows/stride, DIB size, dirty rectangle, frame
sequence, and console/window routing. Reproduce guest Alt+Enter in Windows
3.1 and retain compact evidence. No behavior change.

### S2 — Input continuation wakes

Keep exactly one original keyboard-service invocation per executor callback.
When queued key work remains after one safe delivery, request another executor
wake so the next CCPU instruction boundary handles the next event without
waiting for a 50 ms device tick. Verify make/break ordering, host hotkeys,
RDP text input, pause/resume, and both package widths.

### S3 — Evidence-led text geometry

Use S1's recorded modes to make the copied text frame and console/window
presenters represent the required original text geometry. If S1 proves the
Prompt never exceeds 80 by 25, close this subtask with that proof and no
geometry expansion. Preserve original `nt_text` ownership and avoid direct
guest-memory reads.

### S4 — Dirty-rectangle RGB presentation

Carry the original renderer's dirty rectangle through the standalone frame
boundary. Convert and invalidate only the changed graphics region; force a
complete update on allocation, mode geometry, palette, or first-frame changes.
The original indexed DIB and `nt_ega`/`nt_vga` algorithms remain unchanged.

### S5 — Content-aware frame publication

Avoid publishing/copying a full maximum-sized runtime frame when text, cursor,
palette, font, or graphics content has not changed. Preserve the two-buffer
ownership rule and make a frame snapshot self-contained for the active mode.

### S6 — Executor pace measurement and bounded decision

Measure Prompt responsiveness, host CPU, guest timer behavior, and Windows
3.1 stability with the current 1 MIPS outer-executor governor. Tune it only
if evidence shows it is the limiting factor; any changed limit needs a dual
width timing and Windows acceptance comparison. Do not alter original timer or
quick-event semantics.

### S7 — Window idle scheduling

Replace needless 16 ms window polling only where a copied-frame or cursor
presentation event can provide equivalent wakeup. Keep cursor blink and title
state correct; do not change the console/window transition structure.

### S8 — Console idle scheduling

Replace `PeekConsoleInput` plus periodic sleep with a bounded waiting design
that still handles console input, frame publication, pause/stop, and text to
graphics routing. The console remains the sole text presenter in console
mode.

## Non-goals

- Replacing or editing original SoftPC CCPU, video, device, BIOS, ROM, BOP,
  timer, or guest media implementations.
- Adding DPMI, DOS, NTVDM, WOW, VDD, console-server, or Windows fullscreen
  product semantics.
- Changing `softpc.ini`, media bytes, the fixed machine profile, or package
  command-line policy.
- Altering the required one-presenter-at-a-time transition structure.

## Admission And Closure

Each S is a separate admitted implementation task with a newly allocated T
number. Every runnable S rebuilds x86 and x64 GCC packages, leaves the
user-owned INI untouched, and runs regression proportional to its surface.
S1 evidence is the admission gate for S3; S6 may close with a measured
no-change decision.
