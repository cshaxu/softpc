# Queue

This is the ordered, unnumbered candidate queue. It neither activates work nor
allocates a numeric task identifier.

1. [M9 warm-reset lifecycle recovery](../proposals/m9-warm-reset-lifecycle.md)
   — Ctrl+Alt+D reaches guest reset, but the warm-boot path hangs after
   `Starting MS-DOS ...`; establish the original hardware/lifecycle cause and
   repair only the standalone boundary that violates it.
