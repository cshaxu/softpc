# M9 T54 — Console Mouse Scale Correction

T53 incorrectly imposed equal numeric X/Y Console input units.  This corrective
task restores the prior physical character-cell conversion: eight horizontal
and sixteen vertical mickeys.  It also removes the application queue's
cross-source mouse coalescing: original InPort acceleration is packet-sized,
so the shared queue must preserve individual relative records.  `ui-window`'s
private native-message coalescing remains unchanged.  No host, recovered
machine, Console ownership, or user configuration changes.
