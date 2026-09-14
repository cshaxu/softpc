# M9 T59 S4 VM admission

Executor delivery 7fbca92 pushed after retrying a GitHub HTTP 408; remote
previously still pointed to 4f3b4af, so the first failed attempt was not counted.
Coordinator reviewed acquire/failure/destroy paths and test assertions: one
atomic resource flag, no additional lifecycle state or Common change. Null
cleanup cannot release a live owner; failure and disposal permit recreation.
Production 3 C/H files +23/-8; one test +25. Full x64/x86 86/86 and fixed EXEs.
The [proposal](../history/M9-T59-product-boundary-cleanup-proposal.md) retains scope and evidence.
S4 closes; S5 proceeds. T59 remains open.
