# M9 T84 S6 — product host-boundary failure closure

The owner accepted S6 on 2026-09-25. Executor `13b4aef3` is pushed. It
closed the post-S5 product-boundary audit without changing the shared
Lib/Common/x86 corpus, the preserved Core mirror, user INI, snapshots or guest
media.

The implementation made partial parallel-port CRT writes retain only their
unconfirmed suffix, validated a complete parallel snapshot before allocating or
replacing live port state, and propagated audio teardown failure through the
existing VM/App lifetime boundary. It also made INI comment markers lexical
(outside quoted values only) and removed the obsolete permanent VM trace path.
No retry worker, second queue, parser, public shared ABI or mirror behavior
was introduced.

Actual text change was application production +80/-154 (net -74),
build/checks +14/-3 (net +11), tests +140/-7 (net +133), and documentation
+106/-16 (net +90): total +340/-180 (net +160). Both packages were refreshed.
Focused injected cases, dual-width Release packages and hidden-background
CTest passed 121/121 on x64 and x86. Documentation governance, manifests,
boundary checks and whitespace checks passed; desktop-labelled tests remained
excluded.
