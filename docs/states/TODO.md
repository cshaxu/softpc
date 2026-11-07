# Long-Term Review Ledger

- P2: Legacy CHAIN2 Copy odd-address dword-fill value/tail assembly is incorrect
  in the retained generated C-VID entry. S33's direct rule probe exposed missing
  bytes even after address spacing was corrected. Current CCPU SAS dword fills
  loop through scalar stores and do not call this entry. Admit a focused
  original-rule repair before enabling a bulk/dword-fill CPU consumer; do not
  treat the continuation replay test as a semantic oracle. See
  [S33 investigation](../etc/evidence/softpc/m9-t99-s33-windowed-dos-addressing.md).

CURRENT owns active tasks; the runtime-performance candidate remains in Queue.
