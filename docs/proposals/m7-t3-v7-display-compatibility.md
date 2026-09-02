# M7 T3: Original V7 Display Compatibility Proof

## Goal

Extend evidence for the already compiled original `nt_graph`, `nt_cga`,
`nt_ega`, and `nt_vga` path across the selected V7 VGA BIOS modes used by real
software.

## Dependency

M7 T2 must close first.

## Scope

- Test original text, palette, planar and V7 high-resolution layouts through
  copied frame presentation.
- Fix only standalone DIB/window endpoint defects discovered by those tests.

## Exclusions

Do not replace original painter logic with a frontend VRAM decoder, and do not
restore MVDM console/full-screen services.

## Exit Evidence

Dual-width CTest plus real guest-mode evidence shows original renderer output
in the independent window without controller changes.
