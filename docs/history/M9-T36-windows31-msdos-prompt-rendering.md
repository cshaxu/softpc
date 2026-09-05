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

Selected original V7 revision-3 extension registers and the original C-VID
CPU-side VGA-write dispatch which consumes their per-plane ALU input.  No
standalone input-to-presentation refresh policy is admitted.

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
  The latter is guest-side VDD code. Its static image contains support for
  several Video Seven generations, so its mere possession of a proprietary
  register sequence does not prove that the selected revision-3 machine
  executes that sequence.
- The selected original controller reports V7 revision 3 (`SR8E/SR8F = 70h`)
  and implements the revision-3 extension set, including `F6` banking and
  `FE` foreground/background control. It does not implement `CD`, `CE`, `E8`,
  or `E9`; those registers must not be imported from a later chipset unless
  the runtime trace proves that the selected configuration reaches them.
- The existing standalone mouse input path requests the original
  `nt_mark_screen_refresh()` after an InPort event. A matching keyboard
  refresh was tested and rejected: it did not restore Prompt text, is not a
  V7 hardware operation, and would force an expensive speculative repaint on
  every keypress.
- The revision-3 V7 manual's color-text-expansion example programs `FE=06h`
  and emits font scan lines through `LODSB`/`STOSB`.  The selected controller
  had V7 dither write tables but did not prepare their temporary four-plane
  ALU input for the solid foreground/background variant.  That is a direct
  machine-side candidate for the invisible graphical Prompt character path;
  it is neither a DOS service nor a frontend repaint.

### Rejected Hypothesis

No keyboard refresh bridge is retained. The V7 CCPU/DIB regression remains as
the baseline: ordinary V7 640 by 480 mode selection and mapped string writes
work, so the remaining investigation must target the guest V7VDD hardware
protocol rather than a generic input repaint.

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
| Determine why typed Prompt characters execute but are invisible | T36 diagnostic path | Active | Guest V7 CCPU/DIB proof excludes generic mapped-write and repaint paths |
| Repair only the implicated original-machine/original-host route | T36 implementation | Pending | Must trace the selected V7VDD runtime register/bank protocol before extending the original controller |
| Preserve independent-VM boundaries and configuration/media | T36 scope | Active | No change admitted to excluded surfaces |
| Produce x64/x86 runnable packages and regression evidence | T36 verification | Complete | GCC x64 22/22; GCC x86 22/22; both package EXEs refreshed |
