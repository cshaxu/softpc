# M9 T85 S19 — Emulator rename closure

S19 replaces the shared Common source and test corpus with the one Emulator
corpus. `src/common` and `test/common` no longer exist; direct consumers,
public symbols, targets, manifests and active documentation use the
`emulator` vocabulary. No compatibility aliases or second implementation were
retained.

Implementation P1 is `e6001412`. Ninja and Makefiles package builds passed on
x64 and x86. Ninja background CTest passed 129/129 on each width, excluding
the explicitly separate native desktop cases. Corpus, manifest, dependency,
boundary and documentation gates passed; the two package EXEs were refreshed.

The following owner-directed admission of a new S records completion of this
delivered S19 and leaves T85 open.
