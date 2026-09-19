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
