# M9 Td S6: Console-object governance closure

## Outcome

Promoted the owner-approved S5 Console-object/UX design to current authority:

- [Product UX](../design/UI.md) owns display, presenter, monitor, pause/resume,
  Window X, and hotkey behavior.
- [System Architecture](../design/ARCHITECTURE.md) owns `base`/`host`/`ux`/
  SoftPC boundaries, Current Console Object, threads, reconciler, and
  generation boundaries.
- [S5 review record](../etc/evidence/softpc/m9-t41-s5-console-design-review-record-zh.md)
  retains decision excerpts/cross-audit only.

The owner changed delivery order: SoftPC implements the generic candidate in
`src/lib`, NXVM adopts it exactly, then SoftPC re-imports it before T41 closure.
This is not a permanent SoftPC library fork.

## Frozen invariants

1. `src/mvdm/softpc.new/` remains unchanged.
2. SoftPC control is sole product-state writer/reconciler.
3. Host has exactly one Current Console Object; replacement is transactional.
4. UX owns presenters and optional raw object, never process-Console
   registration or SoftPC policy.
5. Monitor is cooked and has no SoftPC hotkeys.
6. VM run generation is SoftPC-only; UX configuration generation is separate.
7. Concurrency proof uses completion gates/barriers, never sleeps.

## Verification

Documentation governance and diff checks passed. No code/package artifact is
changed by this Td; T41 remains active for implementation.
