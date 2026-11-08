# T99 S38 C-VID rules closure

Owner accepted and approved closure on 2026-11-08. P1 a43c6d24 and P2
19a68782 are pushed. T99 stays open; the owner next requests a read-only
necessity/special-case/reduction audit of every retained mirror difference.

S38 repairs five native GDP subfield mappings, the Unchained DWORD second-WORD
offset, Copy DWORD odd-start pattern/tail handling, and empty WORD/DWORD fills.
No bulk CPU consumer is activated. Original device ownership and dirty/cleanup
paths remain. Combined production +24/-8 (net +16), tests +99/-2 (net +97).
See [evidence](../etc/evidence/softpc/m9-t99-s38-gdp-subfields.md) for exact
per-P accounting, independent before/after proof and future integration limits.

Both final EXEs are published; x64/x86 each pass 187 background and eight native
tests (390 total). No new non-mirror warnings. Coordinator review inspected the
actual committed changes and confirmed original loops, unsigned pattern shifts,
bounded tails and cleanup ownership. Public eight trees, INI, media and
snapshots are unchanged. All admitted TODO findings are repaired.

Scratch was removed and only reusable caches remain. This owner closure changes
documentation only; no redundant build/test run. Documentation, whitespace and
build-residue checks pass. S39 admission follows separately; T99 is not closed.
