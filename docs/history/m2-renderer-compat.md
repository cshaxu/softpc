# M2 Original Renderer Compatibility Restoration

## Objective

Compile the original `nt_cga`, `nt_ega`, `nt_vga`, and `nt_graph` paths against
a standalone conapi/DIB compatibility boundary, with no standalone branches in
those source files.

## Acceptance

Original text, palette, dirty-region, planar, and V7 mode paths publish to an
opaque host surface. A frontend does not decode EGA/VGA planes or access guest
memory.

## Stop Condition

Stop if an original renderer requires an unavailable product display service
whose behavior cannot be expressed as a DIB/palette/invalidate capability.
