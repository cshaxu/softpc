# M9 T98 S9 - 64-bit execution budget

Owner accepts closure and advancement to S10 on 2026-10-21. Executor dbff43df
and actual-change review d987f10f are pushed. T98 remains open.

Private execution budget is lib_u64 on both architectures; zero is continuous,
all nonzero values finite. Existing active state and counting path are retained.
Production +14/-20 (net -6); tests +62/-4 (net +58). No public or mirror change.

Cached dual builds and focused proof pass. Each architecture has 191/192
first-pass full-regression successes; unchanged isolated compact-package
supplements pass, covering all 192 registrations without skips. This does
not repair or conceal the recorded package intermittency. Existing mirror
header warnings remain. Both EXEs are published; hashes and full evidence
are retained in the proposal. Scratch is retired; user INI/media and
untracked snapshot.debug are preserved.
