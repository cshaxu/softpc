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
an estimate, not an acceptance claim.

## Approved implementation

The owner approved the two proven repairs. Retain the original device owners
and interfaces; do not change Lib/Common, guest settings, media or snapshot
format. Estimated production change remains 5--15 lines in two mirror files;
focused tests extend the existing VGA smoke rather than add a framework.

The finite verification universe is: (1) planar 256-colour stride versus
odd/even and chain-four addressing, including downstream row/wrap units;
(2) extended BIOS mode numbers with/without preserve bit, clear/preserve
semantics and invalid requests; (3) real installed-driver initialization and
complete boot-logo output; (4) both-width full regression and package builds.
Each requires passed evidence or an explicit unresolved result, never a
guest-specific workaround. Completion requires all four and a mirror-diff
review. S10 remains open pending implementation and owner validation.

## Implementation evidence

Production changes are +6/-5 in the two named mirror files (net +1), measured
with `git diff 6541e43 --numstat -- src`. No Lib/Common/VM/Compat production
changes, interfaces, state or guest-detection branches were added. The existing
VGA smoke gains 76 lines; it covers sixteen stride/wrap combinations, all
sixteen legal V7 extended modes with/without bit 7, graphics clear/preserve,
invalid-mode rejection and independent packed-memory banks. The pre-fix
preserved-mode transition failed; the repaired smoke passes on both widths.
Existing dirty/split/panning/painter-boundary cases remain in that same test.

A bounded checkpoint boot now records successful E7 activation and the BIOS
query returning mode 67. The installed driver reaches the normal desktop and
welcome window without the prior display warning. A separate overlay cold
boot produces the complete 640-by-400 logo and the same normal desktop.
Neither run uses the diagnostic stride override; both link the repaired
production objects. The source medium's SHA-256 is unchanged.

Similar-issue sweep: `is_v7vga_mode` callers in `ega_vide.c` and `mouse_io.c`
consume decoded current/next mode values rather than a set-mode flag-bearing
BL, so their predicate remains unchanged. The native-host macro override
does not validate a BIOS request. In `vga_mode.c`, stride and wrap calculations
are corrected together; screen-pointer selection is retained because it
selects the original memory-plane receiver rather than a scanline increment.
The planar VGA painters consume the corrected stride directly; no compensating
scale is added to painters or presentation.

Both Release builds pass 109/109 full CTest cases, including snapshot and
package integration, and documentation governance passes. Package x86 size
is unchanged; x64 shrinks by 512 bytes. Executable SHA-256 values:

- x86: `1A3BE8FC4EB4BAE846B647133072C7FEEED1D1CE4590D9EE8DED7A77823446AD`
- x64: `1E3BEA1CAD9FDB5FE9C02619FCD44547440E88C33F97DF8653E81826841E5EE7`

Coordinator review confirms only the two original device calculations changed;
no diagnostic hooks, guest configuration or media are included in the delivery.
The implementation is ready for owner testing, not automatic T70 closure.
