# M9 T41: NXVM library adoption and binding closure

## Owner-directed closure

The owner closed T41 after its NXVM `src/lib` import, non-MVDM ownership audit,
and binding baseline were completed. The selected 52-file corpus was recorded
at `c7b5e668b7d9fc8e8710c3dd77c863ce07801553` with manifest hash
`C22012E985A30E130369AAC8A5E0C0E93DA9F56C842843A4C5B3685769E3E9A6`.

T41 established the imported `base`, `host`, `storage`, and `ux` baseline and
the non-MVDM migration ledger. The incomplete Console/Window product behavior
is deliberately not declared accepted here: it is transferred intact to the
separately admitted [T42 proposal](m9-t42-console-object-ux-recomposition.md).

## Boundary preserved

`src/mvdm/softpc.new/` remains unchanged. The user-owned
`assets/binary/softpc.ini` and guest media were not changed by this closure.
T42 may alter only the generic candidate library and non-MVDM SoftPC binding,
subject to NXVM's exact adoption and SoftPC's exact re-import.
