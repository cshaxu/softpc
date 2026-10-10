# M9 Td S20: S20 closure and display-repair queue

## Closure record

The owner accepts M9 T85 S20 P4.  Commit `d3bd20f9` restores the intended
single completion result for snapshot load: Machine publishes its existing
truthful `PAUSED` fact, while Product Monitor emits a generic pause line only
for `RUNNING -> PAUSED`.  The App-owned `Machine loaded and paused.` line is
therefore not duplicated.  Focused Emulator tests, manifest/corpus/boundary
checks and both package builds passed; the two package EXEs were refreshed.

T85 remains open.  This closes S20 only; it neither closes T85 nor changes
Machine, Session, App, Core, Compat, INI, guest media or snapshot format.

## Queued display work

The owner records two new candidates at the queue head:

1. [Raw Console to cooked monitor handoff repair](../proposals/m9-raw-console-cooked-handoff.md), a shared Console transition defect to investigate against the read-only NTVDM64 evidence.
2. [Win3.1 File Manager VGA width-transition repair](../proposals/m9-win31-file-manager-vga-width-transition.md), a separate producer-side geometry investigation with an explicit prohibition on guest-program special cases.

They are intentionally separate because the first concerns native Console
reader/cursor ownership, whereas the second concerns guest-visible display
geometry publication.
