# T99 S37 original-device audit closure

Owner accepted and approved closure on 2026-11-08. Implementation commits
7a77c818 (P1) and fd87c1bd (P2) are pushed to main. T99 remains open;
no next S is admitted. The next unused S identifier is S38.

The five-item audit repairs six BIOS PG_U conditions, the slave BIOS EOI
command port and VGA V86 text height through existing virtual CRTC I/O.
PIC reinitialization and keyboard ordering retain their proven existing paths
with regression coverage. Owner additionally admitted four WORD/DWORD ROM
fallback return casts, now preserving IU16/IU32 results.

Final diff against 2b7a097a: production +25/-11, net +14 in three mirror
files; tests +361/-1, net +360 in four App fixtures; root CMake +9/-2.
No new device owner, public API, guest-specific workaround or second path.
Public eight trees, user configuration, guest media and snapshots are unchanged.

Both EXEs were published. Each architecture passes 187 background and eight
native cases, 390 distinct architecture/test cases in total. Disconnected
native failures and successful connected reruns remain explicitly recorded.
The original ROM width fixture passes 12 cases plus the existing 120 permission
cases per architecture. See [S37 evidence](../etc/evidence/softpc/m9-t99-s37-original-device-audit.md)
for the finite applicability ledger, counterfactuals and coverage limits.

Coordinator review inspected actual committed production/test changes and
verified the minimal width/permission/port edits and the necessary virtual
register source, without extrapolating tests to all Windows scenarios.
The resolved fallback TODO was removed; three independent C-VID findings remain
in TODO. No remaining implementation S is admitted, and the separate runtime
performance candidate remains unnumbered in Queue. T99 completion audit and
owner closure have not been performed.

Task scratch was removed; only reusable Ninja/ccache roots remain. This closure
changes documentation only, so unchanged EXEs and runtime tests are not rebuilt
or rerun. Documentation, whitespace and build-residue checks pass.
