# M1 T2 S9 SAS Vector Profile Selection Evidence

`base/ccpu386/sascdef.c` is restored to the selected OpenNT SHA-256
`B8B7832D02BD8F4161E1F71DF2968051609DA065F4DA8287A65E6B14B38A749C`.

It is not selected by the current CPU40/C-VID link.  That link correctly
selects the original `base/cvidc/sascdef.c`.  Adding the CCPU vector source
mixes two original SAS vector implementations and introduces unresolved
`c_sas_touch` and `c_VirtualiseInstruction` services.  Therefore no standalone
stub or replacement is added, and the recovered CCPU source remains unbuilt
for this fixed profile.

x64 rebuilt and `softpc-bop-smoke` passed after removing the incorrect source
selection.  M2 owns any future profile-selection change.
