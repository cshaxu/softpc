# M9 T61 S2 — No-behavior mirror cleanup

## Request and executor delivery

Owner requested fewer unnecessary OpenNT differences, not fewer current lines.
P1 `6df4104` restores original formatting/comments in eight mirror files and
removes the inactive DIB branch, without new behavior, ABI or state owners.
The [ledger](../etc/evidence/softpc/pristine-divergence-current.md) records
all candidates, retained reasons, exact preprocessing and assembly hashes.

Tracked production delta is +17/-34, net -17; test source delta is zero.
keyba.c is byte-identical to OpenNT, including its original first-line trailing
space (the single deliberate diff-check exception). Auxiliary content totals
fall by 43 changed lines, 11 hunks and one divergent file. Working-copy EOL
normalization is explicitly distinguished from tracked functional changes.

Both builds and full suites pass 97/97 each. Tests cover input, PIC, VGA,
FPU, BOP and fixed packages. All selected preprocessing comparisons match
except the removed single-call braces; both widths produce identical assembly
for that exception. Necessary PIC/FPU/input/display repairs remain intact.
Documentation governance passes. Temporary probe scripts were removed.

## Coordinator actual-commit review

Reviewed actual pushed `6df4104`, its eight source diffs, package hashes and
candidate dispositions against the packet. No shared corpus, Compat, VM, App,
INI or media changes. Non-CCPU/NTVDM restoration stays S3; keyboard and sound
remain S4/S5. Removed dead DIB directives preserve the selected original branch.
Restored comments do not roll back the fixed FPU representation.

Post-commit IRQ/keycode/VGA/x87 focused tests pass 4/4 per width. HEAD and
origin/main agree and the reviewed worktree is clean. Fixed EXE hashes remain
those recorded in the ledger. S2 closes; T61 remains open and S3 is admitted.
