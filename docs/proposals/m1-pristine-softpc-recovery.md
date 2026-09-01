# M1 Pristine SoftPC Recovery And Divergence Ledger

## Objective

Make `core/softpc-pristine` a byte-accountable recovered-machine baseline and
classify every current divergence from the selected original source.

## Scope

Inventory CCPU, C-VID, BIOS, controllers, ROM/BOP, and `nt_*` renderer files;
restore standalone scheduler/BOP/renderer branches from original source where
possible; record each remaining x86/x64 portability delta and its owner.

## Acceptance

The ledger gives every changed original file one disposition: restored,
port-ABI overlay, compatibility-host implementation, or explicitly blocked.
No standalone policy branch remains in a file classified pristine.

## Stop Condition

Stop if an original source path or its intended host contract cannot be
identified without guessing.
