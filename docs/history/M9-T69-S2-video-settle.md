# M9 T69 S2 bounded closure

The owner accepts closure of this S and explicitly transfers the remaining
display faults to the next S. T69 remains open.

## Delivered change and evidence

Commit `f70dcda` retains the existing video mode-settle interval and restarts
it after another controller invalidation. Production accounting against
`7cf9d9f`: three MVDM files, +25/-7, net +18 lines. The focused VGA test adds
13 lines. Both package executables were rebuilt and pushed. Focused VGA,
text-surface and documentation checks passed on x86/x64.

The reported x64 broad run passed 105/108: two package stage-16 checks and
types-layout-selftest failed. Their independence from this change was not
proven by a controlled baseline comparison. No complete x86 suite result is
claimed for this delivery. Earlier statements that all failures were unrelated
were stronger than the evidence supports.

## Owner acceptance and remaining scope

Original feedback: “现在好多了，宽度加倍的闪烁频率降低了很多，过好久一会儿才会加倍再跳回正常kvm-window宽度。。。cls也确实能清屏幕。”

The owner also reports fullscreen corruption and broken characters, including
new typed characters not appearing, after returning to a windowed DOS Prompt.
The supplied screenshot shows relatively intact window decorations around
misordered or missing text. This localizes the investigation but does not prove
a keyboard-input failure, font defect, or particular VGA register defect.

| Requirement | Disposition | Receiver / evidence |
| --- | --- | --- |
| Re-arm existing mode wait after controller writes | Delivered | Focused two-write/tick regression |
| CLS clears screen | Owner observed improvement | Current manual feedback |
| Eliminate doubled-width excursions | Unresolved | S3; reduced frequency is not completion |
| Window to fullscreen display | Unresolved | S3; corruption persists |
| Fullscreen to window display and visible typing | Unresolved | S3; supplied screenshot |

The earlier 1280/640 frame trace establishes an upstream geometry alternation.
It does not establish the register values at that instant. In particular,
`80 * 8 * 2` was an inferred candidate calculation, not a captured controller
record. S3 must test that hypothesis and the retained settle change rather than
add more delay or treat a Boolean-gate test as end-to-end proof.
