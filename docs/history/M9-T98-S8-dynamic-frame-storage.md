# M9 T98 S8 - Dynamic frame storage

After verified delivery, the owner explicitly authorizes advancing to S9 on
2026-10-21. This closes S8 administratively; no additional owner-run test is
inferred. Executor f2677d01 and actual-change review 34e53055 are pushed.
T98 remains open.

Four-slot ownership is retained with actual-size type-specific lazy payloads.
Production +431/-190 (net +241); tests +767/-564 (net +203). The included
original host text stride/preflight change is +12/-8; public X86 trees and
root test infrastructure are unchanged. Both EXEs are published.

Final x64 and x86 each pass all 192 registrations, zero skips: 184 background,
five desktop and three native audio. The proposal preserves exact verification,
allocation accounting, package hashes and review. Earlier initialization exits
remain TODO; the measured initial-prompt fixture budget correction is not a
claim to repair all historical startup failures. Scratch is retired; caches,
user INI/media and untracked snapshot.debug are preserved.
