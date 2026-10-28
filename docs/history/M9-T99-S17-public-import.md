# M9 T99 S17: NXVM Public Eight-Tree Import

The owner reports functional acceptance and closes S17. Executor `7a262fb1`
and actual-change review `d6ea5352` were pushed. The 296 shared paths match
NXVM commit `346094bf2b166a547b0a38abe1c98b8956e77fd6`. A subsequent
read-only simplification review found no justified reduction that preserves
the accepted window-sizing semantics.

Both EXEs were rebuilt and published. Each architecture has passing evidence
for all 193 registered tests, combining initial and supplemental runs; the
initial compact Console failures and their limits remain recorded in the
[S17 qualification](../proposals/m9-public-handoff-simplification.md#s17-import-qualification-and-self-review).
The owner reports functional acceptance; no independent native snap-message
trace is claimed. Scratch was retired; user INI and media were preserved.
This closes S17 only; T99 remains open.
