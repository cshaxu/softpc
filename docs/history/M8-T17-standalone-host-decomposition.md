# M8 T17: Standalone-Host Decomposition

## Status

Active.

## Task Brief

Move only repository-owned standalone host code out of the transitional
`src/core/softpc_*` aggregation and organize it beneath
`src/host/{platform,media,video,input,compat,machine}`. The recovered
`src/mvdm/softpc.new/` tree remains source-shaped and is not a source of new
host implementation. The task changes paths and build routing, not controller,
firmware, ROM, BOP, CCPU/C-VID, guest-media, or presentation semantics.

## Entry Evidence

T16 completed with a 98-row current-path ledger, zero `local-standalone` rows
under the recovered machine root, and GCC x64/x86 CTest 20/20. The next audit
will assign every standalone host source one taxonomy owner before it moves.
