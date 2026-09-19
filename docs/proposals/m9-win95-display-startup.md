# Windows 95 startup display investigation

The owner confirms successful installation and normal desktop operation after
S9. Two new observations require separate investigation: the startup logo
occupies only the upper half of the display, with a black lower half; the
desktop reports that display settings or adapter selection do not match the
hardware. The supplied installed disk, adjacent configuration and executable
are reproduction inputs, not files to edit.

## S10 research brief

Original request: “启动画面，怎么只有屏幕上半部，下半部全黑，帮我调研一下”;
“进入桌面说显示器配置不对”; followed by “准入S10调研”.

Investigate, do not implement a speculative fix. Compare copied frame height,
painted rows, original painter geometry and VGA scanline registers during
startup. Read installed display configuration and boot logs through read-only
disk inspection. Distinguish an adapter/driver mismatch from failed emulated
capability or mode setup. Do not assume the two observations share a cause.

| Coverage | Required evidence |
| --- | --- |
| Logo geometry | Reproduce frame output and correlate its painted height with controller/painter geometry. |
| Desktop warning | Identify installed driver/settings and failing initialization evidence; do not silently change guest settings. |
| Shared cause | State proven common cause or keep independent hypotheses. |
| Repair boundary | Name owning component, minimal candidate change and focused verification before requesting implementation. |

Production/test diff estimate: zero for research. No Lib/Common, mirror,
snapshot contract, user INI or guest image changes. Runtime observations use
overlay only. Ignored diagnostics under build/t70-s10 are bounded to 600 seconds
and 64 MiB per run; at most one 128 MiB checkpoint if required. Remove exhausted
outputs and terminate owned probes. External comparison trees remain read-only.
Exit with an evidence-backed report or a precise unresolved question and the
next discriminating observation; do not report an unimplemented repair as done.

## S10 observations (research, not a delivered repair)

- An overlay cold boot reproduces both symptoms in copied machine frames,
  before a KVM presenter consumes them. The logo frame is 640 by 400; the
  later desktop frame contains the display-settings warning. The source
  medium's SHA-256 remains unchanged after the run.
- A link-time observation of `nt_vga_hi_graph_std` records start 32768,
  width 80 plane groups, height 400, display stride 160, actual plane stride
  80, and unit character/pixel heights. The renderer receives all 400 rows;
  neither frame clipping nor Window scaling explains the half-height logo.
- Offline decoding of the same captured video memory/palette reproduces the
  half-height result with stride 160 and the complete image with stride 80,
  retaining the same start address. A diagnostic-only override of the
  derived stride after mode selection also produces a complete logo during
  a real cold boot. No tracked production source was changed for this test.
- `choose_vga_display_mode` doubles the plane stride for odd/even chained
  memory; the unchained 256-colour painter then multiplies that stride by
  four to address interleaved planes. The repair candidate is the shared
  display-address calculation, not image stretching or a logo-specific
  painter branch. Dirty-range, split and wrap users of the derived stride
  must be covered before an implementation is accepted.
- Read-only installed configuration and installer INF inspection agree on
  the Video Seven driver family. Boot logging shows frame-buffer and VGA
  drivers; registry bytes also contain display-fallback notification text.
  This does not yet prove whether the current initialization fails or a
  previously scheduled notification remains. Driver failure attribution is
  still open; changing the configured adapter is not an established fix.

Research changes so far: zero tracked production/test lines. Diagnostic
programs and captured frames remain in the packet's ignored bounded directory.
