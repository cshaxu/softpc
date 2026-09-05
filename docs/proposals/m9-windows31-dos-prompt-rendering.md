# M9: Windows 3.1 MS-DOS Prompt Rendering Recovery

## Purpose

Repair the missing visible character updates in the graphical, windowed
Windows 3.1 MS-DOS Prompt.  The guest receives keyboard input and executes
the typed commands, but their VGA output is not visibly rendered.  This
predates M9's outer-runtime performance work, so the investigation begins at
the recovered C-VID/VGA update boundary rather than treating frame filtering
as the root cause.

## Scope

Trace one guest character from the C-VID write/update path through the
selected original EGA/VGA painter and the standalone DIB outlet.  Restore a
missing original-machine or original-host compatibility condition only when
the trace identifies it.  The application frame mailbox may gain observation
needed to distinguish a changed DIB from a rejected publication, but it is
not an assumed repair surface.

## Boundaries

The recovered original machine remains the sole owner of CCPU, guest RAM,
VGA/EGA planes, register/latch semantics, controllers, BIOS, ROM and BOP.
`host/` may supply or correct the original renderer's standalone DIB/
invalidation contract.  `app/` remains a copied-frame consumer and must not
decode guest VRAM or manufacture display updates.

## Investigation Order

1. Reproduce the graphical MS-DOS Prompt with the installed Windows 3.1
   image and establish whether the selected update callback runs after a
   typed character.
2. Record the selected display mode, plane bytes, C-VID dirty/update input,
   original painter call and its DIB before/after region.
3. Compare the implicated original paths against the read-only OpenNT source
   and classify any divergence as a missing selected source, a standalone
   host-ABI defect, or a prior mirror modification.
4. Apply the smallest owner-correct repair, sweep equivalent small-region
   planar/VGA updates, and remove or bound temporary diagnostics.

## Non-goals

- Reimplementing VGA, a Windows 3.1 video driver, DOS, DPMI, NTVDM,
  fullscreen product services, or a frontend VRAM decoder.
- Changing the one-presenter-at-a-time console/window lifecycle.
- Editing `assets/binary/softpc.ini`, guest media, or ROM firmware.

## Exit Evidence

- A graphical Windows 3.1 MS-DOS Prompt displays typed characters while the
  commands continue to execute.
- Evidence identifies the repaired stage and proves the original painter/DIB
  receives the expected changed region.
- Equivalent small dirty updates in the selected planar/VGA route are covered
  by focused regression or diagnostic proof.
- GCC x64 and x86 full CTest and refreshed executable packages pass.
