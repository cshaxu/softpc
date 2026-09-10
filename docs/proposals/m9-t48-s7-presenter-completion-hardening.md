# M9 T48 S7 — Presenter completion hardening

Make component and broker completion handling fail-closed and one-way.  Cover
Window close, component retirement, stale completion facts, broker replacement,
and no late UX input after retirement.  No callback may invoke a product API;
all facts re-enter the single control queue.
