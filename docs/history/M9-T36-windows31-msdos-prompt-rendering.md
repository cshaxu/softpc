# M9 T36: Windows 3.1 MS-DOS Prompt Rendering Recovery

## Task Brief

### Original Owner Request

“对啊，你来帮我研究和修复一下看看怎么回事” — following the
observation that commands typed in the Windows 3.1 graphical MS-DOS Prompt
execute but their characters do not display.

### Objective

Identify and repair the missing visible character-update path without
introducing a second VGA implementation or treating the already-closed outer
runtime performance work as the presumed cause.

### Baseline

M9 T35 is closed.  Window and console idle scheduling are responsive, but the
Windows 3.1 graphical MS-DOS Prompt remains unable to display typed text even
though its commands execute.  The issue existed before T28–T35 dirty-frame
and scheduling changes.

### Affected Boundary

The standalone machine input-to-presentation port.  The selected original
C-VID/VGA controller and painters remain unchanged.

### Non-goals

No DPMI, DOS, Windows-driver, NTVDM, BOP-policy, ROM, media, INI, or
console/window lifecycle change.  No frontend guest-VRAM renderer.

### Findings

- A guest-executed CCPU regression selects the installed profile's V7
  `INT 10h/6Fh` mode `67h` (640 by 480, 256 colours), performs byte, `REP
  STOSB`, and operand-size-32 `REP STOSD` VGA writes, and proves the selected
  original painter mutates the DIB and publishes its dirty rectangle.
  Therefore neither generic CCPU writes, C-VID marking, V7 mode selection,
  nor the copied-frame filter is the root cause.
- The installed Windows 3.1 profile contains `V7VGA.DRV` and `V7VDD.386`.
  `V7VDD.386` is the guest's Video Seven virtual-display component for a
  windowed DOS Prompt; it can alter its presentable virtual screen while
  handling guest input without an ordinary mapped VGA store.
- The existing standalone mouse input path already requests the original
  `nt_mark_screen_refresh()` after an InPort event for that same virtual
  display route.  Keyboard make events had no corresponding request.
- The new regression proves that a keyboard make event did not create an
  original DIB dirty rectangle before the repair.  It passes after the host
  bridge requests the original refresh on a successfully delivered make
  event.  Release events do not refresh.

### Repair

`src/host/machine.c` now calls the existing standalone presentation-refresh
port after a successful keyboard make.  This invokes the imported original
renderer's `nt_mark_screen_refresh()` and does not alter CCPU, V7 registers,
VRAM, BIOS, ROM, BOP policy, or Windows/DOS semantics.

### Focused Verification

Owner acceptance remains: run Windows 3.1, open graphical MS-DOS Prompt, type
a visible command and verify both displayed text and execution.  Sweep a
normal Program Manager graphics repaint, guest text console route, and
graphical Prompt transition.

### Full Regression

GCC x64 and x86 CTest each passed 22/22; refreshed `assets/binary/softpc64.exe`
and `softpc32.exe`. The owner-owned `softpc.ini` remains untouched.

### Risks And Stop Conditions

If the fault requires a new VGA controller/Windows driver/DOS semantic or a
large unclassified mirror rewrite, stop with evidence and queue a new design
task.  If a selected original source needs a portability change, use the
port-ABI overlay discipline rather than inserting standalone policy into the
mirror.

## Requirement Ledger

| Requirement | Owner | Status | Evidence |
| --- | --- | --- | --- |
| Determine why typed Prompt characters execute but are invisible | T36 diagnostic path | Implemented | Guest V7 CCPU/DIB proof and keyboard-refresh regression |
| Repair only the implicated original-machine/original-host route | T36 implementation | Implemented, owner verification pending | Keyboard make → existing original renderer refresh port |
| Preserve independent-VM boundaries and configuration/media | T36 scope | Active | No change admitted to excluded surfaces |
| Produce x64/x86 runnable packages and regression evidence | T36 verification | Complete | GCC x64 22/22; GCC x86 22/22; both package EXEs refreshed |
