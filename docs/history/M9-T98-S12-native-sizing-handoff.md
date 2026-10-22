# M9 T98 S12 Closure

Owner accepted the snap/drag-off exit repair on 2026-10-22 and admitted S13
for the revised nine-case sizing policy. T98 remains open.

Executor fc2fa687 and actual-diff review c1d6e645 are pushed. Unknown native
WM_SIZING directions now defer to DefWindowProc rather than fail the component.
The observed direction 9 formerly produced terminal IO_ERROR. The production
change was +18/-21 (net -3), tests +41/-2 (net +39).

Both architectures passed all 192 registered tests, with exact XML/name
coverage and no skips: 184 background, five desktop, three native audio.
The proposal retains first-run fixture and disconnected-session failures;
later passing evidence is not a claim those runs passed. Both EXEs were
published and the task scratch retired. INI/media/snapshot were preserved.

The width-led manual resizing policy was implemented as then requested;
owner now requests vertical-led top/bottom resizing and direction-aware snap
restoration in S13. That follow-up does not retract acceptance of the exit fix.
See the [proposal](M9-T98-nonpublic-quality-performance-proposal.md#s12---width-driven-snap-and-restoration)
for the original scope and qualification.
