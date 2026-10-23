# Long-Term Review Ledger

On 2026-10-22 the owner transfers all eight previous entries
into T99's work; after the owner inserts the public import
as S9, their receivers are S6-S8 and S10-S12. None is declared fixed by
this transfer. The [audited disposition and S plan](../proposals/m9-public-handoff-simplification.md#todo-audit-and-owner-admitted-t99-expansion)
preserves current evidence, historical references and individual acceptance
criteria. CURRENT remains the sole active-subtask authority.

- [ ] TODO(Medium): Audit retained original VGA cursor end-row and wrap semantics.
  S10 research finds vga_prts.c uses End-Start and a split when End<Start,
  whereas IBM VGA/V7 references specify inclusive end and no wrap in VGA mode.
  Current vga_frame_smoke explicitly expects the legacy values. Owner: original
  VGA hardware logic and App-local test oracle, not shared Lib presentation.
  Admission: owner-approved hardware-mode research and focused port/frame
  reproduction, preserving genuine EGA behavior and Lib two-span capability;
  do not fold a shape-policy rewrite into S10's bit-5 repair. Evidence and
  references: [S10 research](../proposals/m9-public-handoff-simplification.md#s10-research-two-site-cursor-disable-repair).
