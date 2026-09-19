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
  Subsequent instruction-boundary observation proves a current initialization
  failure, rather than merely a retained notification (details below).

Research changes so far: zero tracked production/test lines. Diagnostic
programs and captured frames remain in the packet's ignored bounded directory.

### Display-driver failure chain

The installed frame-buffer driver's exported Enable path reaches its failure
return. Adapter identification succeeds; its requested mode is 640 by 480 at
eight bits per pixel. The mode lookup rejects the detected video-memory size
of 65536 bytes. This is not evidence that the allocated emulated VRAM is that
small: bank 1 reads back bank 0's probe value while the VGA registers still
describe text mode.

A BIOS-boundary observation explains that state: function 6F05 receives
BL=E7 (mode 67 plus the preserve-memory bit), returns without setting graphics
mode, and the following 6F04 query reports text mode 03. In
`v7vga_extended_set_mode`, the extended-mode validation uses the unmasked BL;
only after that guard does it extract the low seven mode bits. The original
OpenNT implementation has the same ordering. The caller's preserve flag is
mistaken for part of the mode number, so banking never enters the requested
graphics configuration.

Candidate repair: extract `video_mode` before validation and validate that
value, while retaining the original BL for the existing clear/preserve flag
handling. Do not change driver code, lie about VRAM capacity, or force VGA.
Focused proof must exercise valid extended modes with and without bit 7,
invalid modes, preservation versus clearing, and actual bank separation.
Then verify the installed driver finishes initialization during a real boot;
the source diagnosis alone does not establish successful repaired operation.

Estimated production surface for the two candidates is `vga_mode.c` (display
stride condition) and `v7_video.c` (mode validation ordering), roughly 5--15
changed lines before reason markers, plus focused regression tests. This is
an estimate, not an implementation admission or acceptance. S10 remains open
pending the repair-boundary decision; no product fix has been delivered.
