# M9 T62 S2: embedded Machine input queue

Original request: [serial proposal](M9-T62-common-lib-simplification-proposal.md).
Baseline c7fafb8; implementation db3b6d8 pushed. Owner then required T61 closure
first; 6741721 completed that closure before this S2 acceptance/next admission.

Actual review: Machine embeds queue storage, initializes its single mutex and
disposes it through normal/partial-create cleanup. No public API, capacity,
clear, dispatch, wake or ordering change. All old private create/destroy uses
were removed across source and tests, without retaining allocation wrappers.
Separate frame buffers and Session FIFO keep distinct purposes and ownership.

Production three paths +24/-37 = -13; test C/H five paths +47/-13 = +34;
test CMake +1/-0. New proof covers failed mutex creation, disposal, reinitialize
and 600 push/pop cycles; existing capacity/mouse FIFO/clear/lifecycle tests remain.
Both builds and fixed EXEs refreshed. Full x64 100/100 (89.81s), x86 100/100
(75.08s); actual-commit queue/sync/machine/runtime tests 6/6 each (1.14s/1.12s).
Manifests, corpus/DAG and governance pass; no INI/media or other production changes.

S2 closed after actual-diff review. Automatically admit S3; T62 stays open.
