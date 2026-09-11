# M9 T54 — Console Mouse Scale Correction

T53 incorrectly imposed equal numeric X/Y Console input units.  This corrective
task restores the prior physical character-cell conversion: eight horizontal
and sixteen vertical surface pixels.  It also removes the application queue's
cross-source mouse coalescing: original InPort acceleration is packet-sized,
so the shared queue must preserve individual relative records.  `ui-window`'s
private native-message coalescing remains unchanged.  SoftPC's guest-input
adapter first converts copied surface-pixel movement to the selected InPort
mickey ratio (one horizontal mickey and two vertical mickeys per surface
pixel), then splits each relative motion into InPort packets no larger than
four mickeys per axis. This preserves the converted total while avoiding the
original driver's packet-level double-speed threshold. No host, recovered machine,
Console ownership, or user configuration changes.
