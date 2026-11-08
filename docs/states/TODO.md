# Long-Term Review Ledger

- P2: Legacy CHAIN2 Copy odd-address dword-fill value/tail assembly is incorrect
  in the retained generated C-VID entry. S33's direct rule probe exposed missing
  bytes even after address spacing was corrected. Current CCPU SAS dword fills
  loop through scalar stores and do not call this entry. Admit a focused
  original-rule repair before enabling a bulk/dword-fill CPU consumer; do not
  treat the continuation replay test as a semantic oracle. See
  [S33 investigation](../etc/evidence/softpc/m9-t99-s33-windowed-dos-addressing.md).

- P2: Compat C-VID GDP byte offsets 1379/1383 currently get independent slots,
  rather than aliasing the colour_comp/dont_care fields at 1376/1380. Setting
  the public whole-field masks does not reach CHAIN2 mode-1 byte reads. S36's
  isolated mask variation returned unchanged bytes after both all-zero and
  all-one compare settings; source mapping confirms the alias gap. Owner:
  app-softpc/compat/cvidc. Admit a focused GDP subfield-alias audit and real-rule
  regression before claiming non-default read-mode-1 mask correctness. Keep
  this separate from the DWORD composition repair and do not add read-side
  compensation.

- P2: S_2097_RdMode0UnchainedDwordRead passes guest offset +8 to its second
  WORD reader, whose body itself scales the guest address by four. Source
  review in S36 indicates the same guest/plane-unit confusion; no dynamic
  Unchained oracle has yet been run. Owner: app-softpc mirror. Admit a focused
  Unchained compound-read audit before routing CCPU directly through this
  legacy DWORD entry. Current phy_r32 instead performs two WORD reads at +2.

- P2: CCPU BIOS WORD/DWORD mapping-failure reads in ccpusas4.c cast biosDoInst's result to IU8 in both protected-mode and V86
  fallbacks. Source inspection during S37 confirms truncation remains separate
  from PG_U admission. Owner: app-softpc mirror. Admit a focused fallback-width
  repair with actual nested guest-handler results before claiming full-width
  fallback correctness; the S37 permission fixture substitutes that handler
  and only proves permission/fallback selection.

CURRENT owns active tasks; the runtime-performance candidate remains in Queue.
