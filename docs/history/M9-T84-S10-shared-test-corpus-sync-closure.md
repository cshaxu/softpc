# M9 T84 S10 — shared Lib/Common test-corpus sync closure

The owner requested exact synchronization of SoftPC `test/lib` and
`test/common` from read-only NXVM commit
`0c71110b0714fffee3ef8c40bb352ee3dd71ee40`, including deletions.  P1
`bbdc1d68` is the complete delivery.

The finite audit began with eight differences across 72 SoftPC and 71 reference
paths.  It deletes the 127-line Common-private Window reset smoke and its
registration, preserving its native loss coverage in the Lib-owned input
admission smoke.  It also imports the Lib worker-retirement reset observation,
Common's pre-reset ledger assertion and a permanent negative rule preventing
Common fixtures from importing Lib implementations.  Both manifests are the
reference manifests.  The post-import ledger has 71 reference paths, 71 local
paths and zero Git-object-hash mismatches.

Production C/H is `+0/-0`.  Test C/H is `+62/-128` (net `-66`); including
manifest and CMake registration changes, test/build material is `+69/-137`
(net `-68`).  There is no API, App, VM, Compat, Core, configuration, media,
snapshot or package-EXE change.

Both x64 and x86 reconfigured and compiled the affected targets.  Their focused
Common physical-identity, Lib input-admission and Lib Window-retirement tests
pass.  Serial hidden-background CTest passes x64 121/121 (127.11 s) and x86
121/121 (123.92 s).  Test manifests, the Lib component DAG, documentation
governance and whitespace checks pass.  Background runs exclude desktop-labelled
tests; the focused Window-retirement test ran separately once per width.
